ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

;Top
Bool Property bMagicSelected Auto
Bool Property bPowerSelected Auto
Bool Property bWeaponSelected Auto
Bool Property bShieldSelected Auto
Bool Property bBothHands Auto
Bool Property bMagicSelectedLeft Auto
Bool Property bPowerSelectedLeft Auto

;Right
Bool Property bMagicSelectedRight Auto
Bool Property bPowerSelectedRight Auto
Bool Property bWeaponSelectedRight Auto
Bool Property bShieldSelectedRight Auto
Bool Property bBothHandsRight Auto
Bool Property bMagicSelectedRightLeft Auto
Bool Property bPowerSelectedRightLeft Auto

;Bottom
Bool Property bMagicSelectedBottom Auto
Bool Property bPowerSelectedBottom Auto
Bool Property bWeaponSelectedBottom Auto
Bool Property bShieldSelectedBottom Auto
Bool Property bBothHandsBottom Auto
Bool Property bMagicSelectedBottomLeft Auto
Bool Property bPowerSelectedBottomLeft Auto

;Left
Bool Property bMagicSelectedLeftSetting Auto
Bool Property bPowerSelectedLeftSetting Auto
Bool Property bWeaponSelectedLeft Auto
Bool Property bShieldSelectedLeft Auto
Bool Property bBothHandsLeft Auto
Bool Property bMagicSelectedLeftLeft Auto
Bool Property bPowerSelectedLeftLeft Auto

Event OnConfigClose() native
string[] function GetSelectedOptions(int a_id, bool a_both, bool a_instant) native
string function GetFormIdForSelection(int a_index) native
string function GetResolutionWidth() native
string function GetResolutionHeight() native

function SetToggles()
    ;Top
    bMagicSelected = GetModSettingInt("uType:TopPage") == 1
    bPowerSelected = GetModSettingInt("uType:TopPage") == 4
    bWeaponSelected = GetModSettingInt("uType:TopPage") == 0
    bShieldSelected = GetModSettingInt("uType:TopPage") == 2
    bBothHands = GetModSettingInt("uHandSelection:TopPage") == 0
    bMagicSelectedLeft = GetModSettingInt("uTypeLeft:TopPage") == 1
    ;is normally not possible
    bPowerSelectedLeft = GetModSettingInt("uTypeLeft:TopPage") == 4
    
    ;Right
    bMagicSelectedRight = GetModSettingInt("uType:RightPage") == 1
    bPowerSelectedRight = GetModSettingInt("uType:RightPage") == 4
    bWeaponSelectedRight = GetModSettingInt("uType:RightPage") == 0
    bShieldSelectedRight = GetModSettingInt("uType:RightPage") == 2
    bBothHandsRight = GetModSettingInt("uHandSelection:RightPage") == 0
    bMagicSelectedRightLeft = GetModSettingInt("uTypeLeft:RightPage") == 1
    ;is normally not possible
    bPowerSelectedRightLeft = GetModSettingInt("uTypeLeft:RightPage") == 4

    ;Bottom
    bMagicSelectedBottom = GetModSettingInt("uType:BottomPage") == 1
    bPowerSelectedBottom = GetModSettingInt("uType:BottomPage") == 4
    bWeaponSelectedBottom = GetModSettingInt("uType:BottomPage") == 0
    bShieldSelectedBottom = GetModSettingInt("uType:BottomPage") == 2
    bBothHandsBottom = GetModSettingInt("uHandSelection:BottomPage") == 0
    bMagicSelectedBottomLeft = GetModSettingInt("uTypeLeft:BottomPage") == 1
    ;is normally not possible
    bPowerSelectedBottomLeft = GetModSettingInt("uTypeLeft:BottomPage") == 4
    
    ;Left
    bMagicSelectedLeftSetting = GetModSettingInt("uType:LeftPage") == 1
    bPowerSelectedLeftSetting = GetModSettingInt("uType:LeftPage") == 4
    bWeaponSelectedLeft = GetModSettingInt("uType:LeftPage") == 0
    bShieldSelectedLeft = GetModSettingInt("uType:LeftPage") == 2
    bBothHandsLeft = GetModSettingInt("uHandSelection:LeftPage") == 0
    bMagicSelectedLeftLeft = GetModSettingInt("uTypeLeft:LeftPage") == 1
    ;is normally not possible
    bPowerSelectedLeftLeft = GetModSettingInt("uTypeLeft:LeftPage") == 4
endfunction

function SetHands(String a_id, String a_mod_set)
    int mod_value = GetModSettingInt(a_ID)
    if (mod_value != 0) || (mod_value != 1)
        SetModSettingInt(a_mod_set, 1)
    endif
endfunction

function SetFromZero(String a_id)
    if(a_ID == "uType:TopPage")
        SetModSettingString("sSelectedItemForm:TopPage", "0")
    elseIf (a_ID == "uTypeLeft:TopPage")
        SetModSettingString("sSelectedItemFormLeft:TopPage", "0")
    endif
    
    if(a_ID == "uType:RightPage")
        SetModSettingString("sSelectedItemForm:RightPage", "0")
    elseIf (a_ID == "uTypeLeft:RightPage")
        SetModSettingString("sSelectedItemFormLeft:RightPage", "0")
    endif

    if(a_ID == "uType:BottomPage")
        SetModSettingString("sSelectedItemForm:BottomPage", "0")
    elseIf (a_ID == "uTypeLeft:BottomPage")
        SetModSettingString("sSelectedItemFormLeft:BottomPage", "0")
    endif

    if(a_ID == "uType:LeftPage")
        SetModSettingString("sSelectedItemForm:LeftPage", "0")
    elseIf (a_ID == "uTypeLeft:LeftPage")
        SetModSettingString("sSelectedItemFormLeft:LeftPage", "0")
    endif
