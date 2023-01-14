#pragma once

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
        potion_health = 0,
        potion_default = 1,
        sword_one_handed = 2,
        axe_one_handed = 3,
        mace = 4,
        dagger = 5,
        sword_two_handed = 6,
        axe_two_handed = 7,
        staff = 8,
        bow = 9,
        crossbow = 10,
        spell_default = 11,
        spell_default_alt = 12,
        shout = 13,
        power = 14,
        food = 15,
        shield = 16,
        icon_default = 17,
        spell_fire = 18,
        spell_frost = 19,
        spell_shock = 20,
        spell_heal = 21,
        poison_default = 22,
        armor_light = 23,
        armor_heavy = 24,
        armor_clothing = 25,
        scroll = 26,
        total
    };


    inline static std::map<icon_image_type, const char*> icon_type_path_map = {
        { icon_image_type::potion_health, R"(.\Data\SKSE\Plugins\resources\icons\health_potion.png)" },
        { icon_image_type::potion_default, R"(.\Data\SKSE\Plugins\resources\icons\default_potion.png)" },
        { icon_image_type::sword_one_handed, R"(.\Data\SKSE\Plugins\resources\icons\one_handed_sword.png)" },
        { icon_image_type::axe_one_handed, R"(.\Data\SKSE\Plugins\resources\icons\one_handed_axe.png)" },
        { icon_image_type::mace, R"(.\Data\SKSE\Plugins\resources\icons\mace.png)" },
        { icon_image_type::dagger, R"(.\Data\SKSE\Plugins\resources\icons\dagger.png)" },
        { icon_image_type::sword_two_handed, R"(.\Data\SKSE\Plugins\resources\icons\two_handed_sword.png)" },
        { icon_image_type::axe_two_handed, R"(.\Data\SKSE\Plugins\resources\icons\two_handed_axe.png)" },
        { icon_image_type::staff, R"(.\Data\SKSE\Plugins\resources\icons\staff.png)" },
        { icon_image_type::bow, R"(.\Data\SKSE\Plugins\resources\icons\bow.png)" },
        { icon_image_type::crossbow, R"(.\Data\SKSE\Plugins\resources\icons\crossbow.png)" },
        { icon_image_type::spell_default, R"(.\Data\SKSE\Plugins\resources\icons\default_spell.png)" },
        { icon_image_type::spell_default_alt, R"(.\Data\SKSE\Plugins\resources\icons\default_spell_alt.png)" },
        { icon_image_type::shout, R"(.\Data\SKSE\Plugins\resources\icons\shout.png)" },
        { icon_image_type::power, R"(.\Data\SKSE\Plugins\resources\icons\power.png)" },
        { icon_image_type::food, R"(.\Data\SKSE\Plugins\resources\icons\food.png)" },
        { icon_image_type::shield, R"(.\Data\SKSE\Plugins\resources\icons\shield.png)" },
        { icon_image_type::icon_default, R"(.\Data\SKSE\Plugins\resources\icons\default.png)" },
        { icon_image_type::spell_fire, R"(.\Data\SKSE\Plugins\resources\icons\fire_spell.png)" },
        { icon_image_type::spell_frost, R"(.\Data\SKSE\Plugins\resources\icons\ice_spell.png)" },
        { icon_image_type::spell_shock, R"(.\Data\SKSE\Plugins\resources\icons\shock_spell.png)" },
        { icon_image_type::spell_heal, R"(.\Data\SKSE\Plugins\resources\icons\heal_spell.png)" },
        { icon_image_type::poison_default, R"(.\Data\SKSE\Plugins\resources\icons\poison_default.png)" },
        { icon_image_type::armor_light, R"(.\Data\SKSE\Plugins\resources\icons\armor_light.png)" },
        { icon_image_type::armor_heavy, R"(.\Data\SKSE\Plugins\resources\icons\armor_heavy.png)" },
        { icon_image_type::armor_clothing, R"(.\Data\SKSE\Plugins\resources\icons\armor_clothing.png)" },
        { icon_image_type::scroll, R"(.\Data\SKSE\Plugins\resources\icons\scroll.png)" }
    };
}
