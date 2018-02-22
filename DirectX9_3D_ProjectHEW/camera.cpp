//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA					cameraWk;

//=============================================================================
// ����������
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
// �I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCamera(void)
{
	CAMERA	*camera = &cameraWk;

	// �N�H�[�^�j�I���p�ϐ�
	// ��]���a��ݒ�
	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
	// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));

	// �΂߉�]���ɑ��x���Z
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

	// �J�������E��]�i�L�[�{�[�h�j
	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_RIGHT))
	{
		// ���������͏����Ȃ�
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

	// �J�������E��]�i�}�E�X�j
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

	// �J�����㉺���]
	if (GetKeyboardTrigger(DIK_F2))
	{
		camera->bCameraReverse = camera->bCameraReverse ? false : true;
	}
	// ���b�g�X�s�[�h�𔽓]
	if (camera->bCameraReverse)
	{
		camera->fRotSpeed *= -1.0f;
	}

	// �J�����㉺��]�i�L�[�{�[�h�j
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
	{
		// ���������͏����Ȃ�
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		camera->fVAngleDiff -= camera->fRotSpeed;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		camera->fVAngleDiff += camera->fRotSpeed;
	}
	// �J�����㉺��]�i�}�E�X�j
	if ((float)GetMobUseY() && !IsButtonPressed(0, R_BUTTON_X))
	{
		camera->fVAngleDiff += ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y;
	}

	// ���_�����܂ňړ�
	if (camera->fLength > CAMERA_LENGTH)
	{
		camera->fLength -= CAMERA_LENGTH_AUTO_SPEED;
		// ���_�������z�����ꍇ�͎w�苗���ɌŒ�
		if (camera->fLength < CAMERA_LENGTH)
		{
			camera->fLength = CAMERA_LENGTH;
			camera->fLength = camera->fLength;
		}
	}

	//// �J���������_���Z�b�g
	//camera->posCameraAt =

	// �N�H�[�^�j�I������
	QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
		&camera->posCameraEye);
	// ��]���ɐݒu
	camera->posCameraEye += camera->posCameraAt;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk;
	/******************** �r���[�C���O�ϊ� ********************/
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxView);

	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// ���_
		&camera->posCameraAt,	// �����_
		&camera->vecCameraUp);	// �����

								// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** �v���W�F�N�V�����ϊ� ********************/
	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxProjection);

	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l�i�߂��ƕ`�悵�Ȃ��j
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l�i�����ƕ`�悵�Ȃ��j

							// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->rotCamera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxProjection;
}

//=============================================================================
// �J�����擾�֐�
//=============================================================================
CAMERA *GetCamera(void)
{
	return(&cameraWk);
}