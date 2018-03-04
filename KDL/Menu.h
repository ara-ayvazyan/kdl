#pragma once


namespace KDL
{
    class LanguageCollection;

    class Menu
    {
    public:
        Menu(HINSTANCE hInstance, HWND hWnd, const LanguageCollection& langs);
        ~Menu();

        void Popup();

    protected:
        void RecalcWidth(bool all = true);

        const LanguageCollection& _langs;
        HMENU _hMenu;
        HWND _hHandlerWnd;
        HWND _hOriginalWnd;

    private:
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        static HWND Create(HINSTANCE hInstance, Menu* menu);

        LRESULT OnWndProc(UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
        void OnDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        void OnMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

        UINT _width;
    };

} // namesapce KDL
