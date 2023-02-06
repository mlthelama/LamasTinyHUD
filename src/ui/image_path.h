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

    static std::list highlight_animation_file_paths = {
        R"(.\Data\SKSE\Plugins\resources\animation\highlight\frame1.png)",
        R"(.\Data\SKSE\Plugins\resources\animation\highlight\frame2.png)",
        R"(.\Data\SKSE\Plugins\resources\animation\highlight\frame3.png)"
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
        destruction = 12,
        shout = 13,
        power = 14,
        food = 15,
        shield = 16,
        icon_default = 17,
        destruction_fire = 18,
        destruction_frost = 19,
        destruction_shock = 20,
        restoration = 21,
        poison_default = 22,
        armor_light = 23,
        armor_heavy = 24,
        armor_clothing = 25,
        scroll = 26,
        arrow = 27,
        hand_to_hand = 28,
        katana = 29,
        halberd = 30,
        whip = 31,
        claw = 32,
        rapier = 33,
        quarter_staff = 34,
        pike = 35,
        potion_stamina = 36,
        potion_magicka = 37,
        potion_fire_resist = 38,
        potion_shock_resist = 39,
        potion_frost_resist = 40,
        potion_magic_resist = 41,
        alteration = 42,
        conjuration = 43,
        illusion = 44,
        torch = 45,
        lantern = 46,
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
        { icon_image_type::destruction, R"(.\Data\SKSE\Plugins\resources\icons\destruction.png)" },
        { icon_image_type::shout, R"(.\Data\SKSE\Plugins\resources\icons\shout.png)" },
        { icon_image_type::power, R"(.\Data\SKSE\Plugins\resources\icons\power.png)" },
        { icon_image_type::food, R"(.\Data\SKSE\Plugins\resources\icons\food.png)" },
        { icon_image_type::shield, R"(.\Data\SKSE\Plugins\resources\icons\shield.png)" },
        { icon_image_type::icon_default, R"(.\Data\SKSE\Plugins\resources\icons\default.png)" },
        { icon_image_type::destruction_fire, R"(.\Data\SKSE\Plugins\resources\icons\destruction_fire.png)" },
        { icon_image_type::destruction_frost, R"(.\Data\SKSE\Plugins\resources\icons\destruction_frost.png)" },
        { icon_image_type::destruction_shock, R"(.\Data\SKSE\Plugins\resources\icons\destruction_shock.png)" },
        { icon_image_type::restoration, R"(.\Data\SKSE\Plugins\resources\icons\restoration.png)" },
        { icon_image_type::poison_default, R"(.\Data\SKSE\Plugins\resources\icons\poison_default.png)" },
        { icon_image_type::armor_light, R"(.\Data\SKSE\Plugins\resources\icons\armor_light.png)" },
        { icon_image_type::armor_heavy, R"(.\Data\SKSE\Plugins\resources\icons\armor_heavy.png)" },
        { icon_image_type::armor_clothing, R"(.\Data\SKSE\Plugins\resources\icons\armor_clothing.png)" },
        { icon_image_type::scroll, R"(.\Data\SKSE\Plugins\resources\icons\scroll.png)" },
        { icon_image_type::arrow, R"(.\Data\SKSE\Plugins\resources\icons\arrow.png)" },
        { icon_image_type::hand_to_hand, R"(.\Data\SKSE\Plugins\resources\icons\handtohand.png)" },
        { icon_image_type::katana, R"(.\Data\SKSE\Plugins\resources\icons\katana.png)" },
        { icon_image_type::halberd, R"(.\Data\SKSE\Plugins\resources\icons\halberd.png)" },
        { icon_image_type::whip, R"(.\Data\SKSE\Plugins\resources\icons\whip.png)" },
        { icon_image_type::claw, R"(.\Data\SKSE\Plugins\resources\icons\claw.png)" },
        { icon_image_type::rapier, R"(.\Data\SKSE\Plugins\resources\icons\rapier.png)" },
        { icon_image_type::quarter_staff, R"(.\Data\SKSE\Plugins\resources\icons\quarter_staff.png)" },
        { icon_image_type::pike, R"(.\Data\SKSE\Plugins\resources\icons\pike.png)" },
        { icon_image_type::potion_stamina, R"(.\Data\SKSE\Plugins\resources\icons\potion_stamina.png)" },
        { icon_image_type::potion_magicka, R"(.\Data\SKSE\Plugins\resources\icons\potion_magicka.png)" },
        { icon_image_type::potion_fire_resist, R"(.\Data\SKSE\Plugins\resources\icons\potion_fire_resist.png)" },
        { icon_image_type::potion_shock_resist, R"(.\Data\SKSE\Plugins\resources\icons\potion_shock_resist.png)" },
        { icon_image_type::potion_frost_resist, R"(.\Data\SKSE\Plugins\resources\icons\potion_frost_resist.png)" },
        { icon_image_type::potion_magic_resist, R"(.\Data\SKSE\Plugins\resources\icons\potion_magic_resist.png)" },
        { icon_image_type::alteration, R"(.\Data\SKSE\Plugins\resources\icons\alteration.png)" },
        { icon_image_type::conjuration, R"(.\Data\SKSE\Plugins\resources\icons\conjuration.png)" },
        { icon_image_type::illusion, R"(.\Data\SKSE\Plugins\resources\icons\illusion.png)" },
        { icon_image_type::torch, R"(.\Data\SKSE\Plugins\resources\icons\torch.png)" },
        { icon_image_type::lantern, R"(.\Data\SKSE\Plugins\resources\icons\lantern.png)" }
    };
}
