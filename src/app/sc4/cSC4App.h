//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSC4APP_H
#define SCION_CSC4APP_H


#include "cIGZApp.h"
#include "cIGZMessage2.h"
#include "cIGZBuffer.h"
#include "cGZApp.h"
#include "cIGZMessageTarget.h"
#include "cRZTimer.h"
#include "cRZMutex.h"
#include "cRZString.h"

#include <list>
#include <map>

struct FileChangeNotification {
    cRZString field_0;
    int field_C{0}, field_10{0}, field_14{0}, field_18{0}, field_1C{0},
            field_20{0};
    inline bool IsStarted() {
        return false;
    }   // lmao (inlining this always-false func to make things run faster)
    inline bool Stop() { return true; }
    inline bool Shutdown() { return true; }
};

    struct cSC4TutorialRegistry {
        struct cItem {};
        std::map<cRZString, std::list<cItem>> field_0;
        std::map<long, cRZString> field_18;
        int field_30{0};
    };
    struct SC4AudioPreferences {
        uint32_t field_0{1}, field_4{1024}, field_8{800}, field_C{1};
        uint32_t field_10{0}, field_14{1}, field_18{1}, field_1C{1};
        uint32_t field_20{1}, field_24{1};
        uint8_t field_28[30];
        uint8_t field_46[30];
        bool field_64{false}, field_65{true}, field_66{true};
        SC4AudioPreferences() {
            memset(field_28, 1, sizeof(field_28));
            memset(field_46, 1, sizeof(field_46));
        }
    };
    struct SC4NewCityPreferences {
        uint8_t field_0[0x200]{};
        uint8_t field_200[0x200]{};
        int field_400{0};
    };
    struct SC4DebugPreferences {
        bool field_0{false};
        uint8_t field_4[0x10]{};
        uint8_t field_14[0x10]{};
        int field_24{0};
    };
    struct SC4AppPreferences {
        bool bPauseOnAppFocusLoss{false};
        int field_4{1};
        uint8_t field_8[64]{};
        bool field_48{true}, field_49{true}, field_4A{false}, field_4B{true};
        uint8_t field_4C[2400]{};
    };
    struct SC4TutorialPreferences2 {
        uint8_t field_0[0x400]{};
        uint8_t field_400[0x100]{};
        uint8_t field_500[0x300]{};
    };
    struct cSC4Preferences {
        bool field_0{false}, field_1{false};
        SC4AudioPreferences m_AudioPrefs{};
        SC4NewCityPreferences m_NewCityPrefs{};
        float field_470{50.0};
        SC4DebugPreferences m_DebugPrefs{};
        bool field_49C{true};
        int field_4A0{0};
        SC4AppPreferences m_AppPrefs{};
        int field_E50{0}, field_E54{0};
        uint8_t field_E58[100]{};
        uint8_t field_EBC[64]{};
        uint32_t field_EFC{0x80000000};
        uint32_t field_F00{0x80000000};
        bool field_F04{true}, field_F05{true}, gap_F06, gap_F07;
        uint8_t field_F08{1}, field_F09{1}, field_F0A{1}, field_F0B{1},
                field_F0C{1}, field_F0D{3}, field_F0E{3}, field_F0F{3}, field_F10{3},
                field_F11{1}, field_F12{1}, field_F13{1}, field_F14{1}, field_F15{1},
                field_F16{3}, field_F17{1}, field_F18{1};
        int nScreenWidth{1024}, nScreenHeight{768};
        int field_F24{0}, field_F28{3}, field_F2C{0}, field_F30{0};
        SC4TutorialPreferences2 m_TutorialPrefs;
    };
    static_assert(__builtin_offsetof(cSC4Preferences, field_F08) == 0xF08);
    static_assert(sizeof(cSC4Preferences) == 0x1734u);
    struct SC4NewCitySpecification {
        uint8_t field_0[0x200]{};
        cIGZBuffer *field_200{NULL};
        int field_204{0};

        SC4NewCitySpecification() { memset(field_0, 0, sizeof(field_0)); }
    };
    struct cSC4App : public cGZApp,
                     public cIGZMessageTarget {
        int field_3C{0}, field_40{0}, field_44{0}, field_48{0}, field_4C{0},
                field_50{0}, field_54{0}, field_58{0};
        cRZString field_5C, field_68, field_74, field_80, field_8C, field_98,
                field_A4, field_B0, field_BC, field_C8, field_D4, field_E0, field_EC,
                field_F8, field_104, field_110, field_11C, field_128;
        cSC4TutorialRegistry m_TutorialRegistry;
        int field_168{0}, field_16C{0}, field_170{0};
        cRZTimer m_Timer1{cRZTimer::Units::TIMER_MUL_60000};
        int field_188{30};
        cRZTimer m_Timer2{cRZTimer::Units::TIMER_MUL_60000};
        int field_1A0{5};
        cRZTimer m_Timer3{cRZTimer::Units::TIMER_UNUSED_MAYBE1};
        cRZTimer m_Timer4{cRZTimer::Units::TIMER_MUL_1000};
        cRZTimer m_Timer5{cRZTimer::Units::TIMER_MUL_60000};
        int field_1E0{30}, field_1E4{5};
        cSC4Preferences *m_Prefs{NULL};
        SC4NewCitySpecification m_NewCitySpecs;
        int field_3F4{0}, field_3F8{0}, field_3FC{0}, field_400{0};
        typedef bool (*ShutdownCallback)(void *);
        struct ShutdownCallbackData {
            ShutdownCallback callback;
            void *userData;
        };
        std::list<ShutdownCallbackData> m_ShutdownCallbackData;
        bool m_bPauseOnAppFocusLoss{false}, m_bFullGamePaused{false};
        FileChangeNotification m_FCN, m_FCN2;
        int field_458{0}, field_45C{0};
        /* cSC4ConfigRuleManager */ void *m_Config{NULL};
        int field_464{0}, field_468{0}, field_46C{0}, field_470{0};
        cRZMutex m_Mutex{cRZString(SZ_APP_MUTEX)};
        bool field_47C{false}, field_47D{true};
        int field_480{0}, field_484{0}, field_488{0}, field_48C{0}, field_490{0},
                field_494{0};
        cSC4App(const char *szName);

        uint32_t AddRef() override;
        uint32_t Release() override;
        bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
        virtual bool RegisterShutdownCallbackFunction(ShutdownCallback fpCb,
                                                      void *pvUserData);
        virtual bool Init();
        bool PreFrameWorkShutDown();
        void CloseCity();
        virtual void ShutdownWinCtrlMgr();
        void SavePreferences();
        void ReadPreferences();
        void SetFontAlias(const cRZString &a2, const cRZString &a3);
        static void SetFontAliasStatic(const cRZString &a1,
                                       const cRZString &a2, void *a3);
        void SetFontStyle(const cRZString &a2, const cRZString &a3);
        static void SetFontStyleStatic(const cRZString &a1,
                                       const cRZString &a2, void *a3);

        void EnableFullGamePauseOnAppFocusLoss(bool v);
        void GainLoseFocus(bool v);
        void FullGamePause(bool v);
        void GetAppPreferencesFilePath(cRZString &dest);
        void GetAppPreferencesFileName(cRZString &dest);
        bool AddCOMDirectors();

        bool DoMessage(const cGZMessage &msg) override;
    };


#endif //SCION_CSC4APP_H
