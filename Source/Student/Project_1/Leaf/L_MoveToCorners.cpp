#include <pch.h>
#include "L_MoveToCorners.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCorners::on_enter()
{
	Vec3 currentPos = agent->get_position();
	
	float distance1 = Vec3::Distance(currentPos, Vec3(0, 0, 0));
	float distance2 = Vec3::Distance(currentPos, Vec3(100, 0, 100));
	if (distance2 < distance1)
	{
		targetPoint = Vec3(100, 0, 100);
	}
	else
	{
		targetPoint = Vec3(0, 0, 0);
	}

	BehaviorNode::on_leaf_enter();
}

void L_MoveToCorners::on_update(float dt)
{
	const auto result = agent->move_toward_point(targetPoint, dt);
	
	if (result == true)
	{
		on_success();
	}
	
	display_leaf_text();
}
