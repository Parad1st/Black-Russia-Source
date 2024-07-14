#pragma once
#include <string>
#define HUD_ELEMENT_CHAT    0
#define HUD_ELEMENT_MAP     1
#define HUD_ELEMENT_TAGS    2
#define HUD_ELEMENT_BUTTONS 3
#define HUD_ELEMENT_HUD     4
#define HUD_ELEMENT_VOICE	5
#define HUD_ELEMENT_TEXTLABELS	6

#define HUD_MAX	7

#define PLAYER_PED_SLOTS	120
#define MAX_TEXT_DRAWS	3072
#define MAX_STREAMS		1000
#define MAX_MATERIALS		16
#define MAX_REMOVE_MODELS	1200
#define MAX_ATTACHED_OBJECTS	25
#define MAX_WEAPON_MODELS 47

typedef unsigned short VEHICLEID;
typedef unsigned short PLAYERID;

#define SA_MAJOR_VERSION	1
#define SA_MINOR_VERSION	8
#define SAMP_MAJOR_VERSION	2
#define SAMP_MINOR_VERSION	0

#define PADDING(x,y) uint8_t x[y]

#define IN_VEHICLE(x) ((x->dwStateFlags & 0x100) >> 8)
#define IS_CROUCHING(x) ((x->dwStateFlags >> 26) & 1)

#define BIT_SET(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define BIT_CLEAR(byte,nbit) ((byte) &= ~(1<<(nbit)))
#define BIT_CHECK(byte,nbit) ((byte) &   (1<<(nbit)))

#define CUSTOM_PACKET_SERIAL_RESPONSE   0   // internal IDs
#define CUSTOM_PACKET_ADDITIONAL_INFO	1

#define LIB_CRASH_OFFSET(offset, value)	*(uint8_t*)(g_libGTASA + 0x001800AC + (offset)) = value

//-----------------------------------------------------------

#define MAKE_PROFILE(var, var_t) \
	uint32_t (var) = GetTickCount(); \
	static uint32_t (var_t) = GetTickCount();
	

#define LOG_PROFILE(var, var_t) \
	if(GetTickCount() - (var_t) >= 5000) \
	{ \
		(var) = GetTickCount(); \
		(var_t) = GetTickCount(); \
	}

#pragma pack(1)
enum eAxis
{
	X = 0,
	Y,
	Z
};


//#define _CDEBUG

#define GAME_EDITION_CR

#ifdef GAME_EDITION_CR
#pragma message "Compiling for CR"
#else
#pragma message "Compiling for GTASA"
#endif

#pragma pack(1)
typedef struct _VECTOR 
{
	float X,Y,Z;
} VECTOR, *PVECTOR;

#pragma pack(1)
struct VehicleAudioPropertiesStruct
{
	int16_t VehicleType;		//	1: +  0
	int16_t EngineOnSound;	//  2: +  2
	int16_t EngineOffSound;	//  3: +  4
	int16_t field_4;			//  4: +  6
	float field_5;			//  5: +  8
	float field_6;			//  6: + 12
	char HornTon;				//  7: + 16
	char field_8[3];			//	8: + 17, zeros
	float HornHigh;			//  9: + 20
	char DoorSound;			// 10: + 24
	char field_11[1];			// 11: + 25, zeros
	char RadioNum;			// 12: + 26
	char RadioType;			// 13: + 27
	char field_14;			// 14: + 28
	char field_15[3];			// 15: + 29, zeros
	float field_16;			// 16: + 32
};

struct CRect
{
public:
	float left;          // x1
	float bottom;        // y1
	float right;         // x2
	float top;           // y2
};

#pragma pack(push, 1)

enum ePrimaryTasks //array indexes
{
	TASK_PRIMARY_PHYSICAL_RESPONSE = 0,
	TASK_PRIMARY_EVENT_RESPONSE_TEMP,
	TASK_PRIMARY_EVENT_RESPONSE_NONTEMP,
	TASK_PRIMARY_PRIMARY,
	TASK_PRIMARY_DEFAULT,
	TASK_PRIMARY_MAX
};

enum eSecondaryTasks //array indexes
{
	TASK_SECONDARY_ATTACK = 0,                // want duck to be after attack
	TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
	TASK_SECONDARY_SAY,
	TASK_SECONDARY_FACIAL_COMPLEX,
	TASK_SECONDARY_PARTIAL_ANIM,
	TASK_SECONDARY_IK,
	TASK_SECONDARY_MAX
};

class CTaskManager
{
public:
	void* m_aPrimaryTasks[5];
	void* m_aSecondaryTasks[6];
	class CPed* m_pPed;
};

