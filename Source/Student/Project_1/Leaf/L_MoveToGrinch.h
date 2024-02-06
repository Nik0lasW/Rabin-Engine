#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_MoveToGrinch : public BaseNode<L_MoveToGrinch>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;

};