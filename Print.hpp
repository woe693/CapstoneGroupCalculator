#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "stdint.h"
#include "PositiveTable.hpp"

namespace Print {

inline void Names(const StringList_t& List) {
    for(const auto& Name : List)
    {
        std::cout << Name << " ";
    }
    // std::cout << std::endl;
}

inline void Names(const StringTable_t& Table) {
    std::cout << std::endl;
    for (const auto& List : Table) {
        Names(List);
        std::cout << std::endl;
    }
}

inline void Names(const StringMatrix_t& Matrix) {

    for(StringTable_t GroupTable : Matrix)
    {
        std::cout << std::endl;
        for (auto List : GroupTable) {
            Names(List);
            std::cout << std::endl;
        }
        std::cout << std::endl ;
    }
    std::cout << std::endl;
    std::cout << "Generated " << Matrix.size() << " Variations Of Combonations of All Groups" << std::endl;
    std::cout << std::endl;
}

// Function to count and print the number of times each person occurs in the table
inline void NameCount(const StringList_t& Names, const StringTable_t& Table) {
    std::map<std::string, uint16_t> CountofNames;

    // Initialize the map with each name set to 0
    for (const auto& name : Names) {
        CountofNames[name] = 0;
    }

    // Count occurrences of each name in the table
    for (const auto& list : Table) {
        for (const auto& Name : Names) {
            if (std::find(list.begin(), list.end(), Name) != list.end()) {
                CountofNames[Name]++;
            }
        }
    }

    // Create a vector of pairs to sort by the value
    std::vector<std::pair<std::string, uint16_t>> sortedVec(CountofNames.begin(), CountofNames.end());

    // Sort the vector by value in descending order
    std::sort(sortedVec.begin(), sortedVec.end(),
        [](const std::pair<std::string, uint16_t>& a, const std::pair<std::string, uint16_t>& b) {
            return a.second > b.second;
        });

    // Print the sorted key-value pairs
    for (const auto& pair : sortedVec) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

}