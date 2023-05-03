#include "engine.hpp"
#include <memory>

int main(int argc, char** argv)
{
	auto engine = new Helios::Helios_Engine();
	engine->run();
	
	engine->shutdown();
}