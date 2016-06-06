#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Node.h"

//��Ϊ���б����
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16

struct _LinkNode
{
	struct _LinkNode* m_p_parent;                          //���ڵ� 
	I_Base_Task_Node* m_p_task;                            //��ǰ�ڵ�
	struct _LinkNode* m_p_child[MAX_CHILD_NODE_COUNT];     //������ӽڵ�
	int               m_nChildCount;                       //�ӽڵ�ĸ���

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

			//������ǰ�ڵ㣬�����Ƿ��Ѿ����ڴ˽ڵ�
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_task == p_task_node)
				{
					//�˽ڵ��Ѿ����ڣ�����Ҫ�ٶ������
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//Ѱ�Ҹ��ڵ��Ƿ����
			_LinkNode* p_node_parent = NULL;

			//������ǰ�ڵ��Ƿ��Ѿ��ڸ��ڵ��д���,����һ���ڵ㽨����ϵ
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_task == p_task_parent)
				{
					//�ҵ���ƥ��ĸ��ڵ�
					bl_is_valid_node = true;

					//��չ�ڵ�����ӽڵ�
					if(m_vec_link_node[i]->m_nChildCount >= MAX_CHILD_NODE_COUNT)
					{
						//�ӽڵ�����������������µ�
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
				//�����нڵ��б��Ҳ������ڵ�
				return false;
			}

			if(bl_is_need_add == true)
			{
				//����µ��ӽڵ�
				p_node_curr  = new _LinkNode();
				p_node_curr->m_p_task   = p_task_node;
				p_node_curr->m_p_parent = p_node_parent;
				m_vec_link_node.push_back(p_node_curr);
			}

			//��Ӹ��ڵ���ӽڵ������Ϣ
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
				//�Ҳ����ӽڵ��ˣ��ص����ڵ�
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
				//û�и��ڵ㣬��������׽ڵ�
				m_curr_node = m_root_node;
			}
		}

		return 0;
	}

private:
	typedef vector<_LinkNode*> vecLinkNode;
private:
	_LinkNode*  m_root_node;        //�����е���Ԫ��
	_LinkNode*  m_curr_node;        //��ǰ����Ԫ��
	vecLinkNode m_vec_link_node;    //���нڵ��б�

};

#endif
