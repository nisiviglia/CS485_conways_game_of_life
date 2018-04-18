/* 
 * The Frame receiver is in charge of all database operations on a frame.
*/

// This is the start of the header guard.
#ifndef FRAME_H
#define FRAME_H
#include "ReceiverBase.hpp"
#include <string>
#include <vector>
#include <list>

/// @brief The Frame Class is responsible for all operations on a frame
class Frame : public ReceiverBase {
    private:
        /// @brief ID of the frame to be operated on 
        int frameId = 0;

        /// @brief Database to be opened
        std::string worldName = "";

	/// @brief holds a list of all the frame id's in a world
	std::list<int> *listOfFrames = NULL;

    public:
        /// @brief Basic frame constructor
        ///
        /// @param inFrameId Frame ID to be operated on 
        /// @param inWorldName Database to be opened
        Frame(int inFrameId, std::string inWorldName);

	/// @brief Basic frame constructor
        ///
        /// @param inWorldName Database to be opened
        Frame(std::string inWorldName);

        /// @brief Insert a frame into the database
        ///
        /// @param inFrame Reference to the frame data to insert
        ///
        /// @return Zero for success, non-zero for failure
        int Insert(std::vector< std::pair<int, int> > &inFrame);

        /// @brief Replacce a frame into the database
        ///
        /// @param inFrame Reference to the frame data to insert
        ///
        /// @return Zero for success, non-zero for failure
        int ReplaceFrame(std::vector< std::pair<int, int> > &inFrame);

        /// @brief Get a frame from the database
        ///
        /// @param inFrame Reference to the frame data to insert
        ///
        /// @return Zero for success, non-zero for failure 
        int Get(std::vector< std::pair<int, int> > &inFrame);

        /// don't need any parameters because we know frameId from object
        ///
        /// @brief Delete a frame from database
        /// 
        /// @return zero for success, non-zero for failure
        int Delete();

 	/// @brief Lists all the frame id's in a world
	 ///
	/// @param worldName name of the world to list frames in
	///
	/// @return zero for success, non-zero for failure
	int ListFrames();
		
	/// @brief returns the list of frames
	///
	/// @return list<int>
	std::list<int>* GetListOfFrames();

        /// @brief Gets the frame ID
        ///
        /// @return integer 
        int GetFrameId();
        
        /// @brief Gets the world name
        ///
        /// @return string
        std::string GetWorldName();
};

//This is the end of the header guard.
#endif
