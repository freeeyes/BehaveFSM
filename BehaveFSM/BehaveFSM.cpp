// BehaveFSM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "Action_FSM.h"
#include "C_Action_Task_Logic.h"
#include "C_Action_Look_Task_Logic.h"
#include "Base_Task.h"
#include "ParseFSMXml.h"

#ifdef WIN32
#include <windows.h> 
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

typedef vector<string> vec_Xml_File_Name;

//����ָ����Ŀ¼���������XML�ļ���
bool Read_Xml_Folder( string folderPath, vec_Xml_File_Name& obj_vec_Xml_File_Name)
{
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return false;
	}
	do
	{
		//�ж��Ƿ�����Ŀ¼
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//���������Ҫ
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				//����֧����Ŀ¼����
				//string newPath = folderPath + "\\" + FileInfo.name;
				//dfsFolder(newPath);
			}
		}
		else  
		{
			string filename = folderPath + "/" + FileInfo.name;
			obj_vec_Xml_File_Name.push_back(filename);
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(folderPath.c_str())) == NULL) 
	{
		printf("cannot open directory: %s\n", folderPath.c_str());
		return false;
	}
	chdir(folderPath.c_str());
	while((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
				continue;
			//����Ҫ֧����Ŀ¼����
			//printf("%*s%s/\n",depth,"",entry->d_name);
			//dfsFolder(entry->d_name,depth+4);
		} 
		else 
		{
			string filename = folderPath + "/" + entry->d_name;
			obj_vec_Xml_File_Name.push_back(filename);
		}
	}
	chdir("..");
	closedir(dp);
#endif
	return true;
}

int main(int argc, char* argv[])
{
	/*
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
	obj_Base_Task.Add_Task_Node(1001, NULL, &obj_Action_Look_Task_Logic, NODE_CLASS_SIGNAL);
	obj_Base_Task.Add_Task_Node(1002, &obj_Action_Look_Task_Logic, &obj_Action_Task_Logic, NODE_CLASS_SIGNAL);

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
	*/

	CParse_And_Create_FSM obj_Parse_And_Create_FSM;
	vec_Xml_File_Name obj_FSM_FileList;
	Read_Xml_Folder("./XML_DATA/FSM", obj_FSM_FileList);

	for(int i = 0; i < obj_FSM_FileList.size(); i++)
	{
		obj_Parse_And_Create_FSM.Parse_FSM_XML_File(obj_FSM_FileList[i].c_str());
	}

	getchar();
	return 0;
}

