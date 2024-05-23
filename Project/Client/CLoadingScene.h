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
	//�ε� ����
	bool m_bCompleted;
	//�ε� ����
	bool m_bLoadingFalse;

	//������ �غ�
	bool m_bRender;
	std::thread* m_pLoadThread;

	//ó�� ������ ����
	std::function<void()> m_fpCreateLevel;

	//��ü �ε� �ܰ�
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

