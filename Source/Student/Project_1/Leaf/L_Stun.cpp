#include <pch.h>
#include "L_Stun.h"
#include "Agent/BehaviorAgent.h"

void L_Stun::on_enter()
{
	startScale = agent->get_scaling();
	agent->set_scaling(Vec3(startScale.x, (startScale.y / 2), startScale.z));
	delay = RNG::range(1.0f, 2.0f);
	BehaviorNode::on_leaf_enter();
}

void L_Stun::on_update(float dt)
{
	delay -= dt;
	if (delay < 0.0f) 
	{
		agent->set_scaling(startScale);
		on_success();
	}
}
