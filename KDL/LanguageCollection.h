#pragma once

#include "Language.h"
#include <vector>


namespace KDL
{
    class LanguageCollection
    {
    public:
        LanguageCollection();

        size_t GetSize() const;

        const Language& operator[](int index) const;
        Language& operator[](int index);

        bool IsCurrent() const;

        const Language& GetCurrent() const;

        int GetCurIndex() const;
        bool SetCurIndex(int index);

        bool MoveNext();

    private:
        void SaveCurrent();

        Config _config;
        int _index;
        std::vector<Language> _langs;
    };

} // namesapce KDL
