# End.exe Runtime Debugging

## Overview

The end.exe reconstruction builds and verifies byte-exact against the original (97% practical / 99% theoretical coverage via `make verify-end`). The code segment matches; remaining issues are in the data segment (which is not covered by verification) and runtime behavior.

## What Works (First Run)

- ✅ Build succeeds (`make` — warnings are forward-reference text macros only)
- ✅ Byte verification passes (`make verify-end`)
- ✅ Text renders ("MISSION DEBRIEFING", "Review Mission", "Exit Debriefing")
- ✅ Stats panel renders correctly (OVERALL, CUMULATIVE, MISSION RATING, etc.)
- ✅ File open/close works (libya.spr, dbicons.spr open successfully)
- ✅ Keyboard navigation works (arrow keys, Enter detected)
- ✅ Menu color cycling animation works
- ✅ Sprite icons render correctly
- ✅ Map background (libya.spr) renders
- ✅ White projector canvas background renders
- ✅ "Review Mission" and "Exit Debriefing" respond correctly
- ✅ Flight replay animation works (with empty test data, just draws origin point)
- ✅ Stats/awards display (routine_27) executes
- ✅ Exit works on first run (returns to F15 → start → end cycle)

## Open Issues (Second Run)

When the game cycles back to end.exe a second time (end → F15 → start → end), the following issues appear:

### 1. White Projector Canvas Background Is Black

**Symptom**: The white background with projector canvas effect that should appear behind the map and stats is blacked out on the second run. The map sprite (libya.spr) still renders correctly on top.

**What we know**:
- First run renders the background correctly
- The background comes from within end.exe's initialization (not start.exe or F15.exe)
- Map sprite renders fine, so the sprite buffer/page struct system works

### 2. Menu Items at Wrong Y Position

**Symptom**: "Review Mission" and "Exit Debriefing" text appears at the top of the screen instead of the bottom. The X position is correct but Y is wrong.

**What we know**:
- First run positions them correctly
- The menu items can still be selected (keyboard input works)
- The item structs at `dat_21e4` have correct Y coordinates in the binary

### 3. No Visible Color Cycling (Second Run)

**Symptom**: On second run, the selected menu item doesn't show the color cycling animation that indicates it's highlighted.

**What we know**:
- Color cycling works on first run
- The timer IRQ handler fires correctly (counters increment)
- `_var_172` and `_var_82` thresholds are met
- The `gfx_jump_29_switchColor` overlay call is made

### 4. Exit Causes CLI Garbage

**Symptom**: When selecting "Exit Debriefing" on the second run, the screen fills with garbage characters scrolling in text mode (low-resolution CLI).

**What we know**:
- Exit path: `routine_25` returns → `routine_26` → `routine_27` → `routine_28` → `routine_8(0x23)`
- `cleanup()` resets video to mode 3 (text mode) before exit
- First run exit works

### Root Cause Theory

All four issues appear only on the second run, suggesting that end.exe's exit path doesn't properly restore state that the graphics overlay (Mgraphic.exe) depends on. The overlay persists in memory between end.exe invocations. The exit path itself is the most likely culprit. Possible causes:
- A page struct or buffer is modified during the first run and the overlay caches stale values
- The overlay's internal page/buffer table gets corrupted by the exit sequence
- A display page isn't being switched back correctly before exit

## Testing Setup

### Directory structure
```
test/           — Test environment with all game files
test/END.EXE   — Our reconstructed end.exe (copy from build or debug_build)
test/F15.EXE   — Modified F15 launcher (skips flight, starts menus) — DO NOT MODIFY
test/f15.exe   — Original f15.exe — DO NOT MODIFY  
test/start.exe — Modified start.exe, skips menu navigation — DO NOT MODIFY
test/F15.LOG   — Debug log output (created at runtime)
```

### Building

```bash
make end                   # Release build → build/end.exe
make debug-end          # Debug build → debug_build/end.exe
make verify-end         # Verify code segment matches original
```

### Running tests

```bash
# Copy build to test directory
cp debug_build/end.exe test/END.EXE    # or build/end.exe for release

# Run in DOSBox (interactive — you must close DOSBox manually)
cd test && dosbox -c "mount c ." -c "c:" -c "F15.EXE"

# Check log after running
cat test/F15.LOG
```

### Important rules

- **DO NOT modify** `test/F15.EXE`, `test/f15.exe`, or `test/start.exe` — these are verified working test harnesses
- The test setup: F15.EXE → start.exe → end.exe. Start.exe sets up shared memory and overlays, then exec's end.exe
- To test second-run issues: let end.exe exit normally (select "Exit Debriefing"), which returns to F15 → start → end again
- The release build has no trace overhead; the debug build writes to F15.LOG
- All issues where verified against the original `bin/end.exe` — if the original works with the same test setup, it's our end.exe's bug

## Debug Infrastructure

### Build modes
```bash
make end             # Normal build (no debug traces)
make debug-end      # Build with IFDEF DEBUG traces enabled (writes to F15.LOG)
```

### Key trace points (debug build)
- `routine_25: enter` — main loop body starts
- `routine_25: before routine_59` / `after routine_59` — menu item processing
- `routine_25: before routine_60` — about to wait for input
- `routine_60: enter handler (var_202 set)` — user pressed Enter
- `routine_96: before routine_131` / `after routine_131` — flight animation
- `routine_96: before routine_130` — word-wrap text about to render
- `mainloop` — inside routine_98's input polling loop

