import sark
import idaapi
import idc
import idautils
import json
import os
import sys
from idautils import *
from idaapi import *

# gets a class name from a symbol name, respecting namespaces
# Gonzo-Rizzo class names are in the format "<nNamespace>::cClassName"
# all namespace names start with "n" and class names start with "c"
def get_class_name_from_symbol_name(symbol_name):
    if symbol_name[0] == "n":
        return symbol_name.split("::")[0] + "::" + symbol_name.split("::")[1]
    return symbol_name.split("::")[0]



# QueryInterface functions have the following signature:
# bool ClassName::QueryInterface(uint32_t riid, void **ppvObject)
# The riid is the second parameter, and the third parameter is a pointer to the interface pointer.
def get_riid_from_queryinterface_call(call_ea):
    return idc.GetOperandValue(call_ea, 1)

# looks in a QueryInterface function for a cmp instruction with a riid as the second operand, and returns a list of all the riids that are checked for
def get_riid_check_in_query_interface(func_ea):
    riids = []
    for head in sark.lines(func_ea, func_ea + idaapi.get_func(func_ea).size()):
        if head.insn.mnem == "cmp":
            if head.insn.operands[1].type.is_imm:
                riid = head.insn.operands[1].value
                riids.append(riid)
    if len(riids) == 0:
        return None
    return riids



# looks through the symbol table for QueryInterface functions, and returns a dictionary of riid -> list of class names that implement that riid
def get_riids_to_class_names():
    riids_to_class_names = {}
    for funcea in idautils.Functions():
        functionName = idaapi.get_func_name(funcea)
        # if the function name is mangled, demangle it
        if functionName.startswith("_"): # gnu c++ mangled name
            name = idaapi.demangle_name(functionName, 0)
            if name is not None:
                functionName = name
            
        if "QueryInterface" in functionName and not functionName.startswith("`"):
            className = get_class_name_from_symbol_name(functionName)
            riids = get_riid_check_in_query_interface(funcea)
            if riids is not None:
                for riid in riids:
                    if riid not in riids_to_class_names:
                        riids_to_class_names[riid] = []
                    riids_to_class_names[riid].append(className)

                
    return riids_to_class_names


# given a function address, get an xref to it that is in a ".rdata" or "__const" segment, checks if it may be a vtable, and returns a list of all the function names in that resulting vtable
def get_vtable_pointer_from_function_address(function_address):
    vtable = []
    for xref in XrefsTo(function_address):
        seg = idaapi.getseg(xref.frm)
        if seg is not None:
            if seg.type == idaapi.SEG_DATA:
                vtable_pointer = xref.frm
                num_dtors = 0 # ensure this is always 1
                for i in range(0, 1000):
                    # address is 32-bit
                    function_address = idaapi.get_wide_dword(vtable_pointer + i * 4)
                    if function_address == 0:
                        break
                    functionName = idaapi.get_func_name(function_address)
                    if functionName is None:
                        break
                    # if the function name is mangled, demangle it
                    if functionName.startswith("_"): # gnu c++ mangled name
                        name = idaapi.demangle_name(functionName, 0)
                        if "~" in name:
                            num_dtors += 1
                        if num_dtors > 1:
                            continue
                    vtable.append(functionName)
                return vtable
            else:
                print("seg is not CONST for address: " + hex(xref.frm))
                continue
        else:
            print("seg is None for address: " + hex(xref.frm))
            continue
    return vtable



# gets vtables (by getting the address of a QueryInterface function and finding a pointer to it), and returns a dict of class name -> vtable
def get_class_names_to_vtables():
    class_names_to_vtables = {}
    for funcea in idautils.Functions():
        functionName = idaapi.get_func_name(funcea)
        # if the function name is mangled, demangle it
        if functionName.startswith("_"): # gnu c++ mangled name
            name = idaapi.demangle_name(functionName, 0)
            if name is not None:
                functionName = name
            
        if "QueryInterface" in functionName and not functionName.startswith("`"):
            className = get_class_name_from_symbol_name(functionName)
            vtable_pointer = get_vtable_pointer_from_function_address(funcea)
            if vtable_pointer is not None:
                class_names_to_vtables[className] = vtable_pointer
    return class_names_to_vtables

filename = os.path.basename(idaapi.get_input_file_path())
out_riids_to_class_names_path = os.path.join(os.path.dirname(__file__), "riids_to_class_names_{}.json".format(filename))
out_class_names_to_vtables_path = os.path.join(os.path.dirname(__file__), "class_names_to_vtables_{}.json".format(filename))
    

# calls get_riids_to_class_names, and writes the result to a json file
def dump_riids_to_json():
    riids_to_class_names = get_riids_to_class_names()
    with open(out_riids_to_class_names_path, "w") as f:
        json.dump(riids_to_class_names, f, indent=4)

# calls get_class_names_to_vtables, and writes the result to a json file
def dump_class_names_to_vtables():
    class_names_to_vtables = get_class_names_to_vtables()
    with open(out_class_names_to_vtables_path, "w") as f:
        json.dump(class_names_to_vtables, f, indent=4)

dump_riids_to_json()
dump_class_names_to_vtables()

    
    