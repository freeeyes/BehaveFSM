#include "MakeTestCode.h"

CMakeTestCode::CMakeTestCode()
{
}

CMakeTestCode::~CMakeTestCode()
{
}

void CMakeTestCode::Create_Code(vec_FSM_Class& obj_vec_FSM_Class, vec_Node_Class& obj_vec_Node_Class)
{
	char szRootPath[200] = {'\0'};

	//生成子目录
	sprintf_safe(szRootPath, 200, "%s/Test/", obj_vec_Node_Class[0].m_szProjectName);
#ifdef WIN32
	_mkdir(szRootPath);
#else
	mkdir(szRootPath), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH);
#endif

	//创建测试
	char szFileName[200] = {'\0'};
	sprintf_safe(szFileName, 200, "%sTestMain.h", szRootPath);
	FILE* pFile = fopen(szFileName, "wb");
	if(NULL == pFile)
	{
		return;
	}

	char szTemp[200]      = {'\0'};

	for(int i = 0; i < (int)obj_vec_FSM_Class.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_vec_FSM_Class[i].m_szFSMClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	for(int i = 0; i < (int)obj_vec_Node_Class.size(); i++)
	{
		sprintf_safe(szTemp, 200, "#include \"%s.h\"\n", obj_vec_Node_Class[i].m_szClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "int main(int argc, char* argv[])\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

·	sprintf_safe(szTemp, 200, "\tC_Action_Param action_param;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	//初始化状态机
	for(int i = 0; i < (int)obj_vec_FSM_Class.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t%s obj_%s;\n", obj_vec_FSM_Class[i].m_szFSMClassName, obj_vec_FSM_Class[i].m_szFSMClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\tobj_%s.Init();\n", obj_vec_FSM_Class[i].m_szFSMClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//初始化所有节点
	for(int i = 0; i < (int)obj_vec_Node_Class.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\t%s obj_%s;\n", obj_vec_Node_Class[i].m_szClassName, obj_vec_Node_Class[i].m_szClassName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\tI_Base_Task obj_Base_Task;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\t//add your init node code at here\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//添加节点关系
	//首先找到首节点
	int nFirstNode = 0;
	for(int i = 0; i < (int)obj_vec_Node_Class.size(); i++)
	{
		if(strcmp(obj_vec_Node_Class[i].m_szParentName, "NULL") == 0)
		{
			nFirstNode = i;
			break;
		}
	}

	sprintf_safe(szTemp, 200, "\tobj_Base_Task.Add_Task_Node(%d, NULL, &%s, NODE_CLASS_SIGNAL);\n", obj_vec_Node_Class[nFirstNode].m_nNodeID, 
		obj_vec_Node_Class[nFirstNode].m_szVariableName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)obj_vec_Node_Class.size(); i++)
	{
		if(i != nFirstNode)
		{
			if(strcmp(obj_vec_Node_Class[i].m_szParentName, "NULL") == 0)
			{
				sprintf_safe(szTemp, 200, "\tobj_Base_Task.Add_Task_Node(%d, NULL, &%s, NODE_CLASS_SIGNAL);\n", obj_vec_Node_Class[i].m_nNodeID, 
					obj_vec_Node_Class[i].m_szVariableName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tobj_Base_Task.Add_Task_Node(%d, &%s, &%s, NODE_CLASS_SIGNAL);\n", obj_vec_Node_Class[i].m_nNodeID, 
					obj_vec_Node_Class[i].m_szParentName,
					obj_vec_Node_Class[i].m_szVariableName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

	}
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//添加测试代码
	sprintf_safe(szTemp, 200, "\twhile (true)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\tobj_Base_Task.Execute_Task(&action_param);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#ifdef WIN32\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\tSleep(1000);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#else\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\tsleep(1);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#endif\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t}");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "}\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	fclose(pFile);
}

