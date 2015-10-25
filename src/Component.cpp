/**
 * @author Nathan Belue
 * @date October 18, 2015
 *
 * Source code for the component base class
 *
 */

#include "Component.h"


/**
 * @brief Update surrounding in NSEWUD direction
 * @param engine	The engine being used
 * @param coords	The coordinates' center
 */
void Redstone::Component::updateSurrounding(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	Map::Coordinates test_coords = coords;

	--test_coords.x;
	this->markUpdate(engine, test_coords);

	test_coords.x += 2;
	this->markUpdate(engine, test_coords);

	--test_coords.x;
	--test_coords.z;
	this->markUpdate(engine, test_coords);

	test_coords.z += 2;
	this->markUpdate(engine, test_coords);

	--test_coords.z;
	--test_coords.y;
	this->markUpdate(engine, test_coords);

	test_coords.y += 2;
	this->markUpdate(engine, test_coords);
}


