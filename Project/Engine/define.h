#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32
#define MAX_MIP   8

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;



enum class LAYER_TYPE
{
	Default,
	Background,
	Player,
	Monster,
	LandScape, //전체 땅
	landform, //지형지물
	Obstacle, //장애물
	Stairs,
	Terrain, //지형
	NavMeshPlane,
	Attack,
	MonsterAttack,
	Light,
	Camera,
	UI = 31,
	End = 32,
};


enum class CAMERA_TYPE
{
	MAIN,
	UI,
	END,
};

enum class LEVEL_TYPE
{
	TEST,
	END = 12,
};

enum class COMPONENT_TYPE
{
	//update 순서

	EQUIP,			// 물체 위치를 행렬을 반환
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	MOTIONBLUR,		// 흔들림 효과 이전 프레임 위치 정보 저장
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌

	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	RIGIDBODY,		// 힘을받는 강체

	PXRIGIDBODY,	// 피직스 강체	
	TRANSFORM,		// 위치, 크기, 회전
	//TRANSFORM,		// 위치, 크기, 회전

	NAVAGENT,		//실질적으로 움질일 메쉬
	RDNAVMESHFIELD,		//라이브러리 네이게이션
	NAVMESHPLANE,   //네비메쉬 발판
	NAVMESH,		// 네비게이션
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	SKYBOX,			// 스카이 박스
	DECAL,			// 내부 렌더링

	END,

	// custom
	SCRIPT,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];


enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합

	DEFEREED_DECAL_BLEND, //0 타겟은 alphablend, 1번 타겟은 one one blend
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	DOMAIN_DEFERRED,	//지연 렌더링 오브젝트
	DOMAIN_DEFERRED_DECAL,  //deffered decal(광원 적용 가능한 decal)

	DOMAIN_OPAQUE,		// 불투명 오브젝트
	DOMAIN_MASK,		// 불투명, 투명
	DOMAIN_DECAL,
	DOMAIN_BLUR,		//블러 처리
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후 처리
	DOMAIN_UI,

	DOMAIN_LIGHT,			// 광원 타입 
	DOMAIN_UNDEFINED,		// 미정
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject
	ERASE_OBJECT,   // wParam : GameObject, lParam : Layer Index
	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	CHANAGE_STATE,   //wParam : FSM , lParam : state type
	CHANAGE_MONSTER_STATE, //wParam : FSM , lParam : state type

	LEVEL_CHANGE,
};

enum class STATE_TYPE
{
	IDLE,
	WALK,
	RUN,
	ROLL,
	ATTACK,
	JUMP,
	JUMPING,
	JUMPEND,
	JUMPATTACK,
	HIT,
	DEAD,
	END,
};

enum class MONSTER_STATE_TYPE
{
	IDLE,
	WALK,
	RUN,
	ROLL,
	ATTACK,
	JUMP,
	JUMP_ATTACK,
	WAIT,
	HIT,
	DEAD,
	END,
};

enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	CYLINDER,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLIDER3D_TYPE
{
	CUBE,
	CIRCLE,
};


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	DUMMY_3,
	ROTATE,

	END,
};

//멀티렌더타겟의 타입
enum class MRT_TYPE
{
	SWAPCHAIN,

	DEFERRED,

	DATA,

	BOTION_BLUER,

	MIDDLE,

	DEFERRED_DECAL,

	LIGHT,

	//SHADOWMAP,

	NEAR_SHADOWMAP,
	MID_SHADOWMAP,
	FAR_SHADOWMAP,

	END,
};