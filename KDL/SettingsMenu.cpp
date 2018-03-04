#include "stdafx.h"
#include "SettingsMenu.h"
#include "LanguageCollection.h"


namespace KDL
{
    SettingsMenu::SettingsMenu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs)
        : Menu{ hInstance, hWnd, langs }
    {
        auto nLangs = _langs.GetSize();

        for (size_t i = 0; i < nLangs; ++i)
        {
            auto hKbds = ::CreatePopupMenu();

            const auto& lang = _langs[i];
            const auto count = lang.GetSize();

            for (size_t j = 0; j < count; ++j)
                ::AppendMenuW(hKbds, MF_STRING, MAKEWORD(j, i), lang[j].GetName().c_str());

            if (lang.IsCurrent())
                ::CheckMenuItem(hKbds, lang.GetCurIndex(), MF_BYPOSITION | MF_CHECKED);

            ::AppendMenuW(
                _hMenu,
                MF_OWNERDRAW | MF_POPUP | (count ? 0 : MF_DISABLED | MF_GRAYED),
                reinterpret_cast<UINT>(hKbds),
                reinterpret_cast<LPCWSTR>(i));
        }

        if (nLangs)
            ::AppendMenuW(_hMenu, MF_SEPARATOR, 0, nullptr);

        ::AppendMenuW(_hMenu, MF_STRING, MAKEWORD(-2, -2), L"&About KDL");
        ::AppendMenuW(_hMenu, MF_SEPARATOR, 0, nullptr);
        ::AppendMenuW(_hMenu, MF_STRING, MAKEWORD(-1, -1), L"E&xit");

        RecalcWidth();
    }

    SettingsMenu::~SettingsMenu()
    {
        size_t nLangs = _langs.GetSize();

        for (size_t i = 0; i < nLangs; ++i)
            ::DestroyMenu(::GetSubMenu(_hMenu, i));
    }

    bool SettingsMenu::CheckUncheckKeyboard(int langIndex, bool check)
    {
        if (langIndex < 0 || langIndex >= static_cast<int>(_langs.GetSize()))
            return false;

        ::CheckMenuItem(
            ::GetSubMenu(_hMenu, langIndex),
            _langs[langIndex].GetCurIndex(),
            MF_BYPOSITION | (check ? MF_CHECKED : MF_UNCHECKED));

        return true;
    }

} // namesapce KDL
