#include "ffpch.h"
#include "Window.hpp"

#include "Faux/Core/Core.hpp"
#include "Faux/Core/Logger.hpp"

namespace Faux
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    Window::Window(const WindowSpecification& specs)
        : m_Specification(specs)
        , m_Window(Obsidian::WindowSpecification()
            .SetTitle(specs.Title)
            .SetWidthAndHeight(specs.Width, specs.Height)
            .SetFlags(specs.Flags)
            .SetEventCallback([this](Obsidian::Event e) { OnEvent(e); })
        )
        , m_RenderContext(m_Window, specs.VSync)
    {
        FF_ASSERT(m_Specification.EventCallback, "Must have an event callback.");
    }

    Window::~Window()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Window::SwapBuffers()
    {
        m_Window.SwapBuffers();
        //m_RenderContext.End();
    }

    void Window::PollEvents()
    {
        m_Window.PollEvents();
        //m_RenderContext.Begin();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void Window::OnEvent(Obsidian::Event e)
    {
        Nano::Events::EventHandler handler(e);

        handler.Handle<Obsidian::WindowCloseEvent>([&](const Obsidian::WindowCloseEvent& wce) { (void)wce; m_Window.Close();  });
        handler.Handle<Obsidian::WindowResizeEvent>([&](const Obsidian::WindowResizeEvent& wre) { m_RenderContext.Resize(wre.GetWidth(), wre.GetHeight()); });

        m_Specification.EventCallback(e);
    }

}
