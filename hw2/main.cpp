#include <iostream>
#include <string>
#include "interface.h"

int main()
{
    std::string str = "pesho-gosho\npesho-sasho\npesho-tosho\ntosho-ivan\n";

    Hierarchy *create = new Hierarchy(str);
    create->hire("ceco", "ivan");

    //HierarchyIter *iter = new HierarchyIter(create);
    //create->fire("tosho");
    //Person *found = getPerson("ceco", iter);

    //std::cout << "Longest: " << create->longest_chain();
    std::cout << create->print();

    // delete create;
    // delete iter;

    return 0;
}