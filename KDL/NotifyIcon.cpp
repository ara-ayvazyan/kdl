#include "stdafx.h"
#include "NotifyIcon.h"


namespace KDL
{
    NotifyIcon::NotifyIcon(HWND hWnd, UINT msg, HICON hIcon, const wchar_t* tip)
        : _hWnd{ hWnd },
          _msg{ msg }
    {
        ::NOTIFYICONDATAW tnid;

        ConfigIcon(tnid, hIcon, tip);
        ::Shell_NotifyIconW(NIM_ADD, &tnid);
    }

    NotifyIcon::~NotifyIcon()
    {
        ::NOTIFYICONDATAW tnid;

        tnid.cbSize = sizeof(::NOTIFYICONDATAW);
        tnid.hWnd = _hWnd;
        tnid.uID = _msg;

        ::Shell_NotifyIconW(NIM_DELETE, &tnid);
    }

    void NotifyIcon::ConfigIcon(::NOTIFYICONDATAW& tnid, HICON hIcon, const wchar_t* tip)
    {
        tnid.cbSize = sizeof(::NOTIFYICONDATAW);
        tnid.hWnd = _hWnd;
        tnid.uID = _msg;
        tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        tnid.uCallbackMessage = _msg;
        tnid.hIcon = hIcon;

        if (tip)
            wcscpy_s(tnid.szTip, tip);
        else
            tnid.szTip[0] = L'\0';
    }

    void NotifyIcon::SetIcon(HICON hIcon, const wchar_t* tip)
    {
        ::NOTIFYICONDATAW tnid;

        ConfigIcon(tnid, hIcon, tip);
        ::Shell_NotifyIconW(NIM_MODIFY, &tnid);
    }

} // namesapce KDL
