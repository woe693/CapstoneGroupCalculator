#include "main.hpp"

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>

#include "names.hpp"
#include "PositiveTable.hpp"
#include "FilterFunctions.hpp"
// #include "main.hpp"
#include "Print.hpp"

using namespace names;

bool isVerbose = false;
int main(int argc, char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-v" || arg == "--verbose") isVerbose = true;
    }

    GroupConstraints foo(names::InstrumentationNames[0], names::SplitRules, names::JoinRules);
    
    StringTable_t Table = CapstoneGroupCalc::Generate::FilteredGroupTable(InstrumentationNames,foo.SplitRules,foo.JoinRules,foo);
    Table = CapstoneGroupCalc::Generate::PossibleConfigurations(Table,foo);
    Table.insert(Table.end(),foo.LargePositiveRules.begin(),foo.LargePositiveRules.end());
    StringMatrix_t PossibleVariationsofGroups = CapstoneGroupCalc::Generate::PossibleMatrixConfigurations(Table, foo);

    Print::Names(PossibleVariationsofGroups);


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by the code: " << duration.count() / 1000000 << " seconds" << std::endl;

    return 0;
}