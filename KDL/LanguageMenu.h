#pragma once

#include "Menu.h"


namespace KDL
{
    class LanguageMenu : public Menu
    {
    public:
        LanguageMenu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs);

        bool Remove(int langIndex);
        bool Insert(int langIndex);
    };

} // namesapce KDL
