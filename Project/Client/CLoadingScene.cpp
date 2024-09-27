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
m_bCompleted, m_bRender, m_iLoadingRes�� ���� ������ std::atomic���� �����ϸ� �ڵ尡 ����������,
mutex�� ���� ������ ���� �� �ֽ��ϴ�. ������, ��� ������ ���� atomic�� ����� �ʿ�� ������,
�ַ� ����ȭ�� �ʿ��� �ܼ��� �÷��׳� ī���Ϳ� ����ϴ� ���� �����ϴ�.
*/


/*
std::condition_variable�� C++���� ������ ���� ����ȭ�� ���� ���Ǵ� Ŭ�����Դϴ�. 
���� �����尡 Ư�� ������ ������ ������ ��ٸ��ų�, Ư�� ������ ������ �� �ٸ� �����忡�� �̸� �˸��� �� ���˴ϴ�.
*/

/*
std::condition_variable�� ������ ���� �� ���� �ֿ� �Լ��� �����˴ϴ�:

wait() �Լ�: Ư�� ������ ������ ������ �����带 ���(block)�մϴ�.
notify_one() �� notify_all() �Լ�: ��� ���� �����忡�� Ư�� ������ �����Ǿ����� �˸��ϴ�.
std::condition_variable�� �׻� std::mutex�� �Բ� ���˴ϴ�. std::mutex�� ������ ���� ������ ������ �����ϰ� �����ϰ�,
std::condition_variable�� �����尡 �����ϰ� ��ٸ���, ��� �� �ֵ��� �����ݴϴ�.
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
	//m_pLoadThread->detach();//�����忡�� �۾�
	//m_pLoadThread->join();
	return TRUE;
}

void CLoadingScene::resources_load(mutex& _mutex)
{
	//�Ӱ迵��
	//���� ������ �۾��� �ϱ� ������ �ٸ� �� �����忡�� �� �۾��� ���� ���ϰ� ��
	//_mutex.lock();
	//{
	//	m_bRender = CEngine::GetInst()->init_mgr();
	//	++m_iLoadingRes;
	//
	//	CEditorObjMgr::GetInst()->init();
	//	++m_iLoadingRes;
	//
	//	// ImGui �ʱ�ȭ
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
	//std::unique_lock�� �����ڿ��� �ڵ����� ���ؽ��� ��װ�, 
	// �Ҹ��ڿ��� �ڵ����� ���ؽ��� �����մϴ�.�̷� ���� ���ܰ� �߻��ص�
	// ���ؽ��� �����ǹǷ� ���ҽ� ������ ������ �� �ֽ��ϴ�.
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
	//���ҽ��� ���� �ε�ǰ� �ε��ۼ�Ʈ�� 100���� ���� level�� ����

	if (m_iLoadingRes == m_iTotalRes && m_fCurProgress >= 99.f)
	{
		//���� ���ξ����尡 ����Ǿ��µ� �ڽ� �����尡 ���Ҵٸ�
		//�ڽ� �����带 ���ξ����忡 ���Խ��Ѽ� ����Ǳ������� block
		for (auto& thread : m_vecLoadThread)
		{
			thread->join();	
		}

		//���ξ������ ������ ���� ���� �������� ������ �����
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
		//���� ����Ÿ�� clear
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->ClearTarget();

		//�ε�â ������
		CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

		static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"LoadingMtrl");
		//
		static bool bSet = false;
		if (!bSet)
		{
			//�ؽ��� ���� �ε�
			wstring strFileName = L"texture\\GameTexture\\Loading_.png";
			//
			bSet = true;
			pMtrl->SetTexParam(TEX_0,
				CResMgr::GetInst()->Load<CTexture>(strFileName, strFileName));
		}

		pMtrl->UpdateData();
		pRectMesh->render(0);

		//��ü �ε� ���� �ܰ迡 ���� �ۼ�Ʈ�� ǥ��
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


