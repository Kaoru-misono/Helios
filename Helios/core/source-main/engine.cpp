#include "engine.hpp"
#include <iostream>

helios::Helios_Engine::Helios_Engine()
{
}

helios::Helios_Engine::~Helios_Engine()
{
}

void helios::Helios_Engine::start_engine()
{
}

void helios::Helios_Engine::shutdown_engine()
{
}

void helios::Helios_Engine::renderer_tick()
{
	while (true)
		std::cout << "hello helios" << std::endl;
}
