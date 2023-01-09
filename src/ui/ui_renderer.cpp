#include "ui_renderer.h"
#include <d3d11.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <dxgi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "image_path.h"
#include "imgui_internal.h"
#include "key_path.h"

#include "event/sink_event.h"
#include "handle/key_position.h"
#include "handle/page_handle.h"
#include "handle/set_data.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"

namespace ui {
    struct image {
        ID3D11ShaderResourceView* texture = nullptr;
        int32_t width = 0;
        int32_t height = 0;
    };

    static image image_struct[static_cast<int32_t>(image_type::total)];
    static image icon_struct[static_cast<int32_t>(icon_image_type::total)];
    static image key_struct[static_cast<int32_t>(key_values::total)];
    static image default_key_struct[static_cast<int32_t>(default_keys::total)];

    LRESULT ui_renderer::wnd_proc_hook::thunk(const HWND h_wnd,
        const UINT u_msg,
        const WPARAM w_param,
        const LPARAM l_param) {
        auto& io = ImGui::GetIO();
        if (u_msg == WM_KILLFOCUS) {
            io.ClearInputCharacters();
            io.ClearInputKeys();
        }

        return func(h_wnd, u_msg, w_param, l_param);
    }

    void ui_renderer::d_3d_init_hook::thunk() {
        func();

        logger::info("D3DInit Hooked"sv);
        const auto render_manager = RE::BSRenderManager::GetSingleton();
        if (!render_manager) {
            logger::error("Cannot find render manager. Initialization failed."sv);
            return;
        }

        const auto [forwarder, context, unk58, unk60, unk68, swapChain, unk78, unk80, renderView, resourceView] =
            render_manager->GetRuntimeData();

        logger::info("Getting swapchain..."sv);
        const auto swapchain = swapChain;
        if (!swapchain) {
            logger::error("Cannot find render manager. Initialization failed."sv);
            return;
        }

        logger::info("Getting swapchain desc..."sv);
        DXGI_SWAP_CHAIN_DESC sd{};
        if (swapchain->GetDesc(std::addressof(sd)) < 0) {
            logger::error("IDXGISwapChain::GetDesc failed."sv);
            return;
        }

        device_ = forwarder;
        context_ = context;

        logger::info("Initializing ImGui..."sv);
        ImGui::CreateContext();
        if (!ImGui_ImplWin32_Init(sd.OutputWindow)) {
            logger::error("ImGui initialization failed (Win32)");
            return;
        }
        if (!ImGui_ImplDX11_Init(device_, context_)) {
            logger::error("ImGui initialization failed (DX11)"sv);
            return;
        }
        logger::info("ImGui initialized.");

        initialized.store(true);

        wnd_proc_hook::func = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrA(sd.OutputWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc_hook::thunk)));
        if (!wnd_proc_hook::func) {
            logger::error("SetWindowLongPtrA failed"sv);
        }
    }

    void ui_renderer::dxgi_present_hook::thunk(std::uint32_t a_p1) {
        func(a_p1);

        if (!d_3d_init_hook::initialized.load()) {
            return;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        draw_ui();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    // Simple helper function to load an image into a DX11 texture with common settings
    bool ui_renderer::load_texture_from_file(const char* filename,
        ID3D11ShaderResourceView** out_srv,
        std::int32_t& out_width,
        std::int32_t& out_height) {
        auto render_manager = RE::BSRenderManager::GetSingleton();
        if (!render_manager) {
            logger::error("Cannot find render manager. Initialization failed."sv);
            return false;
        }

        auto [forwarder, context, unk58, unk60, unk68, swapChain, unk78, unk80, renderView, resourceView] =
            render_manager->GetRuntimeData();

        // Load from disk into a raw RGBA buffer
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr) {
            return false;
        }

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* p_texture = nullptr;
        D3D11_SUBRESOURCE_DATA sub_resource;
        sub_resource.pSysMem = image_data;
        sub_resource.SysMemPitch = desc.Width * 4;
        sub_resource.SysMemSlicePitch = 0;
        device_->CreateTexture2D(&desc, &sub_resource, &p_texture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
        ZeroMemory(&srv_desc, sizeof srv_desc);
        srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MipLevels = desc.MipLevels;
        srv_desc.Texture2D.MostDetailedMip = 0;
        forwarder->CreateShaderResourceView(p_texture, &srv_desc, out_srv);
        p_texture->Release();

        out_width = image_width;
        out_height = image_height;
        stbi_image_free(image_data);

        return true;
    }

    ui_renderer::ui_renderer() = default;

    void ui_renderer::draw_text(const float a_x,
        const float a_y,
        const float a_offset_x,
        const float a_offset_y,
        const char* a_text,
        const ImU32 a_color) {
        const ImFont* font = ImGui::GetFont();
        const auto font_size = config::mcm_setting::get_slot_count_text_font_size();

        //get data from normal hud, and add an offset config for each image
        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        const auto extra_x = config::mcm_setting::get_slot_count_text_offset();
        const auto extra_y = extra_x;

        ImVec2 position;
        if (width_setting > a_x || height_setting > a_y) {
            position = ImVec2(0.f, 0.f);
        } else {
            position = ImVec2(width_setting + a_offset_x + extra_x, height_setting + a_offset_y + extra_y);
        }

        ImGui::GetWindowDrawList()->AddText(font, font_size, position, a_color, a_text, nullptr, 0.0f, nullptr);
    }

    void ui_renderer::draw_element(ID3D11ShaderResourceView* a_texture,
        const ImVec2 a_center,
        const ImVec2 a_size,
        const float a_angle,
        const ImU32 a_color) {
        const float cos_a = cosf(a_angle);
        const float sin_a = sinf(a_angle);
        const ImVec2 pos[4] = { a_center + ImRotate(ImVec2(-a_size.x * 0.5f, -a_size.y * 0.5f), cos_a, sin_a),
                                a_center + ImRotate(ImVec2(+a_size.x * 0.5f, -a_size.y * 0.5f), cos_a, sin_a),
                                a_center + ImRotate(ImVec2(+a_size.x * 0.5f, +a_size.y * 0.5f), cos_a, sin_a),
                                a_center + ImRotate(ImVec2(-a_size.x * 0.5f, +a_size.y * 0.5f), cos_a, sin_a)

        };
        constexpr ImVec2 uvs[4] = { ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f) };


        ImGui::GetWindowDrawList()
            ->AddImageQuad(a_texture, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], a_color);
    }

    void ui_renderer::draw_hud(const float a_x, const float a_y) {
        constexpr auto angle = 0.f;

        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        ImVec2 center;
        if (width_setting > a_x || height_setting > a_y) {
            center = ImVec2(0.f, 0.f);
        } else {
            center = ImVec2(width_setting, height_setting);
        }

        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::hud)];

        const auto size = ImVec2(static_cast<float>(width) * config::mcm_setting::get_hud_image_scale_width(),
            static_cast<float>(height) * config::mcm_setting::get_hud_image_scale_height());

        draw_element(texture, center, size, angle);
    }


    void ui_renderer::draw_slot(const float a_x,
        const float a_y,
        const float a_offset_x,
        const float a_offset_y,
        const uint32_t a_modify) {
        constexpr auto angle = 0.f;

        //get data from normal hud, and add an offset config for each image
        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        ImVec2 center;
        if (width_setting > a_x || height_setting > a_y) {
            center = ImVec2(0.f, 0.f);
        } else {
            center = ImVec2(width_setting + a_offset_x, height_setting + a_offset_y);
        }

        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::round)];

        //for now use scale from normal setting, but later add separate config
        const auto size = ImVec2(static_cast<float>(width) * config::mcm_setting::get_hud_image_scale_width() +
                                 config::file_setting::get_extra_size_for_image(),
            static_cast<float>(height) * config::mcm_setting::get_hud_image_scale_height() +
            config::file_setting::get_extra_size_for_image());

        const ImU32 color = IM_COL32(a_modify, a_modify, a_modify, draw_full);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_slots(const float a_x,
        const float a_y,
        const std::map<position, page_setting*>& a_settings) {
        for (auto [position, page_setting] : a_settings) {
            //we could access fade settings here too
            const auto offset_setting = page_setting->offset_setting;
            draw_slot(a_x,
                a_y,
                offset_setting->offset_slot_x,
                offset_setting->offset_slot_y,
                page_setting->button_press_modify);
            draw_icon(a_x,
                a_y,
                offset_setting->offset_slot_x,
                offset_setting->offset_slot_y,
                page_setting->icon_type,
                page_setting->icon_opacity);
            if (auto slot_settings = page_setting->slot_settings;
                position == handle::page_setting::position::left && !slot_settings.empty() && slot_settings.front()->
                item_count > 0) {
                draw_text(a_x,
                    a_y,
                    offset_setting->offset_slot_x,
                    offset_setting->offset_slot_y,
                    std::to_string(slot_settings.front()->item_count).c_str());
            }
        }
    }


    void ui_renderer::draw_key(const float a_x, const float a_y, const float a_offset_x, const float a_offset_y) {
        constexpr auto angle = 0.f;

        //get data from normal hud, and add an offset config for each image
        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        ImVec2 center;
        if (width_setting > a_x || height_setting > a_y) {
            center = ImVec2(0.f, 0.f);
        } else {
            center = ImVec2(width_setting + a_offset_x, height_setting + a_offset_y);
        }

        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::key)];

        //for now use scale from normal setting, but later add separate config
        //add hardcoded 1px
        const auto size = ImVec2(static_cast<float>(width) * config::mcm_setting::get_hud_image_scale_width() +
                                 config::file_setting::get_extra_size_for_image(),
            static_cast<float>(height) * config::mcm_setting::get_hud_image_scale_height() +
            config::file_setting::get_extra_size_for_image());

        draw_element(texture, center, size, angle);
    }

    void ui_renderer::draw_keys(const float a_x, const float a_y, const std::map<position, page_setting*>& a_settings) {
        for (auto [position, page_setting] : a_settings) {
            //we could access fade settings here too
            const auto offset_setting = page_setting->offset_setting;
            draw_key(a_x, a_y, offset_setting->offset_key_x, offset_setting->offset_key_y);
            draw_key_icon(a_x,
                a_y,
                offset_setting->offset_key_x,
                offset_setting->offset_key_y,
                page_setting->key,
                draw_full);
        }
    }

    void ui_renderer::draw_icon(const float a_x,
        const float a_y,
        const float a_offset_x,
        const float a_offset_y,
        const icon_image_type a_type,
        const uint32_t a_opacity) {
        constexpr auto angle = 0.f;

        //get data from normal hud, and add an offset config for each image
        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        ImVec2 center;
        if (width_setting > a_x || height_setting > a_y) {
            center = ImVec2(0.f, 0.f);
        } else {
            center = ImVec2(width_setting + a_offset_x, height_setting + a_offset_y);
        }

        const auto [texture, width, height] = icon_struct[static_cast<int32_t>(a_type)];

        //for now use scale from normal setting, but later add separate config
        //add hardcoded 1px
        const auto size = ImVec2(static_cast<float>(width) * config::mcm_setting::get_icon_scale_width() +
                                 config::file_setting::get_extra_size_for_image(),
            static_cast<float>(height) * config::mcm_setting::get_icon_scale_height() +
            config::file_setting::get_extra_size_for_image());

        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_opacity);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_key_icon(const float a_x,
        const float a_y,
        const float a_offset_x,
        const float a_offset_y,
        const uint32_t a_key,
        const uint32_t a_opacity) {
        constexpr auto angle = 0.f;

        //get data from normal hud, and add an offset config for each image
        const auto width_setting = config::mcm_setting::get_hud_image_position_width();
        const auto height_setting = config::mcm_setting::get_hud_image_position_height();

        ImVec2 center;
        if (width_setting > a_x || height_setting > a_y) {
            center = ImVec2(0.f, 0.f);
        } else {
            center = ImVec2(width_setting + a_offset_x, height_setting + a_offset_y);
        }

        const auto [texture, width, height] = get_key_icon(a_key);

        //for now use scale from normal setting, but later add separate config
        //add hardcoded 1px
        const auto size = ImVec2(static_cast<float>(width) * config::mcm_setting::get_key_icon_scale_width() +
                                 config::file_setting::get_extra_size_for_image(),
            static_cast<float>(height) * config::mcm_setting::get_key_icon_scale_height() +
            config::file_setting::get_extra_size_for_image());

        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_opacity);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_ui() {
        if (!show_ui_)
            return;

        if (const auto ui = RE::UI::GetSingleton(); !ui || ui->GameIsPaused() || !ui->IsCursorHiddenWhenTopmost() ||
                                                    !ui->IsShowingMenus() || !ui->GetMenu<RE::HUDMenu>()) {
            return;
        }

        if (const auto control_map = RE::ControlMap::GetSingleton();
            !control_map || !control_map->IsMovementControlsEnabled() ||
            control_map->contextPriorityStack.back() != RE::UserEvents::INPUT_CONTEXT_ID::kGameplay) {
            return;
        }

        static constexpr ImGuiWindowFlags window_flag =
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;


        const float screen_size_x = ImGui::GetIO().DisplaySize.x, screen_size_y = ImGui::GetIO().DisplaySize.y;


        ImGui::SetNextWindowSize(ImVec2(screen_size_x, screen_size_y));
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

        ImGui::Begin(hud_name, nullptr, window_flag);

        const auto settings = handle::page_handle::get_singleton()->get_page();

        draw_hud(screen_size_x, screen_size_y);
        draw_slots(screen_size_x, screen_size_y, settings);
        draw_keys(screen_size_x, screen_size_y, settings);

        ImGui::End();
    }


    void ui_renderer::message_callback(SKSE::MessagingInterface::Message* msg)
    //CallBack & LoadTextureFromFile should called after resource loaded.
    {
        if (msg->type == SKSE::MessagingInterface::kDataLoaded && d_3d_init_hook::initialized) {
            // Read Texture only after game engine finished load all it renderer resource.

            load_images(image_type_path_map, image_struct);
            load_images(icon_type_path_map, icon_struct);
            load_images(key_icon_path_map, key_struct);
            load_images(default_key_icon_path_map, default_key_struct);

            show_ui_ = true;
            event::sink_events();
            //config is already loaded
            handle::set_data::set_slot_data();
            logger::info("done with data loaded");
        }
    }

    template <typename T>
    void ui_renderer::load_images(std::map<T, const char*> a_map, image a_struct[]) {
        for (auto [type, path] : a_map) {
            const auto idx = static_cast<int32_t>(type);
            if (load_texture_from_file(path,
                &a_struct[idx].texture,
                a_struct[idx].width,
                a_struct[idx].height)) {
                logger::info("loaded texture {}"sv, path);
            } else {
                logger::error("failed to load texture {}"sv, path);
            }

            a_struct[idx].width *= static_cast<int32_t>(get_resolution_scale_width());
            a_struct[idx].height *= static_cast<int32_t>(get_resolution_scale_height());
        }
    }

    image ui_renderer::get_key_icon(const uint32_t a_key) {
        auto return_image = default_key_struct[static_cast<int32_t>(default_keys::key)];
        if (key_icon_path_map.contains(static_cast<key_values>(a_key))) {
            return_image = key_struct[a_key];
        }
        return return_image;
    }

    bool ui_renderer::install() {
        auto g_message = SKSE::GetMessagingInterface();
        if (!g_message) {
            logger::error("Messaging Interface Not Found. return."sv);
            return false;
        }

        g_message->RegisterListener(message_callback);

        SKSE::AllocTrampoline(14 * 2);

        stl::write_thunk_call<d_3d_init_hook>();
        stl::write_thunk_call<dxgi_present_hook>();

        logger::info("installed callback for ui. return.");
        return true;
    }

    float ui_renderer::get_resolution_scale_width() { return ImGui::GetIO().DisplaySize.x / 1920.f; }

    float ui_renderer::get_resolution_scale_height() { return ImGui::GetIO().DisplaySize.y / 1080.f; }

    float ui_renderer::get_resolution_width() { return ImGui::GetIO().DisplaySize.x; }

    float ui_renderer::get_resolution_height() { return ImGui::GetIO().DisplaySize.y; }
}
