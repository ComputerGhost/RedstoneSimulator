/**
 *
 * @author Nathan Belue
 * @date October 19, 2015
 *
 * This file contains the glass component class definition.  It inherits
 * from Component and represents a Minecraft glass block.
 *
 */

#include "GlassBlock.h"


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::GlassBlock::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;
	return true;
}

