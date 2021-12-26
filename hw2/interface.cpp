#include "interface.h"
#include <iostream>
#include <algorithm>
#include <queue>

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
    parent = boss;

    if (boss)
    {
        boss->addSubordinate(this);
    }
}

void Person::addSubordinate(Person *new_subordiante)
{
    LOG std::cout << "Subordinate added to " << name << ": " << new_subordiante->name << std::endl;

    new_subordiante->parent = this;
    subordinates.push_back(new_subordiante);

    // std::sort(subordinates.begin(), subordinates.end());
    std::sort(subordinates.begin(), subordinates.end(), [](const Person *lhs, const Person *rhs)
              { return lhs->getName() < rhs->getName(); });
}

string Person::getName() const
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
        if (subordinates[i]->name == who)
        {
            // subordinates[i]->parent = nullptr;
            subordinates.erase(subordinates.begin() + i);

            return;
        }
    }
}

Person *Person::nextSubordinate() const
{
    if (this->parent)
    {
        for (std::size_t i = 0; i < this->parent->subordinates.size(); i++)
        {
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
    if (this->subordinates.size() > 0)
    {
        return this->subordinates[0];
    }

    return nullptr;
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

void Hierarchy::traverse(const Person *current_r)
{
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
    // std::cout << "enter";

    traverse(r.head_manager);
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

            // new Person(subordinate_name, getPerson(boss_name, iter));
            new Person(subordinate_name, this->getPerson(boss_name, head_manager));
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

    return this->getPerson(name, head_manager);
}

string Hierarchy::manager(const string &name) const
{
    // TODO: throw
    HierarchyIter *iter = this->iter();
    iter->begin();

    // return getPerson(name, head_manager, PARENT)->getName();
    return getPerson(name, head_manager)->getParent()->getName();
}

int Hierarchy::num_subordinates(const string &name) const
{
    // TODO: return negative if nullptr
    HierarchyIter *iter = this->iter();
    iter->begin();

    Hierarchy subtree(getPerson(name, head_manager));

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

    unsigned int direct_subordinates = getPerson(who, head_manager)->subordinatesNumber();
    std::cout << "!!!!GOT PEROSN " << who << std::endl;

    if (direct_subordinates == 0)
    {
        return 0;
    }

    unsigned int indirect_subordinates = num_subordinates(who) - direct_subordinates;

    return 500 * direct_subordinates + 50 * indirect_subordinates;
}

bool Hierarchy::hire(const string &who, const string &boss)
{
    HierarchyIter *iter = this->iter();
    iter->begin();

    Person *p = getPerson(boss, head_manager);

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

    Person *boss = getPerson(who, head_manager)->getParent();
    Person *cur = getPerson(who, head_manager);

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

Person *Hierarchy::getPerson(const string &name, Person *current) const
{
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
            if (current->getParent())
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
    HierarchyIter *iter = this->iter();
    iter->begin();
    std::string output;

    // printHelp(head_manager, output);
    printLevelOrder(head_manager, output);

    return output;
}

void Hierarchy::promote(Person *who, Person *boss, Person *old_hierarchy_boss)
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

void Hierarchy::incorporate()
{
    HierarchyIter *iter = this->iter();
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

void Hierarchy::addFromRight(Person *p_right, const Hierarchy &l_hierarchy, Hierarchy &new_hierachy) const
{
    // add if person is only in the right hierarchy
    if (!l_hierarchy.getPerson(p_right->getName(), l_hierarchy.head_manager))
    {
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
            new_hierachy.hire(p_left->getName(), p_left->getParent()->getName());
        }
    }
    else if (p_right->getParent())
    {
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
    Hierarchy new_hierarchy(new Person(head_manager->getName(), nullptr));
    addFromRight(right.head_manager, *this, new_hierarchy);
    joinHelp(this->head_manager, right, new_hierarchy);

    return new_hierarchy;
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