#include "Node.h"

#include <algorithm>
#include <iostream>
#include <queue>

Node::Node(int value, Node *parent)
{
    m_value = value;
    m_parent = parent;

    if (m_parent)
    {
        m_parent->AddChild(this);
    }
}

void Node::AddChild(Node* childToBeAdded)
{
    if(childToBeAdded->m_value == m_value)
        throw std::invalid_argument("Invalid child value.");

    childToBeAdded->SetParent(this);
    m_children.push_back(childToBeAdded);
}

void Node::SortChildren()
{
    _sortChildrenHelper(this);
}

void Node::_sortChildrenHelper(Node* currentNode)
{
    //std::cout << "Enter _sortChildrenHelper()" << std::endl;
    if(currentNode->m_children.empty())
        return;

    std::sort(currentNode->m_children.begin(), currentNode->m_children.end(), [](const Node* first, const Node* second) -> bool
    {
        return first->GetValue() < second->GetValue();
    });

    //std::cout << "Children Sorted" << std::endl;

    for(std::size_t i = 0; i < currentNode->m_children.size(); i++)
    {
        _sortChildrenHelper(currentNode->m_children.at(i));
    }
}

void Node::Print()
{
    // Create an empty queue for level order traversal 
    std::queue<Node *> q; 
  
    // Enqueue Root and initialize height 
    q.push(this); 
  
    while (q.empty() == false) 
    { 
        // nodeCount (queue size) indicates number
        // of nodes at current level. 
        int nodeCount = q.size(); 
  
        // Dequeue all nodes of current level and 
        // Enqueue all nodes of next level 
        while (nodeCount > 0)
        { 
            Node *node = q.front(); 
            std::cout << node->m_value << " "; 
            q.pop(); 

            for(std::size_t i = 0; i < node->m_children.size(); i++)
            {
                q.push(node->m_children.at(i));
            }
            nodeCount--; 
        } 
        std::cout << std::endl; 
    } 
}

Node* Node::CreateSumNode()
{
    int sumValue = 0;
    _createSumNodeHelper(this, sumValue);

    return new Node(sumValue, nullptr);
}

void Node::_createSumNodeHelper(Node* current, int& sumValue)
{
    if(!current)
        return;

    sumValue += current->GetValue();

    std::vector<Node*> children = current->GetChildren();
    for(std::size_t i = 0;i < children.size(); i++)
    {
        _createSumNodeHelper(children.at(i), sumValue);
    }
}

Node::~Node()
{
    // delete all children
    for (std::size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children.at(i);
    }
}