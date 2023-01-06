#pragma once

namespace ui {
    constexpr auto hud_name = "lamas_tiny_hud";

    constexpr auto draw_full = 255;

    constexpr auto hud_image_path = R"(.\Data\SKSE\Plugins\img\hud.png)";
    constexpr auto round_image_path = R"(.\Data\SKSE\Plugins\img\round.png)";
    constexpr auto key_image_path = R"(.\Data\SKSE\Plugins\img\key.png)";

    enum class image_type { hud = 0, round = 1, key = 2, total = 3 };

    inline static std::map<image_type, const char*> image_type_path_map = { { image_type::hud, hud_image_path },
                                                                            { image_type::round, round_image_path },
                                                                            { image_type::key, key_image_path } };


    constexpr auto health_potion_image_path = R"(.\Data\SKSE\Plugins\icons\health_potion.png)";
    constexpr auto default_potion_image_path = R"(.\Data\SKSE\Plugins\icons\default_potion.png)";

    constexpr auto one_handed_sword_image_path = R"(.\Data\SKSE\Plugins\icons\one_handed_sword.png)";
    constexpr auto one_handed_axe_image_path = R"(.\Data\SKSE\Plugins\icons\one_handed_axe.png)";
    constexpr auto mace_image_path = R"(.\Data\SKSE\Plugins\icons\mace.png)";
    constexpr auto dagger_image_path = R"(.\Data\SKSE\Plugins\icons\dagger.png)";
    constexpr auto two_handed_sword_image_path = R"(.\Data\SKSE\Plugins\icons\two_handed_sword.png)";
    constexpr auto two_handed_axe_image_path = R"(.\Data\SKSE\Plugins\icons\two_handed_axe.png)";
    constexpr auto staff_image_path = R"(.\Data\SKSE\Plugins\icons\staff.png)";
    constexpr auto bow_image_path = R"(.\Data\SKSE\Plugins\icons\bow.png)";
    constexpr auto crossbow_image_path = R"(.\Data\SKSE\Plugins\icons\crossbow.png)";

    constexpr auto fire_spell_image_path = R"(.\Data\SKSE\Plugins\icons\fire_spell.png)";
    constexpr auto ice_spell_image_path = R"(.\Data\SKSE\Plugins\icons\ice_spell.png)";
    constexpr auto shock_spell_image_path = R"(.\Data\SKSE\Plugins\icons\shock_spell.png)";
    constexpr auto healing_spell_image_path = R"(.\Data\SKSE\Plugins\icons\heal_spell.png)";
    constexpr auto default_spell_image_path = R"(.\Data\SKSE\Plugins\icons\default_spell.png)";

    constexpr auto shout_image_path = R"(.\Data\SKSE\Plugins\icons\shout.png)";

    constexpr auto power_image_path = R"(.\Data\SKSE\Plugins\icons\power.png)";

    constexpr auto default_image_path = R"(.\Data\SKSE\Plugins\icons\default.png)";

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
        fire_spell = 11,
        ice_spell = 12,
        shock_spell = 13,
        healing_spell = 14,
        default_spell = 15,
        shout = 16,
        power = 17,
        default_icon = 18,
        total
    };

    inline static std::map<icon_image_type, const char*> icon_type_path_map = {
        { icon_image_type::health_potion, health_potion_image_path },
        { icon_image_type::default_potion, default_potion_image_path },
        { icon_image_type::one_handed_sword, one_handed_sword_image_path },
        { icon_image_type::one_handed_axe, one_handed_axe_image_path },
        { icon_image_type::mace, mace_image_path },
        { icon_image_type::dagger, dagger_image_path },
        { icon_image_type::two_handed_sword, two_handed_sword_image_path },
        { icon_image_type::two_handed_axe, two_handed_axe_image_path },
        { icon_image_type::staff, staff_image_path },
        { icon_image_type::bow, bow_image_path },
        { icon_image_type::crossbow, crossbow_image_path },
        { icon_image_type::fire_spell, fire_spell_image_path },
        { icon_image_type::ice_spell, ice_spell_image_path },
        { icon_image_type::shock_spell, shock_spell_image_path },
        { icon_image_type::healing_spell, healing_spell_image_path },
        { icon_image_type::default_spell, default_spell_image_path },
        { icon_image_type::shout, shout_image_path },
        { icon_image_type::power, power_image_path },
        { icon_image_type::default_icon, default_image_path }
    };
}
