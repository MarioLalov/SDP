#include "Node.h"

#include <algorithm>
#include <iostream>

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

Node::~Node()
{
    // delete all children
    for (std::size_t i = 0; i < m_children.size(); i++)
    {
        delete m_children.at(i);
    }
}