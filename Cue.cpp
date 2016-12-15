///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cue.cpp
// Cueオブジェクトに対して物理干渉する
// プレイヤが直接干渉出来るオブジェクト
//	ryuji oka
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "Cue.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Manager.h"
#include "InputKeyboard.h"
#include "Ball.h"
#include <math.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// マクロ定義
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TOP (4)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// プロトタイプ宣言
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 Cue::m_sPos = D3DXVECTOR3(NULL,NULL,NULL);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Cue::Cue(int nPriority)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Cue::~Cue()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Init関数
//	:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Cue::Init(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	m_fpos = pos;

	return S_OK;
}
//////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//	Uninit関数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::Uninit(void)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Update関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::Update(void)
{

	/* 白球を中点として回転移動させる */
	
	if(InputKeyboard::GetKeyboardPress(DIK_A))
	{
		m_rot.y+=0.02f;
	}
	
	if(InputKeyboard::GetKeyboardPress(DIK_D))
	{
		m_rot.y-=0.02f;
	}

	D3DXVECTOR3 wBallPos = Ball::GetWhiteBallPosition();

	m_pos.x = (cosf(m_rot.y)*m_fpos.x - sinf(m_rot.y)*m_fpos.z) + wBallPos.x;
	m_pos.z = (sinf(m_rot.y)*m_fpos.x + cosf(m_rot.y)*m_fpos.z) + wBallPos.z;

	/* マウス */
	
	// 白球を打つ
	if(InputKeyboard::GetKeyboardTrigger(DIK_SPACE))
	{
		D3DXVECTOR3 addvec = wBallPos - m_pos;
		
		D3DXVec3Normalize(&addvec,&addvec);
		addvec *= 10;
		addvec.y = 0;
		Ball::SetWhiteBallAddVector(addvec);
	}

	m_sPos = m_pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw関数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::Draw(void)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Create関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Cue* Cue::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int nPriority)
{
	Cue *pRet = new Cue(nPriority);
	pRet->Init(pos,rot);
	return pRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetPosition関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::SetPosition(float x,float y,float z)
{
	m_pos.x=x;
	m_pos.y=y;
	m_pos.z=z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetPosition関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::SetPosition(D3DXVECTOR3 pos)
{
	m_pos=pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetRotation関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::SetRotation(D3DXVECTOR3 rot)
{
	m_rot=rot;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetRotation関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cue::SetRotation(float x,float y,float z)
{
	m_rot.x=x;
	m_rot.y=y;
	m_rot.z=z;
}