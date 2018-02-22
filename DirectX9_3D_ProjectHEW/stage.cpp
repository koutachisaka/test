//=============================================================================
//
// �X�e�[�W�J�ڏ��� [stage.cpp]
// Author : GP11B243 24 �l���F��
//
// �^�C�g���E�Q�[���E���U���g�̑J�ڊǗ�
//
//=============================================================================
#include "main.h"
#include "stage.h"

/* Stage */
#include "title.h"
#include "game.h"
#include "result.h"

/* Camera */
#include "camera.h"

/* System */
#include "input.h"
#include "sound.h"
#include "light.h"
#include "fade.h"

/* Debug */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int					g_nStage;				// �����X�e�[�W
int					g_nPlayerWin;			// �����v���C���[

#ifdef _DEBUG
bool				g_bDispDebug = true;	// �f�o�b�O�\��ON/OFF
#endif

//=============================================================================
// ����������
//=============================================================================
HRESULT InitStage(HINSTANCE hInstance, HWND hWnd)
{
	// �O���[�o���ϐ��̏�����
	g_nStage = START_STAGE;			// �����X�e�[�W��ݒ�
	g_nPlayerWin = STAGE_WIN_NON;	// �����v���C���[��������

	InitInput(hInstance, hWnd);		// ����
	InitCamera();					// �J����
	InitSound(hWnd);				// �T�E���h
	InitLight();					// ���C�g
	InitFade();						// �t�F�[�h
	InitStageEach(STAGE_INIT_FAST);	// �e�X�e�[�W�̏�����

#ifdef _DEBUG
	InitDebugProc();				// �f�o�b�O�\�������̏�����
#endif
	return S_OK;
}

//=============================================================================
// �ď���������
//=============================================================================
HRESULT InitStageEach(int nType)
{
	InitTitle(nType);		// �^�C�g��
	InitGame(nType);		// �Q�[��
	InitResult(nType);		// ���U���g
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStage(void)
{
	UninitCamera();					// �J����
	UninitInput();					// ����
	UninitSound();					// �T�E���h
	UninitFade();					// �t�F�[�h
	UninitTitle();					// �^�C�g��
#ifdef _DEBUG
	UninitDebugProc();				// �f�o�b�O�\�������̏I������
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStage(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{	// �f�o�b�O�\��ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}
	UpdateDebugProc();				// �f�o�b�O
	PrintDebugProc("�y STAGE:%d �z\n", g_nStage);
#endif

	UpdateInput();					// ����

	// �X�e�[�W�ɉ������X�V����
	switch (g_nStage)
	{
	case STAGE_TITLE:
		UpdateTitle();				// �^�C�g��
		break;
	case STAGE_GAME:
		UpdateGame();				// �Q�[��
		break;
	case STAGE_RESULT:
		UpdateResult();				// ���U���g
		break;
	}
	UpdateCamera();					// �J����
	UpdateFade();					// �t�F�[�h


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStage(void)
{
	SetCamera();					// �J�����̐ݒ菈��

	// �X�e�[�W�ɉ������`�揈��
	switch (g_nStage)
	{
	case STAGE_TITLE:
		DrawTitle();				// �^�C�g��
		break;
	case STAGE_GAME:
		DrawGame();					// �Q�[��
		break;
	case STAGE_RESULT:
		DrawResult();				// ���U���g
		break;
	}

	DrawFade();						// �t�F�[�h

#ifdef _DEBUG
	if (g_bDispDebug)
	{	// �f�o�b�O�\������ꍇ
		DrawDebugProc();			// �f�o�b�O
	}
#endif
}

//=============================================================================
// �X�e�[�W�ݒ�֐�
//=============================================================================
void SetStage(int nStage)
{
	g_nStage = nStage;
}

//=============================================================================
// �X�e�[�W�擾�֐�
//=============================================================================
int GetStage(void)
{
	return (g_nStage);
}

//=============================================================================
// �����v���C���[�ݒ�֐�
//=============================================================================
void SetStageWinPlayer(int nPlayer)
{
	g_nPlayerWin = nPlayer;
}

//=============================================================================
// �����v���C���[�擾�֐�
//=============================================================================
int GetStageWinPlayer(void)
{
	return (g_nPlayerWin);
}