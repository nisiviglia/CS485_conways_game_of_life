/* This is the Object Library Dispatcher for project WebMind.
 * This is the only file you will need to include to use the Object Library.
 *
 * The idea behind this is to be simple interface that uses a formula 
 * to accomplish operations on the database as apposed to having a dozen functions.
 * Once the function is called it decodes the parameters and calls the proper class
 * and/or method to complete the operation.
 */

// This is the start of the header guard.
#ifndef OBJDISPATCHER_H
#define OBJDISPATCHER_H
#include <string>
#include <list>
#include <vector>

/**The main namespace for the object library. */
namespace ol {

    /**This is a list of operations the object library can do. */
    enum Ops {
        INSERT = 0, /**< Add an item to the db. */
        GET  = 2, /**< Get an existing item from the db. */
        MODIFY = 4, /**< Modify an existing item in the db. */
        DELETE = 8, /**< Delete an item from the db. */
    };
    
    /**This is a list of objects the library will operate on.*/
    enum Obj_Type {
        FRAME = 0, /**< frame object */
        WORLD = 2, /**< world object */
        THING = 4, /**< thing object */
    };
    
    /**This is a list of locations where the objects are stored.*/
    enum Location {
        LOC_WORLD = 0, /**< Look in the world */
        LOC_THING = 2, /**< Look in the thingverse */
    };
    
    /**The ObjDispatcher class is the only class needed to use the Object Library.
     *Usage is simple and self explanatory. For example, to add a world "testworld"
     *you could use the following commands.
     *      ol::ObjDispatcher dispatch; 
     *      dispatch.single(ol::INSERT, ol::WORLD, "", "testWorld", 0);
     *
     *If you wanted to add frame 41 to world "testWorld" use the following.
     *      dispatch.single(ol::INSERT, ol::FRAME, "41", "testWorld", &obj);
     * 
     *If you wanted to delete frame 41 from world "testWorld" do the following.
     *      dispatch.single(ol::DELETE, ol::FRAME, "41", "testWorld", null);
     *
     *Add thing to thingverse.
     *      dispatch.single(ol::INSERT, ol::THING, "mything", "", &obj);
     *
     *If you want a list of all the worlds on the machine do the following command.
     *      dispatch.getList(ol::WORLD, "");   
     */
    class ObjDispatcher {
        private:
            /// @brief This holds any details on any error that occurred. 
            std::string errorLog;

        public:
		
	    ObjDispatcher() = default;
	    virtual ~ObjDispatcher() = default;

            /**@brief This function is used to do single operations on the db.
            *
            * @param ops the operation to be performed.
            * @param obj what object to work on.
            * @param frame_name name of the frame. Put "" if none.
            * @param world_name name of the world. Put "" if none.
            * @param pointer to object to be saved or where data is to be stored.
            *
            * @return integer. Zero is returned for success and one for error.
            */
            virtual int Single(
                    Ops ops, 
                    Obj_Type obj, 
                    int frame_name, 
                    std::string world_name, 
                    std::vector< std::pair<int, int> > *data);
            
            /// @brief This function is used to get a list of an item 
            ///
            /// @param onj what object to list.
            /// @param location narrow search to this area. Put "" if none. 
            /// @param returnList list that data will be inserted into.
            ///
            /// @return list of names.
            virtual int GetList(
                    Obj_Type onj, 
                    std::string location, 
                    std::list<std::string> *returnList);

	    /// @brief This function is used to get a list of an item 
	    ///
	    /// @param onj what object to list.
	    /// @param location narrow search to this area. Put "" if none. 
	    /// @param returnList list that data will be inserted into.
	    ///
	    /// @return list of names.
	    int GetList(
	            Obj_Type onj, 
	            std::string location, 
	            std::list<int> *returnList);

            /// @brief This will return information on any error that could of 
            /// occurred.
            ///
            /// @return string
            virtual std::string GetErrorLog();
    };
}

//This is the end of the header guard.
#endif
