#include "pch.h"
#include "CLoadingScene.h"
#include "ImGuiMgr.h"
#include "CEditorObjMgr.h"

#include <Engine\CEngine.h>
#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CFontMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CInstancingBuffer.h>
#include <Engine\CPhysxMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CMRT.h>
#include <Engine\CPathMgr.h>
#include <Engine\CDevice.h>
#include <Engine\CFontMgr.h>
#include <Engine\CTimeMgr.h>
extern HWND g_hWnd;

CLoadingScene::CLoadingScene() :
	m_bCompleted(false),
	m_bLoadingFalse(false),
	m_bRender(false),
	m_fpCreateLevel(nullptr),
	m_fCurProgress(0.f),
	m_iLoadingRes(0),
	m_iTotalRes(3)
{

}

CLoadingScene::~CLoadingScene()
{
	delete m_pLoadThread;
	m_pLoadThread = nullptr;
}


bool CLoadingScene::init()
{
	//std::mutex mutex;

	//load resources
	m_pLoadThread = new std::thread(&CLoadingScene::resources_load, this);

	//m_pLoadThread->detach();//백드라운드에서 작업
	//m_pLoadThread->join();
	return TRUE;
}

void CLoadingScene::resources_load()
{
	//임계영역
	//현재 데이터 작업을 하기 때문에 다른 쪽 스레드에서 작업을 하지 못하게 함
	//_mutex.lock();
	{
		m_bRender = CEngine::GetInst()->init_mgr();

		++m_iLoadingRes;

		CEditorObjMgr::GetInst()->init();
		++m_iLoadingRes;

		// ImGui 초기화
		ImGuiMgr::GetInst()->init(g_hWnd);
		++m_iLoadingRes;
	}
    //_mutex.unlock();

	//complete
	m_bCompleted = true;
}

bool CLoadingScene::tick()
{
	CTimeMgr::GetInst()->tick();
	//리소스가 전부 로드되고 로딩퍼센트가 100까지 차면 level로 진입
	if (m_bCompleted && m_fCurProgress >= 99.f) 
	{
		//만약 메인쓰레드가 종료되었는데 자식 스레드가 남았다면
		//자식 쓰레드를 메인쓰레드에 편입시켜서 종료되기전까지 block
		m_pLoadThread->join();

		//메인쓰레드와 완전히 분히 시켜 독립전인 스레드 운영가능
		//m_pLoadThread->detach();
		
		//level enter
		if (m_fpCreateLevel != nullptr)
			m_fpCreateLevel();

		return true;
	}

	return false;
}

void CLoadingScene::render()
{
	if (m_bRender)
	{
		//메인 렌더타겟 clear
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->ClearTarget();

		//로딩창 렌더링
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

		static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"LoadingMtrl");
		//
		static bool bSet = false;
		if (!bSet)
		{
			//텍스쳐 따로 로딩
			wstring strFileName = L"texture\\GameTexture\\Loading_.png";
			//
			bSet = true;
			pMtrl->SetTexParam(TEX_0,
				CResMgr::GetInst()->Load<CTexture>(strFileName, strFileName));
		}

		pMtrl->UpdateData();
		pRectMesh->render(0);

		//전체 로딩 진행 단계에 따라 퍼센트로 표현
		float progress = (float)m_iLoadingRes / m_iTotalRes * 100.0f;
		m_fCurProgress = Lerp<float>(m_fCurProgress, progress, DT *2.f);
	    wstring strTotal = std::to_wstring((int)m_fCurProgress);

		Vec2 vPos = CEngine::GetInst()->GetWindowResolution() / 2.f;
		CFontMgr::GetInst()->AddFont(L"Loading : " + strTotal, vPos.x - 150.f, vPos.y, 60.f, FONT_RGBA(255, 255, 255, 255));
		CFontMgr::GetInst()->render();

		CDevice::GetInst()->Present(); // 렌더 종료
	}

}

void CLoadingScene::OnEnter()
{
	m_bCompleted = false;
	m_bLoadingFalse = false;
}

void CLoadingScene::OnExit()
{

}


