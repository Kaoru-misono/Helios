#include "source-main/engine.hpp"
#include "runtime/entry-point.hpp"


class Sandbox : public helios::Helios_Engine
{
public:
	Sandbox()
		:Helios_Engine()
	{

	}
	~Sandbox() = default;
};


helios::Helios_Engine* create_engine_instance()
{
	return new Sandbox();
}
