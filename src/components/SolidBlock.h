/** @file
* @author Nathan Belue
* @date October 19, 2015
*
* This file contains the solid block component class definition.  It 
* inherits from the Component class and represents any solid Minecraft 
* block.
*
*/

#ifndef REDSTONE_COMPONENTS_SOLIDBLOCK_H
#define REDSTONE_COMPONENTS_SOLIDBLOCK_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Solid block class
	*
	* This class is a component that represents a solid Minecraft block.
	*
	*/
	class SolidBlock : public Component
	{

	public:


		/* Functions */

		SolidBlock() = default;
		SolidBlock(const SolidBlock &) = default;
		SolidBlock & operator =(const SolidBlock &) = delete;

		/**
		 * @brief Need this to clone the component
		 * @returns Simply a clone of the class
		 */
		SolidBlock * clone() const
		{
			return new SolidBlock(*this);
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
			return Component::ID::SOLID_BLOCK;
		}

		/**
		 * @brief Comparison operator
		 * @brief b	The component class to compare to
		 * @returns true if they are equal
		 */
		bool operator==(const Component & b) const;

		/**
		 * @brief Get whether it is strongly powered
		 * @returns true if it is strongly powered
		 */
		bool isStronglyPowered() const
		{
			return this->_stronglyPowered;
		}

		/**
		 * @brief Get the power level of the block
		 * @returns 0-15, inclusive, which is the power level
		 */
		int getPowerLevel() const
		{
			if (this->_stronglyPowered)
				return 15;
			return this->_powerLevel;
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

		/**
		 * @brief Handles the case where there's redstone dust above us
		 * @param component	The component beside us
		 */
		void _onRedstoneDustAbove(
			const Component * component);

		/**
		 * @brief Handles the case where there's redstone dust beside us
		 * @param component	The component beside us
		 * @param direction	Direction to us
		 */
		void _onRedstoneDustBeside(
			const Component * component,
			const Map::Direction & direction);

		/**
		 * @brief Handles the case where there's a redstone torch below us
		 * @param component	The component below us
		 */
		void _onRedstoneTorchBelow(
			const Component * component);

		/**
		 * @brief Handles the case where there's a switch beside us
		 * @param component	The component beside us
		 * @param direction	Direction to us
		 */
		void _onSwitchBeside(
			const Component * component,
			const Map::Direction & direction);


	private:

		/* data */

		bool _stronglyPowered = false;
		int _powerLevel = 0;

	};


} // End of namespace


#endif