endfunction

function RefreshItemsMain(string a_type, string a_selection, string a_action, bool a_hands, string a_instant_setting)
    SetToggles()
    bool instant_cast = GetModSettingInt(a_instant_setting) == 1
    string[] menu_list = GetSelectedOptions(GetModSettingInt(a_type), a_hands, instant_cast)
    SetMenuOptions(a_selection, menu_list, menu_list)
    SetModSettingInt(a_selection, 0)

    SetModSettingInt(a_action, 0)
    RefreshMenu()
endFunction

function RefreshItems()
    SetToggles()
    RefreshItemsMain("uType:TopPage", "uSelectedItem:TopPage", "uSlotAction:TopPage", bBothHands, "uSlotAction:TopPage")
endfunction

function RefreshItemsLeft()
    SetToggles()
    RefreshItemsMain("uTypeLeft:TopPage", "uSelectedItemLeft:TopPage", "uSlotActionLeft:TopPage", bBothHands, "uSlotActionLeft:TopPage")
endfunction

function RefreshItemsRight()
    RefreshItemsMain("uType:RightPage", "uSelectedItem:RightPage", "uSlotAction:RightPage", bBothHandsRight, "uSlotAction:RightPage")
endfunction

function RefreshItemsRightLeft()
    RefreshItemsMain("uTypeLeft:RightPage", "uSelectedItemLeft:RightPage", "uSlotActionLeft:RightPage", bBothHandsRight, "uSlotActionLeft:RightPage")
endfunction

function RefreshItemsBottom()
    RefreshItemsMain("uType:BottomPage", "uSelectedItem:BottomPage", "uSlotAction:BottomPage", bBothHandsBottom, "uSlotAction:BottomPage")
endfunction

function RefreshItemsBottomLeft()
    RefreshItemsMain("uTypeLeft:BottomPage", "uSelectedItemLeft:BottomPage", "uSlotActionLeft:BottomPage", bBothHandsBottom, "uSlotActionLeft:BottomPage")
endfunction

function RefreshItemsLeftList()
    RefreshItemsMain("uType:LeftPage", "uSelectedItem:LeftPage", "uSlotAction:LeftPage", bBothHandsLeft, "uSlotAction:LeftPage")
endfunction

function RefreshItemsLeftLeft()
    RefreshItemsMain("uTypeLeft:LeftPage", "uSelectedItemLeft:LeftPage", "uSlotActionLeft:LeftPage", bBothHandsLeft, "uSlotActionLeft:LeftPage")
endfunction

Event OnSettingChange(String a_ID)
    ;could also do a refresh of the items on a "uTopType:TopPage" or "uTopTypeLeft:TopPage" setting change
    ;not sure if zero formid setting is nice like that
    if (a_ID == "uSelectedItem:TopPage")
        SetModSettingString("sSelectedItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:TopPage")
        SetModSettingString("sSelectedItemFormLeft:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:TopPage")
        SetFromZero(a_ID)
        SetHands(a_ID, "uHandSelection:TopPage")
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uTypeLeft:TopPage")
        SetFromZero(a_ID)
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uHandSelection:TopPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:RightPage")
        SetModSettingString("sSelectedItemForm:RightPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:RightPage")
        SetModSettingString("sSelectedItemFormLeft:RightPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:RightPage")
        SetFromZero(a_ID)
        SetHands(a_ID, "uHandSelection:RightPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:RightPage")
        SetFromZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:RightPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:BottomPage")
        SetModSettingString("sSelectedItemForm:BottomPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:BottomPage")
        SetModSettingString("sSelectedItemFormLeft:BottomPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:BottomPage")
        SetFromZero(a_ID)
        SetHands(a_ID, "uHandSelection:BottomPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:BottomPage")
        SetFromZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:BottomPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:LeftPage")
        SetModSettingString("sSelectedItemForm:LeftPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:LeftPage")
        SetModSettingString("sSelectedItemFormLeft:LeftPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:LeftPage")
        SetFromZero(a_ID)
        SetHands(a_ID, "uHandSelection:LeftPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:LeftPage")
        SetFromZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:LeftPage")
        SetToggles()
        RefreshMenu()
    endif
EndEvent

Event OnPageSelect(string a_page)
    if (a_page == "$LamasTinyHUD_TopPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:TopPage"), bBothHands, "uSlotAction:TopPage")
        SetMenuOptions("uSelectedItem:TopPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:TopPage"), bBothHands, "uSlotActionLeft:TopPage")
        SetMenuOptions("sSelectedItemFormLeft:TopPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_RightPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:RightPage"), bBothHandsRight, "uSlotAction:RightPage")
        SetMenuOptions("uSelectedItem:RightPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:RightPage"), bBothHandsRight, "uSlotActionLeft:RightPage")
        SetMenuOptions("sSelectedItemFormLeft:RightPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_BottomPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:BottomPage"), bBothHandsBottom, "uSlotAction:BottomPage")
        SetMenuOptions("uSelectedItem:BottomPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:BottomPage"), bBothHandsBottom, "uSlotActionLeft:BottomPage")
        SetMenuOptions("sSelectedItemFormLeft:BottomPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_LeftPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:LeftPage"), bBothHandsLeft, "uSlotAction:LeftPage")
        SetMenuOptions("uSelectedItem:LeftPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:LeftPage"), bBothHandsLeft, "uSlotActionLeft:LeftPage")
        SetMenuOptions("sSelectedItemFormLeft:LeftPage", menu_list, menu_list)

        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent