# End.exe Full-Game Display Issues

## Summary

When end.exe runs after a real egame session (full game with flight data), several display issues appear that do NOT occur when skipping directly from start.exe to end.exe (empty flight data). The code segment is verified byte-exact against the original binary. The initialized data segment has been compared and is correct (only benign CRT IOB differences remain). The issue must be a runtime interaction between real game data and our code/data layout.

## Symptoms

1. **Icons not showing** — Sprite icons on the map (targets, waypoints) don't render
2. **Distorted horizontal lines** — Corrupted pixel rows visible on screen
3. **"CUMULATIVE" and second "MISSION RATING" text missing** — These should appear after the mission event replay
4. **Lockup / crash** — In release builds the program crashes or freezes; in debug builds it stabilizes enough to continue but with display corruption
5. **"Review Mission" causes R6001 crash** — Selecting this menu item fills screen with garbage and prints null pointer assignment error

## What Works

- Everything works perfectly when flight records are empty (skip path from start.exe)
- "OVERALL" and first "MISSION RATING" render correctly
- "MISSION SUMMARY" section (Pri/Sec Targets, Other Targets, etc.) renders correctly  
- "Exit Debriefing" works correctly
- The flight replay animation partially works (records are processed, routine_135 is called)
- Code segment verification passes (97%/99% coverage)

## Root Cause Analysis

### Verified correct
- All initialized data matches the original (modulo expected +0x42 pointer shift)
- Page struct declarations (22-byte and 30-byte sprite descriptors) match original byte-for-byte
- Menu item struct at `dat_21e4` matches original
- Flight record struct layout is correct (6 bytes: time[2], cx[1], cy[1], status[1], pad[1])
- `_var_93`/`_var_94` (map viewport) correctly initialized and never modified
- Buffer `_var_229` is allocated successfully (seg=0x5FC0 in test)
- `buf=0x0000` in page structs is intentional (0 = VGA display page, not a null pointer)
- Overlay jump table is not being corrupted by data writes

### Key Finding: "CUMULATIVE" Section Not Reached

The most concrete finding from log analysis: the code path at `LAB_1000_34eb` (which draws "CUMULATIVE" + "MISSION RATING" on the second stats panel) is never reached. Instead, `routine_96` returns early and `routine_25`'s main loop calls it again, producing a second "OVERALL" display.

**Evidence:** The debug log shows `drawString: '·OVERALL' x=232 y=70` where `drawString: '·CUMULATIVE' x=232 y=70` should appear. Both use y=0x46=70, but the string content proves the `LAB_1000_2cc2` path (OVERALL, from a fresh `routine_96` call) is executing instead of `LAB_1000_34eb` (CUMULATIVE, from within the same call).

### Flow Analysis

```
routine_96 entry:
  SI = flags word at [dat_21e4 + item*50 + 0x30] = 0x1808
  test SI, 0x800  → set, continue
  and SI, 7       → 0 (not 7)
  jmp LAB_1000_2fbf

LAB_1000_2fbf:
  test flags, 0x1000 → set, continue  
  check _var_207 == 1 → yes (initialized to 1)
  → LAB_1000_2fde (second stats phase)

LAB_1000_2fde:
  _var_207 = 0
  blit map sprite (_var_102)
  clearRect
  routine_63 → compute score
  draw "OVERALL" at y=100      ← SEEN IN LOG ✓
  draw "MISSION RATING" at y=108 ← SEEN IN LOG ✓  
  draw score at y=116           ← SEEN IN LOG ✓
  fall through to LAB_1000_30cc

LAB_1000_30cc:
  draw "MISSION EVENT" at y=20  ← SEEN IN LOG ✓
  draw event text               ← SEEN IN LOG ✓
  process event via switch
  call routine_130 (word-wrap text renderer)
  check record status for PRIMARY/SECONDARY OBJECTIVE flags
  → LAB_1000_34eb

LAB_1000_34eb:                  ← NOT REACHED (or reached but corrupted)
  routine_63 → compute cumulative score  
  draw "CUMULATIVE" at y=70     ← NOT IN LOG ✗
  draw "MISSION RATING" at y=78 ← NOT IN LOG ✗
  draw score
  call routine_131 (flight animation)
  call routine_130 (press next)
  → LAB_1000_35db (exit routine_96)
```

