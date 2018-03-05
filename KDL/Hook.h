#pragma once


namespace KDL
{
    class Keyboard;

    class Hook
    {
    public:
        explicit Hook(HINSTANCE hInstance);
        ~Hook();

        void SetKeyboard(const Keyboard* kbd);

    private:
        HHOOK _hook;
    };

} // namesapce KDL
