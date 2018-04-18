#include "../include/all/ObjDispatcher.hpp"
#include "../include/all/World.hpp"
#include "../include/all/Frame.hpp"
#include "../include/all/unqlite.h"
#include <iostream>
#include <list>

namespace ol{
    
    /* All operations on a single object in the database go though here.
     * The function takes the parameters and runs through a set of if 
     * statements to decide what class and function should be called. Once decided
     * the class is created, called, and deleted. Zero is returned for 
     * success and non-zero indicating an error.
    */
    int ObjDispatcher::Single(Ops operation, Obj_Type object, int frame_name,
            std::string world_name, std::vector< std::pair<int, int> > *data){
        
        //Used to indicate success or failure following an operation.
        // I'm aware that using the magic number -100 is not the best practise.
        // Ideally we should have a list of enums for return codes.
        int rc = -100;
        
        //The order in whitch the transaction is checked goes like the following:
        // operation -> object
        if(operation == INSERT){
            if(object == WORLD){
                World *world = new World();
                rc = world->Insert(world_name);
                if(rc){errorLog = world->GetErrorLog();}
                delete(world);
            }
            else if(object == FRAME){
                Frame *frame = new Frame(frame_name, world_name);
                rc = frame->Insert(*data);
                if(rc){errorLog = frame->GetErrorLog();}
                delete(frame); 
            }
        }
        else if(operation == GET){
            if(object == FRAME){
                Frame *frame = new Frame(frame_name, world_name);
                rc = frame->Get(*data);
                if(rc){errorLog = frame->GetErrorLog();}
                delete(frame); 
            } 
        }
        else if(operation == DELETE){
            if(object == WORLD){
                World *world = new World();
                rc = world->Delete(world_name);
                if(rc){errorLog = world->GetErrorLog();}
                delete(world);
            }
            else if(object == FRAME){
                Frame *frame = new Frame(frame_name, world_name);
                rc = frame->Delete();
                if(rc){errorLog = frame->GetErrorLog();}
                delete(frame); 
            }
        }

        //Check for invalid operaton.
        //If rc is still -100 by the end of this function
        //then that concludes that no operation was done.
        if(rc == -100){
            errorLog = "Invalid Operation"; 
        }

        return(rc);
    }

    /* This function is used to create lists of items. Those items can be
     * worlds or things. list<string> is returned.
     * 
    */
    int ObjDispatcher::GetList(
            Obj_Type object, 
            std::string location, 
            std::list<std::string> *returnList){
        
        //Used to indicate success or failure following an operation.
        // I'm aware that using the magic number -100 is not the best practise.
        // Ideally we should have a list of enums for return codes.
        int rc = -100;

        if(object == WORLD){
            World *world = new World();
            rc = world->ListWorlds();
            if(rc){
                errorLog = world->GetErrorLog();
                return 1;
            }
            *returnList = *world->GetListOfWorlds();
            delete(world); 
        }

        //Check for invalid operaton.
        //If rc is still -100 by the end of this function
        //then that concludes that no operation was done.
        if(rc == -100){
            errorLog = "Invalid Operation"; 
        }

        return rc;    
    }

    /* This function is used to create lists of items. Those items can be
     * worlds or things. list<int> is returned.
     * 
    */
    int ObjDispatcher::GetList(
            Obj_Type object, 
            std::string location, 
            std::list<int> *returnList){
        
        //Used to indicate success or failure following an operation.
        // I'm aware that using the magic number -100 is not the best practise.
        // Ideally we should have a list of enums for return codes.
        int rc = -100;

        if(object == FRAME){
            Frame *frame = new Frame(location);
            rc = frame->ListFrames();
            if(rc){
                errorLog = frame->GetErrorLog();
                return 1;
            }
            *returnList = *frame->GetListOfFrames();
            delete(frame);
        }

        //Check for invalid operaton.
        //If rc is still -100 by the end of this function
        //then that concludes that no operation was done.
        if(rc == -100){
            errorLog = "Invalid Operation"; 
        }

        return rc;    
    }

    //This will return information on any error that could of occurred.
    std::string ObjDispatcher::GetErrorLog(){
        return errorLog;       
    }

}
