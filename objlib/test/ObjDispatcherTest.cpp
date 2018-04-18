
/* This is the test file for the ObjDispatcher.cpp file.
 * For more information on how this works see the googletest primer page.
 * https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
*/
#include "../include/all/ObjDispatcher.hpp"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <vector>

namespace {

    TEST(ObjDispatcherTest, InsertAFrameGetAFrame){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;

        //Crate dummy data 
        int frameId = 10;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(3);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        std::pair<int, int> pair2 (16, 16);
        inputData.at(0) = pair0;
        inputData.at(1) = pair1;
        inputData.at(2) = pair2;

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Store data
        rc = dispatcher->Single(
                ol::INSERT, ol::FRAME, frameId, worldName, &inputData); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Fetch data
        std::vector< std::pair<int, int> > outputData(inputData.size());
        rc = dispatcher->Single(
                ol::GET, ol::FRAME, frameId, worldName, &outputData); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Verify data
        ASSERT_EQ(outputData,  inputData);


        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    TEST(ObjDispatcherTest, InsertAFrameDeleteAFrame){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;

        //Crate dummy data 
        int frameId = 10;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(3);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        std::pair<int, int> pair2 (16, 16);
        inputData.at(0) = pair0;
        inputData.at(1) = pair1;
        inputData.at(2) = pair2;

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Store data
        rc = dispatcher->Single(
                ol::INSERT, ol::FRAME, frameId, worldName, &inputData); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Delete data
        rc = dispatcher->Single(ol::DELETE, ol::FRAME, frameId, worldName, NULL);
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Try to Fetch deleted data
        std::vector< std::pair<int, int> > outputData(inputData.size());
        rc = dispatcher->Single(
                ol::GET, ol::FRAME, frameId, worldName, &outputData); 
        ASSERT_TRUE(rc) << dispatcher->GetErrorLog();

        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    TEST(ObjDispatcherTest, InsertFramaesAndGetListOfIDs){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;
        
        //Create dummy data 
        std::string worldName = "test_world_name";
        int frameId1 = 1;
        int frameId2 = 2;
        std::list<int> *inputList = new std::list<int>;
        inputList->push_back(frameId1);
        inputList->push_back(frameId2);

        std::vector< std::pair<int, int> > inputData1(3);
        std::pair<int, int> pair0 (32, 32);
        inputData1.at(0) = pair0;

        std::vector< std::pair<int, int> > inputData2(3);
        std::pair<int, int> pair1 (12, 12);
        inputData2.at(0) = pair1;
        
        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        
        //Store data
        rc = dispatcher->Single(
                ol::INSERT, ol::FRAME, frameId1, worldName, &inputData1); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        rc = dispatcher->Single(
                ol::INSERT, ol::FRAME, frameId2, worldName, &inputData2); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Get list of frame id's
        std::list<int> *returnList = new std::list<int>;
        rc = dispatcher->GetList(ol::FRAME, worldName, returnList);
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Verify data
        ASSERT_EQ(*inputList, *returnList);
    
        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    TEST(ObjDispatcherTest, GetListOfFrameIDsInEmptyWorld){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;
        
        //Create dummy data 
        std::string worldName = "test_world_name";
        
        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        
        //Get list of frame id's
        std::list<int> *returnList = new std::list<int>;
        rc = dispatcher->GetList(ol::FRAME, worldName, returnList);
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Verify data
        ASSERT_EQ(returnList->size(), 0);
    
        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }


    TEST(ObjDispatcherTest, InsertWorldsAndGetListOfWorlds){
        system("rm -r universe 2>> /dev/null");

        //Create data 
        int rc = 0;
        std::list<std::string> *inputData = new std::list<std::string>;
        std::list<std::string>::iterator it;
        inputData->push_front("test_world_1");
        inputData->push_front("test_world_2");
        it = inputData->begin();

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create worlds
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, *it, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        it++;
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, *it, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Get list of worlds on local machine
        std::list<std::string> *listOfWorlds = new std::list<std::string>;
        rc = dispatcher->GetList(ol::WORLD, "", listOfWorlds);
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Validate data
        ASSERT_EQ(*listOfWorlds, *inputData);
        
        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
        
    }

    TEST(ObjDispatcherTest, DeleteWorld){
        system("rm -r universe 2>> /dev/null");
        
        //Create data 
        int rc = 0;
        std::list<std::string> *inputData = new std::list<std::string>;
        std::list<std::string>::iterator it;
        inputData->push_front("test_world_1");
        inputData->push_front("test_world_2");
        it = inputData->begin();

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create worlds
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, *it, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        it++;
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, *it, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Delete one world
        it = inputData->begin();
        rc = dispatcher->Single(ol::DELETE, ol::WORLD, 0, *it, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();
        
        //Get list of worlds on local machine
        std::list<std::string> *listOfWorlds = new std::list<std::string>;
        rc = dispatcher->GetList(ol::WORLD, "", listOfWorlds);
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Validate one world was deleted
        ASSERT_EQ(listOfWorlds->size(), 1);
        
        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    //Operation tries to use an item (ol::THING) that have not been implemented
    // yet. Remove this if we have time to implement this feature. 
    TEST(ObjDispatcherTest, ErrorOnBadListOperation){
        system("rm -r universe 2>> /dev/null");

        //Create data 
        int rc = 0;

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Get list of worlds on local machine
        std::list<std::string> *listOfWorlds = new std::list<std::string>;
        rc = dispatcher->GetList(ol::THING, "", listOfWorlds);
        ASSERT_TRUE(rc) << dispatcher->GetErrorLog();

        //clean up
        system("rm -r universe 2>> /dev/null");
        
        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    TEST(ObjDispatcherTest, ErrorOnBadOperation){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;

        //Crate dummy data 
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(1);
        std::pair<int, int> pair0 (32, 32);
        inputData.at(0) = pair0;

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Try invalid operation
        rc = dispatcher->Single(ol::GET, ol::WORLD, 0, worldName, NULL);
        ASSERT_TRUE(rc) << dispatcher->GetErrorLog();

        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }

    TEST(ObjDispatcherTest, ErrorOnGetInvalidFrame){
        system("rm -r universe 2>> /dev/null");
        int rc = 0;

        //Crate dummy data 
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > outputData;

        //Create dispatcher 
        ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

        //Create dummy world
        rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL); 
        ASSERT_FALSE(rc) << dispatcher->GetErrorLog();

        //Try to get invlaid frame 
        rc = dispatcher->Single(ol::GET, ol::FRAME, 10, worldName, &outputData);
        ASSERT_TRUE(rc) << dispatcher->GetErrorLog();

        //Clean up
        delete(dispatcher);
        system("rm -r universe 2>> /dev/null");
    }
}// namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
