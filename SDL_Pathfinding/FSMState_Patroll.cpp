#include "FSMState_Patroll.h"

void FSMState_Patroll::Enter(Agent* _agent)
{
    _agent->setTarget()
}

void FSMState_Patroll::Exit(Agent* _agent)
{
}

FSMState* FSMState_Patroll::Update(Agent* _agent, float _dtime)
{
    return nullptr;
}
