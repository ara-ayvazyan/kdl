#pragma once

#include "Menu.h"


namespace KDL
{
    class SettingsMenu : public Menu
    {
    public:
        SettingsMenu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs);
        ~SettingsMenu();

        bool CheckUncheckKeyboard(int langIndex, bool check);
    };

} // namesapce KDL
