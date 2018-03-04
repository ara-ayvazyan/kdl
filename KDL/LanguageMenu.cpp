#include "stdafx.h"
#include "LanguageMenu.h"
#include "LanguageCollection.h"


namespace KDL
{
    LanguageMenu::LanguageMenu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs)
        : Menu{ hInstance, hWnd, langs }
    {
        size_t nLangs = _langs.GetSize();

        for (size_t i = 0; i < nLangs; ++i)
            if (_langs[i].IsCurrent())
                ::AppendMenuW(_hMenu, MF_OWNERDRAW, MAKEWORD(-1, i), reinterpret_cast<LPCWSTR>(i));

        RecalcWidth(false);
    }

    bool LanguageMenu::Remove(int langIndex)
    {
        if (::RemoveMenu(_hMenu, MAKEWORD(-1, langIndex), MF_BYCOMMAND))
        {
            RecalcWidth(false);
            return true;
        }

        return false;
    }

    bool LanguageMenu::Insert(int langIndex)
    {
        if (langIndex < 0 || langIndex >= static_cast<int>(_langs.GetSize()))
            return false;

        auto nItems = ::GetMenuItemCount(_hMenu);
        int pos = 0;

        while (pos < nItems && HIBYTE(LOWORD(::GetMenuItemID(_hMenu, pos))) <= langIndex)
            ++pos;

        if (::InsertMenuW(
                _hMenu,
                pos,
                MF_OWNERDRAW | MF_BYPOSITION,
                MAKEWORD(-1, langIndex),
                reinterpret_cast<LPCWSTR>(langIndex)))
        {
            RecalcWidth(false);
            return true;
        }

        return false;
    }

} // namesapce KDL
