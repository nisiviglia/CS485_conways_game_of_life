/*This is the base class for the Receivers. In here should be all the data and 
 * functions that will be used by multiple receivers.
*/

// This is the start of the header guard.
#ifndef RECEIVERBASE_H 
#define RECEIVERBASE_H
#include "unqlite.h"
#include <string>

/// @brief This is the base class for the receivers. In here should be all the data
/// and functions used by multiple receivers.
class ReceiverBase{
    private:
        /// @brief This will contain any information on an error if it occurs.
        std::string errorLog = "";
        
        /// @brief Check for database folds and create if not found.
        ///
        /// @return zero for success and non-zero for failure. 
        int InitDatabaseFolder();

    protected:
        /// @brief Database handle
        unqlite *pDb = NULL; 
        
	/// @brief Database cursor
	unqlite_kv_cursor *pCursor = NULL;

        /// @brief Used to hold the return value of a database entry. Lets us know
        // if the operation was successful. 
        int rc = 0;
        
        /// @brief This will hold the name of the database folder.
        const std::string DB_FOLDER_NAME = "universe";

        /// @brief This is the file extention for a database.
        const std::string DB_FILE_EXTENTION = ".wrd";

	/// @brief This is used to init a new database
	const int DB_INIT_KEYVALUE = -1;
        
        /// @brief Database error handler. In the event of an error this function
        /// will give more information on what went wrong. 
        ///
        /// @param pDb Database handle used in the failed operation.
        /// @param zMsg message to send back if database error log is empty.
        void Fatal(unqlite *pDb, const char *zMsg);

    public:
        /// @brief Constructor will init db folder but NOT make connection.
        ReceiverBase();

        /// @brief Constructor that will init db folder and 
        /// open the database connection.
        ReceiverBase(std::string  worldName);
        
        /// @brief Destructor that will close the database connection.
        virtual ~ReceiverBase();

        /// @brief Returns the database folder name
        ///
        /// @return string 
        std::string GetDatabaseFolderName();

        /// @brief Returns the database file extention
        ///
        /// @return string 
        std::string GetDatabaseFileExtention();

        /// @brief Returns errorLog 
        ///
        /// @return string
        std::string GetErrorLog();
};

// This is the end of the header guard.
#endif
