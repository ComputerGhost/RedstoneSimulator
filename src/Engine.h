/** @file
* @author Nathan Belue
* @date October 18, 2015
*
* This file contains the engine class.  This is where the fun happens!
*
*/

#ifndef REDSTONE_ENGINE_H
#define REDSTONE_ENGINE_H

#include <queue>

#include "Map.h"


/* Redstone namespace */
namespace Redstone
{


	/* Forward declarations */
	class Component;


	/**
	* @brief Engine class that runs a map.
	*/
	class Engine
	{

	public:

		/**
		 * @brief Run the map for one tick
		 */
		void run();

		/**
		 * @brief Set the map to use, reset ticks to zero, and init
		 * @param map	The map to use
		 */
		void setMap(const Map & map);

		/**
		 * @brief Get the map currently being used
		 * @returns The map being used
		 */
		Map & getMap()
		{
			return this->_map;
		}

		/**
		 * @brief Check whether the circuit has become "still" (no updates)
		 * @returns True if there are to be more updates
		 */
		bool isStill() const
		{
			return this->_updates.empty() && this->_nextUpdates.empty();
		}

		/**
		 * @brief Get the tick number
		 * @returns The current tick number 0 to x
		 */
		int getTickNumber() const
		{
			return this->_tickNumber;
		}


	private:

		/* Helper functions for Component */
		friend class Component;

		/**
		 * @brief Mark a position to be updated
		 * @param coords	The position to be updated
		 */
		void markUpdate(const Map::Coordinates & coords);

		/**
		 * @brief Mark a position to be updated on the next game tick
		 * @param coords	The position to be updated
		 */
		void markNextUpdate(const Map::Coordinates & coords);


	private:

		/* Data */

		Map _map;
		int _tickNumber = 0;

		std::queue<Map::Coordinates> _updates;
		std::queue<Map::Coordinates> _nextUpdates;

	};


} // End of namespace


#endif

