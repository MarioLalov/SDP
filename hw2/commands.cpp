#include "commands.h"
#include <iterator>

std::vector<Hierarchy *> Commands::hierarchies;
std::vector<std::string> Commands::names;

Hierarchy *Commands::getHierarchy(std::string &name)
{
    for (std::size_t i = 0; i < names.size(); i++)
    {
        if (names[i] == name)
        {
            return hierarchies[i];
        }
    }

    return nullptr;
}

void Commands::load(std::string hierarchy_name, std::string file_name)
{
    if (file_name != "")
    {
        std::cout << "Enter with file " << file_name << std::endl;
        std::ifstream file(file_name);

        if (!file.is_open())
            throw std::invalid_argument("Invalid file name!");

        std::string str(std::istreambuf_iterator<char>{file}, {});
        file.close();

        hierarchies.push_back(new Hierarchy(str));
        names.push_back(hierarchy_name);
    }
    else
    {
        std::string str;
        std::string cur = " ";

        while (cur != "")
        {
            std::getline(std::cin, cur);

            if (cur == "")
            {
                break;
            }

            str += cur + '\n';
        }

        hierarchies.push_back(new Hierarchy(str));
        names.push_back(hierarchy_name);
    }
}

void Commands::save(std::string hierarchy_name, std::string file_name)
{
    if (file_name != "")
    {
        std::ofstream file(file_name);

        if (!file.is_open())
            throw std::invalid_argument("Invalid file name!");

        Hierarchy *current = getHierarchy(hierarchy_name);

        if (current)
            file << current->print();

        file.close();
    }
    else
    {
        Hierarchy *current = getHierarchy(hierarchy_name);

        if (current)
            std::cout << current->print();
    }
}

void Commands::find(std::string hierarchy_name, std::string person_name)
{
    // todo throw
    Hierarchy *current = getHierarchy(hierarchy_name);
    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    std::cout << person_name << (current->find(person_name) ? " is " : " isn't ")
              << "emplyed in " << hierarchy_name << std::endl;
}

/*void Commands::help()
{
   // std::cout << menu;
}*/

void Commands::num_subordinates(std::string hierarchy_name, std::string person_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);
    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    int subordinates = current->num_subordinates(person_name);

    std::cout << person_name << " has ";

    if (subordinates == 0)
        std::cout << "no";
    else
        std::cout << subordinates;

    std::cout << " subordinates." << std::endl;
}

void Commands::manager(std::string hierarchy_name, std::string person_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);
    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    // to do check for head manager
    std::cout << "The manager of " << person_name << " is " << current->manager(person_name) << std::endl;
}

void Commands::num_employees(std::string hierarchy_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);
    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    std::cout << "There are " << current->num_employees()
              << " employees in " << hierarchy_name << "." << std::endl;
}

void Commands::overloaded(std::string hierarchy_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);
    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    int result = current->num_overloaded();

    if (result == 0)
        std::cout << "No overloaded employees in " << hierarchy_name << std::endl;
    else
        std::cout << "The number of overloaded employees in " << hierarchy_name << " is " << result << std::endl;
}

void Commands::join(std::string hierarchy1_name, std::string hierarchy2_name, std::string hierarchy_result_name)
{
    Hierarchy *h1 = getHierarchy(hierarchy1_name);
    Hierarchy *h2 = getHierarchy(hierarchy2_name);

    if (!h1 || !h2)
        throw std::invalid_argument("No such hierarchy!");

    Hierarchy *result = new Hierarchy(h1->join(*h2));
    hierarchies.push_back(result);
    names.push_back(hierarchy_result_name);

    std::cout << hierarchy_result_name << " created." << std::endl;
}

void Commands::hire(std::string hierarchy_name, std::string person_name, std::string boss_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);

    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    current->hire(person_name, boss_name);

    std::cout << person_name << " was hired." << std::endl;
}

void Commands::fire(std::string hierarchy_name, std::string person_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);

    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    current->fire(person_name);

    std::cout << person_name << " was fired." << std::endl;
}

void Commands::salary(std::string hierarchy_name, std::string person_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);

    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    std::cout << "The salary is " << current->getSalary(person_name) << " BGN" << std::endl;
}

void Commands::incorporate(std::string hierarchy_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);

    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    current->incorporate();

    std::cout << hierarchy_name << " was incorporated." << std::endl;
}

void Commands::modernize(std::string hierarchy_name)
{
    Hierarchy *current = getHierarchy(hierarchy_name);

    if (!current)
        throw std::invalid_argument("No such hierarchy!");

    current->modernize();

    std::cout << hierarchy_name << " was modernized." << std::endl;
}

void Commands::initiateCommand(std::string fullCommand, bool &end)
{
    std::string command;
    std::string param;
    std::vector<std::string> params;

    std::size_t endPos;
    std::size_t startPos;

    // get command
    endPos = fullCommand.find(' ');
    command = fullCommand.substr(0, endPos);

    // make command in upper case
    toUpper(command);

    // get parameters
    while (endPos < fullCommand.length())
    {
        while (fullCommand[endPos + 1] == ' ')
        {
            endPos++;
        }

        startPos = endPos + 1;

        if (fullCommand[startPos] == '"')
        {
            startPos += 1;
            endPos = fullCommand.find('"', startPos);

            param = fullCommand.substr(startPos, endPos - startPos);

            endPos++;
        }
        else
        {
            endPos = fullCommand.find(' ', startPos);

            param = fullCommand.substr(startPos, endPos - startPos);
        }

        params.push_back(param);
    }

    if (command == "HELP" && params.size() == 0)
    {
        // help();
    }
    else if (command == "LOAD" && (params.size() == 1 || params.size() == 2))
    {
        if (params.size() == 1)
        {
            load(params[0]);
        }
        else
        {
            load(params[0], params[1]);
        }
    }
    else if (command == "SAVE" && (params.size() == 1 || params.size() == 2))
    {
        if (params.size() == 1)
        {
            save(params[0]);
        }
        else
        {
            save(params[0], params[1]);
        }
    }
    else if (command == "FIND" && params.size() == 2)
    {
        find(params[0], params[1]);
    }
    else if (command == "NUM_SUBORDINATES" && params.size() == 2)
    {
        num_subordinates(params[0], params[1]);
    }
    else if (command == "MANAGER" && params.size() == 2)
    {
        manager(params[0], params[1]);
    }
    else if (command == "NUM_EMPLOYEES" && params.size() == 1)
    {
        num_employees(params[0]);
    }
    else if (command == "OVERLOADED" && params.size() == 1)
    {
        overloaded(params[0]);
    }
    else if (command == "JOIN" && params.size() == 3)
    {
        join(params[0], params[1], params[2]);
    }
    else if (command == "FIRE" && params.size() == 2)
    {
        fire(params[0], params[1]);
    }
    else if (command == "HIRE" && params.size() == 3)
    {
        hire(params[0], params[1], params[2]);
    }
    else if (command == "SALARY" && params.size() == 2)
    {
        salary(params[0], params[1]);
    }
    else if (command == "INCORPORATE" && params.size() == 1)
    {
        incorporate(params[0]);
    }
    else if (command == "MODERNIZE" && params.size() == 1)
    {
        modernize(params[0]);
    }
    else if (command == "EXIT")
    {
        end = true;
    }
    else
    {
        throw std::invalid_argument("Unknown command: " + command);
    }
}

void toUpper(std::string &input) // make whole string to upper
{
    for (std::size_t i = 0; i < input.length(); i++)
    {
        input[i] = std::toupper(input[i]);
    }
}