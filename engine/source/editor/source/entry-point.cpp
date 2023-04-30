#include "engine.hpp"
#include <memory>

int main(int argc, char** argv)
{
	auto engine = new helios::Helios_Engine();
	engine->renderer_tick();
}