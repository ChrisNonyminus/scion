#include "hooks.h"

#include "RZStatics.h"
#include "cRZString.h"

#ifdef TARGET_GAME_sc4
#include "cSC4RenderProperties.h"
#include "Statics.h"
#endif

#include <sys/mman.h>
#include <signal.h>
#include <string.h>

#include <map>

#include "ld-mac.hh"

void hook_function(intptr_t hook_addr, void *hook_dst) {
  if (hook_addr == -1) {
    printf("ERROR! Tried to hook against symbol that doesn't exist....!\n");
    raise(SIGSEGV);
  }
  if (hook_addr == (intptr_t) hook_dst) {
    printf("Attempted to hook addr %x to itself!\n", hook_addr);
    return;
  }

  *(uint8_t *) (hook_addr) = 0xe9; // jmp
  *(uint32_t *) (hook_addr + 1) =
      ((uintptr_t) hook_dst - hook_addr - 5);
}

void hook_function_inv(intptr_t hook_addr, void *hook_dst) {
  intptr_t hook_int_addr = (intptr_t) hook_dst;
  intptr_t hook_addr_ptr = (intptr_t) hook_addr;

  mprotect((void *) (hook_int_addr & ~0xFFF),
           0x1000,
           PROT_READ | PROT_WRITE | PROT_EXEC);

  *(uint8_t *) (hook_int_addr) = 0xe9; // jmp
  *(uint32_t *) (hook_int_addr + 1) =
      ((uintptr_t) hook_addr_ptr - hook_int_addr - 5);

  mprotect((void *) (hook_int_addr & ~0xFFF),
           0x1000,
           PROT_READ | PROT_EXEC);
}

void hook_abort(intptr_t hook_addr) {
  *(uint8_t *) (hook_addr) = 0x0f; // und
  *(uint8_t *) (hook_addr + 1) = 0x0b;
}

void hook_uint32(intptr_t hook_addr, uint32_t value) {
  *(uint32_t *) (hook_addr) = value;
}
void hook_pointer(intptr_t hook_addr, void *value) {
  *(intptr_t *) (hook_addr) = (intptr_t) value;
}

void hook_jmp(intptr_t hook_addr, intptr_t jmp_target) {
  if (hook_addr == jmp_target) {
    printf("Attempted to hook addr %x to itself!\n", hook_addr);
    return;
  }

  *(uint8_t *) (hook_addr) = 0xe9; // jmp
  *(uint32_t *) (hook_addr + 1) = (jmp_target - hook_addr - 5);
}

void hook_patch(intptr_t target_addr,
                void *patch_addr,
                uint32_t patch_size,
                intptr_t return_addr) {
  // allocate memory for the patch (patch_size + 5 bytes for jmp back)
  void *patch_mem = mmap(NULL,
                         patch_size + 5,
                         PROT_READ | PROT_WRITE | PROT_EXEC,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1,
                         0);
  if (patch_mem == MAP_FAILED) {
    printf("Failed to allocate memory for patch!\n");
    raise(SIGSEGV);
  }

  // copy the patch into the allocated memory
  memcpy(patch_mem, patch_addr, patch_size);

  // add a jmp back to the original code
  *(uint8_t *) ((intptr_t) patch_mem + patch_size) =
      0xe9; // jmp
  *(uint32_t *) ((intptr_t) patch_mem + patch_size + 1) =
      ((uintptr_t) return_addr - (intptr_t) patch_mem
          - patch_size - 5);

  // hook the target address to the patch
  hook_function(target_addr, patch_mem);

  // unprotect the patch memory
  mprotect(patch_mem, patch_size + 5, PROT_READ | PROT_EXEC);

}

int get_func_length(void *funcaddress) {
  int length = 0;
  for (length = 0;
       *((uint32_t *) (&((unsigned char *) funcaddress)[length]))
           != 0x90909090; ++length);
  return length;
}

#ifdef TARGET_GAME_sc4
// libstdc++ functions
#define __ZNSaIcEC1Ev_ADDR 0x00C738F5
#define __ZNSsC2EPKcRKSaIcE_ADDR 0x00C7385F
#define __ZNSaIcED1Ev_ADDR 0x00C737C4
#define __ZNSsC2Ev_ADDR 0x00C738CD
#define __ZNSsC2ERKSsjj_ADDR 0x00C73981
#define __ZNSsixEj_ADDR 0x00C739B3
#define __ZNSs5eraseEjj_ADDR 0x00C73963
#define __ZNSs6assignEPKcj_ADDR 0x00C739D1
#define __Znwj_ADDR 0x00C7399F
#define __ZNSsD2Ev_ADDR 0x00C737E2
#define __ZNSs6assignERKSs_ADDR 0x00C738AF
#define ___cxa_guard_acquire_ADDR 0xc73891
#define ___cxa_guard_release_ADDR 0xc73913
#define ___cxa_atexit_ADDR 0x000033A7
#define __ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS__ADDR 0x00C73ABC
#define __ZNSsC2EPKcjRKSaIcE_ADDR 0x00C7396D
#define __ZdlPv_ADDR 0x00C73846
#define __ZNSs5beginEv_ADDR 0x00C739FE
#define __ZNSs6assignERKSsjj_ADDR 0x00C739C7
#define __ZNKSs7compareEPKc_ADDR 0x00C73972
#define __ZNSs6appendEPKc_ADDR 0x00C7393B
#define __ZNSsC1Ev_ADDR 0x00C73AD0
#define __ZNSs6assignEPKc_ADDR 0x00C73D28
#define __ZNSsC1ERKSs_ADDR 0x00C7394F
#define __ZNSsD1Ev_ADDR 0x00C739CC
#define __ZNSs5clearEv_ADDR 0x00C73BF7
#define __ZNSs9push_backEc_ADDR 0x00C7394A
#define __ZNSs6appendEPKcj_ADDR 0x00C73A2B
#define __ZNSs3endEv_ADDR 0x00C739DB
#define __ZNKSs4findEcj_ADDR 0x00C739BD
#define __ZNKSs4findERKSsj_ADDR 0x00C7397C
#define __ZNSt15_List_node_base4hookEPS__ADDR 0x00C73968
DECL_EXTERN_C_FUNC(_ZNSs6appendERKSs, 0x00C739A4)
DECL_EXTERN_C_FUNC(
    _ZStplIcSt11char_traitsIcESaIcEESbIT_T0_T1_ERKS6_S8_,
    0x00C73959)
