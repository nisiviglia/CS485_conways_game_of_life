/* The World receiver is in charge of all database operations on a world.
*/

// This is the start of the header guard.
#ifndef WORLD_H
#define WORLD_H
#include "ReceiverBase.hpp"
#include <string>
#include <list>

/// @brief The World receiver is in charge of all database operations on a world.
class World : public ReceiverBase {
    private:
        std::list<std::string> *listOfWorlds = NULL;

    public:
        
        /// @brief World Constructor
        World();

        /// @brief Add a world to the database.
        ///
        /// @param name name of the database to be added.
        ///
        /// @return returns zero on success and one on failure. 
        int Insert(std::string name);

        /// @delete a world 
        int Delete(std::string name);

        /// @brief Gets list of worlds on computer in puts in listOfWorlds 
        ///
        /// @return returns zero for success and non-zero for failure.
        int ListWorlds();

        /// @brief returns the listOfWorlds object.
        ///
        /// @return returns list of worlds found by ListWorlds().
        std::list<std::string>* GetListOfWorlds();
}; 

//This is the end of the header guard.
#endif
