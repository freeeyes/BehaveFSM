#include "ParseLogicNodeXml.h"

CParse_And_Create_Node::CParse_And_Create_Node()
{
}

CParse_And_Create_Node::~CParse_And_Create_Node()
{
}

bool CParse_And_Create_Node::Parse_Node_XML_File(const char* pFileName)
{
	_Node_Class obj_Node_Class;
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

	sprintf_safe(obj_Node_Class.m_szProjectName, NODE_SIZE, "%s", (char* )pRootElement->Attribute("ProjectName"));

	//获得一个FSM的声明
	TiXmlNode* pMainNode = pRootElement->FirstChildElement();
	if(NULL == pMainNode)
	{
		delete pTiXmlDocument;
		return false;
	}

	sprintf_safe(obj_Node_Class.m_szClassName, NODE_SIZE, "%s", (char* )pMainNode->ToElement()->Value());
	sprintf_safe(obj_Node_Class.m_szVariableName, NODE_SIZE, "%s", (char* )pMainNode->ToElement()->Attribute("name"));
	sprintf_safe(obj_Node_Class.m_szParentName, NODE_SIZE, "%s", (char* )pMainNode->ToElement()->Attribute("parent"));
	obj_Node_Class.m_nNodeID = atoi((char* )pMainNode->ToElement()->Attribute("parent"));
	sprintf_safe(obj_Node_Class.m_szDesc, NODE_DESC, "%s", (char* )pMainNode->ToElement()->Attribute("desc"));


	//获得引用文件
	TiXmlNode* pSecondNode = NULL;
	for(pSecondNode = pMainNode->ToElement()->FirstChildElement();pSecondNode;pSecondNode = pSecondNode->NextSiblingElement())
	{
		if(strcmp((char* )pSecondNode->ToElement()->Value(), "IncludeFile") == 0)
		{
			//得到配置文件头
			TiXmlNode* pIncludeNode = NULL;
			for(pIncludeNode = pSecondNode->ToElement()->FirstChildElement();pIncludeNode;pIncludeNode = pIncludeNode->NextSiblingElement())
			{
				_Node_Include obj_Node_Include;
				sprintf_safe(obj_Node_Include.m_szInlcudeFile, NODE_SIZE, "%s", (char* )pIncludeNode->ToElement()->GetText());
				obj_Node_Class.m_vec_Node_Include.push_back(obj_Node_Include);
			}
		}
		else if(strcmp((char* )pSecondNode->ToElement()->Value(), "Variable") == 0)
		{
			//得到包含参数文件
			TiXmlNode* pVariableNode = NULL;
			for(pVariableNode = pSecondNode->ToElement()->FirstChildElement();pVariableNode;pVariableNode = pVariableNode->NextSiblingElement())
			{
				_Variable obj_Variable;
				sprintf_safe(obj_Variable.m_szClassName, NODE_SIZE, "%s", (char* )pVariableNode->ToElement()->Attribute("class"));
				sprintf_safe(obj_Variable.m_szName, NODE_SIZE, "%s", (char* )pVariableNode->ToElement()->Attribute("name"));
				sprintf_safe(obj_Variable.m_szDesc, NODE_DESC, "%s", (char* )pVariableNode->ToElement()->Attribute("desc"));
				obj_Node_Class.m_vec_Variable.push_back(obj_Variable);
			}
		}
		else if(strcmp((char* )pSecondNode->ToElement()->Value(), "Function") == 0)
		{
			//得到包含函数文件
			TiXmlNode* pFunctionNode = NULL;
			for(pFunctionNode = pSecondNode->ToElement()->FirstChildElement();pFunctionNode;pFunctionNode = pFunctionNode->NextSiblingElement())
			{
				_Func_info  obj_Func_info;
				sprintf_safe(obj_Func_info.m_szFunctionName, NODE_SIZE, "%s", (char* )pFunctionNode->ToElement()->Attribute("name"));
				sprintf_safe(obj_Func_info.m_szReturn, NODE_SIZE, "%s", (char* )pFunctionNode->ToElement()->Attribute("return"));
				sprintf_safe(obj_Func_info.m_szDesc, NODE_DESC, "%s", (char* )pFunctionNode->ToElement()->Attribute("desc"));
				
				TiXmlNode* pFuncParamNode = NULL;
				for(pFuncParamNode = pFunctionNode->ToElement()->FirstChildElement();pFuncParamNode;pFuncParamNode = pFuncParamNode->NextSiblingElement())
				{
					_Func_Param obj_Func_Param;
					sprintf_safe(obj_Func_Param.m_szClassName, NODE_SIZE, "%s", (char* )pFuncParamNode->ToElement()->Attribute("class"));
					sprintf_safe(obj_Func_Param.m_szName, NODE_SIZE, "%s", (char* )pFuncParamNode->ToElement()->Attribute("name"));
					obj_Func_info.m_vec_Func_Param.push_back(obj_Func_Param);
				}

				obj_Node_Class.m_vec_Func_info.push_back(obj_Func_info);
			}
		}
	}

	m_vec_Node_Class.push_back(obj_Node_Class);
	delete pTiXmlDocument;
	return true;
}

