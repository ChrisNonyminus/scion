#ifndef BLOBS_HOOKS_H
#define BLOBS_HOOKS_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string>

void hook_function(intptr_t hook_addr, void *hook_dst);
void hook_function_inv(intptr_t hook_addr, void *hook_dst);
void hook_abort(intptr_t hook_addr);
void hook_uint32(intptr_t hook_addr, uint32_t value);
void hook_pointer(intptr_t hook_addr, void *value);
void hook_jmp(intptr_t hook_addr, intptr_t jmp_target);

void hook_patch(intptr_t target_addr,
                void *patch_addr,
                uint32_t patch_size,
                intptr_t return_addr);

#define DECL_EXTERN_C_FUNC(symbol, address) \
static const intptr_t _ ## symbol ## _ADDR = address; extern "C" void symbol();
#define DECL_EXTERN_C_PTR(symbol, address) \
static const intptr_t _ ## symbol ## _ptr_ADDR = address; extern "C" void* symbol;
#define HOOK_FUNC(symbol) hook_function(_ ## symbol ## _ADDR, CAST_CPP_FUNCPTR(symbol));
#define HOOK_PTR(symbol) hook_pointer(_ ## symbol ## _ptr_ADDR, &(symbol));
#define CAST_CPP_FUNCPTR(func) reinterpret_cast<void*>((func))
#define DECL_JMP(symbol, jmp_address, intended_target_address) \
static const intptr_t j_ ## symbol ## _ADDR = jmp_address; \
static const intptr_t _ ## symbol ## _ADDR = intended_target_address;
#define HOOK_JMP(symbol) hook_jmp(j_ ## symbol ## _ADDR, _ ## symbol ## _ADDR);
#define STUB_FUNC(symbol, address) DECL_EXTERN_C_FUNC(symbol, address) \
void symbol() {}
void do_hooks();


/*helper functions*/
void mkdirpwrapper(const std::string & p);

#endif