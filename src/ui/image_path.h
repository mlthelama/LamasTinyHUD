﻿#pragma once

namespace ui {
    constexpr auto hud_name = "lamas_tiny_hud";
    constexpr auto draw_full = 255;

    static std::string icon_directory = R"(.\Data\SKSE\Plugins\resources\icons)";
    static std::string img_directory = R"(.\Data\SKSE\Plugins\resources\img)";
    static std::string highlight_animation_directory = R"(.\Data\SKSE\Plugins\resources\animation\highlight)";

    enum class image_type {
        hud,
        round,
        key,
        total
    };

    inline static std::map<const char*, image_type> image_type_name_map = {
        {  R"(hud.png)", image_type::hud },
        {  R"(round.png)", image_type::round },
        {  R"(key.png)", image_type::key }
    };

    enum class icon_image_type {
        potion_health,
        potion_default,
        sword_one_handed,
        axe_one_handed,
        mace,
        dagger,
        sword_two_handed,
        axe_two_handed,
        staff,
        bow,
        crossbow,
        spell_default,
        destruction,
        shout,
        power,
        food,
        shield,
        icon_default,
        destruction_fire,
        destruction_frost,
        destruction_shock,
        restoration,
        poison_default,
        armor_light,
        armor_heavy,
        armor_clothing,
        scroll,
        arrow,
        hand_to_hand,
        katana,
        halberd,
        whip,
        claw,
        rapier,
        quarter_staff,
        pike,
        potion_stamina,
        potion_magicka,
        potion_fire_resist,
        potion_shock_resist,
        potion_frost_resist,
        potion_magic_resist,
        alteration,
        conjuration,
        illusion,
        torch,
        lantern,
        mask,
        total
    };


    inline static std::map<const char*, icon_image_type> icon_type_name_map = {
        {  R"(potion_health.png)", icon_image_type::potion_health },
        {  R"(potion_default.png)", icon_image_type::potion_default },
        {  R"(sword_one_handed.png)", icon_image_type::sword_one_handed },
        {  R"(axe_one_handed.png)", icon_image_type::axe_one_handed },
        {  R"(mace.png)", icon_image_type::mace },
        {  R"(dagger.png)", icon_image_type::dagger },
        {  R"(sword_two_handed.png)", icon_image_type::sword_two_handed },
        {  R"(axe_two_handed.png)", icon_image_type::axe_two_handed },
        {  R"(staff.png)", icon_image_type::staff },
        {  R"(bow.png)", icon_image_type::bow },
        {  R"(crossbow.png)", icon_image_type::crossbow },
        {  R"(spell_default.png)", icon_image_type::spell_default },
        {  R"(destruction.png)", icon_image_type::destruction },
        {  R"(shout.png)", icon_image_type::shout },
        {  R"(power.png)", icon_image_type::power },
        {  R"(food.png)", icon_image_type::food },
        {  R"(shield.png)", icon_image_type::shield },
        {  R"(icon_default.png)", icon_image_type::icon_default },
        {  R"(destruction_fire.png)", icon_image_type::destruction_fire },
        {  R"(destruction_frost.png)", icon_image_type::destruction_frost },
        {  R"(destruction_shock.png)", icon_image_type::destruction_shock },
        {  R"(restoration.png)", icon_image_type::restoration },
        {  R"(poison_default.png)", icon_image_type::poison_default },
        {  R"(armor_light.png)", icon_image_type::armor_light },
        {  R"(armor_heavy.png)", icon_image_type::armor_heavy },
        {  R"(armor_clothing.png)", icon_image_type::armor_clothing },
        {  R"(scroll.png)", icon_image_type::scroll },
        {  R"(arrow.png)", icon_image_type::arrow },
        {  R"(hand_to_hand.png)", icon_image_type::hand_to_hand },
        {  R"(katana.png)", icon_image_type::katana },
        {  R"(halberd.png)", icon_image_type::halberd },
        {  R"(whip.png)", icon_image_type::whip },
        {  R"(claw.png)", icon_image_type::claw },
        {  R"(rapier.png)", icon_image_type::rapier },
        {  R"(quarter_staff.png)", icon_image_type::quarter_staff },
        {  R"(pike.png)", icon_image_type::pike },
        {  R"(potion_stamina.png)", icon_image_type::potion_stamina },
        {  R"(potion_magicka.png)", icon_image_type::potion_magicka },
        {  R"(potion_fire_resist.png)", icon_image_type::potion_fire_resist },
        {  R"(potion_shock_resist.png)", icon_image_type::potion_shock_resist },
        {  R"(potion_frost_resist.png)", icon_image_type::potion_frost_resist },
        {  R"(potion_magic_resist.png)", icon_image_type::potion_magic_resist },
        {  R"(alteration.png)", icon_image_type::alteration },
        {  R"(conjuration.png)", icon_image_type::conjuration },
        {  R"(illusion.png)", icon_image_type::illusion },
        {  R"(torch.png)", icon_image_type::torch },
        {  R"(lantern.png)", icon_image_type::lantern },
        {  R"(mask.png)", icon_image_type::mask }
    };
}
