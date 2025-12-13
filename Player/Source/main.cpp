#include <iostream>

#include <Faux/Core/Window.hpp>
#include <Faux/Core/Logger.hpp>

#include <Faux/Renderer/RenderContext.hpp>
#include <Faux/Renderer/Renderer.hpp>

using namespace Faux;

int main(const int argc, const char* argv[])
{
	(void)argc; (void)argv;

	Window window(WindowSpecification()
		.SetWidthAndHeight(1280, 720)
		.SetTitle("Test Window")
		.SetFlags(Obsidian::WindowFlags::Default)
		.SetEventCallback([](const Obsidian::Event& e) 
		{
			Logger::Trace("EVENT OOOAA");
		})
	);

	while (window.IsOpen())
	{
		window.GetContext().Begin();

		// ...

		window.GetContext().End();

		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
