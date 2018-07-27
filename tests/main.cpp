#include "mesh_lib.hpp"
//#include <vld.h> //Activate for memory leak detection -  VS feature

using namespace mesh_lib;

int main(int argc, const char * argv[]) {

	if (argc == 2)
	{
		std::string input = argv[1];
		display::display_MANDATORY("Reading: " + input);
	}
	else
	{
		throw (error("One argument expected"));
	}

	auto main_mesh = new unstructured_mesh(argv[1]);

	//Output
	display::display_MANDATORY("--------------");
	display::display_MANDATORY("Write Ensight gold file");
	display::display_MANDATORY("--------------");

	ensight_gold_writer ensight_gold(main_mesh);

	display::display_MANDATORY("Done");

	delete main_mesh;

	return 0;
}
