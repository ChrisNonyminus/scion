# for IDA 7.x only

import sark
import idaapi
import idc
import idautils
import os
import sys
from idautils import *
from idaapi import *

# Takes in a user inputted name and address for a vtable struct, and generates a C++ interface from it
# The user is prompted for the name of the vtable struct, and the address of the vtable struct

vtableName = ida_kernwin.ask_str("cITSPersistSystem", 0, "Enter the name of the vtable struct")
vtableSymbol = ida_kernwin.ask_str("0x1F2AA28", 0, "Enter the address of the vtable struct in hex")
vtableSize = int(ida_kernwin.ask_str("0x240", 0, "Enter the size of the vtable struct in hex"), 16)

# Gets the address of the vtable struct
vtableStruct = int(vtableSymbol, 16)

# GCC vtable structures are laid out as follows:
# 0x0: 0x0 (offset of this vtable from the start of the class)
# 0x4: offset to the RTTI structure
# 0x8...: pointers to the virtual functions
# 0x??? (Next vtable struct) (optional, if the class has multiple base classes): (offset of this vtable from the start of the class)
# 0x???+0x4...: pointers to the virtual functions
# and so on
# I don't feel like implementing these shenanigans, so I'm just going to assume that the user is inputting the address that contains a pointer pointing directly to a vtable's first virtual function

vtables = []

# Gets the virtual functions in the vtable struct
for off in range(vtableStruct, vtableStruct + vtableSize, 4):
    vtableFunc = idc.get_wide_dword(off)
    if vtableFunc == 0:
        continue
    vtables.append(vtableFunc)

# Gets the name of the class that implements the interface
firstFuncSymbol = idc.get_func_name(vtables[0])
firstFuncSymbol = firstFuncSymbol[1:] # Removes the extra leading underscore (mac binaries add this)
# Demangles the symbol
firstFuncSymbol = idc.demangle_name(firstFuncSymbol, idc.get_inf_attr(INF_SHORT_DN))
# Gets the class name
className = firstFuncSymbol.split("::")[0]

interfaceName = vtableName

# Generates the C++ interface
interface = "class " + interfaceName + " {\n"
interface += "public:\n"
for vtable in vtables:
    vtableFuncSymbol = idc.get_func_name(vtable)
    print(vtableFuncSymbol)
    vtableFuncSymbol = vtableFuncSymbol[1:] # Removes the extra leading underscore (mac binaries add this)
    vtableFuncSymbol = idc.demangle_name(vtableFuncSymbol, idc.get_inf_attr(INF_SHORT_DN))
    # removes the class name from the symbol
    vtableSplit = vtableFuncSymbol.split("::")
    vtableSplit.pop(0)
    vtableFuncSymbol = "::".join(vtableSplit)
    interface += "    virtual uint32_t " + vtableFuncSymbol + " = 0;\n"
interface += "};\n"

ida_kernwin.msg(interface)
# Saves the interface
with open(f"{os.path.dirname(__file__)}/{interfaceName}.h", "w") as f:
    f.write(interface)



