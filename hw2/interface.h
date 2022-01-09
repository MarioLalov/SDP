#pragma once
#include <string>
#include <vector>
#include <queue>

using std::string;

class Person
{
private:
    Person *parent;
    string name;
    std::vector<Person *> subordinates;

public:
    Person(const std::string in_name, Person *boss);

    string getName() const;
    std::vector<Person *> getSubordinates() const;
    std::size_t subordinatesNumber() const;
    Person *getParent() const;

    void addSubordinate(Person *new_subordiante);
    void removeSubordinate(const std::string &who);

    ~Person();
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

    // add head manager only
    Hierarchy(Person *head);

    // promotion and demotion of people
    void promote(Person *who, Person *boss);
    void demote(Person *who, Person *boss);

    // get highest salary from vector of people
    Person *getHighestSalary(const std::vector<Person *> &people);

    // get person by name
    Person *getPerson(const string &name, Person *current) const;

    // join helpers
    // add all people that exist in the right tree only
    void addFromRight(Person *p_right, const Hierarchy &l_hierarchy, Hierarchy &new_hierachy) const;
    // add people that exist in left or both hierarchies
    void joinHelp(Person *left, const Hierarchy &h_right, Hierarchy &new_hierachy) const;

    // get height of tree
    unsigned int height(Person *current) const;

    // retrun both direct and indirect subordinates
    int num_all_subordinates(const string &name) const;

    // promote team leaders on current row
    void promoteRow(std::queue<Person *> &row);

private:
    Person *head_manager = nullptr;
    std::size_t total_employees = 0;

    // traverse tree for copy constructor
    void traverse(const Person *current);

    // count people in hierarchy
    void count(Person *cur, unsigned int &employees) const;

    // helper for num_overloaded()
    void help_overloaded(int level, Person *cur, int &count) const;

    // returns the longest chain
    int calculateLongest(Person *cur) const;

    // returns subtree with head as root
    unsigned int getSubtreeEmplyees(Person *head) const;
};

// helpers
std::string getSubstring(const std::string &str, std::size_t cur_pos, char stmbl);

// returns all people on current level
std::queue<Person *> onLevel(Person *head, int cur_level);