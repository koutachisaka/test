//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11B243 24 人見友基
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA					cameraWk;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	CAMERA *camera = &cameraWk;

	camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	camera->posCameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	camera->fVAngle = CAMERA_V_ANGLE * 1.3f;
	camera->fHAngle = CAMERA_H_ANGLE;
	camera->fHAngleMargin = 0.0f;
	camera->fLength = CAMERA_LENGTH;
	camera->fLengthTemp = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{
	CAMERA	*camera = &cameraWk;

	// クォータニオン用変数
	// 回転半径を設定
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// 現在の角度に垂直な回転軸ベクトルを設定
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));

	// 斜め回転時に速度減算
	if ( ((GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN)) && (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT)))
		|| ((IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_DOWN)) && (IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, BUTTON_RIGHT)))
		|| ((IsButtonPressed(0, LSTICK_UP) || IsButtonPressed(0, LSTICK_DOWN)) && (IsButtonPressed(0, LSTICK_LEFT) || IsButtonPressed(0, LSTICK_RIGHT)))
		)
	{
		camera->fRotSpeed = CAMERA_ROT_SLANT;
	}
	else
	{
		camera->fRotSpeed = CAMERA_ROT_SPEED;
	}

	// カメラ左右回転（キーボード）
	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_RIGHT))
	{
		// 同時押しは処理なし
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		camera->fHAngleDiff += camera->fRotSpeed;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{
		camera->fHAngleDiff -= camera->fRotSpeed;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}

	// カメラ左右回転（マウス）
	if ((float)GetMobUseX() && !IsButtonPressed(0, R_BUTTON_X))
	{
		camera->fHAngleDiff -= ((float)GetMobUseX()) * CAMERA_ROT_MOUSE_X;
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}

	if (IsButtonPressed(0, RSTICK_LEFT) || IsButtonPressed(0, RSTICK_RIGHT))
	{
		camera->fHAngleDiff -= camera->fRotSpeed  * GetStick(0, PAD_STICK_R_X);
		//camera->fHAngleDiff = PiCalculate360(camera->fHAngle);
	}

	// カメラ上下反転
	if (GetKeyboardTrigger(DIK_F2))
	{
		camera->bCameraReverse = camera->bCameraReverse ? false : true;
	}
	// ロットスピードを反転
	if (camera->bCameraReverse)
	{
		camera->fRotSpeed *= -1.0f;
	}

	// カメラ上下回転（キーボード）
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
	{
		// 同時押しは処理なし
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		camera->fVAngleDiff -= camera->fRotSpeed;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		camera->fVAngleDiff += camera->fRotSpeed;
	}
	// カメラ上下回転（マウス）
	if ((float)GetMobUseY() && !IsButtonPressed(0, R_BUTTON_X))
	{
		camera->fVAngleDiff += ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y;
	}

	// 視点距離まで移動
	if (camera->fLength > CAMERA_LENGTH)
	{
		camera->fLength -= CAMERA_LENGTH_AUTO_SPEED;
		// 視点距離を越えた場合は指定距離に固定
		if (camera->fLength < CAMERA_LENGTH)
		{
			camera->fLength = CAMERA_LENGTH;
			camera->fLength = camera->fLength;
		}
	}

	//// カメラ注視点をセット
	//camera->posCameraAt =

	// クォータニオン処理
	QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
		&camera->posCameraEye);
	// 回転軸に設置
	camera->posCameraEye += camera->posCameraAt;
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk;
	/******************** ビューイング変換 ********************/
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxView);

	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// 視点
		&camera->posCameraAt,	// 注視点
		&camera->vecCameraUp);	// 上方向

								// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** プロジェクション変換 ********************/
	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxProjection);

	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値（近いと描画しない）
		VIEW_FAR_Z);		// ビュー平面のFarZ値（遠いと描画しない）

							// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->rotCamera;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxView;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxProjection;
}

//=============================================================================
// カメラ取得関数
//=============================================================================
CAMERA *GetCamera(void)
{
	return(&cameraWk);
}