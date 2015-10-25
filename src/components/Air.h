/** @file
 * @author Nathan Belue
 * @date October 25, 2015
 *
 * This file contains the air component class.  Air...  it doesn't really 
 * do anything at all!
 *
 */

#ifndef REDSTONE_COMPONENTS_AIR_H
#define REDSTONE_COMPONENTS_AIR_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Air class
	*
	* This class is a component that represents air in Minecraft.
	*
	*/
	class Air : public Component
	{

	public:


		/* Functions */

		Air() = default;
		Air(const Air &) = default;
		Air & operator =(const Air &) = delete;

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		Air * clone() const
		{
			return new Air(*this);
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
			return Component::ID::AIR;
		}

		/**
		* @brief Comparison operator
		* @brief b	The component class to compare to
		* @returns true if they are equal
		*/
		bool operator==(const Component & b) const
		{
			return b.getId() == this->getId();
		}

	};


} // End of namespace


#endif
