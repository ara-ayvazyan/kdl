#pragma once


namespace KDL
{
    class Keyboard;

    class Hook
    {
    public:
        Hook();
        ~Hook();

        void SetKeyboard(const Keyboard* kbd);

    private:
        HHOOK _hook;
    };

} // namesapce KDL
