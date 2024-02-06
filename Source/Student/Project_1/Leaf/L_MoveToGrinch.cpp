#include <pch.h>
#include "L_MoveToGrinch.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToGrinch::on_enter()
{
	bool targetFound = false;
	Vec3 grinchPos;
	const auto& allAgents = agents->get_all_agents();

	for (const auto& a : allAgents)
	{
		if (a->get_type() == "Grinch")
		{
			const auto& agentPos = a->get_position();
			grinchPos = agentPos;
			targetFound = true;
		}
	}
	if (targetFound == true)
	{
		targetPoint = grinchPos;
		BehaviorNode::on_leaf_enter();
	}
	else // couldn't find a viable agent
	{
		on_failure();
	}
}

void L_MoveToGrinch::on_update(float dt)
{
	const auto result = agent->move_toward_point(targetPoint, dt);

	if (result == true)
	{
		if (agent->get_type() == "Cop")
		{
			const auto& allAgents = agents->get_all_agents();
			float distance = 0.0f;
			for (const auto& a : allAgents)
			{
				if (a->get_type() == "Grinch")
				{
					const auto& agentPos = a->get_position();
					distance = Vec3::Distance(targetPoint, agentPos);
				}
			}
			if (distance <= 6.0f)
			{
				on_success();
			}
			else
			{
				on_failure();
			}
		}
		else
		{
			on_success();
		}
		
		
	}

	display_leaf_text();
}
