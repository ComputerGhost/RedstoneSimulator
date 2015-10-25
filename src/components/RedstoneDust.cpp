/**
* @author Nathan Belue
* @date October 19, 2015
*
* This file contains the redstone dust component class definition.  It 
* inherits from Component and represents Minecraft redstone dust.
*
*/

#include "RedstoneDust.h"

#include "RedstoneBlock.h"
#include "RedstoneTorch.h"
#include "SolidBlock.h"
#include "Switch.h"
#include <algorithm>


/**
* @brief Update the component
* @brief engine	The engine being used
* @brief coords	The coordinates of the component in the map
*/
void Redstone::RedstoneDust::update(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	// We'll test against these for state changes
	int oldLevel = this->_level;
	int oldDirection = this->_direction;
	this->_level = 0;
	this->_direction = 0;

	// Find neighbors that may update us
	for (auto d : this->_diagonals)
		d = true;
	this->_processNeighbors(engine.getMap(), coords);
	this->_processDiagonals(engine.getMap(), coords);

	// If we get updated, update surrounding
	if (oldLevel != this->_level || oldDirection != this->_direction) {
		this->updateSurrounding(engine, coords);
		this->_updateDiagonals(engine, coords);
	}
	else {
		this->_level = oldLevel;
		this->_direction = oldDirection;
	}
}


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::RedstoneDust::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;

	const RedstoneDust * bPtr = dynamic_cast<const RedstoneDust*>(&b);
	if (this->_direction != bPtr->_direction)
		return false;
	if (this->_level != bPtr->_level)
		return false;

	return true;
}


/**
* @brief Get whether the redstone points in some direction
* @param direction	The direction to test
* @returns true if it points in the given direction
*/
bool Redstone::RedstoneDust::hasDirection(const Map::Direction & direction) const
{
	switch (direction)
	{
	case Map::Direction::NORTH:
		return (this->_direction & 0x8) != 0;
	case Map::Direction::EAST:
		return (this->_direction & 0x4) != 0;
	case Map::Direction::SOUTH:
		return (this->_direction & 0x2) != 0;
	case Map::Direction::WEST:
		return (this->_direction & 0x1) != 0;
	}
	return false;
}


/* Helper functions */


/**
 * @brief Check to see if surrounding blocks will change our state
 * @param map		The map being used
 * @param coords	The current coordinates
 */
void Redstone::RedstoneDust::_processNeighbors(
	Redstone::Map & map, 
	const Redstone::Map::Coordinates & coords)
{
	// We need this for all six directions
	auto process = [&](int x, int y, int z, Map::Direction direction) {
		Map::Coordinates test_coords = coords;
		test_coords.x += x;
		test_coords.y += y;
		test_coords.z += z;
		this->_processNeighbor(map.get(test_coords), direction, test_coords);
	};

	// Now let's process surrounding blocks
	process(1, 0, 0, Map::Direction::WEST);
	process(-1, 0, 0, Map::Direction::EAST);
	process(0, 0, 1, Map::Direction::NORTH);
	process(0, 0, -1, Map::Direction::SOUTH);
	process(0, 1, 0, Map::Direction::DOWN);
	process(0, -1, 0, Map::Direction::UP);
}


/**
 * @brief Process diagonals, if they are redstone
 * @param map		The map being used
 * @param coords	The current coordinates
 */
void Redstone::RedstoneDust::_processDiagonals(
	Redstone::Map & map,
	const Redstone::Map::Coordinates & coords)
{
	for (int i = 0; i != 8; ++i) {
		if (this->_diagonals[i] == false)
			continue;

		// Figure out coordinates
		Map::Coordinates test_coords = coords;
		test_coords.y += (i < 4) ? 1 : -1;
		switch (i % 4) {
		case 0: ++test_coords.z; break;
		case 1: ++test_coords.x; break;
		case 2: --test_coords.z; break;
		case 3: --test_coords.x; break;
		}

		// Verify it is redstone
		this->_diagonals[i] = false;
		Component * test_comp = map.get(test_coords);
		if (test_comp == nullptr)
			continue;
		if (test_comp->getId() != Component::ID::REDSTONE_DUST)
			continue;
		this->_diagonals[i] = true;

		// Process direction
		this->_direction |= (1 << (i % 4));

		// Process signal
		int newLevel = dynamic_cast<const RedstoneDust *>(test_comp)->getLevel() - 1;
		this->_level = std::max(this->_level, newLevel);
	}
}


/**
 * @brief Check to see if a surrounding block will change our state
 * @param component	The component next to us
 * @param direction	The direction towards us
 * @param coords	Coordinates of component
 */
void Redstone::RedstoneDust::_processNeighbor(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction, 
	const Redstone::Map::Coordinates & coords )
{
	if (component == nullptr)
		return;

	switch (component->getId()) {

	case Component::ID::REDSTONE_BLOCK:

		this->_onRedstoneBlockBeside( direction );
		break;

	case Component::ID::REDSTONE_DUST:

		this->_onRedstoneDustBeside(component, coords, direction);
		break;

	case Component::ID::REDSTONE_TORCH:

		this->_onTorchBeside(component, direction);
		break;

	case Component::ID::SOLID_BLOCK:

		if (direction == Map::Direction::DOWN)
			this->_onSolidAbove(component, coords);
		else if (direction == Map::Direction::UP)
			this->_onSolidBelow(component, coords);
		else
			this->_onSolidBeside(component, coords, direction);
		break;

	case Component::ID::SWITCH:

		this->_onSwitchBeside(component, direction);
		break;

	}
}


