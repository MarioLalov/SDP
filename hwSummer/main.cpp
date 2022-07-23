#include "Tree/Tree.h"
#include <sstream>
#include <iostream>

int main()
{
    std::istringstream stream("| 8 |");
    Tree* tree1 = CreateTree(stream);

    std::istringstream stream2("| 2 |\n|  8 6 |\n| 11 | |");
    Tree* tree2 = CreateTree(stream2);

    if(tree1->Contains(*tree2))
        std::cout << "true";
    else
        std::cout << "false";

    delete tree1;
    delete tree2;
}