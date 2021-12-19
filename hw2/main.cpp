#include <iostream>
#include <string>
#include "interface.h"

int main()
{
    std::string str = "pesho-gosho\npesho-sasho\npesho-tosho\ntosho-ivan\n";

    Hierarchy *create = new Hierarchy(str);

    HierarchyIter *iter = new HierarchyIter(create);
    Person *found = getPerson("ivn", iter);

    std::cout << create->manager("pesho");

    // delete create;
    // delete iter;

    return 0;
}