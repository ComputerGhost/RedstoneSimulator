/** @file
* @author Nathan Belue
* @date October 19, 2015
*
* This file contains the redstone dust component class definition.  It 
* inherits from Component and represents Minecraft redstone dust.
*
*/

#ifndef REDSTONE_COMPONENTS_REDSTONEDUST_H
#define REDSTONE_COMPONENTS_REDSTONEDUST_H

#include "../Map.h"
#include "../Component.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	* @brief Redstone dust class
	*
	* This class is a component that represents Minecraft redstone dust.
	*
	*/
	class RedstoneDust : public Component
	{

	public:


		/* Functions */

		RedstoneDust() = default;
		RedstoneDust(const RedstoneDust &) = default;
		RedstoneDust & operator = (const RedstoneDust &) = delete;

		/**
		* @brief Need this to clone the component
		* @returns Simply a clone of the class
		*/
		RedstoneDust * clone() const
		{
			return new RedstoneDust(*this);
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
			return Component::ID::REDSTONE_DUST;
		}

		/**
		* @brief Comparison operator
		* @brief b	The component class to compare to
		* @returns true if they are equal
		*/
		bool operator==(const Component & b) const;


		/**
		 * @brief Set the power level
		 * @note For initialization only, please.
		 * @param level	The power level
		 */
		void setLevel(int level)
		{
			this->_level = level;
		}


		/**
		 * @brief Get the power level
		 * @returns The level, between 0 (none) and 15, inclusively
		 */
		int getLevel() const
		{
			return this->_level;
		}


		/**
		 * @brief Get whether the redstone points in some direction
		 * @param direction	The direction to test
		 * @returns true if it points in the given direction
		 */
		bool hasDirection(const Map::Direction & direction) const;


	private:

		/* Helper functions */

		/**
		* @brief Check to see if a surrounding block will change our state
		* @param component	The component next to us
		* @param direction	The direction towards us
		* @param coords		Our coordinates
		*/
		void _processNeighbor(
			const Component * component, 
			const Map::Direction & direction, 
			const Redstone::Map::Coordinates & coords );

		/**
		 * @brief Check to see if surrounding blocks will change our state
		 * @param map		The map being used
		 * @param coords	The current coordinates
		 */
		void _processNeighbors(Map & map, const Map::Coordinates & coords);

		/**
		 * @brief Process diagonals, if they are redstone
		 * @param map		The map being used
		 * @param coords	The current coordinates
		 */
		void _processDiagonals(Map & map, const Map::Coordinates & coords);

		/**
		 * @brief Updated surrounding diagonals
		 * @param engine	The engine being used
		 * @param coords	The coordinates at the center
		 */
		void _updateDiagonals(Engine & engine, const Map::Coordinates & coords);

		/**
		 * @brief Make us point from a power source
		 * @param direction	Direction from source to us
		 */
		void _attachDirection(const Map::Direction & direction );


	private:

		/* Helper functions */

		/**
		 * @brief Handles the case where there's a solid above us
		 * @param component	The component above us
		 * @param coords	The coordinates of the solid
		 */
		void _onSolidAbove(const Component * component, const Map::Coordinates & coords);

		/**
		 * @brief Handles the case where there's a solid below us
		 * @param component	The component below us
		 * @param coords	The coordinates of the solid
		 */
		void _onSolidBelow(const Component * component, const Map::Coordinates & coords);

		/**
		* @brief Handles the case where there's a solid beside us
		* @param component	The component beside us
		* @param coords		The coordinates of the solid
		* @param direction	Direction to us
		*/
		void _onSolidBeside(
			const Component * component,
			const Map::Coordinates & coords,
			const Map::Direction & direction);

		/**
		 * @brief Handles the case where there's redstone dust beside us
		 * @param component	The component beside us
		 * @param coords	The coordinates of the dust
		 * @param direction	Direction to us
		 */
		void _onRedstoneDustBeside(
			const Component * component,
			const Map::Coordinates & coords,
			const Map::Direction & direction);

		/**
		 * @brief Handles the case where there's a redstone block beside us
		 * @param direction	Direction to us
		 */
		void _onRedstoneBlockBeside(const Map::Direction & direction);

		/**
		 * @brief Handles the case where there's a switch beside us
		 * @param component	The component beside us
		 * @param direction	Direction to us
		 */
		void _onSwitchBeside(
			const Component * component,
			const Map::Direction & direction);

		/**
		* @brief Handles the case where there's a torch beside us
		* @param component	The component beside us
		* @param direction	Direction to us
		*/
		void _onTorchBeside(
			const Component * component,
			const Map::Direction & direction);


	private:

		/* data */

		int _level = 0;
		int _direction = 0xF; // bits {north, east, south, west}

		bool _diagonals[8]; // Up (N, E, S, W), Down (N, E, S, W)

	};


} // End of namespace


#endif
