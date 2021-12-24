#include "interface.h"
#include <iostream>

//#define LOG_ACTIVE
#define LOG

#ifndef LOG_ACTIVE
#define LOG //
#endif

// person
Person::Person(const std::string in_name, Person *boss)
{
    LOG std::cout << "Person added: " << in_name << ", boss: " << (boss ? boss->name : "owner") << std::endl;

    name = in_name;

    if (boss)
    {
        boss->addSubordinate(this);
    }
}

void Person::addSubordinate(Person *new_subordiante)
{
    LOG std::cout << "Subordinate added to " << name << ": " << new_subordiante->name << std::endl;

    subordinates.push_back(new_subordiante);
}

string Person::getName() const
{
    return name;
}

std::vector<Person *> Person::getSubordinates() const
{
    return subordinates;
}

std::size_t Person::subordinatesNumber() const
{
    return subordinates.size();
}

void Person::removeSubordinate(const std::string &who)
{
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        if (subordinates[i]->name == who)
        {
            subordinates.erase(subordinates.begin() + i);

            return;
        }
    }
}

void Hierarchy::count(Person *cur)
{
    if (cur->subordinatesNumber() == 0)
    {
        return;
    }

    std::vector<Person *> subordinates = cur->getSubordinates();

    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        total_employees++;

        count(subordinates[i]);
    }
}

// hierarchy
Hierarchy *Hierarchy::getSubtree(Person *head) const
{
    return new Hierarchy(head);
}

Hierarchy::Hierarchy(Person *head)
{
    head_manager = head;
    count(head);

    iterator = new HierarchyIter(this);
}

Hierarchy::Hierarchy(const string &data)
{
    HierarchyIter *iter = new HierarchyIter(this);

    bool first = true;

    for (std::size_t i = 0; i < data.size(); i++)
    {
        std::string boss_name = getSubstring(data, i, '-');
        if (boss_name == "")
        {
            // throw
        }

        i += 1 + boss_name.size();

        std::string subordinate_name = getSubstring(data, i, '\n');
        if (subordinate_name == "")
        {
            // throw
        }

        i += subordinate_name.size();

        if (first)
        {
            head_manager = new Person(boss_name, nullptr);
            new Person(subordinate_name, head_manager);

            total_employees += 2;

            first = false;
        }
        else
        {
            delete iter;

            iter = new HierarchyIter(this);

            new Person(subordinate_name, getPerson(boss_name, iter));
            total_employees++;
        }
    }

    delete iter;

    iterator = new HierarchyIter(this);
}

HierarchyIter *Hierarchy::iter() const
{
    if (!iterator)
    {
        throw std::invalid_argument("No iterator found!");
    }

    return iterator;
}

bool Hierarchy::find(const string &name) const
{
    HierarchyIter *iter = this->iter();
    iter->begin();

    return getPerson(name, iter);
}

string Hierarchy::manager(const string &name) const
{
    // TODO: throw
    HierarchyIter *iter = this->iter();
    iter->begin();

    return getPerson(name, iter, PARENT)->getName();
}

int Hierarchy::num_subordinates(const string &name) const
{
    // TODO: return negative if nullptr
    HierarchyIter *iter = this->iter();
    iter->begin();

    Hierarchy subtree(getPerson(name, iter));

    return subtree.num_employees();
}

int Hierarchy::num_employees() const
{
    return total_employees;
}

unsigned long Hierarchy::getSalary(const string &who) const
{
    HierarchyIter *iter = this->iter();
    iter->begin();

    unsigned int direct_subordinates = getPerson(who, iter)->subordinatesNumber();
    unsigned int indirect_subordinates = num_subordinates(who) - direct_subordinates;

    return 500 * direct_subordinates + 50 * indirect_subordinates;
}

bool Hierarchy::hire(const string &who, const string &boss)
{
    HierarchyIter *iter = this->iter();
    iter->begin();

    Person *p = getPerson(boss, iter);

    if (!p)
        return false;

    new Person(who, p);

    // update iterator
    delete iterator;
    iterator = new HierarchyIter(this);

    return true;
}

bool Hierarchy::fire(const string &who)
{
    HierarchyIter *iter = this->iter();
    iter->begin();

    Person *boss = getPerson(who, iter, PARENT);
    Person *cur = getPerson(who, iter);

    std::vector<Person *> cur_subordinates = cur->getSubordinates();
    boss->removeSubordinate(who);

    for (std::size_t i = 0; i < cur_subordinates.size(); i++)
    {
        boss->addSubordinate(cur_subordinates[i]);
    }

    delete cur;

    // update iterator
    delete iterator;
    iterator = new HierarchyIter(this);
}

