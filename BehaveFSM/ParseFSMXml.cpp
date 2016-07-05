#include "ParseFSMXml.h"

CParse_And_Create_FSM::CParse_And_Create_FSM()
{

}

CParse_And_Create_FSM::~CParse_And_Create_FSM()
{

}

void CParse_And_Create_FSM::Close()
{
	m_vec_FSM_Class.clear();
}

void CParse_And_Create_FSM::Create_FSM_Path(_FSM_Class obj_FSM_Class)
{
	char szRootPath[200] = {'\0'};

	//生成主目录
#ifdef WIN32
	_mkdir(obj_FSM_Class.m_szProjectName);
#else
	mkdir(m_vec_FSM_Class[i].m_szProjectName), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//生成子目录
	sprintf_safe(szRootPath, 200, "%s/FSM/", obj_FSM_Class.m_szProjectName);
#ifdef WIN32
	_mkdir(szRootPath);
#else
	mkdir(szRootPath), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建Base目录
	sprintf_safe(szRootPath, 100, "%s/Base", obj_FSM_Class.m_szProjectName);
#ifdef WIN32
	_mkdir(szRootPath);
#else
	mkdir(szRootPath, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//拷贝指定文件到目录中去
	char szTempFile[FSM_BUFF_100] = {'\0'};
	sprintf_safe(szTempFile, FSM_BUFF_100, "%s/Base_Conmon.h", szRootPath);
	Tranfile("../Base/Base_Conmon.h", szTempFile);
	sprintf_safe(szTempFile, FSM_BUFF_100, "%s/Base_FSM.h", szRootPath);
	Tranfile("../Base/Base_FSM.h", szTempFile);
	sprintf_safe(szTempFile, FSM_BUFF_100, "%s/Base_Task.h", szRootPath);
	Tranfile("../Base/Base_Task.h", szTempFile);
	sprintf_safe(szTempFile, FSM_BUFF_100, "%s/Base_Task_Logic.h", szRootPath);
	Tranfile("../Base/Base_Task_Logic.h", szTempFile);
}

void CParse_And_Create_FSM::Create_FSM_Head(const char* pRootPath, _FSM_Class obj_FSM_Class)
{
	//创建头文件
	char szFileName[200] = {'\0'};
	sprintf_safe(szFileName, 200, "%s%s.h", pRootPath, obj_FSM_Class.m_szFSMClassName);
	FILE* pFile = fopen(szFileName, "wb");
	if(NULL == pFile)
	{
		return;
	}

	char szTemp[200]      = {'\0'};
	char szHText[100]     = {'\0'};

	To_Upper_String(obj_FSM_Class.m_szFSMClassName, szHText);
	sprintf_safe(szTemp, 200, "#ifndef %s_H_\n", szHText);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define %s_H_\n\n", szHText);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//引用头文件
	sprintf_safe(szTemp, 200, "#include \"Base_FSM.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_vec_FSM_Include.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"%s\"\n", obj_FSM_Class.m_vec_FSM_Include[i].m_szInlcudeFile);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//创建枚举
	sprintf_safe(szTemp, 200, "//%s\n", obj_FSM_Class.m_FSM_Event_Enum.m_szDesc);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "enum %s\n", obj_FSM_Class.m_FSM_Event_Enum.m_szName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event.size(); i++)
	{
		if(i == 0)
		{
			sprintf_safe(szTemp, 200, "\t%s = 1, //%s\n", 
				obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szEventName, 
				obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szDesc);
		}
		else
		{
			sprintf_safe(szTemp, 200, "\t%s, //%s\n", 
				obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szEventName, 
				obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szDesc);
		}

		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "};\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//定义参数类
	sprintf_safe(szTemp, 200, "class %s : public I_Param\n", obj_FSM_Class.m_FSM_ParamClass.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "public:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t%s()\n", obj_FSM_Class.m_FSM_ParamClass.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column.size(); i++)
	{
		if(obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_nLength > 0)
		{
			sprintf_safe(szTemp, 200, "\t\tm_%s[0] = %s;\n", obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szColumnName, 
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szInitValue);
		}
		else
		{
			sprintf_safe(szTemp, 200, "\t\tm_%s = %s;\n", obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szColumnName, 
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szInitValue);
		}
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\t}\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column.size(); i++)
	{
		if(obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_nLength > 0)
		{
			sprintf_safe(szTemp, 200, "\t%s m_%s[%d];\n", obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szClassTypeName,
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szColumnName, 
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_nLength);

		}
		else
		{
			sprintf_safe(szTemp, 200, "\t%s m_%s;\n", obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szClassTypeName,
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column[i].m_szColumnName);
		}
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "};\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//创建类文件
	sprintf_safe(szTemp, 200, "class %s : public I_Base_FSM\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "public:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t%s();\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t~%s();\n\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tvoid Init();\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tint Do_Event(I_Param* p_fsm_param);\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_Function.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tint %s(FSM_STATE_ID %s, I_Param* p_fsm_param);\n", 
			obj_FSM_Class.m_FSM_Function[i].m_szFunction,
			obj_FSM_Class.m_FSM_Function[i].m_szEventID);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "};\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#endif\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
}

void CParse_And_Create_FSM::Create_FSM_Cpp(const char* pRootPath, _FSM_Class obj_FSM_Class)
{
	//创建头文件
	char szFileName[200] = {'\0'};
	sprintf_safe(szFileName, 200, "%s%s.cpp", pRootPath, obj_FSM_Class.m_szFSMClassName);
	FILE* pFile = fopen(szFileName, "wb");
	if(NULL == pFile)
	{
		return;
	}

	char szTemp[200]      = {'\0'};
	char szHText[100]     = {'\0'};

	//引用头文件
	sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//编写函数
	sprintf_safe(szTemp, 200, "%s::%s()\n", obj_FSM_Class.m_szFSMClassName, obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "%s::~%s()\n", obj_FSM_Class.m_szFSMClassName, obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "void %s::Init()\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tInit_FSM_State_Count(%d);\n", obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event.size());
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tSet_State_ID(%s);\n", obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szEventName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\tSet_FSM_Curr_State(%s);\n", obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[0].m_szEventName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "int %s::Do_Event(I_Param* p_fsm_param)\n", obj_FSM_Class.m_szFSMClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tFSM_STATE_ID s_curr_state_id = Get_FSM_Curr_State_ID();\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tFSM_FUNCTION_BEGIN(s_curr_state_id);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_Function.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tFSM_FUNCTION(%s,   %s,   s_curr_state_id, p_fsm_param);\n", 
			obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event[i].m_szEventName, 
			obj_FSM_Class.m_FSM_Function[i].m_szFunction);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\tFSM_FUNCTION_END;\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\treturn 0;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_FSM_Class.m_FSM_Function.size(); i++)
	{
		sprintf_safe(szTemp, 200, "int %s::%s(FSM_STATE_ID %s, I_Param* p_fsm_param)\n", 
			obj_FSM_Class.m_szFSMClassName, 
			obj_FSM_Class.m_FSM_Function[i].m_szFunction,
			obj_FSM_Class.m_FSM_Function[i].m_szEventID);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s* p_action_param = (%s* )p_fsm_param;\n",
			obj_FSM_Class.m_FSM_ParamClass.m_szClassName,
			obj_FSM_Class.m_FSM_ParamClass.m_szClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tPRINT_SAFE(\"[%s]s_state_id=%%d.\\n\", s_state_id);\n",
			obj_FSM_Class.m_FSM_Function[i].m_szFunction);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t//add your code at here.\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\treturn 0;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}


	fclose(pFile);
}

