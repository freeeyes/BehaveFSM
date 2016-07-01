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

	//��XML�任Ϊ���ݽṹ

	//��ø�Ԫ��
	TiXmlElement* pRootElement = pTiXmlDocument->RootElement();

	if(NULL == pRootElement)
	{
		delete pTiXmlDocument;
		return false;
	}

	sprintf_safe(obj_FSM_Class.m_szProjectName, FSM_BUFF_100, "%s", (char* )pRootElement->Attribute("ProjectName"));

	//���һ��FSM������
	TiXmlNode* pMainNode = pRootElement->FirstChildElement();
	if(NULL == pMainNode)
	{
		delete pTiXmlDocument;
		return false;
	}

	sprintf_safe(obj_FSM_Class.m_szFSMClassName, FSM_BUFF_100, "%s", (char* )pMainNode->ToElement()->Value());
	sprintf_safe(obj_FSM_Class.m_szFSMDesc, FSM_BUFF_100, "%s", (char* )pMainNode->ToElement()->Attribute("desc"));

	//��������ļ�
	TiXmlNode* pSecondNode = NULL;
	for(pSecondNode = pMainNode->ToElement()->FirstChildElement();pSecondNode;pSecondNode = pSecondNode->NextSiblingElement())
	{
		if(strcmp((char* )pSecondNode->ToElement()->Value(), "FSMSInclude") == 0)
		{
			//�õ�����ͷ�ļ�
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
			//�õ��¼�ID������
			TiXmlNode* pEventNode = NULL;
			for(pEventNode = pSecondNode->ToElement()->FirstChildElement();pEventNode;pEventNode = pEventNode->NextSiblingElement())
			{
				_FSM_Event obj_FSM_Event;
				sprintf_safe(obj_FSM_Event.m_szEventName, FSM_BUFF_100, "%s", (char* )pEventNode->ToElement()->GetText());
				sprintf_safe(obj_FSM_Event.m_szDesc, FSM_BUFF_100, "%s", (char* )pEventNode->ToElement()->Attribute("desc"));
				obj_FSM_Class.m_vec_FSM_Event.push_back(obj_FSM_Event);
			}
		}
		else if((char* )strcmp(pSecondNode->ToElement()->Value(), "FSMParam") == 0)
		{
			//�õ�����������
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
				sprintf_safe(obj_FSM_Param_Column.m_szDesc, FSM_BUFF_100, "%s", (char* )pParamColumnNode->ToElement()->Attribute("desc"));
				obj_FSM_Class.m_FSM_ParamClass.m_vec_FSM_Param_Column.push_back(obj_FSM_Param_Column);
			}
		}
		else if((char* )strcmp(pSecondNode->ToElement()->Value(), "FSMFunction") == 0)
		{
			//�õ���������
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
	//����FSM�����ļ�
}

