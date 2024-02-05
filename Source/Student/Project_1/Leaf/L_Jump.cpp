#include <pch.h>
#include "L_Jump.h"
#include "Agent/BehaviorAgent.h"

void L_Jump::on_enter()
{
	rise = true;
	fall = false;
	startPos = agent->get_position();
	endPos = Vec3(agent->get_position().x, agent->get_position().y + 5.0f, agent->get_position().z);
	BehaviorNode::on_leaf_enter();
}

void L_Jump::on_update(float dt)
{
	while (rise == true)
	{
		const auto result = agent->move_toward_point(endPos, dt);

		if (result == true)
		{
			rise = false;
			fall = true;
		}
	}
	while (fall == true)
	{
		const auto result = agent->move_toward_point(startPos, dt);

		if (result == true)
		{
			on_success();
		}
	}
	display_leaf_text();
}
