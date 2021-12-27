#include "interface.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <cctype>

//#define LOG_ACTIVE
#define LOG

#ifndef LOG_ACTIVE
#define LOG //
#endif

// person
Person::Person(const std::string in_name, Person *boss)
{
    name = in_name;
    parent = boss;

    if (boss)
    {
        boss->addSubordinate(this);
    }
}

void Person::addSubordinate(Person *new_subordiante)
{
    new_subordiante->parent = this;
    subordinates.push_back(new_subordiante);

    //sort subordinates
    std::sort(subordinates.begin(), subordinates.end(), [](const Person *lhs, const Person *rhs)
              { return lhs->getName() < rhs->getName(); });
}

std::string Person::getName() const
{
    return name;
}

std::vector<Person *> Person::getSubordinates() const
{
    return subordinates;
}

Person *Person::getParent() const
{
    return parent;
}

std::size_t Person::subordinatesNumber() const
{
    return subordinates.size();
}

void Person::removeSubordinate(const std::string &who)
{
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        //find and remove
        if (subordinates[i]->name == who)
        {
            // subordinates[i]->parent = nullptr;
            subordinates.erase(subordinates.begin() + i);

            /*std::sort(subordinates.begin(), subordinates.end(), [](const Person *lhs, const Person *rhs)
                      { return lhs->getName() < rhs->getName(); });*/

            return;
        }
    }
}

Person *Person::nextSubordinate() const
{
    //check if whether the current node root
    if (this->parent)
    {
        for (std::size_t i = 0; i < this->parent->subordinates.size(); i++)
        {
            //get right sibling
            if (this->parent->subordinates[i] == this && i + 1 != this->parent->subordinates.size())
            {
                return this->parent->subordinates[i + 1];
            }
        }
    }

    return nullptr;
}

Person *Person::firstChild() const
{
    //get leftmost child if such exists
    if (this->subordinates.size() > 0)
    {
        return this->subordinates[0];
    }

    return nullptr;
}

//Hierarchy

void Hierarchy::count(Person *cur)
{
    if (cur->subordinatesNumber() == 0)
    {
        return;
    }

    //count
    std::vector<Person *> subordinates = cur->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        total_employees++;

        count(subordinates[i]);
    }
}

Hierarchy *Hierarchy::getSubtree(Person *head) const
{
    return new Hierarchy(head);
}

void Hierarchy::traverse(const Person *current_r)
{
    //traverse hierarchy
    if (current_r->getParent())
    {
        new Person(current_r->getName(), getPerson(current_r->getParent()->getName(), head_manager));
    }
    else
    {
        head_manager = new Person(current_r->getName(), nullptr);
    }

    std::vector<Person *> subordinates = current_r->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        traverse(subordinates[i]);
    }
}

Hierarchy::Hierarchy(const Hierarchy &r)
{
    traverse(r.head_manager);
}

Hierarchy::Hierarchy(Person *head)
{
    head_manager = head;
    total_employees = 1;
    count(head);
}

Hierarchy::Hierarchy(const string &data)
{
    bool first = true;

    for (std::size_t i = 0; i < data.size(); i++)
    {
        std::string boss_name = getSubstring(data, i, '-');
        if (boss_name == "")
        {
            throw std::invalid_argument("No boss name!");
        }

        i += 1 + boss_name.size();

        std::string subordinate_name = getSubstring(data, i, '\n');
        if (subordinate_name == "")
        {
            throw std::invalid_argument("No subordinate name");
        }

        i += subordinate_name.size();

        // trim
        boss_name.erase(std::remove_if(boss_name.begin(), boss_name.end(), ::isspace), boss_name.end());
        subordinate_name.erase(std::remove_if(subordinate_name.begin(), subordinate_name.end(), ::isspace), subordinate_name.end());

        if (first)
        {
            head_manager = new Person(boss_name, nullptr);
            new Person(subordinate_name, head_manager);

            total_employees += 2;

            first = false;
        }
        else
        {
            // new Person(subordinate_name, getPerson(boss_name, iter));
            new Person(subordinate_name, this->getPerson(boss_name, head_manager));
            total_employees++;
        }
    }
}

HierarchyIter *Hierarchy::iter() const
{
    return iterator;
}

bool Hierarchy::find(const string &name) const
{
    Person *returned = this->getPerson(name, head_manager);

    return returned ? true : false;
}

