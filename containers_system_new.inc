
#define ContSystem:%0(%1)	ContSystem_%0(%1)
#define ContainerSystem 	ContainerSystem

// ---- цвета, можно удалить если они у вас уже есть
#define cl_white 	"{ffffff}"
#define cl_orange 	"{ffae00}"
#define cl_grey 	"{c8cacc}"

// ---- макрос вычисления модуля целого числа
#if defined abs
	#undef abs
#endif
#define abs(%0)\
	(%0 < 0) ? (-(%0)) : (%0)

// ---- ID объектов для контейнера
#define CONTAINER_OBJECT_ID 		19321	// - ID объекта самого контейнера
#define CONTAINER_DOOR_OBJECT_ID 	3062	// - ID объекта дверей контейнера
#define CONTAINER_CLOTH_OBJECT 		1271	// - ID объекта ящика (при выигрыше одежды)
#define CONTAINER_ACCESORY_OBJECT 	2969	// - ID объекта ящика (при выигрыше аксессуара)
#define CONTAINER_GAME_VALUES_OBJ 	1550	// - ID объекта при выигрыше игровых ценностей

// ----- Лимиты
#define MAX_CONTAINERS_COUNT		100 	// - максимальное число контейнеров
#define MAX_CONTAINER_PRIZES  		100 	// - максимальное количество призов
#define MAX_PRIZE_BY_TYPES 			20 		// - максимальное число призов определённого класса и типа (используется в функции ContSystem:UpdatePrize)


// ---- Радиусы действия/зоны прорисовки
#define CONTAINER_DYNAMIC_AREA_SIZE	1.5 	// - радиус динамической зоны, где доступно взаимодействие через Left ALT
#define CONTAINER_TEXT_DRAW_DIST 	20.0 	// - дальность прорисовки 3D-текста перед контейнером

// ---- Время
#define CONTAINER_ACUTION_TIME 		10800  	// - время аукциона (10800 секунд = 3 часа)
#define CONTAINER_STOP_AUCTION_TIME 45 		// - 45 секунд с момента ставки = конец аукциона
#define CONTAINER_WIN_TIME 			3 		// - время игроку на то, чтобы забрать приз (3 минуты)
#define CONTAINER_TAKE_PRIZE_TIME 	2  		// - время игрок после открытия контейнера на то, чтобы посмотрел на приз

// ---- Остальное
#define CONTAINER_PRIZE_TYPE_COUNT 		3 		// - количество типов призов (транспорт, акссесуар или одежда, деньги или дона)
#define CONTAINER_CLASSES_COUNT 		3 		// - количество классов контейнера (1 - низкий, 2 - средний, 3 - высокий)
#define CONTAINER_BID_PRICE				300		// - шаг ставки на контейнер
#define CONTAINER_PRIZE_SELL_PERCENT 	0.65 	// - процент, за который будет продан контейнер в фонд порта (65%)
#define CONTAINER_DOORS_COUNT 			2 		// - количество объектов двери у контейнера


// ---- Статусы контейнера
#define CONTAINER_STATUS_NONE 			0 // - контейнера не существует
#define CONTAINER_STATUS_WAIT 			1 // - контейнер ожидает начала аукциона
#define CONTAINER_STATUS_AUCTION 		2 // - у контейнера проводится аукнцион
#define CONTAINER_STATUS_WINNER_WAIT	3 // - контейнер ожидает, пока его откроет победитель
#define CONTAINER_STATUS_TAKE_PRIZE 	4 // - победитель забирает приз у контейнера

// ---- Контейнер не определён (не существует)
#define CONTAINER_CLASS_NONE 			0 

// ---- Типы контейнеров
#define CONTAINER_TYPE_NONE 			-1 // - контейнер без типа (не существует)
#define CONTAINER_TYPE_CAR 				1  // - транспортный контейнер
#define CONTAINER_TYPE_ACCESORYS		2  // - контейнер с аксессуарами или одеждой
#define CONTAINER_TYPE_GAME_VALUES		3  // - контейнер с игровыми ценностями (опыт, деньги, донат)

// ---- Типы конкретных призов
#define CONTAINER_TYPE_PRIZE_VEHICLE 	1  // - транспортный приз
#define CONTAINER_TYPE_PRIZE_CLOTH 		2  // - приз одежда
#define CONTAINER_TYPE_PRIZE_ACSESSORYS 3  // - приз аксессуар
#define CONTAINER_TYPE_PRIZE_EXP 		4  // - приз игровой опыт
#define CONTAINER_TYPE_PRIZE_MONEY 		5  // - приз денежные средства
#define CONTAINER_TYPE_PRIZE_DONATE 	6  // - приз донат

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