DECL_EXTERN_C_FUNC(_ZNSsC2ERKSs, 0x00C7398B)
DECL_EXTERN_C_FUNC(_ZNKSs5rfindEcj, 0x00C73AF8)
DECL_EXTERN_C_FUNC(_ZNKSs6substrEjj, 0x00C73B02)
DECL_EXTERN_C_FUNC(_ZNKSs7compareERKSs, 0x00C73945)
DECL_EXTERN_C_FUNC(_ZNSs6appendEjc, 0x00C739C2)
DECL_EXTERN_C_FUNC(_ZNKSs4findEPKcj, 0x00C7395E)
DECL_EXTERN_C_PTR(_ZNSt8ios_base5truncE, 0x00C7207C)
DECL_EXTERN_C_FUNC(
    _ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode,
    0x00C73AA3)
DECL_EXTERN_C_FUNC(_ZN9__gnu_cxx18__exchange_and_addEPVii,
                   0x00C73A0D)
DECL_EXTERN_C_FUNC(_ZN9__gnu_cxx12__atomic_addEPVii, 0x00C73CA6)
DECL_EXTERN_C_FUNC(
    _ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base,
    0x00C73AC6)
DECL_EXTERN_C_FUNC(_ZNSs4swapERSs, 0x00C73A30)
DECL_EXTERN_C_FUNC(_ZNKSs4findEPKcjj, 0x00C73A17)
DECL_EXTERN_C_FUNC(_ZNSt8ios_base4InitC1Ev, 0x00C73AA8)
DECL_EXTERN_C_FUNC(_ZNSsC1EPKcRKSaIcE, 0x00C73B61)
DECL_JMP(
    _ZNSt3mapIPvjSt4lessIS0_ESaISt4pairIKS0_jEEEC1Ev,
    0x00C744D5,0x0082F704)
DECL_JMP(_ZNSaISt4pairIKPvmEEC1Ev,0x00C74467,0x0082F1B2)
DECL_JMP(_ZN9__gnu_cxx13new_allocatorISt4pairIKPvmEEC2Ev,0x00C74485,0x0082F1A6)
DECL_JMP(
    _ZNSt3mapIP8ENUMLISTbSt4lessIS1_ESaISt4pairIKS1_bEEEC1Ev,
    0x00C748A9,
    0x008311B0)
DECL_JMP(_ZNSaISt4pairIKP8ENUMLISTbEEC1Ev,
         0x00C74624,
         0x00830624)
DECL_JMP(_ZN9__gnu_cxx13new_allocatorISt4pairIKP8ENUMLISTbEEC2Ev,
         0x00C7484A,
         0x00830618)
DECL_JMP(
    _ZNSt8_Rb_treeIP8ENUMLISTSt4pairIKS1_bESt10_Select1stIS4_ESt4lessIS1_ESaIS4_EEC1ERKS8_RKS9_,
    0x00C749A8,
    0x008309C0)
DECL_JMP(_ZNSaISt13_Rb_tree_nodeISt4pairIKP8ENUMLISTbEEEC1IS4_EERKSaIT_E,
         0x00C747AA,
         0x00830798)
DECL_EXTERN_C_FUNC(_Znaj,0x00C73832)
DECL_EXTERN_C_FUNC(_ZNSs6resizeEjc,0x00C73990)
DECL_EXTERN_C_FUNC(_ZNKSs13find_first_ofEPKcj,0x00C73C42)
DECL_EXTERN_C_FUNC(_ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base,0x00C73AC1)
DECL_EXTERN_C_FUNC(_ZNKSs12find_last_ofERKSsj,0x00C73DA5)
DECL_EXTERN_C_FUNC(__cxa_allocate_exception,0x00C737D3)
#define ___cxa_throw_override_ADDR 0x00C737CE
DECL_EXTERN_C_FUNC(_ZNKSs12find_last_ofEPKcj,0x00C73DAA)
DECL_EXTERN_C_FUNC(_ZNSs7reserveEj,0x00C73B0C)
DECL_JMP(__ZNSt3mapIPvmSt4lessIS0_ESaISt4pairIKS0_mEEED1Ev,0x00C74417,0x0082FCEA)
DECL_JMP
(_ZNSt8_Rb_treeIPvSt4pairIKS0_mESt10_Select1stIS3_ESt4lessIS0_ESaIS3_EEC1ERKS7_RKS8_,0x00C74462,0x0082F4CE)
DECL_JMP(_ZNSaISt13_Rb_tree_nodeISt4pairIKPvmEEEC1IS3_EERKSaIT_E,0x00C74499,0x0082F265)
DECL_JMP(_ZN9__gnu_cxx13new_allocatorISt13_Rb_tree_nodeISt4pairIKPvmEEEC2Ev,
         0x00C7445D,0x0082F258)
