#pragma once

#include <shellapi.h>


namespace KDL
{
    class NotifyIcon
    {
    public:
        NotifyIcon(HWND hWnd, UINT msg, HICON hIcon, const wchar_t* tip);
        ~NotifyIcon();

        void SetIcon(HICON hIcon, const wchar_t* tip);

    private:
        void ConfigIcon(::NOTIFYICONDATAW& tnid, HICON hIcon, const wchar_t* tip);

        HWND _hWnd;
        UINT _msg;
    };

} // namesapce KDL
