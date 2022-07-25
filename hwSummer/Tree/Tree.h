#include "../Node/Node.h"

#include <string>
#include <istream>

class Tree
{
public:
    Tree(const Tree &other);
    Tree(Node* root);

    Node* GetNode(int value, Node* current) const;
    std::vector<Node*> GetNodes(int value) const;

    bool Contains(Tree other);
    void Remove(Tree other);
    void Print();

    ~Tree() noexcept;
private:
    Node *m_root = nullptr;
    bool isDummy = false;
    
    Tree _getSubtreeCopy(Node* subtreeRoot) const;

    void _getNodesHelper(int value, Node* current, std::vector<Node*>& result) const;

    void _sortTreeChildren();
    bool _compareWith(Tree other);
    bool _compareWithHelper(Node* left, Node* right);

    // returns subtree with head as root
    unsigned int _getSubtree(Node *head) const;
    Node* _copyHelper(const Node* other);

    void _removeSubtree(const Tree& other);
    void _getNoRemoveNodes(Node* currentLeft, const Node* currentRight, std::vector<Node*>& noRemoveNodes);
    
};

Tree* CreateTree(std::istream& stream);

std::vector<int> toInts(const std::string& str);
std::vector<std::string> DivideIntoSections(const std::string& str);