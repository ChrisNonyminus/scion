#ifndef BLOBS_MMAP_H
#define BLOBS_MMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <memory.h>

#include <sys/mman.h>
#include <signal.h>
#include <execinfo.h>
#include <errno.h>
#include <malloc.h>

#include "hooks.h"
#define LDMAC_IMPL 1
#include "ld-mac.hh"
#define ALIGNPAGE(x) (((x) + 0xFFF) & ~0xFFF)

MachO *gMachO = NULL;
LDMac::MachOLoader gMALoader;

extern "C" {
void full_write(int fd, const char *buf, size_t len);
void crash_handler_basic(int sig);
}

void full_write(int fd, const char *buf, size_t len) {
  while (len > 0) {
    ssize_t ret = write(fd, buf, len);

    if ((ret == -1) && (errno != EINTR))
      break;

    buf += (size_t) ret;
    len -= (size_t) ret;
  }
}

void crash_handler_basic(int sig) {
  static const char start[] = "BACKTRACE:\n----------------------\n";
  static const char end[] = "";

  void *bt[1024];
  int bt_size;
  char **bt_syms;
  int i;

  bt_size = backtrace(bt, 1024);
  bt_syms = backtrace_symbols(bt, bt_size);
  full_write(STDERR_FILENO, start, strlen(start));
  for (i = 1; i < bt_size; i++) {
    size_t len = strlen(bt_syms[i]);
    full_write(STDERR_FILENO, bt_syms[i], len);
    full_write(STDERR_FILENO, "\n", 1);
  }
  full_write(STDERR_FILENO, end, strlen(end));

  char *crash_print = (char *) malloc(1024);
  strcpy(crash_print, start);
  for (i = 1; i < bt_size; i++) {
    strcat(crash_print, bt_syms[i]);
    strcat(crash_print, "\n");
  }
  strcat(crash_print, end);

  FILE *f = fopen("crash.log", "w");
  if (f) {
    fwrite(crash_print, 1, strlen(crash_print), f);
    fclose(f);
  }

  free(crash_print);

  free(bt_syms);
  exit(-1);
}

inline void MapMacho(const char *path) {
  /*signal(SIGSEGV, crash_handler_basic);
  signal(SIGABRT, crash_handler_basic);
  if (mmap((void*)0x1000, MACHO_EXE_END - 0x1000, PROT_READ | PROT_WRITE, MAP_ANON|MAP_PRIVATE|MAP_FIXED, -1, 0) == (void*)-1) {
      printf("ERROR mmapping: %s\n", strerror(errno));
      exit(-1);
  }

  // Fill with illegal instructions
  for (int i = 0; i < MACHO_EXE_END- MACHO_TEXT_ADDRESS; i += 2)
  {
      *(uint8_t*)(MACHO_TEXT_ADDRESS+i) = 0x0f;
      *(uint8_t*)(MACHO_TEXT_ADDRESS+i+1) = 0x0b;
  }

  // Zero all data sections before loading
  memset((void*)MACHO_DATA_ADDRESS, 0, MACHO_DATA_MEM_SIZE);
  memset((void*)MACHO_IMPORT_ADDRESS, 0, MACHO_IMPORT_MEM_SIZE);
#ifdef TARGET_GAME_sc4
  memset((void*)MACHO_OBJC_ADDRESS, 0, MACHO_OBJC_MEM_SIZE);
#endif

  FILE* f = fopen(path, "rb"); // we must assume the macho is an x86 binary extracted from the UB version
  if (!f) {
      printf("Failed to open `%s`! Make sure the file exists in the current working directory.", path);
      exit(-1);
  }

  fseek(f, MACHO_TEXT_FILE_OFF, SEEK_SET);
  printf("Using macho blob...\n");
  fread((void*)MACHO_TEXT_ADDRESS, MACHO_TEXT_FILE_SIZE, 1, f);

  fseek(f, MACHO_DATA_FILE_OFF, SEEK_SET);
  fread((void*)MACHO_DATA_ADDRESS, MACHO_DATA_FILE_SIZE, 1, f);

  fseek(f, MACHO_IMPORT_FILE_OFF, SEEK_SET);
  fread((void*)MACHO_IMPORT_ADDRESS, MACHO_IMPORT_FILE_SIZE, 1, f);

#ifdef TARGET_GAME_sc4
  fseek(f, MACHO_OBJC_FILE_OFF, SEEK_SET);
  fread((void*)MACHO_OBJC_ADDRESS, MACHO_OBJC_FILE_SIZE, 1, f);
#endif



  fclose(f);

  do_hooks();

  if (mprotect((void*)0x1000, MACHO_TEXT_MEM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) == -1){
      printf("ERROR setting macho code section at %p as executable: %s\n", (void*)0x1000, strerror(errno));
      exit(-1);
  }

  if (mprotect((void*)MACHO_IMPORT_ADDRESS, MACHO_IMPORT_MEM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) == -1){
      printf("ERROR setting macho code section at %p as executable: %s\n", (void*)MACHO_IMPORT_ADDRESS, strerror(errno));
      exit(-1);
  }


  for (uint32_t i = MACHO___mod__init_func_ADDRESS;
  i < MACHO___mod__init_func_ADDRESS + MACHO___mod__init_func_MEM_SIZE;
  i+=4) {
      gInitFuncs.push_back(*(StaticInitFunc*)i);
  }
   return;
*/

  LDMac::g_timer.start();
  LDMac::initSignalHandler();
  LDMac::initRename();
  LDMac::initNoTrampoline();
  LDMac::initLibMac();
  LDMac::initDlfcn();

  gMachO = MachO::read(path, "i386");
  if (gMachO->is64()) {
    fprintf(stderr, "%s: not 32bit binary\n", path);
    exit(1);
  }

  gMALoader.load(*gMachO);

  do_hooks();

  gMALoader.runInitFuncs(-1, NULL, NULL, NULL);

}

#endif