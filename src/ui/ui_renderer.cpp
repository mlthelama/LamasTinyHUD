﻿#include "ui_renderer.h"
#include <d3d11.h>
#include <dxgi.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#pragma warning(push)
#pragma warning(disable : 4244)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

#include "image_path.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "animation_handler.h"
#include "event/key_manager.h"
#include "event/sink_event.h"
#include "handle/handle/ammo_handle.h"
#include "handle/handle/name_handle.h"
#include "handle/handle/page_handle.h"
#include "handle/setting/set_setting_data.h"
#include "imgui_internal.h"
#include "key_path.h"
#include "setting/file_setting.h"
#include "setting/mcm_setting.h"

namespace ui {
    using mcm = config::mcm_setting;

    static std::map<animation_type, std::vector<image>> animation_frame_map = {};
    static std::vector<std::pair<animation_type, std::unique_ptr<animation>>> animation_list;

    static std::map<uint32_t, image> image_struct;
    static std::map<uint32_t, image> icon_struct;
    static std::map<uint32_t, image> key_struct;
    static std::map<uint32_t, image> default_key_struct;
    static std::map<uint32_t, image> ps_key_struct;
    static std::map<uint32_t, image> xbox_key_struct;


    float fade = 1.0f;
    bool fade_in = true;
    float fade_out_timer = mcm::get_fade_timer_outside_combat();
    ImFont* loaded_font;


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

    void ui_renderer::draw_animations_frame() {
        auto it = animation_list.begin();
        while (it != animation_list.end()) {
            logger::trace("in draw animation draw loop"sv);
            if (!it->second->is_over()) {
                logger::trace("animation playing"sv);
                animation* anim = it->second.get();
                draw_element(animation_frame_map[it->first][anim->current_frame].texture,
                    anim->center,
                    anim->size,
                    anim->angle,
                    IM_COL32(anim->r_color, anim->g_color, anim->b_color, anim->alpha));
                anim->animate_action(ImGui::GetIO().DeltaTime);
                ++it;
            } else {
                logger::trace("animation complete"sv);
                it = animation_list.erase(it);
            }
        }
    }

