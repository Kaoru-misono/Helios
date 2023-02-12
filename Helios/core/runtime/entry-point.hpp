#include "source-main/engine.hpp"
#include <memory>


extern helios::Helios_Engine* create_engine_instance();

int main(int argc, char** argv)
{
	auto engine = create_engine_instance();
	engine->renderer_tick();
}