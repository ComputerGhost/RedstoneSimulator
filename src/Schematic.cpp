/**
* @author Nathan Belue
* @date october 22, 2015
*
* In this file you will find the Schematic class, which can save and load
* Map objects from a stream.  It uses .schematic file format.  It supports
* compressed .schematic files.
*
*/

#include "Schematic.h"

#include <cstdio>
#include <gzstream.h>
#include <list>
#include <stdexcept>
#include <vector>

#include "_bits/NBTTag.h"
#include "components/Air.h"
#include "components/GlassBlock.h"
#include "components/RedstoneBlock.h"
#include "components/RedstoneDust.h"
#include "components/RedstoneTorch.h"
#include "components/SolidBlock.h"
#include "components/Switch.h"


/**
 * @brief Save the map to a schematic
 * @param outFile	The file name to save to
 */
void Redstone::Schematic::save(const char * outFile) const
{
	ogzstream out(outFile);

	NBTTag tag;
	std::list<NBTTag> parts;

	// Set up the entities
	tag.setName("Entities");
	tag.setListType(NBTTag::Type::COMPOUND);
	tag.vList(std::vector<NBTTag>());
	parts.push_back(tag);

	// Set up the tile entities
	tag.setName("TileEntities");
	tag.setListType(NBTTag::Type::COMPOUND);
	tag.vList(std::vector<NBTTag>());
	parts.push_back(tag);

	// Set Materials tag
	tag.setName("Materials");
	tag.vString("Alpha");
	parts.push_back(tag);

	// Set up the width, height, and length

	Map::Size size = this->_map.size();

	tag.setName("Width");
	tag.vShort(size.x);
	parts.push_back(tag);

	tag.setName("Height");
	tag.vShort(size.y);
	parts.push_back(tag);

	tag.setName("Length");
	tag.vShort(size.z);
	parts.push_back(tag);

	// Set up the blocks
	std::vector<__int8> blocks = this->_writeBlocks();
	tag.setName("Blocks");
	tag.vByteArray(blocks);
	parts.push_back(tag);

	// Set up the data
	std::vector<__int8> data = this->_writeData();
	tag.setName("Data");
	tag.vByteArray(data);
	parts.push_back(tag);

	// Create the root and save it
	NBTTag root;
	root.setName("Schematic");
	root.vCompound(parts);
	root.write(out);

	// Clean up!
	out.close();
}


/**
 * @brief Load the map from a schematic
 * @note Only supported blocks are loaded
 * @param inFile	The file name to read from
 */
void Redstone::Schematic::load(const char * inFile)
{
	igzstream in(inFile);

	NBTTag tag;
	tag.read(in);
	if (tag.getType() != NBTTag::Type::COMPOUND)
		throw std::runtime_error("Invalid file format!");

	// Read in our data
	const std::list<NBTTag> * root = tag.vCompound();
	const std::vector<__int8> * blocks = nullptr;
	const std::vector<__int8> * data = nullptr;
	int width = 0, height = 0, length = 0;
	for (auto t = root->cbegin(); t != root->cend(); ++t) {
	//for (auto t : root) {
		if (t->getName() == "Width")
			width = t->vShort();
		else if (t->getName() == "Height")
			height = t->vShort();
		else if (t->getName() == "Length")
			length = t->vShort();
		else if (t->getName() == "Blocks")
			blocks = t->vByteArray();
		else if (t->getName() == "Data")
			data = t->vByteArray();
	}

	// Set up our map
	this->_map = Map(Map::Size(width, height, length));

	// Load it into the map
	this->_readBlocks(blocks);
	this->_readData(data);

	// Clean up!
	in.close();
}

