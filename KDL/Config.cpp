#include "stdafx.h"
#include "Config.h"
#include <shlobj.h>


namespace KDL
{
    namespace
    {
        std::wstring GetAppFolder()
        {
            wchar_t path[MAX_PATH];
            ::SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, path);
            return path;
        }

#ifndef NDEBUG
        const std::wstring g_appFolder = L".\\";
#else
        const std::wstring g_appFolder = GetAppFolder() + L"\\KDL\\";
#endif

    } // anonymous

    Config::Config(const std::wstring& path)
        : _path{ g_appFolder + path },
          _buffer(MAX_PATH)
    {}

    template <typename GetFunc>
    std::wstring Config::ReadContent(GetFunc&& getFunc, uint8_t offset) const
    {
        auto size = getFunc();

        if (_buffer.size() < offset)
        {
            _buffer.resize(offset);
        }

        while (size == _buffer.size() - offset)
        {
            _buffer.resize(_buffer.size() * 2);
            size = getFunc();
        }

        return std::wstring{ _buffer.data(), size };
    }

    std::wstring Config::GetString(const wchar_t* section, const wchar_t* key) const
    {
        return ReadContent([&]
            {
                return ::GetPrivateProfileStringW(section, key, nullptr, _buffer.data(), _buffer.size(), _path.c_str());
            }, 1);
    }

    std::wstring Config::GetPath(const wchar_t* section, const wchar_t* key) const
    {
        std::wstring path = _path;

        auto pos = path.find_last_of('\\');
        if (pos != std::wstring::npos)
        {
            path.erase(pos + 1);
        }

        return path + GetString(section, key);
    }

    std::wstring Config::GetSection(const wchar_t* section) const
    {
        return ReadContent([&]
            {
                return ::GetPrivateProfileSectionW(section, _buffer.data(), _buffer.size(), _path.c_str());
            }, 2);
    }

    std::wstring Config::GetSectionNames() const
    {
        return ReadContent([&]
            {
                return ::GetPrivateProfileSectionNamesW(_buffer.data(), _buffer.size(), _path.c_str());
            }, 2);
    }

    unsigned int Config::GetInteger(const wchar_t* section, const wchar_t* key, int def) const
    {
        return ::GetPrivateProfileIntW(section, key, def, _path.c_str());
    }

    void Config::SetString(const wchar_t* section, const wchar_t* key, const wchar_t* value)
    {
        ::WritePrivateProfileStringW(section, key, value, _path.c_str());
    }

} // namesapce KDL
