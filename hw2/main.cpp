#include <iostream>
#include <string>
#include "interface.h"
#include <queue>

int main()
{
    std::cout << "Enter";
    // std::string str = "pesho-gosho\npesho-sasho\npesho-tosho\ntosho-ivan\n";
    std::string str = "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n";

    // std::cout << "Longest: " << create->longest_chain();
    // create->incorporate();
    Hierarchy create(str);
    create.modernize();

    std::cout << "Begin incorp!!!!!!!!!!" << std::endl;
    std::cout << create.print();

    // create->incorporate();
    // create->hire("Ceco", "Gosho");
    // std::cout << "Incorporation done" << std::endl;
    // std::cout << create->print();
    /*std::cout << "---------------------" << std::endl;
    std::cout << second->print();*/

    // delete create;
    // delete iter;

    return 0;
}