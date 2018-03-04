#include "stdafx.h"
#include "Language.h"
#include <sstream>


namespace KDL
{
    Language::Language(const std::wstring& path)
        : _config{ path },
          _index{ static_cast<int>(_config.GetInteger(L"Settings", L"Current")) },
          _name{ _config.GetString(L"Settings", L"Name") },
          _icon{
            static_cast<HICON>(::LoadImageW(
                nullptr,
                _config.GetPath(L"Settings", L"Icon").c_str(),
                IMAGE_ICON,
                0,
                0,
                LR_DEFAULTSIZE | LR_LOADFROMFILE)),
            &::DestroyIcon }
    {
        auto sections = _config.GetSectionNames();

        for (auto key = sections.data(); *key; key += std::wcslen(key) + 1)
        {
            if (_wcsicmp(key, L"Settings") != 0)
            {
                _kbds.emplace_back(_config, key);
            }
        }
    }

    const std::wstring& Language::GetName() const
    {
        return _name;
    }

    std::wstring Language::GetFullName() const
    {
        std::wostringstream text;

        if (IsCurrent())
        {
            text << L"KDL - " << _name;

            const auto& name = _kbds[_index].GetName();

            if (!name.empty())
            {
                text << L" (" << name << L')';
            }
        }

        return text.str();
    }

    const Keyboard& Language::operator[](int index) const
    {
        return _kbds[index];
    }

    size_t Language::GetSize() const
    {
        return _kbds.size();
    }

    bool Language::IsCurrent() const
    {
        return _index >= 0 && _index < static_cast<int>(_kbds.size());
    }

    int Language::GetCurIndex() const
    {
        return _index;
    }

    void Language::SetCurIndex(int index)
    {
        _index = index < static_cast<int>(_kbds.size()) ? index : -1;
        _config.SetString(L"Settings", L"Current", std::to_wstring(_index).c_str());
    }

    HICON Language::GetIcon() const
    {
        return _icon.get();
    }

} // namesapce KDL
