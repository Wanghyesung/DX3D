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

	//atomic<int>�� ���� ��ŭ ������Ʋ ������ ���� ������ ���� �÷��̾ �Ҹ��� �� ����

	bool m_bEnginUpdate; //������ ������Ʈ ���� �� 
	mutex m_mutex;
	condition_variable m_CV; //���� ������� ����ȭ 

public:
	void AddMonsterFSM(CMonsterFSM* _pMonsterFSM);
	void DeleteMonsterFSM(UINT _iMonsterID);

	void SetUpdateEngine();

public:
	static void CalculateFSM();

	friend class CMonsterFSM;
};

