/**
* @author Nathan Belue
* @date October 22, 2015
*
* So far, I have coded the Map class and a couple of components.  I need to
* make sure that it all compiles and runs fine.  This test will run it
* through a few tasks to make sure it's all fine and dandy.
*
* Specifically, I will be manipulating a few Map class objects.
*
*/

#include <iostream>
#include <iomanip>

#include "../src/Engine.h"
#include "../src/Map.h"
#include "../src/components/SolidBlock.h"
#include "../src/components/GlassBlock.h"
#include "../src/components/RedstoneBlock.h"
#include "../src/components/RedstoneDust.h"
#include "../src/components/RedstoneTorch.h"
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
 * @brief Test empty maps in the engine
 *
 * I want to make sure nothing breaks if I try to do this.
 *
 */
void testEmptyMaps()
{
	Redstone::Map map;
	Redstone::Map::Size size;
	Redstone::Engine engine;

	outputTest("engine.run() on unspecified map", "n/a", "n/a");
	engine.run();

	outputTest("engine.run() on default-constructed (empty) map", "n/a", "n/a");
	engine.setMap(map);
	engine.run();

	outputTest("engine.run() on empty 10x10x10 map", "n/a", "n/a");
	size.width = size.height = size.depth = 10;
	engine.setMap(Redstone::Map(size));
	engine.run();
}


/**
 * @brief Test maps that will not update further
 *
 * Basically, before we get into the complicated stuff, does it
 * just...  work right?
 *
 */
void testStillMaps()
{
	int x, z;
	Redstone::Map map(5, 5, 5);

	Redstone::GlassBlock * glass = new Redstone::GlassBlock();
	Redstone::SolidBlock * solid = new Redstone::SolidBlock();
	Redstone::RedstoneBlock * redstoneBlock = new Redstone::RedstoneBlock();
	Redstone::RedstoneDust * dust = new Redstone::RedstoneDust();

	// Construct the map

	for (x = 0; x != 5; ++x) {
		for (z = 0; z != 5; ++z) {
			map.set(Redstone::Map::Coordinates(x, 0, z), solid->clone());
			map.set(Redstone::Map::Coordinates(x, 1, z), dust->clone());
			map.set(Redstone::Map::Coordinates(x, 4, z), glass->clone());
		}
		map.set(Redstone::Map::Coordinates(x, 1, 1), redstoneBlock->clone());
		map.set(Redstone::Map::Coordinates(x, 2, 1), dust->clone());
	}
	map.set(Redstone::Map::Coordinates(2, 1, 2), solid->clone());
	map.set(Redstone::Map::Coordinates(2, 2, 2), dust->clone());

	// Now set it in the engine

	outputTest("engine.setMap(map); with basic \"still\" map", "n/a", "n/a");
	Redstone::Engine engine;
	engine.setMap(map);

	// Test to make sure it's still

	outputTest("engine.isStill()", (1 == 1), engine.isStill());

	// Clean up
	delete glass;
	delete solid;
	delete redstoneBlock;
	delete dust;
}


/**
 * @brief Test an active map (#1)
 *
 * I want to make sure the basic redstone stuff I've done will work right in
 * this engine.  I will put in a simple map that will update, and I will run
 * the engine.
 *
 */
void testActiveMap1()
{
	int x, z;
	Redstone::Map map(5, 5, 5);

	Redstone::GlassBlock * glass = new Redstone::GlassBlock();
	Redstone::SolidBlock * solid = new Redstone::SolidBlock();
	Redstone::RedstoneBlock * redstoneBlock = new Redstone::RedstoneBlock();
	Redstone::RedstoneDust * dust = new Redstone::RedstoneDust();

	// Construct the map
	for (x = 0; x != 5; ++x) {
		for (z = 0; z != 5; ++z) {
			map.set(Redstone::Map::Coordinates(x, 0, z), solid->clone());
			map.set(Redstone::Map::Coordinates(x, 1, z), dust->clone());
			map.set(Redstone::Map::Coordinates(x, 4, z), glass->clone());
		}
		map.set(Redstone::Map::Coordinates(x, 1, 1), redstoneBlock->clone());
		map.set(Redstone::Map::Coordinates(x, 2, 1), dust->clone());
	}
	map.set(Redstone::Map::Coordinates(2, 1, 2), solid->clone());
	map.set(Redstone::Map::Coordinates(2, 2, 2), dust->clone());

	// Now set it in the engine
	outputTest("engine.setMap(map); with simple running map", "n/a", "n/a");
	Redstone::Engine engine;
	engine.setMap(map);

	// Test to make sure it's not still
	outputTest("engine.isStill()", (1 == 0), engine.isStill());

	// Run it 2 ticks
	engine.run();
	engine.run();

	// Test to make sure it's still
	outputTest("engine.isStill()", (1 == 1), engine.isStill());

	// Clean up
	delete glass;
	delete solid;
	delete redstoneBlock;
	delete dust;
}


