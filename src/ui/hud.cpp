#include "hud.h"

#define DKU_G_DEBUG
#include "DKUTil/GUI.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "event/sink_event.h"

namespace ui {

    struct image {
        ID3D11ShaderResourceView* texture = nullptr;
        int32_t width = 0;
        int32_t height = 0;
    };

    static image image_struct;

    // Simple helper function to load an image into a DX11 texture with common settings
    static bool load_texture_from_file(const char* filename,
        ID3D11ShaderResourceView** out_srv,
        std::int32_t& out_width,
        std::int32_t& out_height) {
        // Load from disk into a raw RGBA buffer
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(filename, &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr)
            return false;

        // Create texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof desc);
        desc.Width = image_width;
        desc.Height = image_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* texture_2d = nullptr;
        D3D11_SUBRESOURCE_DATA sub_resource;
        sub_resource.pSysMem = image_data;
        sub_resource.SysMemPitch = desc.Width * 4;
        sub_resource.SysMemSlicePitch = 0;
        DKU_G_DEVICE->CreateTexture2D(&desc, &sub_resource, &texture_2d);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
        ZeroMemory(&view_desc, sizeof view_desc);
        view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        view_desc.Texture2D.MipLevels = desc.MipLevels;
        view_desc.Texture2D.MostDetailedMip = 0;
        DKU_G_DEVICE->CreateShaderResourceView(texture_2d, &view_desc, out_srv);
        texture_2d->Release();

        out_width = image_width;
        out_height = image_height;
        stbi_image_free(image_data);

        return true;
    }


    void hud::draw_ui() {
        if (const auto ui = RE::UI::GetSingleton();
            !ui || ui->GameIsPaused() || !ui->IsCursorHiddenWhenTopmost() || !ui->IsShowingMenus() || !ui->GetMenu<
                RE::HUDMenu>()) {
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

        ImGui::Begin("lamas_tiny_hud", nullptr, window_flag);

        //draw shit here

        ImGui::Image(image_struct.texture,
            ImVec2(static_cast<float>(image_struct.width), static_cast<float>(image_struct.height)));

        ImGui::End();
        open_ = true;
    }


    void hud::callback(SKSE::MessagingInterface::Message* a_msg) {
        if (a_msg->type == SKSE::MessagingInterface::kInputLoaded) {
            DKUtil::GUI::InitD3D(); //Init d3d11 right before the main menu opened.
            DKUtil::GUI::AddCallback(FUNC_INFO(draw_ui));

            logger::info("init gui elements");
        } else if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            // Read Texture only after game engine finished load all it renderer resource.
            const std::string path = R"(.\Data\SKSE\Plugins\img\hud.png)";
            if (load_texture_from_file(path.c_str(), &image_struct.texture, image_struct.width, image_struct.height)) {
                logger::info("loaded texture {}", path.c_str());
            } else {
                logger::error("failed to load texture {}", path.c_str());
            }

            image_struct.width *= static_cast<int32_t>(get_resolution_scale_width());
            image_struct.height *= static_cast<int32_t>(get_resolution_scale_height());

            //img/hud.png
            //show_hud_ = true;
            event::sink_events();
            logger::info("done with data loaded");
        } else if (a_msg->type == SKSE::MessagingInterface::kPreLoadGame) {
            DKUtil::GUI::RemoveCallback(FUNC_INFO(draw_ui));


            DKUtil::GUI::AddCallback(FUNC_INFO(draw_ui));
            logger::info("done with pre load game");
        }
    }

    bool hud::install() {
        const auto g_message = SKSE::GetMessagingInterface();
        if (!g_message) {
            logger::error("Messaging Interface Not Found!");
            return false;
        }

        g_message->RegisterListener(callback);

        logger::info("installed callback for ui");
        return true;
    }

    float hud::get_resolution_scale_width() {
        return ImGui::GetIO().DisplaySize.x / 1920.f;
    }

    float hud::get_resolution_scale_height() {
        return ImGui::GetIO().DisplaySize.y / 1080.f;
    }

    bool hud::is_open() {
        return open_;
    }
}
