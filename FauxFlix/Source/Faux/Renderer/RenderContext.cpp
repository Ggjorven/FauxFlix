#include "ffpch.h"
#include "RenderContext.hpp"

#include "Faux/Core/Core.hpp"
#include "Faux/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>

namespace Faux
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    RenderContext::RenderContext(Obsidian::Window& window, bool vsync)
        : m_Window(window)
        , m_Device(Obsidian::DeviceSpecification()
            .SetNativeWindow(m_Window.GetNativeWindow())
            .SetDestroyCallback([this](Obsidian::DeviceDestroyFn fn) { DestroyCallback(fn); })
            .SetMessageCallback([this](Obsidian::DeviceMessageType type, const std::string& message) { MessageCallback(type, message); })
        )
        , m_Swapchain(m_Device, Obsidian::SwapchainSpecification()
            .SetWindow(m_Window)
            .SetFormat(Obsidian::Format::BGRA8Unorm)
            .SetColourSpace(Obsidian::ColourSpace::SRGB)
            .SetVSync(vsync)
            .SetDebugName("SwapChain")
        )
    {
        // Create CommandListPools & ComposeLists for each frame
        for (uint8_t i = 0; i < m_GraphicsListPools.size(); i++)
        {
            m_GraphicsListPools[i].Construct(m_Swapchain, Obsidian::CommandListPoolSpecification()
                .SetQueue(Obsidian::CommandQueue::Graphics)
                .SetDebugName(std::format("CommandPool({0}) for Graphics CommandLists", i))
            );
        }
    }

    RenderContext::~RenderContext()
    {
        m_Device.Wait();

        for (uint8_t i = 0; i < m_GraphicsListPools.size(); i++)
            m_Swapchain.FreePool(m_GraphicsListPools[i]);

        m_Device.DestroySwapchain(m_Swapchain);

        DestroyQueue();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void RenderContext::Begin()
    {
        DestroyQueue();

        m_Swapchain.AcquireNextImage();
        m_GraphicsListPools[GetCurrentFrame()]->Reset();
    }

    void RenderContext::End()
    {
        m_Swapchain.Present();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Other
    ////////////////////////////////////////////////////////////////////////////////////
    void RenderContext::Resize(uint32_t width, uint32_t height)
    {
        m_Swapchain.Resize(width, height);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    void RenderContext::DestroyQueue()
    {
        while (!m_DestroyQueue.empty())
        {
            m_DestroyQueue.front()();
            m_DestroyQueue.pop();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Callbacks
    ////////////////////////////////////////////////////////////////////////////////////
    void RenderContext::DestroyCallback(Obsidian::DeviceDestroyFn fn)
    {
        m_DestroyQueue.push(fn);
    }

    void RenderContext::MessageCallback(Obsidian::DeviceMessageType type, const std::string& message) const
    {
        switch (type)
        {
        // Note: We don't want to get the Obsidian Trace messages
        case Obsidian::DeviceMessageType::Info:
            Logger::Info("(OBSIDIAN) - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Warn:
            Logger::Warning("(OBSIDIAN) - {0}", message);
            break;
        case Obsidian::DeviceMessageType::Error:
            Logger::Error("(OBSIDIAN) - {0}", message);
            break;

        default:
            break;
        }
    }

}
