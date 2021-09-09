#include "main.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GorillaLocomotion/Player.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Object.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/PrimitiveType.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "UnityEngine/Renderer.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "ExitGames/Client/Photon/SendOptions.hpp"
#include "ExitGames/Client/Photon/EventData.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Realtime/RaiseEventOptions.hpp"
#include "Photon/Realtime/ReceiverGroup.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "monkecomputer/shared/Register.hpp"
#include "custom-types/shared/register.hpp"
#include "ScrewGravityWatchView.hpp"
#include "config.hpp"
using namespace GlobalNamespace;
using namespace GorillaLocomotion;
using namespace UnityEngine;
using namespace ExitGames::Client::Photon;
using namespace Photon::Realtime;
using namespace Photon::Pun;
//ik i dont need to include so much and use that many namespaces bug mehh

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

bool trigger;
bool once; //someont told me about smth with getting when the player joined with a patching thingy but i forgot, ill just be keeping this here until i figure it out lol
float upAmount = 10;

void SetSpeed() {
    if(config.upSpeed  == 0) {
        upAmount = 10;
    }
    if(config.upSpeed  == 1) {
        upAmount = 50;
    }
    if(config.upSpeed  == 2) {
        upAmount = 100;
    }
    if(config.upSpeed  == 3) {
        upAmount = 200;
    }
    if(config.upSpeed == 4) {
        upAmount = 1000;
    }
    if(config.upSpeed == 5) {
        upAmount = 10000;
    }
}

//haunted is reading this code rn, and just dying inside lmao
MAKE_HOOK_MATCH(Player_Update, &GorillaLocomotion::Player::Update, void, GorillaLocomotion::Player* self) {
    //getLogger().info("Debugging");
    Player_Update(self);
    Il2CppObject* currentRoom = CRASH_UNLESS(il2cpp_utils::RunMethod("Photon.Pun", "PhotonNetwork", "get_CurrentRoom"));
    if (currentRoom) {
        config.isRoom = !CRASH_UNLESS(il2cpp_utils::RunMethod<bool>(currentRoom, "get_IsVisible"));
    }
    else {
        config.isRoom = true;
    }
    if(config.isRoom && config.enabled) {
        trigger = OVRInput::Get(OVRInput::Button::One, OVRInput::Controller::RTouch);
        if(trigger) {
            if(!config.gravitySetting) {
                GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_useGravity(false);
                GorillaLocomotion::Player::get_Instance()->playerRigidBody->AddForce(upAmount * UnityEngine::Vector3::get_up());
            }
            else {
                GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_useGravity(false);
            }
        }
        else if(config.frozen) {
            GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_useGravity(true);
        }
        if(!config.frozen) {
            GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_useGravity(false);
            GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_velocity(Vector3::get_zero());
        }
        once = false;
    }
    else if(!once) {
        GorillaLocomotion::Player::get_Instance()->playerRigidBody->set_useGravity(true);
        once = true;
    }
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    GorillaUI::Init();

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), Player_Update);
    // Install our hooks (none defined yet)
    getLogger().info("Installed all hooks!");

    custom_types::Register::AutoRegister();

    GorillaUI::Register::RegisterWatchView<ScrewGravity::ScrewGravityWatchView*>("<b><color=#004797>Screw </color></b><color=#EEECCA>Gravity</color>", VERSION); //if you take the first and last letter from EEECCA and add games, then its in the game lol im so funny

    LoadConfig();
}