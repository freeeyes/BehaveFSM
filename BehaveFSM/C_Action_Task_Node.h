#pragma once

#include "Base_Task_Node.h"
#include "Action_FSM.h"

class C_Action_Task_Node : public I_Base_Task_Node
{
public:
	C_Action_Task_Node();
	virtual ~C_Action_Task_Node();

	void Init(C_Action_FSM* p_action_fsm);

	void Set_Action_FSM(C_Action_FSM* p_action_fsm);

	EM_TASK_NODE_EXECUTE Execute_Node(I_Param* p_param);

private:
	C_Action_FSM*      m_p_action_fsm;        //当前状态机
	vec_State_List     m_vec_State_List;      //可选状态列表 
	short              m_curr_state_index;    //当前状态id在列表中的索引
};

