//
// Created by chris on 1/17/2023.
//

#include "SC4Statics.h"
#include "cGZFramework.h"
#include "cSC4App.h"

#include <algorithm>

bool gbAppResourcesInitializedOK = true;
uint32_t gbAppResourceErrorLanguage;
bool gbShouldUseFrameworkOverrideClasses; /*
                                             this boolean is put in
                                             zero-initialized memory (bss)
                                             and is never written to, so it
                                             will always be false, lmao
                                           */
void SC4DisplayResourceError(uint32_t language) {
    const char *v1;   // ebx

    switch (language) {
        /*case 3u:
            v1 = "Donn\xE9\x65\x73 du jeu introuvables. Il s'agit peut-"
                 "\xEA\x74\x72e d'un probl\xE8\x6D\x65 d'installation o"
                 "u de syst\xE8\x6D\x65 d'exploitation.";
            break;
        case 4u:
            v1 = "Es k\xF6\x6E\x6E\x65n keine Spieldateien gefunden wer"
                 "den. Dies ist wahrscheinlich ein des System- oder Ins"
                 "tallationsproblem.";
            break;
        case 5u:
            v1 =
                "Impossibile trovare i dati del gioco. Probabilmente s"
                "i tratta di un problema di installazione o del sistema
        operativo."; break; case 6u: v1 = "No se encuentran los datos del juego.
        Puede deberse a" " un problema de la instalaci\xF3\x6E\x20\x6F del
        sistema " "operativo"; break; case 7u: v1 = "Geen spelgegevens gevonden.
        Dit probleem houdt waarsc" "hijnlijk verband met de installatie of met
        het besturingssysteem."; break; case 8u: v1 = "Kan ikke finde spil-data.
        Dette er formentlig et prob" "lem med installationen eller operativ
        systemet."; break; case 9u: v1 = "Kan inte hitta speldata. Felet
        orsakades antagligen a" "v ett installationsproblem eller ett problem
        med " "operativsystemet."; break; case 0xAu: v1 = "Kan ikke finne
        spilldata. Dette er sannsynligvis et p" "roblem med installasjonen eller
        operativsystemet."; break; case 0xBu: v1 = "Pelitiedostoja ei
        l\xF6\x79\x64\x79. Kyseess\xE4\x20\x6F" "n todenn\xE4\x6B\xF6isesti joko
        asennusongelma tai k\xE4"
                 "\x79\x74t\xF6\x6A\xE4\x72jestelm\xE4\x6E\x20toimintaa"
                 "n liittyv\xE4\x20\x6Fngelma.";
            break;
        case 0xEu:
            v1 = "N\xE3\x6F\x20foi poss\xED\x76\x65l encontrar os dados"
                 " do jogo. Provavelmente ocorreu um erro durante a ins"
                 "tala\xE7\xE3\x6F, ou com seu sistema operacional.";
            break;
        case 0xFu:
            v1 = "\x83Q\x81[\x83\x80\x83f\x81[\x83^\x82\u14B4C\xA9\x82ł"
                 "\xAB\x82܂\xB9\x82\xF1\x81\x42\x83C\x83\x93\x83X\x83g\x81"
                 "[\x83\x8B\x82\xE0\x82\xB5\x82\xAD\x82͂n\x82r\x82ɖ\xE2"
                 "\x91\xE8\x82\xAA\x94\xAD\x90\xB6\x82\xB5\x82\xBD\x82悤\x82ł\xB7"
                 "\x81B";
            break;
        case 0x10u:
            v1 = "Nie mo\xBFna odnale\x9F\xE6\x20\x7Aainstalowanej gry."
                 " Powodem mo\xBFe by\xE6\x20\x6Eiepoprawna instalacja "
                 "gry b\xB9d\x9F b\xB3\xB9d systemu operacyjnego.";
            break;
        case 0x11u:
            v1 = "\xCE\xDE\xB7\xA8\xD5\xD2\xB5\xBD\xD3\xCEϷ\xB5\xB5\xB0"
                 "\xB8\xBCУ\xAC\xCB\xC6\xBA\xF5\xB7\xA2\xC9\xFA\xB0\xB2"
                 "װ\xCE\xCA\xCC\xE2\xBB\xF2\xD7\xF7\xD2\xB5ϵͳ\xB5\xC4\xCE\xCA\xCC⡣";
            break;
        case 0x12u:
            v1 = "\xA7䤣\xA8\xEC\xB9\x43\xC0\xB8\xC0\xC9\xAEק\xA8\xA1A\xA6"
                 "\xFC\xA5G\xB5o\xA5ͦw\xB8˰\xDD\xC3D\xA9Χ@\xB7~\xA8t\xB2"
                 "Ϊ\xBA\xB0\xDD\xC3D\xA1C";
            break;
        case 0x13u:
            v1 = "\xE4\xC1\xE8\xBE\xBA\xA2\xE9\xCD\xC1\xD9\xC5\xE0\xA1\xC1"
                 " \xBBѭ\xCB\xD2\xCD\xD2\xA8\xE0\xA1\xD4\xB4\xA8ҡ\xA1\xD2"
                 "\xC3\xB5Դ\xB5\xD1\xE9\xA7\xE4\xC1\xE8\xB6١\xB5\xE9\xCD"
                 "\xA7 \xCB\xC3\xD7\xCD\xC3\xD0\xBA\xBA\xBB\xAFԺѵԡ\xD2\xC3"
                 "\xC1\xD5\xBBѭ\xCB\xD2";
            break;
        case 0x14u:
            v1 = "\xB0\xD4\xC0\xD3\x20\xB5\xA5\xC0\xCC\xC5\xCD\xB8\xA6 "
                 "ã\xC0\xBB \xBC\xF6\x20\xBE\xF8\xBD\xC0\xB4ϴ\xD9\x2E \xBC"
                 "\xB3ġ\xB0\xA1 \xC0\xDF\xB8\xF8\xB5Ǿ\xFA\xB0ų\xAA OS\xBF"
                 "\xCD\x20\xB0\xFC\xB7õ\xC8\x20\xB9\xAE\xC1\xA6\xB0\xA1"
                 " \xC0\xD6\xBD\xC0\xB4ϴ\xD9\x2E";
            break;*/
        default:
            v1 = "Unable to find game data. This is likely an installation "
                 "problem "
                 "or operating system problem.";
            break;
    }
    cRZString title("SimCity 4");
    cRZString msg(v1);
    SC4OSMessageBoxAlert(msg);
}

