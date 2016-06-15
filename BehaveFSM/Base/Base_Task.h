#ifndef _BASE_TASK_H
#define _BASE_TASK_H

#include "Base_Task_Logic.h"

//��Ϊ���б����
//add by freeeyes

#define MAX_CHILD_NODE_COUNT 16    //�����ӽڵ����
#define MAX_NODE_STEP_COUNT  100   //���ĵ��νڵ���ת���� 

enum EM_TASK_NODE_CLASS
{
	NODE_CLASS_ACTION = 0,    //ִ�нڵ� 
	NODE_CLASS_SIGNAL,        //��һִ��
	NODE_CLASS_CHOOSE_ONE,    //��ѡһ���ӽڵ�ִ��
	NODE_CLASS_ORDER_BY,      //˳��ִ�У�ֱ������һ���ڵ㷵��TASK_NODE_EXCUTE����TASK_NODE_NEXT
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
		m_em_class           = NODE_CLASS_ACTION;                 //Ĭ����ִ�нڵ�

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

	//���ýڵ�����
	void Set_Node_Class(EM_TASK_NODE_CLASS& em_class)
	{
		m_em_class = em_class;
	}

	//��ýڵ�����
	EM_TASK_NODE_CLASS Get_Node_Class()
	{
		return m_em_class;
	}

	//���ýڵ�ID
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
			//�ӽڵ�����
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
	I_Task_Node*        m_p_parent;                          //���ڵ� 
	I_Task_Node*        m_p_child[MAX_CHILD_NODE_COUNT];     //������ӽڵ�
	int                 m_n_child_node_count;                //�ӽڵ�ĸ���
	EM_TASK_NODE_CLASS  m_em_class;                          //�ڵ�����
	I_Base_Task_Logic*  m_p_logic;                           //�ڵ��߼�ִ��ģ��
	int                 m_n_node_id;                         //�ڵ�ID
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

			//������ǰ�ڵ㣬�����Ƿ��Ѿ����ڴ˽ڵ�
			bool bl_is_need_add = true;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->Get_Logic() == p_task_logic)
				{
					//�˽ڵ��Ѿ����ڣ�����Ҫ�ٶ������
					bl_is_need_add = false;
					p_node_curr    = m_vec_link_node[i];
					break;
				}
			}

			//Ѱ�Ҹ��ڵ��Ƿ����
			I_Task_Node* p_node_parent = NULL;

			//������ǰ�ڵ��Ƿ��Ѿ��ڸ��ڵ��д���,����һ���ڵ㽨����ϵ
			bool bl_is_valid_node = false;
			for(int i = 0; i < (int)m_vec_link_node.size(); i++)
			{
				if(m_vec_link_node[i]->Get_Logic() == p_task_parent)
				{
					//�ҵ���ƥ��ĸ��ڵ�
					bl_is_valid_node = true;

					//��չ�ڵ�����ӽڵ�
					if(m_vec_link_node[i]->Get_Child_Count() >= MAX_CHILD_NODE_COUNT)
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
				p_node_curr  = new I_Task_Node(n_node_id, em_class);
				p_node_curr->Set_Logic(p_task_logic);
				p_node_curr->Set_Parent_Node(p_node_parent);
				m_vec_link_node.push_back(p_node_curr);
			}

			//��Ӹ��ڵ���ӽڵ������Ϣ
			p_node_parent->Set_Chlid_Node(p_node_curr);
		}
		return true;
	}

	int Execute_Task(I_Param* p_param)
	{
		int n_node_index = 0;
		while(true)
		{
			//���Ѱ��ִ�нڵ������������
			if(n_node_index >= MAX_NODE_STEP_COUNT)
			{
				return -1;
			}

			//���û���߼�ģ��
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
						//�Ҳ����ӽڵ��ˣ��ص����ڵ�
						m_curr_node = m_root_node;
					}
				}
				else
				{
					//�Ҳ����ӽڵ��ˣ��ص����ڵ�
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
						//�Ҳ����ӽڵ��ˣ��ص����ڵ�
						m_curr_node = m_root_node;
					}
				}
				else
				{
					//û�и��ڵ㣬��������׽ڵ�
					m_curr_node = m_root_node;
				}
			}
			else
			{
				//��������ִ��״̬
				return 0;
			}

			n_node_index++;
		}


		return 0;
	}

private:
	typedef vector<I_Task_Node*> vecLinkNode;
private:
	I_Task_Node*  m_root_node;        //�����е���Ԫ��
	I_Task_Node*  m_curr_node;        //��ǰ����Ԫ��
	vecLinkNode   m_vec_link_node;    //���нڵ��б�
};

#endif
