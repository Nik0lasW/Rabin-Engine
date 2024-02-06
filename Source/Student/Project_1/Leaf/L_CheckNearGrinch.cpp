#include <pch.h>
#include "L_CheckNearGrinch.h"
#include "Agent/BehaviorAgent.h"

void L_CheckNearGrinch::on_update(float dt)
{
    bool targetFound = false;

    const auto& allAgents = agents->get_all_agents();

    const auto& currPos = agent->get_position();

    for (const auto& a : allAgents)
    {
        if (a->get_type() == "Grinch")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance <= 10.0f)
            {
                targetFound = true;
            }
        }
    }
    if (targetFound == true)
    {
        on_success();
    }
    else // couldn't find a viable agent
    {
        on_failure();
    }

    display_leaf_text();
}
