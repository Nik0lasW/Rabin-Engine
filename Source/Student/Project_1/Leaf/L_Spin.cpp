#include <pch.h>
#include "L_Spin.h"
#include "Agent/BehaviorAgent.h"

void L_Spin::on_enter()
{
	BehaviorNode::on_leaf_enter();
}

void L_Spin::on_update(float dt)
{
	float yawRate = (360 - agent->get_yaw()) / 2.0f;
	for (float t = 0.0f; t <= 2.0f; t += dt)
	{
		agent->set_yaw(agent->get_yaw() + yawRate * dt);
		if (agent->get_yaw() >= 360.0f)
		{
			on_success();
			break;
		}
	}
}
