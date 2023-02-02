#!/usr/bin/python3

# Takes a Mach-O file and outputs a linker script with symbol definitions (mangled_name = 0xaddress;) for all symbols in the file.

import sys
import subprocess
import re
from macholib.MachO import *
from macholib.SymbolTable import *


def main():
    if len(sys.argv) != 2:
        print("Usage: %s <mach-o file>" % sys.argv[0])
        sys.exit(1)

    macho = MachO(sys.argv[1])
    symtab = SymbolTable(macho)

    for symbol, name in symtab.nlists:
        if symbol.n_type & 0x0e == 0xe:
            # if the name's a bytestring, decode it
            if isinstance(name, bytes):
                name = name.decode("utf-8")
            if len(name) == 0:
                continue
            print("%s = 0x%x;" % (name, symbol.n_value))

    # define the sections the linux elf will link together, so it will not overlap with the mach-o's symbols
    print("")
    print("SECTIONS {")
    # base address of elf will be 0x10000000
    print(" .text 0x10000000 : { *(.text) }")
    # data section will be right after the text section
    print(" .data 0x10000000 + SIZEOF(.text) : { *(.data) }")
    # rodata section will be right after the data section
    print(" .rodata 0x10000000 + SIZEOF(.text) + SIZEOF(.data) : { *(.rodata) }")
    # bss section will be right after the rodata section
    print(" .bss 0x10000000 + SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.rodata) : { *(.bss) }")


    print("}")

    


if __name__ == "__main__":
    main()