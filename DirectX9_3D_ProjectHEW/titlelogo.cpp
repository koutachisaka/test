//=============================================================================
//
// タイトル処理 [titlelogo.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "titlelogo.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitlelogo(int no);
void SetVertexTitlelogo(int no);
void SetDiffuseTitlelogo(int no, float fAlpha);
void SetTextureTitlelogo(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLELOGO					titlelogoWk[TITLELOGO_MAX];

// テクスチャへのポリゴン
LPDIRECT3DTEXTURE9		pD3DTextureTitlelogo[TITLELOGO_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitlelogo(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[0];

	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TITLELOGO_00,				// ファイルの名前
			&pD3DTextureTitlelogo[0]);			// 読み込むメモリのポインタ

		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TITLELOGO_01,				// ファイルの名前
			&pD3DTextureTitlelogo[1]);			// 読み込むメモリのポインタ
	}

	// タイトルの初期化
	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		titlelogo->bUse = true;
		titlelogo->bAlpha = true;
		titlelogo->pos = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
		titlelogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		titlelogo->fAlpha = TITLELOGO_ALPHA_MAX;

		titlelogo->Texture = pD3DTextureTitlelogo[i];			// テクスチャへのエネミー
		titlelogo->size = D3DXVECTOR2(TEXTURE_TITLELOGO_SIZE_X, TEXTURE_TITLELOGO_SIZE_Y);

		// 頂点情報の作成
		MakeVertexTitlelogo(i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitlelogo(void)
{
	TITLELOGO *titlelogo = &titlelogoWk[0];

	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		// メモリ解放
		if (&pD3DTextureTitlelogo[i] != NULL)
		{
			pD3DTextureTitlelogo[i]->Release();
			pD3DTextureTitlelogo[i] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitlelogo(void)
{
	TITLELOGO *titlelogo = &titlelogoWk[0];

	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		if (titlelogo->bUse)
		{
			// タイトルの外枠のα値を変更
			if (i == 0)
			{
				if (titlelogo->bAlpha)
				{
					titlelogo->fAlpha -= TITLELOGO_ALPHA_CHANGE;
					if (titlelogo->fAlpha <= TITLELOGO_ALPHA_MIN)
					{
						titlelogo->fAlpha = TITLELOGO_ALPHA_MIN;
						titlelogo->bAlpha = false;
					}
				}
				else
				{
					titlelogo->fAlpha += TITLELOGO_ALPHA_CHANGE;
					if (titlelogo->fAlpha >= TITLELOGO_ALPHA_MAX)
					{
						titlelogo->fAlpha = TITLELOGO_ALPHA_MAX;
						titlelogo->bAlpha = true;
					}
				}
				SetDiffuseTitlelogo(i, titlelogo->fAlpha);
#ifdef _DEBUG
				PrintDebugProc("【 TITLELOGO 】\n");
				PrintDebugProc("Alpha:%f\n", titlelogo->fAlpha);
#endif
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitlelogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[0];

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		if (titlelogo->bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, titlelogo->Texture);

			// タイトルの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLELOGO, titlelogo->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitlelogo(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// 頂点座標の設定
	SetVertexTitlelogo(no);

	// rhwの設定
	titlelogo->vertexWk[0].rhw =
	titlelogo->vertexWk[1].rhw =
	titlelogo->vertexWk[2].rhw =
	titlelogo->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	titlelogo->vertexWk[0].diffuse =
	titlelogo->vertexWk[1].diffuse =
	titlelogo->vertexWk[2].diffuse =
	titlelogo->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	titlelogo->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	titlelogo->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLELOGO, 0.0f );
	titlelogo->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO );
	titlelogo->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLELOGO, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO );

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVertexTitlelogo(int no)
{
	TITLELOGO *titlelogo = &titlelogoWk[no];

	titlelogo->vertexWk[0].vtx.x = titlelogo->pos.x - TEXTURE_TITLELOGO_SIZE_X;
	titlelogo->vertexWk[0].vtx.y = titlelogo->pos.y - TEXTURE_TITLELOGO_SIZE_Y;
	titlelogo->vertexWk[0].vtx.z = 0.0f;

	titlelogo->vertexWk[1].vtx.x = titlelogo->pos.x + TEXTURE_TITLELOGO_SIZE_X;
	titlelogo->vertexWk[1].vtx.y = titlelogo->pos.y - TEXTURE_TITLELOGO_SIZE_Y;
	titlelogo->vertexWk[1].vtx.z = 0.0f;

	titlelogo->vertexWk[2].vtx.x = titlelogo->pos.x - TEXTURE_TITLELOGO_SIZE_X;
	titlelogo->vertexWk[2].vtx.y = titlelogo->pos.y + TEXTURE_TITLELOGO_SIZE_Y;
	titlelogo->vertexWk[2].vtx.z = 0.0f;

	titlelogo->vertexWk[3].vtx.x = titlelogo->pos.x + TEXTURE_TITLELOGO_SIZE_X;
	titlelogo->vertexWk[3].vtx.y = titlelogo->pos.y + TEXTURE_TITLELOGO_SIZE_Y;
	titlelogo->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseTitlelogo(int no, float fAlpha)
{
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// 反射光の設定
	titlelogo->vertexWk[0].diffuse =
	titlelogo->vertexWk[1].diffuse =
	titlelogo->vertexWk[2].diffuse =
	titlelogo->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f,1.0f, fAlpha);
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTitlelogo( int no, int cntPattern )
{
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TITLELOGO;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TITLELOGO;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TITLELOGO;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO;
	titlelogo->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	titlelogo->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	titlelogo->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	titlelogo->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// タイトル取得関数
//=============================================================================
TITLELOGO *GetTitlelogo(int no)
{
	return(&titlelogoWk[no]);
}

