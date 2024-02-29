#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
    createMap(map);

    setNeighbors(map);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
    deallocateMap(map);
}

PathResult AStarPather::compute_path(PathRequest &request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

    // WRITE YOUR CODE HERE
    GridPos goal = terrain->get_grid_position(request.goal);
    GridPos start = terrain->get_grid_position(request.start);
    Heuristic heur = Heuristic::NUM_ENTRIES;
    if (request.newRequest)
    {
        clearMap(map);
        openList.clear();        
        openList.push(map[start.row][start.col]);
        map[start.row][start.col]->list = AStarPather::onList::open;
        heur = request.settings.heuristic;
    }
    while (!openList.empty())
    {
        Node* parentNode = openList.pop();
        if (parentNode->gridPos == goal)
        {
            return PathResult::COMPLETE;
        }
        parentNode->list = AStarPather::onList::closed;
        for (int i = 0; i < parentNode->neighbors.size(); i++)
        {
            if (parentNode->gridPos.row == parentNode->neighbors[i]->gridPos.row || parentNode->gridPos.col == parentNode->neighbors[i]->gridPos.col)
            {
                parentNode->neighbors[i]->givenCost = parentNode->givenCost + 1;
            }
            else
            {
                parentNode->neighbors[i]->givenCost = parentNode->givenCost + (float)std::sqrt(2);
            }
            float heurcost = 0.0f;
            if (heur == Heuristic::MANHATTAN)
            {
                heurcost = manhattan(parentNode->neighbors[i], map[goal.row][goal.col]);
            }
            if (heur == Heuristic::CHEBYSHEV)
            {
                heurcost = chebyshev(parentNode->neighbors[i], map[goal.row][goal.col]);
            }
            if (heur == Heuristic::EUCLIDEAN)
            {
                heurcost = euclidean(parentNode->neighbors[i], map[goal.row][goal.col]);
            }
            if (heur == Heuristic::OCTILE)
            {
                heurcost = octile(parentNode->neighbors[i], map[goal.row][goal.col]);
            }
            if (heur == Heuristic::INCONSISTENT)
            {
                heurcost = inconsistent(parentNode->neighbors[i], map[goal.row][goal.col]);
            }
            float prevFinal = parentNode->neighbors[i]->finalCost;
            //need to add weight to equation (h(x) * weight)
            parentNode->neighbors[i]->finalCost = parentNode->neighbors[i]->givenCost + heurcost;
            if (parentNode->neighbors[i]->list == AStarPather::onList::none)
            {
                parentNode->neighbors[i]->list = AStarPather::onList::open;
                openList.push(parentNode->neighbors[i]);
            }
            else if (parentNode->neighbors[i]->finalCost < prevFinal)
            {
                openList.remove(parentNode->neighbors[i]);
                parentNode->neighbors[i]->list = AStarPather::onList::open;
                openList.push(parentNode->neighbors[i]);
            }
            else
            {
                parentNode->neighbors[i]->finalCost = prevFinal;
            }
        }
        if (request.settings.singleStep == true)
        {
            return PathResult::PROCESSING;
        }
    }
    return PathResult::IMPOSSIBLE;
    
    
    // Just sample code, safe to delete
    
    //GridPos goal = terrain->get_grid_position(request.goal);
    //terrain->set_color(start, Colors::Orange);
    //terrain->set_color(goal, Colors::Orange);
    //request.path.push_back(request.start);
    //request.path.push_back(request.goal);
    //return PathResult::COMPLETE;
}

void AStarPather::createMap(Node* map[40][40])
{
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            map[i][j] = new Node(nullptr, { i, j }, 0.0f, 0.0f, AStarPather::onList::none);
        }
    }
}

void AStarPather::setNeighbors(Node* map[40][40])
{
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            Node* curr = map[i][j];
            curr->neighbors.clear();

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int neighborRow = i + x;
                    int neighborCol = j + y;
                    //order SE, E, NE, S, N, SW, W, NW
                    if (neighborRow >= 0 && neighborRow < 40 && neighborCol >= 0 && neighborCol < 40 && !(x == 0 && y == 0))
                    {
                        curr->neighbors.push_back(map[neighborRow][neighborCol]);
                    }
                }
            }
        }
    }
}

void AStarPather::clearMap(Node* map[40][40])
{
    for (int i = 0; i < 40; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            map[i][j]->parent = nullptr;
            map[i][j]->finalCost = std::numeric_limits<float>::infinity();
            map[i][j]->givenCost = std::numeric_limits<float>::infinity();
            map[i][j]->list = AStarPather::onList::none;
        }
    }
}

void AStarPather::deallocateMap(Node* map[40][40])
{
    for (int i = 0; i < 40; ++i) 
    {
        for (int j = 0; j < 40; ++j) 
        {
            delete map[i][j];
        }
    }
}

float AStarPather::manhattan(Node* currentNode, Node* goalNode)
{
    float xDiff = (float)std::abs(currentNode->gridPos.row - goalNode->gridPos.row);
    float yDiff = (float)std::abs(currentNode->gridPos.col - goalNode->gridPos.col);
    float result = xDiff + yDiff;
    return result;
}

float AStarPather::chebyshev(Node* currentNode, Node* goalNode)
{
    float xDiff = (float)std::abs(currentNode->gridPos.row - goalNode->gridPos.row);
    float yDiff = (float)std::abs(currentNode->gridPos.col - goalNode->gridPos.col);
    float result = std::max(xDiff, yDiff);
    return result;
}

float AStarPather::euclidean(Node* currentNode, Node* goalNode)
{
    float xDiff = (float)std::abs(currentNode->gridPos.row - goalNode->gridPos.row);
    float yDiff = (float)std::abs(currentNode->gridPos.col - goalNode->gridPos.col);
    float result = (float)std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
    return result;
}

float AStarPather::octile(Node* currentNode, Node* goalNode)
{
    float xDiff = (float)std::abs(currentNode->gridPos.row - goalNode->gridPos.row);
    float yDiff = (float)std::abs(currentNode->gridPos.col - goalNode->gridPos.col);
    float result = std::min(xDiff, yDiff) * (float)std::sqrt(2) + std::max(xDiff, yDiff) - std::min(xDiff, yDiff);
    return result;
}

float AStarPather::inconsistent(Node* currentNode, Node* goalNode)
{
    if ((currentNode->gridPos.row + currentNode->gridPos.col) % 2 > 0)
    {
        return euclidean(currentNode, goalNode);
    }
    return 0.0f;
}