class CPedIntelligence
{
public:
	class CPed* m_pPed;
	CTaskManager   m_TaskMgr;
};


#pragma pack(pop)

#pragma pack(1)
typedef struct _MATRIX4X4 
{
	VECTOR right;		// 0-12 	; r11 r12 r13
	uint32_t  flags;	// 12-16
	VECTOR up;			// 16-28	; r21 r22 r23
	float  pad_u;		// 28-32
	VECTOR at;			// 32-44	; r31 r32 r33
	float  pad_a;		// 44-48
	VECTOR pos;			// 48-60
	float  pad_p;		// 60-64
} MATRIX4X4, *PMATRIX4X4;

//-----------------------------------------------------------

#define RW_FRAME_NAME_LENGTH      23
#pragma pack(push, 1)
struct RwListEntry
{
	RwListEntry* next, * prev;
};

struct RwList
{
	RwListEntry root;
};

struct RwObject
{
	unsigned char type;
	unsigned char subtype;
	unsigned char flags;
	unsigned char privateFlags;
	void* parent;            // should be RwFrame with RpClump
};

struct RwFrame
{
	RwObject        object;                 // 0
	void* pad1, * pad2;            // 8
	MATRIX4X4        modelling;              // 16
	MATRIX4X4        ltm;                    // 32
	RwList          objects;                // 48
	struct RwFrame* child;                  // 56
	struct RwFrame* next;                   // 60
	struct RwFrame* root;                   // 64

	// Rockstar Frame extension (0x253F2FE) (24 bytes)
	unsigned char pluginData[8];                               // padding
	char          szName[RW_FRAME_NAME_LENGTH + 1];            // name (as stored in the frame extension)
};
#pragma pack(pop)

#define ATOMIC_ID_FLAG_TWO_VERSIONS_DAMAGED     2

struct SHandlingData
{
	uint8_t flag;
	float fValue;
	int iValue;
	SHandlingData(uint8_t uFlag, float fvalue, int ivalue) : flag(uFlag), fValue(fvalue), iValue(ivalue) {}
};

typedef struct _CVector2D
{
	int X;
	int Y;
} CVector2D;

typedef struct _CVector2DFloat
{
	float X;
	float Y;
} CVector2DFloat;


#pragma pack(1)
typedef struct _ATTACHED_OBJECT_INFO_INTERNAL
{
	uint32_t dwModelId;
	uint32_t dwBone;
	VECTOR vecOffset;
	VECTOR vecRotation;
	VECTOR vecScale;
	uint32_t dwColor[2];

	bool bState;
	class CObject* pObject;
} ATTACHED_OBJECT_INFO_INTERNAL;

#pragma pack(1)
typedef struct _ATTACHED_OBJECT_INFO
{
	uint32_t dwModelId;
	uint32_t dwBone;
	VECTOR vecOffset;
	VECTOR vecRotation;
	VECTOR vecScale;
	uint32_t dwColor[2];
} ATTACHED_OBJECT_INFO;

struct RpHAnimHierarchy
{
	int32_t             flags;          /**< Flags for the hierarchy  */
	int32_t             numNodes;      /**< Number of nodes in the hierarchy  */

	MATRIX4X4* pMatrixArray;   /**< Pointer to node matrices*/
};


#pragma pack(1)
struct MaterialInfo
{
	uint8_t m_bCreated;
	uint16_t wModelID;
	uint32_t dwColor;

	uint32_t oldFlags;
	struct RwTexture* pTex;
};

#pragma pack(1)
typedef struct _ENTITY_TYPE
{
	// ENTITY STUFF
	uint32_t vtable; 		// 0-4		;vtable
	PADDING(_pad91, 16);	// 4-20
	MATRIX4X4 *mat; 		// 20-24	;mat
	uintptr_t m_RwObject;	// 24 - 28
	PADDING(_pad92, 6);	// 28-34
	uint16_t nModelIndex; 	// 34-36	;ModelIndex
	PADDING(_pad93, 32);	// 36-68
	VECTOR vecMoveSpeed; 	// 68-80	;vecMoveSpeed
	VECTOR vecTurnSpeed; 	// 80-92	;vecTurnSpeed
	PADDING(_pad94, 88);	// 92-180
	uintptr_t dwUnkModelRel; // 180-184 ;сотка инфа

} ENTITY_TYPE;

//-----------------------------------------------------------

#pragma pack(1)
typedef struct _WEAPON_SLOT_TYPE
{
	uint32_t dwType;
	uint32_t dwState;
	uint32_t dwAmmoInClip;
	uint32_t dwAmmo;
	PADDING(_pwep1,12);
} WEAPON_SLOT_TYPE;  // MUST BE EXACTLY ALIGNED TO 28 bytes

