/** @file
* @author Nathan Belue
* @date October 21, 2015
*
* This file contains the redstone block component class definition.  It 
* inherits from Component and represents a Minecraft redstone block.
*
*/

#ifndef REDSTONE_COMPONENTS_REDSTONEBLOCK_H
#define REDSTONE_COMPONENTS_REDSTONEBLOCK_H

#include "../Map.h"
#include "../Component.h"

/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Redstone block class
	*
	* This class is a component that represents a Minecraft redstone block.
	*
	*/
	class RedstoneBlock : public Component
	{

	public:


		/* Functions */

		RedstoneBlock() = default;
		RedstoneBlock(const RedstoneBlock &) = default;
		RedstoneBlock & operator = (const RedstoneBlock &) = delete;

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		RedstoneBlock * clone() const
		{
			return new RedstoneBlock(*this);
		}

		/**
		* @brief Update the component
		* @brief engine	The engine being used
		* @brief coords	The coordinates of the component in the map
		*/
		void update(Engine & engine, const Map::Coordinates & coords)
		{}

		/**
		* @brief Get the ID, which is easier to test than the class name.
		* @returns The ID of the component
		*/
		virtual Component::ID getId() const
		{
			return Component::ID::REDSTONE_BLOCK;
		}

		/**
		* @brief Comparison operator
		* @brief b	The component class to compare to
		* @returns true if they are equal
		*/
		bool operator==(const Component & b) const;

	};


} // End of namespace


#endif
