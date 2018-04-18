/* This is the test file for the Frame.cpp file.
 * For more information on how this works see the googletest primer page.
 * https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
*/
#include "../include/all/Frame.hpp"
#include "../include/all/World.hpp"
#include "../include/all/ReceiverBase.hpp"
#include "../include/all/unqlite.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <vector>

namespace {
    
    /*
     * Make sure Frame can insert and get a single
     * frame without a problem.
    */
    TEST(FrameTest, InsertAFrameGetAFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 10;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(3);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        std::pair<int, int> pair2 (16, 16);
        inputData.at(0) = pair0;
        inputData.at(1) = pair1;
        inputData.at(2) = pair2;

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Fetch data 
        std::vector< std::pair<int, int> > outputData(inputData.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Verify data
        ASSERT_EQ(outputData,  inputData);
        system("rm -r universe 2>> /dev/null");
    }

    /*
     * Make sure Frame can handle empty data
     * without error.
    */
    TEST(FrameTest, InsertEmptyFrameAndRetrieve){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 11;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(0);

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Fetch data 
        std::vector< std::pair<int, int> > outputData(inputData.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Verify data
        ASSERT_EQ(outputData,  inputData);
        system("rm -r universe 2>> /dev/null");
    }

    /*
     * testing if frame can be deleted
    */
    TEST(FrameTest, InsertFrameDeleteFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;

        //Create dummy data 
        int frameId = 10;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(3);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        std::pair<int, int> pair2 (16, 16);
        inputData.at(0) = pair0;
        inputData.at(1) = pair1;
        inputData.at(2) = pair2;

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //delete frame
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Delete();
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
    }

    /*
     * Trying to delete nonexistant frame
    */
    TEST(FrameTest, DeleteFrameFromEmptyWorld){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        std::string worldName = "test_world_name";
        int frameId = 22;

        //create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);

        //try to delete frame from this empty world
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Delete();
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);
    }

/*
     * Inserts two frames into a world then gets a list of the frame ids.
    */
    TEST(FrameTest, GetListOfFrameIDsInWorld){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
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

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data
        Frame *frame = new Frame(frameId1, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData1);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
        frame = new Frame(frameId2, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData2);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Get list of frame ids
        std::list<int> *returnList = new std::list<int>; 
        frame = new Frame(worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->ListFrames();
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        returnList = frame->GetListOfFrames();
        delete(frame);
        
        //Verify data
        ASSERT_EQ(*returnList, *inputList);
        system("rm -r universe 2>> /dev/null");
    }


    /*
     * Tests to make sure that an empty world returns an empty frame
     * id list.
    */
    TEST(FrameTest, GetListOfFrameIDsInEmptyWorld){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        std::string worldName = "test_world_name";

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Get list of frame ids
        std::list<int> *returnList = new std::list<int>; 
        Frame *frame = new Frame(worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->ListFrames();
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        returnList = frame->GetListOfFrames();
        delete(frame);
        
        //Verify data
        ASSERT_EQ(returnList->size(), 0);
        system("rm -r universe 2>> /dev/null");
    }

    /* 
     * Returns an error code when you try to insert
     * a frame id that already exists.
    */
    TEST(FrameTest, ErrorOnInsertDuplicateFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 22;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData1(2);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        inputData1.at(0) = pair0;
        inputData1.at(1) = pair1;

        //Create second set of dummy data
        std::vector< std::pair<int, int> > inputData2(2);
        std::pair<int, int> pair2 (2, 2);
        std::pair<int, int> pair3 (4, 4);
        inputData2.at(0) = pair2;
        inputData2.at(1) = pair3;

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data 
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData1);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
        
        //Store duplicate data 
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData2);
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);

        //Fetch data 
        std::vector< std::pair<int, int> > outputData(inputData1.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Verify data
        ASSERT_EQ(outputData,  inputData1);
        system("rm -r universe 2>> /dev/null");
    }

    /*
     * Returns an error code if you try to get a 
     * frame id that does not exist.
    */
    TEST(FrameTest, ErrorOnGetNoneexistentFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 13;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(2);

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Fetch nonexistent data 
        std::vector< std::pair<int, int> > outputData(inputData.size());
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);

        //Clean up
        system("rm -r universe 2>> /dev/null");
    }
    
