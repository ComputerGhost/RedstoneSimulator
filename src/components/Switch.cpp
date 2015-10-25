/**
* @author Nathan Belue
* @date October 22, 2015
*
* This file contains the redstone switch class definition.  It inherits from
* the Component class and represents any Minecraft Redstone switch.
*
*/

#include "Switch.h"

#include "SolidBlock.h"


/**
* @brief Update the component
* @brief engine	The engine being used
* @brief coords	The coordinates of the component in the map
*/
void Redstone::Switch::update(
	Redstone::Engine & engine,
	const Redstone::Map::Coordinates & coords)
{
	if (engine.getTickNumber() == 0) {
		this->_engine = &engine;
		this->_coords = coords;
	}
}


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::Switch::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;

	auto bPtr = dynamic_cast<const Switch *>(&b);
	if (bPtr->isOn() != this->isOn())
		return false;
	if (bPtr->getDirection() != this->getDirection())
		return false;

	return true;
}


/* Helper functions */

/**
 * @brief Flip the switch
 */
void Redstone::Switch::flip()
{
	this->_isOn = !this->_isOn;
	if ( this->_engine )
		this->updateSurrounding(*this->_engine, this->_coords);
}

