//=============================================================================
//
// タイトル処理 [titlelogo.h]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_TITLELOGO		(2)	// ポリゴン数

#define TEXTURE_GAME_TITLELOGO_00	_T("data/TEXTURE/title_back.png")	// タイトル用画像
#define TEXTURE_GAME_TITLELOGO_01	_T("data/TEXTURE/title.png")	// タイトル用画像

#define TEXTURE_TITLELOGO_SIZE_X	(1050 * SCREEN_SCALE/2) // テクスチャサイズ
#define TEXTURE_TITLELOGO_SIZE_Y	(250 * SCREEN_SCALE/2) // 同上

#define TITLELOGO_POS_X				(SCREEN_CENTER_X)				// 初期座標X
#define TITLELOGO_POS_Y				(150 * SCREEN_SCALE)			// 初期座標Y

#define TITLELOGO_ALPHA_MIN			(0.15f)
#define TITLELOGO_ALPHA_MAX			(0.98f)
#define TITLELOGO_ALPHA_CHANGE		(0.01f)

#define TEXTURE_PATTERN_DIVIDE_X_TITLELOGO	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_TITLELOGO			(TEXTURE_PATTERN_DIVIDE_X_TITLELOGO*TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO)	// アニメーションパターン数
#define TIME_ANIMATION_TITLELOGO				(4)	// アニメーションの切り替わるカウント

#define TITLELOGO_MAX					(2)	// タイトルの最大数

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// テクスチャ情報
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	D3DXVECTOR3				pos;					// タイトルの移動量
	D3DXVECTOR3				rot;					// タイトルの回転量
	D3DXVECTOR2				size;					// タイトルのテクスチャサイズ(x,y)
	float					fAlpha;					// α値
	bool					bAlpha;					// α値変更フラグ
	bool					bUse;					// true:使用  false:未使用
}TITLELOGO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitlelogo(int nType);
void UninitTitlelogo(void);
void UpdateTitlelogo(void);
void DrawTitlelogo(void);
TITLELOGO *GetTitlelogo(int no);


#endif
