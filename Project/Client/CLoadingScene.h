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
	//�ε� ����
	//bool m_bCompleted;
	//�ε� ����
	bool m_bLoadingFalse;

	//������ �غ�
	bool m_bRender;

	vector<thread> m_vecLoadThread;
	mutex m_Mutex;

	//ó�� ������ ����
	std::function<void()> m_fpCreateLevel;
	condition_variable m_CV;

	//��ü �ε� �ܰ�
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

