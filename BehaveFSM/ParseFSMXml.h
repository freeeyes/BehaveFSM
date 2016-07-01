#ifndef PARSE_FSM_XML_H
#define PARSE_FSM_XML_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Base/Base_Conmon.h"

//处理状态机的XML文件，变现成实际C++代码
//add by freeeyes

#define FSM_BUFF_100 100 

struct _FSM_Include
{
	char m_szInlcudeFile[FSM_BUFF_100];

	_FSM_Include()
	{
		m_szInlcudeFile[0] = '\0';
	}
};
typedef vector<_FSM_Include> vec_FSM_Include;

struct _FSM_Event
{
	char m_szEventName[FSM_BUFF_100];
	char m_szDesc[FSM_BUFF_100];

	_FSM_Event()
	{
		m_szEventName[0]  = '\0';
		m_szDesc[0]       = '\0';
	}
};
typedef vector<_FSM_Event> vec_FSM_Event;

struct _FSM_Param_Column
{
	char m_szColumnName[FSM_BUFF_100];
	char m_szClassTypeName[FSM_BUFF_100];
	int  m_nLength;
	char m_szInitValue[FSM_BUFF_100];
	char m_szDesc[FSM_BUFF_100];

	_FSM_Param_Column()
	{
		m_szColumnName[0]    = '\0';
		m_szClassTypeName[0] = '\0';
		m_szInitValue[0]     = '\0';
		m_nLength            = 0;
		m_szDesc[0]          = '\0';
	}
};
typedef vector<_FSM_Param_Column> vec_FSM_Param_Column;

struct _FSM_ParamClass
{
	char m_szClassName[FSM_BUFF_100];
	char m_desc[FSM_BUFF_100];
	vec_FSM_Param_Column m_vec_FSM_Param_Column;

	_FSM_ParamClass()
	{
		m_szClassName[0]  = '\0';
		m_desc[0]         = '\0';
	}
};

struct _FSM_Function
{
	char m_szFunction[FSM_BUFF_100];
	char m_szEventID[FSM_BUFF_100];

	_FSM_Function()
	{
		m_szFunction[0]    = '\0';
		m_szEventID[0]     = '\0';

	}
};
typedef vector<_FSM_Function> vec_FSM_Function;

struct _FSM_Class
{
	char m_szProjectName[FSM_BUFF_100];
	char m_szFSMClassName[FSM_BUFF_100];
	char m_szFSMDesc[FSM_BUFF_100];
	vec_FSM_Include  m_vec_FSM_Include;
	vec_FSM_Event    m_vec_FSM_Event;
	_FSM_ParamClass  m_FSM_ParamClass;
	vec_FSM_Function m_FSM_Function;

	_FSM_Class()
	{
		m_szProjectName[0]  = '\0';
		m_szFSMClassName[0] = '\0';
		m_szFSMDesc[0]      = '\0';
	}
};
typedef vector<_FSM_Class> vec_FSM_Class;

class CParse_And_Create_FSM
{
public:
	CParse_And_Create_FSM();
	~CParse_And_Create_FSM();

	bool Parse_FSM_XML_File(const char* pFileName);

	void Create_FSM_Code();

	void Close();

private:
	vec_FSM_Class m_vec_FSM_Class;
};

#endif
