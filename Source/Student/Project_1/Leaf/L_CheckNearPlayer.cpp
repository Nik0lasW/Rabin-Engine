#include <pch.h>
#include "L_CheckNearPlayer.h"
#include "Agent/BehaviorAgent.h"

void L_CheckNearPlayer::on_update(float dt)
{
    bool targetFound = false;

    const auto& allAgents = agents->get_all_agents();

    const auto& currPos = agent->get_position();

    for (const auto& a : allAgents)
    {
        if (a->get_type() == "Player")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance <= 6.0f)
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
