#pragma once

#include <vector>

class Node
{
public:
    Node(int value, Node* parent);
    ~Node();

    int GetValue() const {return m_value;}
    std::vector<Node *> GetChildren() const {return m_children;}
    std::size_t GetChildrenNumber() const {return m_children.size();}
    Node* GetParent() const {return m_parent;}

    void SetParent(Node* parent){m_parent = parent;};

    void AddChild(Node* childToBeAdded);

    void SortChildren();

private:
    void _sortChildrenHelper(Node* currentNode);

private:
    Node* m_parent;
    int m_value;
    std::vector<Node *> m_children;
};