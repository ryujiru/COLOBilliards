///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CollisionManager.cpp
// 登録されたオブジェクトの衝突管理
//	ryuji oka
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "Manager.h"
#include "Ball.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// マクロ定義
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BALL_SIZE (5)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// プロトタイプ宣言
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ball *CollisionManager::m_pSceneList[MAX_BALL];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CollisionManager::CollisionManager(int nPriority)
{
	for (int n = 0; n<MAX_BALL; n++)
	{
		m_pSceneList[n] = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CollisionManager::~CollisionManager()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Init関数
//	:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CollisionManager::Init(void)
{

	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Uninit関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::Uninit(void)
{
	for(int n =0; n<MAX_BALL;n++)
	{
		if(m_pSceneList[n] != NULL)
		{
			m_pSceneList[n]->Release();
			m_pSceneList[n] = NULL;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Update関数
//	登録されたオブジェクト同士の衝突判定を行う
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::Update(void)
{
	int i = 0;
	int j = 1;
	// 今回は一般的なビリヤードなのでこういう形式を取ったが球の個数が増えると重くなる可能性の高いやり方です。
	for(int n =0; n<MAX_BALL; n++)
	{
		if(m_pSceneList[n] != NULL)
		{
			for(i=j; i<MAX_BALL; i++)
			{
				if(m_pSceneList[i] != NULL)
				{
					Collision(m_pSceneList[n],m_pSceneList[i]);
				}
			}
		}

		j++;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Collision関数
//	軽量化のためビリヤードに特化した判定(平面の上の"円"同士の衝突判定かつ大きさは定数)
//	衝突していた場合お互いの運動ベクトル、擬似的な回転(笑)などを考慮した結果をオブジェクトに送る
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::Collision(Ball *Obj1,Ball *Obj2)
{
	// 計算用に中心点の位置情報を一時格納
	D3DXVECTOR3 pos1 = Obj1->GetPosition();
	D3DXVECTOR3 pos2 = Obj2->GetPosition();

	// 衝突判定(円)
	if((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.z - pos1.z) * (pos2.z - pos1.z) <= (BALL_SIZE + BALL_SIZE) * (BALL_SIZE + BALL_SIZE))
	{
		/* 衝突していた場合 */

		// それぞれの運動ベクトルと回転を取得
		D3DXVECTOR3 vec1 = Obj1->GetAddVector();
		D3DXVECTOR3 vec2 = Obj2->GetAddVector();
		D3DXVECTOR3 spin1 = Obj1->GetSpin();
		D3DXVECTOR3 spin2 = Obj2->GetSpin();

		// 衝突後の運動量を計算 ////////////////////////////////
		
		// お互いの位置から発生するフラットなベクトルの算出
		// この場合相手の位置から自身の位置へと向かうベクトルのこと
		pos1.y=0;
		pos2.y=0;

		D3DXVECTOR3 posVec1 = pos1 - pos2;
		D3DXVECTOR3 posVec2 = pos2 - pos1;
		// 正規化
		D3DXVec3Normalize(&posVec1,&posVec1);
		D3DXVec3Normalize(&posVec2,&posVec2);

		// 反発係数補正
		//posVec1 = posVec1 * 0.1f;
		//posVec2 = posVec2 * 0.1f;

		// フラットなベクトルに運動ベクトルを合成
		D3DXVECTOR3 addvec1 = (posVec1 + vec1 + vec2)/2;
		D3DXVECTOR3 addvec2 = (posVec2 + vec2 + vec1)/2;

		// 衝突補正をベクトルに付与
		addvec1 *= 0.9f;
		addvec2 *= 0.9f;

		// 回転補正をベクトルに付与
		//addvec1 += (spin1 * 0.1f);
		//addvec1 += (spin2 * 0.1f);

		// 衝突後の回転を計算
		D3DXVECTOR3 n_spin;
		n_spin = spin1 - spin2;
		spin2 = spin2 - spin1;
		spin1 = n_spin;

		//////////////////////////////////////////////////////

		// それぞれのオブジェクトに計算結果をSetする
		Obj1->SetAddVector(addvec1);
		Obj1->SetSpin(spin1);
		Obj2->SetAddVector(addvec2);
		Obj2->SetSpin(spin2);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Draw関数
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::Draw(void)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Create関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CollisionManager* CollisionManager::Create(void)
{
	CollisionManager *pRet = new CollisionManager(1);
	pRet->Init();
	return pRet;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	AddCollision関数
//	publicかつstaticな関数で、外部から呼び出し登録できる
//	渡されたSceneオブジェクトをリストに登録する
//	(手抜き…じゃなくてビリヤードに特化した配列で管理)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CollisionManager::AddCollision(Ball *Object,int num)
{
	m_pSceneList[num] = Object;
	
	if(m_pSceneList[num] != NULL)
	{
		return true;
	}
	
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ReleaseCollision関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::ReleaseCollision(int num)
{
	m_pSceneList[num] = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetPosition関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::SetPosition(float x,float y,float z)
{
	m_pos.x=x;
	m_pos.y=y;
	m_pos.z=z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetPosition関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::SetPosition(D3DXVECTOR3 pos)
{
	m_pos=pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetRotation関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::SetRotation(D3DXVECTOR3 rot)
{
	m_rot=rot;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SetRotation関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionManager::SetRotation(float x,float y,float z)
{
	m_rot.x=x;
	m_rot.y=y;
	m_rot.z=z;
}