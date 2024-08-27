#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <cmath>
#include <algorithm>


namespace names
{

    const std::string Andon = "andon";
    const std::string Bryan = "bryan";
    const std::string Case = "case";
    const std::string Colin = "colin";
    const std::string Daniel = "daniel";
    const std::string Dylan = "dylan";
    const std::string Ethan = "ethan";
    const std::string Evan = "evan";
    const std::string Grant = "grant";
    const std::string Hunter = "hunter";
    const std::string James = "james";
    const std::string Jayde = "jayde";
    const std::string Jon = "jon";
    const std::string Kobe = "kobe";
    const std::string Kyler = "kyler";
    const std::string Lane = "lane";
    const std::string Loepp = "loepp";
    const std::string Neil = "neil";
    const std::string Noah = "noah";
    const std::string Paydan = "paydan";
    const std::string Zach = "zach";

    const std::vector<std::vector<std::string>> InstrumentationNames = {
        {
            Andon, Bryan, Case, Colin, Daniel, Dylan, Ethan, Evan, 
            Grant, Hunter, James, Jayde, Jon, Kobe, Kyler, Lane, 
            Loepp, Neil, Noah, Paydan, Zach
        },
    };

    const std::vector<std::string> Leaders = {
        James, Jon, Loepp, Kobe, Ethan
    };

    const std::vector<std::vector<std::string>> NegativeRules = {
        Leaders,
        {Case, Evan, Jayde, Kobe},
        {Dylan, Ethan, Lane},
        {Colin, Hunter, Loepp, Noah},
        {Jon, Paydan},
        {James, Neil},
        {Zach,Kyler},

        //Maybe Rules

    };
    

    const std::vector<std::vector<std::string>> PositiveRules = {
        {Ethan, Andon, Daniel, Grant, Neil,},
        {James, Evan},
        {Jon,Dylan,Noah},
        {Loepp, Bryan, Lane},
        //Maybe Rules

        
    };
    

    const size_t MaxGroupSize = static_cast<size_t>(std::ceil(std::sqrt(InstrumentationNames[0].size())));
    const size_t MinGroupSize = static_cast<size_t>(std::floor(std::sqrt(InstrumentationNames[0].size())));
    const size_t MaxGroups = static_cast<size_t>(std::ceil(std::sqrt(InstrumentationNames[0].size())));

} // namespace names
