#include "main.hpp"

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>

#include "names.hpp"
#include "PositiveTable.hpp"
#include "FilterFunctions.hpp"
// #include "main.hpp"
#include "Print.hpp"

using namespace names;

int main()
{
    GroupConstraints foo(names::InstrumentationNames[0], names::NegativeRules, names::PositiveRules);
    
    StringTable_t Table = CapstoneGroupCalc::Generate::FilteredGroupTable(InstrumentationNames,foo.NegativeRules,foo.PositiveRules,names::Leaders, foo.MinGroupSize);
    Print::Names(Table);

    Table = CapstoneGroupCalc::Generate::PossibleConfigurations(Table,names::Leaders);
    Print::Names(Table);

    Table = CapstoneGroupCalc::Generate::Positive::Table(Table,foo.PositiveRules,foo.MinGroupSize);
    Print::Names(Table);

    Sort::LeaderFirst(Table);
    StringMatrix_t PossibleVariationsofGroups = CapstoneGroupCalc::Generate::PossibleConfigurations(Table, foo.MaxGroupSize);

    Print::Names(PossibleVariationsofGroups);

    // std::cout<< std::endl << "Amount of Tables Generated: " << PossibleVariationsofGroups.size() << std::endl;

    return 0;
}