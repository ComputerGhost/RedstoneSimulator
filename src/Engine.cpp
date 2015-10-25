/**
* @author Nathan Belue
* @date October 18, 2015
*
* Source code for the engine class
*
*/

#include "Engine.h"

#include "Component.h"
#include <algorithm>


/**
 * @brief Run the map for one tick
 */
void Redstone::Engine::run()
{
	while (! this->_nextUpdates.empty()) {
		this->_updates.push(this->_nextUpdates.front());
		this->_nextUpdates.pop();
	}

	// Go through updates
	while (!this->_updates.empty()) {
		Redstone::Map::Coordinates coords = this->_updates.front();
		Redstone::Component * comp = this->_map.get(coords);

		this->_updates.pop();
		if (comp)
			comp->update(*this, coords);
	}

	// Increment tick number, and done!
	++ this->_tickNumber;
}


/**
 * @brief Set the map to use, reset ticks to zero, and init
 * @param map	The map to use
 */
void Redstone::Engine::setMap(const Redstone::Map & map)
{
	this->_map = map;
	this->_tickNumber = 0;
	this->_updates = {};
	this->_nextUpdates = {};

	// We also need to set everything to update
	Map::Size size = map.size();
	Map::Coordinates coords;
	for (coords.x = 0; coords.x != size.x; ++coords.x) {
		for (coords.z = 0; coords.z != size.z; ++coords.z) {
			for (coords.y = 0; coords.y != size.y; ++coords.y)
				this->markNextUpdate(coords);
		}
	}

	// And now run one tick to update it
	this->run();
}


/**
 * @brief Mark a position to be updated
 * @param corods	The position to be updated
 */
void Redstone::Engine::markUpdate(
	const Redstone::Map::Coordinates & coords)
{
	this->_updates.push(coords);
}


/**
 * @brief Mark a position to be updated on the next game tick
 * @param coords	The position to be updated
 */
void Redstone::Engine::markNextUpdate(
	const Redstone::Map::Coordinates & coords)
{
	this->_nextUpdates.push(coords);
}

