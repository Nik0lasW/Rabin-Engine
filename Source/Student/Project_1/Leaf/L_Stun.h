#pragma once
#include "BehaviorNode.h"

class L_Stun : public BaseNode<L_Stun>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float delay;
    Vec3 startScale;
};