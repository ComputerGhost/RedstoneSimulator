/**
 * @author Nathan Belue
 * @date October 19, 2015
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

#include "..src/Map.h"
#include "../src/components/SolidBlock.h"
#include "../src/components/GlassBlock.h"


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
 * @brief Test basic components
 *
 * In this function, I will just test a couple of really simple components 
 * that I have coded.  I just want to make sure a few things work with them 
 * in general.
 */
void testBasicComponents()
{
	Redstone::Component* compA = new Redstone::SolidBlock();
	Redstone::Component* compB = new Redstone::GlassBlock();

	Redstone::Component* compC = compA->clone();

	outputTest("compA->getId() == ID::SOLID_BLOCK", (1 == 1), 
		(compA->getId() == Redstone::Component::ID::SOLID_BLOCK));
	outputTest("compC->getId() == ID::SOLID_BLOCK", (1 == 1),
		(compC->getId() == Redstone::Component::ID::SOLID_BLOCK));
}


/**
 * @brief Test empty maps
 *
 * In this function, I want to test just a few empty maps to make sure their 
 * non-manipulating code works swell.
 */
void testEmptyMaps()
{
	Redstone::Map::Size size;
	size.width = size.height = size.depth = 10;

	Redstone::Map mapA;
	Redstone::Map mapB(size);
	Redstone::Map mapC(mapB);
	Redstone::Map mapD;
	mapD = mapC;

	Redstone::Map::Coordinates coords;
	coords.x = -10; coords.y = 5; coords.z = 5;

	outputTest("mapA.get({-10, 5, 5})", static_cast<Redstone::Component*>(nullptr),
		mapA.get(coords));
	outputTest("mapC.get({-10, 5, 5})", static_cast<Redstone::Component*>(nullptr),
		mapC.get(coords));

	coords.x = 5;
	outputTest("mapA.get({5, 5, 5})", static_cast<Redstone::Component*>(nullptr),
		mapA.get(coords));
	outputTest("mapD.get({5, 5, 5})", static_cast<Redstone::Component*>(nullptr),
		mapD.get(coords));
}


/**
 * @brief Test adding and removing components
 *
 * I want to add and remove components from a map or two, to make sure that 
 * I can do this with no hickups.
 */
void testManipulatingMaps()
{
	Redstone::Map::Size size;
	size.width = size.height = size.depth = 10;

	Redstone::Map mapA(size);

	Redstone::Map::Coordinates coords;
	coords.x = -10; coords.y = 5; coords.z = 5;

	Redstone::Component * compA = new Redstone::SolidBlock();

	outputTest("mapA.set({-10, 5, 5}, compA->clone());", "n/a", "n/a");
	mapA.set(coords, compA->clone());

	outputTest("mapA.set({5, 5, 5}, compA);", "n/a", "n/a");
	coords.x = 5;
	mapA.set(coords, compA);

	outputTest("mapA.get({5, 5, 5}) == compA", (1 == 1), 
		(mapA.get(coords) == compA));
}


/**
 * @brief Test full maps
 *
 * I want to add and remove components, manipulate them, and do all sorts of 
 * other stuff that the engine and other parts of the program may do.  This 
 * is the final test of the Map class.
 */
void testFullMaps()
{
	Redstone::Map::Size size;
	size.width = size.height = size.depth = 10;

	Redstone::Map mapA(size);
	Redstone::Component* compA = new Redstone::SolidBlock();
	Redstone::Component* compB = new Redstone::GlassBlock();

	outputTest("Propogate mapA", "n/a", "n/a");
	Redstone::Map::Coordinates coords;
	for (coords.x = 0; coords.x != 10; ++coords.x) {
		for (coords.y = 0; coords.y != 10; ++coords.y) {
			for (coords.z = 0; coords.z != 10; ++coords.z) {
				if (coords.z % 2)
					mapA.set(coords, compA->clone());
				else
					mapA.set(coords, compB->clone());
			}
		}
	}

	Redstone::Map mapB(mapA);
	coords.x = coords.y = coords.z = 5;

	outputTest("mapB.set({5, 5, 5}, compB->clone())", "n/a", "n/a");
	mapB.set(coords, compB->clone());

	outputTest("mapB.get({5, 5, 5})->getId() == ID::GLASS_BLOCK", (1 == 1),
		(mapB.get(coords)->getId() == Redstone::Component::ID::GLASS_BLOCK));

	delete compA;
	delete compB;
}


/**
 * @brief Main function
 */
int main()
{
	// Start
	std::cout << "== test of Redstone::Map and basic Components ==" << std::endl << std::endl;

	std::cout << "--Testing basic components..." << std::endl << std::endl;
	testBasicComponents();

	std::cout << "--Testing empty maps..." << std::endl << std::endl;
	testEmptyMaps();

	std::cout << "--Testing adding and removing components..." << std::endl << std::endl;
	testManipulatingMaps();

	std::cout << "--Testing full maps..." << std::endl << std::endl;
	testFullMaps();

	// Done
	std::cout << "== done ==" << std::endl << std::endl;
}
	

