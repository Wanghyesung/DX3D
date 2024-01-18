#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_RWBuffer;
    CStructuredBuffer* m_ModuleDataBuffer;

    tParticleModule             m_ModuleData;
    Ptr<CParticleUpdateShader>  m_UpdateCS;

    float                       m_AccTime;


public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    void SetParticleModule(const tParticleModule& _pModule) { m_ModuleData = _pModule; }
    const tParticleModule& GetParticleMoudle() { return m_ModuleData; }

    CLONE(CParticleSystem);

    void SetDir(Vec3 _vDir) { m_ModuleData.vVelocityDir = _vDir; }
public:
    CParticleSystem();
    ~CParticleSystem();
};

