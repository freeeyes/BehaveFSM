#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Logic.h"

//行为树列表基类
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16    //最大的子节点个数
#define MAX_NODE_STEP_COUNT  100   //最大的单次节点跳转次数 

enum EM_TASK_NODE_CLASS
{
	NODE_CLASS_ACTION = 0,    //执行节点 
	NODE_CLASS_SIGNAL,        //单一执行
	NODE_CLASS_CHOOSE_ONE,    //单选一个子节点执行
	NODE_CLASS_ORDER_BY,      //顺序执行，直到其中一个节点返回TASK_NODE_EXCUTE或者TASK_NODE_NEXT
};

class I_Task_Node
{
public:
	I_Task_Node()
	{
		m_n_node_id          = 0;
		m_n_child_node_count = 0;
		m_p_logic            = NULL;
		m_p_parent           = NULL;
		m_em_class           = NODE_CLASS_ACTION;                 //默认是执行节点

		for(int i = 0; i < MAX_CHILD_NODE_COUNT; i++)
		{
			m_p_child[i] = NULL;
		}
	}

	I_Task_Node(int n_node_id, EM_TASK_NODE_CLASS rm_class)
	{
		m_n_node_id          = n_node_id;
		m_n_child_node_count = 0;
		m_p_logic            = NULL;
		m_p_parent           = NULL;
		m_em_class           = rm_class;

		for(int i = 0; i < MAX_CHILD_NODE_COUNT; i++)
		{
			m_p_child[i] = NULL;
		}
	}

	virtual ~I_Task_Node()
	{

	}

	//设置节点类型
	void Set_Node_Class(EM_TASK_NODE_CLASS& em_class)
	{
		m_em_class = em_class;
	}

	//获得节点类型
	EM_TASK_NODE_CLASS Get_Node_Class()
	{
		return m_em_class;
	}

	//设置节点ID
	void Set_Node_ID(int n_node_id)
	{
		m_n_node_id = n_node_id;
	}

	int Get_Node_ID()
	{
		return m_n_node_id;
	}

	int Get_Child_Count()
	{
		return m_n_child_node_count;
	}

	void Set_Parent_Node(I_Task_Node* p_parent)
	{
		m_p_parent = p_parent;
	}

	I_Task_Node* Get_Parent_Node()
	{
		return m_p_parent;
	}

	int Set_Chlid_Node(I_Task_Node* p_child)
	{
		if(m_n_child_node_count >= MAX_CHILD_NODE_COUNT)
		{
			//子节点已满
			return -1;
		}
		else
		{
			m_p_child[m_n_child_node_count] = p_child;
			m_n_child_node_count++;
			return 0;
		}
	}

	I_Task_Node* Get_Child_Node(int n_node_id)
	{
		for(int i = 0; i < m_n_child_node_count; i++)
		{
			if(m_p_child[i]->Get_Node_ID() == n_node_id)
			{
				return m_p_child[i];
			}
		}

		return NULL;
	}

	void Set_Logic(I_Base_Task_Logic* p_ligic)
	{
		m_p_logic = p_ligic;
	}

	I_Base_Task_Logic* Get_Logic()
	{
		return m_p_logic;
	}

private:
	I_Task_Node*        m_p_parent;                          //父节点 
	I_Task_Node*        m_p_child[MAX_CHILD_NODE_COUNT];     //下面的子节点
	int                 m_n_child_node_count;                //子节点的个数
	EM_TASK_NODE_CLASS  m_em_class;                          //节点类型
	I_Base_Task_Logic*  m_p_logic;                           //节点逻辑执行模块
	int                 m_n_node_id;                         //节点ID
};

class I_Base_Task
{
public:
	I_Base_Task() 
	{
		m_root_node = NULL;
	}

	virtual ~I_Base_Task() {};

	bool Add_Task_Node(int n_node_id, I_Base_Task_Logic* p_task_parent, I_Base_Task_Logic* p_task_logic, EM_TASK_NODE_CLASS em_class)
	{
		if(NULL == m_root_node)
		{
			m_root_node = new I_Task_Node(n_node_id, em_class);
			m_root_node->Set_Parent_Node(NULL);
			m_root_node->Set_Logic(p_task_logic);

			m_curr_node = m_root_node;

			m_vec_link_node.push_back(m_root_node);
		}
		else
		{
			if(NULL == p_task_parent)
			{
				return false;
			}

			I_Task_Node* p_node_curr = NULL;

			//遍历当前节点，看看是否已经存在此节点
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->Get_Logic() == p_task_logic)
				{
					//此节点已经存在，不需要再额外添加
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//寻找父节点是否存在
			I_Task_Node* p_node_parent = NULL;

			//遍历当前节点是否已经在父节点中存在,和上一个节点建立关系
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->Get_Logic() == p_task_parent)
				{
					//找到了匹配的父节点
					bl_is_valid_node = true;

					//扩展节点添加子节点
					if(m_vec_link_node[i]->Get_Child_Count() >= MAX_CHILD_NODE_COUNT)
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
				p_node_curr  = new I_Task_Node(n_node_id, em_class);
				p_node_curr->Set_Logic(p_task_logic);
				p_node_curr->Set_Parent_Node(p_node_parent);
				m_vec_link_node.push_back(p_node_curr);
			}

			//添加父节点和子节点关联信息
			p_node_parent->Set_Chlid_Node(p_node_curr);
		}
		return true;
	}

	int Execute_Task(I_Param* p_param)
	{
		int n_node_index = 0;
		while(true)
		{
			//如果寻找执行节点次数超过上限
			if(n_node_index >= MAX_NODE_STEP_COUNT)
			{
				return -1;
			}

			//如果没有逻辑模块
			if(m_curr_node->Get_Logic() == NULL)
			{
				return -1;
			}

			_Logic_Return obj_Logic_Return = m_curr_node->Get_Logic()->Execute_Logic(p_param);
			if(obj_Logic_Return.m_em_execute == TASK_LOGIC_NEXT)
			{
				if(obj_Logic_Return.m_n_node_id != NONE_NODE_ID && m_curr_node->Get_Child_Count() > 0)
				{
					m_curr_node = m_curr_node->Get_Child_Node(obj_Logic_Return.m_n_node_id);
					if(NULL == m_curr_node)
					{
						//找不到子节点了，回到主节点
						m_curr_node = m_root_node;
					}
				}
				else
				{
					//找不到子节点了，回到主节点
					m_curr_node = m_root_node;
				}
			}
			else if(obj_Logic_Return.m_em_execute == TASK_LOGIC_BACK)
			{
				if(NULL != m_curr_node->Get_Parent_Node())
				{
					m_curr_node = m_curr_node->Get_Parent_Node();
					if(NULL == m_curr_node)
					{
						//找不到子节点了，回到主节点
						m_curr_node = m_root_node;
					}
				}
				else
				{
					//没有父节点，本身就是首节点
					m_curr_node = m_root_node;
				}
			}
			else
			{
				//进入正在执行状态
				return 0;
			}

			n_node_index++;
		}


		return 0;
	}

private:
	typedef vector<I_Task_Node*> vecLinkNode;
private:
	I_Task_Node*  m_root_node;        //链表中的首元素
	I_Task_Node*  m_curr_node;        //当前所在元素
	vecLinkNode   m_vec_link_node;    //所有节点列表
};

#endif
