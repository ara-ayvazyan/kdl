#pragma once

#include "MainWnd.h"


namespace KDL
{
    class Application
    {
    public:
        explicit Application(HINSTANCE hInstance);

        int Run();

    private:
        static int MessageLoop();

        MainWnd	_wnd;
    };

} // namesapce KDL