    /*
     * Make sure data can me retrieved multiple
     * times without error.
    */
    TEST(FrameTest, GetFrameTwice){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 22;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(4);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        std::pair<int, int> pair2 (2, 2);
        std::pair<int, int> pair3 (4, 4);
        inputData.at(0) = pair0;
        inputData.at(1) = pair1;
        inputData.at(2) = pair2;
        inputData.at(3) = pair3;

        //Create dummy world
        World *world = new World();
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data 
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
        
        //Fetch data 
        std::vector< std::pair<int, int> > outputData1(inputData.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData1);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
        
        //Verify data
        ASSERT_EQ(outputData1,  inputData);

        //Fetch data again
        std::vector< std::pair<int, int> > outputData2(inputData.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData2);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Verify data again
        ASSERT_EQ(outputData2,  inputData);
        system("rm -r universe 2>> /dev/null");
    }

    /*
     * Make sure Frame returns error when trying to insert
     * into a world that does not exist.
    */
    TEST(FrameTest, ErroronInsertIntoNoneexistentWorld){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 10;
        std::string worldName = "test_world_name";
        std::vector< std::pair<int, int> > inputData(1);
        std::pair<int, int> pair0 (32, 32);
        inputData.at(0) = pair0;

        //Store data
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData);
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);

        system("rm -r universe 2>> /dev/null");
    }

    /*
     * Test replace frame to see if data gets replaced
    */
    TEST(FrameTest, ReplaceExistingFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 22;
        std::vector< std::pair<int, int> > inputData1(2);
        std::pair<int, int> pair0 (32, 32);
        std::pair<int, int> pair1 (64, 64);
        inputData1.at(0) = pair0;
        inputData1.at(1) = pair1;

        //Create second set of dummy data
        std::vector< std::pair<int, int> > inputData2(2);
        std::pair<int, int> pair2 (2, 2);
        std::pair<int, int> pair3 (4, 4);
        inputData2.at(0) = pair2;
        inputData2.at(1) = pair3;

        //Create dummy world
        World *world = new World();
        std::string worldName = "test_world_name";
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Store data 
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Insert(inputData1);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);
        
        //Call ReplaceFrame and edit frame
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->ReplaceFrame(inputData2);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Fetch data 
        std::vector< std::pair<int, int> > outputData(inputData1.size());
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_FALSE(rc) << frame->GetErrorLog();
        delete(frame);

        //Verify data
        //Frame should now contain inputData2
        ASSERT_EQ(outputData,   inputData2);
        //and inputData1 should be replaced
        ASSERT_NE(outputData,   inputData1);
        system("rm -r universe 2>> /dev/null");
    }

    /*
     * Try to replace a non-existing frame
    */
    TEST(FrameTest, ReplaceNonExistingFrame){
        system("rm -r universe 2>> /dev/null");
        int rc;
        
        //Create dummy data 
        int frameId = 22;

        //Create second set of dummy data
        std::vector< std::pair<int, int> > inputData2(2);
        std::pair<int, int> pair2 (2, 2);
        std::pair<int, int> pair3 (4, 4);
        inputData2.at(0) = pair2;
        inputData2.at(1) = pair3;

        //Create dummy world
        World *world = new World();
        std::string worldName = "test_world_name";
        rc = world->Insert(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);

        //Try to fetch data (should not work because frameId was never inserted)
        std::vector< std::pair<int, int> > outputData(inputData2.size());
        Frame *frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->Get(outputData);
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);
        
        //Call ReplaceFrame on non existing frame
        frame = new Frame(frameId, worldName);
        ASSERT_EQ(frame->GetErrorLog(), "") << frame->GetErrorLog();
        rc = frame->ReplaceFrame(inputData2);
        ASSERT_TRUE(rc) << frame->GetErrorLog();
        delete(frame);

        system("rm -r universe 2>> /dev/null");
    }
    
}// namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
