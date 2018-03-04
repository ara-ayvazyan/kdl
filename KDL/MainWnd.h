#pragma once

#include "NotifyIcon.h"
#include "LanguageCollection.h"
#include "LanguageMenu.h"
#include "SettingsMenu.h"
#include "Hook.h"


namespace KDL
{
    class MainWnd
    {
    public:
        MainWnd(HINSTANCE hInstance);

    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        static BOOL CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

        static HWND Create(HINSTANCE hInstance, HICON icon, MainWnd* wnd);

        LRESULT OnWndProc(UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
        LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
        void OnClose();

        bool SwitchLanguage(int index = -1);
        bool SwitchKeyboard(int langIndex, int kbdIndex);
        void Update();

        HICON _hAppIcon;
        HWND _hWnd;
        NotifyIcon _icon;
        ATOM _hotkeyId;
        LanguageCollection _langs;
        LanguageMenu _languageMenu;
        SettingsMenu _settingsMenu;
        Hook _hook;
    };

} // namesapce KDL
