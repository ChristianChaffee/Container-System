
#define ContSystem:%0(%1)	ContSystem_%0(%1)
#define ContainerSystem 	ContainerSystem

// ---- �����, ����� ������� ���� ��� � ��� ��� ����
#define cl_white 	"{ffffff}"
#define cl_orange 	"{ffae00}"
#define cl_grey 	"{c8cacc}"

// ---- ������ ���������� ������ ������ �����
#if defined abs
	#undef abs
#endif
#define abs(%0)\
	(%0 < 0) ? (-(%0)) : (%0)

// ---- ID �������� ��� ����������
#define CONTAINER_OBJECT_ID 		19321	// - ID ������� ������ ����������
#define CONTAINER_DOOR_OBJECT_ID 	3062	// - ID ������� ������ ����������
#define CONTAINER_CLOTH_OBJECT 		1271	// - ID ������� ����� (��� �������� ������)
#define CONTAINER_ACCESORY_OBJECT 	2969	// - ID ������� ����� (��� �������� ����������)
#define CONTAINER_GAME_VALUES_OBJ 	1550	// - ID ������� ��� �������� ������� ���������

// ----- ������
#define MAX_CONTAINERS_COUNT		100 	// - ������������ ����� �����������
#define MAX_CONTAINER_PRIZES  		100 	// - ������������ ���������� ������
#define MAX_PRIZE_BY_TYPES 			20 		// - ������������ ����� ������ ������������ ������ � ���� (������������ � ������� ContSystem:UpdatePrize)


// ---- ������� ��������/���� ����������
#define CONTAINER_DYNAMIC_AREA_SIZE	1.5 	// - ������ ������������ ����, ��� �������� �������������� ����� Left ALT
#define CONTAINER_TEXT_DRAW_DIST 	20.0 	// - ��������� ���������� 3D-������ ����� �����������

// ---- �����
#define CONTAINER_ACUTION_TIME 		10800  	// - ����� �������� (10800 ������ = 3 ����)
#define CONTAINER_STOP_AUCTION_TIME 45 		// - 45 ������ � ������� ������ = ����� ��������
#define CONTAINER_WIN_TIME 			3 		// - ����� ������ �� ��, ����� ������� ���� (3 ������)
#define CONTAINER_TAKE_PRIZE_TIME 	2  		// - ����� ����� ����� �������� ���������� �� ��, ����� ��������� �� ����

// ---- ���������
#define CONTAINER_PRIZE_TYPE_COUNT 		3 		// - ���������� ����� ������ (���������, ��������� ��� ������, ������ ��� ����)
#define CONTAINER_CLASSES_COUNT 		3 		// - ���������� ������� ���������� (1 - ������, 2 - �������, 3 - �������)
#define CONTAINER_BID_PRICE				300		// - ��� ������ �� ���������
#define CONTAINER_PRIZE_SELL_PERCENT 	0.65 	// - �������, �� ������� ����� ������ ��������� � ���� ����� (65%)
#define CONTAINER_DOORS_COUNT 			2 		// - ���������� �������� ����� � ����������


// ---- ������� ����������
#define CONTAINER_STATUS_NONE 			0 // - ���������� �� ����������
#define CONTAINER_STATUS_WAIT 			1 // - ��������� ������� ������ ��������
#define CONTAINER_STATUS_AUCTION 		2 // - � ���������� ���������� ��������
#define CONTAINER_STATUS_WINNER_WAIT	3 // - ��������� �������, ���� ��� ������� ����������
#define CONTAINER_STATUS_TAKE_PRIZE 	4 // - ���������� �������� ���� � ����������

// ---- ��������� �� �������� (�� ����������)
#define CONTAINER_CLASS_NONE 			0 

// ---- ���� �����������
#define CONTAINER_TYPE_NONE 			-1 // - ��������� ��� ���� (�� ����������)
#define CONTAINER_TYPE_CAR 				1  // - ������������ ���������
#define CONTAINER_TYPE_ACCESORYS		2  // - ��������� � ������������ ��� �������
#define CONTAINER_TYPE_GAME_VALUES		3  // - ��������� � �������� ���������� (����, ������, �����)

// ---- ���� ���������� ������
#define CONTAINER_TYPE_PRIZE_VEHICLE 	1  // - ������������ ����
#define CONTAINER_TYPE_PRIZE_CLOTH 		2  // - ���� ������
#define CONTAINER_TYPE_PRIZE_ACSESSORYS 3  // - ���� ���������
#define CONTAINER_TYPE_PRIZE_EXP 		4  // - ���� ������� ����
#define CONTAINER_TYPE_PRIZE_MONEY 		5  // - ���� �������� ��������
#define CONTAINER_TYPE_PRIZE_DONATE 	6  // - ���� �����

//==============================================================================
#define GetPrizeData(%0,%1)		g_prizes_data[%0][%1]
#define SetPrizeData(%0,%1,%2) 	g_prizes_data[%0][%1] = %2
#define GetContainerData(%0,%1) 		g_container_data[%0][%1]
#define SetContainerData(%0,%1,%2)		g_container_data[%0][%1] = %2
#define AddContainerData(%0,%1,%2,%3) 	g_container_data[%0][%1] %2= %3

#define GetContainerStatusStr(%0) container_status_str[GetContainerData(%0, EC_STATUS)]
#define GetContainerClassStr(%0) container_class_str[GetContainerData(%0, EC_CLASS)]
#define GetContainerTypeStr(%0) container_type_str[GetContainerData(%0, EC_TYPE)]

//==============================================================================

