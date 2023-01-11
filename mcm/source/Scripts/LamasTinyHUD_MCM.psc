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

;Top
Bool Property bMagicSelectedSetOne Auto
Bool Property bPowerSelectedSetOne Auto
Bool Property bWeaponSelectedSetOne Auto
Bool Property bShieldSelectedSetOne Auto
Bool Property bBothHandsSetOne Auto
Bool Property bMagicSelectedLeftSetOne Auto
Bool Property bPowerSelectedLeftSetOne Auto

;Right
Bool Property bMagicSelectedRightSetOne Auto
Bool Property bPowerSelectedRightSetOne Auto
Bool Property bWeaponSelectedRightSetOne Auto
Bool Property bShieldSelectedRightSetOne Auto
Bool Property bBothHandsRightSetOne Auto
Bool Property bMagicSelectedRightLeftSetOne Auto
Bool Property bPowerSelectedRightLeftSetOne Auto

;Bottom
Bool Property bMagicSelectedBottomSetOne Auto
Bool Property bPowerSelectedBottomSetOne Auto
Bool Property bWeaponSelectedBottomSetOne Auto
Bool Property bShieldSelectedBottomSetOne Auto
Bool Property bBothHandsBottomSetOne Auto
Bool Property bMagicSelectedBottomLeftSetOne Auto
Bool Property bPowerSelectedBottomLeftSetOne Auto

;Left
Bool Property bMagicSelectedLeftSettingSetOne Auto
Bool Property bPowerSelectedLeftSettingSetOne Auto
Bool Property bWeaponSelectedLeftSetOne Auto
Bool Property bShieldSelectedLeftSetOne Auto
Bool Property bBothHandsLeftSetOne Auto
Bool Property bMagicSelectedLeftLeftSetOne Auto
Bool Property bPowerSelectedLeftLeftSetOne Auto

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
    
    ;Top
    bMagicSelectedSetOne = GetModSettingInt("uType:TopPageSetOne") == 1
    bPowerSelectedSetOne = GetModSettingInt("uType:TopPageSetOne") == 4
    bWeaponSelectedSetOne = GetModSettingInt("uType:TopPageSetOne") == 0
    bShieldSelectedSetOne = GetModSettingInt("uType:TopPageSetOne") == 2
    bBothHandsSetOne = GetModSettingInt("uHandSelection:TopPageSetOne") == 0
    bMagicSelectedLeftSetOne = GetModSettingInt("uTypeLeft:TopPageSetOne") == 1
    ;is normally not possible
    bPowerSelectedLeftSetOne = GetModSettingInt("uTypeLeft:TopPageSetOne") == 4
    
    ;Right
    bMagicSelectedRightSetOne = GetModSettingInt("uType:RightPageSetOne") == 1
    bPowerSelectedRightSetOne = GetModSettingInt("uType:RightPageSetOne") == 4
    bWeaponSelectedRightSetOne = GetModSettingInt("uType:RightPageSetOne") == 0
    bShieldSelectedRightSetOne = GetModSettingInt("uType:RightPageSetOne") == 2
    bBothHandsRightSetOne = GetModSettingInt("uHandSelection:RightPageSetOne") == 0
    bMagicSelectedRightLeftSetOne = GetModSettingInt("uTypeLeft:RightPageSetOne") == 1
    ;is normally not possible
    bPowerSelectedRightLeftSetOne = GetModSettingInt("uTypeLeft:RightPageSetOne") == 4

    ;Bottom
    bMagicSelectedBottomSetOne = GetModSettingInt("uType:BottomPageSetOne") == 1
    bPowerSelectedBottomSetOne = GetModSettingInt("uType:BottomPageSetOne") == 4
    bWeaponSelectedBottomSetOne = GetModSettingInt("uType:BottomPageSetOne") == 0
    bShieldSelectedBottomSetOne = GetModSettingInt("uType:BottomPageSetOne") == 2
    bBothHandsBottomSetOne = GetModSettingInt("uHandSelection:BottomPageSetOne") == 0
    bMagicSelectedBottomLeftSetOne = GetModSettingInt("uTypeLeft:BottomPageSetOne") == 1
    ;is normally not possible
    bPowerSelectedBottomLeftSetOne = GetModSettingInt("uTypeLeft:BottomPageSetOne") == 4
    
    ;Left
    bMagicSelectedLeftSettingSetOne = GetModSettingInt("uType:LeftPageSetOne") == 1
    bPowerSelectedLeftSettingSetOne = GetModSettingInt("uType:LeftPageSetOne") == 4
    bWeaponSelectedLeftSetOne = GetModSettingInt("uType:LeftPageSetOne") == 0
    bShieldSelectedLeftSetOne = GetModSettingInt("uType:LeftPageSetOne") == 2
    bBothHandsLeftSetOne = GetModSettingInt("uHandSelection:LeftPageSetOne") == 0
    bMagicSelectedLeftLeftSetOne = GetModSettingInt("uTypeLeft:LeftPageSetOne") == 1
    ;is normally not possible
    bPowerSelectedLeftLeftSetOne = GetModSettingInt("uTypeLeft:LeftPageSetOne") == 4
