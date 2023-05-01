#include "engine.hpp"
#include <memory>

int main(int argc, char** argv)
{
	auto engine = new Helios::Helios_Engine();
	engine->start_engine();
	engine->renderer_tick();
	engine->shutdown_engine();
}