/**
 * @brief Test an active map (#2)
 *
 * I want to make sure the basic redstone stuff I've done will work right in
 * this engine.  I will put in a simple map that will update, and I will run
 * the engine.
 *
 */
void testActiveMap2()
{
	int x, z;
	Redstone::Map map(3, 4, 3);

	Redstone::GlassBlock * glass = new Redstone::GlassBlock();
	Redstone::SolidBlock * solid = new Redstone::SolidBlock();
	Redstone::RedstoneBlock * redstoneBlock = new Redstone::RedstoneBlock();
	Redstone::RedstoneDust * dust = new Redstone::RedstoneDust();
	Redstone::RedstoneTorch * torch = new Redstone::RedstoneTorch();
	Redstone::Switch * toggle = new Redstone::Switch();
	toggle->setDirection(Redstone::Map::Direction::NORTH);

	// Construct the map (a NAND gate)
	for (x = 0; x != 3; ++x) {
		for (z = 0; z != 3; ++z) {
			map.set(Redstone::Map::Coordinates(x, 0, z), solid->clone());
			map.set(Redstone::Map::Coordinates(x, 3, z), glass->clone());
		}
	}
	map.set(Redstone::Map::Coordinates(0, 1, 0), toggle->clone());
	map.set(Redstone::Map::Coordinates(2, 1, 0), toggle->clone());
	map.set(Redstone::Map::Coordinates(0, 1, 1), solid->clone());
	map.set(Redstone::Map::Coordinates(1, 1, 1), solid->clone());
	map.set(Redstone::Map::Coordinates(2, 1, 1), solid->clone());
	torch->setDirection(Redstone::Map::Direction::DOWN);
	map.set(Redstone::Map::Coordinates(0, 2, 1), torch->clone());
	map.set(Redstone::Map::Coordinates(2, 2, 1), torch->clone());
	map.set(Redstone::Map::Coordinates(1, 2, 1), dust->clone());
	torch->setDirection(Redstone::Map::Direction::SOUTH);
	map.set(Redstone::Map::Coordinates(1, 1, 2), torch->clone());

	// Now set it in the engine
	outputTest("engine.setMap(map); with AND gate map", "n/a", "n/a");
	Redstone::Engine engine;
	engine.setMap(map);

	// Test to make sure it's not still
	outputTest("engine.isStill()", (1 == 0), engine.isStill());

	// Run it for 3 ticks
	outputTest("engine.run() for 3 ticks", "n/a", "n/a");
	engine.run();
	engine.run();
	engine.run();

	// Test to make sure it's still
	outputTest("engine.isStill()", (1 == 1), engine.isStill());

	// Make sure the output torch is off
	auto output = dynamic_cast<const Redstone::RedstoneTorch *>(
		engine.getMap().get(Redstone::Map::Coordinates(1, 1, 2)));
	outputTest("output torch (off)", (1 == 0), output->isOn());


	// Flip the switches
	outputTest("flip switches", "n/a", "n/a");
	dynamic_cast<Redstone::Switch *>(
		engine.getMap().get(Redstone::Map::Coordinates(0, 1, 0))
		)->flip();
	dynamic_cast<Redstone::Switch *>(
		engine.getMap().get(Redstone::Map::Coordinates(2, 1, 0))
		)->flip();

	// Test to make sure it's not still
	outputTest("engine.isStill()", (1 == 0), engine.isStill());

	// Run it for 4 ticks
	outputTest("engine.run() for 4 ticks", "n/a", "n/a");
	engine.run();
	engine.run();
	engine.run();
	engine.run();

	// Test to make sure it's still
	outputTest("engine.isStill()", (1 == 1), engine.isStill());

	// Make sure the output torch is on
	outputTest("output torch (off)", (1 == 1), output->isOn());

	// Clean up
	delete glass;
	delete solid;
	delete redstoneBlock;
	delete dust;
	delete torch;
	delete toggle;
}


/**
* @brief Main function
*/
int main()
{
	// Start
	std::cout << "== test of Redstone::Engine ==" << std::endl << std::endl;

	std::cout << "--Testing empty map..." << std::endl << std::endl;
	testEmptyMaps();

	std::cout << "--Testing \"still\" map..." << std::endl << std::endl;
	testStillMaps();

	std::cout << "--Testing active map 1..." << std::endl << std::endl;
	testActiveMap1();

	std::cout << "--Testing active map 2..." << std::endl << std::endl;
	testActiveMap2();

	// Done
	std::cout << "== done ==" << std::endl << std::endl;
}


