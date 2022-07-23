#include "../Node/Node.h"

#include <string>
#include <istream>

class Tree
{
public:
    Tree(const Tree &other);
    Tree(Node* root);

    Node* GetNode(int value, Node* current) const;
    bool Contains(Tree other);

    ~Tree() noexcept;
private:
    Node *m_root = nullptr;
    //std::size_t total_employees = 0;

    // traverse tree for copy constructor
    void _traverse(const Node *current);
    
    Tree _getSubtreeCopy(Node* subtreeRoot) const;

    void _sortTreeChildren();
    bool _compareWith(Tree other);
    bool _compareWithHelper(Node* left, Node* right);

    // count people in hierarchy
    //void count(Person *cur, unsigned int &employees) const;

    // returns subtree with head as root
    unsigned int _getSubtree(Node *head) const;
};

Tree* CreateTree(std::istream& stream);

std::vector<int> toInts(const std::string& str);
std::vector<std::string> DivideIntoSections(const std::string& str);