DECL_EXTERN_C_FUNC(_ZNSt15_List_node_base6unhookEv,0x00C739AE)

// libgcc functions
#define _malloc_ADDR 0xc7373d
#define _realloc_ADDR 0x00C73D05
#define _time_ADDR 0x00C73904
#define _clock_ADDR 0x00C73B98
#define _memmove_ADDR 0x00C73A08
#define _memset_ADDR 0x00C739A9
#define ___udivdi3_ADDR 0x00C73CC9
#define _memcpy_ADDR 0x00C7382D
#define _sprintf_ADDR 0x00C73878
#define _strncpy_ADDR 0x00C73B34
#define _isupper_ADDR 0x00C73A35
#define _tolower_ADDR 0x00C739EF
#define _setlocale_ADDR 0xc73a7b
#define _strchr_ADDR 0x00C73A4E
#define _atoi_ADDR 0xc73995
#define ___error_ADDR 0x00C73A49
#define _free_ADDR 0x00C73738
#define _sscanf_ADDR 0x00C73873
#define _setjmp_ADDR 0x00C74381
#define _vsnprintf_ADDR 0x00C73A99
#define _strncasecmp_ADDR 0x00C73BA2

// unix stuff
#define _pthread_mutexattr_init_ADDR 0x00C73B7A
#define _pthread_mutexattr_settype_ADDR 0x00C73B8E
#define _pthread_mutex_init_ADDR 0x00C73B84
#define _pthread_mutexattr_destroy_ADDR 0x00C73B75
#define _pthread_mutex_lock_ADDR 0x00C73B89
#define _pthread_mutex_unlock_ADDR 0x00C73B7F
#define _pthread_mutex_destroy_ADDR 0x00C73B70
#define _open_ADDR 0x00C73B11
#define _pthread_mutex_trylock_ADDR 0x00C73B6B
#define _stat_ADDR 0x00C73ADF
static const intptr_t _opendir_ADDR = 0x00C73AF3;
#define _pthread_cond_init_ADDR 0x00C73CB5
static const intptr_t _readdir_ADDR = 0x00C73B07;
static const intptr_t _fnmatch_ADDR = 0x00C73AE4;
static const intptr_t _closedir_ADDR =0x00C73AFD;
#define _mkdir_ADDR 0x00C73AE9
#define _remove_ADDR 0x00C73797
#define _pthread_self_ADDR 0x00C73B4D
#define _localtime_ADDR 0x00C73931
#define _acosf64_ADDR 0x00C73CA1
#define _strcmp_ADDR 0x00C73A3F
#define _lseek_ADDR 0x00C73B2A
#define _close_ADDR 0x00C73B1B
#define _pthread_cond_signal_ADDR 0x00C73CAB
#define _read_ADDR 0x00C73B39

// mac-specific functions (must implement)
#define _Gestalt_ADDR 0x00C73BC0
#define _Microseconds_ADDR 0x00C74359

// RZ/GZ framework functions (must implement)
#define __Z19RZGetCurrentAppPathR10cIGZString_ADDR 0x00016500
#define __ZN9cRZString9MakeLowerEv_ADDR 0x000137E6

// Aspyr-made functions (must implement)
#define __ZN14MacPreferences11GetCFStringEPKcPPK10__CFStringS1__ADDR 0x00450620
#define __ZN14MacPreferences11PutCFStringEPKcPK10__CFString_ADDR 0x004506A8
DECL_EXTERN_C_FUNC(_ZN14MacPreferences9GetStringEPKcPciS1_,
                   0x004506F6)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences9PutStringEPKcS1_,
                   0x004507A6)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences10GetIntegerEPKci,
                   0x00450816)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences10PutIntegerEPKci,
                   0x00450864)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences8GetFloatEPKcf,
                   0x004508A4)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences8PutFloatEPKcf,
                   0x0045090E)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences10GetBooleanEPKcb,
                   0x00450952)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences10PutBooleanEPKcb,
                   0x00450976)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences7GetRectEPKcR6CGRect,
                   0x00450986)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences7PutRectEPKcRK6CGRect,
                   0x00450AC2)
DECL_EXTERN_C_FUNC(_ZN14MacPreferences11SynchronizeEv,
                   0x00450BFE)


// SC4-specific functions (must implement)
#define __Z23SC4GetUserDataDirectoryR10cIGZStringb_ADDR 0x00023F5C
#define __ZN20cSC4RenderPropertiesC1Ev_ADDR 0x001E4370

// Static library functions
DECL_JMP(_ZN3SND14CODA_DetectCPUC1Ev, 0x00C74610, 0x00830482)
DECL_EXTERN_C_FUNC(__GLOBAL__I_Reset_Swap_EnumLists, 0x00AB6FE8)

