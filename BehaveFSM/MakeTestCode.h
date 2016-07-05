#ifndef _MAKE_TEST_CODE
#define _MAKE_TEST_CODE

#include "ParseFSMXml.h"
#include "ParseLogicNodeXml.h"

class CMakeTestCode
{
public:
	CMakeTestCode();
	~CMakeTestCode();

	void Create_Code(vec_FSM_Class& obj_vec_FSM_Class, vec_Node_Class& obj_vec_Node_Class);

	void Create_Make_File_Define(vec_FSM_Class& obj_vec_FSM_Class, vec_Node_Class& obj_vec_Node_Class);

	void Create_Make_File(vec_FSM_Class& obj_vec_FSM_Class, vec_Node_Class& obj_vec_Node_Class);
};

#endif
