#include "interface.h"

void WordsMultiset::add(const std::string &word, size_t times)
{
    if (times < 1)
    {
        return;
    }

    table.addWord(word, times);
}

bool WordsMultiset::contains(const std::string &word) const
{
    return table.getOccurences(word) != 0;
}

size_t WordsMultiset::countOf(const std::string &word) const
{
    return table.getOccurences(word);
}

size_t WordsMultiset::countOfUniqueWords() const
{
    return table.numOccupied();
}

std::multiset<std::string> WordsMultiset::words() const
{
    return table.getInMultiSet();
}

std::list<std::pair<std::string, std::size_t>> WordsMultiset::wordsList() const
{
    return table.getInList();
}

std::size_t WordsMultiset::wordsTotal() const
{
    return table.getTotal();
}

void WordsMultiset::remove(std::string word)
{
    table.remove(word);
}

void Comparator::read(std::istream &stream, WordsMultiset &table)
{
    std::string cur_word;
    cur_word.clear();
    char cur;

    while (stream.get(cur))
    {

        if (!std::isspace(cur))
        {
            cur_word += cur;
        }
        else
        {
            if (!cur_word.empty())
            {
                table.add(cur_word, 1);
                cur_word.clear();
            }
        }
    }

    if (!cur_word.empty())
    {
        table.add(cur_word);
        cur_word.clear();
    }
}

ComparisonReport Comparator::compare(std::istream &a, std::istream &b)
{
    WordsMultiset table_a;
    read(a, table_a);

    WordsMultiset table_b;
    read(b, table_b);

    std::list<std::pair<std::string, std::size_t>> list1 = table_a.wordsList();

    ComparisonReport report;

    for (auto &el : list1)
    {
        std::size_t b_count = table_b.countOf(el.first);

        report.commonWords.add(el.first, b_count < el.second ? b_count : el.second);

        if (el.second > b_count)
        {
            report.uniqueWords[0].add(el.first, el.second - b_count);

            // remove all occurences of el.first from the second table
            if (b_count != 0)
            {
                table_b.remove(el.first);
            }
        }
        else
        {
            report.uniqueWords[1].add(el.first, b_count - el.second);

            // remove all occurences of el.first from the second table
            if (b_count != 0)
            {
                table_b.remove(el.first);
            }
        }
    }

    //a list that contains only the unique elements for file2
    std::list<std::pair<std::string, std::size_t>> list2 = table_b.wordsList();
    for (auto el : list2)
    {
        report.uniqueWords[1].add(el.first);
    }

    return report;
}