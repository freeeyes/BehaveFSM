#ifndef _BASE_TASK_LOGIC_H
#define _BASE_TASK_LOGIC_H

//AI的节点类
//add by freeeyes

#include "Base_Conmon.h"
#include <time.h>

//节点判定后的执行状态
enum EM_TASK_LOGIC_EXECUTE
{
	TASK_LOGIC_EXCUTE = 0,    //正在执行
	TASK_LOGIC_NEXT,          //需要走下一个节点
	TASK_LOGIC_BACK,          //返回上一个节点
};

enum EM_TASK_LOGIC_STATE
{
	TASK_LOGIC_RUNNING = 0,
	TASK_LOGIC_FINISH,
};

#define NONE_NODE_ID -1     //无节点ID

//逻辑返回信息
struct _Logic_Return
{
	int                   m_n_node_id;     //下一个节点ID
	EM_TASK_LOGIC_EXECUTE m_em_execute;    //当前节点执行状态

	_Logic_Return()
	{
		m_n_node_id  = NONE_NODE_ID;
		m_em_execute = TASK_LOGIC_NEXT;
	}
};

//节点逻辑执行模块
class I_Base_Task_Logic
{
public:
	//所有任务节点都可以有一个状态机，也可以没有
	I_Base_Task_Logic()
	{
		m_tt_update_time = 0;
		m_em_node_state  = TASK_LOGIC_FINISH;
	}

	virtual ~I_Base_Task_Logic()
	{
	}

	void Set_Execute_Time()
	{
		m_tt_update_time = time(NULL);
	}

	time_t Get_Execute_Time()
	{
		return m_tt_update_time;
	}

	//设置当前节点状态
	void Set_Task_Node_State(EM_TASK_LOGIC_STATE em_node_state)
	{
		m_em_node_state = em_node_state;
	}

	//获得当前节点状态
	EM_TASK_LOGIC_STATE Get_Task_Node_State()
	{
		return m_em_node_state;
	}

	//执行节点逻辑
	virtual _Logic_Return Execute_Logic(I_Param* p_param) = 0;


private:
	time_t m_tt_update_time;
	EM_TASK_LOGIC_STATE m_em_node_state;
};

#endif