#pragma pack(1)
typedef struct _PED_TYPE
{
	ENTITY_TYPE entity; 		// 0000-0184	;entity
	PADDING(_pad106, 174);		// 0184-0358
	uint32_t _pad107;			// 0358-0362	;dwPedType
	PADDING(_pad101, 722);		// 0362-1084
	CPedIntelligence* pPedIntelligence; // 1084-1088
	PADDING(_pad100, 8);		// 1088-1096
	uint32_t dwAction;			// 1096-1100	;Action
	PADDING(_pad102, 52);		// 1100-1152
	uint32_t dwStateFlags; 		// 1152-1156	;StateFlags

	uintptr_t dwInvulFlags; // 1136-1140		0x1000 = can_decap
	PADDING(_pad228, 8); // 1140-1148
	uintptr_t Tasks; // 1148-1152
	uintptr_t dwPlayerInfoOffset; // 1152-1156

	PADDING(_pad103, 168);		// 1156-1344
	float fHealth;		 		// 1344-1348	;Health
	float fMaxHealth;			// 1348-1352	;MaxHealth
	float fArmour;				// 1352-1356	;Armour
	float fAim;
	PADDING(_pad104, 8);		// 1356-1368
	float fRotation1;			// 1368-1372	;Rotation1
	float fRotation2;			// 1372-1376	;Rotation2
	PADDING(_pad105, 44);		// 1376-1420
	uint32_t pVehicle;			// 1420-1424	;pVehicle
	PADDING(_pad108, 8);		// 1424-1432
	uint32_t dwPedType;			// 1432-1436	;dwPedType
	uint32_t dwUnk1;	 // 1436-1440
	WEAPON_SLOT_TYPE WeaponSlots[13]; // 1440-1804
	PADDING(_pad270, 12); // 1804-1816
	uint8_t byteCurWeaponSlot; // 1816-1817
	PADDING(_pad280, 23); // 1817-1840
	uint32_t pFireObject;	 // 1840-1844
	PADDING(_pad281, 44); // 1844-1888
	uint32_t  dwWeaponUsed; // 1888-1892
	uintptr_t pdwDamageEntity; // 1892-1896
} PED_TYPE;

#pragma pack(1)
struct AIM_SYNC_DATA
{
	uint8_t	byteCamMode;
	float	vecAimf1[3];
	float	vecAimPos[3];
	float	fAimZ;
	uint8_t	byteCamExtZoom : 6;	// 0-63 normalized
	uint8_t	byteWeaponState : 2;		// see eWeaponState
	uint8_t	bUnk;
};
//-----------------------------------------------------------

#pragma pack(1)
struct BULLET_SYNC
{
	uint8_t hitType;
	uint16_t hitId;
	float origin[3];
	float hitPos[3];
	float offsets[3];
	uint8_t weapId;
};

enum eVehicleLightsSize : unsigned char {
	LIGHTS_LONG,
	LIGHTS_SMALL,
	LIGHTS_BIG,
	LIGHTS_TALL
};

enum eVehicleHandlingFlags : unsigned int {
	VEHICLE_HANDLING_1G_BOOST = 0x1,
	VEHICLE_HANDLING_2G_BOOST = 0x2,
	VEHICLE_HANDLING_NPC_ANTI_ROLL = 0x4,
	VEHICLE_HANDLING_NPC_NEUTRAL_HANDL = 0x8,
	VEHICLE_HANDLING_NO_HANDBRAKE = 0x10,
	VEHICLE_HANDLING_STEER_REARWHEELS = 0x20,
	VEHICLE_HANDLING_HB_REARWHEEL_STEER = 0x40,
	VEHICLE_HANDLING_ALT_STEER_OPT = 0x80,
	VEHICLE_HANDLING_WHEEL_F_NARROW2 = 0x100,
	VEHICLE_HANDLING_WHEEL_F_NARROW = 0x200,
	VEHICLE_HANDLING_WHEEL_F_WIDE = 0x400,
	VEHICLE_HANDLING_WHEEL_F_WIDE2 = 0x800,
	VEHICLE_HANDLING_WHEEL_R_NARROW2 = 0x1000,
	VEHICLE_HANDLING_WHEEL_R_NARROW = 0x2000,
	VEHICLE_HANDLING_WHEEL_R_WIDE = 0x4000,
	VEHICLE_HANDLING_WHEEL_R_WIDE2 = 0x8000,
	VEHICLE_HANDLING_HYDRAULIC_GEOM = 0x10000,
	VEHICLE_HANDLING_HYDRAULIC_INST = 0x20000,
	VEHICLE_HANDLING_HYDRAULIC_NONE = 0x40000,
	VEHICLE_HANDLING_NOS_INST = 0x80000,
	VEHICLE_HANDLING_OFFROAD_ABILITY = 0x100000,
	VEHICLE_HANDLING_OFFROAD_ABILITY2 = 0x200000,
	VEHICLE_HANDLING_HALOGEN_LIGHTS = 0x400000,
	VEHICLE_HANDLING_PROC_REARWHEEL_1ST = 0x800000,
	VEHICLE_HANDLING_USE_MAXSP_LIMIT = 0x1000000,
	VEHICLE_HANDLING_LOW_RIDER = 0x2000000,
	VEHICLE_HANDLING_STREET_RACER = 0x4000000,
	VEHICLE_HANDLING_SWINGING_CHASSIS = 0x10000000
};

