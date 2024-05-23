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
	m_fCurTime(0.f),
	m_fLoadingTime(2.f)
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

		CEditorObjMgr::GetInst()->init();

		// ImGui 초기화
		ImGuiMgr::GetInst()->init(g_hWnd);

	}
    //_mutex.unlock();

	//complete
	m_bCompleted = true;
}

bool CLoadingScene::tick()
{
	CTimeMgr::GetInst()->tick();
	if (m_bCompleted)
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

		Vec2 vPos = CEngine::GetInst()->GetWindowResolution() / 2.f;

		m_fCurTime += DT;
		if (m_fCurTime >= m_fLoadingTime)
			m_fCurTime = 0.f;

		float fTime = (m_fLoadingTime - m_fCurTime) * 3;
		int iNum = fTime;
		if (iNum >= 4)
			int a = 10;
		wstring strStack = L"";
		for (int i = 0; i < iNum; ++i)
			strStack += L".";

	

		CFontMgr::GetInst()->AddFont(L"LOADING" + strStack, vPos.x -100.f, vPos.y,60.f, FONT_RGBA(255, 255, 255, 255));
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


