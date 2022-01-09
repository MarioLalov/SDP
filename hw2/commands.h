#pragma once

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "interface.h"

class Commands
{

private:
    static std::vector<Hierarchy *> hierarchies;
    static std::vector<std::string> names;
    static std::vector<bool> saved;

public:
    // get an already created hierarchy
    static Hierarchy *getHierarchy(std::string &name);
    //mark changes
    static void change(std::string& hierarchy_name, bool save);
    // print help menu
    static void help();
    static void load(std::string hierarchy_name, std::string file_name = "");
    static void save(std::string hierarchy_name, std::string file_name = "");
    static void find(std::string hierarchy_name, std::string person_name);
    static void num_subordinates(std::string hierarchy_name, std::string person_name);
    static void manager(std::string hierarchy_name, std::string person_name);
    static void num_employees(std::string hierarchy_name);
    static void overloaded(std::string hierarchy_name);
    static void join(std::string hierarchy1_name, std::string hierarchy2_name, std::string hierarchy_result_name);
    static void hire(std::string hierarchy_name, std::string person_name, std::string boss_name);
    static void fire(std::string hierarchy_name, std::string person_name);
    static void salary(std::string hierarchy_name, std::string person_name);
    static void incorporate(std::string hierarchy_name);
    static void modernize(std::string hierarchy_name);
    static void exit(bool& end);

    // execute command
    static void initiateCommand(std::string command, bool &end);
};

// helpers
void toUpper(std::string &input); // make whole string to upper