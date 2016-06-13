#ifndef _ACTION_FSM_H
#define _ACTION_FSM_H

#include "Base_FSM.h"

//定义状态机事件ID
enum EM_STATE_ACTION_ID
{
	ACTION_STAND = 1,
	ACTION_SIT,
	ACTION_JAMP,
	ACTION_RUN,
	ACTION_LOOK,
	ACTION_ATTACK,
	ACTION_DEFANCE,
	ACTION_DROP,

};

//定义参数类
class C_Action_Param : public I_Param
{
public:
	C_Action_Param()
	{
		m_szParam[0] = '\0';
	}

	char m_szParam[50];
};

class C_Action_FSM : public I_Base_FSM
{
public:
	C_Action_FSM(void);
	~C_Action_FSM(void);

	void Init();

	int Do_Event(I_Param* p_fsm_param);

private:
	int Action_Stand(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Sit(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Jamp(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Run(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Look(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Attack(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Defance(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
	int Action_Drop(FSM_STATE_ID s_state_id, I_Param* p_fsm_param);
};
#endif

