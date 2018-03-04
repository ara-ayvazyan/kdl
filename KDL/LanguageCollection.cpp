#include "stdafx.h"
#include "LanguageCollection.h"


namespace KDL
{
    LanguageCollection::LanguageCollection()
        : _config{ L"KDL.ini" },
          _index{ static_cast<int>(_config.GetInteger(L"Settings", L"Current")) }
    {
        auto section = _config.GetSection(L"Languages");

        for (auto path = section.data(); *path; path += std::wcslen(path) + 1)
        {
            _langs.emplace_back(path);
        }
    }

    size_t LanguageCollection::GetSize() const
    {
        return _langs.size();
    }

    const Language& LanguageCollection::operator[](int index) const
    {
        return _langs[index];
    }

    Language& LanguageCollection::operator[](int index)
    {
        return _langs[index];
    }

    bool LanguageCollection::IsCurrent() const
    {
        return _index >= 0 && _index < static_cast<int>(_langs.size());
    }

    const Language& LanguageCollection::GetCurrent() const
    {
        return _langs[_index];
    }

    int LanguageCollection::GetCurIndex() const
    {
        return _index;
    }

    bool LanguageCollection::SetCurIndex(int index)
    {
        if (index >= static_cast<int>(_langs.size()) || !_langs[index].IsCurrent())
            return false;

        _index = index;
        SaveCurrent();

        return true;
    }

    bool LanguageCollection::MoveNext()
    {
        const size_t size = _langs.size();

        switch (size)
        {
        case 0:
            return false;

        case 1:
            _index = 0;
            SaveCurrent();
            return _langs[_index].GetCurIndex() >= 0;
        };

        const int lastIndex = _index;
        while ((_index = (_index + 1) % size) != lastIndex && _langs[_index].GetCurIndex() < 0);

        if (_index == lastIndex)
            return false;

        SaveCurrent();

        return true;
    }

    void LanguageCollection::SaveCurrent()
    {
        _config.SetString(L"Settings", L"Current", std::to_wstring(_index).c_str());
    }

} // namesapce KDL
