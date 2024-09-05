#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include "PositiveTable.hpp"
#include "FilterFunctions.hpp"
#include "Print.hpp"
#include <optional>

extern bool isVerbose;

std::optional<StringList_t> HasNamesFromLargeLists(const StringList_t& List, const GroupConstraints& Constraints)
{
    std::vector<uint64_t> NameCounts;
    StringList_t BigRuleUsed;

    for(const auto& LargeRule : Constraints.LargePositiveRules)
    {
        uint64_t count;
        for(const auto& Name : LargeRule)
        {
            if(std::find(List.begin(),List.end(),Name) != List.end())
            {
                count++;
            }
        }

        if(count == Constraints.MaxGroupSize)
        {
            BigRuleUsed = LargeRule;
        }
        NameCounts.push_back(count);
    }

    for(const auto& Counts : NameCounts)
    {
        if(Counts == 0) return std::nullopt;
    }
    
    return BigRuleUsed;
}


StringTable_t CapstoneGroupCalc::Generate::Positive::Table(const StringTable_t& Names, const StringTable_t& Rules, const GroupConstraints& Constraints)
{
    if(isVerbose) std::cout << "GeneratePositiveTable " << std::endl;

    StringTable_t newTable = Names;
    StringTable_t tempTable;
    std::unordered_set<StringList_t> uniqueLists;

    for (const auto& Rule : Rules) // Pass Rule by reference
    {        
        for (auto& list : newTable) // Pass list by reference
        {
            auto templist = List(list, Rule, Constraints.MinGroupSize);

            // Check if the list is too small to be a group and that no other list
            if(!HasNamesFromLargeLists(list,Constraints)) continue;
            if (templist.size() >= Constraints.MinGroupSize && uniqueLists.find(templist) == uniqueLists.end()) // Knocks off 4 secs
            {
                if (isVerbose) {
                    Print::Names(templist);
                    std::cout << std::endl;
                }
                tempTable.push_back(templist);
                uniqueLists.insert(templist);
            }
        }

        //Noah this is recursive and thus just uses the new table as the new input for the for loop
        uniqueLists.clear();
        newTable.clear();
        newTable = std::move(tempTable);
    }
    
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

StringTable_t CapstoneGroupCalc::Generate::FilteredGroupTable(const StringTable_t& Names, const StringTable_t& negativeRules, const StringTable_t& positiveRules, const GroupConstraints& Constraints)
{
    auto Table = Names;
    for (auto RuleSet : negativeRules){
        Table = CapstoneGroupCalc::Generate::Negative::Table(Table, RuleSet);
    }

    Table = CapstoneGroupCalc::Generate::Positive::Table(Table, positiveRules, Constraints); 

    return Table;
}


StringTable_t CapstoneGroupCalc::Generate::PossibleConfigurations(const StringList_t& List, const GroupConstraints& constraints)
{
    std::vector<bool> select(List.size(), false);
    std::fill(select.begin(), select.begin() + constraints.MinGroupSize, true);

    StringTable_t variations;

    do {
        std::vector<std::string> group;
        for (int i = 0; i < List.size(); ++i) {
            if (select[i]) {
                group.push_back(List[i]);
            }
        }

        // Check if the group is valid based on the large list constraints.
        auto PossibleNewGroup = HasNamesFromLargeLists(group, constraints);


        variations.push_back(group);  // Otherwise, push back the current group.


    // Generate the next combination.
    } while (std::prev_permutation(select.begin(), select.end()));

    // Sort the resulting variations lexicographically.
    std::sort(variations.begin(), variations.end());

    return variations;
}


StringMatrix_t CapstoneGroupCalc::Generate::PossibleMatrixConfigurations(const StringTable_t& Table, const GroupConstraints& Constraints) {
    // Initialize a vector of booleans for selecting groups
    if(isVerbose) std::cout << " Generate Possible Configurations " << std::endl;
    std::vector<bool> select(Table.size(), false);
    std::fill(select.begin(), select.begin() + Constraints.NumGroup, true);

    StringMatrix_t LegalVariations;

    do {
        StringTable_t group;
        std::unordered_set<std::string> uniqueNames;
        bool hasDuplicates = false;

        for (int i = 0; i < Table.size(); ++i) {
            if (select[i]) {
                for (const auto& name : Table[i]) {
                    if (!uniqueNames.insert(name).second) {
                        hasDuplicates = true;
                        break;
                    }
                }
                if (hasDuplicates) break;
                group.push_back(Table[i]);
            }
        }

        if (!hasDuplicates ) {
            LegalVariations.push_back(group);
        }
    } while (std::prev_permutation(select.begin(), select.end()));


    return LegalVariations;
}



// Ensure the table is not const if modifications are expected
StringTable_t CapstoneGroupCalc::Generate::PossibleConfigurations(const StringTable_t& Table, const GroupConstraints& Constraints) {
    // Generate new table based on configurations
    StringTable_t NewTable;

    for (auto& List : Table) {
        // Ensure non-const strings for modification
        if (Constraints.LargePositiveRulesHashes.find(List) == Constraints.LargePositiveRulesHashes.end()) {
            StringTable_t tempTable = Generate::PossibleConfigurations(List, Constraints);
            NewTable.insert(NewTable.end(), tempTable.begin(), tempTable.end());
        }
    }
    NewTable = Positive::Table(NewTable, Constraints.JoinRules, Constraints);
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

    if (FilteredRules.empty())
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
    if(isVerbose) std::cout << " Initializing People In Large Groups " << std::endl;
    StringList_t PeopleInLargePositiveGroups;

    for(auto& Rule : PositiveRuleSets)
    {
        if(Rule.size() < MinGroupSize) continue;
        PeopleInLargePositiveGroups.insert(PeopleInLargePositiveGroups.end(), Rule.begin(),Rule.end());
    }
	
	return PeopleInLargePositiveGroups;
}