/* The World receiver is in charge of all database operations on a world.
*/
#include "../include/all/World.hpp"
#include "../include/all/ReceiverBase.hpp"
#include "../include/all/unqlite.h"
#include "dirent.h"
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <cstdio>

World::World() : ReceiverBase(){
    //This is here to guarantee the correct ReceiverBase
    //constructor is called.
}

/* This function will insert a new world database into the database folder.
 * If one already exists it will throw an error.
*/
int World::Insert(std::string name){
    name = DB_FOLDER_NAME + "/" + name + DB_FILE_EXTENTION; 
    
    //Check if database already exists
    std::ifstream f(name);
	if(f.fail() != true){
        f.close();
        Fatal(0, "Database already exists");
        return 1;
	}

    //Create a database 
    rc = unqlite_open(&pDb, name.c_str(), UNQLITE_OPEN_CREATE);
    if(rc != UNQLITE_OK){
        Fatal(0, "Unable to open database");
        return 1;
    }
	
    //Init the new database by writing a key/value
    rc = unqlite_kv_store(pDb, &DB_INIT_KEYVALUE, -1, &DB_INIT_KEYVALUE, sizeof(DB_INIT_KEYVALUE));
    if( rc != UNQLITE_OK ){
        Fatal(0, "Failed to Init database");
        return 1;
    }

    return 0;
}

//delete a world 
int World::Delete(std::string  name){
    name = DB_FOLDER_NAME + "/" + name + DB_FILE_EXTENTION;
    std::ifstream f(name);
    if(f.fail() != true){
        //this is where a database will be deleated 
       int rc =  std::remove(name.c_str());
       if(rc){Fatal(0, "Unable To Remove Database");}
        
    }else{
        Fatal(0, "Database does not exist");
    }
    f.close();
    return 0;
}

/* This function will open the db folder and list all the databases inside.
 * Then it will store those names in the listOfWorlds object.
*/
int World::ListWorlds(){
    listOfWorlds = new std::list<std::string>(); 

    //open dirctory
    DIR *dir;
    dir = opendir(DB_FOLDER_NAME.c_str());
    if(dir == NULL){
        Fatal(0, "Unable to open database folder");
        return 1;
    }

    //list all databases
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
        std::string file = ent->d_name;
        int startPos = file.length() - DB_FILE_EXTENTION.length();
        if(file.find(DB_FILE_EXTENTION, startPos) != std::string::npos){
            file = file.substr(0, file.rfind(DB_FILE_EXTENTION));
            listOfWorlds->push_back(file);
        }
    
    }
     
    return 0;
}

//Return the list of worlds found by ListWorlds()
std::list<std::string>* World::GetListOfWorlds(){
    return listOfWorlds;
}


