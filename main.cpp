#include "memory.h"
#include "offset.h"
#include "world.h"
#include "render.h"
#include "network.h"
#include "config.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace std::chrono;

/*
 * State Context: 
 * - process id
 * - module address
 * - world address (branch out to : localplayer / animal,item,bullet tables)
 * - network client address
 */

/*
 * Globals
 */
world g_world;
network g_network;

/*
 * Debug
 */
int g_frame_counter;
int g_frame_time;

/*
 * Timer
 */
steady_clock::time_point g_start;
steady_clock::time_point g_end;

bool is_target_running() {
    return process::get_process_id("DayZ_x64.exe") != 0;
}

void state_initialize() { // Post death / server reconnection
    g_process_id = process::get_process_id("DayZ_x64.exe");
    g_module_address = process::get_module_address(L"DayZ_x64.exe");
    g_world = { world::get_address() };
    g_network = { memory::read<address_t>(g_module_address + O_NETWORK_MANAGER + O_NETWORK_CLIENT) };
}

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
int main() {
    if(!is_target_running()) { // DayZ process isnt running - terminate
        return -1;
    }
    if(!memory::init()) { // Memory driver failed to initilzie - terminate
        return -1;
    }
    if(!render::backend::init()) { // Rendering devices failed to init - terminate
        return -1;
    }

    state_initialize();

    /**
     * IMGUI Initializaiton
     */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(window::overlay_handle);
    ImGui_ImplDX11_Init(render::backend::d3d11_device, render::backend::d3d11_device_context);
    ImGuiIO& io = ImGui::GetIO();
    static float clear_color[4] = { 0.f, 0.f, 0.f, 0.f };

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while(msg.message != WM_QUIT) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        /**
         * Colors 
         */
        static const auto COLOR_RED = ImColor(255, 0, 0);
        static const auto COLOR_GREEN = ImColor(0, 255, 0);
        static const auto COLOR_BLUE = ImColor(0, 0, 255);
        static const auto COLOR_PURPLE = ImColor(127, 0, 255);
        static const auto COLOR_GOLD = ImColor(255, 215, 0);
        static const auto COLOR_WHEAT = ImColor(245, 222, 179);
        static const auto COLOR_SILVER = ImColor(192, 192, 192);
        static const auto COLOR_MAGENTA = ImColor(255, 0, 255);
        static const auto COLOR_TEAL = ImColor(0, 128, 128);
        static const auto COLOR_DEEPPINK = ImColor(255, 20, 147);
        static const auto COLOR_LAVENDER = ImColor(230, 230, 250);
        static const auto COLOR_AQUAMARINE = ImColor(127, 255, 212);
        static const auto COLOR_GREENYELLOW = ImColor(173, 255, 47);

        g_start = high_resolution_clock::now();

        /**
         * Frame beginning
         */
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        /*
         * Menu 
         */
        if (GetAsyncKeyState(VK_HOME) & 0x01) { // Toggle menu - xor window transparency flag
            SetWindowLongPtrA(window::overlay_handle, GWL_EXSTYLE, GetWindowLongA(window::overlay_handle, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);
            SetForegroundWindow(window::overlay_handle);
            config::show_menu = !config::show_menu;
        }
        if (config::show_menu) { // Render menu
            ImGui::Begin("menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
            ImGui::Text("DayZ External Utility");
            if (ImGui::Button("Initialize State")) { state_initialize(); };
            ImGui::Checkbox("Near Entities", &config::table_entity_near);
            ImGui::Checkbox("Far Entities", &config::table_entity_far);
            ImGui::Checkbox("Slow Entities", &config::table_entity_slow);
            ImGui::Checkbox("Names", &config::entity_name);
            ImGui::Checkbox("Skeleton", &config::entity_skeleton);
            ImGui::Checkbox("Inventory Items", &config::item_inventoryitem);
            ImGui::End();
        }

        /**
         * Overlay
         */
        ImGui::SetNextWindowSize(ImVec2( window::overlay_width, window::overlay_height ), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2( 0.f, 0.f ), ImGuiCond_Always);
        ImGui::Begin("window-overlay", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);

        render::text_stroke(ImVec2(200, 200), COLOR_GREEN, " -= DayZ External Utility =-");
        render::text_stroke(ImVec2(227, 215), COLOR_SILVER, std::string("[#] Player Count : " + std::to_string(g_network.get_player_count())));
        render::text_stroke(ImVec2(227, 230), COLOR_SILVER, std::string("[#] Frame Time : " + std::to_string(g_frame_time) + "ms"));

        /**
         * Entities
         */
        for(auto entity : g_world.get_entities()) { // Rendering entities
            auto pos3 = entity.get_position();
            auto pos2 = g_world.m_camera.world_to_screen(pos3);
            auto distance = g_world.m_localplayer.get_position().distance(pos3); /* World distance from local player to target */

            if(config::entity_name) { // Entity class name
                render::text_stroke(ImVec2(pos2.x, pos2.y), COLOR_RED, std::string(entity.m_class_name + "[" + std::to_string((int)distance) + "m]").c_str());
            }
            
            /**
             * Bones 
             */
            if (config::entity_skeleton) {
                const auto bone_indices = entity.m_network_id != 0 ? entity::HUMAN_JOINT_INDICES : entity::INFECTED_JOINT_INDICES;
                for (uint32_t index = 0; index < bone_indices.size(); index += 2) { // Rendering bones
                    auto f3 = entity.get_bone_position(bone_indices.at(index));
                    auto t3 = entity.get_bone_position(bone_indices.at(index + 1));

                    auto f2 = g_world.m_camera.world_to_screen(f3);
                    auto t2 = g_world.m_camera.world_to_screen(t3);

                    if (f2 == vector2::zero || t2 == vector2::zero) { continue; }

                    const auto BONE_COLOR = entity.m_network_id ? COLOR_PURPLE : COLOR_RED;
                    render::line(ImVec2(f2.x, f2.y), ImVec2(t2.x, t2.y), BONE_COLOR);
                }
            }
        }

        /**
         * Items 
         */
        for (auto& entity : g_world.get_items()) {
            auto pos3 = entity.get_position();
            auto pos2 = g_world.m_camera.world_to_screen(pos3);
            auto distance = g_world.m_localplayer.get_position().distance(pos3); /* World distance from local player to item */

            auto brush_color = COLOR_LAVENDER;
            if (entity.m_config_name == ENTITY_CONFIGNAME_WEAPON) {
                brush_color = COLOR_GREENYELLOW;
            }
            else if (entity.m_config_name == ENTITY_CONFIGNAME_MAGAZINES) {
                brush_color = COLOR_GOLD;
            }
            else if (entity.m_config_name == ENTITY_CONFIGNAME_CLOTHING) {
                brush_color = COLOR_DEEPPINK;
            }
            else if (entity.m_config_name == ENTITY_CONFIGNAME_OPTICS) {
                brush_color = COLOR_TEAL;
            }

            render::text_stroke(ImVec2(pos2.x, pos2.y), brush_color, std::string(entity.m_class_name + "[" + std::to_string((int)distance) + "m]").c_str());
        }

        ImGui::End();

        /**
         * Frame ending
         */
        ImGui::Render();
        render::backend::d3d11_device_context->OMSetRenderTargets(1, &render::backend::d3d11_render_target, nullptr);
        render::backend::d3d11_device_context->ClearRenderTargetView(render::backend::d3d11_render_target, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        render::backend::swapchain->Present(0, 0);

        g_end = high_resolution_clock::now();
        g_frame_time = duration_cast<milliseconds>(g_end - g_start).count();
    }

    /**
     * Cleanup
     */
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    render::backend::terminate();
    memory::terminate();
    return 0;
}
