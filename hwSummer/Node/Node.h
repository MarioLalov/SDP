#pragma once

#include <vector>

class Node
{
public:
    Node(int value, Node* parent);
    Node(const Node& other);
    ~Node();

    int GetValue() const {return m_value;}
    std::vector<Node *> GetChildren() const {return m_children;}
    std::size_t GetChildrenNumber() const {return m_children.size();}
    Node* GetParent() const {return m_parent;}

    Node* CreateSumNode();

    void SetParent(Node* parent){m_parent = parent;};

    void AddChild(Node* childToBeAdded);

    void SortChildren();

    void Print();

private:
    void _sortChildrenHelper(Node* currentNode);
    void _createSumNodeHelper(Node* current, int& sumValue);

private:
    Node* m_parent;
    int m_value;
    std::vector<Node *> m_children;
};