// ---- Смещение дверей контейнера относительно самого контейнера, когда угол его поворота равен 0 градусов
new const Float:container_doors_offset [CONTAINER_DOORS_COUNT][4] = {
	{-1.563965, -3.518683, -0.060003, 0.0},
	{1.577027, -3.518683, -0.060003, 180.0}
};

// ---- Смещение 3D-текста перед контейнером относительно самого контейнера, когда угол его поворота равен 0 градусов
new const Float:container_text_offset [2] = {
	0.0, -4.971753
};

// ---- Времена, когда стартует аукцион
new const container_start_time [][] = {
	"12:00", "15:00", "18:00", "21:00"
};

// ---- Статусы контейнера в строковом виде
new const container_status_str [][] = {
	"Не создан",
	"В ожидании аукциона",
	"Проходит аукцион",
	"Ожидает пока заберут приз",
	"Забирают приз"
};

// ---- Классы контейнера в строковом виде
new const container_class_str [][] = {
	"-",
	"Низкий",
	"Средний",
	"Высокий"
};

// ---- Типы призов контейнера в строков виде
new const container_type_str [][] = {
	"-",
	"Автомобильный",
	"Одежда или аксессуары",
	"Игровые ценности"
};

// ---- Стартовые цены за определённые классы контейнеров
new const container_start_prices [CONTAINER_CLASSES_COUNT+1] = {
	0, 	  // - никакой (не создан)
	500,  // - низкий
	1000, // - средний
	2000  // - высокий
};

// ---- Типы конкретных призов в строковом виде
new const container_prize_types_str [][] = {
	"-",
	"Транспорт",
	"Одежда",
	"Аксессуар",
	"Игровой опыт",
	"Денежные средства",
	"Донат"
};

new edit_player_container_id [MAX_PLAYERS] = { INVALID_OBJECT_ID, ... }; // - создаваемый объект при добавлении нового контейнера
new edit_player_container_listitem [MAX_PLAYERS][MAX_CONTAINERS_COUNT]; // - упорядоченный список id контейнеров/призов
new bool:containers_freezed = false; // - заморозка контейнеров администрацией

//==============================================================================
enum E_ENUM_CONTAINER_DATA
{
	EC_SQL_ID,				// - ID контейнера в таблице MySQL
	EC_OBJECT_ID,			// - ID объекта контейнера
	EC_DYNAMIC_AREA,		// - ID динамической арены для взаимодействия с контейнером
	EC_CLASS,				// - класс контейнера
	EC_TYPE,				// - тип контейнера
	EC_PRIZE,				// - ID приза в контейнере
	EC_STATUS,				// - статус контейнера
	EC_STOP_AUCTION_TIME,	// - время, когда закончится аукцион (через сколько секунд)
	EC_PLAYER_ID,			// - ID последнего игрока, который поставил ставку на контейнер
	EC_LAST_CLASS,			// - последний класс (для рандомайзера призов)
	EC_LAST_TYPE,			// - последний тип (для рандомайзера призов)
	EC_LAST_PRIZE,			// - последний приз (для рандомайзера призов)
	EC_START_PRICE,			// - стартовая цена за контейнер
	EC_PRICE,				// - текущая цена за контейнер
	Float:EC_POS_X,			// - местоположение контейнера по X
	Float:EC_POS_Y,			// - местоположение контейнера по Y
	Float:EC_POS_Z,			// - местоположение контейнера по Z
	Float:EC_POS_ROT,		// - угол поворота контейнера
	EC_STOP_TIMER,			// - время отсчёта после того как игрок поставил ставку на контейнер
	EC_WAIT_TIMER,			// - время за которое победитель должен забрать выигрыш
	EC_DOOR_OBJECT_1,		// - ID двери контейнера №1
	EC_DOOR_OBJECT_2,		// - ID двери контейнера №2
	EC_PRIZE_OBJECT,		// - ID призового объекта/транспорта
	Text3D:EC_TEXT_LABEL	// - динамический 3D-текст перед контейнером
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
	EC_PRIZE_SQL_ID,	// - ID приза в таблице MySQL
	EC_PRIZE_CLASS,		// - класс приза
	EC_PRIZE_TYPE,		// - тип приза
	EC_PRIZE_VALUE,		// - выдаваемый ID приза или количество игровой ценности (в зависимости от типа)
	EC_PRIZE_PRICE		// - полная гос. стоимость приза (без учёта процента)
}
new g_prizes_data [MAX_CONTAINER_PRIZES][E_ENUM_CONTAINER_PRIZE_DATA];
new const g_prizes_data_default [E_ENUM_CONTAINER_PRIZE_DATA] = {
	-1,
	-1,
	-1,
	-1,
	-1
};