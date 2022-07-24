#include "Tree.h"

#include <queue>
#include <iostream>

Tree::Tree(const Tree &other)
{
    m_root = _copyHelper(other.m_root);
}

Node* Tree::_copyHelper(const Node* other)
{
    if(other == nullptr)
    {
        return nullptr;
    }

    Node* newNode = new Node(other->GetValue(), nullptr);

    const std::vector<Node*> otherChildren = other->GetChildren();
    for(std::size_t i = 0; i < otherChildren.size(); i++)
    {
        Node* copiedNode = _copyHelper(otherChildren.at(i));

        if(copiedNode)
            newNode->AddChild(copiedNode);
    }

    return newNode;
}

Tree::Tree(Node* root)
{
    m_root = root;
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

std::vector<Node*> Tree::GetNodes(int value) const
{
    std::vector<Node*> result;
    _getNodesHelper(value, m_root, result);
    std::cout << "Root Nodes: " << result.size() << std::endl;

    return result;
}

void Tree::_getNodesHelper(int value, Node* current, std::vector<Node*>& result) const
{
    if(!current)
        return;

    if(value == current->GetValue())
        result.push_back(current);

    std::vector<Node*> children = current->GetChildren();
    for(std::size_t i = 0; i < children.size(); i++)
    {
        _getNodesHelper(value, children.at(i), result);
    }
}

bool Tree::Contains(Tree other)
{
    //do for all nodes with such value
    std::vector<Node*> rootNodes = GetNodes(other.m_root->GetValue());
    std::cout << "Root Nodes: " << rootNodes.size() << std::endl;
    for(std::size_t i = 0; i < rootNodes.size(); i++)
    {
        Tree treeCopy = _getSubtreeCopy(rootNodes.at(i));
        if(treeCopy._compareWith(other))
        {
            return true;
        }
    }

    return false;
}

void Tree::Remove(Tree other)
{
    while(this->Contains(other))
    {
        _removeSubtree(other);
    }
}

void Tree::_removeSubtree(const Tree& other)
{
    std::vector<Node*> rootNodes = GetNodes(other.m_root->GetValue());
    for(std::size_t i = 0; i < rootNodes.size(); i++)
    {
        Tree treeCopy = _getSubtreeCopy(rootNodes.at(i));
        if(treeCopy._compareWith(other))
        {
            
            Node* parentNode = treeCopy.m_root->GetParent();

            std::vector<Node*> noRemoveNodes;
            _getNoRemoveNodes(treeCopy.m_root, other.m_root, noRemoveNodes);

            for(std::size_t i = 0; i < noRemoveNodes.size(); i++)
            {
                Node* sumNode = noRemoveNodes.at(i)->CreateSumNode();
                try
                {
                    parentNode->AddChild(sumNode);
                }
                catch(const std::invalid_argument&)
                {
                    delete sumNode;
                }
            }

            delete rootNodes.at(i);
        }
    }
}

void Tree::_getNoRemoveNodes(Node* left, const Node* right, std::vector<Node*>& noRemoveNodes)
{
    std::vector<Node*> lChildren = left->GetChildren();
    std::vector<Node*> rChildren = right->GetChildren();

    bool isPresent = false;
    for(std::size_t i = 0; i < lChildren.size(); i++)
    {
        for(std::size_t j = 0; j < rChildren.size(); j++)
        {
            if(lChildren.at(i)->GetValue() == rChildren.at(j)->GetValue())
            {
                isPresent = true;
            }
        }

        if(!isPresent)
        {
            // detach
            lChildren.at(i)->SetParent(nullptr);
            noRemoveNodes.push_back(lChildren.at(i));
        }

        isPresent = false;
    }

    for(std::size_t i = 0; i < rChildren.size(); i++)
    {
        // get correct children from left
        for(std::size_t j = 0; j < lChildren.size(); j++)
        {
            if(lChildren.at(i)->GetValue() == rChildren.at(j)->GetValue())
            {
                _getNoRemoveNodes(lChildren.at(j), rChildren.at(i), noRemoveNodes);
            }
        }
    }
}

Tree Tree::_getSubtreeCopy(Node* subtreeRoot) const
{
    Tree dummyTree(subtreeRoot);
    Tree copyTree(dummyTree);

    return copyTree;
}

void Tree::_sortTreeChildren()
{
    m_root->SortChildren();
}

bool Tree::_compareWith(Tree other)
{
    return _compareWithHelper(this->m_root, other.m_root);
}

bool Tree::_compareWithHelper(Node* left, Node* right)
{
    if(left->GetValue() != right->GetValue())
    {
        return false;
    }

    //compare children
    std::vector<Node*> lChildren = left->GetChildren();
    std::vector<Node*> rChildren = right->GetChildren();
    
    //compare children values
    bool isPresent = false;
    for(std::size_t i = 0; i < lChildren.size(); i++)
    {
        for(std::size_t j = 0; j < rChildren.size(); j++)
        {
            if(lChildren.at(i)->GetValue() == rChildren.at(j)->GetValue())
            {
                isPresent = true;
            }
        }

        if(!isPresent)
        {
            lChildren.erase(lChildren.begin() + i);
        }

        isPresent = false;
    }

    if(lChildren.size() != rChildren.size())
    {
        return false;
    }

    for(std::size_t i = 0; i < rChildren.size(); i++)
    {
        // if its not contained in a child
        if(!_compareWithHelper(lChildren.at(i), rChildren.at(i)))
        {
            return false;
        }
    }

    return true;
}

Tree::~Tree()
{
    if (m_root)
    {
        delete m_root;
    }
}

void Tree::Print()
{
    m_root->Print();
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