bool CParse_And_Create_FSM::Parse_FSM_XML_File(const char* pFileName)
{
	_FSM_Class obj_FSM_Class;
	TiXmlDocument* pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == pTiXmlDocument)
	{
		return false;
	}

	if(false == pTiXmlDocument->LoadFile())
	{
		delete pTiXmlDocument;
		return false;
	}

	//将XML变换为数据结构

	//获得根元素
	TiXmlElement* pRootElement = pTiXmlDocument->RootElement();

	if(NULL == pRootElement)
	{
		delete pTiXmlDocument;
		return false;
	}

	sprintf_safe(obj_FSM_Class.m_szProjectName, FSM_BUFF_100, "%s", (char* )pRootElement->Attribute("ProjectName"));

	//获得一个FSM的声明
	TiXmlNode* pMainNode = pRootElement->FirstChildElement();
	if(NULL == pMainNode)
	{
		delete pTiXmlDocument;
		return false;
	}

	sprintf_safe(obj_FSM_Class.m_szFSMClassName, FSM_BUFF_100, "%s", (char* )pMainNode->ToElement()->Value());
	sprintf_safe(obj_FSM_Class.m_szFSMDesc, FSM_BUFF_100, "%s", (char* )pMainNode->ToElement()->Attribute("desc"));

	//获得引用文件
	TiXmlNode* pSecondNode = NULL;
	for(pSecondNode = pMainNode->ToElement()->FirstChildElement();pSecondNode;pSecondNode = pSecondNode->NextSiblingElement())
	{
		if(strcmp((char* )pSecondNode->ToElement()->Value(), "FSMSInclude") == 0)
		{
			//得到包含头文件
			TiXmlNode* pIncludeNode = NULL;
			for(pIncludeNode = pSecondNode->ToElement()->FirstChildElement();pIncludeNode;pIncludeNode = pIncludeNode->NextSiblingElement())
			{
				_FSM_Include obj_FSM_Include;
				sprintf_safe(obj_FSM_Include.m_szInlcudeFile, FSM_BUFF_100, "%s", (char* )pIncludeNode->ToElement()->GetText());
				obj_FSM_Class.m_vec_FSM_Include.push_back(obj_FSM_Include);
			}
		}
		else if((char* )strcmp(pSecondNode->ToElement()->Value(), "FSMState") == 0)
		{
			//得到事件ID类声明
			sprintf_safe(obj_FSM_Class.m_FSM_Event_Enum.m_szName, FSM_BUFF_100, "%s", (char* )pSecondNode->ToElement()->Attribute("name"));
			sprintf_safe(obj_FSM_Class.m_FSM_Event_Enum.m_szDesc, FSM_BUFF_100, "%s", (char* )pSecondNode->ToElement()->Attribute("desc"));

			TiXmlNode* pEventNode = NULL;
			for(pEventNode = pSecondNode->ToElement()->FirstChildElement();pEventNode;pEventNode = pEventNode->NextSiblingElement())
			{
				_FSM_Event obj_FSM_Event;
				sprintf_safe(obj_FSM_Event.m_szEventName, FSM_BUFF_100, "%s", (char* )pEventNode->ToElement()->GetText());
				sprintf_safe(obj_FSM_Event.m_szDesc, FSM_BUFF_100, "%s", (char* )pEventNode->ToElement()->Attribute("desc"));
				obj_FSM_Class.m_FSM_Event_Enum.m_vec_FSM_Event.push_back(obj_FSM_Event);
			}
		}
		else if((char* )strcmp(pSecondNode->ToElement()->Value(), "FSMParam") == 0)
		{
			//得到参数类声明
			TiXmlNode* pParamClassNode = NULL;
			pParamClassNode = pSecondNode->FirstChildElement();

			sprintf_safe(obj_FSM_Class.m_FSM_ParamClass.m_szClassName, FSM_BUFF_100, "%s", (char* )pParamClassNode->ToElement()->Value());
			sprintf_safe(obj_FSM_Class.m_FSM_ParamClass.m_desc, FSM_BUFF_100, "%s", (char* )pParamClassNode->ToElement()->Attribute("desc"));

			TiXmlNode* pParamColumnNode = NULL;
			for(pParamColumnNode = pParamClassNode->ToElement()->FirstChildElement();pParamColumnNode;pParamColumnNode = pParamColumnNode->NextSiblingElement())
			{
				_FSM_Param_Column obj_FSM_Param_Column;
				sprintf_safe(obj_FSM_Param_Column.m_szColumnName, FSM_BUFF_100, "%s", (char* )pParamColumnNode->ToElement()->Attribute("name"));
				sprintf_safe(obj_FSM_Param_Column.m_szClassTypeName, FSM_BUFF_100, "%s", (char* )pParamColumnNode->ToElement()->Attribute("classtype"));
				char* pData = (char* )pParamColumnNode->ToElement()->Attribute("length");
				if(NULL != pData)
				{
					obj_FSM_Param_Column.m_nLength = atoi(pData);
				}
				sprintf_safe(obj_FSM_Param_Column.m_szInitValue, FSM_BUFF_100, "%s", (char* )pParamColumnNode->ToElement()->Attribute("value"));
				sprintf_safe(obj_FSM_Param_Column.m_szDesc, FSM_BUFF_100, "%s", (char* )pParamColumnNode->ToElement()->Attribute("desc"));
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column.push_back(obj_FSM_Param_Column);
			}
		}
		else if((char* )strcmp(pSecondNode->ToElement()->Value(), "FSMFunction") == 0)
		{
			//得到函数声明
			TiXmlNode* pFuncNode = NULL;
			for(pFuncNode = pSecondNode->ToElement()->FirstChildElement();pFuncNode;pFuncNode = pFuncNode->NextSiblingElement())
			{
				_FSM_Function obj_FSM_Function;
				sprintf_safe(obj_FSM_Function.m_szEventID, FSM_BUFF_100, "%s", (char* )pFuncNode->ToElement()->Attribute("EventName"));
				sprintf_safe(obj_FSM_Function.m_szFunction, FSM_BUFF_100, "%s", (char* )pFuncNode->ToElement()->Attribute("name"));
				obj_FSM_Class.m_FSM_Function.push_back(obj_FSM_Function);
			}
		}
	}

	m_vec_FSM_Class.push_back(obj_FSM_Class);
	delete pTiXmlDocument;
	return true;
}

void CParse_And_Create_FSM::Create_FSM_Code()
{
	//生成FSM代码文件
	for(int i = 0; i < (int)m_vec_FSM_Class.size(); i++)
	{
		char szRootPath[200] = {'\0'};
		sprintf_safe(szRootPath, 200, "%s/FSM/", m_vec_FSM_Class[i].m_szProjectName);

		Create_FSM_Path(m_vec_FSM_Class[i]);

		Create_FSM_Head(szRootPath, m_vec_FSM_Class[i]);
		Create_FSM_Cpp(szRootPath, m_vec_FSM_Class[i]);
	}
}

