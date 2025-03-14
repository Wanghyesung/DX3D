#pragma once

class CMonsterFSMMgr : public CSingleton<CMonsterFSMMgr>
{
	SINGLE(CMonsterFSMMgr);

private:
	static map<UINT, CMonsterFSM*> m_mapMonsterFSM;
	static vector<UINT> m_vecDeleteExpected;
	static vector<CMonsterFSM*> m_vecAddExpected;

	thread m_FSMThread;
	atomic<bool> m_bRunning;

	//atomic<int>로 늦은 만큼 업데이틀 받으면 몬스터 로직에 따라 플레이어가 불리할 수 있음

	bool m_bEnginUpdate; //엔진이 업데이트 받을 때 
	mutex m_mutex;
	condition_variable m_CV; //메인 스레드와 동기화 

public:
	void AddMonsterFSM(CMonsterFSM* _pMonsterFSM);
	void DeleteMonsterFSM(UINT _iMonsterID);

	void SetUpdateEngine();

public:
	static void CalculateFSM();

	friend class CMonsterFSM;
};

