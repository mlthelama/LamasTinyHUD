#pragma once
#include <imgui.h>

namespace  ui {
    enum class animation_type {highlight=0};

    class animation {
    public:
        animation(const animation&) = delete;
        animation& operator =(const animation&) = delete;
        animation(ImVec2 center,
            ImVec2 size,
            float angle,
            uint32_t alpha,
            uint32_t r_color,
            uint32_t g_color,
            uint32_t b_color,
            float duration,
            uint32_t length ) {
            this->center = center;
            this->size = size;
            this->angle = angle;
            this->alpha = alpha;
            this->r_color = r_color;
            this->g_color = g_color;
            this->b_color = b_color;
            this->duration = duration;
            this->length = length;

            this->current_frame = 0;
            this->delta_time = 0.0f;
            this->single_frame_time = duration / static_cast<float>(length);
        };

        virtual void animate_action(float delta_time) {};
        virtual bool is_over() {
            return current_frame >= length;
        }
        ImVec2 center;
        ImVec2 size; 
        float angle;
        uint32_t current_frame;
        uint32_t alpha;
        uint32_t r_color;
        uint32_t g_color;
        uint32_t b_color;
        float duration;
        uint32_t length;
    protected:
        float delta_time;
        float single_frame_time;
    };

    class fade_framed_out_animation: public animation {
    public:
        fade_framed_out_animation(const fade_framed_out_animation&) = delete;
        fade_framed_out_animation& operator =(const fade_framed_out_animation&) = delete;
        fade_framed_out_animation(
            ImVec2 center,
            ImVec2 size,
            float angle,
            uint32_t alpha,
            uint32_t r_color,
            uint32_t g_color,
            uint32_t b_color,
            float duration,
            uint32_t length,
            uint32_t fade_step
            ): 
            animation (
                  center,
                  size,
                  angle,
                  alpha,
                  r_color,
                  g_color,
                  b_color,
                  duration,
                  length
            ) {
            this->fade_step = fade_step;
        };
        uint32_t fade_step;
        void animate_action(float delta_time) override {
            if(!is_over()) {
                this->delta_time += delta_time;
                if(this->delta_time >= this->single_frame_time) {
                    float frames_to_add = this->delta_time / this->single_frame_time;
                    this->current_frame += static_cast<uint32_t>(std::floor(frames_to_add));
                    this->delta_time -= this->current_frame * this->single_frame_time;
                }
            }
        }

        bool is_over() override {
            return current_frame >= length || alpha <= 0;
        }
    };
}