void CParse_And_Create_Node::Create_Node_Code()
{
	//生成Node代码文件
	for(int i = 0; i < (int)m_vec_Node_Class.size(); i++)
	{
		char szRootPath[200] = {'\0'};
		sprintf_safe(szRootPath, 200, "%s/Node_Logic/", m_vec_Node_Class[i].m_szProjectName);

		Create_Node_Path(m_vec_Node_Class[i]);

		Create_Node_Head(szRootPath, m_vec_Node_Class[i]);
		Create_Node_Cpp(szRootPath, m_vec_Node_Class[i]);
	}
}

void CParse_And_Create_Node::Close()
{
	m_vec_Node_Class.clear();
}

void CParse_And_Create_Node::Create_Node_Path(_Node_Class obj_Node_Class)
{
	char szRootPath[200] = {'\0'};

	//生成主目录
#ifdef WIN32
	_mkdir(obj_Node_Class.m_szProjectName);
#else
	mkdir(m_vec_FSM_Class[i].m_szProjectName), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//生成子目录
	sprintf_safe(szRootPath, 200, "%s/Node_Logic/", obj_Node_Class.m_szProjectName);
#ifdef WIN32
	_mkdir(szRootPath);
#else
	mkdir(szRootPath), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif
}

void CParse_And_Create_Node::Create_Node_Head(const char* pRootPath, _Node_Class obj_Node_Class)
{
	//创建头文件
	char szFileName[200] = {'\0'};
	sprintf_safe(szFileName, 200, "%s%s.h", pRootPath, obj_Node_Class.m_szClassName);
	FILE* pFile = fopen(szFileName, "wb");
	if(NULL == pFile)
	{
		return;
	}

	char szTemp[200]      = {'\0'};
	char szHText[100]     = {'\0'};

	To_Upper_String(obj_Node_Class.m_szClassName, szHText);
	sprintf_safe(szTemp, 200, "#ifndef %s_H_\n", szHText);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define %s_H_\n\n", szHText);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//引用头文件
	sprintf_safe(szTemp, 200, "#include \"Base_Task_Logic.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_Node_Class.m_vec_Node_Include.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"%s\"\n", obj_Node_Class.m_vec_Node_Include[i].m_szInlcudeFile);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//声明类文件
	sprintf_safe(szTemp, 200, "//%s\n", obj_Node_Class.m_szDesc);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "class %s : public I_Base_Task_Logic\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "public:\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t%s();\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t~%s();\n\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0 ; i < (int)obj_Node_Class.m_vec_Func_info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t//%s\n", obj_Node_Class.m_vec_Func_info[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t%s %s(", obj_Node_Class.m_vec_Func_info[i].m_szReturn,  obj_Node_Class.m_vec_Func_info[i].m_szFunctionName);;
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		int nParamSize = obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param.size();
		for(int j = 0; j < nParamSize; j++)
		{
			if(nParamSize == 1 || j == nParamSize - 1)
			{
				sprintf_safe(szTemp, 200, "%s %s", obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szClassName,  obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szName);;
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "%s %s, ", obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szClassName,  obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szName);;
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, ");\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\t_Logic_Return Execute_Logic(I_Param* p_param);\n\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//添加变量
	sprintf_safe(szTemp, 200, "private:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)obj_Node_Class.m_vec_Variable.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t%s %s; //%s\n", obj_Node_Class.m_vec_Variable[i].m_szClassName, 
			obj_Node_Class.m_vec_Variable[i].m_szName,
			obj_Node_Class.m_vec_Variable[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	}

	sprintf_safe(szTemp, 200, "};\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "#endif\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
}

void CParse_And_Create_Node::Create_Node_Cpp(const char* pRootPath, _Node_Class obj_Node_Class)
{
	//创建体文件
	char szFileName[200] = {'\0'};
	sprintf_safe(szFileName, 200, "%s%s.cpp", pRootPath, obj_Node_Class.m_szClassName);
	FILE* pFile = fopen(szFileName, "wb");
	if(NULL == pFile)
	{
		return;
	}

	char szTemp[200]      = {'\0'};
	char szHText[100]     = {'\0'};

	sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//创建默认函数
	sprintf_safe(szTemp, 200, "%s::%s()\n", obj_Node_Class.m_szClassName, obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "%s::~%s()\n", obj_Node_Class.m_szClassName, obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "_Logic_Return %s::Execute_Logic(I_Param* p_param)\n", obj_Node_Class.m_szClassName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t_Logic_Return obj_Logic_Return;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t//add your code at here.\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\treturn obj_Logic_Return;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0 ; i < (int)obj_Node_Class.m_vec_Func_info.size(); i++)
	{
		sprintf_safe(szTemp, 200, "%s %s::%s(", obj_Node_Class.m_vec_Func_info[i].m_szReturn, obj_Node_Class.m_szClassName, 
			obj_Node_Class.m_vec_Func_info[i].m_szFunctionName);;
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		int nParamSize = obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param.size();
		for(int j = 0; j < nParamSize; j++)
		{
			if(nParamSize == 1 || j == nParamSize - 1)
			{
				sprintf_safe(szTemp, 200, "%s %s", obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szClassName,  obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szName);;
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "%s %s, ", obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szClassName,  obj_Node_Class.m_vec_Func_info[i].m_vec_Func_Param[j].m_szName);;
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, ")\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		int nReturn = strlen(obj_Node_Class.m_vec_Func_info[i].m_szReturn);

		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		if(strcmp(obj_Node_Class.m_vec_Func_info[i].m_szReturn, "void") != 0)
		{
			//如果是包含返回值，编写返回值代码
			if(obj_Node_Class.m_vec_Func_info[i].m_szReturn[nReturn - 1] != '*')
			{
				sprintf_safe(szTemp, 200, "\t%s obj_%s;\n", obj_Node_Class.m_vec_Func_info[i].m_szReturn, obj_Node_Class.m_vec_Func_info[i].m_szReturn);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			{
				//如果是指针
				char m_FuncReturn[NODE_SIZE] = {'\0'};
				memcpy(m_FuncReturn, obj_Node_Class.m_vec_Func_info[i].m_szReturn, nReturn - 2);
				m_FuncReturn[nReturn - 2] = '\0';
				sprintf_safe(szTemp, 200, "\t%s p%s;\n", obj_Node_Class.m_vec_Func_info[i].m_szReturn, m_FuncReturn);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		sprintf_safe(szTemp, 200, "\t//add your code at here.\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		if(strcmp(obj_Node_Class.m_vec_Func_info[i].m_szReturn, "void") != 0)
		{
			if(obj_Node_Class.m_vec_Func_info[i].m_szReturn[nReturn - 1] != '*')
			{
				sprintf_safe(szTemp, 200, "\treturn obj_%s;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//如果是指针
				char m_FuncReturn[NODE_SIZE] = {'\0'};
				memcpy(m_FuncReturn, obj_Node_Class.m_vec_Func_info[i].m_szReturn, nReturn - 2);
				m_FuncReturn[nReturn - 2] = '\0';
				sprintf_safe(szTemp, 200, "\treturn p%s;\n",  m_FuncReturn);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

		}
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	fclose(pFile);
}
