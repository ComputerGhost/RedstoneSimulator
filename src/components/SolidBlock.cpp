/**
* @author Nathan Belue
* @date October 19, 2015
*
* This file contains the solid block component class definition.  It
* inherits from the Component class and represents any solid Minecraft
* block.
*
*/

#include "SolidBlock.h"

#include "RedstoneDust.h"
#include "RedstoneTorch.h"
#include "Switch.h"


/**
 * @brief Update the component
 * @brief engine	The engine being used
 * @brief coords	The coordinates of the component in the map
 */
void Redstone::SolidBlock::update(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	Map & map = engine.getMap();

	// We'll test against these state changes
	bool oldStrength = this->_stronglyPowered;
	int oldLevel = this->_powerLevel;
	this->_stronglyPowered = false;
	this->_powerLevel = 0;

	// We need this for all six directions
	auto process = [&](int x, int y, int z, Map::Direction direction) {
		Map::Coordinates test_coords = coords;
		test_coords.x += x;
		test_coords.y += y;
		test_coords.z += z;
		this->_processNeighbor(map.get(test_coords), direction, test_coords);
	};

	// Now let's do it
	process(1, 0, 0, Map::Direction::WEST);
	process(-1, 0, 0, Map::Direction::EAST);
	process(0, 0, 1, Map::Direction::NORTH);
	process(0, 0, -1, Map::Direction::SOUTH);
	process(0, 1, 0, Map::Direction::DOWN);
	process(0, -1, 0, Map::Direction::UP);

	// If we updated, update surrounding
	if (oldStrength != this->_stronglyPowered || oldLevel != this->_powerLevel) {
		this->updateSurrounding(engine, coords);
	}
	else {
		this->_powerLevel = oldLevel;
		this->_stronglyPowered = oldStrength;
	}
}


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::SolidBlock::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;

	auto bPtr = dynamic_cast<const SolidBlock *>(&b);
	if (bPtr->getPowerLevel() != this->getPowerLevel())
		return false;
	if (bPtr->isStronglyPowered() != this->isStronglyPowered())
		return false;

	return true;
}


/**
 * @brief Check to see if a surrounding block will change our state
 * @param component	The component next to us
 * @param direction	The direction towards us
 * @param coords	Coordinates of component
 */
void Redstone::SolidBlock::_processNeighbor(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction,
	const Redstone::Map::Coordinates & coords)
{
	if (!component)
		return;

	switch (component->getId()) {

	case Component::ID::REDSTONE_DUST:

		if (direction == Map::Direction::UP)
			;
		else if (direction == Map::Direction::DOWN)
			this->_onRedstoneDustAbove(component);
		else
			this->_onRedstoneDustBeside(component, direction);
		break;

	case Component::ID::REDSTONE_TORCH:

		if (direction == Map::Direction::UP)
			this->_onRedstoneTorchBelow(component);
		break;

	case Component::ID::SWITCH:

		this->_onSwitchBeside(component, direction);
		break;

	}
}


/**
* @brief Handles the case where there's redstone dust beside us
* @param component	The component beside us
*/
void Redstone::SolidBlock::_onRedstoneDustAbove(
	const Redstone::Component * component)
{
	auto dust = dynamic_cast<const RedstoneDust *>(component);

	this->_powerLevel = std::max(this->_powerLevel, dust->getLevel());
}


/**
* @brief Handles the case where there's redstone dust beside us
* @param component	The component beside us
* @param direction	Direction to us
*/
void Redstone::SolidBlock::_onRedstoneDustBeside(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction)
{
	auto dust = dynamic_cast<const RedstoneDust *>(component);

	if (dust->hasDirection(direction))
		this->_powerLevel = std::max(this->_powerLevel, dust->getLevel());
}


/**
 * @brief Handles the case where there's a redstone torch below us
 * @param component	The component below us
 */
void Redstone::SolidBlock::_onRedstoneTorchBelow(
	const Redstone::Component * component)
{
	auto torch = dynamic_cast<const RedstoneTorch *>(component);

	if (torch->isOn())
		this->_stronglyPowered = true;
}


/**
 * @brief Handles the case where there's a switch beside us
 * @param component	The component beside us
 * @param direction	Direction to us
 */
void Redstone::SolidBlock::_onSwitchBeside(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction)
{
	auto toggle = dynamic_cast<const Switch *>(component);

	if (toggle->getDirection() == direction) {
		if (toggle->isOn())
			this->_stronglyPowered = true;
	}
}

