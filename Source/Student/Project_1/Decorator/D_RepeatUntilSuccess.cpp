#include <pch.h>
#include "D_RepeatUntilSuccess.h"

void D_RepeatUntilSuccess::on_update(float dt)
{
    // repeat the child until 2 successes, stop on any failure
    BehaviorNode *child = children.front();

    child->tick(dt);

    if (child->succeeded() == true)
    {
        on_success();
    }
    else if (child->failed() == true)
    {
        child->set_status(NodeStatus::READY);
    }
}
