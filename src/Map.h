/** @file
* @author Nathan Belue
* @date October 18, 2015
*
* This file contains the map class definition.  It is simply a 
* three-dimensional array of components.
*
*/

#ifndef REDSTONE_MAP_H
#define REDSTONE_MAP_H


#include <cstdlib>


/* Redstone namespace */
namespace Redstone
{


	/* Forward declarations */
	class Component;
	

	/**
	* @brief Map class
	*
	* This class is a convenient holder for components in the world.
	*
	*/
	class Map
	{

	public:

		/* Types */

		/**
		 * @brief Map coordinates
		 */
		struct Coordinates
		{
			int x, y, z;

			Coordinates() : x(0), y(0), z(0) {}
			Coordinates(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
		};

		/**
		 * @brief Map size
		 */
		union Size
		{
			struct { size_t x, y, z; };
			struct { size_t width, height, depth; };

			Size() : x(0), y(0), z(0) {}
			Size(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
		};

		/**
		 * @brief Cardinal directions
		 */
		enum class Direction {
			NORTH,	/** towards -z */
			SOUTH,	/** towards +z */
			EAST,	/** towards +x */
			WEST,	/** towards -x */
			UP,		/** towards +y */
			DOWN	/** towards -y */
		};


		/* Functions */

		/**
		 * @brief Constructor
		 * @param size	The size of the map
		 */
		Map(const Size & size = Size());

		/**
		 * @brief Construct with a freakin' map size!
		 * @param width	The width of the map
		 * @param depth	The depth of the map
		 * @param height	The height of the map
		 */
		Map(int width, int depth, int height) :
			Map(Size(width, depth, height))
		{}

		/**
		 * @brief Copy constructor
		 * @param src	The other map to copy from
		 */
		Map(const Map & src);

		/**
		 * @brief We'll need a destructor
		 */
		~Map();

		/**
		 * @brief Get the size of the map
		 * @returns The size of the map
		 */
		const Size & size() const
		{
			return this->_size;
		}

		/**
		 * @brief Get the component at a certain location
		 * @param coords	The coordinates to look at
		 * @returns A pointer to the component there, or nullptr
		 */
		Component * get(const Coordinates & coords);

		/**
		 * @brief Get the component at a certain location
		 * @param coords	The coordinates to look at
		 * @returns A pointer to the component there, or nullptr
		 */
		const Component * get(const Coordinates & coords) const;

		/**
		 * @brief Set the component at a certain location
		 * @warning Let the class handle deallocation!
		 * @param coords	The coordinates for it to be at
		 * @param component	The component to put there
		 */
		void set(const Coordinates & coords, Component * component);

		/**
		 * @brief Assignment operator
		 * @param src	The map to copy from
		 */
		Map & operator = (const Map & src);


	private:

		/* Helper functions */

		/**
		 * @brief Clean up the memory
		 */
		void _cleanup();

		/**
		 * @brief Copy another Map over
		 * @warning Assumes _map is nullptr
		 */
		void _copy(const Map & src);


	private:

		/* Data */
		Component ** _map = nullptr;
		Size _size;

	};


} // End of namespace


#endif

