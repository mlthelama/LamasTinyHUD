ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

Bool Property bMagicSelected Auto
Bool Property bPowerSelected Auto
Bool Property bWeaponSelected Auto
Bool Property bBothHands Auto

Event OnConfigClose() native
string[] function GetSelectedOptions(int a_id, bool a_both) native
int function GetFormIdForSelection(int a_index) native
string function GetResolutionWidth() native
string function GetResolutionHeight() native

function SetToggles()
    bMagicSelected = GetModSettingInt("uTopType:TopPage") == 1
    bPowerSelected = GetModSettingInt("uTopType:TopPage") == 3
    bWeaponSelected = GetModSettingInt("uTopType:TopPage") == 4
    bBothHands = GetModSettingInt("uTopHandSelection:TopPage") == 1
endfunction

function RefreshItems()
    SetToggles()
    string[] menu_list = GetSelectedOptions(GetModSettingInt("uTopType:TopPage"), bBothHands)
    SetMenuOptions("uTopSelectedItem:TopPage", menu_list, menu_list)
    SetModSettingInt("uTopSelectedItem:TopPage", 0)

    ;SetToggles()
    SetModSettingInt("uTopSlotAction:TopPage", 0)
    ;SetModSettingInt("uTopHandSelection:TopPage", 0)
    RefreshMenu()
endfunction

Event OnSettingChange(String a_ID)
    if (a_ID == "uTopSelectedItem:TopPage")
        SetModSettingString("sSelectedTopItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    endif
EndEvent

Event OnPageSelect(string a_page)
    if (a_page == "$LamasTinyHUD_TopPage")
        ;bBothHands = GetModSettingInt("uTopHandSelection:TopPage") == 2
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uTopType:TopPage"), bBothHands)
        SetMenuOptions("uTopSelectedItem:TopPage", menu_list, menu_list)

        ;SetToggles()
        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent