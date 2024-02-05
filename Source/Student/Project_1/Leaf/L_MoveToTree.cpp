#include <pch.h>
#include "L_MoveToTree.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToTree::on_enter()
{
	bool targetFound = false;
	Vec3 treePos;
	const auto& allAgents = agents->get_all_agents();

	for (const auto& a : allAgents)
	{
		if (a->get_type() == "Tree")
		{
			const auto& agentPos = a->get_position();
			treePos = agentPos;
			targetFound = true;
		}
	}
	if (targetFound == true)
	{
		targetPoint = treePos;
		BehaviorNode::on_leaf_enter();
	}
	else // couldn't find a viable agent
	{
		on_failure();
	}
}

void L_MoveToTree::on_update(float dt)
{
	const auto result = agent->move_toward_point(targetPoint, dt);

	if (result == true)
	{
		on_success();
	}

	display_leaf_text();
}