string Hierarchy::manager(const string &name) const
{
    // TODO: throw

    // return getPerson(name, head_manager, PARENT)->getName();
    Person *returned = getPerson(name, head_manager);

    if (!returned)
    {
        return "";
    }

    return returned->getParent() ? returned->getParent()->getName() : "";
}

int Hierarchy::num_subordinates(const string &name) const
{
    // TODO: return negative if nullptr

    // fixed to return only direct

    // Hierarchy subtree(getPerson(name, head_manager));

    // return subtree.num_employees();
    Person *returned = getPerson(name, head_manager);
    return returned ? returned->subordinatesNumber() : -1;
}

int Hierarchy::num_all_subordinates(const string &name) const
{
    Hierarchy subtree(getPerson(name, head_manager));

    return subtree.num_employees() - 1;
}

int Hierarchy::num_employees() const
{
    return total_employees;
}

unsigned long Hierarchy::getSalary(const string &who) const
{

    Person *returned = getPerson(who, head_manager);
    if (!returned)
    {
        return -1;
    }

    unsigned int direct_subordinates = returned->subordinatesNumber();

    unsigned int indirect_subordinates = num_all_subordinates(who) - direct_subordinates;

    return 500 * direct_subordinates + 50 * indirect_subordinates;
}

bool Hierarchy::hire(const string &who, const string &boss)
{
    Person *p = getPerson(who, head_manager);

    if (p && p->getParent())
    {
        Person *b = getPerson(p->getParent()->getName(), head_manager);
        Person *new_b = getPerson(boss, head_manager);

        if (!new_b)
            return false;
        // same boss
        if (b->getName() == boss)
            return true;

        p->getParent()->removeSubordinate(p->getName());
        new_b->addSubordinate(p);

        return true;
    }

    p = getPerson(boss, head_manager);

    if (!p)
        return false;

    new Person(who, p);

    total_employees++;
    return true;
}

bool Hierarchy::fire(const string &who)
{
    std::cout << "Enter with " << who << std::endl;
    if (head_manager->getName() == who)
    {
        return false;
    }

    // Person *boss = getPerson(who, head_manager)->getParent();
    Person *cur = getPerson(who, head_manager);

    if (!cur || !cur->getParent())
    {
        return false;
    }

    Person *boss = cur->getParent();

    if (!cur || !boss)
    {
        return false;
    }

    std::vector<Person *> cur_subordinates = cur->getSubordinates();
    boss->removeSubordinate(who);

    for (std::size_t i = 0; i < cur_subordinates.size(); i++)
    {
        boss->addSubordinate(cur_subordinates[i]);
    }

    delete cur;

    // update iterator

    total_employees--;
    return true;
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

    if (head_manager)
    {
        help(level, head_manager, count);
    }

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

Person *Hierarchy::getPerson(const string &name, Person *current) const
{
    if (!current)
    {
        return nullptr;
    }

    if (current->getName() == name)
    {
        return current;
    }

    std::vector<Person *> subordinates = current->getSubordinates();

    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        Person *returned = getPerson(name, subordinates[i]);

        if (returned)
        {
            return returned;
        }
    }

    return nullptr;
}

int Hierarchy::longest_chain() const
{
    if (!head_manager)
    {
        return 0;
    }

    return calculateLongest(head_manager);
}

void printLevelOrder(Person *head, std::string &output)
{
    if (head == nullptr)
        return;

    // queue to store elements of on current level
    std::queue<Person *> queue;
    queue.push(head);

    while (!queue.empty())
    {
        // get the number of nodes on the current row
        int nodeCount = queue.size();

        while (nodeCount > 0)
        {
            // add to string
            Person *current = queue.front();
            if (current && current->getParent())
            {
                output += current->getParent()->getName() + "-" + current->getName() + "\n";
            }

            queue.pop();

            // add the nodes of the next row
            for (std::size_t i = 0; i < current->getSubordinates().size(); i++)
            {
                queue.push(current->getSubordinates()[i]);
            }

            nodeCount--;
        }
    }
}

void Hierarchy::printHelp(Person *current, std::string &output) const
{
    // left child, right sibling
    if (current->getParent())
    {
        output += current->getParent()->getName() + "-" + current->getName() + "\n";
    }

    if (current->nextSubordinate())
    {
        printHelp(current->nextSubordinate(), output);
    }

    if (current->firstChild())
    {
        printHelp(current->firstChild(), output);
    }
}

std::string Hierarchy::print() const
{
    std::string output;

    // printHelp(head_manager, output);
    printLevelOrder(head_manager, output);

    return output;
}

