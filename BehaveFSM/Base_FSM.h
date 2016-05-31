#ifndef _BASE_FSM_H
#define _BASE_FSM_H

#include "Base_Conmon.h"

//基础的FSM基类
//add by freeeyes

class I_Base_FSM
{
public:
	I_Base_FSM() 
	{
		m_s_fsm_id               = 0; 
		m_s_fsm_state_count      = 0;
		m_curr_state_id          = 0;
		m_s_fsm_curr_state_index = 0; 
		m_p_state_list           = NULL;
	}

	virtual ~I_Base_FSM()
	{
		Close_FSM();
	};

	//设置当前状态机ID
	void Set_FSM_ID(FSM_ID s_fsm_id)
	{
		m_s_fsm_id = s_fsm_id;
	}

	//得到当前状态机的ID
	FSM_ID Get_FSM_ID()
	{
		return m_s_fsm_id;
	}

	//获得当前状态值
	FSM_STATE_ID Get_FSM_Curr_State_ID()
	{
		return m_curr_state_id;
	}

	//设置当前状态值
	int Set_FSM_Curr_State(FSM_STATE_ID s_fsm_state_id)
	{
		if(NULL == m_p_state_list)
		{
			return -1;
		}

		if(0 != Check_State_ID_Exist(s_fsm_state_id))
		{
			return -1;
		}

		m_curr_state_id = s_fsm_state_id;
		return 0;
	};

	//执行当前事件
	virtual int Do_Event(I_Param* p_fsm_param)               = 0;

protected:
	//删除所有的fsm列表
	void Close_FSM()
	{
		if(NULL != m_p_state_list)
		{
			delete [] m_p_state_list;
			m_s_fsm_state_count = 0;
			m_p_state_list      = NULL;
		}
		m_curr_state_id          = 0;
		m_s_fsm_curr_state_index = 0; 
	}

	//初始化状态的个数
	void Init_FSM_State_Count(short s_state_count)
	{
		Close_FSM();
		m_p_state_list      = new FSM_STATE_ID[s_state_count];
		m_s_fsm_state_count = s_state_count;
	}

	//设置状态
	int Set_State_ID(FSM_STATE_ID s_fsm_state_id)
	{
		if(NULL == m_p_state_list || m_s_fsm_curr_state_index >= m_s_fsm_state_count)
		{
			return -1;
		}
		else
		{
			m_p_state_list[m_s_fsm_curr_state_index] = s_fsm_state_id;
			m_s_fsm_curr_state_index++;
			return 0;
		}
	}

private:
	int Check_State_ID_Exist(FSM_STATE_ID s_fsm_state_id)
	{
		for(short i = 0; i < m_s_fsm_curr_state_index; i++)
		{
			if(m_p_state_list[i] == s_fsm_state_id)
			{
				return 0;
			}
		}

		return -1;
	}

private:
	FSM_ID        m_s_fsm_id;                  //状态机编号
	short         m_s_fsm_state_count;         //状态机包含可用状态个数
	short         m_s_fsm_curr_state_index;    //当前index
	FSM_STATE_ID* m_p_state_list;              //状态机可用状态列表
	FSM_STATE_ID  m_curr_state_id;             //当前状态
};

#endif 

