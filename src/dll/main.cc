#include "pch.h"

#include <iostream>

#include "memory.hh"
#include "structs.hh"

using namespace rw::structs;
using namespace rw::il;
// DecaGames_RotMG_SocketManger__GotMessageHandler
rw::mem::hook::detour<SocketManager *, IncomingPacket *, MethodInfo *> g_gotMessageHandlerHook;
// DecaGames_RotMG_SocketManager__SendMessage
rw::mem::hook::detour<SocketManager *, OutgoingPacket *, MethodInfo *> g_sendMessageHandlerHook;
// DecaGames_RotMG_CameraManager__Update
rw::mem::hook::detour<CameraManager *, MethodInfo *> g_cameraManagerUpdateHook;
CameraManager *g_cameraManager = nullptr;
ProCamera2DPixelPerfect_ResizeCameraToPixelPerfect g_resizeCamera = nullptr;
CameraManager_UpdateCameraOffset g_updateCameraOffset = nullptr;
float g_originalPixelsPerUnit = 0.0f;
// FJBJOOHPBDL_MAEKPBAECML
rw::mem::hook::detour<ScreenInit *, MethodInfo *> g_screenInitSetupHook;
ScreenInit *g_screenInitSetup = nullptr;
bool g_disableFogOfWar = false;

DWORD WINAPI MainThread(LPVOID lpParam) {
  //#ifdef __DEBUG__
  rw::debug::alloc_console();
  //#endif
  MH_Initialize();
  const auto mi = rw::mem::get_module_info("GameAssembly.dll");

  g_resizeCamera = reinterpret_cast<ProCamera2DPixelPerfect_ResizeCameraToPixelPerfect>(mi.m_begin + 0x2751510);
  g_updateCameraOffset = reinterpret_cast<CameraManager_UpdateCameraOffset>(mi.m_begin + 0x1218170);

  g_gotMessageHandlerHook.m_install(mi.m_begin + 0x1710250, [](auto self, auto rawPacket, auto method) -> RWHOOK {
    auto packetID = rawPacket->klass->vtable.getId.ptr(rawPacket);
#if __DEBUG__
    auto packetName = copy_string_utf8(packet->klass->vtable.toString.ptr(rawPacket));
    std::cout << "got packet packet! " << packetName << " id: " << (int)packetID << std::endl;
#endif
    return g_gotMessageHandlerHook.m_getTrampoline()(self, rawPacket, method);
  });

  g_sendMessageHandlerHook.m_install(mi.m_begin + 0x1711CB0, [](auto self, auto rawPacket, auto method) -> RWHOOK {
    auto packetID = rawPacket->base_fields.packetId;
#if __DEBUG__
    auto packetName = copy_string_utf8(rawPacket->klass->vtable.toString.ptr(rawPacket));
    std::cout << "sent packet! " << packetName << " id: " << (int)packetID << std::endl;
#endif
    if (packetID == packets::ID::SendText) {
      auto packet = static_cast<packets::outgoing::Text *>(rawPacket);
      auto text = copy_string_utf8(packet->fields.text);
      if (text[0] == '/') {
        if (text.substr(0, 3) == "/ms") {
          try {
            auto mscale = std::stof(text.substr(3, text.length() - 1));
            if (g_cameraManager) {
              if (g_originalPixelsPerUnit == 0)
                g_originalPixelsPerUnit = g_cameraManager->fields.cameraPixelPerfect->fields.pixelsPerUnit;
              g_cameraManager->fields.cameraPixelPerfect->fields.pixelsPerUnit = g_originalPixelsPerUnit * mscale;
              // update camera and offset
              g_resizeCamera(g_cameraManager->fields.cameraPixelPerfect, nullptr);
              g_updateCameraOffset(g_cameraManager, nullptr);
              std::cout << "set mscale to: " << mscale << std::endl;
              return nullptr;
            }
          } catch (std::invalid_argument) {
            std::cout << "invalid integral agument passed to /ms <int32>" << std::endl;
            return nullptr;
          }
        }

        if (text == "/tf") {
          g_disableFogOfWar = !g_disableFogOfWar;
          std::cout << "fog of war: " << (g_disableFogOfWar ? "off" : "on") << std::endl;
          return nullptr;
        }
      }
    }
    return g_sendMessageHandlerHook.m_getTrampoline()(self, rawPacket, method);
  });

  g_cameraManagerUpdateHook.m_install(mi.m_begin + 0x1219C80, [](auto self, auto method) -> RWHOOK {
    if (g_disableFogOfWar && g_screenInitSetup) {
      g_screenInitSetup->fields.unknown0 = 625.0f;
      g_screenInitSetup->fields.unknown1 = 25.0f;
    }
    g_cameraManager = self;
    return g_cameraManagerUpdateHook.m_getTrampoline()(self, method);
  });

  g_screenInitSetupHook.m_install(mi.m_begin + 0x1E06C80, [](auto self, auto method) -> RWHOOK {
    g_screenInitSetup = self;
    return g_screenInitSetupHook.m_getTrampoline()(self, method);
  });

  return 0;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reasonForCall, LPVOID reserved) {
  switch (reasonForCall) {
  case DLL_PROCESS_ATTACH:
    CreateThread(NULL, 0, MainThread, 0, 0, 0);
    break;
  case DLL_PROCESS_DETACH:
    MH_Uninitialize();
    break;
  }

  return TRUE;
}

