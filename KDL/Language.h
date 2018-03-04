#pragma once

#include "Keyboard.h"
#include "Config.h"
#include <vector>
#include <string>
#include <memory>


namespace KDL
{
    class Language
    {
    public:
        explicit Language(const std::wstring& path);

        const std::wstring& GetName() const;
        std::wstring GetFullName() const;

        const Keyboard& operator[](int index) const;

        size_t GetSize() const;

        bool IsCurrent() const;

        int GetCurIndex() const;
        void SetCurIndex(int index = -1);

        HICON GetIcon() const;

    private:
        Config _config;
        int _index;
        const std::wstring _name;
        std::unique_ptr<std::remove_pointer_t<HICON>, decltype(&::DestroyIcon)> _icon;
        std::vector<Keyboard> _kbds;
    };

} // namesapce KDL
