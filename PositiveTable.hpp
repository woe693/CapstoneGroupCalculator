#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cmath>

using StringList_t = std::vector<std::string>;
using StringTable_t = std::vector<StringList_t>;
using StringMatrix_t = std::vector<StringTable_t>;

struct GroupConstraints
{
    uint64_t MaxGroupSize;
    uint64_t MinGroupSize;
    uint64_t NumGroup;
    StringTable_t NegativeRules;
    StringTable_t PositiveRules;
    StringList_t Names;

    GroupConstraints(StringList_t _Names, StringTable_t _NegativeRules, StringTable_t _PositiveRules) 
    {
        using namespace std;
        Names = _Names; 
        NegativeRules = _NegativeRules;
        PositiveRules = _PositiveRules;
        MaxGroupSize = NumGroup = ceil(sqrt(Names.size()));
        MinGroupSize = floor(sqrt(Names.size()));
    }
};



namespace CapstoneGroupCalc
{

    void init(StringList_t Names, StringTable_t NegativeRules, StringTable_t PositiveRules);
    
    namespace Generate
    {
        namespace Positive
        {
            // StringTable_t Table(StringTable_t, StringTable_t, uint64_t);
            StringTable_t Table(StringTable_t&, StringTable_t&, uint64_t&);
            StringList_t List(StringList_t, StringList_t, uint64_t);
        }

        namespace Negative
        {
            StringList_t InverseList(StringList_t, StringList_t);
            StringTable_t Table(StringTable_t, StringList_t);
            StringTable_t Table(StringList_t, StringList_t);
        }

        StringTable_t FilteredGroupTable(StringTable_t, StringTable_t, StringTable_t, StringList_t, uint64_t);
        StringTable_t PossibleConfigurations(StringList_t);
        StringMatrix_t PossibleConfigurations(StringTable_t, uint64_t&);
        StringTable_t PossibleConfigurations(StringTable_t&, StringList_t);
        StringTable_t PossibleConfigurations(StringTable_t&);  // Remove duplicate if unintentional

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