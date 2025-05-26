#!/usr/bin/env python3
"""Print an ASCII tree of all files in the repository."""

import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

for dirpath, dirnames, filenames in os.walk(ROOT):
    relpath = os.path.relpath(dirpath, ROOT)
    indent = "" if relpath == "." else "    " * (relpath.count(os.sep))
    for d in sorted(dirnames):
        print(f"{indent}{d}/")
    for f in sorted(filenames):
        print(f"{indent}{f}")
