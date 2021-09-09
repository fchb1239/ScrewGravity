#include "ScrewGravityWatchView.hpp"
#include "config.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "UnityEngine/Vector3.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"


DEFINE_TYPE(ScrewGravity, ScrewGravityWatchView);

using namespace GorillaUI;
using namespace UnityEngine;
using namespace Photon::Pun;

bool isRoom;

extern void SetSpeed();

namespace ScrewGravity
{
    void ScrewGravityWatchView::Awake()
    {
        settingSelector = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, false);
        settingSelector->max = 4;
        upSelect = new UISelectionHandler(EKeyboardKey::Left, EKeyboardKey::Right, EKeyboardKey::Enter, false, true);
        upSelect->max = 6;
        upSelect->currentSelectionIndex = config.upSpeed;

        SetSpeed();
    }

    void ScrewGravityWatchView::DidActivate(bool firstActivation)
    {
        std::function<void(bool)> fun = std::bind(&ScrewGravityWatchView::OnEnter, this, std::placeholders::_1);
        settingSelector->selectionCallback = fun;
        Redraw();
    }

    void ScrewGravityWatchView::OnEnter(int index)
    {
        if (settingSelector->currentSelectionIndex == 0) 
        {
            config.enabled ^= 1;
            SaveConfig();
        }
        if (settingSelector->currentSelectionIndex == 1) 
        {
            config.gravitySetting ^= 1;
            SaveConfig();
        }
        if (settingSelector->currentSelectionIndex == 2) 
        {
            config.frozen ^= 1;
            SaveConfig();
        }
    }

    void ScrewGravityWatchView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        MonkeWatch::Redraw();
    }

    void ScrewGravityWatchView::DrawHeader()
    {
        text += "<color=#ffff00>========================</color>\n";
        text += "<b><color=#004797>Screw </color></b><color=#EEECCA>Gravity</color>\n";
        text += "<color=#EEECCA>by: </color><b><color=#004797>fchb1239</color></b>\n";
        text += "<color=#ffff00>========================</color>\n";
    }

    void ScrewGravityWatchView::DrawBody()
    {
        text += settingSelector->currentSelectionIndex == 0 ? "<size=60><color=#FF5115>></color></size>" : "<size=60> </size>";
        text += config.enabled ? "<size=60><color=#00ff00> Enabled</color></size>" : "<size=60><color=#ff0000> Disabled</color></size>"; 
        text += "<size=60>\n\n</size>";
        text += settingSelector->currentSelectionIndex == 1 ? "<size=60><color=#FF5115>></color></size>" : "<size=60> </size>";
        text += config.gravitySetting ? "<size=60><color=#004797> Zero</color></size>" : "<size=60><color=#EEECCA> Custom</color></size>"; 
        text += "<size=60>\n\n</size>";
        text += settingSelector->currentSelectionIndex == 2 ? "<size=60><color=#FF5115>></color></size>" : "<size=60> </size>";
        text += config.frozen ? "<size=60><color=#004797> Unfrozen</color></size>" : "<size=60><color=#EEECCA> Frozen</color></size>"; 
        text += "<size=60>\n\n</size>";
        text += "<color=#ffff00>========================</color>\n";
        text += "<b><color=#004797>Custom </color></b><color=#EEECCA>Gravity <b><color=#004797>Settings:</color></b></color>\n";
        text += "<color=#ffff00>========================</color>\n";
        text += settingSelector->currentSelectionIndex == 3 ? "<size=60><color=#FF5115>></color></size>" : "<size=60> </size>";
        text += "<size=60><color=#004797> Gravity: </color></size>";
        //haunted's eyes are burning rn xDDDDD
        switch (upSelect->currentSelectionIndex) {
            case 0:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>10</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>10</color></size>";
                }
                break;
            case 1:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>50</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>50</color></size>";
                }
                break;
            case 2:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>100</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>100</color></size>";
                }
                break;
            case 3:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>200</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>200</color></size>";
                }
                break;
            case 4:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>1000</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>1000</color></size>";
                }
                break;
            case 5:
                if(!config.gravitySetting) {
                    text += "<size=60><color=#EEECCA>10000</color></size>";
                } else {
                    text += "<size=60><color=#ff0000>10000</color></size>";
                }
                break;
            default:
                break;    
            } 

        text += "<size=60>\n\n</size>";
        
        /*
        if (config.enabled && config.isRoom)
        {
            text += "<size=50><color=#FF6D49>\n               It no work, go join\n               a private lobby lol. \n</color></size>";
        }
        */
    }

    void ScrewGravityWatchView::OnKeyPressed(int value)
    {
        EKeyboardKey key = (EKeyboardKey)value;
        if (!settingSelector->HandleKey(key)) // if it was not up/down/enter
        {
            switch (settingSelector->currentSelectionIndex)
            {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    upSelect->HandleKey(key);  
                    break;
                case 4:      
                    break;
                default:
                    break;
            }
            config.upSpeed = upSelect->currentSelectionIndex;
            SetSpeed();
            SaveConfig();
        }
        Redraw();
    }
}