/**
* @brief Read in the blocks from the byte array
* @param blocks	The blocks to parse
*/
void Redstone::Schematic::_readBlocks(
	const std::vector<__int8> * blocks)
{
	// Read in the block IDs
	auto i = blocks->cbegin();
	Map::Coordinates coords;
	Map::Size size = this->_map.size();
	for (coords.y = 0; coords.y != size.y; ++coords.y) {
		for (coords.z = 0; coords.z != size.z; ++coords.z) {
			for (coords.x = 0; coords.x != size.x; ++coords.x, ++i) {
				if (i == blocks->cend())
					return;

				Component * comp = nullptr;

				switch (*i) {

				// Air:
				case 0: comp = new Air(); break;

				// Solids:
				case 1:		// Stone
				case 2:		// Grass
				case 3:		// Dirt
				case 4:		// Cobblestone
				case 5:		// Planks
				case 14:	// Gold ore
				case 15:	// Iron ore
				case 16:	// Coal ore
				case 17:	// Log
				case 21:	// Lapis ore
				case 22:	// Lapis block
				case 24:	// Sandstone
				case 35:	// Wool
				case 41:	// Gold block
				case 42:	// Iron block
				case 43:	// Double stone slab
				case 45:	// Brick block
				case 47:	// Bookshelf
				case 48:	// Mossy cobblestone
				case 56:	// Diamond ore
				case 57:	// Diamond block
				case 58:	// Crafting table
				case 80:	// Snow
				case 82:	// Clay
				case 86:	// Pumpkin
				case 87:	// Netherrack
				case 91:	// Lit pumpkin 
				case 97:	// Monster egg
				case 98:	// Stone brick
				case 99:	// Brown mushroom block
				case 100:	// Red mushroom block
				case 103:	// Melon block
				case 110:	// Mycelium
				case 112:	// Nether brick
				case 125:	// Double wooden slab
				case 129:	// Emerald ore
				case 133:	// Emerald block
				case 153:	// Quartz ore
				case 155:	// Quartz block
				case 159:	// Stained hardened clay
				case 162:	// Log 2
				case 168:	// Prismarine
				case 169:	// Sea lantern
				case 170:	// Hay block
				case 172:	// Hardened clay
				case 173:	// Coal block
				case 174:	// Packed ice
				case 179:	// Red sandstone
				case 181:	// Double stone slab 2
				case 201:	// Purpur block
				case 204:	// Purpur double slab
				case 206:	// End bricks
					comp = new SolidBlock();
					break;

				// Redstone block
				case 152: comp = new RedstoneBlock(); break;
					
				// Glass
				case 20: comp = new GlassBlock(); break;

				// Redstone dust
				case 55: comp = new RedstoneDust(); break;

				// Redstone torch (off)
				case 75: comp = new RedstoneTorch(false); break;

				// Redstone torch (on)
				case 76: comp = new RedstoneTorch(); break;

				// Lever
				case 69: comp = new Switch(); break;

				}

				// Set the map!
				this->_map.set(coords, comp);

			}
		}
	}
}


/**
 * @brief Read in the block data from the byte array
 * @param blocks	The block data to parse
 */
void Redstone::Schematic::_readData(
	const std::vector<__int8> * data)
{
	// Read in the block IDs
	auto i = data->cbegin();
	Map::Coordinates coords;
	Map::Size size = this->_map.size();
	for (coords.y = 0; coords.y != size.y; ++coords.y) {
		for (coords.z = 0; coords.z != size.z; ++coords.z) {
			for (coords.x = 0; coords.x != size.x; ++coords.x, ++i) {
				auto comp = this->_map.get(coords);
				if (comp == nullptr)
					continue;

				switch (comp->getId())
				{

				case Component::ID::REDSTONE_DUST:
					{
						auto dust = dynamic_cast<RedstoneDust *>(comp);
						dust->setLevel(*i);
					}
					break;

				case Component::ID::REDSTONE_TORCH:
					{
						auto torch = dynamic_cast<RedstoneTorch *>(comp);
						switch (*i) {
						case 5:
							torch->setDirection(Map::Direction::DOWN);
							break;
						case 0:
							torch->setDirection(Map::Direction::NORTH);
							break;
						case 4:
							torch->setDirection(Map::Direction::SOUTH);
							break;
						case 1:
							torch->setDirection(Map::Direction::WEST);
							break;
						case 2:
							torch->setDirection(Map::Direction::EAST);
							break;
						default:
							torch->setDirection(Map::Direction::DOWN);
							break;
						}
					}
					break;

				case Component::ID::SWITCH:
					{
						auto toggle = dynamic_cast<Switch *>(comp);
						if (*i & 8)
							toggle->flip();

						switch (*i & 7) {
						case 5: case 6:
							toggle->setDirection(Map::Direction::UP);
							break;
						case 0: case 7:
							toggle->setDirection(Map::Direction::DOWN);
							break;
						case 4:
							toggle->setDirection(Map::Direction::SOUTH);
							break;
						case 3:
							toggle->setDirection(Map::Direction::NORTH);
							break;
						case 2:
							toggle->setDirection(Map::Direction::WEST);
							break;
						case 1:
							toggle->setDirection(Map::Direction::EAST);
							break;
						}
					}
					break;

				}

			}
		}
	}
}


