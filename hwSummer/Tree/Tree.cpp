#include "Tree.h"

#include <queue>
#include <iostream>

void Tree::_traverse(const Node* current)
{
    // traverse tree and copy values
    if (current->GetParent())
    {
        new Node(current->GetValue(), GetNode(current->GetParent()->GetValue(), m_root));
    }
    else
    {
        m_root = new Node(current->GetValue(), nullptr);
    }

    std::vector<Node *> children = current->GetChildren();
    for (std::size_t i = 0; i < children.size(); i++)
    {
        _traverse(children.at(i));
    }
}

Tree::Tree(const Tree &other)
{
    // copy all elements
    _traverse(other.m_root);
    //total_employees = r.total_employees;
}

Tree::Tree(Node* root)
{
    // set head and count all subordinates of head
    m_root = root;
    //unsigned int cnt = 0;

    //count(head, cnt);
    //total_employees = 1 + cnt;
}


Node* Tree::GetNode(int value, Node* current) const
{
    // person found in subtree
    if (current->GetValue() == value)
    {
        return current;
    }

    const std::vector<Node *>& children = current->GetChildren();
    for (std::size_t i = 0; i < children.size(); i++)
    {
        return GetNode(value, children.at(i));
    }

    return nullptr;
}

bool Tree::Contains(Tree other)
{
    //do for all nodes with such value
    //std::cout << "Enter Contains()" << std::endl;
    Node* rootNode = GetNode(other.m_root->GetValue(), m_root);
    Tree treeCopy = _getSubtreeCopy(rootNode);
    return treeCopy._compareWith(other);
}

Tree Tree::_getSubtreeCopy(Node* subtreeRoot) const
{
    Tree dummyTree(subtreeRoot);

    Node* dummyParentNode = dummyTree.m_root->GetParent();
    if(dummyParentNode)
        dummyTree.m_root->SetParent(nullptr);

    Tree copyTree(dummyTree);

    dummyTree.m_root->SetParent(dummyParentNode);

    return copyTree;
}

void Tree::_sortTreeChildren()
{
    m_root->SortChildren();
}

bool Tree::_compareWith(Tree other)
{
    //std::cout << "Enter _compareWith()" << std::endl;
    this->_sortTreeChildren();
    other._sortTreeChildren();

    return _compareWithHelper(this->m_root, other.m_root);
}

bool Tree::_compareWithHelper(Node* left, Node* right)
{
    //std::cout << "Enter _compareWithHelper()" << std::endl;
    if(right->GetChildren().empty())
    {
        return true;
    }
    else if(left->GetChildren().empty())
    {
        return false;
    }
    else if(right->GetChildren().size() > left->GetChildren().size())
    {
        return false;
    }

    //compare children
    const std::vector<Node*>& lChildren = right->GetChildren();
    const std::vector<Node*>& rChildren = right->GetChildren();

    for(std::size_t i = 0; i < rChildren.size(); i++)
    {
        if(lChildren.at(i)->GetValue() != rChildren.at(i)->GetValue())
        {
            return false;
        }
    }

    
    for(std::size_t i = 0; i < rChildren.size(); i++)
    {
        _compareWithHelper(lChildren.at(i), rChildren.at(i));
    }
}

Tree::~Tree()
{
    if (m_root)
    {
        delete m_root;
    }
}


Tree* CreateTree(std::istream& stream)
{
    Tree* tree = nullptr; 

    std::vector<std::string> levels;
    std::queue<Node*> previousNodes;

    // read tree levels
    while(!stream.eof())
    {
        std::string buff;
        std::getline(stream, buff);
        levels.push_back(buff);
    }

    int cnt = 0;

    for(const auto& level : levels)
    {
        if(cnt == 0)
        {
            auto result = toInts(level);

            Node* root = new Node(result.at(0), nullptr);
            tree = new Tree(root);

            previousNodes.push(root);

            cnt++;
            continue;
        }

        std::vector<std::string> sections = DivideIntoSections(level);
        for(const auto& section : sections)
        { 
            std::vector<int> result = toInts(section);
            //std::cout << "Parent: " << previousNodes.front()->GetValue() << std::endl;
            for(const auto& value : result)
            {
                //std::cout << value;
                Node* newNode = new Node(value, previousNodes.front());
                previousNodes.push(newNode);
            }

            //std::cout << std::endl;
            previousNodes.pop();
        }

        cnt++;
    }

    return tree;
}

std::vector<int> toInts(const std::string& str)
{
    std::vector<int> result;

    std::string buff;
    for(std::size_t i = 0; i < str.size(); i++)
    {
        if(std::isdigit(str.at(i)))
        {
            buff.push_back(str.at(i));
        }
        else if(buff.empty())
        {
            continue;
        }
        else
        {
            result.push_back(std::stoi(buff));
            buff.clear();
        }
    }

    return result;
}

std::vector<std::string> DivideIntoSections(const std::string& str)
{
    std::vector<std::string> result;

    std::size_t curPos = str.find("|", 0) + 1;
    std::size_t nextPos = curPos;
    while(curPos < str.size())
    {
        nextPos = str.find("|", curPos);
        std::string buff = str.substr(curPos, nextPos-curPos);
        result.push_back(buff);

        curPos = nextPos+1;
    }

    return result;
}