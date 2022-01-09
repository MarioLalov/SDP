#include <iostream>
#include <string>
#include "interface.h"
#include "commands.h"

int main()
{
    bool end = false;

    while (!end)
    {
        std::string command;

        std::cout << "Enter command:";
        std::getline(std::cin, command);

        try
        {
            Commands::initiateCommand(command, end);
        }
        catch (const std::invalid_argument &err)
        {
            std::cout << err.what() << std::endl;
        }

        command.clear();
    }

    return 0;
}