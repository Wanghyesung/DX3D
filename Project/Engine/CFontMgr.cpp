#include "pch.h"
#include "CFontMgr.h"
#include "CDevice.h"


CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
	, m_pFontWrapper(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	if (nullptr != m_pFontWrapper)
		m_pFontWrapper->Release();
}

void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_fFontSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::render()
{
	for (int i = 0; i < m_vecFont.size(); ++i)
	{
		CFontMgr::GetInst()->DrawFont(m_vecFont[i].Str.c_str(),
			m_vecFont[i].fPosX, m_vecFont[i].fPosY, m_vecFont[i].fFontSize, m_vecFont[i].Color);
	}

	m_vecFont.clear();
}


void CFontMgr::AddFont(const wstring& _strName, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	tFontInfo tFont = {};
	tFont.Str = _strName;
	tFont.fPosX = _fPosX;
	tFont.fPosY = _fPosY;
	tFont.fFontSize = _fFontSize;
	tFont.Color = _Color;
	m_vecFont.push_back(tFont);
}
