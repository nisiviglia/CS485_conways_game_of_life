/* This is the test file for the World.cpp file.
 * For more information on how this works see the googletest primer page.
 * https://github.com/google/googletest/blob/master/googletest/docs/Primer.md
*/
#include "../include/all/World.hpp"
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

namespace {
    

    /* Test to see if you can create a new database file.
     *
     * Create a database file and check for reported error.
     * Next check that database file exists.
     */
    TEST(WorldTest, InsertAWorld){
        system("rm -r universe 2>> /dev/null");
        std::string worldName = "test_world_name";
        int rc;
        
        //Make sure runs coreectly 
        World *world = new World();
        rc = world->Insert(worldName);
        if(rc){std::cout << world->GetErrorLog() << std::endl;} 
        ASSERT_FALSE(rc) << world->GetErrorLog();

        //Check that db file exists
        std::string filename =  world->GetDatabaseFolderName() + "/" + 
            worldName + world->GetDatabaseFileExtention(); 
        std::ifstream f(filename);
        ASSERT_TRUE(f);

        //Clean up
        f.close();
        delete(world);
        system("rm -r universe 2>> /dev/null");
    }


    /* Test to confirm that Delete acctually deletes the
     * databse file.
    */
    TEST(WorldTest, DeleteAWorld){
        system("rm -r universe 2>> /dev/null");
        std::string worldName = "test_world_name";
        int rc;

        //Create db file
        World *world = new World();
        rc = world->Insert(worldName);
        if(rc){std::cout << world->GetErrorLog() << std::endl;} 
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);
        
        //Check that file was created 
        std::string filename =  world->GetDatabaseFolderName() + "/" + 
            worldName + world->GetDatabaseFileExtention(); 
        std::ifstream f(filename);
        ASSERT_TRUE(f);
        f.close();

        //Delete db file
        world = new World(); 
        rc = world->Delete(worldName);
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);

        //Check that file was deleted
        std::ifstream fCheck(filename);
        ASSERT_FALSE(fCheck);

        //Clean up
        fCheck.close();
        system("rm -r universe 2>> /dev/null");
    }

    /* Test to make sure that an error is thrown if 
     * the objlib tries to create a database file that
     * already exits.
    */
    TEST(WorldTest, InsertWorldThatAlreadyExists){
        system("rm -r universe 2>> /dev/null");
        std::string worldName = "test_world_name";
        int rc;

        //Create db file
        World *world = new World();
        rc = world->Insert(worldName);
        if(rc){std::cout << world->GetErrorLog() << std::endl;} 
        ASSERT_FALSE(rc) << world->GetErrorLog();
        delete(world);

        //try to create a second time
        world = new World();
        rc = world->Insert(worldName);
        delete(world);

        //delete up folder when finished
        system("rm -r universe 2>> /dev/null");
        ASSERT_EQ(1, rc);
    }

    /* Test that objlib only lists items that
     * end with the database file extention.
    */
    TEST(WorldTest, GetListOfThreeWorlds){
        system("rm -r universe 2>> /dev/null");
        World *world = new World();

        //create 3 database files
        for(int i=0; i < 3; i++){
            std::string dbName = std::to_string(i) + 
                world->GetDatabaseFileExtention();
            std::ofstream ofile(world->GetDatabaseFolderName() + "/" + dbName); 
            ofile << "test test test" << std::endl;
            ofile.close();
        }

        //create 1 dummy file
        std::ofstream ofile(world->GetDatabaseFolderName() + "/" + "dummy_file.txt");
        ofile << "test test test" << std::endl;
        ofile.close();

        //get list of database files
        int rc = world->ListWorlds();
        if(rc){world->GetErrorLog();}
        ASSERT_FALSE(rc);

        std::list<std::string> *resultsList = world->GetListOfWorlds();
        delete(world);
        system("rm -r universe 2>> /dev/null");
        ASSERT_EQ(3, resultsList->size());
    }

    /* Test that feeding the objlib an empty folder
     * does not throw an error.
    */
    TEST(WorldTest, GetListOfWorldsWithEmptyFolder){
        system("rm -r universe 2>> /dev/null");
        World *world = new World();
        
        //get list of database files
        int rc = world->ListWorlds();
        if(rc){world->GetErrorLog();}
        ASSERT_FALSE(rc);

        std::list<std::string> *resultsList = world->GetListOfWorlds();
        delete(world);
        system("rm -r universe 2>> /dev/null");
        ASSERT_EQ(0, resultsList->size());
    
    }

    /* Test that a file name with the database file extention not at
     * the end is not listed. file.wrd.old
    */
    TEST(WorldTest, GetListOfWorldsWithMalformedFilenames){
        system("rm -r universe 2>> /dev/null");
        World *world = new World();

        //create 1 malnamed file 'bad_file.wrd.old'
        std::string badFilename = "bad_file" + world->GetDatabaseFileExtention() + ".old";
        std::ofstream ofile_bad(world->GetDatabaseFolderName() + "/" + badFilename);
        ofile_bad << "bad bad bad" << std::endl;
        ofile_bad.close();

        //create 1 valid filename. 'good_file.wrd'
        std::string goodFile = "good_file" + world->GetDatabaseFileExtention();
        std::ofstream ofile_good(world->GetDatabaseFolderName() + "/" + goodFile);
        ofile_good << "good good good" << std::endl;
        ofile_good.close();
        
        //get list of database files
        int rc = world->ListWorlds();
        if(rc){world->GetErrorLog();}
        ASSERT_FALSE(rc);

        std::list<std::string> *resultsList = world->GetListOfWorlds();
        delete(world);
        system("rm -r universe 2>> /dev/null");
        ASSERT_EQ(1, resultsList->size());
    }


    

} // namespace

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
