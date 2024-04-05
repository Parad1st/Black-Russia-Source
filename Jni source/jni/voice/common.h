#pragma once

#define MAX_CALLBACKS (ENetEventType::ENET_EVENT_TYPE_RECEIVE + 1)

typedef void (*ENetPacketCallback)(ENetEvent&);