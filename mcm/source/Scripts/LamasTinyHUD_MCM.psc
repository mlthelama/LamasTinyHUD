ScriptName LamasTinyHUD_MCM Extends MCM_ConfigBase

Event OnConfigClose() native

function RefreshItems()
    SetMenuOptions("uTopSelectedItem:TopPage", GetSelectedOptions(GetModSettingInt("uTopSelectedItem:TopPage")))
endfunction

string[] function GetSelectedOptions(int a_id) native

int function GetFormIdForSelection(int a_index) native

Event OnSettingChange(String a_ID)
    if (a_ID == "uTopSelectedItem:TopPage")
        SetModSettingString("sSelectedTopItemForm:TopPage", GetFormIdForSelection(GetModSettingInt(a_ID)))
        RefreshMenu()
    endif
EndEvent