// ---- �������� ������ ���������� ������������ ������ ����������, ����� ���� ��� �������� ����� 0 ��������
new const Float:container_doors_offset [CONTAINER_DOORS_COUNT][4] = {
	{-1.563965, -3.518683, -0.060003, 0.0},
	{1.577027, -3.518683, -0.060003, 180.0}
};

// ---- �������� 3D-������ ����� ����������� ������������ ������ ����������, ����� ���� ��� �������� ����� 0 ��������
new const Float:container_text_offset [2] = {
	0.0, -4.971753
};

// ---- �������, ����� �������� �������
new const container_start_time [][] = {
	"12:00", "15:00", "18:00", "21:00"
};

// ---- ������� ���������� � ��������� ����
new const container_status_str [][] = {
	"�� ������",
	"� �������� ��������",
	"�������� �������",
	"������� ���� ������� ����",
	"�������� ����"
};

// ---- ������ ���������� � ��������� ����
new const container_class_str [][] = {
	"-",
	"������",
	"�������",
	"�������"
};

// ---- ���� ������ ���������� � ������� ����
new const container_type_str [][] = {
	"-",
	"�������������",
	"������ ��� ����������",
	"������� ��������"
};

// ---- ��������� ���� �� ����������� ������ �����������
new const container_start_prices [CONTAINER_CLASSES_COUNT+1] = {
	0, 	  // - ������� (�� ������)
	500,  // - ������
	1000, // - �������
	2000  // - �������
};

// ---- ���� ���������� ������ � ��������� ����
new const container_prize_types_str [][] = {
	"-",
	"���������",
	"������",
	"���������",
	"������� ����",
	"�������� ��������",
	"�����"
};

new edit_player_container_id [MAX_PLAYERS] = { INVALID_OBJECT_ID, ... }; // - ����������� ������ ��� ���������� ������ ����������
new edit_player_container_listitem [MAX_PLAYERS][MAX_CONTAINERS_COUNT]; // - ������������� ������ id �����������/������
new bool:containers_freezed = false; // - ��������� ����������� ��������������

//==============================================================================
enum E_ENUM_CONTAINER_DATA
{
	EC_SQL_ID,				// - ID ���������� � ������� MySQL
	EC_OBJECT_ID,			// - ID ������� ����������
	EC_DYNAMIC_AREA,		// - ID ������������ ����� ��� �������������� � �����������
	EC_CLASS,				// - ����� ����������
	EC_TYPE,				// - ��� ����������
	EC_PRIZE,				// - ID ����� � ����������
	EC_STATUS,				// - ������ ����������
	EC_STOP_AUCTION_TIME,	// - �����, ����� ���������� ������� (����� ������� ������)
	EC_PLAYER_ID,			// - ID ���������� ������, ������� �������� ������ �� ���������
	EC_LAST_CLASS,			// - ��������� ����� (��� ������������ ������)
	EC_LAST_TYPE,			// - ��������� ��� (��� ������������ ������)
	EC_LAST_PRIZE,			// - ��������� ���� (��� ������������ ������)
	EC_START_PRICE,			// - ��������� ���� �� ���������
	EC_PRICE,				// - ������� ���� �� ���������
	Float:EC_POS_X,			// - �������������� ���������� �� X
	Float:EC_POS_Y,			// - �������������� ���������� �� Y
	Float:EC_POS_Z,			// - �������������� ���������� �� Z
	Float:EC_POS_ROT,		// - ���� �������� ����������
	EC_STOP_TIMER,			// - ����� ������� ����� ���� ��� ����� �������� ������ �� ���������
	EC_WAIT_TIMER,			// - ����� �� ������� ���������� ������ ������� �������
	EC_DOOR_OBJECT_1,		// - ID ����� ���������� �1
	EC_DOOR_OBJECT_2,		// - ID ����� ���������� �2
	EC_PRIZE_OBJECT,		// - ID ��������� �������/����������
	Text3D:EC_TEXT_LABEL	// - ������������ 3D-����� ����� �����������
}
new g_container_data [MAX_CONTAINERS_COUNT][E_ENUM_CONTAINER_DATA];
new g_container_data_default [E_ENUM_CONTAINER_DATA] = {
	-1,
	INVALID_OBJECT_ID,
	-1,
	CONTAINER_CLASS_NONE,
	CONTAINER_TYPE_NONE,
	0,
	CONTAINER_STATUS_NONE,
	0,
	INVALID_PLAYER_ID,
	-1,
	-1,
	-1,
	0,
	0,
	0.0,
	0.0,
	0.0,
	0.0,
	CONTAINER_STOP_AUCTION_TIME,
	CONTAINER_WIN_TIME,
	INVALID_OBJECT_ID,
	INVALID_OBJECT_ID,
	INVALID_OBJECT_ID
};

//==============================================================================
enum E_ENUM_CONTAINER_PRIZE_DATA
{
	EC_PRIZE_SQL_ID,	// - ID ����� � ������� MySQL
	EC_PRIZE_CLASS,		// - ����� �����
	EC_PRIZE_TYPE,		// - ��� �����
	EC_PRIZE_VALUE,		// - ���������� ID ����� ��� ���������� ������� �������� (� ����������� �� ����)
	EC_PRIZE_PRICE		// - ������ ���. ��������� ����� (��� ����� ��������)
}
new g_prizes_data [MAX_CONTAINER_PRIZES][E_ENUM_CONTAINER_PRIZE_DATA];
new const g_prizes_data_default [E_ENUM_CONTAINER_PRIZE_DATA] = {
	-1,
	-1,
	-1,
	-1,
	-1
};