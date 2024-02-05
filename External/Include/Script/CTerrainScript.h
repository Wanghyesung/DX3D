#pragma once
#include <Engine\CScript.h>

class CTerrainScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other)override;
    virtual void OnOverlap(CCollider3D* _Other)override;
    virtual void EndOverlap(CCollider3D* _Other)override;

   
    void Initialize(const wstring& _strFbxName);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;
    CLONE(CTerrainScript);

public:
    CTerrainScript();
    ~CTerrainScript();
};

