#pragma once



struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	//�� ǥ�� ����
	Vec3 vNormal;
	//�� ǥ����⿡ ������ x
	Vec3 vTangent;
	//�� ǥ�� ����� tan���⿡ ������ ����
	Vec3 vBinormal;

	// Animation ����ġ �� �ε���
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
	Vec4 vDiffuse;	// ���� ����
	Vec4 vAmbient;	// �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // ���� ����

	Vec4		vWorldPos;   // ������ ���� �����̽� ��ġ
	Vec4		vWorldDir;	 // ���� ������ ����

	UINT		LightType;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		Radius;		 // ���� �ݰ�(��Ÿ�)
	float		Angle;		 // ���� ����
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
	Vec4	vLocalPos;		// ������Ʈ�κ��� ������ �Ÿ�
	Vec4	vWorldPos;		// ��ƼŬ ���� ������ġ
	Vec4	vWorldScale;	// ��ƼŬ ũ��	
	Vec4	vColor;			// ��ƼŬ ����
	Vec4	vVelocity;		// ��ƼŬ ���� �ӵ�
	Vec4	vForce;			// ��ƼŬ�� �־��� ��
	Vec4	vRandomForce;	// ��ƼŬ�� ����Ǵ� �������� ��

	float   Age;			// ���� �ð�
	float   PrevAge;		// ���� ������ ���� �ð�
	float   NomalizedAge;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	LifeTime;		// ����
	float	Mass;			// ����
	float   ScaleFactor;	// �߰� ũ�� ����

	int     Active;			// ��ƼŬ Ȱ��ȭ ����
	int     pad;
};

struct tRWParticleBuffer
{
	int		SpawnCount;			// ���� ��ų ��ƼŬ ����
	int		padding[3];
};


struct tParticleModule
{
	// ���� ���
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;
	float	fSphereShapeRadius;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int		SpawnRate;			// �ʴ� ���� ����
	int		Space;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)
	float   MinLifeTime;		// �ּ� ����
	float   MaxLifeTime;		// �ִ� ����
	int     spawnpad[3];

	// Color Change ���
	Vec4	vStartColor;		// �ʱ� ����
	Vec4	vEndColor;			// ���� ����

	// Scale Change ���
	float	StartScale;			// �ʱ� ����
	float	EndScale;			// ���� ����	

	// ���� �ִ�ũ��
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity ���
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag ��� - �ӵ� ����
	float	StartDrag;
	float	EndDrag;

	// NoiseForce ��� - ���� �� ����	
	float	fNoiseTerm;		// ���� �� ���� ����
	float	fNoiseForce;	// ���� �� ũ��

	// Render ���
	int		VelocityAlignment;	// 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
	int		VelocityScale;		// 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����	
	float   vMaxSpeed;			// �ִ� ũ�⿡ �����ϴ� �ӷ�
	Vec4	vMaxVelocityScale;	// �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
	int		renderpad;

	//ȸ�� ���
	Matrix  vRotateMatrix;
	Vec3	vDataPad;
	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};

//���� ����ü
struct tRay
{
	//���� ��
	Vec3 vStart;
	//����
	Vec3 vDir;
};

//Raycast ����� ���� ����ü
struct tRaycastOut
{
	Vec2 vUV;//���� UV����
	float fDist;//�Ÿ�
	int bSuccess;//����
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
	Matrix				matOffset;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				matBone;   // �̰� �Ⱦ�
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			strAnimName;
	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;

	double			dStartTime; //ó�� �ð�
	double			dEndTime; //�� �ð�
	double			dTimeLength; //��ü �ð�
	float			fUpdateTime; // �̰� �Ⱦ�

	FbxTime::EMode	eMode; //30������
};

//physx ������Ʈ ����


// ===========
// Instancing
// ===========\
//��ü �����ؼ� ��Ƶδ� ��
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

//���� �޽� , ���� (�޽� �ε���)�� ������Ʈ�鳢�� ��Ƽ�
//�ѹ��� ������ó�� �� �� ������Ʈ���� ��ġ�� �� ����ü�� ����
struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx; //���° �޽�����
};

// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matWorldInv;
	Matrix matPrevWorld; //���������� �������

	Matrix matView;
	Matrix matViewInv;
	Matrix matPrevView; //���������� �������

	Matrix matProj;
	Matrix matProjInv;
	Matrix matPrevProj; //���������� �������

	//inv�����

	Matrix matWV;//view������ ���� ���
	Matrix matWVP;//�������� ���� ���


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

	//��Ȳ�� �°� ���
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	//��� �ε����� �ؽ��ĸ� ���ε����� Ȯ�ο� bool int
	int arrTex[(UINT)TEX_PARAM::TEX_END];

	// 3D Animation ����
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


//���� ������, ���� ..
//���� ������ ����
struct tAttack
{
	float fDamage;

	float fAttRcnt;//������ ���� �� X
	float fAttUpperRcnt; //������ ����� �� Y

	float fAttackTime; //���� ���ݱ��� ������ �ð�

	float fAddForceTime; //������ ���󰡴� �ð�

	bool bDown; //������ �Ѿ�������
	//float fAttRigidityTime; //���� �ð�

	int iMaxCount; //���� ������ ��
	//int iCurCount; //���� ���� ��
};

//���� ����
struct tAttackInfo
{
	int iAttackNum;

	//�����Ҷ� ������
	Vec3 vForce;
	//z�� ������
	float fForce;
	float fRotate;
	float fMoveTime; //������ �ð�

	//�������� ����������
	int iStartFrame;
	int iEndFrame;

	//���� ��Ʈ�ڽ�
	float fOffsetPos;//ĳ���� �պ��� �߽����� �󸶳� �������� ��Ÿ����
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
	//������ ���� ��
	float fHitRcnt;
	
	//���󰡴� �ð�
	float fHitTime;
	
	//������
	float fDamage;

	//�Ѿ�����
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