/*void Hierarchy::promote(Person *who, Person *boss, Person *old_hierarchy_boss)
{
    std::vector<Person *> siblings = old_hierarchy_boss->getSubordinates();
    std::cout << "Siblings: " << siblings.size() << std::endl;

    for (std::size_t i = 0; i < siblings.size(); i++)
    {
        if (siblings[i]->getName() != who->getName())
        {
            boss->removeSubordinate(siblings[i]->getName());
            who->addSubordinate(siblings[i]);
        }
    }
}*/

void Hierarchy::promote(Person *who, Person *boss)
{
    std::vector<Person *> siblings = boss->getSubordinates();
    std::cout << "Promoting: " << who->getName() << std::endl;

    for (std::size_t i = 0; i < siblings.size(); i++)
    {
        if (siblings[i]->getName() != who->getName())
        {
            boss->removeSubordinate(siblings[i]->getName());
            who->addSubordinate(siblings[i]);
        }
    }
}

Person *Hierarchy::getHighestSalary(const std::vector<Person *> &people)
{
    int highest = 0;
    std::size_t index = 0;

    for (std::size_t i = 0; i < people.size(); i++)
    {
        std::string name = people[i]->getName();
        int current = this->getSalary(people[i]->getName());
        if (highest < current)
        {
            highest = current;
            index = i;
        }
        // todo add for equal salaries
    }

    return people[index];
}

void Hierarchy::getForPromotion(std::vector<Person *> &to_be_promoted, std::vector<Person *> &bosses, Person *current)
{
    if (current->subordinatesNumber() > 1)
    {
        to_be_promoted.push_back(getHighestSalary(current->getSubordinates()));
        bosses.push_back(current);
    }

    std::vector<Person *> subordinates = current->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        getForPromotion(to_be_promoted, bosses, subordinates[i]);
    }
}

void Hierarchy::promoteRow(std::queue<Person *> &row)
{
    while (!row.empty())
    {
        Person *current = row.front();

        if (current->subordinatesNumber() > 1)
        {
            promote(getHighestSalary(current->getSubordinates()), current);
        }

        row.pop();
    }
}

void Hierarchy::incorporate()
{
    /*HierarchyIter *iter = this->iter();
    iter->begin();
    std::vector<Person *> bosses;
    std::vector<Person *> to_be_promoted;

    getForPromotion(to_be_promoted, bosses, head_manager);
    Hierarchy old(*this);

    for (std::size_t i = 0; i < to_be_promoted.size(); i++)
    {
        // std::cout << "Promote: " << to_be_promoted[i]->getName() << std::endl;
        // std::cout << "Parent: " << bosses[i]->getName() << std::endl;
        promote(to_be_promoted[i], bosses[i], old.getPerson(to_be_promoted[i]->getName(), old.head_manager)->getParent());
    }*/
    std::size_t level = height(head_manager);
    std::cout << level << std::endl;

    while (level > 0)
    {
        std::queue<Person *> queue = onLevel(head_manager, level);
        promoteRow(queue);
        level--;
    }
}

unsigned int getLevel(Person *current, int cur_level, std::string name)
{
    if (current->getName() == name)
        return cur_level;

    for (std::size_t i = 0; i < current->getSubordinates().size(); i++)
    {
        // go down one level
        int new_level = getLevel(current->getSubordinates()[i], cur_level + 1, name);

        if (new_level != 0)
            return new_level;
    }

    return 0;
}

std::queue<Person *> onLevel(Person *head, int cur_level)
{
    std::size_t level = 1;
    // queue to store elements of on current level
    std::queue<Person *> queue;
    queue.push(head);

    if (cur_level == level)
    {
        return queue;
    }

    while (!queue.empty())
    {
        // get the number of nodes on the current row
        int nodeCount = queue.size();

        while (nodeCount > 0)
        {

            // add to string
            Person *current = queue.front();
            queue.pop();

            // add the nodes of the next row
            for (std::size_t i = 0; i < current->getSubordinates().size(); i++)
            {
                queue.push(current->getSubordinates()[i]);
            }

            nodeCount--;
        }

        level++;

        if (level == cur_level)
        {
            return queue;
        }
    }
}

unsigned int Hierarchy::height(Person *current) const
{
    if (current == nullptr)
    {
        return 0;
    }
    /* compute the depth of each subtree */
    unsigned int total = 0;

    std::vector<Person *> subordinates = current->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        unsigned int returned = height(subordinates[i]);

        if (total < returned)
        {
            total = returned;
        }
    }

    return total + 1;
}

