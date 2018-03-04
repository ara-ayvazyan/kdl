#include "stdafx.h"
#include "Keyboard.h"
#include "Config.h"


namespace KDL
{
    Keyboard::Keyboard(const Config& config, const wchar_t* name)
        : _name{ name },
          _map{ 0 }
    {
        auto keys = config.GetString(_name.c_str(), nullptr);

        for (auto key = keys.data(); *key; key += std::wcslen(key) + 1)
        {
            auto value = config.GetString(_name.c_str(), key);

            BYTE vk = static_cast<BYTE>(std::wcstoul(key, nullptr, 16));

            auto comma = std::wcschr(value.c_str(), L',');
            WORD low = static_cast<WORD>(std::wcstoul(value.c_str(), nullptr, 16));
            WORD high = comma ? static_cast<WORD>(std::wcstoul(comma + 1, nullptr, 16)) : 0;

            _map[vk] = MAKELONG(low, high);
        }
    }

    const std::wstring& Keyboard::GetName() const
    {
        return _name;
    }

    DWORD Keyboard::operator[](BYTE vk) const
    {
        return _map[vk];
    }

} // namesapce KDL
