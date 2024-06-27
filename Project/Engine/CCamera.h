#pragma once
#include "CComponent.h"

#include "CFrustum.h"

class CCamera :
    public CComponent
{
private:
    CFrustum    m_Frustum;

    float       m_fAspectRatio;
    float       m_fScale;       // Orthograpic 에서 사용하는 카메라 배율
    float       m_Far;
    float       m_FOV;

    float       m_OrthoWidth;
    float       m_OrthoHeight;

    PROJ_TYPE   m_ProjType;

    Matrix      m_matView;
    Matrix      m_matViewInv;
    Matrix      m_matPrevView; //이전프레임 뷰 행렬

    Matrix      m_matProj;
    Matrix      m_matProjInv;
    Matrix      m_matPrevProj; //이전프레임 투영 행렬

    UINT        m_iLayerMask;

    int         m_iCamIdx;  // 카메라 우선순위
    tRay                    m_ray;      // 마우스 방향을 향하는 직선


    map<ULONG64, vector<tInstObj>>      m_mapInstGroup_S;       // shadow 
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

    //vector<CGameObject*>    m_vecDeferred;
    //vector<CGameObject*>    m_vecDeferredDecal;

    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecShadow;

    vector<CGameObject*>    m_vecBlur;



public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetFar(float _fFar) { m_Far = _fFar; }
    float GetFar() { return m_Far; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetFOV(float _Radian) { m_FOV = _Radian; }
    float GetFOV() { return m_FOV; }

    void SetOrthoWidth(float _width) { m_OrthoWidth = _width; }
    void SetOrthoHeight(float _height) { m_OrthoHeight = _height; }

    float GetorthoWidth() { return m_OrthoWidth; }
    float GetOrthoHeight() { return m_OrthoHeight; }

    void SetCameraIndex(int _idx);

    const tRay& GetRay() { return m_ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }


    //현재 카메라의 핼렬로 transform상수버퍼 초기화
    void UpdateMatrix();
public:
    //재질에 참조하는 쉐이더에 따라서 (쉐이더 도메인 (투명, 반투명, 후처리) )
    //쉐이더 도메인에 따라서 렌더링 순서 분류

    void SortObject();
    void SortObject_Shadow();//광원 카메라 전용
    void render();
    void render_shadowmap();//광원 카메라 전용 깊이값 저장

public:
    virtual void begin() override;
    virtual void finaltick() override;

protected:
    void CalRay();  // 마우스 방향으로 광선 연산

private:
    void clear();
    void clear_shadow();

    
    void render_blur();
    void render_opaque();
    void render_deferred();
    void render_forward();//mask
    void render_decal();
    void render_transparent();
    void render_postprocess();
    void render_ui();

    void render_middle(); //중간 병합
    void render_final(); //최종 병합

    void CalcViewMat();
    void CalcProjMat();
   

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCamera);
public:
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};