endfunction

function SetHands(String a_id, String a_mod_set)
    int mod_value = GetModSettingInt(a_ID)
    if (mod_value != 0) || (mod_value != 1)
        SetModSettingInt(a_mod_set, 1)
    endif
endfunction

function SetFormZero(String a_id)
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

    if(a_ID == "uType:TopPageSet")
        SetModSettingString("sSelectedItemForm:TopPageSetOne", "0")
    elseIf (a_ID == "uTypeLeft:TopPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:TopPageSetOne", "0")
    endif
    
    if(a_ID == "uType:RightPageSetOne")
        SetModSettingString("sSelectedItemForm:RightPageSetOne", "0")
    elseIf (a_ID == "uTypeLeft:RightPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:RightPageSetOne", "0")
    endif

    if(a_ID == "uType:BottomPageSetOne")
        SetModSettingString("sSelectedItemForm:BottomPageSetOne", "0")
    elseIf (a_ID == "uTypeLeft:BottomPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:BottomPageSetOne", "0")
    endif

    if(a_ID == "uType:LeftPageSetOne")
        SetModSettingString("sSelectedItemForm:LeftPageSetOne", "0")
    elseIf (a_ID == "uTypeLeft:LeftPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:LeftPageSetOne", "0")
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

function RefreshItemsPageOne()
    SetToggles()
    RefreshItemsMain("uType:TopPageSetOne", "uSelectedItem:TopPageSetOne", "uSlotAction:TopPageSetOne", bBothHandsSetOne, "uSlotAction:TopPageSetOne")
endfunction

function RefreshItemsLeftPageOne()
    SetToggles()
    RefreshItemsMain("uTypeLeft:TopPageSetOne", "uSelectedItemLeft:TopPageSetOne", "uSlotActionLeft:TopPageSetOne", bBothHandsSetOne, "uSlotActionLeft:TopPageSetOne")
endfunction

function RefreshItemsRightPageOne()
    RefreshItemsMain("uType:RightPageSetOne", "uSelectedItem:RightPageSetOne", "uSlotAction:RightPageSetOne", bBothHandsRightSetOne, "uSlotAction:RightPageSetOne")
endfunction

function RefreshItemsRightLeftPageOne()
    RefreshItemsMain("uTypeLeft:RightPageSetOne", "uSelectedItemLeft:RightPageSetOne", "uSlotActionLeft:RightPageSetOne", bBothHandsRightSetOne, "uSlotActionLeft:RightPageSetOne")
endfunction

function RefreshItemsBottomPageOne()
    RefreshItemsMain("uType:BottomPageSetOne", "uSelectedItem:BottomPageSetOne", "uSlotAction:BottomPageSetOne", bBothHandsBottomSetOne, "uSlotAction:BottomPageSetOne")
endfunction

function RefreshItemsBottomLeftPageOne()
    RefreshItemsMain("uTypeLeft:BottomPageSetOne", "uSelectedItemLeft:BottomPageSetOne", "uSlotActionLeft:BottomPageSetOne", bBothHandsBottomSetOne, "uSlotActionLeft:BottomPageSetOne")
endfunction

function RefreshItemsLeftListPageOne()
    RefreshItemsMain("uType:LeftPageSetOne", "uSelectedItem:LeftPageSetOne", "uSlotAction:LeftPageSetOne", bBothHandsLeftSetOne, "uSlotAction:LeftPageSetOne")
endfunction

function RefreshItemsLeftLeftPageOne()
    RefreshItemsMain("uTypeLeft:LeftPageSetOne", "uSelectedItemLeft:LeftPageSetOne", "uSlotActionLeft:LeftPageSetOne", bBothHandsLeftSetOne, "uSlotActionLeft:LeftPageSetOne")
endfunction

