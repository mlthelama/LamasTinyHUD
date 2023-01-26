ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

bool property bChangeable Auto
bool property bSpell Auto
bool property bSpellLeft Auto
bool property bElden Auto
bool property bCombat Auto

Event OnConfigClose() native
string function GetResolutionWidth() native
string function GetResolutionHeight() native

string[] function GetSectionNames(int a_position) native
string function GetPage(int a_index, int a_position) native
string function GetPosition(int a_index, int a_position) native
int function GetSelectionType(int a_index, bool a_left, int a_position) native
string function GetFormString(int a_index, bool a_left, int a_position) native
int function GetSlotAction(int a_index, bool a_left, int a_position) native
int function GetHandSelection(int a_index, int a_position) native
string function GetFormName(int a_index, bool a_left, int a_position) native
function ResetSection(int a_index, int a_position) native
function SetActionValue(int a_index, bool a_left, int a_value, int a_position) native
function InitConfigForPosition(int a_position) native

function ResetSlot()
    ResetSection(GetModSettingInt("uPageList:Page"), GetModSettingInt("uPositionSelect:Page"))
    string[] menu_list = GetSectionNames(GetModSettingInt("uPositionSelect:Page"))
    SetMenuOptions("uPageList:Page", menu_list, menu_list)
    SetModSettingInt("uPageList:Page", 0)
    RefreshMenu()
endfunction

function TriggerConfig()
    InitConfigForPosition(GetModSettingInt("uConfigPosition:Controls"))
    RefreshMenu()
endfunction

Event OnSettingChange(String a_ID)
    if (a_ID == "uPageList:Page")
        int idx = GetModSettingInt(a_ID)
        int position = GetModSettingInt("uPositionSelect:Page")

        SetModSettingString("sPage:Page", GetPage(idx, position))
        SetModSettingString("sPosition:Page", GetPosition(idx, position))
        int type = GetSelectionType(idx, false, position)
        if (type < 0)
            SetModSettingInt("uType:Page", 0)
        else
            SetModSettingInt("uType:Page", type)
        endif
        ;magic, power, scroll, empty (to allow if something should be unequiped)
        bSpell = (type == 1) || (type == 4) || (type == 7) || (type == 8)
        
        SetModSettingInt("uHandSelection:Page", GetHandSelection(idx, position))
        SetModSettingInt("uSlotAction:Page", GetSlotAction(idx, false, position))
        SetModSettingString("sFormName:Page", GetFormName(idx, false, position))
        SetModSettingString("sSelectedItemForm:Page", GetFormString(idx, false, position))
        
        type = GetSelectionType(idx, true, position)
        bSpellLeft = (type == 1) || (type == 8)
        if (type < 0)
            SetModSettingInt("uTypeLeft:Page", 0)
        else
            SetModSettingInt("uTypeLeft:Page", type)
        endif
        
        SetModSettingInt("uSlotActionLeft:Page", GetSlotAction(idx, true, position))
        SetModSettingString("sFormNameLeft:Page", GetFormName(idx, true, position))
        SetModSettingString("sSelectedItemFormLeft:Page", GetFormString(idx, true, position))
        
        RefreshMenu()
        
    elseif (a_ID == "uSlotAction:Page")
        int value = GetModSettingInt(a_ID)
        SetActionValue(GetModSettingInt("uPageList:Page"), False, value, GetModSettingInt("uPositionSelect:Page"))
        RefreshMenu()
    elseif (a_ID == "uSlotActionLeft:Page")
        int value = GetModSettingInt(a_ID)
        SetActionValue(GetModSettingInt("uPageList:Page"), True, value, GetModSettingInt("uPositionSelect:Page"))
        RefreshMenu()
    elseif (a_ID == "bEldenDemonSouls:MiscSetting")
        bElden = GetModSettingBool(a_ID)
        RefreshMenu()
    elseif (a_ID == "uPositionSelect:Page")
        string[] menu_list = GetSectionNames(GetModSettingInt(a_ID))
        SetMenuOptions("uPageList:Page", menu_list, menu_list)
        SetModSettingInt("uPageList:Page", 0)
        RefreshMenu()
    elseif (a_ID == "bHideOutsideCombat:MiscSetting")
        bCombat = GetModSettingBool(a_ID)
        RefreshMenu()
    endif
EndEvent

Event OnPageSelect(string a_page)
    if ( a_page == "$LamasTinyHUD_Pages")
        string[] menu_list = GetSectionNames(GetModSettingInt("uPositionSelect:Page"))
        SetMenuOptions("uPageList:Page", menu_list, menu_list)
        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent

Event OnConfigOpen()
    bElden = GetModSettingBool("bEldenDemonSouls:MiscSetting")
    bCombat = GetModSettingBool("bHideOutsideCombat:MiscSetting")
EndEvent