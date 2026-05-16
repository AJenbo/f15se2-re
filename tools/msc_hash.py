#!/usr/bin/env python3
"""
MSC 5.1 local variable name hash tool.

MSC 5.1 uses a 16-bucket hash table for local variable symbols.
  hash(name) = sum(ord(c.upper()) for c in name) % 16

Collision resolution: backward linear probing (probe to bucket-1 on collision).
Allocation order: iterate buckets 0..15, allocating stack slots (bp-2, bp-4, ...).

Usage:
  python3 msc_hash.py <name>           — compute hash for a single name
  python3 msc_hash.py <name1> <name2> ... — show allocation order for given names
"""

import sys
from itertools import product
import string

def msc_hash(name):
    """Compute MSC 5.1 local variable name hash."""
    return sum(ord(c.upper()) for c in name) % 16

def allocate_vars(names):
    """Simulate MSC 5.1 hash table insertion and return allocation order.
    
    Returns list of (name, bp_offset) tuples in allocation order.
    """
    TABLE_SIZE = 16
    table = [None] * TABLE_SIZE
    
    # Insert each variable in declaration order
    for name in names:
        bucket = msc_hash(name)
        # Find empty slot with backward probing
        for probe in range(TABLE_SIZE):
            idx = (bucket - probe) % TABLE_SIZE
            if table[idx] is None:
                table[idx] = name
                break
        else:
            raise ValueError(f"Hash table full, cannot insert '{name}' (16 vars max without considering arrays)")
    
    # Iterate buckets 0..15 to determine allocation order
    result = []
    bp_offset = 2
    for idx in range(TABLE_SIZE):
        if table[idx] is not None:
            result.append((table[idx], bp_offset))
            bp_offset += 2  # each scalar gets a 2-byte slot
    
    return result

def show_allocation(names):
    """Display the allocation for a set of variable names."""
    alloc = allocate_vars(names)
    print(f"{'Name':<20} {'Hash':>4} {'Bucket':>6} {'BP offset':>10}")
    print("-" * 44)
    for name, offset in alloc:
        h = msc_hash(name)
        print(f"{name:<20} {h:>4} {h:>6} bp-0x{offset:02x}")

def find_names_for_target(target_positions, var_sizes, max_name_len=6):
    """
    Given target bucket positions (the desired allocation order),
    find variable names that hash to produce that ordering.
    
    target_positions: list of target hash bucket values for each variable
    var_sizes: list of variable sizes (not used in hash, just for reference)
    """
    # For each target bucket, find candidate names
    charset = string.ascii_lowercase + '_'
    
    for target_bucket in sorted(set(target_positions)):
        print(f"\nNames that hash to bucket {target_bucket}:")
        candidates = []
        # Try 1-char names
        for c in charset:
            if msc_hash(c) == target_bucket:
                candidates.append(c)
        # Try 2-char names
        for c1, c2 in product(charset, repeat=2):
            name = c1 + c2
            if msc_hash(name) == target_bucket:
                candidates.append(name)
        # Show first 20
        print(f"  {candidates[:20]}")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    
    if len(sys.argv) == 2:
        name = sys.argv[1]
        h = msc_hash(name)
        print(f"hash('{name}') = {h} (bucket {h} of 16)")
    else:
        names = sys.argv[1:]
        show_allocation(names)
