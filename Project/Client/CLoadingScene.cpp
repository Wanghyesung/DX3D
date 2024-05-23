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

	//m_pLoadThread->detach();//�����忡�� �۾�
	//m_pLoadThread->join();
	return TRUE;
}

void CLoadingScene::resources_load()
{
	//�Ӱ迵��
	//���� ������ �۾��� �ϱ� ������ �ٸ� �� �����忡�� �۾��� ���� ���ϰ� ��
	//_mutex.lock();
	{
		m_bRender = CEngine::GetInst()->init_mgr();

		CEditorObjMgr::GetInst()->init();

		// ImGui �ʱ�ȭ
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
		//���� ���ξ����尡 ����Ǿ��µ� �ڽ� �����尡 ���Ҵٸ�
		//�ڽ� �����带 ���ξ����忡 ���Խ��Ѽ� ����Ǳ������� block
		m_pLoadThread->join();

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

		CDevice::GetInst()->Present(); // ���� ����
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


