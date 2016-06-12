#include "C_Action_Task_Logic.h"

C_Action_Task_Logic::C_Action_Task_Logic()
{
	m_p_action_fsm     = NULL;
	m_curr_state_index = 0;
}

C_Action_Task_Logic::~C_Action_Task_Logic()
{
}

void C_Action_Task_Logic::Set_Action_FSM(C_Action_FSM* p_action_fsm)
{
	m_p_action_fsm = p_action_fsm;
}

void C_Action_Task_Logic::Init(C_Action_FSM* p_action_fsm)
{
	_STATE_LIST_INFO obj_STATE_LIST_INFO;
	obj_STATE_LIST_INFO.m_state_id     = ACTION_STAND;
	obj_STATE_LIST_INFO.m_execute_time = 2;
	m_vec_State_List.push_back(obj_STATE_LIST_INFO);
	obj_STATE_LIST_INFO.m_state_id     = ACTION_RUN;
	obj_STATE_LIST_INFO.m_execute_time = 4;
	m_vec_State_List.push_back(obj_STATE_LIST_INFO);
	obj_STATE_LIST_INFO.m_state_id     = ACTION_SIT;
	obj_STATE_LIST_INFO.m_execute_time = 10;
	m_vec_State_List.push_back(obj_STATE_LIST_INFO);
	m_vec_State_List.push_back(obj_STATE_LIST_INFO);
	obj_STATE_LIST_INFO.m_state_id     = ACTION_JAMP;
	obj_STATE_LIST_INFO.m_execute_time = 2;
	m_vec_State_List.push_back(obj_STATE_LIST_INFO);

	Set_Action_FSM(p_action_fsm);
}

EM_TASK_LOGIC_EXECUTE C_Action_Task_Logic::Execute_Node(I_Param* p_param)
{
	//�����һ��ִ�е�ʱ��
	time_t tt_Last_Update = Get_Execute_Time();

	if(0 == tt_Last_Update)
	{
		//��һ��ִ�У�������һ��ִ��״̬
		int nIndex = Random_Number(1, (int)m_vec_State_List.size());

		//��¼��ǰ״̬
		m_curr_state_index = nIndex;

		//���õ�ǰִ��ʱ��
		Set_Execute_Time();

		//ִ��ָ��״̬��״̬��
		if(NULL != m_p_action_fsm)
		{
			m_p_action_fsm->Set_FSM_Curr_State(m_vec_State_List[nIndex].m_state_id);
			m_p_action_fsm->Do_Event(p_param);
		}

		Set_Task_Node_State(TASK_LOGIC_RUNNING);
		return TASK_LOGIC_EXCUTE;
	}
	else
	{
		if(Get_Task_Node_State() == TASK_LOGIC_FINISH)
		{
			//��������һ������¼�
			int nIndex = Random_Number(1, (int)m_vec_State_List.size());

			//��¼��ǰ״̬
			m_curr_state_index = nIndex;

			//���õ�ǰִ��ʱ��
			Set_Execute_Time();

			//ִ��ָ��״̬��״̬��
			if(NULL != m_p_action_fsm)
			{
				m_p_action_fsm->Set_FSM_Curr_State(m_vec_State_List[nIndex].m_state_id);
				m_p_action_fsm->Do_Event(p_param);
			}

			Set_Task_Node_State(TASK_LOGIC_RUNNING);
			return TASK_LOGIC_EXCUTE;
		}
		else
		{
			//�����Ƿ���Ҫ������һ��
			int n_interval = (int)(time(NULL) - tt_Last_Update);
			if(n_interval < m_vec_State_List[m_curr_state_index].m_execute_time)
			{
				//ʲô������
				return TASK_LOGIC_EXCUTE;
			}
			else
			{
				//ִ�����ˣ���Ҫִ����һ���ڵ�
				Set_Task_Node_State(TASK_LOGIC_FINISH);
				return TASK_LOGIC_NEXT;
			}
		}
	}

	
}

