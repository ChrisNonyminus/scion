#ifndef fcaseopen_h
#define fcaseopen_h
/*
 *
fcaseopen()
casechdir()

case-insensitive fopen and friends for case-sensitive file-systems.

MIT license.

 https://github.com/OneSadCookie/fcaseopen

Modified to add additional case insensitive FS functions.
 * */
#include <stdio.h>
#include <dirent.h>
#include <fnmatch.h>

#if defined(__cplusplus)
extern "C" {
#endif

extern FILE *fcaseopen(char const *path, char const *mode);

extern void casechdir(char const *path);

extern DIR* caseopendir(char const* path);

extern int casefnmatch(const char *pattern, const char *string, int flags);

#if defined(__cplusplus)
}
#endif

#endif