void __GLOBAL__I_Reset_Swap_EnumLists() {
  ; //stub this out for now, it calls a crap ton of unresolved jmps
}
#elif TARGET_GAME_sims2
#define ___cxa_atexit_ADDR 0x00002F3D
#define _atexit_ADDR 0x00002F6A
#include "../The Sims 2 Bon Voyage~.x86_imp_addrs.txt"
#include "../The Sims 2 Bon Voyage~.x86_ptr_addrs.txt"
#include "../The Sims 2 Bon Voyage~.x86_imp_overrides.txt"
#define _bsd_initialization_shit_ADDR 0x0153C64E
#define ___Z41__static_initialization_and_destruction_0ii_11_ADDR 0x0153CC48
#define __ZN31TryLockFunctionEntryPointFinderC2Ev_ADDR 0x0050535C

#define _THREAD_setup_ADDR 0x005745AC
#define _stat_ADDR 0x000048A4

#endif

#include <string>
#include <string.h>

#define CDECL __attribute__((cdecl))


// function override declarations
extern "C" {
// libstdc++ functions
void _ZNSaIcEC1Ev(); // std::allocator<char>::allocator()
void _ZNSsC2EPKcRKSaIcE(); // std::string::string(char const*, std::allocator<char> const&)
void _ZNSaIcED1Ev(); // std::allocator<char>::~allocator()
void _ZNSsC2Ev(); // std::string::string()
void _ZNSsC2ERKSsjj(); // std::string::string(std::string const&, unsigned int, unsigned int)
void _ZNSsixEj(); // std::string::operator[](unsigned int)
void _ZNSs5eraseEjj(); // std::string::erase(unsigned int, unsigned int)
void _ZNSs6assignEPKcj(); // std::string::assign(char const*, unsigned int)
void _ZNSsD2Ev(); // std::string::~string()
void _ZNSs6assignERKSs(); // std::string::assign(std::string const&)
//void __cxa_guard_acquire(); // __cxa_guard_acquire
void __cxa_atexit(); // __cxa_atexit
void _ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_(); // std::_Rb_tree_insert_and_rebalance(bool, std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, std::_Rb_tree_node_base&)
void _ZNSsC2EPKcjRKSaIcE(); // std::string::string(char const*, unsigned int, std::allocator<char> const&)
void _ZdlPv(); // operator delete(void*)
void _ZNSs6assignERKSsjj(); // std::string::assign(std::string const&, unsigned int, unsigned int)
void _ZNKSs7compareEPKc(); // std::string::compare(char const*) const
void _ZNSs6appendEPKc(); // std::string::append(char const*)
void _ZNSsC1Ev(); // std::string::string()
void _ZNSs6assignEPKc(); // std::string::assign(char const*)
void _ZNSsC1ERKSs(); // std::string::string(std::string const&)
void _ZNSsD1Ev(); // std::string::~string()
void _ZNSs5clearEv(); // std::string::clear()
void _ZNSs9push_backEc(); // std::string::push_back(char)
void _ZNSs6appendEPKcj(); // std::string::append(char const*, unsigned int)
void _ZNKSs4findEcj();
void _ZNKSs4findERKSsj();
void _ZNSt15_List_node_base4hookEPS_();

// libgcc functions
int64_t __udivdi3(int64_t a,
                  int64_t b); // __udivdi3 (must implement)


int *__error();

// unix stuff
int open_override(const char *pathname, int flags);
int mkdir_debug(const char *dirname, mode_t dirmode);

// mac-specific functions (must implement)
typedef uint32_t OSErr;
typedef uint32_t OSType;
OSErr Gestalt(OSType selector, int32_t *response);
void Microseconds(uint32_t *microsecs);

// RZ/GZ framework functions (must implement)
void _Z19RZGetCurrentAppPathR10cIGZString(); // RZGetCurrentAppPath(cIGZString&) (implemented in RZStatics.cpp)
void _ZN9cRZString9MakeLowerEv(); // cRZString::MakeLower() (implemented in cRZString.cpp)

// Game-specific functions (must implement)
#ifdef TARGET_GAME_sc4
void _Z23SC4GetUserDataDirectoryR10cIGZStringb(); // SC4GetUserDataDirectory(cIGZString&, bool) (implemented in Statics.cpp)
void _ZN20cSC4RenderPropertiesC1Ev(); // cSC4RenderProperties constructor (implemented in cSC4RenderProperties.cpp)
#endif

}