int SC4AppStart(cRZCmdLine *cmdLine, bool a2) {
    cGZFramework *pFW;
    cSC4App *pApp;
    if (/*gbShouldUseFrameworkOverrideClasses*/false) {
        // never reached (see gbShouldUseFrameworkOverrideClasses definition)

        // in the game binary, this would replace the GZFramework instance
        // with an instance of an SC4Framework class inheriting GZFramework
        // while doing nothing that GZFramework doesn't already do. since
        // the code is never reached, I won't implement this tomfoolery.
    } else {
        pFW = new cGZFramework();
    }
    pApp = new cSC4App(SZ_DEFAULT_APP_TITLE);
    pApp->AddRef();
    cGZFramework::mpFramework = pFW;
    cGZFramework::mpApp = pApp;
    int rc = cGZFramework::Main(*cmdLine, a2);
    if (!gbAppResourcesInitializedOK) /*intialization failed :((((*/
        SC4DisplayResourceError(gbAppResourceErrorLanguage);
    return rc;
}
int SC4GamePause(bool a1, bool a2, bool a3, bool a4, bool a5) { return 0; }
void SC4AdjustPathSeparatorsForPlatform(const cRZString &in,
                                        cRZString &out) {
    out = in;
    std::replace(out.begin(), out.end(), '\\', '/');
}
