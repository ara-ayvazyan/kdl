#include "stdafx.h"
#include "MainWnd.h"
#include "Language.h"
#include "Keyboard.h"
#include "resource.h"


namespace KDL
{
    namespace
    {
        constexpr auto c_title = L"Keyboard Driver Light";
        constexpr auto c_wndClass = L"KDLWndClass";
        constexpr UINT MSG_MENU = WM_APP + 1;
    }

    MainWnd::MainWnd(HINSTANCE hInstance)
        : _hAppIcon{ ::LoadIconW(hInstance, MAKEINTRESOURCE(IDI_KDL)) },
          _hWnd{ Create(hInstance, _hAppIcon, this) },
          _icon{ _hWnd, MSG_MENU, nullptr, nullptr },
          _hotkeyId{ ::GlobalAddAtomW(c_wndClass) },
          _languageMenu{ hInstance, _hWnd, _langs },
          _settingsMenu{ hInstance, _hWnd, _langs },
          _hook{ hInstance }
    {
        Update();
        ::RegisterHotKey(_hWnd, _hotkeyId, MOD_ALT | MOD_SHIFT, 0);
    }

    HWND MainWnd::Create(HINSTANCE hInstance, HICON icon, MainWnd* wnd)
    {
        ::WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(::WNDCLASSEXW);
        wcex.style = 0;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = icon;
        wcex.hCursor = nullptr;
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = c_wndClass;
        wcex.hIconSm = wcex.hIcon;

        ::RegisterClassExW(&wcex);

        auto hWnd = ::CreateWindowW(
            c_wndClass, c_title, WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

        if (!hWnd)
            return nullptr;

#ifdef _WIN64
        ::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
#else
        ::SetWindowLongW(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(wnd));
#endif

        return hWnd;
    }

    LRESULT CALLBACK MainWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        auto wnd =
#ifdef _WIN64
            reinterpret_cast<MainWnd*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
#else
            reinterpret_cast<MainWnd*>(::GetWindowLongW(hWnd, GWL_USERDATA));
#endif

        return wnd
            ? wnd->OnWndProc(message, wParam, lParam)
            : ::DefWindowProcW(hWnd, message, wParam, lParam);
    }

    LRESULT MainWnd::OnWndProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case MSG_MENU:
            return OnNotifyIcon(wParam, lParam);

        case WM_COMMAND:
            return OnCommand(wParam, lParam);

        case WM_HOTKEY:
            if (SwitchLanguage())
                Update();
            break;

        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        }

        return ::DefWindowProcW(_hWnd, message, wParam, lParam);
    }

    LRESULT MainWnd::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
    {
        switch (lParam)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            switch (wParam)
            {
            case MSG_MENU:
                _languageMenu.Popup();
                break;
            }
            break;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
            switch (wParam)
            {
            case MSG_MENU:
                _settingsMenu.Popup();
                break;
            }
            break;
        }

        return 0;
    }

    BOOL CALLBACK MainWnd::AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM /*lParam*/)
    {
        switch (message)
        {
        case WM_INITDIALOG:
            return TRUE;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint(hDlg, &ps);
            ::DrawIconEx(ps.hdc, 8, 8, ::LoadIconW(::GetModuleHandleW(nullptr),
                MAKEINTRESOURCE(IDI_KDL)), 16, 16, 0, nullptr, DI_NORMAL);
            ::EndPaint(hDlg, &ps);
        }
        break;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                ::EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        }

        return FALSE;
    }

    LRESULT MainWnd::OnCommand(WPARAM wParam, LPARAM lParam)
    {
        WORD code = LOWORD(wParam);
        BYTE nLang = HIBYTE(code);
        BYTE nKbd = LOBYTE(code);

        if (nLang == 0xFF && nKbd == 0xFF)
        {
            OnClose();
            return 0;
        }

        if (nLang == 0xFE && nKbd == 0xFE)
        {
            ::DialogBoxW(nullptr, MAKEINTRESOURCE(IDD_ABOUT), nullptr, (DLGPROC)AboutProc);
            return 0;
        }

        if (nKbd == 0xFF ? SwitchLanguage(nLang) : SwitchKeyboard(nLang, nKbd))
            Update();

        return ::DefWindowProcW(_hWnd, WM_COMMAND, wParam, lParam);
    }

    void MainWnd::OnClose()
    {
        ::UnregisterHotKey(_hWnd, _hotkeyId);
        ::SendMessageW(_hWnd, WM_CLOSE, 0, 0);
    }

    bool MainWnd::SwitchLanguage(int index)
    {
        return index < 0
            ? _langs.MoveNext()
            : _langs.SetCurIndex(index != _langs.GetCurIndex() ? index : -1);
    }

    bool MainWnd::SwitchKeyboard(int langIndex, int kbdIndex)
    {
        if (langIndex < 0 || langIndex >= static_cast<int>(_langs.GetSize()))
            return false;

        auto& lang = _langs[langIndex];

        const int lastKbdIndex = lang.GetCurIndex();

        _settingsMenu.CheckUncheckKeyboard(langIndex, false);

        if (kbdIndex != lastKbdIndex)
        {
            lang.SetCurIndex(kbdIndex);
            _settingsMenu.CheckUncheckKeyboard(langIndex, true);

            if (lastKbdIndex < 0)
                _languageMenu.Insert(langIndex);
        }
        else
        {
            lang.SetCurIndex();

            if (lastKbdIndex >= 0)
                _languageMenu.Remove(langIndex);
        }

        return true;
    }

    void MainWnd::Update()
    {
        if (_langs.IsCurrent() && _langs.GetCurrent().IsCurrent())
        {
            _hook.SetKeyboard(&_langs.GetCurrent()[_langs.GetCurrent().GetCurIndex()]);
            _icon.SetIcon(_langs.GetCurrent().GetIcon(), _langs.GetCurrent().GetFullName().c_str());
        }
        else
        {
            _hook.SetKeyboard(nullptr);
            _icon.SetIcon(_hAppIcon, L"Keyboard Driver Light");
        }
    }

} // namesapce KDL
