#include "Action_FSM.h"

C_Action_FSM::C_Action_FSM(void)
{
}

C_Action_FSM::~C_Action_FSM(void)
{
}

void C_Action_FSM::Init()
{
	Init_FSM_State_Count(10);
	Set_State_ID(ACTION_STAND);
	Set_State_ID(ACTION_SIT);
	Set_State_ID(ACTION_JAMP);
	Set_State_ID(ACTION_RUN);
	Set_State_ID(ACTION_LOOK);
	Set_State_ID(ACTION_ATTACK);
	Set_State_ID(ACTION_DEFANCE);
	Set_State_ID(ACTION_DROP);
	Set_FSM_Curr_State(ACTION_STAND);
}

int C_Action_FSM::Do_Event(I_Param* p_fsm_param)
{
	FSM_STATE_ID s_curr_state_id = Get_FSM_Curr_State_ID();
	FSM_FUNCTION_BEGIN(s_curr_state_id);
	FSM_FUNCTION(ACTION_STAND,   Action_Stand,   s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_SIT,     Action_Sit,     s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_JAMP,    Action_Jamp,    s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_RUN,     Action_Run,     s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_LOOK,    Action_Look,    s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_ATTACK,  Action_Attack,  s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_DEFANCE, Action_Defance, s_curr_state_id, p_fsm_param);
	FSM_FUNCTION(ACTION_DROP,    Action_Drop,    s_curr_state_id, p_fsm_param);
	FSM_FUNCTION_END;

	return 0;
}

int C_Action_FSM::Action_Stand(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Stand]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Sit(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Sit]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Jamp(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Jamp]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Run(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Run]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Look(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Look]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Attack(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Attack]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Defance(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Defance]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}

int C_Action_FSM::Action_Drop(FSM_STATE_ID s_state_id, I_Param* p_fsm_param)
{
	C_Action_Param* p_action_param = (C_Action_Param* )p_fsm_param;
	PRINT_SAFE("[Action_Drop]s_state_id=%d,param=%s.\n", s_state_id, p_action_param->m_szParam);
	return 0;
}
