/** @file
* @author Nathan Belue
* @date October 19, 2015
*
* This file contains the glass component class definition.  It inherits 
* from Component and represents a Minecraft glass block.
*
*/

#ifndef REDSTONE_COMPONENTS_GLASSBLOCK_H
#define REDSTONE_COMPONENTS_GLASSBLOCK_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Glass block class
	*
	* This class is a component that represents a Minecraft glass block.
	*
	*/
	class GlassBlock : public Component
	{

	public:


		/* Functions */

		GlassBlock() = default;
		GlassBlock(const GlassBlock &) = default;
		GlassBlock & operator =(const GlassBlock &) = delete;

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		GlassBlock * clone() const
		{
			return new GlassBlock(*this);
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
			return Component::ID::GLASS_BLOCK;
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