Something between `LAB_1000_30cc` and `LAB_1000_34eb` causes `routine_96` to exit early or execution goes off the rails.

### Theories

1. **Stack corruption from `routine_130`**: `routine_130` allocates 0x3FA bytes on the stack (`sub SP, 3FAh`). In a SMALL model with 2048-byte stack, this is ~1KB. Combined with `routine_96`'s own stack frame (0x22 bytes) and `routine_98`'s frame, this could overflow the stack, corrupting the return address and causing a wild jump.

2. **Event switch jump table executing wrong case**: The switch at `switchD_317d` uses a CS-relative jump table (`jmp word ptr CS:[BX + 3442h]`). While code is verified, if AX (the record type) has an unexpected value > 11, the `ja caseD_4_343d` check should catch it. But if the record type byte is corrupt...

3. **BSS overlap**: Our BSS is ~14KB vs original's ~9.5KB. While each variable is accessed by its own label (so offsets are correct), the extra BSS pushes the stack segment further. This could affect the overlay's assumptions about memory layout.

4. **Overlay state**: The graphics overlay (Mgraphic.exe) persists in memory across end.exe invocations. If the overlay caches pointers to end.exe's data segment, and our data segment is at a different offset than the original's, overlay functions could read/write wrong memory.

## Recommended Next Steps

### 1. Add Targeted ASM Traces

Add traces at critical points that are only in ASM (not in C functions we can already trace). This requires restoring minimal debug infrastructure:

```asm
; At LAB_1000_34eb entry (CUMULATIVE section):
IFDEF DEBUG
    mov AX, offset _trace_cumulative_enter
    push AX
    call _my_trace
    add SP, 2
ENDIF

; At LAB_1000_35db (routine_96 exit):
IFDEF DEBUG
    mov AX, offset _trace_r96_exit
    push AX
    call _my_trace
    add SP, 2
ENDIF

; At caseD_4_343d (after routine_130 returns, before status check):
IFDEF DEBUG
    push word ptr [_var_190]
    mov AX, offset _trace_after_r130
    push AX
    call _my_trace
    add SP, 4
ENDIF
```

This will confirm whether:
- `routine_96` exits at `LAB_1000_35db` before reaching `LAB_1000_34eb`
- The event switch at `LAB_1000_3432` processes the record correctly
- `routine_130` returns normally

### 2. Check Stack Usage

Add a stack depth check at `routine_130` entry:
```asm
IFDEF DEBUG
    mov AX, SP
    push AX
    mov AX, offset _trace_r130_sp
    push AX
    call _my_trace
    add SP, 4
ENDIF
```

The stack top is at `__stack_top` (BSS end + 0x800). If SP is below the BSS end when entering `routine_130`, the 0x3FA-byte allocation will corrupt BSS/data. Original stack size is 0x800 (2048 bytes). Check if nested calls (routine_25 → routine_98 → routine_96 → event section → routine_130) consume too much.

### 3. Compare BSS Layout

Our BSS is ~4.7KB larger than the original (14206 vs 9488 bytes). While variable access uses symbolic offsets (so code is correct), the larger BSS shifts the stack segment. Verify:
- Do any overlay functions assume a specific DS:SS relationship?
- Does the original end.exe's stack segment overlap with the overlay's memory?

Tools:
```python
# Check stack position relative to overlay
# From debug log: overlay at 0x164E, gfx buffer at 0x18CC
# Our SS = 0x0BBD, stack top = SS:0800 = linear 0xC3D0
# Original SS = 0x0B4D, stack top = linear 0xBCD0
```

### 4. Interactive Debugging in DOSBox

Use DOSBox debugger (`dosbox-debug` or `dosbox-x` with debugger) to set breakpoints:
```
# Break at LAB_1000_34eb equivalent in our build
# Use the linker map to find the actual address
bp CS:XXXX    # LAB_1000_34eb
bp CS:YYYY    # LAB_1000_35db (routine_96 exit)

# When hit, examine:
# - SP value (stack depth)
# - DS:[_var_190] (current record index)
# - DS:[_var_196 + _var_190*6] (current record status)
# - Return address on stack (is it sane?)
```