    void ui_renderer::draw_text(const float a_x,
        const float a_y,
        const float a_offset_x,
        const float a_offset_y,
        const float a_offset_extra_x,
        const float a_offset_extra_y,
        const char* a_text,
        const ImU32 a_color,
        const float a_font_size) {
        //it should center the text, it kind of does
        const ImVec2 text_size = ImGui::CalcTextSize(a_text, nullptr, true);
        const auto position = ImVec2(a_x + a_offset_x + a_offset_extra_x - text_size.x * 0.5f,
            a_y + a_offset_y + a_offset_extra_y - text_size.y * 0.5f);

        auto font = loaded_font;
        if (!font) {
            font = ImGui::GetDefaultFont();
        }

        ImGui::GetWindowDrawList()->AddText(font, a_font_size, position, a_color, a_text, nullptr, 0.0f, nullptr);
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

    void ui_renderer::draw_hud(const float a_x,
        const float a_y,
        const float a_scale_x,
        const float a_scale_y,
        const uint32_t a_alpha) {
        constexpr auto angle = 0.f;

        const auto center = ImVec2(a_x, a_y);
        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::hud)];
        const auto size = ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y);
        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_alpha);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_slot(const float a_screen_x,
        const float a_screen_y,
        const float a_scale_x,
        const float a_scale_y,
        const float a_offset_x,
        const float a_offset_y,
        const uint32_t a_modify,
        const uint32_t a_alpha) {
        constexpr auto angle = 0.f;

        const auto center = ImVec2(a_screen_x + a_offset_x, a_screen_y + a_offset_y);
        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::round)];
        const auto size = ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y);
        const ImU32 color = IM_COL32(a_modify, a_modify, a_modify, a_alpha);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::init_animation(const animation_type animation_type,
        const float a_screen_x,
        const float a_screen_y,
        const float a_scale_x,
        const float a_scale_y,
        const float a_offset_x,
        const float a_offset_y,
        [[maybe_unused]] const uint32_t a_modify,
        const uint32_t a_alpha,
        float a_duration) {
        logger::trace("starting inited animation");
        constexpr auto angle = 0.0f;

        const auto size = static_cast<uint32_t>(animation_frame_map[animation_type].size());
        const auto width = static_cast<uint32_t>(animation_frame_map[animation_type][0].width);
        const auto height = static_cast<uint32_t>(animation_frame_map[animation_type][0].height);

        std::unique_ptr<animation> anim =
            std::make_unique<fade_framed_out_animation>(ImVec2(a_screen_x + a_offset_x, a_screen_y + a_offset_y),
                ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y),
                angle,
                a_alpha,
                ui::draw_full,
                ui::draw_full,
                ui::draw_full,
                a_duration,
                size,
                10);
        animation_list.emplace_back(static_cast<ui::animation_type>(animation_type), std::move(anim));
        logger::trace("done inited animation. return.");
    }

    void
        ui_renderer::draw_slots(const float a_x, const float a_y, const std::map<position, page_setting*>& a_settings) {
        for (auto [position, page_setting] : a_settings) {
            if (!page_setting) {
                continue;
            }
            const auto draw_setting = page_setting->draw_setting;
            draw_slot(a_x,
                a_y,
                draw_setting->hud_image_scale_width,
                draw_setting->hud_image_scale_height,
                draw_setting->offset_slot_x,
                draw_setting->offset_slot_y,
                page_setting->button_press_modify,
                draw_setting->background_icon_transparency);
            draw_icon(a_x,
                a_y,
                page_setting->draw_setting->icon_scale_width,
                page_setting->draw_setting->icon_scale_height,
                draw_setting->offset_slot_x,
                draw_setting->offset_slot_y,
                page_setting->icon_type,
                draw_setting->icon_transparency);
            if (page_setting->highlight_slot) {
                page_setting->highlight_slot = false;
                init_animation(animation_type::highlight,
                    a_x,
                    a_y,
                    draw_setting->hud_image_scale_width,
                    draw_setting->hud_image_scale_height,
                    draw_setting->offset_slot_x,
                    draw_setting->offset_slot_y,
                    draw_full,
                    255 / 5,  //TODO setting
                    0.1f);    //TODO setting
            }

            if (mcm::get_elden_demon_souls() && page_setting->item_name && page_setting->slot_settings.front()->form) {
                const ImU32 color =
                    IM_COL32(draw_full, draw_full, draw_full, page_setting->draw_setting->text_transparency);
                draw_text(draw_setting->width_setting,
                    draw_setting->height_setting,
                    draw_setting->offset_slot_x,
                    draw_setting->offset_slot_y,
                    draw_setting->offset_name_text_x,
                    draw_setting->offset_name_text_y,
                    page_setting->slot_settings.front()->form->GetName(),
                    color,
                    page_setting->font_size);
            }


            if (auto slot_settings = page_setting->slot_settings; !slot_settings.empty()) {
                const auto first_type = slot_settings.front()->type;
                const ImU32 color = IM_COL32(mcm::get_slot_count_red(),
                    mcm::get_slot_count_green(),
                    mcm::get_slot_count_blue(),
                    page_setting->draw_setting->text_transparency);
                switch (first_type) {
                    case handle::slot_setting::slot_type::scroll:
                    case handle::slot_setting::slot_type::consumable:
                        draw_text(draw_setting->width_setting,
                            draw_setting->height_setting,
                            draw_setting->offset_slot_x,
                            draw_setting->offset_slot_y,
                            draw_setting->offset_text_x,
                            draw_setting->offset_text_y,
                            std::to_string(slot_settings.front()->item_count).c_str(),
                            color,
                            page_setting->font_size);
                        break;
                    case handle::slot_setting::slot_type::shout:
                    case handle::slot_setting::slot_type::power:
                    case handle::slot_setting::slot_type::magic:
                        draw_text(draw_setting->width_setting,
                            draw_setting->height_setting,
                            draw_setting->offset_slot_x,
                            draw_setting->offset_slot_y,
                            draw_setting->offset_text_x,
                            draw_setting->offset_text_y,
                            slot_settings.front()->action == handle::slot_setting::acton_type::instant ? "I" : "E",
                            color,
                            page_setting->font_size);
                        break;
                    case handle::slot_setting::slot_type::weapon:
                    case handle::slot_setting::slot_type::shield:
                    case handle::slot_setting::slot_type::armor:
                    case handle::slot_setting::slot_type::empty:
                    case handle::slot_setting::slot_type::misc:
                    case handle::slot_setting::slot_type::light:
                    case handle::slot_setting::slot_type::lantern:
                    case handle::slot_setting::slot_type::mask:
                        //Nothing, for now
                        break;
                }
            }
        }
        const auto ammo_handle = handle::ammo_handle::get_singleton();
        if (const auto current_ammo = ammo_handle->get_current(); current_ammo && mcm::get_elden_demon_souls()) {
            const ImU32 color = IM_COL32(mcm::get_slot_count_red(),
                mcm::get_slot_count_green(),
                mcm::get_slot_count_blue(),
                mcm::get_text_transparency());
            draw_slot(a_x,
                a_y,
                mcm::get_hud_arrow_image_scale_width(),
                mcm::get_hud_arrow_image_scale_height(),
                mcm::get_arrow_slot_offset_x(),
                mcm::get_arrow_slot_offset_y(),
                current_ammo->button_press_modify,
                mcm::get_background_icon_transparency());
            draw_icon(a_x,
                a_y,
                mcm::get_arrow_icon_scale_width(),
                mcm::get_arrow_icon_scale_height(),
                mcm::get_arrow_slot_offset_x(),
                mcm::get_arrow_slot_offset_y(),
                icon_image_type::arrow,
                mcm::get_icon_transparency());
            draw_text(a_x,
                a_y,
                mcm::get_arrow_slot_offset_x(),
                mcm::get_arrow_slot_offset_y(),
                mcm::get_arrow_slot_count_text_offset(),
                mcm::get_arrow_slot_count_text_offset(),
                std::to_string(current_ammo->item_count ? current_ammo->item_count : 0).c_str(),
                color,
                mcm::get_arrow_count_font_size());

            if (current_ammo->highlight_slot) {
                current_ammo->highlight_slot = false;
                init_animation(animation_type::highlight,
                    a_x,
                    a_y,
                    mcm::get_hud_arrow_image_scale_width(),
                    mcm::get_hud_arrow_image_scale_height(),
                    mcm::get_arrow_slot_offset_x(),
                    mcm::get_arrow_slot_offset_y(),
                    draw_full,
                    255 / 5,  //TODO setting
                    0.1f);    //TODO setting
            }
        }
        draw_animations_frame();
    }

    void ui_renderer::draw_key(const float a_x,
        const float a_y,
        const float a_scale_x,
        const float a_scale_y,
        const float a_offset_x,
        const float a_offset_y,
        const uint32_t a_alpha) {
        constexpr auto angle = 0.f;

        const auto center = ImVec2(a_x + a_offset_x, a_y + a_offset_y);
        const auto [texture, width, height] = image_struct[static_cast<int32_t>(image_type::key)];
        const auto size = ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y);
        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_alpha);

        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_keys(const float a_x, const float a_y, const std::map<position, page_setting*>& a_settings) {
        for (auto [position, page_setting] : a_settings) {
            if (!page_setting) {
                continue;
            }
            const auto draw_setting = page_setting->draw_setting;
            if (config::file_setting::get_draw_key_background()) {
                draw_key(a_x,
                    a_y,
                    draw_setting->key_icon_scale_width,
                    draw_setting->key_icon_scale_height,
                    draw_setting->offset_key_x,
                    draw_setting->offset_key_y);
            }
            draw_key_icon(a_x,
                a_y,
                draw_setting->key_icon_scale_width,
                draw_setting->key_icon_scale_height,
                draw_setting->offset_key_x,
                draw_setting->offset_key_y,
                page_setting->key,
                draw_setting->key_transparency);
        }

        if (mcm::get_draw_toggle_button()) {
            draw_key_icon(mcm::get_hud_image_position_width(),
                mcm::get_hud_image_position_height(),
                mcm::get_key_icon_scale_width(),
                mcm::get_key_icon_scale_height(),
                mcm::get_toggle_key_offset_x(),
                mcm::get_toggle_key_offset_y(),
                mcm::get_toggle_key(),
                mcm::get_key_transparency());
        }
    }

    void ui_renderer::draw_icon(const float a_x,
        const float a_y,
        const float a_scale_x,
        const float a_scale_y,
        const float a_offset_x,
        const float a_offset_y,
        const icon_image_type a_type,
        const uint32_t a_alpha) {
        constexpr auto angle = 0.f;

        const auto center = ImVec2(a_x + a_offset_x, a_y + a_offset_y);

        const auto [texture, width, height] = icon_struct[static_cast<int32_t>(a_type)];

        const auto size = ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y);

        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_alpha);


        draw_element(texture, center, size, angle, color);
    }

    void ui_renderer::draw_key_icon(const float a_x,
        const float a_y,
        const float a_scale_x,
        const float a_scale_y,
        const float a_offset_x,
        const float a_offset_y,
        const uint32_t a_key,
        const uint32_t a_alpha) {
        constexpr auto angle = 0.f;

        const auto center = ImVec2(a_x + a_offset_x, a_y + a_offset_y);

        const auto [texture, width, height] = get_key_icon(a_key);

        const auto size = ImVec2(static_cast<float>(width) * a_scale_x, static_cast<float>(height) * a_scale_y);

        const ImU32 color = IM_COL32(draw_full, draw_full, draw_full, a_alpha);

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

        if (mcm::get_hide_outside_combat()) {
            if (!RE::PlayerCharacter::GetSingleton()->IsInCombat()) {
                fade_in = false;
            } else {
                fade_in = true;
            }
        } else {
            fade_in = true;
        }

        static constexpr ImGuiWindowFlags window_flag =
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

        const float screen_size_x = ImGui::GetIO().DisplaySize.x, screen_size_y = ImGui::GetIO().DisplaySize.y;

        ImGui::SetNextWindowSize(ImVec2(screen_size_x, screen_size_y));
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

        ImGui::GetStyle().Alpha = fade;

        ImGui::Begin(hud_name, nullptr, window_flag);

        if (const auto settings = handle::page_handle::get_singleton()->get_active_page(); !settings.empty()) {
            auto x = mcm::get_hud_image_position_width();
            auto y = mcm::get_hud_image_position_height();
            const auto scale_x = mcm::get_hud_image_scale_width();
            const auto scale_y = mcm::get_hud_image_scale_height();
            const auto alpha = mcm::get_background_transparency();
            if (screen_size_x < x || screen_size_y < y) {
                x = 0.f;
                y = 0.f;
            }

            draw_hud(x, y, scale_x, scale_y, alpha);
            draw_slots(x, y, settings);
            draw_keys(x, y, settings);
            if (mcm::get_draw_current_items_text()) {
                const ImU32 color = IM_COL32(mcm::get_current_items_red(),
                    mcm::get_current_items_green(),
                    mcm::get_current_items_blue(),
                    mcm::get_text_transparency());
                draw_text(x,
                    y,
                    mcm::get_current_items_offset_x(),
                    mcm::get_current_items_offset_y(),
                    0.f,
                    0.f,
                    handle::name_handle::get_singleton()->get_item_name_string().c_str(),
                    color,
                    mcm::get_current_items_font_size());
            }
        }

        ImGui::End();

        if (mcm::get_hide_outside_combat()) {
            if (fade_in && fade != 1.0f) {
                fade_out_timer = mcm::get_fade_timer_outside_combat();
                fade += 0.01f;
                if (fade > 1.0f) {
                    fade = 1.0f;
                }
            } else if (!fade_in && fade != 0.0f) {
                if (fade_out_timer > 0.0f) {
                    fade_out_timer -= ImGui::GetIO().DeltaTime;
                } else {
                    fade -= 0.01f;
                    if (fade < 0.0f) {
                        fade = 0.0f;
                    }
                }
            }
        }
    }


    void ui_renderer::message_callback(SKSE::MessagingInterface::Message* msg)
    //CallBack & LoadTextureFromFile should call after resource loaded.
    {
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (msg->type) {
            case SKSE::MessagingInterface::kDataLoaded:
                if (d_3d_init_hook::initialized) {
                    logger::trace("Load Images, scale values width {}, height {}"sv,
                        get_resolution_scale_width(),
                        get_resolution_scale_height());

                    load_images(image_type_name_map, image_struct, img_directory);
                    load_images(icon_type_name_map, icon_struct, icon_directory);
                    load_images(key_icon_name_map, key_struct, key_directory);
                    load_images(default_key_icon_path_map, default_key_struct, key_directory);
                    load_images(gamepad_ps_icon_name_map, ps_key_struct, key_directory);
                    load_images(gamepad_xbox_icon_name_map, xbox_key_struct, key_directory);

                    load_animation_frames(highlight_animation_directory,
                        animation_frame_map[animation_type::highlight]);
                    logger::trace("frame length is {}"sv, animation_frame_map[animation_type::highlight].size());
                    load_font();
                    //show_ui_ = true;
                    event::sink_events();
                    logger::info("done with data loaded"sv);
                }
                break;
            case SKSE::MessagingInterface::kPostLoadGame:
            case SKSE::MessagingInterface::kNewGame:
                handle::set_setting_data::read_and_set_data();
                show_ui_ = true;
                break;
        }
    }

    template <typename T>
    void ui_renderer::load_images(std::map<std::string, T>& a_map, std::map<uint32_t, image>& a_struct, std::string& file_path) {
        for(const auto& entry : std::filesystem::directory_iterator(file_path)) {

            if(a_map.contains(entry.path().filename().string())) {
                const auto index = static_cast<int32_t>(a_map[entry.path().filename().string()]);
                if (load_texture_from_file(entry.path().string().c_str(), &a_struct[index].texture, a_struct[index].width, a_struct[index].height)) {
                    logger::info("loaded texture {} width: {}, height: {}"sv,
                        entry.path().filename().string().c_str(),
                        a_struct[index].width,
                        a_struct[index].height);
                } else {
                    logger::error("failed to load texture {}"sv, entry.path().filename().string().c_str());
                }

                a_struct[index].width = static_cast<int32_t>(a_struct[index].width * get_resolution_scale_width());
                a_struct[index].height = static_cast<int32_t>(a_struct[index].height * get_resolution_scale_height());
            }
        }
    }

    void ui_renderer::load_animation_frames(std::string& file_path, std::vector<image>& frame_list) {
        std::vector<std::string> paths;
        std::string current_directory = std::filesystem::current_path().string();
        for(const auto& entry : std::filesystem::directory_iterator(file_path)) {
            paths.push_back(entry.path().string());
            logger::info("in here");

            ID3D11ShaderResourceView* texture = nullptr;
            int32_t width = 0;
            int32_t height = 0;
            load_texture_from_file(entry.path().string().c_str(), &texture, width, height);
            logger::trace("loading animation frame: {}"sv, entry.path().string().c_str());
            image img;
            img.texture = texture;
            img.width = static_cast<int32_t>(width * get_resolution_scale_width());
            img.height = static_cast<int32_t>(height * get_resolution_scale_height());
            frame_list.push_back(img);
        }
    }

    image ui_renderer::get_key_icon(const uint32_t a_key) {
        auto return_image = default_key_struct[static_cast<int32_t>(default_keys::key)];
        if (a_key >= event::key_manager::k_gamepad_offset) {
            if (mcm::get_controller_set() == static_cast<uint32_t>(controller_set::playstation)) {
                return_image = ps_key_struct[a_key];
            } else {
                return_image = xbox_key_struct[a_key];
            }
        } else {
            if (key_struct.contains(a_key)) {
                return_image = key_struct[a_key];
            }
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

    void ui_renderer::set_fade(const bool a_in, const float a_value) {
        fade_in = a_in;
        fade = a_value;
        if (a_in) {
            fade_out_timer = mcm::get_fade_timer_outside_combat();
        }
    }

    bool ui_renderer::get_fade() { return fade_in; }

    void ui_renderer::load_font() {
        ImGuiIO& io = ImGui::GetIO();
        ImVector<ImWchar> ranges;
        ImFontGlyphRangesBuilder builder;
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        if (config::file_setting::get_font_chinese_full()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
        }
        if (config::file_setting::get_font_chinese_simplified_common()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        }
        if (config::file_setting::get_font_cyrillic()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
        }
        if (config::file_setting::get_font_japanese()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
        }
        if (config::file_setting::get_font_korean()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesKorean());
        }
        if (config::file_setting::get_font_thai()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesThai());
        }
        if (config::file_setting::get_font_vietnamese()) {
            builder.AddRanges(io.Fonts->GetGlyphRangesVietnamese());
        }
        builder.BuildRanges(&ranges);


        std::string path = R"(Data\SKSE\Plugins\resources\font\)" + config::file_setting::get_font_file_name();
        logger::trace("Trying to load Font file {}"sv, path);

        loaded_font =
            io.Fonts->AddFontFromFileTTF(path.c_str(), config::file_setting::get_font_size(), nullptr, ranges.Data);
        if (io.Fonts->Build()) {
            ImGui_ImplDX11_CreateDeviceObjects();
            logger::info("Custom Font {} loaded."sv, path);
            return;
        }
    }

    void ui_renderer::toggle_show_ui() {
        if (show_ui_) {
            show_ui_ = false;
        } else {
            show_ui_ = true;
        }
        logger::trace("Show UI is now {}"sv, show_ui_);
    }
}
