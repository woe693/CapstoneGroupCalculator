#include "PositiveTable.hpp"
#include <iostream>
#include "FilterFunctions.hpp"
#include <algorithm>
#include <unordered_set>
#include <string>
#include <cmath>

StringTable_t CapstoneGroupCalc::Generate::Positive::Table(StringTable_t& Names, StringTable_t& Rules, uint64_t& MinGroupSize)
{
    std::cout << " GeneratePositiveTable " << std::endl;
    StringTable_t newTable = Names;

    for (const auto Rule : Rules)
    {
        StringTable_t tempTable;
        for (auto list : newTable)
        {
            tempTable.push_back(List(list, Rule, MinGroupSize));
        }

        if(Rule.size() >= MinGroupSize)
        {
            tempTable.push_back(Rule);
        }
        newTable = std::move(tempTable);
    }
    
    newTable = Filter::rmListsTooSmall(newTable,MinGroupSize);
    Filter::OutDuplicates(newTable);
    return newTable;
}

StringList_t CapstoneGroupCalc::Generate::Positive::List(StringList_t Names, StringList_t Rule, uint64_t MinGroupSize) {
    
    if(Names.empty() || Rule.empty()) {return {{}};}

    // TempList will store names that either are not in the Rule or match the entire Rule
    StringList_t NamesThatDONTfollowRules;

    // NamesInListThatDoNotFollowTheRule will store names that are in both Names and Rule
    StringList_t NamesThatFollowRules;

    //Serperate the Names into two list 
    for(auto& Name : Names)
    {
        std::find(Rule.begin(),Rule.end(),Name) != Rule.end() ? NamesThatFollowRules.push_back(Name) : NamesThatDONTfollowRules.push_back(Name);
    }

    if(NamesThatFollowRules.size() > MinGroupSize)
    {
        return Rule;
    }

    if(NamesThatFollowRules.size() == Rule.size() || NamesThatFollowRules.size() == 0)
    {
        return Names;
    }

    // Return the original Names list if no other condition is met
    return NamesThatDONTfollowRules;
}

StringTable_t CapstoneGroupCalc::Generate::FilteredGroupTable(StringTable_t Names, StringTable_t negativeRules, StringTable_t positiveRules, StringList_t Leaders, uint64_t MinGroupSize)
{
    std::cout << " Generating Filtered Group Table " << std::endl;

    auto Table = Names;
    std::cout << " Generating Lists With Negative Rule Sets " << std::endl;
    for (auto RuleSet : negativeRules){
        Table = CapstoneGroupCalc::Generate::Negative::Table(Table, RuleSet);
    }

    // Print::Names(Table);
    Table = CapstoneGroupCalc::Generate::Positive::Table(Table,positiveRules, MinGroupSize); 

    Filter::OutDuplicates(Table);

    Filter::OutLeaderlessLists(Table,Leaders);
    return Table;
}


StringTable_t CapstoneGroupCalc::Generate::PossibleConfigurations(StringList_t List)
{
    std::vector<bool> select(List.size(), false);
    std::fill(select.begin(), select.begin() + 4, true);

    StringTable_t variations;

    do {
        std::vector<std::string> group;
        for (int i = 0; i < List.size(); ++i) {
            if (select[i]) {
                group.push_back(List[i]);
            }
        }
        variations.push_back(group);
    } while (std::prev_permutation(select.begin(), select.end()));
    return variations;
}

