#include "Statics.h"
#include "cRZString.h"

#include <boost/filesystem.hpp>

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#elif defined(_WIN32)
#include <windows.h>
#include <shlobj.h>

#endif


void SC4GetUserDataDirectory(cIGZString& output, bool a2) {
#ifdef __linux__
    output.FromChar(getenv("HOME"));
    output.Append(cRZString("/.scion/SC4/"));
    if (a2) { // create directory if it doesn't exist
        /*boost::filesystem::create_directories(boost::filesystem::path(output.Data()));*/
    }
#elif defined(_WIN32)
    char szPath[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK) {
        output.FromChar(szPath);
        output.Append("\\SimCity 4\\");
        if (a2) { // create directory if it exists
            CreateDirectoryA(output.Data(), NULL);
        }
    }
#endif
}