enum eVehicleHandlingModelFlags : unsigned int {
	VEHICLE_HANDLING_MODEL_IS_VAN = 0x1,
	VEHICLE_HANDLING_MODEL_IS_BUS = 0x2,
	VEHICLE_HANDLING_MODEL_IS_LOW = 0x4,
	VEHICLE_HANDLING_MODEL_IS_BIG = 0x8,
	VEHICLE_HANDLING_MODEL_REVERSE_BONNET = 0x10,
	VEHICLE_HANDLING_MODEL_HANGING_BOOT = 0x20,
	VEHICLE_HANDLING_MODEL_TAILGATE_BOOT = 0x40,
	VEHICLE_HANDLING_MODEL_NOSWING_BOOT = 0x80,
	VEHICLE_HANDLING_MODEL_NO_DOORS = 0x100,
	VEHICLE_HANDLING_MODEL_TANDEM_SEATS = 0x200,
	VEHICLE_HANDLING_MODEL_SIT_IN_BOAT = 0x400,
	VEHICLE_HANDLING_MODEL_CONVERTIBLE = 0x800,
	VEHICLE_HANDLING_MODEL_NO_EXHAUST = 0x1000,
	VEHICLE_HANDLING_MODEL_DOUBLE_EXHAUST = 0x2000,
	VEHICLE_HANDLING_MODEL_NO1FPS_LOOK_BEHIND = 0x4000,
	VEHICLE_HANDLING_MODEL_FORCE_DOOR_CHECK = 0x8000,
	VEHICLE_HANDLING_MODEL_AXLE_F_NOTILT = 0x10000,
	VEHICLE_HANDLING_MODEL_AXLE_F_SOLID = 0x20000,
	VEHICLE_HANDLING_MODEL_AXLE_F_MCPHERSON = 0x40000,
	VEHICLE_HANDLING_MODEL_AXLE_F_REVERSE = 0x80000,
	VEHICLE_HANDLING_MODEL_AXLE_R_NOTILT = 0x100000,
	VEHICLE_HANDLING_MODEL_AXLE_R_SOLID = 0x200000,
	VEHICLE_HANDLING_MODEL_AXLE_R_MCPHERSON = 0x400000,
	VEHICLE_HANDLING_MODEL_AXLE_R_REVERSE = 0x800000,
	VEHICLE_HANDLING_MODEL_IS_BIKE = 0x1000000,
	VEHICLE_HANDLING_MODEL_IS_HELI = 0x2000000,
	VEHICLE_HANDLING_MODEL_IS_PLANE = 0x4000000,
	VEHICLE_HANDLING_MODEL_IS_BOAT = 0x8000000,
	VEHICLE_HANDLING_MODEL_BOUNCE_PANELS = 0x10000000,
	VEHICLE_HANDLING_MODEL_DOUBLE_RWHEELS = 0x20000000,
	VEHICLE_HANDLING_MODEL_FORCE_GROUND_CLEARANCE = 0x40000000,
	VEHICLE_HANDLING_MODEL_IS_HATCHBACK = 0x80000000
};

#pragma pack(push, 4)
struct tTransmissionGear
{
	float m_fMaxVelocity;
	float m_fChangeUpVelocity;
	float m_fChangeDownVelocity;
};

class cTransmission {
public:
	tTransmissionGear m_aGears[6];
	unsigned char m_nDriveType; // F/R/4
	unsigned char m_nEngineType; // P/D/E
	unsigned char m_nNumberOfGears; // 1 to 6
	char field_4B;
	unsigned int  m_nHandlingFlags;
	float         m_fEngineAcceleration; // 0.1 to 10.0
	float         m_fEngineInertia; // 0.0 to 50.0
	float         m_fMaxGearVelocity; // 5.0 to 150.0
	int field_5C;
	float         m_fMinGearVelocity;
	float         m_fCurrentSpeed;
};

