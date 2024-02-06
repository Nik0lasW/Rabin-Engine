#pragma once
#include "BehaviorNode.h"
#include <Misc/NiceTypes.h>

class L_MoveToCorners : public BaseNode<L_MoveToCorners>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
private:
    int random;
    Vec3 targetPoint;
};