#ifndef _BASE_COMMON_H
#define _BASE_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
using namespace std;

#define FSM_FUNCTION_BEGIN(x) switch(x) { 
#define FSM_FUNCTION(w, x,y,z) case w: { x(y, z); break; }
#define FSM_FUNCTION_END }

typedef short FSM_STATE_ID;
typedef short FSM_ID;

struct _STATE_LIST_INFO
{
	FSM_STATE_ID       m_state_id;       //状态类型
	int                m_execute_time;   //持续时间（单位秒值） 

	_STATE_LIST_INFO()
	{
		m_state_id     = 0;
		m_execute_time = 10;
	}
};
typedef vector<_STATE_LIST_INFO> vec_State_List;

inline int Random_Number(int n_begin, int n_end)
{
	return n_begin + rand()%(n_end - n_begin);
}

//参数基类
class I_Param
{
public:
	I_Param() {};
	virtual ~I_Param() {};
}; 

#ifdef WIN32
#define PRINT_SAFE printf_s 
#else
#define PRINT_SAFE printf
#endif

static void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(szText, nLen, fmt, ap);
	szText[nLen - 1] = '\0';

	va_end(ap);
};

#endif
