/**
* @author Nathan Belue
* @date October 22, 2015
*
* This file contains the redstone torch component class definition.  It 
* inherits from the Component class and represents any Minecraft Redstone 
* torch.
*
* Torches are weird and my research yielded conflicting specifics.  However, 
* built a timing device with repeater locks and instant repeaters to time 
* redstone torches for myself.
*
* My results are that redstone torches turn off after 3 game ticks and turn 
* on after 1 game tick.
*
*/

#include "RedstoneTorch.h"

#include "RedstoneBlock.h"
#include "SolidBlock.h"


/**
* @brief Update the component
* @brief engine	The engine being used
* @brief coords	The coordinates of the component in the map
*/
void Redstone::RedstoneTorch::update(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	Map & map = engine.getMap();

	// We'll test against this
	bool oldIsOn = this->_isOn;
	this->_isOn = true;

	// Only the block to which we're attached will change our state
	Map::Coordinates test_coords = coords;
	Map::Direction test_dir;
	switch (this->_direction) {
	case Map::Direction::DOWN:
		--test_coords.y;
		test_dir = Map::Direction::UP;
		break;
	case Map::Direction::SOUTH:
		++test_coords.z;
		test_dir = Map::Direction::NORTH;
		break;
	case Map::Direction::EAST:
		++test_coords.x;
		test_dir = Map::Direction::WEST;
		break;
	case Map::Direction::NORTH:
		--test_coords.z;
		test_dir = Map::Direction::SOUTH;
		break;
	case Map::Direction::WEST:
		--test_coords.x;
		test_dir = Map::Direction::EAST;
		break;
	}

	// Process that block
	this->_processNeighbor(map.get(test_coords), test_dir, test_coords);

	// If we updated
	if (oldIsOn != this->_isOn) {
		oldIsOn = this->_isOn;

		// On after 1
		if (this->_isOn) {
			this->updateSurrounding(engine, coords);
		}

		// Off after 3
		else if (!this->_prevStates[2]) {
			this->updateSurrounding(engine, coords);
		}

		// Can't turn off yet, so wait until the next tick
		else {
			this->_isOn = true;
			this->markNextUpdate(engine, coords);
		}

	}

	// Shift previous states, if not already done this game tick
	if (engine.getTickNumber() != this->_oldTickNumber) {
		this->_prevStates[2] = this->_prevStates[1];
		this->_prevStates[1] = this->_prevStates[0];
		this->_oldTickNumber = engine.getTickNumber();
	}
	this->_prevStates[0] = oldIsOn;
}


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::RedstoneTorch::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;

	auto bPtr = dynamic_cast<const RedstoneTorch *>(&b);
	if (bPtr->isOn() != this->isOn())
		return false;
	if (bPtr->getDirection() != this->getDirection())
		return false;

	return true;
}


/* Helper functions */


/**
* @brief Check to see if a surrounding block will change our state
* @param component	The component next to us
* @param direction	The direction towards us
* @param coords	Coordinates of component
*/
void Redstone::RedstoneTorch::_processNeighbor(
	const Redstone::Component * component,
	const Redstone::Map::Direction & direction,
	const Redstone::Map::Coordinates & coords)
{
	if (!component)
		return;

	switch (component->getId()) {

	case Component::ID::REDSTONE_BLOCK:

		this->_isOn = false;
		break;

	case Component::ID::SOLID_BLOCK:

		if (dynamic_cast<const SolidBlock *>(component)->getPowerLevel())
			this->_isOn = false;
		break;

	}
}

