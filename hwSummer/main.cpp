#include "TreeContainer/TreeContainer.h"
#include <sstream>
#include <iostream>

int main()
{
    //std::istringstream stream("| 2 |\n| 8 6 |\n| 11 | |\n| |");
    //Tree* tree1 = CreateTree(stream);

    //std::istringstream stream2("| 8 |\n| 3 2 9 |\n| 2 | 8 6 | 2 |\n| 8 6 | 11 | 12 | 8 5 |\n| 11 | 4 3 | | | 11 | 6 |\n| | | | | |");
    //Tree* tree2 = CreateTree(stream2);

    std::vector<std::string> names = {"sfsdg", "sfahky", "yu7tu", "y790"};

    TreeContainer container;
    for(const auto& name : names)
    {
        std::istringstream stream("| 2 |\n| 8 6 |\n| 11 | |\n| |");
        Tree* tree1 = CreateTree(stream);

        container.Insert(name, tree1);
    }

    /*std::istringstream stream2("| 8 |\n| 3 9 12 |\n| 2 | 2 | |\n| 8 6 | 8 5 |\n| 11 | 4 3 | | | 11 | 6 |");
    Tree* tree2 = CreateTree(stream2);*/

    //tree2->Remove(*tree1);
    //tree2->Print();

    // delete tree1;
    // delete tree2;
}