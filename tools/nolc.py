#!/usr/bin/env python3
import re, sys
from pathlib import Path

def q(s):
    return '"' + s.replace("\\","\\\\").replace('"','\\"') + '"'

def compile_nol(src):
    out = ['#include "terminal.h"', '#include "nol_runtime.h"', '']
    for raw in src.splitlines():
        line = raw.strip()
        if not line or line.startswith("#"): continue
        m = re.match(r"fn\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*\{?$", line)
        if m:
            name,args=m.groups()
            aa=[]
            for a in [x.strip() for x in args.split(",") if x.strip()]:
                aa.append("const char *"+a if a=="cmd" else "int "+a)
            out.append("void %s(%s) {"%(name,", ".join(aa))); continue
        if line=="}": out.append("}"); continue
        if line=="else {": out.append("} else {"); continue
        m=re.match(r'else if\s+cmd\s*==\s*"(.*)"\s*\{$',line)
        if m: out.append("else if (nol_streq(cmd, %s)) {"%q(m.group(1))); continue
        m=re.match(r'if\s+cmd\s*==\s*"(.*)"\s*\{$',line)
        if m: out.append("if (nol_streq(cmd, %s)) {"%q(m.group(1))); continue
        m=re.match(r'print\s+"(.*)"$',line)
        if m: out.append("terminal_write(%s);"%q(m.group(1))); continue
        m=re.match(r"put\s+(.+)$",line)
        if m: out.append("terminal_putc(%s);"%m.group(1)); continue
        if line in ("backspace","backspace;"): out.append("terminal_backspace();"); continue
        if line in ("clear","clear;"): out.append("terminal_clear();"); continue
        m=re.match(r"call\s+([A-Za-z_][A-Za-z0-9_]*)\s*;?$",line)
        if m: out.append("%s();"%m.group(1)); continue
        if line in ("return","return;"): out.append("return;"); continue
        raise SyntaxError("NOL: unsupported syntax: "+line)
    return "\n".join(out)+"\n"

if __name__=="__main__":
    if len(sys.argv)!=3:
        print("usage: nolc.py input.nol output.c"); raise SystemExit(2)
    Path(sys.argv[2]).write_text(compile_nol(Path(sys.argv[1]).read_text()))