struct tHandlingData {
	int           m_nVehicleId;
	float         m_fMass; // 1.0 to 50000.0
	float field_8;
	float         m_fTurnMass;
	float         m_fDragMult;
	VECTOR       m_vecCentreOfMass; // x, y, z - 1.0 to 50000.0
	unsigned char m_nPercentSubmerged; // 10 to 120 (> 100% vehicle sinks)
	PADDING(pad_0, 3);
	float         m_fBuoyancyConstant;
	float         m_fTractionMultiplier; // 0.5 to 2.0
	cTransmission m_transmissionData;
	float         m_fBrakeDeceleration; // 0.1 to 10.0
	float         m_fBrakeBias; // 0.0 > x > 1.0
	char          m_bABS; // 0/1
	char field_9D;
	char field_9E;
	char field_9F;
	float         m_fSteeringLock; // 10.0 to 40.0
	float         m_fTractionLoss;
	float         m_fTractionBias;
	float         m_fSuspensionForceLevel; // not [L/M/H]
	float         m_fSuspensionDampingLevel; // not [L/M/H]
	float         m_fSuspensionHighSpdComDamp; // often zero - 200.0 or more for bouncy vehicles
	float         m_fSuspensionUpperLimit;
	float         m_fSuspensionLowerLimit;
	float         m_fSuspensionBiasBetweenFrontAndRear;
	float         m_fSuspensionAntiDiveMultiplier;
	float         m_fCollisionDamageMultiplier; // 0.2 to 5.0
	union {
		eVehicleHandlingModelFlags m_nModelFlags;
		struct {
			unsigned int m_bIsVan : 1;
			unsigned int m_bIsBus : 1;
			unsigned int m_bIsLow : 1;
			unsigned int m_bIsBig : 1;
			unsigned int m_bReverseBonnet : 1;
			unsigned int m_bHangingBoot : 1;
			unsigned int m_bTailgateBoot : 1;
			unsigned int m_bNoswingBoot : 1;
			unsigned int m_bNoDoors : 1;
			unsigned int m_bTandemSeats : 1;
			unsigned int m_bSitInBoat : 1;
			unsigned int m_bConvertible : 1;
			unsigned int m_bNoExhaust : 1;
			unsigned int m_bDoubleExhaust : 1;
			unsigned int m_bNo1fpsLookBehind : 1;
			unsigned int m_bForceDoorCheck : 1;
			unsigned int m_bAxleFNotlit : 1;
			unsigned int m_bAxleFSolid : 1;
			unsigned int m_bAxleFMcpherson : 1;
			unsigned int m_bAxleFReverse : 1;
			unsigned int m_bAxleRNotlit : 1;
			unsigned int m_bAxleRSolid : 1;
			unsigned int m_bAxleRMcpherson : 1;
			unsigned int m_bAxleRReverse : 1;
			unsigned int m_bIsBike : 1;
			unsigned int m_bIsHeli : 1;
			unsigned int m_bIsPlane : 1;
			unsigned int m_bIsBoat : 1;
			unsigned int m_bBouncePanels : 1;
			unsigned int m_bDoubleRwheels : 1;
			unsigned int m_bForceGroundClearance : 1;
			unsigned int m_bIsHatchback : 1;
		};
	};
	union {
		eVehicleHandlingFlags m_nHandlingFlags;
		struct {
			unsigned int m_b1gBoost : 1;
			unsigned int m_b2gBoost : 1;
			unsigned int m_bNpcAntiRoll : 1;
			unsigned int m_bNpcNeutralHandl : 1;
			unsigned int m_bNoHandbrake : 1;
			unsigned int m_bSteerRearwheels : 1;
			unsigned int m_bHbRearwheelSteer : 1;
			unsigned int m_bAltSteerOpt : 1;
			unsigned int m_bWheelFNarrow2 : 1;
			unsigned int m_bWheelFNarrow : 1;
			unsigned int m_bWheelFWide : 1;
			unsigned int m_bWheelFWide2 : 1;
			unsigned int m_bWheelRNarrow2 : 1;
			unsigned int m_bWheelRNarrow : 1;
			unsigned int m_bWheelRWide : 1;
			unsigned int m_bWheelRWide2 : 1;
			unsigned int m_bHydraulicGeom : 1;
			unsigned int m_bHydraulicInst : 1;
			unsigned int m_bHydraulicNone : 1;
			unsigned int m_bNosInst : 1;
			unsigned int m_bOffroadAbility : 1;
			unsigned int m_bOffroadAbility2 : 1;
			unsigned int m_bHalogenLights : 1;
			unsigned int m_bProcRearwheelFirst : 1;
			unsigned int m_bUseMaxspLimit : 1;
			unsigned int m_bLowRider : 1;
			unsigned int m_bStreetRacer : 1;
			unsigned int m_bSwingingChassis : 1;
		};
	};
	float              m_fSeatOffsetDistance; // // ped seat position offset towards centre of car
	unsigned int       m_nMonetaryValue; // 1 to 100000
	eVehicleLightsSize m_nFrontLights;
	eVehicleLightsSize m_nRearLights;
	unsigned char      m_nAnimGroup;
};
#pragma pack(pop)

