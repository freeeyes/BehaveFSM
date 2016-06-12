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
	//获得上一次执行的时间
	time_t tt_Last_Update = Get_Execute_Time();

	if(0 == tt_Last_Update)
	{
		//第一次执行，随机获得一个执行状态
		int nIndex = Random_Number(1, (int)m_vec_State_List.size());

		//记录当前状态
		m_curr_state_index = nIndex;

		//设置当前执行时间
		Set_Execute_Time();

		//执行指定状态的状态机
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
			//随机获得下一次随机事件
			int nIndex = Random_Number(1, (int)m_vec_State_List.size());

			//记录当前状态
			m_curr_state_index = nIndex;

			//设置当前执行时间
			Set_Execute_Time();

			//执行指定状态的状态机
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
			//看看是否需要进行下一步
			int n_interval = (int)(time(NULL) - tt_Last_Update);
			if(n_interval < m_vec_State_List[m_curr_state_index].m_execute_time)
			{
				//什么都不做
				return TASK_LOGIC_EXCUTE;
			}
			else
			{
				//执行完了，需要执行下一个节点
				Set_Task_Node_State(TASK_LOGIC_FINISH);
				return TASK_LOGIC_NEXT;
			}
		}
	}

	
}

