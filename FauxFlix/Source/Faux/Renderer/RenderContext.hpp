#pragma once

#include "Faux/Core/Core.hpp"
#include "Faux/Core/Logger.hpp"

#include <Obsidian/Core/Information.hpp>
#include <Obsidian/Core/Window.hpp>

#include <Obsidian/Renderer/Device.hpp>
#include <Obsidian/Renderer/Swapchain.hpp>

#include <cstdint>
#include <vector>

namespace Faux
{

	class Renderer;

    ////////////////////////////////////////////////////////////////////////////////////
    // RenderContext
    ////////////////////////////////////////////////////////////////////////////////////
    class RenderContext
    {
    public:
        // Constructor & Destructor
        RenderContext(Obsidian::Window& window, bool vsync);
        ~RenderContext();

        // Methods
        // Note: These functions are called by the window
        void Begin();
        void End();

        // Other
        void Resize(uint32_t width, uint32_t height);

        // Getters
        inline Obsidian::Window& GetWindow() { return m_Window; } // Note: This is here, because we can't have it in Application because the Project/Scene already requires the window in that initialization
        inline Obsidian::Device& GetDevice() { return m_Device; }
        inline Obsidian::Swapchain& GetSwapchain() { return m_Swapchain; }
        inline Obsidian::CommandListPool& GetGraphicsPool(uint8_t frame) { FF_ASSERT((frame < m_GraphicsListPools.size()), "Frame exceeds pool size."); return m_GraphicsListPools[frame]; }

        inline uint8_t GetCurrentFrame() const { return m_Swapchain.GetCurrentFrame(); }

    private:
        // Private methods
        void DestroyQueue();

        // Callbacks
        void DestroyCallback(Obsidian::DeviceDestroyFn fn);
        void MessageCallback(Obsidian::DeviceMessageType type, const std::string& message) const;

    private:
        Obsidian::Window& m_Window;
        Obsidian::Device m_Device;
        Obsidian::Swapchain m_Swapchain;

        std::array<Nano::Memory::DeferredConstruct<Obsidian::CommandListPool>, Obsidian::Information::FramesInFlight> m_GraphicsListPools = { };

        std::queue<Obsidian::DeviceDestroyFn> m_DestroyQueue = {};

		friend class Renderer;
    };

}
