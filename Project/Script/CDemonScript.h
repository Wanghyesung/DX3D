#pragma once
#include "CMonsterScript.h"

class CDemonScript : public CMonsterScript
{
private:
    CGage* m_pGage;
    CEngineUI* m_pFrame;

    Vec3 m_vJumpForce; //공격, 점프..


private:
    void jump_start();
    void jump_end();

    void set_attack();
    
protected:
    virtual void init_hp()override;
public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;


    virtual void begin();

    virtual void Initialize(const wstring& _strFbxName)override;
public:
	CDemonScript();
	virtual ~CDemonScript();

};

