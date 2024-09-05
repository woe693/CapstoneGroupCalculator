#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cmath>
#include <unordered_set>

using StringList_t = std::vector<std::string>;
using StringTable_t = std::vector<StringList_t>;
using StringMatrix_t = std::vector<StringTable_t>;

namespace std {
    template <>
    struct hash<StringList_t> {
        std::size_t operator()(const StringList_t& list) const {
            std::size_t seed = 0;
            for (const auto& str : list) {
                // Combine the hash of the current string with the seed
                seed ^= std::hash<std::string>{}(str) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    template <>
    struct hash<StringTable_t> {
        std::size_t operator()(const StringTable_t& Table) const {
            std::size_t seed = 0;
            for (const auto& list : Table) {
                // Combine the hash of the current string with the seed
                seed ^= std::hash<StringList_t>{}(list) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

struct GroupConstraints
{
    uint64_t MaxGroupSize;
    uint64_t MinGroupSize;
    uint64_t NumGroup;
    StringTable_t SplitRules;
    StringTable_t JoinRules;
    StringTable_t LargePositiveRules;
    StringList_t Names;
    std::unordered_set<std::string> NameHashes;
    std::unordered_set<StringList_t> LargePositiveRulesHashes;

    GroupConstraints(StringList_t _Names, StringTable_t _NegativeRules, StringTable_t _PositiveRules) 
    {
        using namespace std;
        Names = _Names; 
        SplitRules = _NegativeRules;
        JoinRules = _PositiveRules;
        MaxGroupSize = NumGroup = ceil(sqrt(Names.size()));
        MinGroupSize = floor(sqrt(Names.size()));
        for(auto& Name : Names)
        {
            NameHashes.insert(Name);
        }

        for(auto& Rule : JoinRules)
        {
            if(Rule.size() == MaxGroupSize)
            {
                std::sort(Rule.begin(), Rule.end());
                LargePositiveRules.push_back(Rule);
            }
        }

        for (auto LargeJoinRule : LargePositiveRules)
        {
            LargePositiveRulesHashes.insert(LargeJoinRule);
        }
    }
};



namespace CapstoneGroupCalc
{

    void init(StringList_t Names, StringTable_t SplitRules, StringTable_t JoinRules);
    
    namespace Generate
    {
        namespace Positive
        {
            // StringTable_t Table(StringTable_t, StringTable_t, uint64_t);
            StringTable_t Table(const StringTable_t& Names, const StringTable_t& Rules, const GroupConstraints& Constraints);
            StringList_t List(StringList_t, StringList_t, uint64_t);
        }

        namespace Negative
        {
            StringList_t InverseList(StringList_t, StringList_t);
            StringTable_t Table(StringTable_t, StringList_t);
            StringTable_t Table(StringList_t, StringList_t);
        }

        StringTable_t FilteredGroupTable(const StringTable_t& Names, const StringTable_t& negativeRules, const StringTable_t& positiveRules, const GroupConstraints& Constraints);

        StringTable_t PossibleConfigurations(const StringList_t& List, const GroupConstraints& constraints);
        StringTable_t PossibleConfigurations(const StringTable_t& Table, const GroupConstraints& Constraints);
        StringMatrix_t PossibleMatrixConfigurations(const StringTable_t& Table, const GroupConstraints& Constraints);

        StringList_t InitializePeopleInLargeGroups(StringTable_t, uint64_t);
    }

    namespace DataManipulation
    {
        namespace Filter
        {
            StringTable_t rmListsTooSmall(StringTable_t);
            StringTable_t OutDuplicates(StringTable_t&);
            StringList_t OutDuplicates(StringList_t&);
            StringList_t NamesFromRules(StringList_t, StringList_t);
            bool less_vectors(const std::vector<std::string>&, const std::vector<std::string>&);
            void OutLeaderlessLists(StringTable_t&, StringList_t);
        };

        namespace Sort
        {
            void LeaderFirst(StringTable_t&);
            bool LeaderSort(const std::string&, const std::string&);
        };
    }
}