#pragma pack(1)
typedef struct _VEHICLE_TYPE
{
	ENTITY_TYPE entity;			// 0000-0184	;entity
	PADDING(_pad201, 716);		// 0184-900
	tHandlingData* pHandling;	// 900-904
	PADDING(_pad212, 160);		// 904 - 1076
	struct {
		unsigned char bIsLawEnforcer : 1; // Is this guy chasing the player at the moment
		unsigned char bIsAmbulanceOnDuty : 1; // Ambulance trying to get to an accident
		unsigned char bIsFireTruckOnDuty : 1; // Firetruck trying to get to a fire
		unsigned char bIsLocked : 1; // Is this guy locked by the script (cannot be removed)
		unsigned char bEngineOn : 1; // For sound purposes. Parked cars have their engines switched off (so do destroyed cars)
		unsigned char bIsHandbrakeOn : 1; // How's the handbrake doing ?
		unsigned char bLightsOn : 1; // Are the lights switched on ?
		unsigned char bFreebies : 1; // Any freebies left in this vehicle ?

		unsigned char bIsVan : 1; // Is this vehicle a van (doors at back of vehicle)
		unsigned char bIsBus : 1; // Is this vehicle a bus
		unsigned char bIsBig : 1; // Is this vehicle a bus
		unsigned char bLowVehicle : 1; // Need this for sporty type cars to use low getting-in/out anims
		unsigned char bComedyControls : 1; // Will make the car hard to control (hopefully in a funny way)
		unsigned char bWarnedPeds : 1; // Has scan and warn peds of danger been processed?
		unsigned char bCraneMessageDone : 1; // A crane message has been printed for this car allready
		unsigned char bTakeLessDamage : 1; // This vehicle is stronger (takes about 1/4 of damage)

		unsigned char bIsDamaged : 1; // This vehicle has been damaged and is displaying all its components
		unsigned char bHasBeenOwnedByPlayer : 1;// To work out whether stealing it is a crime
		unsigned char bFadeOut : 1; // Fade vehicle out
		unsigned char bIsBeingCarJacked : 1; // Fade vehicle out
		unsigned char bCreateRoadBlockPeds : 1;// If this vehicle gets close enough we will create peds (coppers or gang members) round it
		unsigned char bCanBeDamaged : 1; // Set to FALSE during cut scenes to avoid explosions
		unsigned char bOccupantsHaveBeenGenerated : 1; // Is true if the occupants have already been generated. (Shouldn't happen again)
		unsigned char bGunSwitchedOff : 1; // Level designers can use this to switch off guns on boats

		unsigned char bVehicleColProcessed : 1;// Has ProcessEntityCollision been processed for this car?
		unsigned char bIsCarParkVehicle : 1; // Car has been created using the special CAR_PARK script command
		unsigned char bHasAlreadyBeenRecorded : 1; // Used for replays
		unsigned char bPartOfConvoy : 1;
		unsigned char bHeliMinimumTilt : 1; // This heli should have almost no tilt really
		unsigned char bAudioChangingGear : 1; // sounds like vehicle is changing gear
		unsigned char bIsDrowning : 1; // is vehicle occupants taking damage in water (i.e. vehicle is dead in water)
		unsigned char bTyresDontBurst : 1; // If this is set the tyres are invincible

		unsigned char bCreatedAsPoliceVehicle : 1;// True if this guy was created as a police vehicle (enforcer, policecar, miamivice car etc)
		unsigned char bRestingOnPhysical : 1; // Dont go static cause car is sitting on a physical object that might get removed
		unsigned char bParking : 1;
		unsigned char bCanPark : 1;
		unsigned char bFireGun : 1; // Does the ai of this vehicle want to fire it's gun?
		unsigned char bDriverLastFrame : 1; // Was there a driver present last frame ?
		unsigned char bNeverUseSmallerRemovalRange : 1;// Some vehicles (like planes) we don't want to remove just behind the camera.
		unsigned char bIsRCVehicle : 1; // Is this a remote controlled (small) vehicle. True whether the player or AI controls it.

		unsigned char bAlwaysSkidMarks : 1; // This vehicle leaves skidmarks regardless of the wheels' states.
		unsigned char bEngineBroken : 1; // Engine doesn't work. Player can get in but the vehicle won't drive
		unsigned char bVehicleCanBeTargetted : 1;// The ped driving this vehicle can be targetted, (for Torenos plane mission)
		unsigned char bPartOfAttackWave : 1; // This car is used in an attack during a gang war
		unsigned char bWinchCanPickMeUp : 1; // This car cannot be picked up by any ropes.
		unsigned char bImpounded : 1; // Has this vehicle been in a police impounding garage
		unsigned char bVehicleCanBeTargettedByHS : 1;// Heat seeking missiles will not target this vehicle.
		unsigned char bSirenOrAlarm : 1; // Set to TRUE if siren or alarm active, else FALSE

		unsigned char bHasGangLeaningOn : 1;
		unsigned char bGangMembersForRoadBlock : 1;// Will generate gang members if NumPedsForRoadBlock > 0
		unsigned char bDoesProvideCover : 1; // If this is false this particular vehicle can not be used to take cover behind.
		unsigned char bMadDriver : 1; // This vehicle is driving like a lunatic
		unsigned char bUpgradedStereo : 1; // This vehicle has an upgraded stereo
		unsigned char bConsideredByPlayer : 1; // This vehicle is considered by the player to enter
		unsigned char bPetrolTankIsWeakPoint : 1;// If false shootong the petrol tank will NOT Blow up the car
		unsigned char bDisableParticles : 1; // Disable particles from this car. Used in garage.

		unsigned char bHasBeenResprayed : 1; // Has been resprayed in a respray garage. Reset after it has been checked.
		unsigned char bUseCarCheats : 1; // If this is true will set the car cheat stuff up in ProcessControl()
		unsigned char bDontSetColourWhenRemapping : 1;// If the texture gets remapped we don't want to change the colour with it.
		unsigned char bUsedForReplay : 1; // This car is controlled by replay and should be removed when replay is done.
	} m_nVehicleFlags;

	unsigned int m_nCreationTime;
	uint8_t byteColor1;			// 1076-1077	;byteColor1
	uint8_t byteColor2;			// 1077-1078	;byteColor2
	PADDING(_pad204, 42);		// 1078-1120
	PED_TYPE* pDriver;			// 1120-1124	;driver
	PED_TYPE* pPassengers[7];	// 1124-1152	;pPassengers
	PADDING(_pad202, 72);		// 1152-1224
	float fHealth;				// 1224-1228	;fHealth
	uint32_t unk;				// 1228 - 1232 - unk
	uint32_t dwTrailer;			// 1232 - 1236 - trailer
	PADDING(_pad203, 48);		// 1236-1284
	uint32_t dwDoorsLocked;
} VEHICLE_TYPE;
typedef struct _RECT
{
	float fLeft;
	float fBottom;
	float fRight;
	float fTop;
} RECT, * PRECT;