// TODO move these somewere else lol
// pretty printers
void pretty_print(packets::outgoing::Hello *packet) {
  std::cout << "Hello: " << '\n'
            << " |-unknown0: " << copy_string_utf8(packet->fields.unknown0) << '\n'
            << " |-unknown1: " << packet->fields.unknown1 << '\n'
            << " |-unknown2: " << copy_string_utf8(packet->fields.unknown2) << '\n'
            << " |-unknown3: " << copy_string_utf8(packet->fields.unknown3) << '\n'
            << " |-unknown4: " << copy_string_utf8(packet->fields.unknown4) << '\n'
            << " |-unknown5: " << packet->fields.unknown5 << '\n'
            << " |-unknown6: " << '\n'
            << "--BYTE ARRAY--" << '\n'
            << " |-unknown7: " << copy_string_utf8(packet->fields.unknown7) << '\n'
            << " |-unknown8: " << copy_string_utf8(packet->fields.unknown8) << '\n'
            << " |-unknown9: " << copy_string_utf8(packet->fields.unknown9) << '\n'
            << " |-unknown10: " << copy_string_utf8(packet->fields.unknown10) << '\n'
            << " |-unknown11: " << copy_string_utf8(packet->fields.unknown11) << '\n'
            << " |-unknown12: " << copy_string_utf8(packet->fields.unknown12) << '\n'
            << " |-unknown13: " << copy_string_utf8(packet->fields.unknown13) << '\n'
            << " |-unknown14: " << copy_string_utf8(packet->fields.unknown14) << std::endl;
}

void pretty_print(packets::incoming::MapInfo *packet) {
  std::cout << "MapInfo:" << '\n'
            << " |-width: " << packet->fields.width << '\n'
            << " |-height: " << packet->fields.height << '\n'
            << " |-mapName: " << copy_string_utf8(packet->fields.mapName) << '\n'
            << " |-mapDisplayName: " << copy_string_utf8(packet->fields.mapDisplayName) << '\n'
            << " |-realmName: " << copy_string_utf8(packet->fields.realmName) << '\n'
            << " |-difficulty: " << packet->fields.difficulty << '\n'
            << " |-rngSeed: " << packet->fields.rngSeed << '\n'
            << " |-background: " << packet->fields.background << '\n'
            << " |-maxPlayers: " << packet->fields.maxPlayers << '\n'
            << " |-allowPlayerTeleport: " << packet->fields.allowPlayerTeleport << '\n'
            << " |-showDisplays: " << packet->fields.showDisplays << '\n'
            << " |-connectionGUID: "
            << "--REDACTED--" << '\n' // copy_string_utf8(packet->fields.connectionGUID)
            << " |-unknown_0: " << copy_string_utf8(packet->fields.unknown_0) << '\n'
            << " |-worldElapsedTime: " << packet->fields.worldElapsedTime << std::endl;
}
