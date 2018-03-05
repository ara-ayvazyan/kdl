#include "stdafx.h"
#include "Hook.h"
#include "Keyboard.h"


namespace KDL
{
    namespace
    {
        const Keyboard* g_kbd;

        HRESULT CALLBACK OnKeyboardLL(int nCode, WPARAM wParam, LPARAM lParam)
        {
            auto info = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);
            DWORD value;
            bool down;

            if (nCode != HC_ACTION
                || !info
                || !g_kbd
                || (!(down = (wParam == WM_KEYDOWN)) && wParam != WM_KEYUP)
                || !!(info->flags & LLKHF_EXTENDED)
                || !!(HIBYTE(::GetKeyState(VK_CONTROL)) & 0x1)
                || !!(HIBYTE(::GetKeyState(VK_LWIN)) & 0x1)
                || !!(HIBYTE(::GetKeyState(VK_RWIN)) & 0x1)
                || !(value = (*g_kbd)[static_cast<BYTE>(info->vkCode)]))
                return ::CallNextHookEx(nullptr, nCode, wParam, lParam);

            if (down)
            {
                const bool capsLockOn = !!(LOBYTE(::GetKeyState(VK_CAPITAL)) & 0x1);
                const bool shiftOn = !!(HIBYTE(::GetKeyState(VK_SHIFT)) & 0x1);
                const bool uppercase = capsLockOn ^ shiftOn;
                const auto character = uppercase ? HIWORD(value) : LOWORD(value);

                INPUT input[2];
                input[0].type = INPUT_KEYBOARD;
                input[0].ki.wVk = 0;
                input[0].ki.wScan = character;
                input[0].ki.dwFlags = info->flags | KEYEVENTF_UNICODE;
                input[0].ki.time = info->time;
                input[0].ki.dwExtraInfo = info->dwExtraInfo;
                input[1] = input[0];
                input[1].ki.dwFlags = info->flags | KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                ::SendInput(2, input, sizeof(INPUT));
            }

            return 1;
        }

    } // anonymous

    Hook::Hook(HINSTANCE hInstance)
        : _hook{ ::SetWindowsHookExW(WH_KEYBOARD_LL, OnKeyboardLL, hInstance, 0) }
    {}

    void Hook::SetKeyboard(const Keyboard* kbd)
    {
        g_kbd = kbd;
    }

    Hook::~Hook()
    {
        g_kbd = nullptr;

        if (_hook)
        {
            ::UnhookWindowsHookEx(_hook);
        }
    }

} // namesapce KDL
