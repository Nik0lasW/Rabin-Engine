#pragma once
#include "BehaviorNode.h"

class L_MoveToAvailablePresent : public BaseNode<L_MoveToAvailablePresent>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};