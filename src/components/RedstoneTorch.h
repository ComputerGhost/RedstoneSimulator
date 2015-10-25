/** @file
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

#ifndef REDSTONE_COMPONENTS_REDSTONETORCH_H
#define REDSTONE_COMPONENTS_REDSTONETORCH_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Redstone torch class
	*
	* This class is a component that represents a Minecraft Redstone torch.
	*
	*/
	class RedstoneTorch : public Component
	{

	public:


		/* Functions */

		RedstoneTorch(const RedstoneTorch &) = default;
		RedstoneTorch & operator =(const RedstoneTorch &) = delete;

		/**
		 * @brief Default constructor
		 * @param init	The initial state of the torch (true for on)
		 */
		RedstoneTorch( bool init = true ) :
			_isOn(init), _direction(Map::Direction::DOWN), _oldTickNumber(-1)
		{
			this->_prevStates[0] = this->_prevStates[1] = this->_prevStates[2] = init;
		}

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		RedstoneTorch * clone() const
		{
			return new RedstoneTorch(*this);
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
			return Component::ID::REDSTONE_TORCH;
		}

		/**
		* @brief Comparison operator
		* @brief b	The component class to compare to
		* @returns true if they are equal
		*/
		bool operator==(const Component & b) const;

		/**
		 * @brief Set the torch direction
		 * @param dir	The direction towards the block to which it's attached
		 */
		void setDirection(const Map::Direction & dir)
		{
			this->_direction = dir;
		}

		/**
		 * @brief Get the direction of the torch
		 * @returns The direction towards the block to which it's attached
		 */
		const Map::Direction & getDirection() const
		{
			return this->_direction;
		}

		/**
		* @brief Get whether it is turned on
		* @returns true if it is turned on
		*/
		bool isOn() const
		{
			return this->_isOn;
		}


	private:

		/* Helper functions */

		/**
		 * @brief Check to see if a surrounding block will change our state
		 * @param component	The component next to us
		 * @param direction	The direction towards us
		 * @param coords	Coordinates of component
		 */
		void _processNeighbor(
			const Component * component,
			const Map::Direction & direction,
			const Map::Coordinates & coords);


	private:

		/* data */

		bool _isOn;
		bool _prevStates[3];
		Map::Direction _direction;

		int _oldTickNumber;

	};


} // End of namespace


#endif
