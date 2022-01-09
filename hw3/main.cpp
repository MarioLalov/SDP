#include <iostream>
#include <string>
#include "interface.h"
#include <vector>
#include <sstream>
#include <cctype>

int main()
{

    std::stringstream a("one");
    std::stringstream b("one two three");
    Comparator c;

    ComparisonReport rep = c.compare(a, b);

    return 0;
}