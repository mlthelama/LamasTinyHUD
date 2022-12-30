ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

Event OnConfigClose() native

function RefreshItems()
    ;RefreshMenu()
    SetMenuOptions("uTopSelectedItem:TopPage", GetSelectedOptions(GetModSettingInt("uTopType:TopPage")))
    ;RefreshMenu()
endfunction

string[] function GetSelectedOptions(int a_id) native

int function GetFormIdForSelection(int a_index) native

Event OnSettingChange(String a_ID)
    if (a_ID == "uTopSelectedItem:TopPage")
        SetModSettingString("sSelectedTopItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
    ;elseif (a_ID == "uTopType:TopPage")
    ;    SetMenuOptions("uTopSelectedItem:TopPage", GetSelectedOptions("uTopType:TopPage"))
    endif
    ;RefreshMenu()
EndEvent

;Event OnConfigOpen()
;    RefreshItems()
;EndEvent

string function GetResolutionWidth() native
string function GetResolutionHeight() native

Event OnPageSelect(string a_page)
    if (a_page == "$LamasTinyHUD_TopPage")
        SetMenuOptions("uTopSelectedItem:TopPage", GetSelectedOptions(GetModSettingInt("uTopType:TopPage")))
    elseif ( a_page == "$LamasTinyHUD_HudSetting" )
        SetModSettingString("sDisplayResolutionWidth:HudSetting",GetResolutionWidth())
        SetModSettingString("sDisplayResolutionHeight:HudSetting",GetResolutionHeight())
    endIf
EndEvent

Event OnConfigInit()
    ;RefreshItems()
EndEvent
