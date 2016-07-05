// BehaveFSM.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "MakeTestCode.h"

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
	CParse_And_Create_FSM obj_Parse_And_Create_FSM;
	vec_Xml_File_Name obj_FSM_FileList;
	Read_Xml_Folder("./XML_DATA/FSM", obj_FSM_FileList);

	for(int i = 0; i < (int)obj_FSM_FileList.size(); i++)
	{
		obj_Parse_And_Create_FSM.Parse_FSM_XML_File(obj_FSM_FileList[i].c_str());
	}

	obj_Parse_And_Create_FSM.Create_FSM_Code();

	obj_FSM_FileList.clear();
	Read_Xml_Folder("./XML_DATA/LogicNode", obj_FSM_FileList);

	CParse_And_Create_Node obj_Parse_And_Create_Node;
	for(int i = 0; i < (int)obj_FSM_FileList.size(); i++)
	{
		obj_Parse_And_Create_Node.Parse_Node_XML_File(obj_FSM_FileList[i].c_str());
	}
	obj_Parse_And_Create_Node.Create_Node_Code();

	CMakeTestCode objMakeTestCode;
	objMakeTestCode.Create_Code(obj_Parse_And_Create_FSM.m_vec_FSM_Class, obj_Parse_And_Create_Node.m_vec_Node_Class);
	objMakeTestCode.Create_Make_File_Define(obj_Parse_And_Create_FSM.m_vec_FSM_Class, obj_Parse_And_Create_Node.m_vec_Node_Class);
	objMakeTestCode.Create_Make_File(obj_Parse_And_Create_FSM.m_vec_FSM_Class, obj_Parse_And_Create_Node.m_vec_Node_Class);

	getchar();
	return 0;
}

