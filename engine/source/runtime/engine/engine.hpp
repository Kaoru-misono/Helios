#pragma once

namespace helios
{
	class Helios_Engine
	{
	public:
		Helios_Engine();
		virtual ~Helios_Engine();

		void start_engine();
		void shutdown_engine();

		void renderer_tick();
	};

	// defined in CLIENT
	Helios_Engine* create_engine_instance();
}