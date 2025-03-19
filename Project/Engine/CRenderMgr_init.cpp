#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"

#include "CStructuredBuffer.h"
#include "CMRT.h"

#include "CResMgr.h"
#include "CTexture.h"

void CRenderMgr::init()
{
    // Light2DBuffer 구조화 버퍼 생성
    m_Light2DBuffer = new CStructuredBuffer;
    m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // Light3DBuffer 구조화 버퍼 생성
    m_Light3DBuffer = new CStructuredBuffer;
    m_Light3DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // MRT 생성
    CreateMRT();
}

void CRenderMgr::CreateMRT()
{
    // ====================
    // SwapChain MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

        Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrRTTex, 1, pDSTex);
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetClearColor(Vec4(0.3f, 0.3f, 0.3f, 1.f), 0);
    }
    
    //deferred MRT 후처리 기능
    //4종류 렌더타켓,DS는 스왑체인용 DS공용으로
    //diffus색상, normal 표면벡터, position 위치, data 나중에 필요한 데이터 있으면 사용


    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"ColorTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"NormalTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[2] = CResMgr::GetInst()->CreateTexture(L"PositionTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[3] = CResMgr::GetInst()->CreateTexture(L"EmissiveTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[4] = CResMgr::GetInst()->CreateTexture(L"DataTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrRTTex, 5, nullptr);
    }

    // ====================
    // motionblur MRT 만들기
    // ====================
    {
        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        m_MRT[(UINT)MRT_TYPE::BOTION_BLUER] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"VelocityTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->FindRes<CTexture>(L"DataTargetTex");

        m_MRT[(UINT)MRT_TYPE::BOTION_BLUER]->Create(arrRTTex, 2, nullptr);
    }


    // ====================
    // Decal MRT 만들기
    // ====================
    {
        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL] = new CMRT;

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex");
        arrRTTex[1] = CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex");

        m_MRT[(UINT)MRT_TYPE::DEFERRED_DECAL]->Create(arrRTTex, 2, nullptr);
    }

    //lightMgr만들기

    {
        m_MRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};
        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[1] = CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R8G8B8A8_UNORM
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        arrRTTex[2] = CResMgr::GetInst()->CreateTexture(L"ShadowTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::LIGHT]->Create(arrRTTex, 3, nullptr);
    }

    //중간 병합 MRT (모션블러 ...) 합친값을 middletargetTex에 넣기
    // ====================
   // MiddleMap MRT 만들기
   // ====================
    {
        m_MRT[(UINT)MRT_TYPE::MIDDLE] = new CMRT;

        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {};

        arrRTTex[0] = CResMgr::GetInst()->CreateTexture(L"MiddleTargetTex", vResol.x, vResol.y
            , DXGI_FORMAT_R32G32B32A32_FLOAT
            , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        m_MRT[(UINT)MRT_TYPE::MIDDLE]->Create(arrRTTex, 1, nullptr);
    }



    // ====================
   // ShadowMap MRT 만들기
   // ====================
    {
        m_MRT[(UINT)MRT_TYPE::NEAR_SHADOWMAP] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::MID_SHADOWMAP] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::FAR_SHADOWMAP] = new CMRT;

        /*
        카메라 거리에 따라 해상도 다르게 GPU가 계산해야 하는 픽셀 수가 줄어들어 연산량이 감소를 목표
        텍스처의 해상도가 클수록 샘플링 비용이 올라감
        해상도를 낮추면 텍스처 접근 비용이 감소
        */

        Vec2 arrResol[3] = { Vec2(4096, 4096) , Vec2(1024, 1024) , Vec2(512, 512) };
        //Vec2 vResol = Vec2(4096, 4096);
        Ptr<CTexture> RTTex = {};
       
        UINT iShadowMRT = (UINT)MRT_TYPE::NEAR_SHADOWMAP;
        for (int i = 0; i < 3; ++i)
        {
            RTTex = CResMgr::GetInst()->CreateTexture(L"DynamicShadowMapTex" + std::to_wstring(i), arrResol[i].x, arrResol[i].y
                , DXGI_FORMAT_R32_FLOAT
                , D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


            Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"DynamicShadowMapDepthTex" + std::to_wstring(i), arrResol[i].x, arrResol[i].y
                , DXGI_FORMAT_D32_FLOAT
                , D3D11_BIND_DEPTH_STENCIL);


            m_MRT[iShadowMRT++]->Create(&RTTex, 1, pDSTex);
        }
    }

}

void CRenderMgr::ClearMRT()
{
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
    {
        if (nullptr != m_MRT[i])
        {
            m_MRT[i]->ClearTarget();
        }
    }
}