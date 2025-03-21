#pragma once
#include "define.h"

template <typename T, UINT _Size>
void DeleteArray(T* (&Arr)[_Size])
{
	for (UINT i = 0; i < _Size; ++i)
	{
		if (nullptr != Arr[i])
			delete Arr[i];
	}
}


// 오브젝트 생성
class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName);

// 오브젝트 삭제
void DestroyObject(CGameObject* _DeletObject);

// 오브젝트 씬에서 제거
void EraseObject(CGameObject* _EraseObject, int _LayerIdx);

bool IsValidObj(CGameObject*& _Target);

// State Chanage
class CFSM;
void ChanageState(CFSM* _pFSM, STATE_TYPE _eNextType);

class CMonsterFSM;
void ChanageMonsterState(CMonsterFSM* _pFSM, MONSTER_STATE_TYPE _eNextType);
CGameObject* InitializeFBX(const wstring& _strFbxName);
void ChanageRSType(CGameObject* const _pGameObj, RS_TYPE _eType);
void ChanageDSType(CGameObject* const _pGameObj, DS_TYPE _eType);

// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCube(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f, bool DepthTest = false);
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f, bool DepthTest = false);

void DrawDebugCylinder(const Matrix& _matWorld, Vec4 _vColor, float _fTime, bool DepthTest);

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);

int GetSizeofFormat(DXGI_FORMAT _eFormat);

void SetFrustomBound(CGameObject* _pGameObj, bool _bActive , int _iBoundSize);

const char* ToString(RES_TYPE);
const wchar_t* ToWSTring(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWSTring(COMPONENT_TYPE);

//Relative Path 
wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath);

// Save / Load
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

void DestroyMgr();

class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>
void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}





template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
		{
			delete arr[i];
			arr[i] = nullptr;
		}
			
	}
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
			_vec[i] = nullptr;
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
		{
			delete pair.second;
		}
			
	}

	_map.clear();
}

template<typename T>
T Lerp(T Start, T End, float t)
{
	return Start + t * (End - Start);
}