void Hierarchy::addFromRight(Person *p_right, const Hierarchy &l_hierarchy, Hierarchy &new_hierachy) const
{
    // add if person is only in the right hierarchy
    if (!l_hierarchy.getPerson(p_right->getName(), l_hierarchy.head_manager))
    {
        std::cout << "Enter with: " << p_right->getName() << std::endl;
        new_hierachy.hire(p_right->getName(), p_right->getParent()->getName());
    }

    std::vector<Person *> subordinates = p_right->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        addFromRight(subordinates[i], l_hierarchy, new_hierachy);
    }
}

void Hierarchy::joinHelp(Person *p_left, const Hierarchy &h_right, Hierarchy &new_hierachy) const
{
    Person *p_right = h_right.getPerson(p_left->getName(), h_right.head_manager);

    if (p_right && p_right->getParent())
    {
        if (p_left->getParent()->getName() != p_right->getParent()->getName())
        {
            unsigned int left = getLevel(head_manager, 0, p_left->getParent()->getName());
            unsigned int right = getLevel(h_right.head_manager, 0, p_right->getParent()->getName());

            if (right > left)
            {
                new_hierachy.hire(p_right->getName(), p_left->getParent()->getName());
            }
            else if (right < left)
            {
                new_hierachy.hire(p_right->getName(), p_right->getParent()->getName());
            }
            else
            {
                // compare
                (p_left->getParent()->getName() > p_right->getParent()->getName())
                    ? new_hierachy.hire(p_right->getName(), p_right->getParent()->getName())
                    : new_hierachy.hire(p_left->getName(), p_left->getParent()->getName());
            }
        }
        else
        {
            std::cout << "Hiring " << p_left->getName() << std::endl;
            new_hierachy.hire(p_left->getName(), p_left->getParent()->getName());
        }
    }
    else if(p_left->getParent())
    {
        std::cout << "Hiring " << p_left->getName() << std::endl;
        new_hierachy.hire(p_left->getName(), p_left->getParent()->getName());
    }

    std::vector<Person *> subordinates = p_left->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        joinHelp(subordinates[i], h_right, new_hierachy);
    }
}

Hierarchy Hierarchy::join(const Hierarchy &right) const
{
    std::cout << "!!!!!!!!!!!!!!!" << std::endl;
    Hierarchy new_hierarchy(new Person(head_manager->getName(), nullptr));
    addFromRight(right.head_manager, *this, new_hierarchy);
    std::cout << "!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    joinHelp(this->head_manager, right, new_hierarchy);

    return new_hierarchy;
}

void Hierarchy::demote(Person *who, Person *boss)
{
    std::vector<Person *> subordinates = who->getSubordinates();
    for (std::size_t i = 0; i < subordinates.size(); i++)
    {
        boss->addSubordinate(subordinates[i]);
        who->removeSubordinate(subordinates[i]->getName());
    }

    fire(who->getName());
}

void Hierarchy::modernize()
{
    // levels from root
    int level = height(head_manager);

    level = (level % 2 == 0) ? level : level - 1;

    std::cout << "at level: " << level;

    std::queue<Person *> current;

    while (level > 1)
    {
        std::cout << level << std::endl;
        current = onLevel(head_manager, level);

        while (!current.empty())
        {
            // demote only if team leader
            if (current.front()->subordinatesNumber() > 0)
            {
                demote(current.front(), current.front()->getParent());
            }

            current.pop();
        }

        level -= 2;
    }
}
// not fully implemented
Hierarchy::~Hierarchy()
{
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
    return cur;
}

Person *HierarchyIter::firstChild()
{
    // TODO: throw
    if (cur->subordinates.empty())
    {
        sibling_index = 0;
        previous = cur;
        cur = nullptr;

        return nullptr;
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
        return nullptr;
    }

    sibling_index++;

    cur = previous->subordinates[sibling_index];

    LOG std::cout << "Loading person: " << cur->name << "||Sibling " << sibling_index << std::endl;
    return cur;
}

Person *HierarchyIter::parent()
{
    /*if (previous == nullptr)
    {
        throw std::invalid_argument("No parent!");
    }*/
    if (previous)
    {
        LOG std::cout << "Parent of " << cur->name << " is " << previous->name << std::endl;
    }

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
        // check first child subtree if such exists
        if (iter->firstChild())
        {

            return getPerson(name, iter, flag);
        }
        else
        {
            if (iter->nextSibiling())
            {
                return getPerson(name, iter, flag);
            }
            else
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