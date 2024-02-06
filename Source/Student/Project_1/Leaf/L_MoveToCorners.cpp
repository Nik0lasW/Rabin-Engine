#include <pch.h>
#include "L_MoveToCorners.h"
#include "Agent/BehaviorAgent.h"

void L_MoveToCorners::on_enter()
{
	random = rand() % 4 + 1;
	switch (random)
	{
	case 1:
		targetPoint = Vec3(0, 0, 0);
		break;
	case 2:
		targetPoint = Vec3(0, 0, 100);
		break;
	case 3:
		targetPoint = Vec3(100, 0, 0);
	case 4:
		targetPoint = Vec3(100, 0, 100);
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
