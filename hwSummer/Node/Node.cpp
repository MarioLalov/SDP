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
    for(const auto& child : m_children)
    {
        if(childToBeAdded->m_value == child->m_value)
            throw std::invalid_argument("Invalid child value.");
    }

    childToBeAdded->SetParent(this);
    m_children.push_back(childToBeAdded);
}

void Node::RemoveChild(int value)
{
    for(std::size_t i = 0; i < m_children.size(); i++)
    {
        if(value == m_children.at(i)->m_value)
        {
            m_children.erase(m_children.begin() + i);

            return;
        }
    }

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
    std::queue<Node *> nodeQueue;
    std::queue<Node *> parentQueue; 
  
    nodeQueue.push(this); 
    parentQueue.push(nullptr);
  
    std::cout << "| ";
    while (nodeQueue.empty() == false) 
    { 
        int nodeCount = nodeQueue.size();
        // how many inital parents
        int totalParentCount = parentQueue.size(); 
   
        while (nodeCount > 0)
        { 
            Node *node = nodeQueue.front();  
            while(!parentQueue.empty() && node->GetParent() != parentQueue.front())
            {
                std::cout << "| ";
                parentQueue.pop();
                totalParentCount--;
            }

            std::cout << node->m_value << " ";
            nodeQueue.pop(); 

            for(std::size_t i = 0; i < node->m_children.size(); i++)
            {
                nodeQueue.push(node->m_children.at(i));
            }
            parentQueue.push(node);
            nodeCount--; 
        } 

        // pop all childless parents
        while(totalParentCount > 0)
        {
            std::cout << "| ";
            parentQueue.pop();
            totalParentCount--;
        }
        //if(parentQueue.size() > 1)
          //  parentQueue.pop();
        std::cout << std::endl << "| "; 
    }

    // print leaves
    int parentsLeft = parentQueue.size(); 
    while(parentsLeft > 0)
    {
        std::cout << "| ";
        parentQueue.pop();
        parentsLeft--;
    }
}

Node* Node::CreateSumNode()
{
    int sumValue = 0;
    for(auto& node : m_children)
    {
        _createSumNodeHelper(node, sumValue);
    }

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