Event OnSettingChange(String a_ID)
    ;could also do a refresh of the items on a "uTopType:TopPage" or "uTopTypeLeft:TopPage" setting change
    ;not sure if zero formid setting is nice like that
    if (a_ID == "uSelectedItem:TopPage")
        SetModSettingString("sSelectedItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHands) 
            SetModSettingString("sSelectedItemFormLeft:TopPage", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:TopPage")
        SetModSettingString("sSelectedItemFormLeft:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:TopPage")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:TopPage")
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uTypeLeft:TopPage")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uHandSelection:TopPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:RightPage")
        SetModSettingString("sSelectedItemForm:RightPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsRight) 
            SetModSettingString("sSelectedItemFormLeft:RightPage", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:RightPage")
        SetModSettingString("sSelectedItemFormLeft:RightPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:RightPage")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:RightPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:RightPage")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:RightPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:BottomPage")
        SetModSettingString("sSelectedItemForm:BottomPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsBottom) 
            SetModSettingString("sSelectedItemFormLeft:BottomPage", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:BottomPage")
        SetModSettingString("sSelectedItemFormLeft:BottomPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:BottomPage")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:BottomPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:BottomPage")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:BottomPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:LeftPage")
        SetModSettingString("sSelectedItemForm:LeftPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsLeft) 
            SetModSettingString("sSelectedItemFormLeft:LeftPage", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:LeftPage")
        SetModSettingString("sSelectedItemFormLeft:LeftPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:LeftPage")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:LeftPage")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:LeftPage")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:LeftPage")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:TopPageSetOne")
        SetModSettingString("sSelectedItemForm:TopPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsSetOne) 
            SetModSettingString("sSelectedItemFormLeft:TopPageSetOne", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:TopPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:TopPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:TopPageSetOne")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:TopPageSetOne")
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uTypeLeft:TopPageSetOne")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
     elseif (a_ID == "uHandSelection:TopPageSetOne")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:RightPageSetOne")
        SetModSettingString("sSelectedItemForm:RightPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsRightSetOne) 
            SetModSettingString("sSelectedItemFormLeft:RightPageSetOne", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:RightPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:RightPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:RightPageSetOne")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:RightPageSetOne")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:RightPageSetOne")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:RightPageSetOne")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:BottomPageSetOne")
        SetModSettingString("sSelectedItemForm:BottomPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsBottomSetOne) 
            SetModSettingString("sSelectedItemFormLeft:BottomPageSetOne", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:BottomPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:BottomPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:BottomPageSetOne")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:BottomPageSetOne")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:BottomPageSetOne")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:BottomPageSetOne")
        SetToggles()
        RefreshMenu()
    endif

    if (a_ID == "uSelectedItem:LeftPageSetOne")
        SetModSettingString("sSelectedItemForm:LeftPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        
        if (!bBothHandsLeftSetOne) 
            SetModSettingString("sSelectedItemFormLeft:LeftPageSetOne", "0")
        endif
        
        RefreshMenu()
    elseIf (a_ID == "uSelectedItemLeft:LeftPageSetOne")
        SetModSettingString("sSelectedItemFormLeft:LeftPageSetOne", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    elseif (a_ID == "uType:LeftPageSetOne")
        SetFormZero(a_ID)
        SetHands(a_ID, "uHandSelection:LeftPageSetOne")
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uTypeLeft:LeftPageSetOne")
        SetFormZero(a_ID)
        SetToggles()
        RefreshMenu()
    elseif (a_ID == "uHandSelection:LeftPageSetOne")
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
        SetMenuOptions("uSelectedItemLeft:TopPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_RightPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:RightPage"), bBothHandsRight, "uSlotAction:RightPage")
        SetMenuOptions("uSelectedItem:RightPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:RightPage"), bBothHandsRight, "uSlotActionLeft:RightPage")
        SetMenuOptions("uSelectedItemLeft:RightPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_BottomPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:BottomPage"), bBothHandsBottom, "uSlotAction:BottomPage")
        SetMenuOptions("uSelectedItem:BottomPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:BottomPage"), bBothHandsBottom, "uSlotActionLeft:BottomPage")
        SetMenuOptions("uSelectedItemLeft:BottomPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_LeftPage")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:LeftPage"), bBothHandsLeft, "uSlotAction:LeftPage")
        SetMenuOptions("uSelectedItem:LeftPage", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:LeftPage"), bBothHandsLeft, "uSlotActionLeft:LeftPage")
        SetMenuOptions("uSelectedItemLeft:LeftPage", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_TopPageSetOne")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPageSetOne here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:TopPageSetOne"), bBothHandsSetOne, "uSlotAction:TopPageSetOne")
        SetMenuOptions("uSelectedItem:TopPageSetOne", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:TopPageSetOne"), bBothHandsSetOne, "uSlotActionLeft:TopPageSetOne")
        SetMenuOptions("uSelectedItemLeft:TopPageSetOne", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_RightPageSetOne")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPageSetOne here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:RightPageSetOne"), bBothHandsRightSetOne, "uSlotAction:RightPageSetOne")
        SetMenuOptions("uSelectedItem:RightPageSetOne", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:RightPageSetOne"), bBothHandsRightSetOne, "uSlotActionLeft:RightPageSetOne")
        SetMenuOptions("uSelectedItemLeft:RightPageSetOne", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_BottomPageSetOne")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPageSetOne here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:BottomPageSetOne"), bBothHandsBottomSetOne, "uSlotAction:BottomPageSetOne")
        SetMenuOptions("uSelectedItem:BottomPageSetOne", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:BottomPageSetOne"), bBothHandsBottomSetOne, "uSlotActionLeft:BottomPageSetOne")
        SetMenuOptions("uSelectedItemLeft:BottomPageSetOne", menu_list, menu_list)

        RefreshMenu()
    elseif (a_page == "$LamasTinyHUD_LeftPageSetOne")
        SetToggles()
        ;no need to set uTopSelectedItem:TopPageSetOne here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uType:LeftPageSetOne"), bBothHandsLeftSetOne, "uSlotAction:LeftPageSetOne")
        SetMenuOptions("uSelectedItem:LeftPageSetOne", menu_list, menu_list)

        menu_list = GetSelectedOptions(GetModSettingInt("uTypeLeft:LeftPageSetOne"), bBothHandsLeftSetOne, "uSlotActionLeft:LeftPageSetOne")
        SetMenuOptions("uSelectedItemLeft:LeftPageSetOne", menu_list, menu_list)

        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent