#include "interface.h"
#include <iostream>

//#define LOG_ACTIVE
#define LOG

#ifndef LOG_ACTIVE
#define LOG //
#endif

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

/*Person *Person::traverse(const std::string &name, Person *cur) const
{
    if (cur->name == name)
    {
        return cur;
    }

    for (std::size_t i = 0; i < cur->subordinates.size(); i++)
    {
        if (cur->subordinates[i])
        {
            Person *returned = traverse(name, cur->subordinates[i]);

            if (returned)
            {
                return returned;
            }
        }
    }

    return nullptr;
}*/

string Person::getName() const
{
    return name;
}

/*Person *Hierarchy::getPerson(const std::string &name) const
{
    return head_manager->traverse(name, head_manager);
}*/

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

            first = false;
        }
        else
        {
            delete iter;

            iter = new HierarchyIter(this);

            new Person(subordinate_name, getPerson(boss_name, iter));
        }
    }

    delete iter;
}

HierarchyIter *Hierarchy::iter() const
{
    return new HierarchyIter(this);
}

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