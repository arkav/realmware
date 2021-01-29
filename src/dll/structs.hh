#pragma once
#include <cstdint>

#include "il2cpp.hh"
namespace rw::structs {
// il2cpp
struct MethodInfo {};
// NAMCOMCIKPH
struct IncomingPacket;
// CLAGKHDHBKL
struct OutgoingPacket;

struct SocketManager {};

template <typename MethodPtr> struct VirtualInvokeData {
  MethodPtr ptr;
  void *methodInfo;
};

// IncomingPacket vtable functions
typedef uint8_t(__fastcall *IncomingPacket_GetId)(IncomingPacket *packet);
typedef il::String *(__fastcall *IncomingPacket_ToString)(IncomingPacket *packet);
struct IncomingPacket_VTable {
  char pad_0[48];
  VirtualInvokeData<IncomingPacket_ToString> toString; // offset 48
  char pad_1[960];
  VirtualInvokeData<IncomingPacket_GetId> getId; // offset 1024
};

struct IncomingPacket_Class {
  char pad[304];
  IncomingPacket_VTable vtable; // offset 304
};

struct IncomingPacket {
  IncomingPacket_Class *klass;
  void *monitor_data;
};
// end IncomingPacket
typedef il::String *(__fastcall *OutgoingPacket_ToString)(OutgoingPacket *packet);
struct OutgoingPacket_VTable {
  char pad[48];
  VirtualInvokeData<OutgoingPacket_ToString> toString; // offset 48
};

struct OutgoingPacket_Class {
  char pad[304];
  OutgoingPacket_VTable vtable; // offset 304
};

struct OutgoingPacket_Fields {
  void *list_system_byte; // buf
  uint8_t packetId;       // id
  uint8_t unknown_0;      // ???
};

struct OutgoingPacket {
  OutgoingPacket_Class *klass;
  void *monitor;
  OutgoingPacket_Fields base_fields;
};
// end outgoing packet
struct _ProCamera2DPixelPerfect_Fields {
  char pad_0[72];
  float pixelsPerUnit; // offset 72 PixelsPerUnit
};
struct ProCamera2DPixelPerfect {
  void *klass;
  void *monitor;
  _ProCamera2DPixelPerfect_Fields fields;
};
struct _CameraManager_Fields {
  char pad_0[12];
  float unknown_0; // offset 12
  char pad_1[24];
  ProCamera2DPixelPerfect *cameraPixelPerfect; // offset 40 BDAEGGDPHCK
};
struct CameraManager {
  void *klass;
  void *monitor;
  _CameraManager_Fields fields;
};
// ProCamera2DPixelPerfect_ResizeCameraToPixelPerfect
using ProCamera2DPixelPerfect_ResizeCameraToPixelPerfect = void *(__stdcall *)(ProCamera2DPixelPerfect *, MethodInfo *);
// CameraManger_ONKBCEJOCHD (called from ZoomIn and ZoomOut)
using CameraManager_UpdateCameraOffset = void *(__stdcall *)(CameraManager *, MethodInfo *);
// end CameraManager

// FJBJOOHPBDL (ScreenInit)
// credit DIA4A
struct _ScreenInit_Fields {
  char pad_0[40];
  float unknown0; // offset 40 DFHJJKMFHNA = 625.0f
  float unknown1; // offset 44 EFFBIGGGNKG = 25.0f
};
struct ScreenInit {
  void *klass;
  void *monitor;
  _ScreenInit_Fields fields; // offset 40
};
// end ScreenInit
namespace packets {
enum ID : uint8_t { SendText = 10, Hello = 1, MapInfo = 92 };
template <typename T> struct IncomingPacketT : IncomingPacket { T fields; };
template <typename T> struct OutgoingPacketT : OutgoingPacket { T fields; };
namespace incoming {
// ECMJOEBPKFH
struct _MapInfo_Types {
  int32_t width;
  int32_t height;
  il::String *mapName;
  il::String *mapDisplayName;
  il::String *realmName;
  int32_t difficulty;
  uint32_t rngSeed;
  int32_t background;
  int32_t maxPlayers;
  bool allowPlayerTeleport;
  bool showDisplays;
  void *clientXML; // List_System_String
  void *extraXML;  // List_System_String
  il::String *connectionGUID;
  il::String *unknown_0;
  int32_t worldElapsedTime;
};
using MapInfo = IncomingPacketT<_MapInfo_Types>;
} // namespace incoming
namespace outgoing {
// PJMBFLNIANG
struct _Hello_Types {
  il::String *unknown0;
  int32_t unknown1;
  il::String *unknown2;
  il::String *unknown3;
  il::String *unknown4;
  int32_t *unknown5;
  void *unknown6; // byte array
  il::String *unknown7;
  il::String *unknown8;
  il::String *unknown9;
  il::String *unknown10;
  il::String *unknown11;
  il::String *unknown12;
  il::String *unknown13;
  il::String *unknown14;
};
using Hello = OutgoingPacketT<_Hello_Types>;
// CKOFCJHOJBA
struct _Text_Types {
  il::String *text;
};
using Text = OutgoingPacketT<_Text_Types>;
} // namespace outgoing
} // namespace packets
} // namespace rw::structs
