/**
* @author Nathan Belue
* @date October 18, 2015
*
* Source code for the map class
*
*/

#include "Component.h"
#include "Map.h"


/**
 * @brief Constructor
 * @param size	The size of the map
 */
Redstone::Map::Map(
	const Redstone::Map::Size & size)
{
	size_t volume = size.x * size.y * size.z;
	this->_size = size;

	if (volume) {
		this->_map = new Component*[volume];
		memset(this->_map, 0, sizeof(Component) * volume);
	}
	else
		this->_map = nullptr;
}


/**
 * @brief Copy constructor
 * @param src	The other map to copy from
 */
Redstone::Map::Map(
	const Redstone::Map & src)
{
	this->_copy(src);
}


/**
 * @brief We'll need a destructor
 */
Redstone::Map::~Map()
{
	this->_cleanup();
}


/**
 * @brief Get the component at a certain location
 * @param coords	The coordinates to look at
 * @returns A reference to the component there
 */
Redstone::Component * Redstone::Map::get(
	const Redstone::Map::Coordinates & coords )
{
	if (this->_map == nullptr)
		return nullptr;
	if (coords.x < 0 || coords.y < 0 || coords.z < 0
		|| coords.x >= static_cast<int>(this->_size.x)
		|| coords.y >= static_cast<int>(this->_size.y)
		|| coords.z >= static_cast<int>(this->_size.z))
		return nullptr;

	size_t offset = (coords.z * this->_size.y + coords.y) * this->_size.x + coords.x;
	return this->_map[offset];
};


/**
 * @brief Get the component at a certain location
 * @param coords	The coordinates to look at
 * @returns A reference to the component there
 */
const Redstone::Component * Redstone::Map::get(
	const Redstone::Map::Coordinates & coords) const
{
	if (this->_map == nullptr)
		return nullptr;
	if (coords.x < 0 || coords.y < 0 || coords.z < 0
		|| coords.x >= static_cast<int>(this->_size.x)
		|| coords.y >= static_cast<int>(this->_size.y)
		|| coords.z >= static_cast<int>(this->_size.z))
		return nullptr;

	size_t offset = (coords.z * this->_size.y + coords.y) * this->_size.x + coords.x;
	return this->_map[offset];
};


/**
 * @brief Set the component at a certain location
 * @warning Will delete component if invalid coords
 * @param coords	The coordinates to put it at
 * @param component	The component to put there
 */
void Redstone::Map::set(
	const Redstone::Map::Coordinates & coords,
	Redstone::Component * component)
{
	if (this->_map == nullptr) {
		delete component;
		return;
	}
	if (coords.x < 0 || coords.y < 0 || coords.z < 0
		|| coords.x >= static_cast<int>(this->_size.x)
		|| coords.y >= static_cast<int>(this->_size.y)
		|| coords.z >= static_cast<int>(this->_size.z))
		return;

	size_t offset = (coords.z * this->_size.y + coords.y) * this->_size.x + coords.x;
	delete this->_map[offset];
	this->_map[offset] = component;
}


/**
 * @brief Assignment operator
 * @param src	The map to copy from
 */
Redstone::Map & Redstone::Map::operator =(
	const Redstone::Map & src)
{
	this->_cleanup();
	this->_copy(src);
	return *this;
}


/* Helper functions */


/**
 * @brief Clean up the memory
 */
void Redstone::Map::_cleanup()
{
	size_t volume = this->_size.x * this->_size.y * this->_size.z;
	for (auto v = this->_map; v != this->_map + volume; ++v)
		delete *v;
	delete[] this->_map;
	this->_map = nullptr;	// redundancy, just in case
}


/**
 * @brief Copy another Map over
 * @warning Assumes _map is nullptr
 */
void Redstone::Map::_copy(
	const Redstone::Map & src)
{
	size_t volume = src._size.x * src._size.y * src._size.z;
	this->_size = src._size;
	if (volume) {
		this->_map = new Component*[volume];
		for (auto d = this->_map, s = src._map; d != this->_map + volume; ++d, ++s)
			*d = (*s)? (*s)->clone() : nullptr;
	}
	else
		this->_map = nullptr;
}

