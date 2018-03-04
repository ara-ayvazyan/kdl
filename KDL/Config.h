#pragma once

#include <string>
#include <vector>


namespace KDL
{
    class Config
    {
    public:
        explicit Config(const std::wstring& path);

        std::wstring GetString(const wchar_t* section, const wchar_t* key) const;

        std::wstring GetPath(const wchar_t* section, const wchar_t* key) const;

        std::wstring GetSection(const wchar_t* section) const;

        std::wstring GetSectionNames() const;

        unsigned int GetInteger(const wchar_t* section, const wchar_t* key, int def = -1) const;

        void SetString(const wchar_t* section, const wchar_t* key, const wchar_t* value);

    private:
        template <typename GetFunc>
        std::wstring ReadContent(GetFunc&& getFunc, uint8_t offset) const;

        const std::wstring _path;
        mutable std::vector<wchar_t> _buffer;
    };

} // namesapce KDL
