#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Node.h"

//行为树列表基类
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16

struct _LinkNode
{
	struct _LinkNode* m_p_parent;                          //父节点 
	I_Base_Task_Node* m_p_task;                            //当前节点
	struct _LinkNode* m_p_child[MAX_CHILD_NODE_COUNT];     //下面的子节点
	int               m_nChildCount;                       //子节点的个数

	_LinkNode()
	{
		m_nChildCount  = 0;
		m_p_task       = NULL;
		m_p_parent     = NULL;

		for(int i = 0; i < MAX_CHILD_NODE_COUNT; i++)
		{
			m_p_child[i] = NULL;
		}
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

	bool Add_Task_Node(I_Base_Task_Node* p_task_parent, I_Base_Task_Node* p_task_node)
	{
		if(NULL == m_root_node)
		{
			m_root_node = new _LinkNode();
			m_root_node->m_p_parent = NULL;
			m_root_node->m_p_task = p_task_node;

			m_curr_node = m_root_node;

			m_vec_link_node.push_back(m_root_node);
		}
		else
		{
			if(NULL == p_task_parent)
			{
				return false;
			}

			_LinkNode* p_node_curr = NULL;

			//遍历当前节点，看看是否已经存在此节点
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_task == p_task_node)
				{
					//此节点已经存在，不需要再额外添加
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//寻找父节点是否存在
			_LinkNode* p_node_parent = NULL;

			//遍历当前节点是否已经在父节点中存在,和上一个节点建立关系
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_task == p_task_parent)
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
				p_node_curr  = new _LinkNode();
				p_node_curr->m_p_task   = p_task_node;
				p_node_curr->m_p_parent = p_node_parent;
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
		EM_TASK_NODE_EXECUTE em_state = m_curr_node->m_p_task->Execute_Node(p_param);
		if(em_state == TASK_NODE_NEXT)
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
		else if(em_state == TASK_NODE_BACK)
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
	typedef vector<_LinkNode*> vecLinkNode;
private:
	_LinkNode*  m_root_node;        //链表中的首元素
	_LinkNode*  m_curr_node;        //当前所在元素
	vecLinkNode m_vec_link_node;    //所有节点列表

};

#endif
