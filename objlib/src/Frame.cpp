/* 
 * The Frame receiver is in charge of all database operations on a frame.
*/

#include "../include/all/Frame.hpp"
#include "../include/all/ReceiverBase.hpp"
#include "../include/all/unqlite.h"
#include <string>
#include <vector>
#include <iostream>

/*
 * A simple constructor that calls the ReceiverBase(std::string) base constructor
 * that opens the database connection.
*/
Frame::Frame(int inFrameId, std::string inWorldName) : ReceiverBase(inWorldName){
    frameId = inFrameId;
    worldName = inWorldName;
}

/*
* A simple constructor that calls the ReceiverBase(std::string) base constructor
* that opens the database connection. This constructor skips the frame id.
*/
Frame::Frame(std::string inWorldName) : ReceiverBase(inWorldName){
    frameId = -1;
    worldName = inWorldName;
}

/*
 * The Function will Insert a frame into a database.
 * First it makes sure the frame id does not already exist.
 * Then it calculates the size of the array and stores the data. 
*/
int Frame::Insert(std::vector< std::pair<int, int> > &inFrame){

    //Check to make sure frame id does not exist by trying to fetch it.
    unqlite_int64 nBytes = 0;
    rc = unqlite_kv_fetch(pDb, &frameId, -1, NULL, &nBytes);
    if (rc == UNQLITE_OK){
        Fatal(0, "Frame Id already exists");
        return 1; 
    }

    //Store data
    unqlite_int64 sizeOfFrame = sizeof(inFrame.data()) * inFrame.size();
    rc = unqlite_kv_store(pDb, &frameId, -1, inFrame.data(), sizeOfFrame);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to insert frame");
        return 1; 
    }

    return 0;
}

/*
 * deletes the current frame from the database
 * using frameId, worldName of the object
*/
int Frame::Delete()
{
    rc = unqlite_kv_delete(pDb, &frameId, -1);
    if (rc == UNQLITE_OK){
        return 0; 
    }
    else if (rc == UNQLITE_NOTFOUND){
        Fatal(0, "Frame Id does not exist");
        return 1; 
    }
    else{
        Fatal(0, "Unknown error when deleting frame");
        return 2; 
    }

    Fatal(0, "UNEXPECTED BEHAVIOUR INSIDE DELETE FRAME");
    return 3; //should never get here
}

/*
 * The Function will preplace a frame into a database.
 * First it makes sure the frame id does not already exist.
 * Then it calculates the size of the array and stores the data. 
*/
int Frame::ReplaceFrame(std::vector< std::pair<int, int> > &inFrame){

    //Check to make sure frame id exists by trying to fetch it.
    unqlite_int64 nBytes = 0;
    rc = unqlite_kv_fetch(pDb, &frameId, -1, NULL, &nBytes);
    if (rc != UNQLITE_OK){
        Fatal(0, "Frame Id does not exist");
        return 1; 
    }

    //Store data
    unqlite_int64 sizeOfFrame = sizeof(inFrame.data()) * inFrame.size();
    rc = unqlite_kv_store(pDb, &frameId, -1, inFrame.data(), sizeOfFrame);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to insert frame");
        return 1; 
    }

    return 0;
}


/*
 * The function will reterive a frame from the database.
 * To do this it first checks to make sure it exists and
 * to get its size. Following this it makes sure data is
 * there to reterive and resises the array to fit the data.
 * After that the data is reterived.
*/
int Frame::Get(std::vector< std::pair<int, int> > &inFrame){

    //Check if frame exists and get its size
    unqlite_int64 nBytes = 0;
    rc = unqlite_kv_fetch(pDb, &frameId, -1, NULL, &nBytes);
    if (rc != UNQLITE_OK){
        Fatal(0, "Frame Id does not exists");
        return 1; 
    }

    //Check to see if there's data to retrieve 
    if(nBytes == 0){
        inFrame.resize(0);
        return 0;
    }

    //Allocate space for frame data
    int64_t numberOfPairs = nBytes / sizeof(std::pair<int, int>);
    inFrame.resize(numberOfPairs);
    
    //Get frame data 
    rc = unqlite_kv_fetch(pDb, &frameId, -1, inFrame.data(), &nBytes);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to fetch meta data");
        return 1;
    }
    
    return 0;
}

/*
* This function will iterate through the choosen db and add frame id's
* to the listOfFrames list.
*/
int Frame::ListFrames(){
	
    //init list that will hold frame id's
    listOfFrames = new std::list<int>;

    //point cursor to first entry
    rc = unqlite_kv_cursor_first_entry(pCursor);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to locate first entry");
        return 1;
    }

    //iterate over the database and add frame id's to list
    while( unqlite_kv_cursor_valid_entry(pCursor) ){

        int returnKey = 0;
        int sizeofthis = sizeof(int);
           
        //get next key
        rc = unqlite_kv_cursor_key(pCursor, &returnKey, &sizeofthis);
         
        //skip key used to init database
        if(returnKey == DB_INIT_KEYVALUE){
            unqlite_kv_cursor_next_entry(pCursor);
            continue;
        }

        //store key in list 
        listOfFrames->push_back(returnKey);

        //move to next entry
        unqlite_kv_cursor_next_entry(pCursor);
    }

    return 0;
}

//returns a list of frames that the ListFrames function gathered.
std::list<int>* Frame::GetListOfFrames(){
    return listOfFrames;
}

/*
 * Returns the frame id 
*/
int Frame::GetFrameId(){
    return frameId;
}

/*
 * Returns the world name 
*/
std::string Frame::GetWorldName(){
 return worldName;
}
