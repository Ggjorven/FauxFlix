#include "ffpch.h"
#include "Renderer.hpp"

#include "Faux/Core/Core.hpp"
#include "Faux/Core/Logger.hpp"

namespace Faux
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
	Renderer::Renderer(RenderContext& context)
		: m_Context(context)
		, m_Renderpass(context.GetDevice(), Obsidian::RenderpassSpecification()
            .SetBindpoint(Obsidian::PipelineBindpoint::Graphics)

            .SetColourImageSpecification(context.GetSwapchain().GetImage(0).GetSpecification())
            .SetColourLoadOperation(Obsidian::LoadOperation::Clear)
            .SetColourStoreOperation(Obsidian::StoreOperation::Store)
            .SetColourStartState(Obsidian::ResourceState::Present)
            .SetColourRenderingState(Obsidian::ResourceState::RenderTarget)
            .SetColourEndState(Obsidian::ResourceState::RenderTarget)

            .SetDebugName("MainPass")
        )
	{
		// Create CommandLists for each frame
        for (uint8_t i = 0; i < m_CommandLists.size(); i++)
            m_CommandLists[i].Construct(context.GetGraphicsPool(i), Obsidian::CommandListSpecification()
                .SetDebugName(std::format("CommandList({0}) for MainPass", i))
            );

        // Create framebuffers for renderpass
        for (uint8_t i = 0; i < context.GetSwapchain().GetImageCount(); i++)
            m_Renderpass.CreateFramebuffer(Obsidian::FramebufferSpecification()
                .SetColourAttachment(Obsidian::FramebufferAttachment()
                    .SetImage(context.GetSwapchain().GetImage(0))
                )
            );
	}

	Renderer::~Renderer() 
	{

	}
}
