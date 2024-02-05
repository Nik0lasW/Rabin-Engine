#pragma once
#include "BehaviorNode.h"

class L_MoveToTree : public BaseNode<L_MoveToTree>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;

};