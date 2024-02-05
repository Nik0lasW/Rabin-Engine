#pragma once
#include "BehaviorNode.h"

class L_Spin : public BaseNode<L_Spin>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:

};
