ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

bool property bChangeable Auto
bool property bSpell Auto
bool property bSpellLeft Auto

Event OnConfigClose() native
string function GetResolutionWidth() native
string function GetResolutionHeight() native

string[] function GetSectionNames() native
string function GetPage(int a_index) native
string function GetPosition(int a_index) native
int function GetSelectionType(int a_index, bool a_left) native
string function GetFormString(int a_index, bool a_left) native
int function GetSlotAction(int a_index, bool a_left) native
int function GetHandSelection(int a_index) native
string function GetFormName(int a_index, bool a_left) native
function ResetSection(int a_index) native
function SetActionValue(int a_index, bool a_left, int a_value) native

function ResetSlot()
    ResetSection(GetModSettingInt("uPageList:Page"))
    string[] menu_list = GetSectionNames()
    SetMenuOptions("uPageList:Page", menu_list, menu_list)
    SetModSettingInt("uPageList:Page", 0)
    RefreshMenu()
endfunction

Event OnSettingChange(String a_ID)
    if (a_ID == "uPageList:Page")
        int idx = GetModSettingInt(a_ID)

        SetModSettingString("sPage:Page", GetPage(idx))
        SetModSettingString("sPosition:Page", GetPosition(idx))
        int type = GetSelectionType(idx, false)
        if (type < 0)
            SetModSettingInt("uType:Page", 0)
        else
            SetModSettingInt("uType:Page", type)
        endif
        ;magic, power, scroll, empty (to allow if something should be unequiped)
        bSpell = (type == 1) || (type == 4) || (type == 7) || (type == 8)
        
        SetModSettingInt("uHandSelection:Page", GetHandSelection(idx))
        SetModSettingInt("uSlotAction:Page", GetSlotAction(idx, false))
        SetModSettingString("sFormName:Page", GetFormName(idx, false))
        SetModSettingString("sSelectedItemForm:Page", GetFormString(idx, false))
        
        type = GetSelectionType(idx, true)
        bSpellLeft = (type == 1) || (type == 8)
        if (type < 0)
            SetModSettingInt("uTypeLeft:Page", 0)
        else
            SetModSettingInt("uTypeLeft:Page", type)
        endif
        
        SetModSettingInt("uSlotActionLeft:Page", GetSlotAction(idx, true))
        SetModSettingString("sFormNameLeft:Page", GetFormName(idx, true))
        SetModSettingString("sSelectedItemFormLeft:Page", GetFormString(idx, true))
        
        RefreshMenu()
        
    elseif (a_ID == "uSlotAction:Page")
        int value = GetModSettingInt(a_ID)
        if (value == 2)
            value = 0
        endif
        SetActionValue(GetModSettingInt("uPageList:Page"), False, value)
        RefreshMenu()
    elseif (a_ID == "uSlotActionLeft:Page")
        int value = GetModSettingInt(a_ID)
        if (value == 2)
            value = 0
        endif
        SetActionValue(GetModSettingInt("uPageList:Page"), True, value)
        RefreshMenu()
    endif
EndEvent

Event OnPageSelect(string a_page)
    if ( a_page == "$LamasTinyHUD_Pages")
        string[] menu_list = GetSectionNames()
        SetMenuOptions("uPageList:Page", menu_list, menu_list)
        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent