#pragma once

#include "Faux/Core/Core.hpp"

#include "Faux/Renderer/RenderContext.hpp"

#include <cstdint>

namespace Faux
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Renderer
    ////////////////////////////////////////////////////////////////////////////////////
    class Renderer
    {
    public:
        // Constructor & Destructor
        Renderer(RenderContext& context);
        ~Renderer();

        // Methods
	
		// Getters

	private:
		RenderContext& m_Context;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandList>, Obsidian::Information::FramesInFlight> m_CommandLists = { };
        Obsidian::Renderpass m_Renderpass;
    };

}

