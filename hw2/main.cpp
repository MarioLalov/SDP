#include <iostream>
#include <string>
#include "interface.h"

int main()
{
    std::cout << "Enter";
    //std::string str = "pesho-gosho\npesho-sasho\npesho-tosho\ntosho-ivan\n";
    //std::string str = "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n";

std::string str1 = "A-B\nA-C\nC-D\nC-F\nD-Q\n";
std::string str2 = "A-B\nA-C\nA-D\nB-F\nF-Q\n";
    Hierarchy *create = new Hierarchy(str1);
    Hierarchy *create1 = new Hierarchy(str2);
    

    //std::cout << "Longest: " << create->longest_chain();
    //create->incorporate();
    Hierarchy result = create->join(*create1);
    std::cout << "Begin incorp!!!!!!!!!!" << std::endl;
    std::cout << result.print();

    //create->incorporate();
    //create->hire("Ceco", "Gosho");
    //std::cout << "Incorporation done" << std::endl;
    //std::cout << create->print();
    /*std::cout << "---------------------" << std::endl;
    std::cout << second->print();*/

    // delete create;
    // delete iter;

    return 0;
}