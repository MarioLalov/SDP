#pragma once
#include <string>
#include <vector>

using std::string;

const unsigned int PARENT = 1;

class HierarchyIter;

class Person
{
private:
    string name;
    std::vector<Person *> subordinates;

public:
    Person(const std::string in_name, Person *boss);
    void addSubordinate(Person *new_subordiante);
    //Person *traverse(const std::string &name, Person *cur) const;
    string getName() const;

    friend class HierarchyIter;
};

class Hierarchy
{
public:
    Hierarchy(Hierarchy &&r) noexcept;
    Hierarchy(const Hierarchy &r);
    Hierarchy(const string &data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy &) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string &name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string &name) const;
    int num_subordinates(const string &name) const;
    unsigned long getSalary(const string &who) const;

    bool fire(const string &who);
    bool hire(const string &who, const string &boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy &right) const;

    // If you need it - add more public methods here
    HierarchyIter *iter() const;
    //Person *getPerson(const std::string &name) const;

private:
    Person *head_manager = nullptr;
    // Add whatever you need here
    friend class HierarchyIter;
};

class HierarchyIter
{
private:
    std::size_t sibling_index;
    Person *previous;
    Person *cur;

public:
    HierarchyIter(const Hierarchy *hierarchy);
    Person *current();
    Person *firstChild();
    Person *nextSibiling();
    Person *parent();
};

// helpers
std::string getSubstring(const std::string &str, std::size_t cur_pos, char stmbl);
Person *getPerson(const std::string &name, HierarchyIter* iter, unsigned int flag = 0);