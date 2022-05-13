//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_RENDER_H
#define DAYZ_EXTERNAL_CPP_RENDER_H

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <cstdint>
#include <dwmapi.h>

#include "math.h"
#include "imgui/imgui.h"

/*
 * Window related functionality
 */
namespace window {

    extern HWND overlay_handle;
    extern int overlay_width;
    extern int overlay_height;

    /**
     * @brief
     */
    static BOOL create(HWND& window_handle, WNDCLASSEX& window_class, const char* window_name, int posX, int posY, int width, int height, DWORD extended_window_style = 0, DWORD window_style = 0);

    /**
     * @brief
     */
    vector2 get_center();

    /**
     * @brief
     */
    BOOL get_size(HWND window_handle, int& height, int& width);
}

namespace render {

    /**
     * @brief
     */
    void text(const ImVec2& pos, const ImColor& color, const std::string& text);

    /**
     * @brief
     */
    void text_stroke(const ImVec2& pos, const ImColor& color, const std::string& text);

    /**
     * @brief
     */
    void line(const ImVec2& from, const ImVec2& to, const ImColor& color);

    /**
     * @brief
     */
    void line_stroke(const ImVec2 from, const ImVec2 to, const ImColor color);

    /**
     * @brief
     */
    void rect(const ImVec2 pos, const ImVec2 length, const ImColor color);

    /**
     * @brief
     */
    void circle(const ImVec2& pos, const float& radius, const ImColor& color);
}

/*
 * Handling DirectX devices
 */
namespace render::backend {

        /**
         * @brief
         */
        extern IDXGISwapChain* swapchain;

        /**
         * @brief
         */
        extern ID3D11Device* d3d11_device;

        /**
         * @brief
         */
        extern ID3D11DeviceContext* d3d11_device_context;

        /**
         * @brief
         */
        extern ID3D11RenderTargetView* d3d11_render_target;

        /**
         * @brief
         */
        static BOOL d3d11_create_device(HWND window_handle, IDXGISwapChain*& swapchain, ID3D11Device*& device, ID3D11DeviceContext*& device_context);

        /**
         * @brief
         */
        static void d3d11_release_device(ID3D11Device* device);

        /**
         * @brief
         */
        static BOOL d3d11_create_render_target(IDXGISwapChain* swapchain, ID3D11Device* device, ID3D11RenderTargetView*& render_target);

        /**
         * @brief
         */
        static void d3d11_release_render_target(ID3D11RenderTargetView* render_target);

        /**
         * @brief
         */
        static void d3d11_release_device_context(ID3D11DeviceContext* device_context);

        /**
         * @brief
         */
        static void dxgi_release_swapchain(IDXGISwapChain* swapchain);

        /**
         * @brief
         */
        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        /**
         * @brief
         */
        BOOL init();

        /**
         * @brief
         */
        void terminate();
}

#endif //DAYZ_EXTERNAL_CPP_RENDER_H