bool THREAD_setup() {
  return false; // STUB/TODO
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <csetjmp>
#include <typeinfo>
#include <cxxabi.h>
#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>

int __attribute__((optimize("O2"))) tolower_override(int c) {
  asm(".intel_syntax noprefix\n\t"
      "push ebp\n\t"
      "mov ebp, esp\n\t"
      "mov edx, [ebp+8]\n\t"
      "lea eax, [edx-65]\n\t"
      "cmp eax, 25\n\t"
      "ja .dontmakelower\n\t"
      "add edx, 32\n\t"
      ".dontmakelower:\n\t"
      "leave\n\t"
      "movsx eax, dl\n\t"
      "ret\n\t"
      ".att_syntax prefix");
}

int __attribute__((optimize("O2"))) toupper_override(int c) {
  asm(".intel_syntax noprefix\n\t"
      "push ebp\n\t"
      "mov ebp, esp\n\t"
      "mov edx, [ebp+8]\n\t"
      "lea eax, [edx-65]\n\t"
      "cmp eax, 25\n\t"
      "ja .dontmakeupper\n\t"
      "sub edx, 32\n\t"
      ".dontmakeupper:\n\t"
      "leave\n\t"
      "movsx eax, dl\n\t"
      "ret\n\t"
      ".att_syntax prefix");
}

int isalnum_override(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int isalpha_override(int c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isspace_override(int c) {
  return c == ' ';
}

// returns 1 if c is a hex digit, 0 otherwise
int isxdigit_override(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int *__error() {
  return &errno;
}

void bsd_initialization_shit() {
  ; // stub
}

void __Z41__static_initialization_and_destruction_0ii_11() {
  ; //stub
}

void _ZN31TryLockFunctionEntryPointFinderC2Ev() {
  ; // stub
}

// libstdc++ functions that need to be overridden

// have to copy and paste the asm from the exact version of libstdc++ they
// used, there's no way around it :(
// side effect: have to use -O2 now because anything below that fucks with these
// functions

void __attribute__((optimize("O2"))) std_string_end_override() {
  asm(".intel_syntax noprefix\n\t"
      "push ebp\n\t"
      "mov ebp, esp\n\t"
      "mov eax, [ebp+8]\n\t"
      "mov eax, [eax]\n\t"
      "add eax, [eax-0xC]\n\t"
      "pop ebp\n\t"
      "ret\n\t"
      ".att_syntax prefix");
}

void __attribute__((optimize("O2"))) std_string_begin_override() {
  asm(".intel_syntax noprefix\n\t"
      "push ebp\n\t"
      "mov ebp, esp\n\t"
      "mov eax, [ebp+8]\n\t"
      "mov eax, [eax]\n\t"
      "pop ebp\n\t"
      "ret\n\t"
      ".att_syntax prefix");
}

void* operator_new_override(size_t size) {
  return _Znwj(LDMac::alignMem(size, 1024));
}

int open_override(const char *pathname, int flags) {
  // TODO: hack for weirdness with what seems like a bad c string
  if (strstr(pathname, "ouiav")) {
    cRZString truepath(pathname);
    truepath.replace(0, 5, "Game/");
    pathname = strdup(truepath.Data());
  }
  printf("[DEBUG] Opening file \"%s\"...\n", pathname);

  return open(pathname, flags);
}

DIR *opendir_override(const char *pathname) {
  // TODO: hack for weirdness with what seems like a bad c string
  if (strstr(pathname, "ouiav")) {
    cRZString truepath(pathname);
    truepath.replace(0, 5, "Game/");
    pathname = strdup(truepath.Data());
  }
  printf("[DEBUG] Opening directory \"%s\"...\n", pathname);
  return opendir(pathname);
}

// libgcc functions
int64_t __udivdi3(int64_t a, int64_t b) {
  return a / b;
}

int mkdir_debug(const char *dirname, mode_t dirmode) {
  // TODO: hack for weirdness with what seems like a bad c string
  if (strstr(dirname, "ouiav")) {
    cRZString truepath(dirname);
    truepath.replace(0, 5, "Game/");
    dirname = strdup(truepath.Data());
  }
  printf(
      "[DEBUG] The game is making a directory at path \"%s\"...\n",
      dirname);
  return mkdir(dirname, dirmode);
}

int stat_override(const char *pathname,
                  struct stat *statbuf) {
  // TODO: hack for weirdness with what seems like a bad c string
  if (strstr(pathname, "ouiav")) {
    cRZString truepath(pathname);
    truepath.replace(0, 5, "Game/");
    pathname = strdup(truepath.Data());
  }
  printf(
      "[DEBUG] The game is checking a file at path \"%s\"...\n",
      pathname);
  return stat(pathname, statbuf);
}

int fnmatch_override(const char *pattern, const char *string, int flags) {
  printf(
      "[DEBUG] The game is checking if \"%s\" matches \"%s\"...\n", string,
      pattern);
  return fnmatch(pattern, string, flags);
}



// Mac-specific function override definitions (TODO: Move these into mac_hooks)

// according to Apple's documentation (https://developer.apple.com/documentation/coreservices/1471624-gestalt), this function gets the information about the system
OSErr Gestalt(
    OSType selector, // the selector code for the information needed 
    int32_t *response  // the address to which to place the result returned by the selector
) {
  //printf("Gestalt called with selector %d\n", selector);
  FILE *pipe;
  char buf[100];
  switch (selector) {
  case 0x70636C6Bu: // pclk (processor clock speed in hertz)
    /*pipe = popen(
        "grep -i --color 'model name' /proc/cpuinfo | uniq | sed -e 's/.*@//'",
        "r");
    fgets(buf, 100, pipe);
    pclose(pipe);
    // set the response
    *response = (int32_t) (atof(buf) * 1000000);*/
    *response = INT32_MAX; // TODO, it seems that every method I've
    // tried for getting stuff from /proc/cpuinfo or /system/.../cpu/...
    // fails miserably
    break;
  case 1937339254: // sysv (Mac OS system version)
    return 0; // this ain't mac os
  default:printf("Gestalt: Unknown selector %d\n", selector);
    raise(SIGSEGV);
    break;
  }

  return 0;
}

// gets the number of microseconds since the system started
void Microseconds(uint32_t *microsecs) {
  struct timespec ts;
  clock_gettime(CLOCK_BOOTTIME, &ts);
  *microsecs = ts.tv_sec * 1000000;
}

#include "mac_hooks.h"
#include "MacTools.h"
#include "MacPreferences.h"

#include "SC4Hooks.h"
#include "TSHooks.h"
#include "RZHooks.h"

#include "luaHooks.h"
#include "cIGZExceptionBase.h"
#include "RZWinAPI.h"

extern "C"
// wrapper for pesky __cxa_throw calls that terminate before showing what the
// problem is
void __cxa_throw_override(void *thrown_exception,
                          std::type_info *tinfo,
                          void (*destr)
                              (void *)) {

  cIGZExceptionBase
      *the_exception = reinterpret_cast<cIGZExceptionBase *>(thrown_exception);
  if (the_exception) {
    printf("[FATAL UNHANDLABLE ERROR, EVEN BY THE ABI] %s\n", the_exception
        ->What());
    raise(SIGSEGV);
    return;
  }
  const abi::__class_type_info *exc =
      dynamic_cast<const abi::__class_type_info *>(&typeid(std::exception));

  const abi::__class_type_info
      *cti = dynamic_cast<abi::__class_type_info *>(tinfo);
  if (cti && exc) {
    std::exception *the_exception =
        reinterpret_cast<std::exception *>(abi::__dynamic_cast(thrown_exception,
                                                               cti,
                                                               exc,
                                                               -1));
    if (the_exception) {
      printf("[FATAL UNHANDLABLE ERROR, EVEN BY THE ABI] %s\n", the_exception
          ->what());
    }
  }
  raise(SIGSEGV);

}

void do_hooks() {

  printf("Hooking functions...\n");
#ifdef TARGET_GAME_sc4
  hook_function(__ZNSaIcEC1Ev_ADDR,
                CAST_CPP_FUNCPTR(_ZNSaIcEC1Ev));
  hook_function(__ZNSsC2EPKcRKSaIcE_ADDR,
                CAST_CPP_FUNCPTR(_ZNSsC2EPKcRKSaIcE));
  hook_function(__ZNSaIcED1Ev_ADDR,
                CAST_CPP_FUNCPTR(_ZNSaIcED1Ev));
  hook_function(__ZNSsC2Ev_ADDR, CAST_CPP_FUNCPTR(_ZNSsC2Ev));
  hook_function(__ZNSsC2ERKSsjj_ADDR,
                CAST_CPP_FUNCPTR(_ZNSsC2ERKSsjj));
  hook_function(__ZNSsixEj_ADDR, CAST_CPP_FUNCPTR(_ZNSsixEj));
  hook_function(__ZNSs5eraseEjj_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs5eraseEjj));
  hook_function(__ZNSs6assignEPKcj_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6assignEPKcj));
  hook_function(__Znwj_ADDR, CAST_CPP_FUNCPTR(_Znwj));
  hook_function(__ZNSsD2Ev_ADDR, CAST_CPP_FUNCPTR(_ZNSsD2Ev));
  hook_function(__ZNSs6assignERKSs_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6assignERKSs));
  hook_function(___cxa_guard_acquire_ADDR,
                CAST_CPP_FUNCPTR(__cxa_guard_acquire));
  hook_function(___cxa_guard_release_ADDR,
                CAST_CPP_FUNCPTR(__cxa_guard_release));
  hook_function(___cxa_atexit_ADDR,
                CAST_CPP_FUNCPTR(__cxa_atexit));
  hook_function(
      __ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS__ADDR,
      CAST_CPP_FUNCPTR(
          _ZSt29_Rb_tree_insert_and_rebalancebPSt18_Rb_tree_node_baseS0_RS_));
  hook_function(__ZNSsC2EPKcjRKSaIcE_ADDR,
                CAST_CPP_FUNCPTR(_ZNSsC2EPKcjRKSaIcE));
  hook_function(__ZdlPv_ADDR, CAST_CPP_FUNCPTR(_ZdlPv));
  hook_function(__ZNSs5beginEv_ADDR,
                CAST_CPP_FUNCPTR(std_string_begin_override));
  hook_function(__ZNSs6assignERKSsjj_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6assignERKSsjj));
  hook_function(__ZNKSs7compareEPKc_ADDR,
                CAST_CPP_FUNCPTR(_ZNKSs7compareEPKc));
  hook_function(__ZNSs6appendEPKc_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6appendEPKc));
  hook_function(__ZNSsC1Ev_ADDR, CAST_CPP_FUNCPTR(_ZNSsC1Ev));
  hook_function(__ZNSs6assignEPKc_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6assignEPKc));
  hook_function(__ZNSsC1ERKSs_ADDR,
                CAST_CPP_FUNCPTR(_ZNSsC1ERKSs));
  hook_function(__ZNSsD1Ev_ADDR, CAST_CPP_FUNCPTR(_ZNSsD1Ev));
  hook_function(__ZNSs5clearEv_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs5clearEv));
  hook_function(__ZNSs9push_backEc_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs9push_backEc));
  hook_function(__ZNSs6appendEPKcj_ADDR,
                CAST_CPP_FUNCPTR(_ZNSs6appendEPKcj));
  hook_function(__ZNSs3endEv_ADDR,
                CAST_CPP_FUNCPTR(std_string_end_override));
  hook_function(__ZNKSs4findEcj_ADDR,
                CAST_CPP_FUNCPTR(_ZNKSs4findEcj));
  hook_function(__ZNKSs4findERKSsj_ADDR,
                CAST_CPP_FUNCPTR(_ZNKSs4findERKSsj));
  hook_function(__ZNSt15_List_node_base4hookEPS__ADDR,
                CAST_CPP_FUNCPTR(_ZNSt15_List_node_base4hookEPS_));
  HOOK_FUNC(_ZNSs6appendERKSs)
  HOOK_FUNC(_ZStplIcSt11char_traitsIcESaIcEESbIT_T0_T1_ERKS6_S8_)
  HOOK_FUNC(_ZNSsC2ERKSs)
  HOOK_FUNC(_ZNKSs5rfindEcj)
  HOOK_FUNC(_ZNKSs6substrEjj)
  HOOK_FUNC(_ZNKSs7compareERKSs)
  HOOK_FUNC(_ZNSs6appendEjc)
  HOOK_FUNC(_ZNKSs4findEPKcj)
  HOOK_PTR(_ZNSt8ios_base5truncE)
  HOOK_FUNC(
      _ZNSt14basic_ofstreamIcSt11char_traitsIcEEC1EPKcSt13_Ios_Openmode)
  HOOK_FUNC(_ZN9__gnu_cxx18__exchange_and_addEPVii)
  HOOK_FUNC(_ZN9__gnu_cxx12__atomic_addEPVii)
  HOOK_FUNC(_ZSt18_Rb_tree_decrementPSt18_Rb_tree_node_base)
  HOOK_FUNC(_ZNSs4swapERSs)
  HOOK_FUNC(_ZNKSs4findEPKcjj)
  HOOK_FUNC(_ZNSt8ios_base4InitC1Ev)
  HOOK_FUNC(_ZNSsC1EPKcRKSaIcE)
  HOOK_JMP(_ZNSt3mapIPvjSt4lessIS0_ESaISt4pairIKS0_jEEEC1Ev)
  HOOK_JMP(_ZNSaISt4pairIKPvmEEC1Ev)
  HOOK_JMP(
      _ZNSt3mapIP8ENUMLISTbSt4lessIS1_ESaISt4pairIKS1_bEEEC1Ev)
  HOOK_JMP(_ZN9__gnu_cxx13new_allocatorISt4pairIKPvmEEC2Ev)
  HOOK_JMP(_ZNSaISt4pairIKP8ENUMLISTbEEC1Ev)
  HOOK_JMP(
      _ZN9__gnu_cxx13new_allocatorISt4pairIKP8ENUMLISTbEEC2Ev)
  HOOK_JMP(
      _ZNSt8_Rb_treeIP8ENUMLISTSt4pairIKS1_bESt10_Select1stIS4_ESt4lessIS1_ESaIS4_EEC1ERKS8_RKS9_)
  HOOK_FUNC(_Znaj)
  HOOK_FUNC(_ZNSs6resizeEjc)
  HOOK_FUNC(_ZNKSs13find_first_ofEPKcj)
  HOOK_FUNC(_ZSt18_Rb_tree_incrementPSt18_Rb_tree_node_base)
  HOOK_FUNC(_ZNKSs12find_last_ofERKSsj)
  HOOK_FUNC(__cxa_allocate_exception)
  HOOK_FUNC(__cxa_throw_override)
  HOOK_FUNC(_ZNKSs12find_last_ofEPKcj)
  HOOK_FUNC(_ZNSs7reserveEj)
  HOOK_JMP(__ZNSt3mapIPvmSt4lessIS0_ESaISt4pairIKS0_mEEED1Ev)
  HOOK_JMP(_ZNSt8_Rb_treeIPvSt4pairIKS0_mESt10_Select1stIS3_ESt4lessIS0_ESaIS3_EEC1ERKS7_RKS8_)
  HOOK_JMP(_ZNSaISt13_Rb_tree_nodeISt4pairIKPvmEEEC1IS3_EERKSaIT_E)
  HOOK_JMP(_ZN9__gnu_cxx13new_allocatorISt13_Rb_tree_nodeISt4pairIKPvmEEEC2Ev)
  HOOK_FUNC(_ZNSt15_List_node_base6unhookEv)

  hook_function(_malloc_ADDR, CAST_CPP_FUNCPTR(malloc));
  HOOK_FUNC(realloc);
  hook_function(_time_ADDR, CAST_CPP_FUNCPTR(time));
  hook_function(_clock_ADDR, CAST_CPP_FUNCPTR(clock));
  hook_function(_memmove_ADDR, CAST_CPP_FUNCPTR(memmove));
  hook_function(_memset_ADDR, CAST_CPP_FUNCPTR(memset));
  hook_function(___udivdi3_ADDR, CAST_CPP_FUNCPTR(__udivdi3));
  hook_function(_memcpy_ADDR, CAST_CPP_FUNCPTR(memcpy));
  hook_function(_sprintf_ADDR, CAST_CPP_FUNCPTR(sprintf));
  hook_function(_strncpy_ADDR, CAST_CPP_FUNCPTR(strncpy));
  hook_function(_isupper_ADDR, CAST_CPP_FUNCPTR(isupper));
  hook_function(_tolower_ADDR, CAST_CPP_FUNCPTR(tolower));
  hook_function(_setlocale_ADDR, CAST_CPP_FUNCPTR(setlocale));
  hook_function(_strchr_ADDR,
                CAST_CPP_FUNCPTR(__builtin_strchr));
  hook_function(_atoi_ADDR, CAST_CPP_FUNCPTR(atoi));
  hook_function(___error_ADDR, CAST_CPP_FUNCPTR(__error));
  hook_function(_free_ADDR, CAST_CPP_FUNCPTR(free));
  HOOK_FUNC(sscanf);
  HOOK_FUNC(setjmp);
  HOOK_FUNC(localtime);
  HOOK_FUNC(acosf64);
  HOOK_FUNC(strcmp);
  HOOK_FUNC(vsnprintf)
  HOOK_FUNC(strncasecmp)

  // unix stuff
  hook_function(_pthread_mutexattr_init_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutexattr_init));
  hook_function(_pthread_mutexattr_settype_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutexattr_settype));
  hook_function(_pthread_mutex_init_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutex_init));
  hook_function(_pthread_mutexattr_destroy_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutexattr_destroy));
  hook_function(_pthread_mutex_lock_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutex_lock));
  hook_function(_pthread_mutex_unlock_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutex_unlock));
  hook_function(_open_ADDR, CAST_CPP_FUNCPTR(open_override));
  hook_function(_pthread_mutex_destroy_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutex_destroy));
  hook_function(_pthread_mutex_trylock_ADDR,
                CAST_CPP_FUNCPTR(pthread_mutex_trylock));
  hook_function(_stat_ADDR, reinterpret_cast<void *>((stat_override)));
  hook_function(_opendir_ADDR, reinterpret_cast<void *>((opendir_override)));
  hook_function(_pthread_cond_init_ADDR,
                CAST_CPP_FUNCPTR(pthread_cond_init));
  HOOK_FUNC(readdir)
  hook_function(_fnmatch_ADDR, reinterpret_cast<void *>((fnmatch_override)));
  hook_function(_mkdir_ADDR, CAST_CPP_FUNCPTR(mkdir_debug));
  HOOK_FUNC(remove)
  HOOK_FUNC(pthread_self)
  HOOK_FUNC(lseek)
  HOOK_FUNC(close)
  HOOK_FUNC(pthread_cond_signal)
  HOOK_FUNC(closedir)
  HOOK_FUNC(read)

  HOOK_FUNC(_ZN14MacPreferences9GetStringEPKcPciS1_)
  HOOK_FUNC(_ZN14MacPreferences9PutStringEPKcS1_)
  HOOK_FUNC(_ZN14MacPreferences10GetIntegerEPKci)
  HOOK_FUNC(_ZN14MacPreferences10PutIntegerEPKci)
  HOOK_FUNC(_ZN14MacPreferences8GetFloatEPKcf)
  HOOK_FUNC(_ZN14MacPreferences8PutFloatEPKcf)
  HOOK_FUNC(_ZN14MacPreferences10GetBooleanEPKcb)
  HOOK_FUNC(_ZN14MacPreferences10PutBooleanEPKcb)
  HOOK_FUNC(_ZN14MacPreferences7GetRectEPKcR6CGRect)
  HOOK_FUNC(_ZN14MacPreferences7PutRectEPKcRK6CGRect)
  HOOK_FUNC(_ZN14MacPreferences11SynchronizeEv)

  hook_function(_Gestalt_ADDR, CAST_CPP_FUNCPTR(Gestalt));
  hook_function(_Microseconds_ADDR,
                CAST_CPP_FUNCPTR(Microseconds));

  hook_function(__Z19RZGetCurrentAppPathR10cIGZString_ADDR,
                CAST_CPP_FUNCPTR(
                    _Z19RZGetCurrentAppPathR10cIGZString));
  hook_function(__ZN9cRZString9MakeLowerEv_ADDR,
                CAST_CPP_FUNCPTR(_ZN9cRZString9MakeLowerEv));
  HookGZFrameFunctions();

