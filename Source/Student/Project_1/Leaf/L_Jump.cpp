#include <pch.h>
#include "L_Jump.h"
#include "Agent/BehaviorAgent.h"

void L_Jump::on_enter()
{
	startPos = agent->get_position();
	endPos = Vec3(startPos.x, startPos.y + 1.0f, startPos.z);
	BehaviorNode::on_leaf_enter();
}

void L_Jump::on_update(float dt)
{

	on_success();
	display_leaf_text();
}
