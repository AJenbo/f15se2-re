# Porting Notes for F15-SE2 Reverse Engineering

## Compiler Flags and Source Files

### end.exe
- `end0.c` — `/Gs /Zi` (stack probes disabled, debug info)
- `end1.c` — `/Gs` (stack probes disabled, no debug info, default optimization)
- `end2.c` — `/Od` (optimizations disabled)

### Observations on flag selection
- `/Gs /Zi` generates verbose instructions: `mov word [bp-x], 0x0` for zeroing
- `/Gs` (no `/Zi`) generates optimized: `sub ax, ax` then `mov [bp-x], ax` for zeroing multiple vars
- `/Gs` also optimizes away loop condition checks when the result is provable at compile time (e.g. `running = 1; do { if (running == 0) return; ...}` skips the first check)
- `/Od` generates `mov ax, SIZE; ... sub sp, ax` instead of `sub sp, SIZE` for large stack frames, and frame sizes differ slightly

### How to determine which file a routine belongs in
1. Look for `sub ax,ax` used to zero multiple locals → `/Gs` (end1.c)
2. Look for `mov word [bp-x], 0` for each zero init → `/Gs /Zi` (end0.c)
3. Look for `mov ax, N; sub sp, ax` frame allocation → `/Od` (end2.c)
4. Check if first-iteration loop conditions are present or optimized away → `/Gs` optimizes them out

## OMF Variable Name Hashing

MSC 5.1 uses a **hash table** to assign local variable names to stack frame slots (BP-relative offsets). The allocation order depends entirely on the variable name, not declaration order.

### Hash function

```
hash(name) = sum(ord(c.upper()) for c in name) % 16
```

- The compiler maintains a **16-bucket hash table** for local variable symbols
- **Collision resolution**: backward linear probing (on collision, probe to bucket−1)
- **Stack frame allocation order**: iterate buckets 0→15, allocating 2-byte slots for each variable in bucket order (bucket 0 → BP-2, bucket 1 → BP-4, etc.)
- **Declaration order does NOT affect allocation** — only the name determines the hash bucket
- **Arrays** participate in the hash table and are placed in hash order among scalars
- **All scalar types** (char, int, pointer) get 2-byte slots regardless of declared type
- **Chained assignments** like `a = b = 0` store the INNER (rightmost) target first, then the outer
- The compiler may generate **temporary variables** at the bottom of the frame for expression evaluation

### Working approach

Proven workflow for matching variable allocation:

1. Use `tools/msc_hash.py` to compute hash buckets for candidate variable names
2. Choose names whose hash buckets produce the desired allocation order (ascending bucket order = BP-2, BP-4, BP-6, ...)
3. Single-letter names are convenient: `a`→bucket 1, `b`→2, `c`→3, ..., `o`→15, `p`→0
4. Verify chained assignment order matches the original (inner assignment stored first)

### Example: routine_130

Solved using single-letter variable names that hash to ascending buckets 0–7, with `buf` at bucket 13:
- `p`→bucket 0 (BP-2), `a`→1 (BP-4), `b`→2 (BP-6), `c`→3 (BP-8), `d`→4 (BP-10), `e`→5 (BP-12), `f`→6 (BP-14), `g`→7 (BP-16), `buf`→13 (BP-18)
- This produces a consistent 1:1 mapping with the original binary's BP offsets

## Porting Workflow

### Successfully ported functions (duplicates from start.exe)
1. Check `map/end.map` for functions without `complete` annotation
2. Find matching function in `start1.c` or `start2.c`
3. Copy the C code to the appropriate `end*.c` file
4. In `end_rc.asm`:
   - Add `EXTRN _functionName:PROC`
   - Replace the `proc`/`endp` block with `functionName equ _functionName`
   - Remove any `PUBLIC _functionName` if present (since it's now extern)
5. Add declaration to `end.h`
6. Build and verify: `make end && make verify-end`
7. Update `map/end.map` with `complete` annotation

### Porting new functions from Ghidra decompilation
1. Export decompilation from Ghidra (see `end.exe.c`)
2. Identify `thunk_EXT_FUN_0000_0000` calls — these are overlay jump table entries (gfx_jump_*, misc_jump_*, etc.)
3. Map Ghidra's `DAT_XXXX_YYYY` references to known variables from the map
4. Write C code matching the logic
5. Key challenges:
   - Choosing the right compiler flags file
   - Matching the `do/while` vs `while` vs `for` loop structure
   - Using chained assignments to trigger `sub ax,ax` optimization
   - Variable name hashing (see above)

## Assembly Functions to Keep in ASM

Some functions should stay in assembly because:
- They use forced instruction encodings (`db 3Dh, 0Ah, 00h` for word-immediate cmp)
- They use segment override instructions (`lodsb`/`stosb` with `lds`/`les`)
- They are interrupt handlers or use specific register conventions
- They are C runtime helpers (`__aNlmul`, `__aNldiv`, `__aNlrem`, `__aNNaldiv`)

## Ghidra Setup

### Scripts (in `tools/`)
- `ImportMap.java` — imports routine/variable names from .map files, applies types from headers
- `FixMsc16BitRuntimeSignatures.java` — identifies and types MSC runtime helpers
- `AnnotateDosInterrupts.java` — annotates INT instructions with DOS function descriptions

### Workflow
1. Load the unpacked binary in Ghidra (16-bit x86, MZ format)
2. Run auto-analysis
3. File > Parse C Source → import `src/end.h`
4. Run `ImportMap.java` → select `map/end.map` then `src/end.h`
5. Export decompilation for reference

### Memory layout (end.exe in Ghidra)
```
CODE_0 @ 1000:0000 - 1000:570f  (main code segment)
CODE_1 @ 1571:0000 - 1571:00ef  (second code segment)
CODE_2 @ 1580:0000 - 1580:3fc5  (third code/data)
DATA   @ 1580:3fc6 - 1580:64d5  (initialized data)
4000   @ 4000:0000 - 4000:ffff  (uninitialized/runtime data)
5000   @ 5000:0000 - 5000:ffff  (uninitialized/runtime data)
```

The map file's segment offsets are byte offsets from the start of the load module (first CODE block at 1000:0000). Variables with large offsets into Data1 may land in the 4000/5000 blocks in Ghidra's view.
