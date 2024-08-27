#pragma once

#include "PositiveTable.hpp"

namespace Filter
{
auto rmListsTooSmall(StringTable_t, uint64_t) -> StringTable_t;
auto OutDuplicates(StringTable_t&) -> StringTable_t;
auto OutDuplicates(StringList_t&) -> StringList_t;
auto NamesFromRules(StringList_t, StringList_t) -> StringList_t;
auto less_vectors(const std::vector<std::string>&,const std::vector<std::string>&) -> bool;
void OutLeaderlessLists(StringTable_t&, StringList_t);
};

namespace Sort
{
    auto LeaderFirst(StringTable_t&) -> void;
    auto LeaderSort(const std::string&,const std::string&) -> bool;
};
