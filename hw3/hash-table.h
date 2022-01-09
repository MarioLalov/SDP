#pragma once

#include "md5.h"
#include <vector>
#include <tuple>
#include <sstream>
#include <set>

static const std::size_t TABLE_SIZE = 4096;

class HashTable
{
private:
    // word - occurences
    std::size_t cells_occupied;
    std::vector<std::pair<std::string, std::size_t>> *table;

    std::size_t getIndex(const std::string &word) const;

public:
    HashTable();
    void addWord(std::string new_word, std::size_t occurences);
    std::size_t getOccurences(const std::string& word) const;
    std::size_t numOccupied() const;
    ~HashTable();
};