/** @file
 * @author Nathan Belue
 * @date October 18, 2015
 *
 * This file contains the component class definition, which is the abstract 
 * class from which redstone components are derived.
 *
 * Also included is an enumeration of all components.  This will function as 
 * their IDs, which can be used elsewhere in if-statements.  This way of 
 * doing it will make for cleaner code than using dynamic_cast tests.
 *
 */

#ifndef REDSTONE_COMPONENT_H
#define REDSTONE_COMPONENT_H

#include "Engine.h"
#include "Map.h"


/* Redstone namespace */
namespace Redstone
{


	/**
	 * @brief Abstract class for a component
	 *
	 * This class provides an interface for its child classes to follow, and it
	 * contains helper functions for them to use.
	 *
	 */
	class Component
	{

	public:

		/* Types */
		enum struct ID;


		/* Functions */

		Component(const Component &) = default;
		Component & operator =(const Component &) = delete;

		/**
		 * @brief Default constructor
		 */
		Component()
		{}

		/**
		 * @brief Destructor
		 */
		virtual ~Component()
		{}

		/**
		 * @brief Need this to clone the component
		 * @returns Simply a copy of the class
		 */
		virtual Component * clone() const = 0;

		/**
		 * @brief Update the component
		 * @param engine	The engine being used
		 * @param coords	The coordinates of the component in the map
		 */
		virtual void update(Engine & engine, const Map::Coordinates & coords) = 0;

		/**
		 * @brief Get the ID, which is easier to test than the class name.
		 * @returns The ID of the component
		 */
		virtual Component::ID getId() const = 0;

		/**
		 * @brief Comparison operator
		 * @param b	The component class to compare to
		 * @returns true if they are equal
		 */
		virtual bool operator==(const Component & b) const = 0;


	protected:

		/**
		 * @brief Set a map location to be updated
		 * @param engine	The engine being used
		 * @param coords	The coordinates to mark to update
		 */
		void markUpdate(Engine & engine, const Map::Coordinates & coords)
		{
			engine.markUpdate(coords);
		}

		/**
		 * @brief Set a map location to be updated on the next game tick
		 * @param engine	The engine being used
		 * @param coords	The coordinates to mark to update
		 */
		void markNextUpdate(Engine & engine, const Map::Coordinates & coords)
		{
			engine.markNextUpdate(coords);
		}

		/**
		 * @brief Update surrounding in NSEWUD direction
		 * @param engine	The engine being used
		 * @param coords	The coordinates' center
		 */
		void updateSurrounding(Engine & engine, const Map::Coordinates & coords);

	};


	/**
	* @brief An enumeration of all components
	*/
	enum struct Component::ID
	{
		AIR,

		/* blocks */
		SOLID_BLOCK,
		HALF_BLOCK, 
		GLASS_BLOCK,
		SAND_BLOCK,
		REDSTONE_BLOCK, 
		SLIME_BLOCK,

		/* basics */
		REDSTONE_DUST,
		REDSTONE_TORCH,

		/* Switches */
		WOODEN_BUTTON,
		STONE_BUTTON,
		SWITCH,
		TRIPWIRE_HOOK,
		WOODEN_PRESSURE_PLATE, 
		STONE_PRESSURE_PLATE, 

		/* Logic */
		REPEATER,
		COMPARATOR,

		/* Containers */
		HOPPER,
		DROPPER,
		DISPENSER,

		/* Mechanical */
		REGULAR_PISTON,
		STICKY_PISTON,

		/* Entities */
		DIAMOND,
		BED,
		CART,

		/* rails */
		RAIL,
		POWERED_RAIL,
		DETECTOR_RAIL,

		/* other */
		TRIPWIRE,

		/* 'cause I'm too lazy to figure out commas */
		END_OF_ENUM
	};


} // End of namespace


#endif

