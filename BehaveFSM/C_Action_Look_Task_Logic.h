#pragma once

#include "Base_Task_Node.h"
#include "Action_FSM.h"

class C_Action_Look_Task_Logic : public I_Base_Task_Logic
{
public:
	C_Action_Look_Task_Logic();
	~C_Action_Look_Task_Logic();

	void Init(C_Action_FSM* p_action_fsm);

	void Set_Action_FSM(C_Action_FSM* p_action_fsm);

	EM_TASK_LOGIC_EXECUTE Execute_Node(I_Param* p_param);

private:
	C_Action_FSM*      m_p_action_fsm;        //��ǰ״̬��
	vec_State_List     m_vec_State_List;      //��ѡ״̬�б� 
	short              m_curr_state_index;    //��ǰ״̬id���б��е�����
};

