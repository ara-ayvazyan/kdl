#pragma once

#include <array>
#include <string>


namespace KDL
{
    class Config;

    class Keyboard
    {
    public:
        Keyboard(const Config& config, const wchar_t* name);

        const std::wstring& GetName() const;
        DWORD operator[](BYTE vk) const;

    private:
        std::wstring _name;
        std::array<DWORD, 256> _map;
    };

} // namesapce KDL
