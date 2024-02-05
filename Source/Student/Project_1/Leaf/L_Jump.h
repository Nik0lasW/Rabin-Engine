#pragma once
#include "BehaviorNode.h"

class L_Jump : public BaseNode<L_Jump>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 startPos;
    Vec3 endPos;
};