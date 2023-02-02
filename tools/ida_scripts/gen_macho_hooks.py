# for IDA 7.x only

import sark
import idaapi
import idc
import idautils
import os
import sys
from idautils import *
from idaapi import *

addr_defines_out_file = open("{}_imp_addrs.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')
override_defines_out_file = open("{}_imp_overrides.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')
hook_function_calls_out_file = open("{}_imp_hooks.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')

# Gets a binary's functions in the "__jump_table" section that don't start with "j_", and for each, does the following:
# Gets the function address
# Gets the function name, removing the first character if it's '_'
# appends to addr_defines_out_file: f"#define _{function_name}_ADDR 0x{function_address:X}\n"
# appends to override_defines_out_file: f"extern \"C\" void {function_name}();\n"
# appends to hook_function_calls_out_file: f"hook_function(_{function_name}_ADDR, CAST_CPP_FUNCPTR({function_name}));"
#
# For each function that starts with "j_", it does the following:
# Gets the function address
# Gets the function name, removing the starting "j_"
# Gets the address being jmped to, by getting the xref the function points to, and getting the name of the function at that address
# appends to addr_defines_out_file: f"DECL_JMP({function_name}, {function_address}, {jmp_target_address})\n"
# appends to hook_function_calls_out_file: f"HOOK_JMP({function_name})\n"

__jump_table = idaapi.get_segm_by_name("__jump_table")

for function in Functions(__jump_table.start_ea, __jump_table.end_ea):
    function_name = idaapi.get_func_name(function)
    if function_name.startswith("j_"):
        function_name = function_name[2:]
        jmp_target_address = idaapi.get_func(idc.get_operand_value(function, 0)).start_ea
        if function_name.startswith("_"):
            function_name = function_name[1:]
        if jmp_target_address == 0:
            print(f"WARNING: Couldn't find jmp target for {function_name}")
            continue
            
        addr_defines_out_file.write(f"DECL_JMP({function_name}, 0x{function:X}, 0x{jmp_target_address:X})\n")
        hook_function_calls_out_file.write(f"HOOK_JMP({function_name})\n")
    else:
        if function_name.startswith("_"):
            function_name = function_name[1:]
        addr_defines_out_file.write(f"#define _{function_name}_ADDR 0x{function:X}\n")
        override_defines_out_file.write(f"extern \"C\" void {function_name}();\n")
        hook_function_calls_out_file.write(f"hook_function(_{function_name}_ADDR, CAST_CPP_FUNCPTR({function_name}));\n")

addr_defines_out_file.close()
override_defines_out_file.close()
hook_function_calls_out_file.close()
