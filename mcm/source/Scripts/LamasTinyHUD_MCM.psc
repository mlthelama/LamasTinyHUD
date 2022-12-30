ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

Event OnConfigClose() native

function RefreshItems()
    string[] menu_list = GetSelectedOptions(GetModSettingInt("uTopType:TopPage"))
    SetMenuOptions("uTopSelectedItem:TopPage", menu_list, menu_list)
    SetModSettingInt("uTopSelectedItem:TopPage", 0)
    RefreshMenu()
endfunction

string[] function GetSelectedOptions(int a_id) native

int function GetFormIdForSelection(int a_index) native

Event OnSettingChange(String a_ID)
    if (a_ID == "uTopSelectedItem:TopPage")
        SetModSettingString("sSelectedTopItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    endif
EndEvent

string function GetResolutionWidth() native
string function GetResolutionHeight() native

Event OnPageSelect(string a_page)
    if (a_page == "$LamasTinyHUD_TopPage")
        ;no need to set uTopSelectedItem:TopPage here
        string[] menu_list = GetSelectedOptions(GetModSettingInt("uTopType:TopPage"))
        SetMenuOptions("uTopSelectedItem:TopPage", menu_list, menu_list)
        RefreshMenu()
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
        RefreshMenu()
    endIf
EndEvent