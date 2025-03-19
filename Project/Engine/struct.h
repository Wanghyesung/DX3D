#pragma once



struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	//내 표면 방향
	Vec3 vNormal;
	//내 표면방향에 수직선 x
	Vec3 vTangent;
	//내 표면 방향과 tan방향에 수직인 방향
	Vec3 vBinormal;

	// Animation 가중치 및 인덱스
	Vec4 vWeights;
	Vec4 vIndices;
};

typedef tVertex Vtx;




// Event
struct tEvent
{
	EVENT_TYPE	Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


struct tDebugShapeInfo
{
	SHAPE_TYPE	eShape;
	Matrix		matWorld;
	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRotation;
	Vec4		vColor;
	float		fMaxTime;
	float		fCurTime;
	bool		bDepthTest;
};



struct tLightColor
{
	Vec4 vDiffuse;	// 빛의 색상
	Vec4 vAmbient;	// 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // 빛의 색상

	Vec4		vWorldPos;   // 광원의 월드 스페이스 위치
	Vec4		vWorldDir;	 // 빛을 보내는 방향

	UINT		LightType;   // 빛의 타입(방향성, 점, 스포트)
	float		Radius;		 // 빛의 반경(사거리)
	float		Angle;		 // 빛의 각도
	int			padding;
};


// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

// Animator2D
struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};


// Particle
struct tParticle
{
	Vec4	vLocalPos;		// 오브젝트로부터 떨어진 거리
	Vec4	vWorldPos;		// 파티클 최종 월드위치
	Vec4	vWorldScale;	// 파티클 크기	
	Vec4	vColor;			// 파티클 색상
	Vec4	vVelocity;		// 파티클 현재 속도
	Vec4	vForce;			// 파티클에 주어진 힘
	Vec4	vRandomForce;	// 파티클에 적용되는 랜덤방향 힘

	float   Age;			// 생존 시간
	float   PrevAge;		// 이전 프레임 생존 시간
	float   NomalizedAge;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	LifeTime;		// 수명
	float	Mass;			// 질량
	float   ScaleFactor;	// 추가 크기 배율

	int     Active;			// 파티클 활성화 여부
	int     pad;
};

struct tRWParticleBuffer
{
	int		SpawnCount;			// 스폰 시킬 파티클 개수
	int		padding[3];
};


struct tParticleModule
{
	// 스폰 모듈
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;
	float	fSphereShapeRadius;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int		SpawnRate;			// 초당 생성 개수
	int		Space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
	float   MinLifeTime;		// 최소 수명
	float   MaxLifeTime;		// 최대 수명
	int     spawnpad[3];

	// Color Change 모듈
	Vec4	vStartColor;		// 초기 색상
	Vec4	vEndColor;			// 최종 색상

	// Scale Change 모듈
	float	StartScale;			// 초기 배율
	float	EndScale;			// 최종 배율	

	// 버퍼 최대크기
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity 모듈
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag 모듈 - 속도 제한
	float	StartDrag;
	float	EndDrag;

	// NoiseForce 모듈 - 랜덤 힘 적용	
	float	fNoiseTerm;		// 랜덤 힘 변경 간격
	float	fNoiseForce;	// 랜덤 힘 크기

	// Render 모듈
	int		VelocityAlignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	int		VelocityScale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	float   vMaxSpeed;			// 최대 크기에 도달하는 속력
	Vec4	vMaxVelocityScale;	// 속력에 따른 크기 변화량 최대치
	int		renderpad;

	//회전 모듈
	Matrix  vRotateMatrix;
	Vec3	vDataPad;
	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};

//광선 구조체
struct tRay
{
	//시작 점
	Vec3 vStart;
	//방향
	Vec3 vDir;
};

//Raycast 결과를 받을 구조체
struct tRaycastOut
{
	Vec2 vUV;//접점 UV비율
	float fDist;//거리
	int bSuccess;//성공
};

// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};


struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				matBone;   // 이거 안씀
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			strAnimName;
	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;

	double			dStartTime; //처음 시간
	double			dEndTime; //끝 시간
	double			dTimeLength; //전체 시간
	float			fUpdateTime; // 이거 안씀

	FbxTime::EMode	eMode; //30프레임
};

//physx 오브젝트 구분


// ===========
// Instancing
// ===========\
//물체 구분해서 모아두는 용
union uInstID
{
	struct
	{
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 llID;
};

class CGameObject;
struct tInstObj
{
	CGameObject* pObj;
	UINT iMtrlIdx;
};

//같은 메쉬 , 재질 (메쉬 인덱스)인 오브젝트들끼리 모아서
//한번에 렌더링처리 그 때 오브젝트마다 위치를 이 구조체에 저장
struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx; //몇번째 메쉬인지
};

// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matWorldInv;
	Matrix matPrevWorld; //이전프레임 월드행렬

	Matrix matView;
	Matrix matViewInv;
	Matrix matPrevView; //이전프레임 월드행렬

	Matrix matProj;
	Matrix matProjInv;
	Matrix matPrevProj; //이전프레임 월드행렬

	//inv역행렬

	Matrix matWV;//view까지만 구한 행렬
	Matrix matWVP;//투영까지 구한 행렬


};

extern tTransform g_transform;

struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};


struct tMtrlConst
{
	tMtrlData mtrl;

	//상황에 맞게 사용
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	//몇번 인덱스에 텍스쳐를 바인딩할지 확인용 bool int
	int arrTex[(UINT)TEX_PARAM::TEX_END];

	// 3D Animation 정보
	int	arrAnimData[4];
};


struct tGlobal
{
	Vec4  CameraPos;
	Vec4  CameraRot;
	Vec2  Resolution;
	float tDT;
	float tAccTime;
	UINT  Light2DCount;
	UINT  Light3DCount;
	int	  globalpadding[2];
};

extern tGlobal GlobalData;


//공격 데미지, 판정 ..
//공격 데미지 정보
struct tAttack
{
	float fDamage;

	float fAttRcnt;//맞으면 날라갈 양 X
	float fAttUpperRcnt; //맞으면 띄어질 양 Y

	float fAttackTime; //다음 공격까지 가능한 시간

	float fAddForceTime; //맞으면 날라가는 시간

	bool bDown; //맞으면 넘어지는지
	//float fAttRigidityTime; //경직 시간

	int iMaxCount; //공격 가능한 수
	//int iCurCount; //현재 공격 수
};

//공격 정보
struct tAttackInfo
{
	int iAttackNum;

	//공격할때 움직임
	Vec3 vForce;
	//z축 움직임
	float fForce;
	float fRotate;
	float fMoveTime; //움직임 시간

	//공격판정 시작프레임
	int iStartFrame;
	int iEndFrame;

	//공격 히트박스
	float fOffsetPos;//캐릭터 앞벡터 중심으로 얼마나 떨어져서 나타낼지
	Vec3 vAttackScale;
	Vec3 vAttackRot;

	tAttack tAttackValue;
};

struct tActorInfo
{
	float fHP;
	float fMaxHP;
	float fSpeed;
};

struct tHitInfo
{
	//맞으면 날라갈 양
	float fHitRcnt;
	
	//날라가는 시간
	float fHitTime;
	
	//데미지
	float fDamage;

	//넘어질지
	bool bDown;

	Vec3 vHitPos;
};

struct tFontInfo
{
	wstring Str;
	float fPosX; float fPosY;
	float fFontSize;
	UINT Color;
};




