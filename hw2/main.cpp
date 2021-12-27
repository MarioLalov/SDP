#include <iostream>
#include <string>
#include "interface.h"
#include <queue>
#include <algorithm>
#include <cctype>

int main()
{
    std::cout << "Enter";
     std::string str = "pesho-gosho\npesho-sasho\npesho-tosho\ntosho-ivan\n";
    //std::string str = "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n";

const string lozenec =
    "Uspeshnia - Gosho \n"
    "Uspeshnia - Misho \n"
    "Gosho     - Pesho \n"
    "Gosho     - Dancho\n"
    "Pesho     - Alex  \n"
    "Dancho    - Boris \n"
    "Dancho    - Kamen \n"
    "Uspeshnia - Slavi \n"
    "Slavi     - Slav1 \n"
    "Slavi     - Slav2 \n"
    "Slav1     - Mecho \n"
    "Mecho     - Q12Adl\n";

    const string large =
    "Uspeshnia - 1\n"
    "1-2\n1-3\n1-4\n1-5\n1-6\n1-7\n"
    "2-21\n2-22\n2-23\n2-24\n2-25\n2-26\n2-27\n2-28\n"
    "3-31\n3-32\n3-33\n3-34\n3-35\n3-36\n3-37\n3-38\n3-39\n";
    // std::cout << "Longest: " << create->longest_chain();
    // create->incorporate();
    //Hierarchy h(lozenec);
    std::string test = "1-2\n1-3\n2-4\n2-5\n3-6\n5-7\n5-8\n6-9\n";

  //  create.modernize();

    //std::cout << "Begin incorp!!!!!!!!!!" << std::endl;
    //std::string str1("  sdf    ");
Hierarchy h(lozenec);
h.incorporate();
std::cout << "!!!!!!" << std::endl;
std::cout << h.print();

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