// FKDLight.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Application.h"


int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPTSTR    lpCmdLine,
                     int       /*nCmdShow*/)
{
    return ::FindWindowW(L"KDLWndClass", nullptr)
        ? 1
        : KDL::Application{ hInstance }.Run();
}
