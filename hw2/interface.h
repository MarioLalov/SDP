#pragma once
#include <string>
#include <vector>

using std::string;

const unsigned int PARENT = 1;

//typedef bool (*rule)(const Person* cur);

class HierarchyIter;

class Person
{
private:
    string name;
    std::vector<Person *> subordinates;

public:
    Person(const std::string in_name, Person *boss);
    void addSubordinate(Person *new_subordiante);
    string getName() const;
    std::vector<Person *> getSubordinates() const;
    std::size_t subordinatesNumber() const;

    void removeSubordinate(const std::string &who);

    friend class HierarchyIter;
};

class Hierarchy
{
public:
    Hierarchy(Hierarchy &&r) noexcept;
    Hierarchy(const Hierarchy &r);
    Hierarchy(const string &data); // done
    ~Hierarchy() noexcept;//in deveopment
    void operator=(const Hierarchy &) = delete;

    string print() const;

    int longest_chain() const; //done
    bool find(const string &name) const; // done
    int num_employees() const;           // done
    int num_overloaded(int level = 20) const;//done

    string manager(const string &name) const;         // done
    int num_subordinates(const string &name) const;   // done
    unsigned long getSalary(const string &who) const; // done

    bool fire(const string &who);                     // done
    bool hire(const string &who, const string &boss); // done

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy &right) const;

    // If you need it - add more public methods here
    Hierarchy(Person *head);
    HierarchyIter *iter() const;

private:
    Person *head_manager = nullptr;
    std::size_t total_employees = 0;
    HierarchyIter *iterator = nullptr;
    // add iter as a a property
    //  Add whatever you need here
    void count(Person *cur);
    void help(int level, Person* cur, int& count) const;
    int calculateLongest(Person* cur) const;
    void printHelp(HierarchyIter* iter, std::string& output) const;
    Hierarchy *getSubtree(Person *head) const;
    friend class HierarchyIter;
};

class HierarchyIter
{
private:
    std::size_t sibling_index;
    Person* root;
    Person *previous;
    Person *cur;

public:
    HierarchyIter(const Hierarchy *hierarchy);
    Person *begin();
    Person *current();
    Person *firstChild();
    Person *nextSibiling();
    Person *parent();
};

// helpers
std::string getSubstring(const std::string &str, std::size_t cur_pos, char stmbl);
Person *getPerson(const std::string &name, HierarchyIter *iter, unsigned int flag = 0);