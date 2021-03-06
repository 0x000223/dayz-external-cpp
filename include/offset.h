//
// Created by Max on 06/05/2022.
//

#ifndef DAYZ_EXTERNAL_CPP_OFFSET_H
#define DAYZ_EXTERNAL_CPP_OFFSET_H

/*
 * ARMA String
 */
#define O_STRING_SIZE 0x8
#define O_STRING_START 0x10

/*
 * Network
 */
#define O_NETWORK_MANAGER 0xE5AB20
#define O_NETWORK_CLIENT 0x50

/*
 * Network Client 
 */
#define O_NETWORK_SCOREBOARD 0x10
#define O_NETWORK_PLAYERCOUNT 0x18
#define O_NETWORK_SERVERNAME 0x330

/*
 * Scoreboard
 */
#define O_SCOREBOARD_SIZE 0x158
#define O_SCOREBOARD_NAME 0xF0

/*
 * World
 */
#define O_WORLD 0x40AE4F0
#define O_WORLD_BULLET_TABLE 0xD70
#define O_WORLD_NEAR_TABLE 0xEB8
#define O_WORLD_FAR_TABLE 0x1000
#define O_WORLD_SLOW_TABLE 0x1F68
#define O_WORLD_ITEM_TABLE 0x1FB8
#define O_WORLD_LOCALPLAYER 0x28B8
#define O_WORLD_CAMERA 0x1B8
#define O_WORLD_CAMERAON 0x28A8

/*
 * Entity
 */
#define O_ENTITY_TYPE 0x148
#define O_ENTITY_VISUAL_STATE 0x198
#define O_ENTITY_NETWORK_ID 0x634
#define O_ENTITY_PLAYER_SKELETON 0x760
 #define O_ENTITY_INFECTED_SKELETON 0x5D0

/**
 * Entity Type
 */
#define O_ENTITYTYPE_CONFIGNAME 0xA0
#define O_ENTITYTYPE_TYPENAME 0x68
#define O_ENTITYTYPE_MODELNAME 0x80
#define O_ENTITYTYPE_CLASSNAME 0x4E0 //Clean name

/*
 * Visual State
 */
#define O_VISUAL_STATE_POSITION 0x2C
#define O_VISUAL_STATE_HEAD_POSITION 0xF8

/*
 * Camera
 */
#define O_CAMERA_VIEWRIGHT 0x8
#define O_CAMERA_VIEWUP 0x14
#define O_CAMERA_VIEFORWARD 0x20
#define O_CAMERA_VIEWTRANSLATION 0x2C
#define O_CAMERA_PROJECTION_D1 0xD0
#define O_CAMERA_PROJECTION_D2 0xDC
#define O_CAMERA_VIEWPORTSIZE 0x58

#endif //DAYZ_EXTERNAL_CPP_OFFSET_H
