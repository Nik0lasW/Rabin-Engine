#include <pch.h>
#include "L_MoveToAvailablePresent.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToAvailablePresent::on_enter()
{
    float closestDistance = 30.0f;
    Vec3 ClosestPoint;
    bool targetFound = false;

	const auto& allAgents = agents->get_all_agents();

	const auto& currPos = agent->get_position();

    for (const auto& a : allAgents)
    {
        if (a->get_type() == "Present")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);
    
            if (distance <= closestDistance)
            {
                closestDistance = distance;
                ClosestPoint = agentPos;
                targetFound = true;
            }
        }
    }
    for (const auto& a : allAgents)
    {
        if (a->get_type() == "Grinch" || a->get_type() == "Tree" || a->get_type() == "Player")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(ClosestPoint, agentPos);
            if (distance < closestDistance)
            {
                targetFound = false;
            }
        }
    }
    if (targetFound == true)
    {
        targetPoint = ClosestPoint;
        BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a viable agent or closer agent
    {
        on_failure();
    }
}

void L_MoveToAvailablePresent::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}
