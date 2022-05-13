//
// Created by Max on 06/05/2022.
//
#include "render.h"

#include <cstdint>
#include <dwmapi.h>

#include "imgui/imgui.h"

HWND window::overlay_handle = nullptr;
int window::overlay_width = 0;
int window::overlay_height = 0;

ID3D11Device* render::backend::d3d11_device = nullptr;
ID3D11RenderTargetView* render::backend::d3d11_render_target = nullptr;
ID3D11DeviceContext* render::backend::d3d11_device_context = nullptr;
IDXGISwapChain* render::backend::swapchain = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL window::create(HWND& window_handle, WNDCLASSEX& window_class, const char* window_name, int posX, int posY, int width, int height, DWORD extended_window_style, DWORD window_style) {
    RegisterClassEx(&window_class);

    window_handle = CreateWindowEx
            (
                    extended_window_style,
                    window_class.lpszClassName,
                    window_name,
                    window_style,
                    posX,
                    posY,
                    width,
                    height,
                    nullptr,
                    nullptr,
                    window_class.hInstance,
                    nullptr
            );

    return window_handle != nullptr;
}

BOOL window::get_size(HWND window_handle, int& height, int& width) {
    RECT rect;

    auto result = GetWindowRect(window_handle, &rect);
    height	= rect.bottom - rect.top;
    width	= rect.right  - rect.left;

    return result;
}

vector2 window::get_center() {
    return vector2 {
            .x = window::overlay_width / 2.f,
            .y = window::overlay_height / 2.f
    };
}

void render::text(const ImVec2& pos, const ImColor& color, const std::string& text) {
    ImGui::GetWindowDrawList()->AddText(pos, color, text.c_str());
}

void render::text_stroke(const ImVec2& pos, const ImColor& color, const std::string& text) {
    auto draw_list = ImGui::GetWindowDrawList();

    draw_list->AddText( ImVec2(pos.x + 1.f, pos.y + 1.f), ImColor(0,0,0,255), text.c_str() );
    draw_list->AddText( ImVec2(pos.x + 1.f, pos.y - 1.f), ImColor(0,0,0,255), text.c_str() );
    draw_list->AddText( ImVec2(pos.x - 1.f, pos.y - 1.f), ImColor(0,0,0,255), text.c_str() );
    draw_list->AddText( ImVec2(pos.x - 1.f, pos.y + 1.f), ImColor(0,0,0,255), text.c_str() );

    draw_list->AddText( pos, color, text.c_str() );
}

void render::line(const ImVec2& from, const ImVec2& to, const ImColor& color) {
    ImGui::GetWindowDrawList()->AddLine(from, to, color);
}

void render::line_stroke(const ImVec2 from, const ImVec2 to, const ImColor color) {
    ImGui::GetWindowDrawList()->AddLine(from, to, ImColor(0,0,0,255), 1.5f);
    ImGui::GetWindowDrawList()->AddLine(from, to, color);
}

void render::rect(const ImVec2 pos, const ImVec2 length, const ImColor color) {
    ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + length.x, pos.y + length.y), color, 0, ImDrawFlags_None);
}

void render::circle(const ImVec2& pos, const float& radius, const ImColor& color) {
    ImGui::GetWindowDrawList()->AddCircle(pos, radius, color);
}

BOOL render::backend::d3d11_create_device(const HWND window_handle, IDXGISwapChain*& swapchain, ID3D11Device*& device, ID3D11DeviceContext*& device_context) {
    if(!window_handle) {
        return FALSE;
    }

    DXGI_MODE_DESC md { 0 };

    md.Height					= 0;
    md.Width					= 0;
    md.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
    md.RefreshRate				= DXGI_RATIONAL { 60, 1 };

    DXGI_SWAP_CHAIN_DESC cd { 0 };

    cd.BufferCount					= 1;
    cd.BufferDesc					= md;
    cd.Windowed						= TRUE;
    cd.OutputWindow					= window_handle;
    cd.SampleDesc					= DXGI_SAMPLE_DESC { 1, 0 };
    cd.BufferUsage					= DXGI_USAGE_RENDER_TARGET_OUTPUT;
    cd.SwapEffect					= DXGI_SWAP_EFFECT_DISCARD;
    cd.Flags						= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL feature_level_array[] =
            {
                    D3D_FEATURE_LEVEL_11_1,
                    D3D_FEATURE_LEVEL_11_0,
                    D3D_FEATURE_LEVEL_10_1,
                    D3D_FEATURE_LEVEL_10_0,
            };

    auto result = D3D11CreateDeviceAndSwapChain
            (
                    nullptr,							// DXGI adapter
                    D3D_DRIVER_TYPE_HARDWARE,			// Driver type
                    nullptr,							// Software
                    D3D11_CREATE_DEVICE_BGRA_SUPPORT,	// Flags
                    feature_level_array,				// Feature levels
                    _countof(feature_level_array),		// Feature levels count
                    D3D11_SDK_VERSION,					// SDK version
                    &cd,								// Swapchain descriptor
                    &swapchain,							// Swapchain pointer
                    &device,							// D3D11 device pointer
                    nullptr,							// Feature levels supported
                    &device_context						// D3D11 device context pointer
            );

    if(FAILED(result)) {
        return FALSE;
    }

    return TRUE;
}

