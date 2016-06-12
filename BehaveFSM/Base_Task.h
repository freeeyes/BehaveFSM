#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Node.h"

//行为树列表基类
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16

enum EM_TASK_NODE_CLASS
{
	NODE_CLASS_SIGNAL = 0,    //单一执行
	NODE_CLASS_CHOOSE_ONE,    //单选一个子节点执行
	NODE_CLASS_ORDER_BY,      //顺序执行，直到其中一个节点返回TASK_NODE_EXCUTE或者TASK_NODE_NEXT
};

struct _Task_Node
{
	struct _Task_Node* m_p_parent;                          //父节点 
	struct _Task_Node* m_p_child[MAX_CHILD_NODE_COUNT];     //下面的子节点
	int               m_nChildCount;                        //子节点的个数
	EM_TASK_NODE_CLASS m_em_class;                          //节点类型
	I_Base_Task_Logic* m_p_logic;                           //节点逻辑执行模块

	_Task_Node()
	{
		m_nChildCount  = 0;
		m_p_logic      = NULL;
		m_p_parent     = NULL;
		m_em_class     = NODE_CLASS_SIGNAL;

		for(int i = 0; i < MAX_CHILD_NODE_COUNT; i++)
		{
			m_p_child[i] = NULL;
		}
	}

	void Set_Node_Class(EM_TASK_NODE_CLASS& em_class)
	{
		m_em_class = em_class;
	}

	EM_TASK_NODE_CLASS Get_Node_Class()
	{
		return m_em_class;
	}
};

class I_Base_Task
{
public:
	I_Base_Task() 
	{
		m_root_node = NULL;
	}

	virtual ~I_Base_Task() {};

	bool Add_Task_Node(I_Base_Task_Logic* p_task_parent, I_Base_Task_Logic* p_task_logic, EM_TASK_NODE_CLASS em_class)
	{
		if(NULL == m_root_node)
		{
			m_root_node = new _Task_Node();
			m_root_node->m_p_parent = NULL;
			m_root_node->m_p_logic  = p_task_logic;
			m_root_node->Set_Node_Class(em_class);

			m_curr_node = m_root_node;

			m_vec_link_node.push_back(m_root_node);
		}
		else
		{
			if(NULL == p_task_parent)
			{
				return false;
			}

			_Task_Node* p_node_curr = NULL;

			//遍历当前节点，看看是否已经存在此节点
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_logic == p_task_logic)
				{
					//此节点已经存在，不需要再额外添加
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//寻找父节点是否存在
			_Task_Node* p_node_parent = NULL;

			//遍历当前节点是否已经在父节点中存在,和上一个节点建立关系
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_logic == p_task_parent)
				{
					//找到了匹配的父节点
					bl_is_valid_node = true;

					//扩展节点添加子节点
					if(m_vec_link_node[i]->m_nChildCount >= MAX_CHILD_NODE_COUNT)
					{
						//子节点已满，不能再添加新的
						return false;
					}
					else
					{
						p_node_parent = m_vec_link_node[i];
					}
					break;
				}
			}

			if(false == bl_is_valid_node)
			{
				//在已有节点列表找不到父节点
				return false;
			}

			if(bl_is_need_add == true)
			{
				//添加新的子节点
				p_node_curr  = new _Task_Node();
				p_node_curr->m_p_logic   = p_task_logic;
				p_node_curr->m_p_parent  = p_node_parent;
				p_node_curr->Set_Node_Class(em_class);
				m_vec_link_node.push_back(p_node_curr);
			}

			//添加父节点和子节点关联信息
			p_node_parent->m_p_child[p_node_parent->m_nChildCount] = p_node_curr;
			p_node_parent->m_nChildCount++;
		}
		return true;
	}

	int Execute_Task(I_Param* p_param)
	{
		EM_TASK_LOGIC_EXECUTE em_state = m_curr_node->m_p_logic->Execute_Node(p_param);
		if(em_state == TASK_LOGIC_NEXT)
		{
			if(m_curr_node->m_nChildCount > 0)
			{
				m_curr_node = m_curr_node->m_p_child[0];
			}
			else
			{
				//找不到子节点了，回到主节点
				m_curr_node = m_root_node;
			}
		}
		else if(em_state == TASK_LOGIC_BACK)
		{
			if(NULL != m_curr_node->m_p_parent)
			{
				m_curr_node = m_curr_node->m_p_parent;
			}
			else
			{
				//没有父节点，本身就是首节点
				m_curr_node = m_root_node;
			}
		}

		return 0;
	}

private:
	typedef vector<_Task_Node*> vecLinkNode;
private:
	_Task_Node*  m_root_node;        //链表中的首元素
	_Task_Node*  m_curr_node;        //当前所在元素
	vecLinkNode  m_vec_link_node;    //所有节点列表
};

#endif
