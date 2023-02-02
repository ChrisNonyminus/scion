#!/usr/bin/python3

import sys
import subprocess
import re
import lief
import cxxfilt

if len(sys.argv) != 2:
    print("Usage: %s <x86-32 mach-o file>" % sys.argv[0])
    sys.exit(1)

addr_defines_out_file = open("{}_imp_addrs.txt".format(sys.argv[1]), 'w')
override_defines_out_file = open("{}_imp_overrides.txt".format(sys.argv[1]), 'w')
hook_functino_calls_out_file = open("{}_imp_hooks.txt".format(sys.argv[1]), 'w')

macho = lief.parse(sys.argv[1])
imports_segment = macho.get_segment("__IMPORT")
jumptable_section = macho.get_section("__jump_table")


def get_demangled(symbol_name):
    demangled = None
    try:
        demangled = cxxfilt.demangle(symbol_name)
    except:
        demangled = symbol_name
    return demangled

def print_funchook(name, address, demangled):
    addr_defines_out_file.write(f"#define {name}_ADDR 0x%x\n" % address)
    if ((name.startswith("_"))):
        name = name[1:]
    override_defines_out_file.write(f"void {name}(); // {demangled}\n")
    hook_functino_calls_out_file.write(f"hook_function({name}_ADDR, CAST_CPP_FUNCPTR({name}));\n")
    addr_defines_out_file.flush()
    override_defines_out_file.flush()
    hook_functino_calls_out_file.flush()


for symbol in macho.imported_symbols:
    name = symbol.name
    address = symbol.value
    demangled = get_demangled(name[1:])
    print_funchook(name, address, demangled)