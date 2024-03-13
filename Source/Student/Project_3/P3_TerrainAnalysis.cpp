#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    float finalResult = std::numeric_limits<float>::infinity();
    for (int i = -1; i < mapdimensions + 1; i++)
    {
        for (int j = -1; j < mapdimensions + 1; j++)
        {
            GridPos current = { i, j };
            if (!terrain->is_valid_grid_position(current) || terrain->is_wall(current))
            {               
                float xDiff = (float)std::abs(current.row - row);
                float yDiff = (float)std::abs(current.col - col);
                float tempResult = (float)std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
                if (tempResult < finalResult)
                {
                    finalResult = tempResult;
                }
            }
        }
    }  
    return finalResult;
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    // WRITE YOUR CODE HERE
    Vec2 p0 = Vec2((float)row0, (float)col0);
    Vec2 p1 = Vec2((float)row1, (float)col1);
    Vec2 c0;
    Vec2 c1;
    bool result = true;
    for (int i = std::min(row0, row1); i <= std::max(row0, row1); i++)
    {
        for (int j = std::min(col0, col1); j <= std::max(col0, col1); j++)
        {
            GridPos temp = { i , j };
            if (terrain->is_wall(temp))
            {
                for (int k = 0; k < 4; k++)
                {
                    switch(k)
                    {
                    case 0:
                        c0 = Vec2(i - 0.55f, j + 0.5f);
                        c1 = Vec2(i + 0.55f, j + 0.5f);
                        if (line_intersect(p0, p1, c0, c1))
                        {
                            return result = false;
                        }
                    case 1:
                        c0 = Vec2(i + 0.5f, j + 0.55f);
                        c1 = Vec2(i + 0.5f, j - 0.55f);
                        if (line_intersect(p0, p1, c0, c1))
                        {
                            return result = false;
                        }
                    case 2:
                        c0 = Vec2(i - 0.55f, j - 0.5f);
                        c1 = Vec2(i + 0.55f, j - 0.5f);
                        if (line_intersect(p0, p1, c0, c1))
                        {
                            return result = false;
                        }
                    case 3:
                        c0 = Vec2(i - 0.5f, j - 0.55f);
                        c1 = Vec2(i - 0.5f, j + 0.55f);
                        if (line_intersect(p0, p1, c0, c1))
                        {
                            return result = false;
                        }
                    }
                }
                
            }           
        }
    }
    return result;
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i,j };
            if (!terrain->is_wall(current))
            {
                float distance = distance_to_closest_wall(current.row, current.col);
                layer.set_value(current, 1 / (distance * distance));
            }
        }
    }
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    float visibleScore = 0.0f;
    float finalScore = 0.0f;
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {          
            GridPos current = { i,j };
            if (!terrain->is_wall(current))
            {
                visibleScore = 0.0f;
                for (int k = 0; k < mapdimensions; k++)
                {
                    for (int l = 0; l < mapdimensions; l++)
                    {
                        if (is_clear_path(i, j, k, l))
                        {
                            visibleScore++;
                        }
                    }
                }
                finalScore = visibleScore / 160;
                if (finalScore > 1.0f)
                {
                    finalScore = 1.0f;
                }
                layer.set_value(current, finalScore);
            }           
        }
    }
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };
            if (!terrain->is_wall(current))
            {
                if (is_clear_path(row, col, i, j))
                {
                    layer.set_value(current, 1.0f);
                }
                else
                {
                    layer.set_value(current, 0.0f);
                }
            }
        }            
    }
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };
            if (!terrain->is_wall(current))
            {
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        int neighborRow = i + x;
                        int neighborCol = j + y;
                        if (layer.get_value(current) == 0.0f)
                        {
                            if (current.row != neighborRow && current.col != neighborCol)
                            {
                                if (!terrain->is_wall({ neighborRow, j }) && !terrain->is_wall({ i, neighborCol }))
                                {
                                    if (layer.get_value({ neighborRow, neighborCol }) == 1.0f)
                                    {
                                        layer.set_value(current, 0.5f);
                                    }
                                }
                            }
                            else
                            {
                                if (layer.get_value({ neighborRow, neighborCol }) == 1.0f)
                                {
                                    layer.set_value(current, 0.5f);
                                }
                            }
                            
                        }                      
                    }
                }
            }
        }
    }
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    GridPos playerPos = terrain->get_grid_position(agent->get_position());
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };
            if (!terrain->is_wall(current))
            {

                if (is_clear_path(playerPos.row, playerPos.col, current.row, current.col))
                {
                    //NEED HELP
                    layer.set_value(current, 1.0f);
                }
            }
        }
    }
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    float tempLayer[40][40];
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };

            if (!terrain->is_wall(current))
            {
                float maxInfluence = 0.0f;
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        int neighborRow = i + x;
                        int neighborCol = j + y;
                        //order SE, E, NE, S, N, SW, W, NW
                        if (neighborRow >= 0 && neighborRow < mapdimensions && neighborCol >= 0 && neighborCol < mapdimensions && !(x == 0 && y == 0))
                        {
                            if (!terrain->is_wall({ neighborRow, neighborCol }))
                            {
                                float influence = layer.get_value({ neighborRow, neighborCol });
                                if (current.row != neighborRow && current.col != neighborCol)
                                {
                                    influence = influence * (float)std::exp(-std::sqrt(2) * 0.2);                  
                                }
                                else
                                {
                                    influence = influence * (float)std::exp(-1 * 0.2);
                                }

                                if (influence > maxInfluence)
                                {
                                    maxInfluence = influence;
                                }
                            }
                        }
                    }
                }
                float result = lerp(layer.get_value(current), maxInfluence, growth);
                tempLayer[i][j] = result;
            }           
        }
    }
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };
            if (!terrain->is_wall(current))
            {
                GridPos current = { i, j };
                layer.set_value(current, tempLayer[i][j]);
            }
        }
    }
}

void propagate_dual_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        Similar to the solo version, but the values range from -1.0 to 1.0, instead of 0.0 to 1.0

        For every cell in the given layer:

        1) Get the value of each neighbor and apply decay factor
        2) Keep the highest ABSOLUTE value from step 1
        3) Linearly interpolate from the cell's current value to the value from step 2
           with the growing factor as a coefficient.  Make use of the lerp helper function.
        4) Store the value from step 3 in a temporary layer.
           A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */

    // WRITE YOUR CODE HERE
}

void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE
    int mapdimensions = terrain->get_map_height();
    float maxInfluence = 0.0f;
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            float influence = 0.0f;
            GridPos current = { i, j };
            influence = layer.get_value(current);
            if (influence > maxInfluence)
            {
                maxInfluence = influence;
            }
        }
    }
    for (int i = 0; i < mapdimensions; i++)
    {
        for (int j = 0; j < mapdimensions; j++)
        {
            GridPos current = { i, j };
            if (layer.get_value(current) > 0)
            {
                layer.set_value(current, layer.get_value(current) / maxInfluence);
            }
        }
    }
}

void normalize_dual_occupancy(MapLayer<float> &layer)
{
    /*
        Similar to the solo version, but you need to track greatest positive value AND 
        the least (furthest from 0) negative value.

        For every cell in the given layer, if the value is currently positive divide it by the
        greatest positive value, or if the value is negative divide it by -1.0 * the least negative value
        (so that it remains a negative number).  This will keep the values in the range of [-1, 1].
    */

    // WRITE YOUR CODE HERE
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // WRITE YOUR CODE HERE
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE

    return false; // REPLACE THIS
}
