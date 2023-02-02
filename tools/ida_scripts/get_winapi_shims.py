# for IDA 7.x only

from io import TextIOWrapper
import sark
import idaapi
import idc
import idautils
import os
import sys
import re
from idautils import *
from idaapi import *
import glob

addr_defines_out_file = open("{}_winapi_addrs.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')
override_defines_out_file = open("{}_winapi_overrides.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')
hook_function_calls_out_file = open("{}_winapi_hooks.txt".format(os.path.basename(idaapi.get_input_file_path())), 'w')

# Gets every function in a binary that contains a known winapi function name in its signature, and for each, does the following:
# Gets the function address
# Gets the function name, removing the first character if it's '_'
# trims the function name to just contain the WinAPI function name
# appends to addr_defines_out_file: f"#define _{function_name}_ADDR 0x{function_address:X}\n"
# appends to override_defines_out_file: f"extern \"C\" void {function_name}();\n"
# appends to hook_function_calls_out_file: f"hook_function(_{function_name}_ADDR, CAST_CPP_FUNCPTR({function_name}));"

# NOTE: this script cannot handle any calls to GetProcAddress that get the function shims (if the shims are being called through an emulated kernel32.dll somehow).

cwd = os.path.dirname(os.path.realpath(__file__))

win32_api_function_name_list = []
win32_api_symbol_map = {}


for def_file in glob.glob("/usr/lib/i386-linux-gnu/wine/lib*.def"):
    print(def_file)
    dll = open(def_file, 'rt')
    for line in dll.readlines():
        m = re.match('  ([^ =\@\n]*)\@[0-9]*[ =\@]', line)
        if m:
            win32_api_function_name_list.append(m.group(1))
            print(m.group(1))
            win32_api_symbol_map[m.group(1)] = "_" + re.match('  (.*\@[0-9]*)[ =\@]', line).group(1)

for function in Functions():
    function_name = idaapi.get_func_name(function)
    for substring in win32_api_function_name_list:
        if substring in function_name: 
            if function_name.startswith("_"):
                function_name = function_name[1:]
            if function_name.startswith("_"): # ignore c++ functions for now
                break
            if function_name != substring:
                print(f"{function_name} does not match {substring}")
                continue # ignore any possible false positives
            print(function_name, ":", win32_api_symbol_map[substring])
            addr_defines_out_file.write(f"#define _{function_name}_ADDR 0x{function:X}\n")
            override_defines_out_file.write(f"extern \"C\" void {win32_api_symbol_map[substring]}();\n")
            hook_function_calls_out_file.write(f"hook_function(_{function_name}_ADDR, CAST_CPP_FUNCPTR({substring}));\n")
            addr_defines_out_file.flush()
            override_defines_out_file.flush()
            hook_function_calls_out_file.flush()
            break

addr_defines_out_file.close()
override_defines_out_file.close()
hook_function_calls_out_file.close()
