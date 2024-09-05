#include "FilterFunctions.hpp"
#include "names.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
extern bool isVerbose;

StringTable_t Filter::rmListsTooSmall(StringTable_t Names, uint64_t MinGroupSize)
{
    StringTable_t tempTable;
    for(const auto& list : Names)
    {
        if(list.size() >= MinGroupSize)
        {
            tempTable.push_back(list);
        }
    }
    return tempTable;
}

StringTable_t Filter::OutDuplicates(StringTable_t& Table)
{
    for(auto& List : Table)
    {
        OutDuplicates(List);
    }
    if(isVerbose) std::cout << std::endl << "Removing Duplicates " << std::endl;

    std::sort(Table.begin(), Table.end()); 
    auto it = std::unique(Table.begin(), Table.end()); 
    Table.erase(it, Table.end()); 
    return Table;
}

StringList_t Filter::OutDuplicates(StringList_t& List)
{
    std::sort(List.begin(), List.end());
    List.erase(std::unique(List.begin(), List.end()), List.end());
    return List;
}

StringList_t Filter::NamesFromRules(StringList_t NameList, StringList_t RuleSet)
{
    StringList_t FilteredRules;
    for(auto& Name : NameList)
    {
        if(std::find(RuleSet.begin(), RuleSet.end(), Name) != RuleSet.end())
        {
            FilteredRules.push_back(Name);
        }
    }
    return FilteredRules;
}

void Filter::OutLeaderlessLists(StringTable_t& NamesTable, StringList_t LeaderList)
{

    if(LeaderList.size() <= 0)
    {
        return;
    }

    StringTable_t NewTable;

    for(auto List : NamesTable)
    {
        StringList_t TempLeaderList;
        for(auto Name : LeaderList)
        {
            bool isLeader = std::find(List.begin(), List.end(), Name) != List.end();
            if(isLeader)
            {
                TempLeaderList.push_back(Name);
            }
        }
        if(TempLeaderList.size() > 0)
        {
            NewTable.push_back(List);
        }
    }
    NamesTable = NewTable;
}

bool less_vectors(const std::vector<std::string>& a ,const std::vector<std::string>& b) {
   return a.size() < b.size();
}

// Comparator function that prioritizes leaders
bool Sort::LeaderSort(const std::string& a, const std::string& b) {
    auto is_a_leader = std::find(names::Leaders.begin(), names::Leaders.end(), a) != names::Leaders.end();
    auto is_b_leader = std::find(names::Leaders.begin(), names::Leaders.end(), b) != names::Leaders.end();

    // Leaders should appear first
    if (is_a_leader && !is_b_leader) return true;
    if (!is_a_leader && is_b_leader) return false;

    // If both are leaders or both are non-leaders, sort alphabetically
    return a < b;
}

// Sorts each list in the table with leaders first, then sorts the table itself
void Sort::LeaderFirst(StringTable_t& Table) {
    for (auto& List : Table) {
        std::sort(List.begin(), List.end(), LeaderSort);
    }

    std::sort(Table.begin(), Table.end());
}

