#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Node.h"

//��Ϊ���б����
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16

enum EM_TASK_NODE_CLASS
{
	NODE_CLASS_SIGNAL = 0,    //��һִ��
	NODE_CLASS_CHOOSE_ONE,    //��ѡһ���ӽڵ�ִ��
	NODE_CLASS_ORDER_BY,      //˳��ִ�У�ֱ������һ���ڵ㷵��TASK_NODE_EXCUTE����TASK_NODE_NEXT
};

struct _Task_Node
{
	struct _Task_Node* m_p_parent;                          //���ڵ� 
	struct _Task_Node* m_p_child[MAX_CHILD_NODE_COUNT];     //������ӽڵ�
	int               m_nChildCount;                        //�ӽڵ�ĸ���
	EM_TASK_NODE_CLASS m_em_class;                          //�ڵ�����
	I_Base_Task_Logic* m_p_logic;                           //�ڵ��߼�ִ��ģ��

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

			//������ǰ�ڵ㣬�����Ƿ��Ѿ����ڴ˽ڵ�
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_logic == p_task_logic)
				{
					//�˽ڵ��Ѿ����ڣ�����Ҫ�ٶ������
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//Ѱ�Ҹ��ڵ��Ƿ����
			_Task_Node* p_node_parent = NULL;

			//������ǰ�ڵ��Ƿ��Ѿ��ڸ��ڵ��д���,����һ���ڵ㽨����ϵ
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->m_p_logic == p_task_parent)
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
				p_node_curr  = new _Task_Node();
				p_node_curr->m_p_logic   = p_task_logic;
				p_node_curr->m_p_parent  = p_node_parent;
				p_node_curr->Set_Node_Class(em_class);
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
		EM_TASK_LOGIC_EXECUTE em_state = m_curr_node->m_p_logic->Execute_Node(p_param);
		if(em_state == TASK_LOGIC_NEXT)
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
		else if(em_state == TASK_LOGIC_BACK)
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
	typedef vector<_Task_Node*> vecLinkNode;
private:
	_Task_Node*  m_root_node;        //�����е���Ԫ��
	_Task_Node*  m_curr_node;        //��ǰ����Ԫ��
	vecLinkNode  m_vec_link_node;    //���нڵ��б�
};

#endif