### Adding new traces
In ASM (only fires in debug build):
```asm
IFDEF DEBUG
    mov AX,offset _trace_my_message
    push AX
    call _my_trace
    add SP,2
ENDIF
```
Add the string in the data section near other trace strings:
```asm
_trace_my_message db 'my message here', 0
```

Printf-style with parameters (push args BEFORE format string):
```asm
IFDEF DEBUG
    push word ptr [_some_var]
    mov AX,offset _trace_fmt
    push AX
    call _my_trace
    add SP,4
ENDIF
; ...
_trace_fmt db 'value = 0x%04X', 0
```

In C code:
```c
TRACE(("message with %d param", value));
```

## Project Structure

| File | Purpose |
|------|---------|
| `src/end_rc.asm` | Main ASM — data segment and most code |
| `src/end0.c` | C reconstructions (drawString, cleanup, sprites, flight replay) |
| `src/end1.c` | C reconstructions (file I/O wrappers, allocBuffer, input) |
| `src/end2.c` | C reconstructions (misc routines) |
| `src/end.h` | Header for end.exe C modules |
| `map/end.map` | IDA-generated map of original binary |
| `bin/end.exe` | Original unpacked reference (md5: 3b7aac9c52ca3fedefff3a8db54b5799) |

## Key Technical Notes

### Data segment layout difference

The built binary's data segment has a +0x42 byte offset compared to the original:
- Original: DS:0x00 = DOSSEG null area + CRT copyright, DS:0x08 = ASM data starts (embedded in NULL area)
- Built: DS:0x00-0x41 = NULL/BEGDATA segment (0x42 bytes), DS:0x42 = `_DATA` starts (ASM `.DATA` content)

All symbolic references (`offset _var_xxx`) resolve correctly regardless of this shift. The linker handles it. **This is not a bug** — it's a known structural difference that doesn't affect runtime behavior.

### Code verification vs data verification

`make verify-end` only verifies the **code segment** (opcode comparison). The **data segment** is NOT verified. Bugs in data declarations (wrong byte counts, wrong values) will NOT be caught by verification. Always compare data against the original binary when debugging data layout issues.

### Comparing data segments
```python
import struct
with open('bin/end.exe', 'rb') as f:
    orig = f.read()
with open('build/end.exe', 'rb') as f:
    built = f.read()

orig_ds = 1024 + 0x5800      # original data segment file offset
built_ds = 1024 + 0x5D20     # built data segment file offset
shift = 0x42                   # built data is shifted +0x42

# Compare: orig[orig_ds + X] should equal built[built_ds + shift + X]
# Exception: pointer values differ by +0x42 (correctly adjusted by linker)
```

### Past bugs found and fixed

1. **`_var_131` vs `_var_115`** (commit 5b1718c was WRONG, reverted): The instruction `mov word ptr [BX + offset _var_115], 2h` sets the menu animation state. Using `_var_131` (file flags table) instead broke color cycling and corrupted file state. The original binary uses offset `0x2212` which is `_var_115` (= `dat_21e4 + 0x2E`, the state field of the menu item struct).

2. **`_pageStruct2` extra byte**: The declaration had 23 bytes instead of 22 (an extra `000h` before `00Fh`). This shifted every byte after it by 1, corrupting all page struct field offsets. The font field at offset 0x0C read as `0x0300` instead of `0x0003`, causing `gfx_jump_2f_charWidth` to hang in an infinite loop.

3. **`_nmsgTable` hardcoded function pointer**: The CRT termination table `_nmsgTable` had a hardcoded value `094h, 054h` (= 0x5494) copied from the original binary, but this is a CODE segment function pointer that differs between builds. In our build, the target function `FUN_1000_5494` (fcloseall) is at a different offset. Fix: `_nmsgTable dw offset FUN_1000_5494`. Without this fix, exit called garbage code that corrupted the null area → R6001 crash and corrupted overlay state causing all second-run issues.

4. **`_var_55`/`_var_56` wrong initial values**: These joystick dead zone variables were initialized to `0x80` but should be `0x00`. The `0x80` value is only set at runtime when no joystick is detected.

5. **`_var_145_heapDS` / `_var_145_heapCount` sizing**: The CRT heap descriptor area had `_var_145_heapDS` at 4 bytes (should be 3) causing a 1-byte shift in the near-heap size field and `_var_146` (stack limit). Fixed by making `_var_145_heapDS` 3 bytes and `_var_145_heapCount` 3 bytes.

### Key data structures

**Page structs** (`_pageStruct`, `_pageStruct2`, `_ps_101`..`_ps_113`): 22-byte structures used by the graphics overlay. Key fields:
- Word[0] (offset +0): buffer segment (set at runtime by allocBuffer)
- Word[3] (offset +6): max color / palette info
- Word[4] (offset +8): current X position
- Word[5] (offset +0A): current Y position  
- Word[6] (offset +0C): font ID

**Menu item structs** (at `dat_21e4`, 50 bytes each):
- Offset +8/+A/+C/+E: rectangle coordinates (x1, y1, x2, y2)
- Offset +10: item type/subtype
- Offset +12: color pair for cycling
- Offset +2E: animation state (0=idle, 1=transitioning, 2=needs processing)
- Offset +30: flags (bit 0x800=has animation, bit 0x1000=has sprite overlay)
