#pragma once
#include "Misc/PathfindingDetails.hpp"

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */  
    enum class onList
    {
        open,
        closed,
        none
    };

    struct Node
    {
        Node* parent;
        GridPos gridPos;
        float finalCost;
        float givenCost;
        onList list;
        std::vector<Node*> neighbors;

        Node(Node* _parent, GridPos _gridpos, float _finalCost, float _givenCost, onList _list)
        {
            parent = _parent;
            gridPos = _gridpos;
            finalCost = _finalCost;
            givenCost = _givenCost;
            list = _list;
        }
    };

    /* ************************************************** */

    Node* map[40][40];

    class OpenList
    {
    private:
        std::vector<Node*> nodeList;
    public:
        void push(Node* node)
        {
            nodeList.push_back(node);
        }
        Node* pop()
        {
            if (nodeList.empty())
            {
                return nullptr;
            }
            auto minNodeIt = std::min_element(nodeList.begin(), nodeList.end(),
                [](const Node* a, const Node* b) {
                    return a->finalCost < b->finalCost;
                }
            );

            Node* minNode = *minNodeIt;
            nodeList.erase(minNodeIt);

            return minNode;
        }
        void clear()
        {
            nodeList.clear();
        }
        void update(Node* node)
        {

        }
        void remove(Node* node)
        {
            auto it = std::remove(nodeList.begin(), nodeList.end(), node);
            nodeList.erase(it, nodeList.end());
        }
        bool empty() const
        {
            return nodeList.empty();
        }
    };

    OpenList openList;
    Heuristic heur = Heuristic::NUM_ENTRIES;

    void createMap();

    void setNeighbors();

    void clearMap();

    void deallocateMap();

    //NEED TO DO CHEB, EUCLID, AND OCTILE
    float manhattan(Node* currentNode, Node* goalNode);

    float chebyshev(Node* currentNode, Node* goalNode);

    float euclidean(Node* currentNode, Node* goalNode);

    float octile(Node* currentNode, Node* goalNode);

    float inconsistent(Node* currentNode, Node* goalNode);
};