#pragma once
#include "CComponent.h"

class CCollider3D : public CComponent
{

private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    Vec3            m_vOffsetRot;

    bool            m_bAbsolute;
    COLLIDER3D_TYPE m_Shape;
    Matrix          m_matCollider3D;    // Collider 의 월드행렬

    int             m_iCollisionCount;  // 충돌 횟수


public:
    virtual void finaltick() override;

public:
    void SetOffsetPos(Vec3 _vOffsetPos);
    void SetOffsetScale(Vec3 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, _vOffsetScale.z); }
    void SetOffsetRot(Vec3 _vRot) { m_vOffsetRot = _vRot; }

    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetCollider2DType(COLLIDER3D_TYPE _Type) { m_Shape = _Type; }

    const Matrix& GetColliderWorldMat() { return m_matCollider3D; }
    const Vec3& GetWorldPos();

    Vec3 GetOffsetPos() { return m_vOffsetPos; }
    Vec3 GetOffsetScale() { return m_vOffsetScale; }
    Vec3 GetOffsetRot() { return m_vOffsetRot; }

    bool GetIsAbsolute() { return m_bAbsolute; }
public:

    void BeginOverlap(CCollider3D* _Other);
    void OnOverlap(CCollider3D* _Other);
    void EndOverlap(CCollider3D* _Other);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCollider3D);
public:
    CCollider3D();
    ~CCollider3D();

};

