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
	TiXmlDocument* pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == pTiXmlDocument)
	{
		return false;
	}

	if(false == pTiXmlDocument->LoadFile())
	{
		return false;
	}

	//��XML�任Ϊ���ݽṹ



	return true;
}