//-----------------------------------------------------------

#define	VEHICLE_SUBTYPE_CAR				1
#define	VEHICLE_SUBTYPE_BIKE			2
#define	VEHICLE_SUBTYPE_HELI			3
#define	VEHICLE_SUBTYPE_BOAT			4
#define	VEHICLE_SUBTYPE_PLANE			5
#define	VEHICLE_SUBTYPE_PUSHBIKE		6
#define	VEHICLE_SUBTYPE_TRAIN			7

//-----------------------------------------------------------

#define TRAIN_PASSENGER_LOCO			538
#define TRAIN_FREIGHT_LOCO				537
#define TRAIN_PASSENGER					570
#define TRAIN_FREIGHT					569
#define TRAIN_TRAM						449
#define HYDRA							520

//-----------------------------------------------------------

#define ACTION_WASTED					55
#define ACTION_DEATH					54
#define ACTION_INCAR					50
#define ACTION_NORMAL					1
#define ACTION_SCOPE					12
#define ACTION_NONE						0 

//-----------------------------------------------------------

#define WEAPON_MODEL_BRASSKNUCKLE		331 // was 332
#define WEAPON_MODEL_GOLFCLUB			333
#define WEAPON_MODEL_NITESTICK			334
#define WEAPON_MODEL_KNIFE				335
#define WEAPON_MODEL_BAT				336
#define WEAPON_MODEL_SHOVEL				337
#define WEAPON_MODEL_POOLSTICK			338
#define WEAPON_MODEL_KATANA				339
#define WEAPON_MODEL_CHAINSAW			341
#define WEAPON_MODEL_DILDO				321
#define WEAPON_MODEL_DILDO2				322
#define WEAPON_MODEL_VIBRATOR			323
#define WEAPON_MODEL_VIBRATOR2			324
#define WEAPON_MODEL_FLOWER				325
#define WEAPON_MODEL_CANE				326
#define WEAPON_MODEL_GRENADE			342 // was 327
#define WEAPON_MODEL_TEARGAS			343 // was 328
#define WEAPON_MODEL_MOLOTOV			344 // was 329
#define WEAPON_MODEL_COLT45				346
#define WEAPON_MODEL_SILENCED			347
#define WEAPON_MODEL_DEAGLE				348
#define WEAPON_MODEL_SHOTGUN			349
#define WEAPON_MODEL_SAWEDOFF			350
#define WEAPON_MODEL_SHOTGSPA			351
#define WEAPON_MODEL_UZI				352
#define WEAPON_MODEL_MP5				353
#define WEAPON_MODEL_AK47				355
#define WEAPON_MODEL_M4					356
#define WEAPON_MODEL_TEC9				372
#define WEAPON_MODEL_RIFLE				357
#define WEAPON_MODEL_SNIPER				358
#define WEAPON_MODEL_ROCKETLAUNCHER		359
#define WEAPON_MODEL_HEATSEEKER			360
#define WEAPON_MODEL_FLAMETHROWER		361
#define WEAPON_MODEL_MINIGUN			362
#define WEAPON_MODEL_SATCHEL			363
#define WEAPON_MODEL_BOMB				364
#define WEAPON_MODEL_SPRAYCAN			365
#define WEAPON_MODEL_FIREEXTINGUISHER	366
#define WEAPON_MODEL_CAMERA				367
#define WEAPON_MODEL_NIGHTVISION		368	// newly added
#define WEAPON_MODEL_INFRARED			369	// newly added
#define WEAPON_MODEL_JETPACK			370	// newly added
#define WEAPON_MODEL_PARACHUTE			371

