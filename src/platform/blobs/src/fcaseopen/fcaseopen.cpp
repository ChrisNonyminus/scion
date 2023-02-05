#include "fcaseopen.h"

#if !defined(_WIN32)
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <unistd.h>

// r must have strlen(path) + 2 bytes
static int casepath(char const *path, char *r)
{
  size_t l = strlen(path);
  char *p = (char*)alloca(l + 1);
  strcpy(p, path);
  size_t rl = 0;

  DIR *d;
  if (p[0] == '/')
  {
    d = opendir("/");
    p = p + 1;
  }
  else
  {
    d = opendir(".");
    r[0] = '.';
    r[1] = 0;
    rl = 1;
  }

  int last = 0;
  char *c = strsep(&p, "/");
  while (c)
  {
    if (!d)
    {
      return 0;
    }

    if (last)
    {
      closedir(d);
      return 0;
    }

    r[rl] = '/';
    rl += 1;
    r[rl] = 0;

    struct dirent *e = readdir(d);
    while (e)
    {
      if (strcasecmp(c, e->d_name) == 0)
      {
        strcpy(r + rl, e->d_name);
        rl += strlen(e->d_name);

        closedir(d);
        d = opendir(r);

        break;
      }

      e = readdir(d);
    }

    if (!e)
    {
      strcpy(r + rl, c);
      rl += strlen(c);
      last = 1;
    }

    c = strsep(&p, "/");
  }

  if (d) closedir(d);
  return 1;
}
#endif

FILE *fcaseopen(char const *path, char const *mode)
{
  FILE *f = fopen(path, mode);
#if !defined(_WIN32)
  if (!f)
  {
    char *r = (char*)alloca(strlen(path) + 2);
    if (casepath(path, r))
    {
      f = fopen(r, mode);
    }
  }
#endif
  return f;
}

void casechdir(char const *path)
{
#if !defined(_WIN32)
  char *r = (char*)alloca(strlen(path) + 2);
  if (casepath(path, r))
  {
    chdir(r);
  }
  else
  {
    errno = ENOENT;
  }
#else
  chdir(path);
#endif
}

DIR* caseopendir(char const* path) {
#if !defined(_WIN32)
  char *r = (char*)alloca(strlen(path) + 2);
  if (casepath(path, r))
  {
    return opendir(r);
  }
  else
  {
    errno = ENOENT;
    return NULL;
  }
#else
  return opendir(path);
#endif
}

int casefnmatch(const char *pattern, const char *string, int flags) {
#if !defined(_WIN32)
  char *r = (char*)alloca(strlen(string) + 2);
  if (casepath(string, r))
  {
    return fnmatch(pattern, r, flags);
  }
  else
  {
    errno = ENOENT;
    return -1;
  }
#else
  return fnmatch(pattern, string, flags);
#endif
}