/**
 * @brief Update surrounding diagonals
 * @param engine	The engine being used
 * @param coords	The coordinates at the center
 */
void Redstone::RedstoneDust::_updateDiagonals(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	for (int i = 0; i != 8; ++i) {
		if (this->_diagonals[i] == false)
			continue;

		// Figure out coordinates
		Map::Coordinates update_coords = coords;
		update_coords.y += (i < 4) ? 1 : -1;
		switch (i % 4) {
		case 0: ++update_coords.z; break;
		case 1: ++update_coords.x; break;
		case 2: --update_coords.z; break;
		case 3: --update_coords.x; break;
		}

		/* // done in _processDiagonals
		Component * update_comp = engine.getMap().get(update_coords);
		if (update_comp == nullptr)
			continue;
		if (update_comp->getId() != Component::ID::REDSTONE_DUST)
			continue;
		*/

		// Mark it to be updated!
		this->markUpdate(engine, update_coords);
	}
}


/**
 * @brief Make us point from a power source
 * @param direction	Direction from source to us
 */
void Redstone::RedstoneDust::_attachDirection(
	const Redstone::Map::Direction & direction)
{
	switch (direction) {
	case Map::Direction::SOUTH: this->_direction |= 0x8; break;
	case Map::Direction::WEST: this->_direction |= 0x4; break;
	case Map::Direction::NORTH: this->_direction |= 0x2; break;
	case Map::Direction::EAST: this->_direction |= 0x1; break;
	}
}


/* Helper functions */


/**
 * @brief Handles the case where there's a solid above us
 * @param component	The component above us
 * @param coords	The coordinates of the solid
 */
void Redstone::RedstoneDust::_onSolidAbove(
	const Redstone::Component * component,
	const Redstone::Map::Coordinates & coords)
{
	// First, let's not process diagonal redstone above us
	for (int i = 0; i != 4; ++i)
		this->_diagonals[i] = false;

	// Second, let's see if it's strongly powered
	if (dynamic_cast<const SolidBlock *>(component)->isStronglyPowered())
		this->_level = 15;
}


/**
 * @brief Handles the case where there's a solid below us
 * @param component	The component below us
 * @param coords	The coordinates of the solid
 */
void Redstone::RedstoneDust::_onSolidBelow(
	const Redstone::Component * component,
	const Redstone::Map::Coordinates & coords)
{
	if (dynamic_cast<const SolidBlock *>(component)->isStronglyPowered())
		this->_level = 15;
}


/**
 * @brief Handles the case where there's a solid beside us
 * @param component	The component beside us
 * @param coords	The coordinates of the solid
 * @param direction	Direction to us
 */
void Redstone::RedstoneDust::_onSolidBeside(
	const Redstone::Component * component,
	const Redstone::Map::Coordinates & coords, 
	const Redstone::Map::Direction & direction )
{
	// First, let's not process diagonal redstone below here
	switch (direction) {
	case Map::Direction::SOUTH: this->_diagonals[4] = false; break;
	case Map::Direction::WEST: this->_diagonals[5] = false; break;
	case Map::Direction::NORTH: this->_diagonals[6] = false; break;
	case Map::Direction::EAST: this->_diagonals[7] = false; break;
	}

	// Second, let's see if it's strongly powered
	if (dynamic_cast<const SolidBlock *>(component)->isStronglyPowered())
		this->_level = 15;
}


/**
 * @brief Handles the case where there's redstone dust beside us
 * @param component	The component beside us
 * @param coords	The coordinates of the dust
 * @param direction	Direction to us
 */
void Redstone::RedstoneDust::_onRedstoneDustBeside(
	const Redstone::Component * component,
	const Redstone::Map::Coordinates & coords,
	const Redstone::Map::Direction & direction)
{
	// First, set our direction
	this->_attachDirection(direction);

	// Second, set our level
	int newLevel = dynamic_cast<const RedstoneDust *>(component)->getLevel() - 1;
	this->_level = std::max(this->_level, newLevel);
}


/**
 * @brief Handles the case where there's a redstone block beside us
 * @param direction	Direction to us
 */
void Redstone::RedstoneDust::_onRedstoneBlockBeside(
	const Redstone::Map::Direction & direction)
{
	this->_attachDirection(direction);
	this->_level = 15;
}


/**
* @brief Handles the case where there's a switch beside us
* @param component	The component beside us
* @param direction	Direction to us
*/
void Redstone::RedstoneDust::_onSwitchBeside(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction)
{
	this->_attachDirection(direction);
	if (dynamic_cast<const Switch *>(component)->isOn())
		this->_level = 15;
}


/**
* @brief Handles the case where there's a torch beside us
* @param component	The component beside us
* @param direction	Direction to us
*/
void Redstone::RedstoneDust::_onTorchBeside(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction)
{
	this->_attachDirection(direction);
	if (dynamic_cast<const RedstoneTorch *>(component)->isOn())
		this->_level = 15;
}

