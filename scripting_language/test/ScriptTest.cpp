/*	
	ScriptTest.cpp	
*/
/*
 *	Test for Scripting Language for CS485 Honors Enhancement Project
 *	Using GoogleTest:
 *		https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
*/
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <fstream>
#include "ObjDispatcher.hpp"

#define WORLD_NAME "test_world"
// 2> /dev/null --> redirects stderr to /dev/null, which throws it away
#define RM_WORLD_SYS_CMD system("rm universe/test_world.wrd 2> /dev/null")

using std::string;
using std::list;
using std::pair;
using std::vector;
using std::cout; 
using std::endl;

//*	running `honors_script` with different `testScript#.in` files
//*	use HONORS_SCRIPT macro defined through cmake
//*	use TEST_DIR to locate testScript#.in files
namespace {

	void execute_script(string fileName) {
		string cmd = "";
		cmd += HONORS_SCRIPT;
		cmd += " ";
		cmd += TEST_DIR;
		cmd += fileName;
		system(cmd.c_str()); 
	}

	//	testScript1.in
	/*
		name "test_world" //	creates world
	*/
	//*	CHECKS
	//	1. check if test_world exists 
	//	2. check that world only has one frame
	//	3. check if there all cells are dead at frameId 1
	TEST(ScriptTest, InitializeEmptyWorld_testScript1) {
		RM_WORLD_SYS_CMD;
		string fileName = "testScript1.in";
		
		execute_script(fileName);

		int rc = 0, frameId = 1;

		//	CHECK 1
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		//	make sure you can't create another `test_world`
		rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, WORLD_NAME, NULL);
		ASSERT_TRUE(rc) << "Error: Script did not create a world";

		//	CHECK 2
		list<int> returnList;
		rc = dispatcher->GetList(ol::FRAME, WORLD_NAME, &returnList);
		ASSERT_EQ(returnList.size(), 1) << "Error: World is not initialized with one frame";

		//	CHECK 3
		//	make sure frameId == 1
		ASSERT_EQ(returnList.front(), 1) << "Error: Initial Frame id is not 1";
		//	get frame
		vector< pair<int,int> > outputData;
		rc = dispatcher->Single(ol::GET, ol::FRAME, frameId, WORLD_NAME, &outputData);
		ASSERT_EQ(outputData.size(), 0) << "Error: Initial Frame is not empty.";

		//	clean up
		delete (dispatcher);
		RM_WORLD_SYS_CMD;
	}

	//	testScript2.in
	/*
		name "test_world" //	creates world

		//	create cells individually
		live at 1,1 2,2 3,3

		//	use range functinality
		live at 4,4:5,5		//live: 4,4 4,5 5,4 5,5

		//	use dimension functionality
		live at 6,6:2x2		//live: 6,6 6,7 7,6 7,7
	*/
	//*	CHECKS
	//	1. check live cells are 1,1 2,2 3,3 4,4 4,5 5,4 5,5 6,6 6,7 7,6 7,7
	TEST(ScriptTest, LiveCommandTest_testScript2) {
		RM_WORLD_SYS_CMD;
		string fileName = "testScript2.in";

		execute_script(fileName);

		int rc = 0, frameId = 1;
		//	expected live_cells after running script
		vector<pair<int,int>> live_cells {	{1,1},
											{2,2},
											{3,3},
											{4,4},
											{4,5},
											{5,4},
											{5,5},
											{6,6},
											{6,7},
											{7,6},
											{7,7}};

		//	lets get the live cells after running script
		//	CHECK 1
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		vector< pair<int,int> > outputData;
		rc = dispatcher->Single(ol::GET, ol::FRAME, frameId, WORLD_NAME, &outputData);
		ASSERT_EQ(outputData, live_cells) << "Error: unexpected content inside frame.";

		delete(dispatcher);
		RM_WORLD_SYS_CMD;
	}


	//	testScript3
	/*
		name "test_world" //	creates world

		//	create cells individually
		live at 1,1 2,2 3,3				//	LIVE: 1,1 2,2 3,3
		dead at 3,3						//	LIVE: 1,1 2,2

		//	use range functinality
		live at 4,4:5,5					//	LIVE: 1,1 2,2 4,4 4,5 5,4 5,5
		dead at 4,3:4,6					//	LIVE: 1,1 2,2 5,4 5,5

		//	use dimension functionality
		live at 6,6:2x2					//	LIVE: 1,1 2,2 5,4 5,5 6,6 6,7 7,6 7,7
		dead at 6,7:4x4					//	LIVE: 1,1 2,2 5,4 5,5 6,6 7,6
	*/
	//*	CHECKS
	//	1. check
	TEST(ScriptTest, DeadCommandTest_testScript3) {
		RM_WORLD_SYS_CMD;
		string fileName = "testScript3.in";

		execute_script(fileName);

		int rc = 0, frameId = 1;
		//	expected live_cells after running script
		vector<pair<int,int>> live_cells {	{1,1},
											{2,2},
											{5,4},
											{5,5},
											{6,6},
											{7,6}};

		//	lets get the live cells after running script
		//	CHECK 1
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		vector< pair<int,int> > outputData;
		rc = dispatcher->Single(ol::GET, ol::FRAME, frameId, WORLD_NAME, &outputData);
		ASSERT_EQ(outputData, live_cells) << "Error: unexpected content inside frame.";

		delete(dispatcher);
		RM_WORLD_SYS_CMD;
	}

	//	testScript4
	/*
		name "test_world" //	creates world

		//	create cells individually
		live at 1,1 2,2 3,3				//	LIVE: 1,1 2,2 3,3
		dead at 3,3						//	LIVE: 1,1 2,2
		toggle at 2,2 1,1				//	LIVE: 

		//	use range functinality
		live at 4,4:5,5					//	LIVE: 4,4 4,5 5,4 5,5
		dead at 4,3:4,6					//	LIVE: 5,4 5,5
		toggle at 1,1:2,2				//	LIVE: 5,4 5,5 

		//	use dimension functionality
		live at 6,6:2x2					//	LIVE: 5,4 5,5 1,1 1,2 2,1 2,2 6,6 6,7 7,6 7,7
		dead at 6,7:4x4					//	LIVE: 5,4 5,5 1,1 1,2 2,1 2,2 6,6 7,6
		toggle at 5,4:3x3				//	LIVE: 1,1 1,2 2,1 2,2 5,6 6,4 6,5 7,4 7,5
	*/
	//*	CHECKS
	//	1. check if live cells are 1,1 1,2 2,1 2,2 5,6 6,4 6,5 7,4 7,5
	TEST(ScriptTest, ToggleCommandTest_testScript4) {
		RM_WORLD_SYS_CMD;
		string fileName = "testScript4.in";

		execute_script(fileName);

		int rc = 0, frameId = 1;
		//	expected live_cells after running script
		vector<pair<int,int>> live_cells {	{1,1},
											{1,2},
											{2,1},
											{2,2},
											{5,6},
											{6,4},
											{6,5},
											{7,4},
											{7,5}};

		//	lets get the live cells after running script
		//	CHECK 1
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		vector< pair<int,int> > outputData;
		rc = dispatcher->Single(ol::GET, ol::FRAME, frameId, WORLD_NAME, &outputData);
		ASSERT_EQ(outputData, live_cells) << "Error: unexpected content inside frame.";

		delete(dispatcher);
		RM_WORLD_SYS_CMD;
	}
}


















