//=============================================================================
//
// �^�C�g������ [titlelogo.h]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_TITLELOGO		(2)	// �|���S����

#define TEXTURE_GAME_TITLELOGO_00	_T("data/TEXTURE/title_back.png")	// �^�C�g���p�摜
#define TEXTURE_GAME_TITLELOGO_01	_T("data/TEXTURE/title.png")	// �^�C�g���p�摜

#define TEXTURE_TITLELOGO_SIZE_X	(1050 * SCREEN_SCALE/2) // �e�N�X�`���T�C�Y
#define TEXTURE_TITLELOGO_SIZE_Y	(250 * SCREEN_SCALE/2) // ����

#define TITLELOGO_POS_X				(SCREEN_CENTER_X)				// �������WX
#define TITLELOGO_POS_Y				(150 * SCREEN_SCALE)			// �������WY

#define TITLELOGO_ALPHA_MIN			(0.15f)
#define TITLELOGO_ALPHA_MAX			(0.98f)
#define TITLELOGO_ALPHA_CHANGE		(0.01f)

#define TEXTURE_PATTERN_DIVIDE_X_TITLELOGO	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_TITLELOGO			(TEXTURE_PATTERN_DIVIDE_X_TITLELOGO*TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_TITLELOGO				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define TITLELOGO_MAX					(2)	// �^�C�g���̍ő吔

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	LPDIRECT3DTEXTURE9		Texture;				// �e�N�X�`�����
	VERTEX_2D				vertexWk[NUM_VERTEX];	// ���_���i�[���[�N
	D3DXVECTOR3				pos;					// �^�C�g���̈ړ���
	D3DXVECTOR3				rot;					// �^�C�g���̉�]��
	D3DXVECTOR2				size;					// �^�C�g���̃e�N�X�`���T�C�Y(x,y)
	float					fAlpha;					// ���l
	bool					bAlpha;					// ���l�ύX�t���O
	bool					bUse;					// true:�g�p  false:���g�p
}TITLELOGO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitlelogo(int nType);
void UninitTitlelogo(void);
void UpdateTitlelogo(void);
void DrawTitlelogo(void);
TITLELOGO *GetTitlelogo(int no);


#endif
