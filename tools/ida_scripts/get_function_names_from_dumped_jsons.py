import sark
import idaapi
import idc
import ida_kernwin
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

# this script searches in symbol-less Maxis game binaries for QueryInterface functions, and, using jsons from the other script, checks a compared riid against the list of riids in the jsons to see if it is a match, and if there is only one class that implements that riid, it renames the QueryInterface function to the "ClassName::QueryInterface", and then gets the vtable pointer from the QueryInterface function, and renames the vtable to "ClassName::vtable", and then renames all the functions in the vtable (minus QueryInterface) to "ClassName::functionName"

riid_to_class_names = {} # riid -> list of class names that implement that riid
class_names_to_vtables = {} # class name -> list of vtable addresses

# looks in a QueryInterface function for a cmp instruction with a riid as the second operand, and returns a list of all the riids that are checked for
def get_riid_check_in_query_interface(func_ea):
    riids = []
    try:
        for head in sark.lines(func_ea, func_ea + idaapi.get_func(func_ea).size()):
            # if no instruction, skip
            if head.insn is None:
                continue
            if head.insn.mnem == "cmp":
                if head.insn.operands[1].type.is_imm:
                    riid = head.insn.operands[1].value
                    riids.append(riid)
    except:
        return None
    return riids

class_names_to_double_check_post = [] # list of class names that we need to double check after we have renamed all the QueryInterface functions

# iterates over all functions in the binary (note that the binary doesn't have to have symbols), and runs get_riid_check_in_query_interface on each function, and if it returns a list of riids, then it iterates over each riid in that list, and it checks if that riid is in the riid_to_class_names dictionary, and if there is only one class that implements that riid, it renames the QueryInterface function to "ClassName::QueryInterface", and then gets the vtable pointer from the QueryInterface function, and renames the vtable to "ClassName::vtable", and then renames all the functions in the vtable (minus QueryInterface) to "ClassName::functionName"
def rename_queryinterface_functions():
    for funcea in idautils.Functions():
        # there is no function name probably, this is a symbol-less binary, so instead we check if the function has an xref that is in a ".rdata" or "__const" segment and is just after a zero, and if so, we assume it is a vtable pointer
        vtable_ptr = 0
        xrefs = list(XrefsTo(funcea))
        if len(xrefs) == 0:
            continue
        for xref in xrefs:
            vtable_ptr = 0
            if idc.get_segm_name(xref.frm) == ".rdata" or idc.get_segm_name(xref.frm) == "__const":
                if idaapi.get_wide_dword(xref.frm - 4) == 0:
                    vtable_ptr = xref.frm

            if vtable_ptr == 0:
                continue

            riids = get_riid_check_in_query_interface(funcea)
            if riids is None:
                continue
            riid = 0
            if len(riids) == 1:
                riid = riids[0]
            else:
                for riid in riids:
                    if str(riid) in riid_to_class_names and len(riid_to_class_names[str(riid)]) == 1:
                        break
            if str(riid) not in riid_to_class_names:
                continue
            if len(riid_to_class_names[str(riid)]) != 1:
                continue
            class_name = riid_to_class_names[str(riid)][0]
            if not "QueryInterface" in idc.get_name(funcea):
                idc.set_name(funcea, class_name + "::QueryInterface")
            print ("Found QueryInterface for class " + class_name)
            idc.set_name(vtable_ptr, class_name + "::vtable_" +  hex(vtable_ptr)[2:] + "h")
            # note that the binary is 32-bit
            if not class_name in class_names_to_vtables:
                print("WARNING: class " + class_name + " not found in class_names_to_vtables")
                class_names_to_double_check_post.append(class_name)
                continue
            dest_vtable_size = 0
            for i in range(0, 1000):
                # if the dword is 0 or there is an xref to the address, then we assume it is the end of the vtable
                if idaapi.get_wide_dword(vtable_ptr + i * 4) == 0:
                    dest_vtable_size = i
                    break
                xrefs = list(XrefsTo(vtable_ptr + i * 4))
                if len(xrefs) > 0 and i > 0:
                    dest_vtable_size = i
                    break
            if dest_vtable_size == 0:
                continue
            print ("Found vtable size for class " + class_name + " of " + str(dest_vtable_size))
            for i in range(0, dest_vtable_size):
                dest_func_ea = idaapi.get_wide_dword(vtable_ptr + i * 4)
                if dest_func_ea == 0:
                    continue
                dest_func_sub_name = idc.get_name(dest_func_ea)
                if "QueryInterface" in dest_func_sub_name:
                    continue
                if dest_func_sub_name is None or dest_func_sub_name == "":
                    dest_func_sub_name = "sub_" + hex(dest_func_ea)[2:]
                if not dest_func_sub_name.startswith("sub_"):
                    dest_func_sub_name = "sub_" + hex(dest_func_ea)[2:]

                dest_func_sub_name = class_name + "::" + dest_func_sub_name
                idc.set_name(dest_func_ea, dest_func_sub_name)
                print ("Found function " + dest_func_sub_name + " in vtable for class " + class_name)


            src_vtable  : list[str] = class_names_to_vtables[class_name]
            print ("Found vtable for class " + class_name)

            ## resize src_vtable to remove the last function, which is an extra virtual destructor
            #src_vtable = src_vtable[:-1]

                
            if dest_vtable_size != len(src_vtable):
                print("WARNING: vtable size mismatch for class " + class_name + " (expected " + str(len(src_vtable)) + ", got " + str(dest_vtable_size) + ")")
                print("Thus, won't rename the functions to the correct names, leaving them as " + class_name + "::sub_xxxxxx")
                class_names_to_double_check_post.append(class_name)
                continue
            for i in range(0, dest_vtable_size):
                dest_func_ea = idaapi.get_wide_dword(vtable_ptr + i * 4)
                if dest_func_ea == 0:
                    continue
                src_func_name = src_vtable[i]
                dest_func_sub_name =  src_func_name
                idc.set_name(dest_func_ea, dest_func_sub_name)
                print ("Found function " + dest_func_sub_name + " in vtable for class " + class_name)


# open a window to select the riids_to_class_names json file and the class_names_to_vtables json file (the json files are generated by the other script)
def select_jsons():
    global riid_to_class_names
    global class_names_to_vtables
    riid_to_class_names = {}
    class_names_to_vtables = {}
    riid_to_class_names_json_path = ida_kernwin.ask_file(0, "*.json", "Select riid_to_class_names.json")
    if riid_to_class_names_json_path is None:
        return
    class_names_to_vtables_json_path = ida_kernwin.ask_file(0, "*.json", "Select class_names_to_vtables.json")
    if class_names_to_vtables_json_path is None:
        return
    with open(riid_to_class_names_json_path, "r") as f:
        riid_to_class_names = json.load(f)
    with open(class_names_to_vtables_json_path, "r") as f:
        class_names_to_vtables = json.load(f)
    rename_queryinterface_functions()

select_jsons()

# write to a txt file the classes that need to be double checked
with open(os.path.join(os.path.dirname(__file__), "classes_to_double_check_{}.txt".format(os.path.basename(idaapi.get_input_file_path()))), "w") as f:
    for class_name in class_names_to_double_check_post:
        f.write(class_name + "\n")





                    
        

