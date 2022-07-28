#include "TreeContainer.h"

TreeContainerNode::TreeContainerNode()
{
    for(std::size_t i = 0; i < ASCII_NUMBER; i++)
    {
        m_children[i] = nullptr;
    }
}

void TreeContainerNode::Insert(TreeContainerNode* current, const std::string& name, Tree* treeToStore)
{
    // start from the root nod
    for (int i = 0; i < name.size(); i++)
    {
        int asciiValue = name.at(i);
        if (!current->m_children[asciiValue]) 
        {
            current->m_children[asciiValue] = new TreeContainerNode();
        }
 
        // go to the next letter
        current = current->m_children[asciiValue];
    }

    current->m_storedTree = treeToStore;
}

Tree* TreeContainerNode::Find(TreeContainerNode* current, const std::string& name)
{
    for (int i = 0; i < name.size(); i++)
    {
        int asciiValue = name.at(i);
        current = current->m_children[asciiValue];
 
        // invlaid name
        if (!current) 
        {
            return nullptr;
        }
    }
 
    // end of string reached
    return current->m_storedTree;
}

TreeContainerNode::~TreeContainerNode()
{
    for(std::size_t i = 0; i < ASCII_NUMBER; i++)
    {
        if(m_children[i])
        {
            delete m_children[i];
        }
    }
}

TreeContainer::TreeContainer()
{
    m_root = new TreeContainerNode();
}

void TreeContainer::Insert(const std::string& name, Tree* treeToStore)
{
    m_root->Insert(m_root, name, treeToStore);
}

Tree* TreeContainer::Find(const std::string& name)
{
    return m_root->Find(m_root, name);
}

TreeContainer::~TreeContainer()
{
    if(m_root)
    {
        delete m_root;
    }
}