void Hierarchy::help(int level, Person *cur, int &count) const
{
    if (getSubtree(cur)->num_employees() > level)
    {
        count++;
    }

    std::vector<Person *> subordinates = cur->getSubordinates();

    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        help(level, subordinates[i], count);
    }
}

int Hierarchy::num_overloaded(int level) const
{
    int count = 0;
    help(level, head_manager, count);

    return count;
}

int Hierarchy::calculateLongest(Person *cur) const
{
    if (cur->subordinatesNumber() == 0)
    {
        return 1;
    }

    int longest = 0;

    std::vector<Person *> subordinates = cur->getSubordinates();

    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        int current = calculateLongest(subordinates[i]);

        if (current + 1 > longest)
        {
            longest = current + 1;
        }
    }

    return longest;
}

int Hierarchy::longest_chain() const
{
    return calculateLongest(head_manager);
}

void Hierarchy::printHelp(HierarchyIter *iter, std::string &output) const
{
    try
    {
        output += iter->parent()->getName() + "-" + iter->current()->getName() + "\n";
    }
    catch (const std::invalid_argument &err)
    {
    }

    try
    {
        // check first child subtree if such exists
        iter->firstChild();

        return printHelp(iter, output);
    }
    catch (const std::invalid_argument &err)
    {
        try
        {
            iter->nextSibiling();

            return printHelp(iter, output);
        }
        catch (const std::invalid_argument &err)
        {
            return;
        }
    }
}

std::string Hierarchy::print() const
{
    HierarchyIter *iter = this->iter();
    iter->begin();
    std::string output;

    printHelp(iter, output);

    return output;
}

// not fully implemented
Hierarchy::~Hierarchy()
{
    delete iterator;
    std::cout << "destruct" << std::endl;
}

// hierarchy iter
HierarchyIter::HierarchyIter(const Hierarchy *hierarchy)
{
    previous = nullptr;
    cur = hierarchy->head_manager;
    root = hierarchy->head_manager;

    LOG std::cout << "Loading person: " << (cur ? cur->name : "none") << std::endl;
}

Person *HierarchyIter::begin()
{
    previous = nullptr;
    cur = root;

    return cur;
}

Person *HierarchyIter::current()
{
    if (!cur)
    {
        throw std::invalid_argument("No person found!");
    }

    return cur;
}

Person *HierarchyIter::firstChild()
{
    // TODO: throw
    if (cur->subordinates.empty())
    {
        throw std::invalid_argument("No first child!");
    }

    sibling_index = 0;
    previous = cur;
    cur = cur->subordinates[sibling_index];
    LOG std::cout << "Loading person: " << cur->name << std::endl;

    return cur;
}

Person *HierarchyIter::nextSibiling()
{
    if (sibling_index + 1 > previous->subordinates.size() - 1)
    {
        throw std::invalid_argument("No next sibling!");
    }

    sibling_index++;

    cur = previous->subordinates[sibling_index];

    LOG std::cout << "Loading person: " << cur->name << "||Sibling " << sibling_index << std::endl;
    return cur;
}

Person *HierarchyIter::parent()
{
    if (previous == nullptr)
    {
        throw std::invalid_argument("No parent!");
    }
    LOG std::cout << "Parent of " << cur->name << " is " << previous->name << std::endl;

    return previous;
}

// helpers
Person *getPerson(const std::string &name, HierarchyIter *iter, unsigned int flag)
{
    // preorder traverse
    if (iter->current()->getName() == name)
    {
        if (flag == PARENT)
            return iter->parent();
        else
            return iter->current();
    }
    else
    {
        try
        {
            // check first child subtree if such exists
            iter->firstChild();

            return getPerson(name, iter, flag);
        }
        catch (const std::invalid_argument &err)
        {
            try
            {
                // check next sibling subtree if such exists
                iter->nextSibiling();

                return getPerson(name, iter, flag);
            }
            catch (const std::invalid_argument &err)
            {
                // return nullptr if no child nodes exist
                return nullptr;
            }
        }
    }
}

std::string getSubstring(const std::string &str, std::size_t cur_pos, char symbl)
{
    std::string::size_type pos = str.find(symbl, cur_pos);

    if (pos != std::string::npos)
    {
        return str.substr(cur_pos, pos - cur_pos);
    }
    else
    {
        return "";
    }
}