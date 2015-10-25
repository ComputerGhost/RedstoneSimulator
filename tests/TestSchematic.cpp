/**
 * @author Nathan Belue
 * @date October 22, 2015
 *
 * So far, I have a basic redstone simulator.  I now want a way to save and 
 * load my maps.  I have coded a Schematic class for this.  And now I need 
 * to test it!
 *
 */

#include <iostream>
#include <iomanip>

#include "../src/Schematic.h"
#include "../src/Engine.h"
#include "../src/Map.h"
#include "../src/components/Switch.h"


/**
 * @brief Echo a label and a test
 * @param lbl		The label for the test
 * @param expected	The expected value of the test
 * @param value		The test value
 * @tparam T	The type of the value variable
 */
template<typename T>
void outputTest(const char * lbl, const T & expected, const T & value)
{
	std::cout << "    " << lbl << std::endl;
	std::cout << "        expect: " << expected << std::endl;
	std::cout << "        actual: " << value << std::endl;

	if (expected != value)
		std::cout << "    ^******** FAILURE ********^" << std::endl;

	std::cout << std::endl;
}


/**
 * @brief Try loading a schematic file
 * @param inFile	The file name of the file to load
 * @returns A Map of the loaded schematic
 */
Redstone::Map testSchematicLoad(
	const std::string & inFile)
{
	outputTest("Loading map from file with schematic", "n/a", "n/a");

	Redstone::Schematic schematic;
	try {
		schematic.load(inFile.c_str());
	}
	catch (...) {
		std::cout << "*** Schematic load failed!!!" << std::endl;
		std::cout << "*** Using empty map..." << std::endl;
	}

	return schematic.getMap();
}


/**
 * @brief Run the map the schematic gave us
 * @param map	The map to run
 * @returns The map, after running through
 */
Redstone::Map testSchematicRun(
	const Redstone::Map & map)
{
	outputTest("Running map for maximum of 1000 ticks", "n/a", "n/a");

	Redstone::Engine engine;
	engine.setMap(map);

	while (!engine.isStill() && engine.getTickNumber() < 1000)
		engine.run();

	if (engine.getTickNumber() >= 1000)
		std::cout << "*** Tick number exceeded; engine stopped." << std::endl;

	return engine.getMap();
}


/**
 * @brief Try saving the map to a schematic file
 * @param outFile	The file to output to
 * @param map		The map to output
 */
void testSchematicSave(
	const std::string & outFile, 
	const Redstone::Map & map)
{
	outputTest("Saving map to file with schematic", "n/a", "n/a");

	Redstone::Schematic schematic;
	schematic.setMap(map);

	try {
		schematic.save(outFile.c_str());
	}
	catch (...) {
		std::cout << "*** Schematic save failed!" << std::endl;
	}
}


/**
* @brief Main function
*/
int main(
	int argc, 
	const char * argv[])
{
	if (argc < 3) {
		std::cerr << "Needs input and output files parameters!" << std::endl;
		return 1;
	}
	std::string inFile = argv[1];
	std::string outFile = argv[2];

	Redstone::Map map;

	// Start
	std::cout << "== test of Redstone::Schematic ==" << std::endl << std::endl;

	std::cout << "--Testing loading of schematic..." << std::endl << std::endl;
	map = testSchematicLoad( inFile );

	std::cout << "--Flip all of the switches..." << std::endl << std::endl;
	Redstone::Map::Coordinates coords;
	Redstone::Map::Size size = map.size();
	for (coords.y = 0; coords.y != size.y; ++coords.y) {
		for (coords.x = 0; coords.x != size.x; ++coords.x) {
			for (coords.z = 0; coords.z != size.z; ++coords.z) {
				Redstone::Component * comp = map.get(coords);
				if (comp == nullptr)
					continue;
				if (comp->getId() == Redstone::Component::ID::SWITCH)
					dynamic_cast<Redstone::Switch *>(comp)->flip();
			}
		}
	}
	std::cout << "\tDone." << std::endl << std::endl;

	std::cout << "--Testing running of schematic..." << std::endl << std::endl;
	map = testSchematicRun(map);

	std::cout << "--Testing saving of schematic..." << std::endl << std::endl;
	testSchematicSave( outFile, map );

	// Done
	std::cout << "== done ==" << std::endl << std::endl;
}


