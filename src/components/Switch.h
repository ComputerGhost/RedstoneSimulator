/** @file
* @author Nathan Belue
* @date October 22, 2015
*
* This file contains the redstone switch class definition.  It inherits from
* the Component class and represents any Minecraft Redstone switch.
*
*/

#ifndef REDSTONE_COMPONENTS_SWITCH_H
#define REDSTONE_COMPONENTS_SWITCH_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Switch class
	*
	* This class is a component that represents a Minecraft switch.
	*
	*/
	class Switch : public Component
	{

	public:


		/* Functions */

		Switch() = default;
		Switch(const Switch &) = default;
		Switch & operator =(const Switch &) = delete;

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		Switch * clone() const
		{
			return new Switch(*this);
		}

		/**
		* @brief Update the component
		* @brief engine	The engine being used
		* @brief coords	The coordinates of the component in the map
		*/
		void update(Engine & engine, const Map::Coordinates & coords);

		/**
		* @brief Get the ID, which is easier to test than the class name.
		* @returns The ID of the component
		*/
		virtual Component::ID getId() const
		{
			return Component::ID::SWITCH;
		}

		/**
		* @brief Comparison operator
		* @brief b	The component class to compare to
		* @returns true if they are equal
		*/
		bool operator==(const Component & b) const;

		/**
		* @brief Set the switch direction
		* @param dir	The direction towards the block to which it's attached
		*/
		void setDirection(const Map::Direction & dir)
		{
			this->_direction = dir;
		}

		/**
		* @brief Get the direction of the switch
		* @returns The direction towards the block to which it's attached
		*/
		const Map::Direction & getDirection() const
		{
			return this->_direction;
		}

		/**
		 * @brief Flip the switch
		 */
		void flip();

		/**
		* @brief Get whether it is turned on
		* @returns true if it is turned on
		*/
		bool isOn() const
		{
			return this->_isOn;
		}


	private:

		/* data */

		bool _isOn = false;
		Map::Direction _direction = Map::Direction::DOWN;

		Engine * _engine = nullptr;
		Map::Coordinates _coords;

	};


} // End of namespace


#endif
