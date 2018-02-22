//=============================================================================
//
// �X�e�[�W�J�ڏ��� [stage.h]
// Author : GP11B243 24 �l���F��
//
// �^�C�g���E�Q�[���E���U���g�̑J�ڊǗ�
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define START_STAGE		(STAGE_TITLE)	// �J�n���̃X�e�[�W

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �񋓌^��`
*******************************************************************************/
enum
{	// �X�e�[�W
	STAGE_TITLE,
	STAGE_GAME,
	STAGE_RESULT,
	STAGE_MAX
};

enum
{
	STAGE_INIT_FAST,
	STAGE_INIT_LOOP
};

enum
{
	STAGE_WIN_NON,
	STAGE_WIN_1P,
	STAGE_WIN_2P
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT		InitStage(HINSTANCE hInstance, HWND hWnd);
HRESULT		InitStageEach(int nType);
void		UninitStage(void);
void		UpdateStage(void);
void		DrawStage(void);

void		SetStage(int nStage);
int			GetStage(void);

void		SetStageWinPlayer(int nPlayer);
int			GetStageWinPlayer(void);

#endif