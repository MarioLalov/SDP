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
Hierarchy *getSubtree(Person *head)
{
    return new Hierarchy(head);
}

Hierarchy::Hierarchy(Person *head)
{
    head_manager = head;
    count(head);
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
}

HierarchyIter *Hierarchy::iter() const
{
    return new HierarchyIter(this);
}

bool Hierarchy::find(const string &name) const
{
    HierarchyIter *iter = this->iter();

    return getPerson(name, iter);
}

string Hierarchy::manager(const string &name) const
{
    // TODO: throw
    HierarchyIter *iter = new HierarchyIter(this);

    return getPerson(name, iter, PARENT)->getName();
}

int Hierarchy::num_subordinates(const string &name) const
{
    // TODO: return negative if nullptr
    HierarchyIter *iter = new HierarchyIter(this);
    Hierarchy subtree(getPerson(name, iter));

    return subtree.num_employees();
}

int Hierarchy::num_employees() const
{
    return total_employees;
}

unsigned long Hierarchy::getSalary(const string &who) const
{
    HierarchyIter *iter = new HierarchyIter(this);
    unsigned int direct_subordinates = getPerson(who, iter)->subordinatesNumber();
    unsigned int indirect_subordinates = num_subordinates(who) - direct_subordinates;

    return 500*direct_subordinates + 50*indirect_subordinates;
}

bool Hierarchy::hire(const string &who, const string &boss)
{
    HierarchyIter *iter = new HierarchyIter(this);

    Person *p = getPerson(boss, iter);

    if (!p)
        return false;

    new Person(who, p);

    return true;
}

bool Hierarchy::fire(const string &who)
{
    HierarchyIter *iter = new HierarchyIter(this);

    Person *boss = getPerson(who, iter, PARENT);
    Person *cur = getPerson(who, iter);

    std::vector<Person *> cur_subordinates = cur->getSubordinates();
    boss->removeSubordinate(who);

    for (std::size_t i = 0; i < cur_subordinates.size(); i++)
    {
        boss->addSubordinate(cur_subordinates[i]);
    }

    delete cur;
}

// not yet implemented
Hierarchy::~Hierarchy()
{
    std::cout << "destruct" << std::endl;
}
// hierarchy iter
HierarchyIter::HierarchyIter(const Hierarchy *hierarchy)
{
    previous = nullptr;
    cur = hierarchy->head_manager;

    LOG std::cout << "Loading person: " << (cur ? cur->name : "none") << std::endl;
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
    // preoredr traverse
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