void render::backend::d3d11_release_device(ID3D11Device* device) {
    if (device) {
        device->Release();
        device = nullptr;
    }
}

BOOL render::backend::d3d11_create_render_target(IDXGISwapChain* swapchain, ID3D11Device* device, ID3D11RenderTargetView*& render_target) {
    ID3D11Texture2D* back_buffer;

    swapchain->GetBuffer
            (
                    NULL,
                    __uuidof(ID3D11Texture2D),
                    reinterpret_cast<void**>(&back_buffer)
            );

    auto result = device->CreateRenderTargetView(back_buffer, nullptr, &render_target);

    back_buffer->Release();

    return result == S_OK ? TRUE : FALSE;
}

void render::backend::d3d11_release_render_target(ID3D11RenderTargetView* render_target) {
    if (render_target)
    {
        render_target->Release();
        render_target = nullptr;
    }
}

void render::backend::d3d11_release_device_context(ID3D11DeviceContext* device_context) {
    if (device_context)
    {
        device_context->Release();
        device_context = nullptr;
    }
}

void render::backend::dxgi_release_swapchain(IDXGISwapChain* swapchain) {
    if(swapchain)
    {
        swapchain->Release();
        swapchain = nullptr;
    }
}

LRESULT WINAPI render::backend::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg)
    {
        case WM_SIZE:

            if (d3d11_device != nullptr && wParam != SIZE_MINIMIZED)
            {
                d3d11_release_render_target(d3d11_render_target);

                swapchain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_B8G8R8A8_UNORM, 0);

                render::backend::d3d11_create_render_target(swapchain, d3d11_device, d3d11_render_target);
            }

            return true;

        case WM_SYSCOMMAND:

            if ((wParam & 0xFFF0) == SC_KEYMENU)
            {
                return 0;
            }

            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL render::backend::init() {

    static WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = "Windows.UI.Core.CoreWindow";
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.style = CS_VREDRAW | CS_HREDRAW;

    window::create(
            window::overlay_handle,
            wc,
            "Core Input",
            0,
            0,
            1920,
            1080,
            WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
            WS_POPUP);

    if (d3d11_create_device(window::overlay_handle, swapchain, d3d11_device, d3d11_device_context) == FALSE) {
        terminate();
        return FALSE;
    }

    if (d3d11_create_render_target(swapchain, d3d11_device, d3d11_render_target) == FALSE) {
        terminate();
        return FALSE;
    }

    SetLayeredWindowAttributes(window::overlay_handle, RGB(0, 0, 0), 0, ULW_COLORKEY);
    SetLayeredWindowAttributes(window::overlay_handle, 0, 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(window::overlay_handle, &margins);

    SetWindowLongPtrA(window::overlay_handle, GWLP_WNDPROC, reinterpret_cast<int64_t>(WndProc));

    ShowWindow(window::overlay_handle, SW_SHOW);
    UpdateWindow(window::overlay_handle);

    window::get_size(window::overlay_handle, window::overlay_height, window::overlay_width);

    MoveWindow(window::overlay_handle, 0, 0, window::overlay_width, window::overlay_height, FALSE);
    return TRUE;
}

void render::backend::terminate() {
    d3d11_release_render_target(d3d11_render_target);
    d3d11_release_device(d3d11_device);
    dxgi_release_swapchain(swapchain);
    DestroyWindow(window::overlay_handle);
}