#ifdef TARGET_GAME_sc4
  HookSC4Functions();
  hook_function(__Z23SC4GetUserDataDirectoryR10cIGZStringb_ADDR,
                CAST_CPP_FUNCPTR(
                    _Z23SC4GetUserDataDirectoryR10cIGZStringb));
  hook_function(__ZN20cSC4RenderPropertiesC1Ev_ADDR,
                CAST_CPP_FUNCPTR(_ZN20cSC4RenderPropertiesC1Ev));


  //hook_patch(0x001E4521, CAST_CPP_FUNCPTR(SC4_001E4521_Patch_MakeStringLowerCase), get_func_length(CAST_CPP_FUNCPTR(SC4_001E4521_Patch_MakeStringLowerCase)), 0x001E4554);
#endif

  HookMiscCarbonFunctions();
  HookCoreFoundationFunctions();
  HookCoreGraphicsFunctions();

  HookMacTools();

  HookLuaLibraryFunctions();

  HOOK_JMP(_ZN3SND14CODA_DetectCPUC1Ev)
  HOOK_FUNC(__GLOBAL__I_Reset_Swap_EnumLists)
#elif TARGET_GAME_sims2
#include "../The Sims 2 Bon Voyage~.x86_imp_hooks.txt"
#include "../The Sims 2 Bon Voyage~.x86_ptr_hooks.txt"
  HOOK_FUNC(__cxa_atexit)
  HOOK_FUNC(bsd_initialization_shit)
  HOOK_FUNC(__Z41__static_initialization_and_destruction_0ii_11)
  HOOK_FUNC(_ZN31TryLockFunctionEntryPointFinderC2Ev)
  HOOK_FUNC(stat)
  HOOK_FUNC(atexit)
  HOOK_FUNC(THREAD_setup)

  DoASLHooks();
  DoRZWinAPIHooks();
  HookGZFrameFunctions();

  HookMiscCarbonFunctions();
  HookCoreFoundationFunctions();
  HookCoreGraphicsFunctions();

  HookSims2Functions();

#endif
}