#include <iostream>
#include <string>
#include <fstream>
#include "interface.h"
#include <vector>
#include <sstream>
#include <cctype>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Invalid number of inputs!" << std::endl;
        return 1;
    }

    std::fstream file1(argv[1]);
    std::fstream file2(argv[2]);

    if(!file1.is_open() || !file2.is_open())
    {
        std::cout << "A problem occured while opening the files" << std::endl;
        return 1;
    }

    Comparator c;
    ComparisonReport report = c.compare(file1, file2);

    std::cout << argv[1] << " contains " << report.uniqueWords[0].wordsTotal() + report.commonWords.wordsTotal() 
            << " words and " << report.commonWords.wordsTotal() << " are present in " << argv[2] << " ("
            << (100*report.commonWords.wordsTotal())/
            (report.uniqueWords[0].wordsTotal() + report.commonWords.wordsTotal()) << "%)" << std::endl;

    std::cout << argv[2] << " contains " << report.uniqueWords[1].wordsTotal() + report.commonWords.wordsTotal() 
            << " words and " << report.commonWords.wordsTotal() << " are present in " << argv[1] << " ("
            << (100*report.commonWords.wordsTotal())/
            (report.uniqueWords[1].wordsTotal() + report.commonWords.wordsTotal()) << "%)" << std::endl;
    

    /*std::stringstream a("one two three four two one");
    std::stringstream b("two one four one one");
    Comparator c;

    ComparisonReport rep = c.compare(a, b);
    std::cout << (rep.commonWords.countOfUniqueWords() == 4) << std::endl;
    std::cout << (rep.uniqueWords[0].countOfUniqueWords() == 2) << std::endl;
    std::cout << (rep.uniqueWords[1].countOfUniqueWords() == 1) << std::endl;

    std::multiset<std::string> expected{"one", "one", "two", "four"};
    std::cout << (rep.commonWords.words() == expected) << std::endl;*/

    return 0;
}