### 5. Verify "Review Mission" Path

The R6001 crash on "Review Mission" (second menu item) is a separate but possibly related issue. The menu handler calls `routine_96` with item index 1, which has flags 0x080F (bit 0x800 set but 0x1000 not set). This enters the `LAB_1000_2c1d` path (SI & 7 == 7 check). With flags & 7 = 7, it enters the FULL stats display including `routine_132` (flight replay). If the flight replay corrupts memory, subsequent operations crash.

## Test Setup

See [end_exe_runtime.md](end_exe_runtime.md) for build/test instructions.

Quick reference:
```bash
make end                    # Release build → build/end.exe
make debug-end             # Debug build → debug_build/end.exe  
make verify-end            # Verify code segment matches original
cp debug_build/end.exe test/END.EXE  # Deploy for testing
cd test && dosbox -c "mount c ." -c "c:" -c "F15.EXE"
cat test/F15.LOG           # Check debug output
```

To reproduce: run through the full game (F15.EXE → start.exe → egame → end.exe). The egame session provides real flight records. A Libya training mission is sufficient.

## File References

| File | Purpose |
|------|---------|
| `src/end_rc.asm` | Main ASM — data segment and most code |
| `src/end0.c` | C reconstructions (drawString, cleanup, sprites, flight replay) |
| `src/end1.c` | C reconstructions (file I/O, allocBuffer, input) |
| `src/end2.c` | C reconstructions (misc routines) |
| `src/end.h` | Header for end.exe C modules |
| `src/debug.h` | TRACE macro definition (active in debug builds) |
| `src/debug.c` | Debug logging implementation (my_trace, log file) |
| `map/end.map` | IDA-generated map of original binary |
| `bin/end.exe` | Original binary reference |
| `build/END.MAP` | Linker map for our build (symbol addresses) |
| `test/F15.LOG` | Debug log from last test run |
| `docs/end_exe_runtime.md` | General runtime debugging doc |

## Key Code Locations (routine_96)

| Label | Original offset | Purpose |
|-------|----------------|---------|
| `routine_96` entry | 0x2C04 | Stats/animation display |
| `LAB_1000_2c1d` | 0x2C1D | Full stats path (SI & 7 == 7) |
| `LAB_1000_2cc2` | 0x2CC2 | First stats: OVERALL, MISSION RATING, routine_132 flight replay |
| `LAB_1000_2fbf` | 0x2FBF | Second phase gate (checks flag 0x1000 + _var_207) |
| `LAB_1000_2fde` | 0x2FDE | Second phase: redraw OVERALL, then event-by-event |
| `LAB_1000_30cc` | 0x30CC | Event display section (MISSION EVENT) |
| `switchD_317d` | 0x317D | Event type switch (12 cases) |
| `caseD_4_343d` | 0x343D | Default case: call routine_130, check objectives |
| `LAB_1000_34b1` | 0x34B1 | Check secondary objective flag |
| `LAB_1000_34eb` | 0x34EB | CUMULATIVE section (never reached in test) |
| `LAB_1000_35db` | 0x35DB | routine_96 exit |

## Previously Fixed Issues

These were found and fixed during this investigation:

1. **`_nmsgTable` hardcoded function pointer** — CRT exit called garbage code → R6001 + overlay corruption
2. **`_var_55`/`_var_56` wrong initial values** — 0x80 instead of 0x00
3. **`_var_145_heapDS`/`_var_145_heapCount` sizing** — 1-byte misalignment in CRT heap descriptor
4. **`debug.h` not included in `end.h`** — TRACE macros were inactive in end.exe C code
5. **Hardcoded switch jump tables** — `switchD_317d`, `switchD_27f7`, `switchD_4225` all used hardcoded CS offsets (`3442h`, `2a4fh`, `42e2h`) for table base and hardcoded target addresses in table entries. These only work if the code segment layout exactly matches the original binary. Fixed to use symbolic `offset var_N` and `offset caseD_X` references. This was the root cause of the CUMULATIVE text not showing and other display corruption — case 8 of the event switch would jump to wrong code.
