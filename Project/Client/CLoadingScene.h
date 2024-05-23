#pragma once
#include <Engine\CSingleton.h>
#include <thread>
#include <mutex>
#include <functional>


class CLoadingScene 
	: public CSingleton< CLoadingScene>
{

	SINGLE(CLoadingScene);

private:
	//로딩 성공
	bool m_bCompleted;
	//로딩 실패
	bool m_bLoadingFalse;

	//렌더링 준비
	bool m_bRender;
	std::thread* m_pLoadThread;

	//처음 생성될 레벨
	std::function<void()> m_fpCreateLevel;

	//전체 로딩 단계
	float m_fCurProgress;
	int m_iLoadingRes;
	int m_iTotalRes;

private:
	void resources_load(/*std::mutex& _mutex*/);


public:
	void SetCreateLevelFunc(std::function<void()> _pFunc){ m_fpCreateLevel = std::move(_pFunc); }

	bool ISLoadingFalse() { return m_bLoadingFalse; }
public:
	bool init();
	bool tick();
	void render();

	void OnEnter();
	void OnExit();


};

