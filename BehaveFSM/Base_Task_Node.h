#ifndef _BASE_TASK_NODE_H
#define _BASE_TASK_NODE_H

//AI�Ľڵ���
//add by freeeyes

#include "Base_Conmon.h"
#include <time.h>

//�ڵ��ж����ִ��״̬
enum EM_TASK_LOGIC_EXECUTE
{
	TASK_LOGIC_EXCUTE = 0,    //����ִ��
	TASK_LOGIC_NEXT,          //��Ҫ����һ���ڵ�
	TASK_LOGIC_BACK,          //������һ���ڵ�
};

enum EM_TASK_LOGIC_STATE
{
	TASK_LOGIC_RUNNING = 0,
	TASK_LOGIC_FINISH,
};

class I_Base_Task_Logic
{
public:
	//��������ڵ㶼������һ��״̬����Ҳ����û��
	I_Base_Task_Logic()
	{	
		m_n_node_id      = 0;
		m_tt_update_time = 0;
		m_em_node_state  = TASK_LOGIC_FINISH;
	}

	virtual ~I_Base_Task_Logic()
	{
	}

	void Set_Node_Id(int n_node_id)
	{
		m_n_node_id = n_node_id;
	}

	int Get_Node_Id()
	{
		return m_n_node_id;
	}

	void Set_Execute_Time()
	{
		m_tt_update_time = time(NULL);
	}

	time_t Get_Execute_Time()
	{
		return m_tt_update_time;
	}

	//���õ�ǰ�ڵ�״̬
	void Set_Task_Node_State(EM_TASK_LOGIC_STATE em_node_state)
	{
		m_em_node_state = em_node_state;
	}

	//��õ�ǰ�ڵ�״̬
	EM_TASK_LOGIC_STATE Get_Task_Node_State()
	{
		return m_em_node_state;
	}

	//ִ�нڵ��ж�����
	virtual EM_TASK_LOGIC_EXECUTE Execute_Node(I_Param* p_param) = 0;


private:
	int    m_n_node_id;
	time_t m_tt_update_time;
	EM_TASK_LOGIC_STATE m_em_node_state;
};

#endif
