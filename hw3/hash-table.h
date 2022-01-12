#pragma once

#include "md5.h"
#include <vector>
#include <list>
#include <tuple>
#include <sstream>
#include <set>

//max hash value = FFFFF
static const std::size_t TABLE_SIZE = 1048576;

struct Element
{
    std::string name;
    std::size_t occurences;
    // used to build list with all elements
    Element *previous = nullptr;

    Element(std::string in_name, std::size_t in_occurences, Element *in_previous)
        : name(in_name), occurences(in_occurences), previous(in_previous){};
};

class HashTable
{
private:
    // word - occurences
    std::size_t total;
    std::size_t cells_occupied;
    std::list<Element> *table;
    Element *last = nullptr;

    std::size_t getIndex(const std::string &word) const;

public:
    HashTable();
    void addWord(std::string new_word, std::size_t occurences);
    std::size_t getOccurences(const std::string &word) const;
    std::size_t numOccupied() const;
    std::size_t getTotal() const;
    void remove(std::string word);

    std::list<std::pair<std::string, std::size_t>> getInList() const;
    std::multiset<std::string> getInMultiSet() const;
    ~HashTable();
};