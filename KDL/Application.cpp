#include "stdafx.h"
#include "Application.h"


namespace KDL
{
    Application::Application(HINSTANCE hInstance)
        : _wnd{ hInstance }
    {}

    int Application::Run()
    {
        return MessageLoop();
    }

    int Application::MessageLoop()
    {
        MSG msg;

        while (::GetMessageW(&msg, nullptr, 0, 0))
            ::DispatchMessageW(&msg);

        return 0;
    }

} // namesapce KDL
