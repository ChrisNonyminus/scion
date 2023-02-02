
#include "cSC4App.h"
#include <SDL.h>

#include "SC4Statics.h"
#include "RZStatics.h"


static const char *kUserTag = "Posted-01/28/04";

int main(int argc, char *argv[]) {
    cRZString args;
    for (int i = 0; i < argc; i++) {
        args += argv[i];
        if (i < argc - 1) args += " ";
    }
    cRZCmdLine cmdLine(args.c_str());

    if (!Display::Initialize()) {
        return 1;
    }

    if (/*Initialize(args) &&*/ kUserTag /*???*/) {
        SC4AppStart(&cmdLine, false);
    }

    return 0;
}

void SC4OSMessageBoxAlert(const cRZString &msg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg.c_str(),
                             nullptr);
}

