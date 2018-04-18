/* This is the base class for the Receivers. In here should be all the data and 
 * functions that will be used by multiple receivers.
*/
#include "../include/all/ReceiverBase.hpp"
#include "../include/all/unqlite.h"
#include "dirent.h"
#include <string>
#include <iostream>

//linux
#include <sys/types.h>
#include <sys/stat.h>

//Inits the db folder
ReceiverBase::ReceiverBase(){
    InitDatabaseFolder();
}

//Opens a db connection
ReceiverBase::ReceiverBase(std::string  worldName){

    //Open database connection
    worldName = DB_FOLDER_NAME + "/" + worldName + DB_FILE_EXTENTION; 
    rc = unqlite_open(&pDb, worldName.c_str(), UNQLITE_OPEN_READWRITE);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to open database");
    }

    //Create a new db cursor instance
    rc = unqlite_kv_cursor_init(pDb, &pCursor); 
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to create kv cursor");
    }
}

ReceiverBase::~ReceiverBase(){
    //Close databse cursor if one is open.
    if(pCursor != NULL){
        unqlite_kv_cursor_release(pDb,pCursor);
    }

    //Close database connection if one is open.
    if(pDb != NULL){
        unqlite_close(pDb);
    }
}

//Checks for database folder and creates one if its not found.
int ReceiverBase::InitDatabaseFolder(){
    DIR *dir;
    dir = opendir(DB_FOLDER_NAME.c_str());

    if(dir == NULL){
        //linux
        int mkResult = 
            mkdir(DB_FOLDER_NAME.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
        if(mkResult == -1){
            Fatal(0, "Unable to create database folder");
            return 1;
        }
    }

    return 0;
}

// Returns the database foldername
std::string ReceiverBase::GetDatabaseFolderName(){
    return DB_FOLDER_NAME;
}

// Returns the database file extention
std::string ReceiverBase::GetDatabaseFileExtention(){
    return DB_FILE_EXTENTION;
}

//Error handler
//Copied (for the most part) from the unqlite_kv_intro.c file.
void ReceiverBase::Fatal(unqlite *pDb, const char *zMsg){
	if( pDb ){
		const char *zErr;
		int iLen = 0; /* Stupid cc warning */

		/* Extract the database error log */
		unqlite_config(pDb,UNQLITE_CONFIG_ERR_LOG,&zErr,&iLen);
		if( iLen > 0 ){
			/* Save the DB error log */
            errorLog = zErr;
		}
	}else{
		if( zMsg ){
		    errorLog = zMsg;	
		}
	}
}

//Returns error log
std::string ReceiverBase::GetErrorLog(){
    return errorLog;
}

