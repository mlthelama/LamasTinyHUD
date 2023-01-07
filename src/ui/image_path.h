﻿#pragma once

namespace ui {
    constexpr auto hud_name = "lamas_tiny_hud";

    constexpr auto draw_full = 255;

    enum class image_type { hud = 0, round = 1, key = 2, total = 3 };

    inline static std::map<image_type, const char*> image_type_path_map = {
        { image_type::hud, R"(.\Data\SKSE\Plugins\resources\img\hud.png)" },
        { image_type::round, R"(.\Data\SKSE\Plugins\resources\img\round.png)" },
        { image_type::key, R"(.\Data\SKSE\Plugins\resources\img\key.png)" }
    };

    enum class icon_image_type {
        health_potion = 0,
        default_potion = 1,
        one_handed_sword = 2,
        one_handed_axe = 3,
        mace = 4,
        dagger = 5,
        two_handed_sword = 6,
        two_handed_axe = 7,
        staff = 8,
        bow = 9,
        crossbow = 10,
        default_spell = 11,
        default_spell_alt = 12,
        shout = 13,
        power = 14,
        food = 15,
        shield = 16,
        default_icon = 17,
        total
    };


    inline static std::map<icon_image_type, const char*> icon_type_path_map = {
        { icon_image_type::health_potion, R"(.\Data\SKSE\Plugins\resources\icons\health_potion.png)" },
        { icon_image_type::default_potion, R"(.\Data\SKSE\Plugins\resources\icons\default_potion.png)" },
        { icon_image_type::one_handed_sword, R"(.\Data\SKSE\Plugins\resources\icons\one_handed_sword.png)" },
        { icon_image_type::one_handed_axe, R"(.\Data\SKSE\Plugins\resources\icons\one_handed_axe.png)" },
        { icon_image_type::mace, R"(.\Data\SKSE\Plugins\resources\icons\mace.png)" },
        { icon_image_type::dagger, R"(.\Data\SKSE\Plugins\resources\icons\dagger.png)" },
        { icon_image_type::two_handed_sword, R"(.\Data\SKSE\Plugins\resources\icons\two_handed_sword.png)" },
        { icon_image_type::two_handed_axe, R"(.\Data\SKSE\Plugins\resources\icons\two_handed_axe.png)" },
        { icon_image_type::staff, R"(.\Data\SKSE\Plugins\resources\icons\staff.png)" },
        { icon_image_type::bow, R"(.\Data\SKSE\Plugins\resources\icons\bow.png)" },
        { icon_image_type::crossbow, R"(.\Data\SKSE\Plugins\resources\icons\crossbow.png)" },
        { icon_image_type::default_spell, R"(.\Data\SKSE\Plugins\resources\icons\default_spell.png)" },
        { icon_image_type::default_spell_alt, R"(.\Data\SKSE\Plugins\resources\icons\default_spell_alt.png)" },
        { icon_image_type::shout, R"(.\Data\SKSE\Plugins\resources\icons\shout.png)" },
        { icon_image_type::power, R"(.\Data\SKSE\Plugins\resources\icons\power.png)" },
        { icon_image_type::food, R"(.\Data\SKSE\Plugins\resources\icons\food.png)" },
        { icon_image_type::shield, R"(.\Data\SKSE\Plugins\resources\icons\shield.png)" },
        { icon_image_type::default_icon, R"(.\Data\SKSE\Plugins\resources\icons\default.png)" }
    };
}