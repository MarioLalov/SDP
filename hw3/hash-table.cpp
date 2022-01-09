#include "hash-table.h"

HashTable::HashTable()
{
    table = new std::vector<std::pair<std::string, std::size_t>>[TABLE_SIZE];
    cells_occupied = 0;
}

std::size_t HashTable::getIndex(const std::string &word) const
{
    std::string hash = md5(word);
    hash = hash.substr(0, 3);
    std::size_t index;

    std::stringstream ss;
    ss << std::hex << hash;
    ss >> index;

    return index;
}

void HashTable::addWord(std::string new_word, std::size_t occurences)
{
    std::size_t index = getIndex(new_word);

    if (table[index].size() == 0)
    {
        cells_occupied++;
    }
    else
    {
        // increment word count
        for (std::size_t i = 0; table[index].size(); i++)
        {
            if (new_word == table[index][i].first)
            {
                table[index][i].second += occurences;

                return;
            }
        }
    }

    table[index].push_back({new_word, occurences});
}

std::size_t HashTable::getOccurences(const std::string &word) const
{
    std::size_t index = getIndex(word);

    for (std::size_t i = 0; i < table[index].size(); i++)
    {
        if (table[index][i].first == word)
        {
            return table[index][i].second;
        }
    }

    return 0;
}

std::size_t HashTable::numOccupied() const
{
    return cells_occupied;
}

HashTable::~HashTable()
{
    delete table;
    std::cout << "destruct" << std::endl;
}