#define WEAPON_MODEL_PARACHUTE			371
#define WEAPON_FIST                        0
#define WEAPON_BRASSKNUCKLE                1
#define WEAPON_GOLFCLUB                    2
#define WEAPON_NITESTICK                3
#define WEAPON_KNIFE                    4
#define WEAPON_BAT                        5
#define WEAPON_SHOVEL                    6
#define WEAPON_POOLSTICK                7
#define WEAPON_KATANA                    8
#define WEAPON_CHAINSAW                    9
#define WEAPON_DILDO                    10
#define WEAPON_DILDO2                    11
#define WEAPON_VIBRATOR                    12
#define WEAPON_VIBRATOR2                13
#define WEAPON_FLOWER                    14
#define WEAPON_CANE                        15
#define WEAPON_GRENADE                    16
#define WEAPON_TEARGAS                    17
#define WEAPON_MOLTOV                    18
#define WEAPON_COLT45                    22
#define WEAPON_SILENCED                    23
#define WEAPON_DEAGLE                    24
#define WEAPON_SHOTGUN                    25
#define WEAPON_SAWEDOFF                    26
#define WEAPON_SHOTGSPA                    27
#define WEAPON_UZI                        28
#define WEAPON_MP5                        29
#define WEAPON_AK47                        30
#define WEAPON_M4                        31
#define WEAPON_TEC9                        32
#define WEAPON_RIFLE                    33
#define WEAPON_SNIPER                    34
#define WEAPON_ROCKETLAUNCHER            35
#define WEAPON_HEATSEEKER                36
#define WEAPON_FLAMETHROWER                37
#define WEAPON_MINIGUN                    38
#define WEAPON_SATCHEL                    39
#define WEAPON_BOMB                        40
#define WEAPON_SPRAYCAN                    41
#define WEAPON_FIREEXTINGUISHER            42
#define WEAPON_CAMERA                    43
#define WEAPON_PARACHUTE                46
#define WEAPON_VEHICLE                    49
#define WEAPON_DROWN                    53
#define WEAPON_COLLISION                54

#define OBJECT_PARACHUTE				3131
#define OBJECT_CJ_CIGGY					1485
#define OBJECT_DYN_BEER_1				1486
#define OBJECT_CJ_BEER_B_2				1543
#define OBJECT_CJ_PINT_GLASS			1546