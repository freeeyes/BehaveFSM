// BehaveFSM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "Action_FSM.h"
#include "C_Action_Task_Logic.h"
#include "C_Action_Look_Task_Logic.h"
#include "Base_Task.h"

#ifdef WIN32
#include <windows.h> 
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
	//��ʼ���������
	srand((int)time(0));

	//��ʼ��״̬��
	C_Action_FSM action_fsm;
	action_fsm.Init();

	//���Գ�ʼ������
	C_Action_Param action_param;
#ifdef WIN32
	sprintf_s(action_param.m_szParam, 50, "(freeeyes){Hello my friend.}");
#else
	sprintf(action_param.m_szParam, "(freeeyes){Hello my friend.}");
#endif
	
	//����������
	C_Action_Task_Logic      obj_Action_Task_Logic;
	C_Action_Look_Task_Logic obj_Action_Look_Task_Logic;
	I_Base_Task              obj_Base_Task;

	obj_Action_Look_Task_Logic.Init(&action_fsm);
	obj_Action_Task_Logic.Init(&action_fsm);

	//��ӽڵ�ӳ����Ϣ
	obj_Base_Task.Add_Task_Node(NULL, &obj_Action_Look_Task_Logic, NODE_CLASS_SIGNAL);
	obj_Base_Task.Add_Task_Node(&obj_Action_Look_Task_Logic, &obj_Action_Task_Logic, NODE_CLASS_SIGNAL);

	//������Ϊ��
	while (true)
	{
		obj_Base_Task.Execute_Task(&action_param);

		//��Ϣ1��
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	/*
	action_fsm.Do_Event((I_Param* )&action_param);
	action_fsm.Set_FSM_Curr_State(ACTION_SIT);
	action_fsm.Do_Event((I_Param* )&action_param);
	action_fsm.Set_FSM_Curr_State(ACTION_JAMP);
	action_fsm.Do_Event((I_Param* )&action_param);
	action_fsm.Set_FSM_Curr_State(ACTION_RUN);
	action_fsm.Do_Event((I_Param* )&action_param);
	*/

	getchar();
	return 0;
}

