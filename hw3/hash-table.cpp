#include "hash-table.h"

HashTable::HashTable()
{
    table = new std::list<Element>[TABLE_SIZE];
    cells_occupied = 0;
    total = 0;
}

std::size_t HashTable::getIndex(const std::string &word) const
{
    std::string hash = md5(word);
    hash = hash.substr(0, 5);
    std::size_t index;

    std::stringstream ss;
    ss << std::hex << hash;
    ss >> index;

    return index;
}

void HashTable::addWord(std::string new_word, std::size_t occurences)
{
    std::size_t index = getIndex(new_word);
    total += occurences;

    if (table[index].size() == 0)
    {
        cells_occupied++;
    }
    else
    {
        // increment word count
        for (auto& el : table[index])
        {
            if (new_word == el.name)
            {    
                el.occurences += occurences;

                return;
            }
        }
    }

    table[index].push_back(Element(new_word, occurences, last));
    last = &table[index].back();
}

std::size_t HashTable::getOccurences(const std::string &word) const
{
    std::size_t index = getIndex(word);

    for (auto const el : table[index])
    {
        if (el.name == word)
        {
            return el.occurences;
        }
    }

    return 0;
}

std::size_t HashTable::numOccupied() const
{
    return cells_occupied;
}

std::list<std::pair<std::string, std::size_t>> HashTable::getInList() const
{
    std::list<std::pair<std::string, std::size_t>> words;
    Element *cur = last;

    // gather all elements
    while (cur)
    {
        if(cur->occurences == 0)
        {
            cur = cur->previous;
            continue;
        }

        words.push_back({cur->name, cur->occurences});
        cur = cur->previous;
    }

    return words;
}

std::size_t HashTable::getTotal() const
{
    return total;
}

std::multiset<std::string> HashTable::getInMultiSet() const
{
    std::multiset<std::string> words;
    Element *cur = last;

    // gather all elements
    while (cur)
    {
        if(cur->occurences == 0)
        {
            cur = cur->previous;
            continue;
        }

        for (std::size_t i = 0; i < cur->occurences; i++)
        {
            words.insert(cur->name);
        }

        cur = cur->previous;
    }

    return words;
}

void HashTable::remove(std::string word)
{
    std::size_t index = getIndex(word);

    for (auto& el : table[index])
    {
        if (el.name == word)
        {
            el.occurences = 0;
        }
    }
}

HashTable::~HashTable()
{
    delete table;
}