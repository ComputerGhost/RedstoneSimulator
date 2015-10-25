/** @file
 * @author Nathan Belue
 * @date october 22, 2015
 *
 * In this file you will find the Schematic class, which can save and load 
 * Map objects from a stream.  It uses .schematic file format.  It supports 
 * compressed .schematic files.
 *
 */

#ifndef REDSTONE_SCHEMATIC_H
#define REDSTONE_SCHEMATIC_H

#include "Map.h"
#include <vector>
#include <gzstream.h>


/* Redstone namespace */
namespace Redstone
{


	/**
	 * @brief Schematic class that saves and loads maps
	 */
	class Schematic
	{

	public:

		/**
		 * @brief Set the map we use
		 * @param map	The map the schematic will contain
		 */
		void setMap(const Map & map)
		{
			this->_map = map;
		}

		/**
		 * @brief Get the map we use
		 * @returns The map the schematic contains
		 */
		const Map & getMap() const
		{
			return this->_map;
		}

		/**
		 * @brief Save the map to a schematic
		 * @param outFile	The name of the output file
		 */
		void save(const char * outFile) const;

		/**
		 * @brief Load the map from a schematic
		 * @note Only supported blocks are loaded
		 * @param inFile	The name of the input file
		 */
		void load(const char * inFile);


	private:


		/* Helper functions */

		/**
		 * @brief Read in the blocks from the byte array
		 * @param blocks	The blocks to parse
		 */
		void _readBlocks(const std::vector<__int8> * blocks);

		/**
		 * @brief Read in the block data from the byte array
		 * @param blocks	The block data to parse
		 */
		void _readData(const std::vector<__int8> * data);

		/**
		 * @brief Write out the blocks to the byte array
		 * @returns A byte array of block ids
		 */
		std::vector<__int8> _writeBlocks() const;

		/**
		 * @brief Write out the block data to the byte array
		 * @returns A byte array of block data
		 */
		std::vector<__int8> _writeData() const;


	private:

		/* data */
		Map _map;

	};


}


#endif
