#pragma once
#include <Engine\CSingleton.h>
#include <functional>
#include <condition_variable>

using std::condition_variable;
using std::mutex;
using std::thread;

class CLoadingScene 
	: public CSingleton< CLoadingScene>
{

	SINGLE(CLoadingScene);

private:
	//로딩 성공
	//bool m_bCompleted;
	//로딩 실패
	bool m_bLoadingFalse;

	//렌더링 준비
	bool m_bRender;

	vector<thread> m_vecLoadThread;
	mutex m_Mutex;

	//처음 생성될 레벨
	std::function<void()> m_fpCreateLevel;
	condition_variable m_CV;

	//전체 로딩 단계
	float m_fCurProgress;
	atomic<int> m_iTotalRes;
	atomic<int> m_iLoadingRes;

	bool m_bResLoad;
private:
	void resources_load(mutex& _mutex);
	void load_imgui();
	void load_mgr();
	void load_editor();
	
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

