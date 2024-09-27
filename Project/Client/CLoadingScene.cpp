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

/*
m_bCompleted, m_bRender, m_iLoadingRes와 같은 변수를 std::atomic으로 변경하면 코드가 간결해지고,
mutex로 인한 병목을 줄일 수 있습니다. 하지만, 모든 변수에 대해 atomic을 사용할 필요는 없으며,
주로 동기화가 필요한 단순한 플래그나 카운터에 사용하는 것이 좋습니다.
*/


/*
std::condition_variable은 C++에서 스레드 간의 동기화를 위해 사용되는 클래스입니다. 
여러 스레드가 특정 조건을 만족할 때까지 기다리거나, 특정 조건이 만족될 때 다른 스레드에게 이를 알리는 데 사용됩니다.
*/

/*
std::condition_variable은 다음과 같은 두 가지 주요 함수로 구성됩니다:

wait() 함수: 특정 조건이 만족될 때까지 스레드를 블록(block)합니다.
notify_one() 및 notify_all() 함수: 대기 중인 스레드에게 특정 조건이 만족되었음을 알립니다.
std::condition_variable은 항상 std::mutex와 함께 사용됩니다. std::mutex는 스레드 간의 데이터 접근을 안전하게 보장하고,
std::condition_variable은 스레드가 안전하게 기다리고, 깨어날 수 있도록 도와줍니다.
*/

CLoadingScene::CLoadingScene() :
	m_bLoadingFalse(false),
	m_bRender(false),
	m_fpCreateLevel(nullptr),
	m_Mutex{},
	m_fCurProgress(0.f),
	m_iTotalRes(3),
	m_iLoadingRes(0),
	m_bResLoad(false)
{

}

CLoadingScene::~CLoadingScene()
{
	for (int i = 0; i < m_vecLoadThread.size(); ++i)
	{
		delete m_vecLoadThread[i];
		m_vecLoadThread[i] = nullptr;
	}
	
}


bool CLoadingScene::init()
{
	//load resources
	m_vecLoadThread.push_back(new thread(&CLoadingScene::load_mgr, this, std::ref(m_iLoadingRes)));
	m_vecLoadThread.push_back(new thread(&CLoadingScene::load_editor, this, std::ref(m_iLoadingRes)));
	m_vecLoadThread.push_back(new thread(&CLoadingScene::load_imgui, this, std::ref(m_iLoadingRes)));

	//for (auto& thread : m_vecLoadThread) 
	//{
	//	if (m_iLoadingRes == 3)
	//	{
	//		thread->join();
	//	}
	//}

	//m_pLoadThread = new std::thread(&CLoadingScene::resources_load, this, std::ref(m_Mutex));
	//m_pLoadThread->detach();//백드라운드에서 작업
	//m_pLoadThread->join();
	return TRUE;
}

void CLoadingScene::resources_load(mutex& _mutex)
{
	//임계영역
	//현재 데이터 작업을 하기 때문에 다른 쪽 스레드에서 이 작업을 하지 못하게 함
	//_mutex.lock();
	//{
	//	m_bRender = CEngine::GetInst()->init_mgr();
	//	++m_iLoadingRes;
	//
	//	CEditorObjMgr::GetInst()->init();
	//	++m_iLoadingRes;
	//
	//	// ImGui 초기화
	//	ImGuiMgr::GetInst()->init(g_hWnd);
	//	++m_iLoadingRes;
	//}
    //_mutex.unlock();
	//
	////complete
	//m_bCompleted = true;
}

void CLoadingScene::load_imgui(int _iLoadingRes)
{
	std::unique_lock<mutex> lock(m_Mutex);
	m_CV.wait(lock, [this]() {return m_bResLoad; });

	ImGuiMgr::GetInst()->init(g_hWnd);
	++m_iLoadingRes;
}

void CLoadingScene::load_mgr(int _iLoadingRes)
{
	//std::unique_lock은 생성자에서 자동으로 뮤텍스를 잠그고, 
	// 소멸자에서 자동으로 뮤텍스를 해제합니다.이로 인해 예외가 발생해도
	// 뮤텍스가 해제되므로 리소스 누수를 방지할 수 있습니다.
	std::unique_lock<mutex> lock(m_Mutex);

	m_bRender = CEngine::GetInst()->init_mgr();
	++m_iLoadingRes;
	m_bResLoad = true;

	m_CV.notify_all();
}

void CLoadingScene::load_editor(int _iLoadingRes)
{
	std::unique_lock<mutex> lock(m_Mutex);
	m_CV.wait(lock, [this]() {return m_bResLoad; });

	CEditorObjMgr::GetInst()->init();
	++m_iLoadingRes;
}

bool CLoadingScene::tick()
{
	CTimeMgr::GetInst()->tick();
	//리소스가 전부 로드되고 로딩퍼센트가 100까지 차면 level로 진입

	if (m_iLoadingRes == m_iTotalRes && m_fCurProgress >= 99.f)
	{
		//만약 메인쓰레드가 종료되었는데 자식 스레드가 남았다면
		//자식 쓰레드를 메인쓰레드에 편입시켜서 종료되기전까지 block
		for (auto& thread : m_vecLoadThread)
		{
			thread->join();	
		}

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

		CDevice::GetInst()->Present(); 
	}

}

void CLoadingScene::OnEnter()
{
	//m_bCompleted = false;
	m_bLoadingFalse = false;
}

void CLoadingScene::OnExit()
{

}