/**
 * @brief Write out the blocks to the byte array
 * @returns A byte array of block ids
 */
std::vector<__int8> Redstone::Schematic::_writeBlocks() const
{
	Map::Size size = this->_map.size();
	std::vector<__int8> blocks;
	blocks.reserve(size.x * size.y * size.z);

	// Read in the block IDs
	Map::Coordinates coords;
	for (coords.y = 0; coords.y != size.y; ++coords.y) {
		for (coords.z = 0; coords.z != size.z; ++coords.z) {
			for (coords.x = 0; coords.x != size.x; ++coords.x) {
				auto comp = this->_map.get(coords);
				if (comp == nullptr) {
					blocks.push_back(0);
					continue;
				}

				switch (comp->getId())
				{

				case Component::ID::AIR:
					blocks.push_back(0);
					break;

				case Component::ID::GLASS_BLOCK:
					blocks.push_back(20);
					break;

				case Component::ID::REDSTONE_BLOCK:
					blocks.push_back(152u);
					break;

				case Component::ID::REDSTONE_DUST:
					blocks.push_back(55);
					break;

				case Component::ID::REDSTONE_TORCH:
					{
						auto torch = dynamic_cast<const RedstoneTorch *>(comp);
						blocks.push_back(torch->isOn() ? 76 : 75);
					}
					break;

				case Component::ID::SOLID_BLOCK:
					blocks.push_back(35);
					break;

				case Component::ID::SWITCH:
					blocks.push_back(69);
					break;

				default:
					blocks.push_back(0);
					break;

				}

			}
		}
	}

	return blocks;
}


/**
 * @brief Write out the block data to the byte array
 * @returns A byte array of block data
 */
std::vector<__int8> Redstone::Schematic::_writeData() const
{
	Map::Size size = this->_map.size();
	std::vector<__int8> data;
	data.reserve(size.x * size.y * size.z);

	// Read in the block IDs
	Map::Coordinates coords;
	for (coords.y = 0; coords.y != size.y; ++coords.y) {
		for (coords.z = 0; coords.z != size.z; ++coords.z) {
			for (coords.x = 0; coords.x != size.x; ++coords.x) {
				auto comp = this->_map.get(coords);
				if (comp == nullptr) {
					data.push_back(0);
					continue;
				}

				switch (comp->getId())
				{

				case Component::ID::REDSTONE_DUST:
					{
						auto dust = dynamic_cast<const RedstoneDust *>(comp);
						data.push_back(dust->getLevel());
					}
					break;

				case Component::ID::REDSTONE_TORCH:
					{
						auto torch = dynamic_cast<const RedstoneTorch *>(comp);
						switch (torch->getDirection()) {
						case Map::Direction::DOWN:
							data.push_back(5);
							break;
						case Map::Direction::NORTH:
							data.push_back(3);
							break;
						case Map::Direction::SOUTH:
							data.push_back(4);
							break;
						case Map::Direction::WEST:
							data.push_back(1);
							break;
						case Map::Direction::EAST:
							data.push_back(2);
							break;
						default:
							data.push_back(5);
							break;
						}
					}
					break;

				case Component::ID::SOLID_BLOCK:
					data.push_back(11);
					break;

				case Component::ID::SWITCH:
					{
						__int8 val = 0;
						auto toggle = dynamic_cast<const Switch *>(comp);
						switch (toggle->getDirection()) {
						case Map::Direction::UP: val = 5; break;
						case Map::Direction::DOWN: val = 0; break;
						case Map::Direction::SOUTH: val = 4; break;
						case Map::Direction::NORTH: val = 3; break;
						case Map::Direction::WEST: val = 2; break;
						case Map::Direction::EAST: val = 1; break;
						}
						val |= toggle->isOn() ? 8 : 0;
						data.push_back(val);
					}
					break;

				default:
					data.push_back(0);
					break;

				}

			}
		}
	}

	return data;
}

