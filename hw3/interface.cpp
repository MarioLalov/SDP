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

    // std::multiset<std::string> set1 = table_a.words();
    // std::multiset<std::string> set2 = table_b.words();
    std::list<std::pair<std::string, std::size_t>> list1 = table_a.wordsList();
    std::list<std::pair<std::string, std::size_t>> list2 = table_b.wordsList();

    ComparisonReport report;

    // std::string current = "";
    // std::size_t count = 0;

    // {1 1 1 2 3 4 4}
    // {2 3 4 5}
    for (auto &el : list1)
    {
        std::size_t b_count = table_b.countOf(el.first);

        report.commonWords.add(el.first, b_count < el.second ? b_count : el.second);

        if (el.second > b_count)
        {
            report.uniqueWords[0].add(el.first, el.second - b_count);

            // for (std::size_t i = 0; i < b_count; i++)
            //{
            if (b_count != 0)
            {
                list2.remove({el.first, b_count});
            }
            //}
        }
        else
        {
            report.uniqueWords[1].add(el.first, b_count - el.second);

            // for (std::size_t i = 0; i < b_count; i++)
            //{
            if (b_count != 0)
            {
                list2.remove({el.first, b_count});
            }
            //}
        }
    }

    /*for (std::multiset<std::string>::iterator it = set1.begin(); it != set1.end(); ++it)
    {
        std::cout << *it << std::endl;
        if (current == "")
        {
            current = *it;
            count = 0;
        }

        if (*it == current)
        {
            count++;
            continue;
        }
        else
        {
            std::size_t b_count = table_b.countOf(current);

            report.commonWords.add(current, b_count < count ? b_count : count);

            if (count > b_count)
            {
                report.uniqueWords[0].add(current, count - b_count);

                for (std::size_t i = 0; i < b_count; i++)
                {
                    set2.erase(set2.find(current));
                }
            }
            else
            {
                report.uniqueWords[1].add(current, b_count - count);

                for (std::size_t i = 0; i < b_count; i++)
                {
                    set2.erase(set2.find(current));
                }
            }

            current = *it;
            count = 1;
        }
    }

    std::size_t b_count = table_b.countOf(current);

    report.commonWords.add(current, b_count < count ? b_count : count);

    if (count > b_count)
    {
        report.uniqueWords[0].add(current, count - b_count);

        for (std::size_t i = 0; i < b_count; i++)
        {
            set2.erase(set2.find(current));
        }
    }
    else
    {
        report.uniqueWords[1].add(current, b_count - count);

        for (std::size_t i = 0; i < b_count; i++)
        {
            set2.erase(set2.find(current));
        }
    }

    for (std::multiset<std::string>::iterator it = set2.begin(); it != set2.end(); ++it)
    {
        report.uniqueWords[1].add(*it);
    }*/

    for (auto el : list2)
    {
        report.uniqueWords[1].add(el.first);
    }

    return report;
}