# for IDA 7.x only

import sark
import idaapi
import idc
import idautils
import os
import sys
from idautils import *
from idaapi import *

addr_defines_out_file = open("{}_ptr_addrs.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')
hook_function_calls_out_file = open("{}_ptr_hooks.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')

# Gets a binary's pointers in the "__pointers" section, and for each, does the following:
# Gets the symbol name
# Gets the name of what it's supposed to be pointing to (which is the pointer's symbol name without the "_ptr") suffix
# Gets the address of both the pointer and what should be pointed to
# appends to addr_defines_out_file: f"#define _{pointee_name}_ptr_ADDR 0x{pointer_address:X}\n"
# appends to addr_defines_out_file: f"#define _{pointee_name}_ADDR 0x{pointee_address:X}\n"
# appends to hook_function_calls_out_file: f"hook_uint32(_{pointee_name}_ptr_ADDR, _{pointee_name}_ADDR);"

pointers_sec = idaapi.get_segm_by_name("__pointers")

for symbol in range(pointers_sec.start_ea, pointers_sec.end_ea, 4):
    symbol_name = idaapi.get_name(symbol)
    if symbol_name.endswith("_ptr"):
        pointee_name = symbol_name[:-4]
        pointer_address = symbol
        # can't get the pointee address through the value of the pointer, because sometimes it's zero
        # pointee_address = idc.get_wide_dword(pointer_address)
        pointee_address = idc.get_name_ea_simple(pointee_name)
        addr_defines_out_file.write(f"#define _{pointee_name}_ptr_ADDR 0x{pointer_address:X}\n")
        addr_defines_out_file.write(f"#define _{pointee_name}_ADDR 0x{pointee_address:X}\n")
        hook_function_calls_out_file.write(f"hook_uint32(_{pointee_name}_ptr_ADDR, _{pointee_name}_ADDR);\n")

addr_defines_out_file.close()
hook_function_calls_out_file.close()