#include "cRZVersion.h"

#ifdef __linux__
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>
#include <limits>
#include <fstream>
#endif

int cRZVersion::GetUserNameA(cRZString &targetName, cRZString *a3) {
#ifdef __linux__
  targetName.assign(getlogin());
  return 1;
#endif
  return 0;
}
int cRZVersion::GetModuleName(cRZString &target) {
#ifdef __linux__
  char buf[1024];
  int len = readlink("/proc/self/exe", buf, 1024);
  if (len != -1) {
    buf[len] = 0;
    target.assign(buf);
    return 1;
  }
#endif
  return 0;
}
int cRZVersion::GetModuleVersion(cRZString &target) {
#ifdef __linux__
  target.assign(" ");
  return 1;
#endif
  return 0;
}
int cRZVersion::GetHostName(cRZString &target, cRZString *a3) {
#ifdef __linux__
  char buf[1024];
  int len = gethostname(buf, 1024);
  if (strlen(buf) > 0) {
    buf[strlen(buf)] = 0;
    target.assign(buf);
    return 1;
  }
#endif
  return 0;
}
int cRZVersion::GetCPUSpeed() {
#ifdef  __linux__
  char buf[1024];
  FILE* pipe = popen(
      "lscpu | grep 'CPU max MHz:' | awk '{print $4}'",
      "r");
  fgets(buf, 100, pipe);
  pclose(pipe);
  // set the response
  return (int32_t) (atof(buf));
#endif
}

/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */
size_t getPeakRSS( )
{
#if defined(_WIN32)
  /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.PeakWorkingSetSize;

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
  /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return (size_t)0L;      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return (size_t)0L;      /* Can't read? */
    }
    close( fd );
    return (size_t)(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
  /* BSD, Linux, and OSX -------------------------------------- */
  struct rusage rusage;
  getrusage( RUSAGE_SELF, &rusage );
#if defined(__APPLE__) && defined(__MACH__)
  return (size_t)rusage.ru_maxrss;
#else
  return (size_t)(rusage.ru_maxrss * 1024L);
#endif

#else
  /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}





/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */
int64_t getCurrentRSS( )
{
#if defined(_WIN32)
  /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
  /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (size_t)0L;      /* Can't access? */
    return (size_t)info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
  /* Linux ---------------------------------------------------- */
  int64_t rss = 0L;
  FILE* fp = NULL;
  if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
    return (size_t)0L;      /* Can't open? */
  if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
  {
    fclose( fp );
    return (size_t)0L;      /* Can't read? */
  }
  fclose( fp );
  return rss * 1024;

#else
  /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;          /* Unsupported. */
#endif
}

uint64_t get_mem_total() {
  FILE *meminfo = fopen("/proc/meminfo", "r");
  if(meminfo == NULL)
  return 0; // handle error

  char line[256];
  while(fgets(line, sizeof(line), meminfo))
  {
    int ram;
    if(sscanf(line, "MemAvailable: %d kB", &ram) == 1)
    {
      fclose(meminfo);
      return ram / 1024;
    }
  }

  // If we got here, then we couldn't find the proper line in the meminfo file:
  // do something appropriate like return an error code, throw an exception, etc.
  fclose(meminfo);
  return 0;
}

int cRZVersion::GetSystemMemory(
    uint32_t &memUsed,
    uint32_t &memFree
    ) {
  uint64_t mem = getCurrentRSS() / 1024ULL / 1024ULL;
  uint64_t allmem = get_mem_total();
  memFree = (allmem - mem);
  memUsed = (mem); // TODO
  return 1;
}
