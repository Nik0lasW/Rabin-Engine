#include <pch.h>
#include "L_MoveToAvailablePresent.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToAvailablePresent::on_enter()
{
    float maxClosest = 30.0f;
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

            if (distance <= maxClosest)
            {
                maxClosest = distance;
                ClosestPoint = agentPos;
                targetFound = true;
            }
        }
    }
    for (const auto& a : allAgents)
    {
        if (a->get_type() == "Enemy" || a->get_type() == "Tree")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(ClosestPoint, agentPos);
            if (distance < maxClosest)
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
