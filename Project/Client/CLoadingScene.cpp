#include "pch.h"
#include "CLoadingScene.h"
#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CFontMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CInstancingBuffer.h>
#include <Engine\CPhysxMgr.h>

CLoadingScene::CLoadingScene():
	m_bCompleted(false)
{

}

CLoadingScene::~CLoadingScene()
{
	delete m_pLoadThread;
	m_pLoadThread = nullptr;
}


void CLoadingScene::init()
{
	std::mutex mutex;

	//load resources
	m_pLoadThread = new std::thread(&CLoadingScene::resources_load, this, std::ref(mutex));
}

void CLoadingScene::resources_load(std::mutex& _mutex)
{
	//�Ӱ迵��
	//���� ������ �۾��� �ϱ� ������ �ٸ� �� �����忡�� �۾��� ���� ���ϰ� ��
	_mutex.lock();
	{
		//CResMgr::GetInst()->init();
		//CRenderMgr::GetInst()->init();
		//
		//CFontMgr::GetInst()->init();
		//
		//CLevelMgr::GetInst()->init();
		//
		////CNavMeshMgr::GetInst()->init();
		//
		//CInstancingBuffer::GetInst()->init();
		//
		//CPhysxMgr::GetInst()->init();
	}
	_mutex.unlock();

	//complete
	m_bCompleted = true;
}

void CLoadingScene::tick()
{
	if (m_bCompleted)
	{
		//���� ���ξ����尡 ����Ǿ��µ� �ڽ� �����尡 ���Ҵٸ�
		//�ڽ� �����带 ���ξ����忡 ���Խ��Ѽ� ����Ǳ������� block
		m_pLoadThread->join();

		//���ξ������ ������ ���� ���� �������� ������ �����
		//m_pLoadThread->detach();
	}
}

void CLoadingScene::render()
{

}

void CLoadingScene::OnEnter()
{

}

void CLoadingScene::OnExit()
{

}


