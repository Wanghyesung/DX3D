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
	bool m_bCompleted;

	std::thread* m_pLoadThread;
private:
	void resources_load(std::mutex& _mutex);

public:
	bool IsCompleted() { return m_bCompleted; }
public:
	void init();
	void tick();
	void render();

	void OnEnter();
	void OnExit();


};

