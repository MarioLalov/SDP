#pragma once

#include "../Tree/Tree.h"

const std::size_t ASCII_NUMBER = 256;

class TreeContainerNode
{
public:
    TreeContainerNode();
    ~TreeContainerNode();

    void Insert(TreeContainerNode* current, const std::string& name, Tree* treeToStore);
    Tree* Find(TreeContainerNode* current, const std::string& name);

private:

    TreeContainerNode* m_children[ASCII_NUMBER];
    Tree* m_storedTree = nullptr;
};

class TreeContainer
{
public:
    TreeContainer();
    ~TreeContainer();

    void Insert(const std::string& name, Tree* treeToStore);
    Tree* Find(const std::string& name);

private:
    TreeContainerNode* m_root;
};