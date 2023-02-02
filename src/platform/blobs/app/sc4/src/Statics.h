#pragma once
#include "cIGZString.h"
#include <string>

// Gets the path to the user data directory the game uses.
// On Windows, this would be something like "C:\\Users\\username\\Documents\\SimCity 4\\".
// Since we're using Linux, we will do things a bit differently.
void SC4GetUserDataDirectory(cIGZString& output, bool a2);