/**
* @author Nathan Belue
* @date October 21, 2015
*
* This file contains the redstone block component class definition.  It 
* inherits from Component and represents a Minecraft redstone block.
*
*/

#include "RedstoneBlock.h"


/**
* @brief Comparison operator
* @brief b	The component class to compare to
* @returns true if they are equal
*/
bool Redstone::RedstoneBlock::operator==(const Redstone::Component & b) const
{
	if (b.getId() != this->getId())
		return false;
	return true;
}

