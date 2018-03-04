#include "stdafx.h"
#include "Menu.h"
#include "LanguageCollection.h"


namespace KDL
{
    namespace
    {
        constexpr auto c_wndClass = L"KDLMenuWndClass";
    }

    Menu::Menu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs)
        : _langs{ langs },
          _hMenu{ ::CreatePopupMenu() },
          _width{ 0 },
          _hHandlerWnd{ Create(hInstance, this) },
          _hOriginalWnd{ hWnd }
    {}

    Menu::~Menu()
    {
        ::DestroyWindow(_hHandlerWnd);
        ::DestroyMenu(_hMenu);
    }

    HWND Menu::Create(HINSTANCE hInstance, Menu* menu)
    {
        ::WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(::WNDCLASSEXW);
        wcex.style = 0;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = nullptr;
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = c_wndClass;
        wcex.hIconSm = nullptr;

        ::RegisterClassExW(&wcex);

        auto hWnd = ::CreateWindowW(
            c_wndClass, nullptr, WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd)
            return nullptr;

#ifdef _WIN64
        ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(menu));
#else
        ::SetWindowLongW(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(menu));
#endif

        return hWnd;
    }

    LRESULT CALLBACK Menu::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto menu =
#ifdef _WIN64
            reinterpret_cast<Menu*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
#else
            reinterpret_cast<Menu*>(::GetWindowLongW(hWnd, GWL_USERDATA));
#endif

        return menu
            ? menu->OnWndProc(message, wParam, lParam)
            : ::DefWindowProcW(hWnd, message, wParam, lParam);
    }

    LRESULT Menu::OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
            return OnCommand(wParam, lParam);

        case WM_DRAWITEM:
            OnDrawItem(reinterpret_cast<LPDRAWITEMSTRUCT>(lParam));
            break;

        case WM_MEASUREITEM:
            OnMeasureItem(reinterpret_cast<LPMEASUREITEMSTRUCT>(lParam));
            break;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        }

        return ::DefWindowProcW(_hOriginalWnd, message, wParam, lParam);
    }

    void Menu::OnDrawItem(LPDRAWITEMSTRUCT item)
    {
        if (item->CtlType != ODT_MENU)
            return;

        auto index = static_cast<size_t>(item->itemData);

        if (index < 0 || index >= _langs.GetSize())
            return;

        auto hDC = item->hDC;
        auto rc = item->rcItem;

        const bool bSelected = !!(item->itemState & ODS_SELECTED);
        const bool bGrayed = !!(item->itemState & ODS_GRAYED);

        const int bgColorIndex = bSelected ? COLOR_HIGHLIGHT : COLOR_MENU;
        const int txtColorIndex = bGrayed ? COLOR_GRAYTEXT : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT);

        ::FillRect(hDC, &rc, GetSysColorBrush(bgColorIndex));

        if (_langs.GetCurIndex() == index)
            ::DrawEdge(hDC, &rc, EDGE_SUNKEN, BF_RECT);

        ::SetTextColor(hDC, ::GetSysColor(txtColorIndex));

        const auto& lang = _langs[index];

        ::DrawIconEx(hDC, rc.left + 3, rc.top + 3, lang.GetIcon(), 16, 16, 0, nullptr, DI_NORMAL);

        rc.left += 25;

        ::SetBkMode(hDC, TRANSPARENT);

        auto hOldObject = ::SelectObject(hDC, ::GetStockObject(DEFAULT_GUI_FONT));
        ::DrawTextW(hDC, lang.GetName().c_str(), -1, &rc, DT_SINGLELINE | DT_VCENTER);
        ::SelectObject(hDC, hOldObject);
    }

    void Menu::OnMeasureItem(LPMEASUREITEMSTRUCT item)
    {
        if (item->CtlType = ODT_MENU)
        {
            item->itemWidth = _width + 25;
            item->itemHeight = 20;
        }
    }

    void Menu::RecalcWidth(bool all)
    {
        _width = 0;

        auto hdc = ::CreateCompatibleDC(nullptr);
        auto hOldObject = ::SelectObject(hdc, ::GetStockObject(DEFAULT_GUI_FONT));

        SIZE size;

        for (size_t i = 0; i < _langs.GetSize(); ++i)
        {
            const auto& lang = _langs[i];

            if (!all && !lang.IsCurrent())
                continue;

            const auto& str = lang.GetName();
            ::GetTextExtentPoint32W(hdc, str.c_str(), str.size(), &size);

            if (static_cast<UINT>(size.cx) > _width)
                _width = size.cx;
        }

        ::SelectObject(hdc, hOldObject);
        ::DeleteDC(hdc);
    }

    LRESULT Menu::OnCommand(WPARAM wParam, LPARAM lParam)
    {
        ::SendMessageW(_hOriginalWnd, WM_COMMAND, wParam, lParam);
        return 0;
    }

    void Menu::Popup()
    {
        POINT pt;
        ::GetCursorPos(&pt);

        ::SetForegroundWindow(_hHandlerWnd);
        ::TrackPopupMenu(_hMenu, TPM_RIGHTBUTTON | TPM_LEFTBUTTON, pt.x, pt.y, 0, _hHandlerWnd, nullptr);
        ::PostMessageW(_hHandlerWnd, WM_NULL, 0, 0);
    }

} // namesapce KDL
