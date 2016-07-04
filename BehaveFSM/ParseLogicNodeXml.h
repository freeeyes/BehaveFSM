#ifndef _PARSE_LOGIC_NODE_XML_H
#define _PARSE_LOGIC_NODE_XML_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Base/Base_Conmon.h"
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#define NODE_SIZE 100
#define NODE_DESC 200

struct _Node_Include
{
	char m_szInlcudeFile[NODE_SIZE];

	_Node_Include()
	{
		m_szInlcudeFile[0] = '\0';
	}
};
typedef vector<_Node_Include> vec_Node_Include;

struct _Variable
{
	char m_szClassName[NODE_SIZE];
	char m_szName[NODE_SIZE];
	char m_szDesc[NODE_DESC];

	_Variable()
	{
		m_szClassName[0] = '\0';
		m_szName[0]      = '\0-';
		m_szDesc[0]      = '\0';
	}
};
typedef vector<_Variable> vec_Variable;

struct _Func_Param
{
	char m_szClassName[NODE_SIZE];
	char m_szName[NODE_SIZE];

	_Func_Param()
	{
		m_szClassName[0] = '\0';
		m_szName[0]      = '\0';
	}
};
typedef vector<_Func_Param> vec_Func_Param;

struct _Func_info
{
	char m_szFunctionName[NODE_SIZE];
	char m_szReturn[NODE_SIZE];
	char m_szDesc[NODE_DESC];
	vec_Func_Param m_vec_Func_Param;

	_Func_info()
	{
		m_szFunctionName[0] = '\0';
		m_szReturn[0]       = '\0';
		m_szDesc[0]         = '\0';
	}
};
typedef vector<_Func_info> vec_Func_info;

//节点类信息
struct _Node_Class
{
	char m_szClassName[NODE_SIZE];
	char m_szVariableName[NODE_SIZE];
	char m_szParentName[NODE_SIZE];
	char m_szProjectName[NODE_SIZE];
	char m_szDesc[NODE_DESC];
	int  m_nNodeID;

	vec_Variable     m_vec_Variable;
	vec_Func_info    m_vec_Func_info;
	vec_Node_Include m_vec_Node_Include;

	_Node_Class()
	{
		m_szClassName[0]    = '\0';
		m_szVariableName[0] = '\0';
		m_szParentName[0]   = '\0';
		m_szProjectName[0]  = '\0';
		m_szDesc[0]         = '\0';
  		m_nNodeID           = 0;
	}
};
typedef vector<_Node_Class> vec_Node_Class;

class CParse_And_Create_Node
{
public:
	CParse_And_Create_Node();
	~CParse_And_Create_Node();

	bool Parse_Node_XML_File(const char* pFileName);

	void Create_Node_Code();

	void Close();

private:
	void Create_Node_Path(_Node_Class obj_Node_Class);
	void Create_Node_Head(const char* pRootPath, _Node_Class obj_Node_Class);
	void Create_Node_Cpp(const char* pRootPath, _Node_Class obj_Node_Class);

public:
	vec_Node_Class m_vec_Node_Class;
};

#endif
