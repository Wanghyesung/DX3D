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
	//임계영역
	//현재 데이터 작업을 하기 때문에 다른 쪽 스레드에서 작업을 하지 못하게 함
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
		//만약 메인쓰레드가 종료되었는데 자식 스레드가 남았다면
		//자식 쓰레드를 메인쓰레드에 편입시켜서 종료되기전까지 block
		m_pLoadThread->join();

		//메인쓰레드와 완전히 분히 시켜 독립전인 스레드 운영가능
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