StringMatrix_t CapstoneGroupCalc::Generate::PossibleConfigurations(StringTable_t List, uint64_t& MaxNumberofGroups) {
    std::vector<bool> select(List.size(), false);
    std::fill(select.begin(), select.begin() + MaxNumberofGroups, true);

    StringMatrix_t variations;

    do {
        StringTable_t group;
        for (int i = 0; i < List.size(); ++i) {
            if (select[i]) {
                group.push_back(List[i]);
            }
        }
        variations.push_back(group);
    } while (std::prev_permutation(select.begin(), select.end()));

    StringMatrix_t LegalVariations;

    for (const auto& Table : variations) {
        StringList_t TempList;
        for (const auto& Group : Table) {
            TempList.insert(TempList.end(), Group.begin(), Group.end());
        }

        // Check for duplicates by using an unordered_set
        std::unordered_set<std::string> uniqueNames(TempList.begin(), TempList.end());

        // If no duplicates were removed, the set size should match the TempList size
        if (uniqueNames.size() == TempList.size()) {
            LegalVariations.push_back(Table);
        }
    }

    return LegalVariations;
}


StringTable_t CapstoneGroupCalc::Generate::PossibleConfigurations(StringTable_t& Table, StringList_t PeopleInLargePositiveGroups) {
    std::cout << " Generate Possible Configurations " << std::endl;
    StringTable_t NewTable;
    
    for (auto& List : Table) {
        // Check if any name in List is in PeopleInLargePositiveGroups
        bool ShouldIgnoreList = std::any_of(List.begin(), List.end(), [&](const std::string& Name) {
            return std::find(PeopleInLargePositiveGroups.begin(), 
                             PeopleInLargePositiveGroups.end(), 
                             Name) != PeopleInLargePositiveGroups.end();
        });

        // If the list should be ignored, skip to the next one
        if (ShouldIgnoreList)
        {
            NewTable.push_back(List);
            continue;
        }

        // Generate configurations for the current List (recursive call)
        StringTable_t tempTable = Generate::PossibleConfigurations(List);
        NewTable.insert(NewTable.end(), tempTable.begin(), tempTable.end());
    }

    // Remove duplicates from the final table
    Filter::OutDuplicates(NewTable);

    return NewTable;
}

StringTable_t CapstoneGroupCalc::Generate::Negative::Table(StringTable_t NameTable, StringList_t RuleSet)
{
    StringTable_t NewTable;

    for (auto NameList : NameTable)
    {
        auto addendumTable = Generate::Negative::Table(NameList, RuleSet);
        NewTable.insert(NewTable.end(), addendumTable.begin(), addendumTable.end());
    }
    return NewTable;
}

StringTable_t CapstoneGroupCalc::Generate::Negative::Table(StringList_t NameList, StringList_t RuleSet)
{
    auto FilteredRules = Filter::NamesFromRules(NameList, RuleSet);
    StringTable_t NewTable;

    for (auto RuleName : FilteredRules)
    {
        StringList_t NewList = InverseList(NameList, FilteredRules);
        NewList.push_back(RuleName);
        std::sort(NewList.begin(), NewList.end());
        NewTable.push_back(NewList);
    }

    NewTable.push_back(InverseList(NameList, FilteredRules));

    return NewTable;
}

StringList_t CapstoneGroupCalc::Generate::Negative::InverseList(StringList_t NameList, StringList_t RuleSet)
{
    StringList_t InverseList;
    StringList_t FilteredRules;

    FilteredRules = Filter::NamesFromRules(NameList, RuleSet);

    if (FilteredRules.size() < 1)
    {
        return NameList;
    }

    for (auto& Name : NameList)
    {
        bool ShouldIncludeName = std::find(FilteredRules.begin(), FilteredRules.end(), Name) != FilteredRules.end();
        if (!ShouldIncludeName)
        {
            InverseList.push_back(Name);
        }
    }
    return InverseList;
}

StringList_t CapstoneGroupCalc::Generate::InitializePeopleInLargeGroups(StringTable_t PositiveRuleSets, uint64_t MinGroupSize)
{
    std::cout << " Initializing People In Large Groups " << std::endl;
    StringList_t PeopleInLargePositiveGroups;

    for(auto& Rule : PositiveRuleSets)
    {
        if(Rule.size() < MinGroupSize) continue;
        PeopleInLargePositiveGroups.insert(PeopleInLargePositiveGroups.end(), Rule.begin(),Rule.end());
    }
	
	return PeopleInLargePositiveGroups;
}