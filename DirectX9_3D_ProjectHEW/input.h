//=============================================================================
//
// ���͏��� [input.h]
// Author :  GP11B243 24 �l���F��
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�

#define	NUM_KEY_MAX			(256)

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.rgdwPOV == 0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.rgdwPOV == 9000)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.rgdwPOV == 18000)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.rgdwPOV == 27000)
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_MINUS	0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)
#define LSTICK_UP		0x00004000l	// ���X�e�B�b�N��(.IY<0)
#define LSTICK_DOWN		0x00008000l	// ���X�e�B�b�N��(.IY>0)
#define LSTICK_LEFT		0x00010000l	// ���X�e�B�b�N��(.IX<0)
#define LSTICK_RIGHT	0x00020000l	// ���X�e�B�b�N�E(.IX>0)
#define RSTICK_UP		0x00040000l	// �E�X�e�B�b�N��(.lRz<0x8000)
#define RSTICK_DOWN		0x00080000l	// �E�X�e�B�b�N��(.lRz>0x8000)
#define RSTICK_LEFT		0x00100000l	// �E�X�e�B�b�N��(.lZ<0x8000)
#define RSTICK_RIGHT	0x00200000l	// �E�X�e�B�b�N�E(.lZ>0x8000)
#define LSTICK_BUTTON	0x00400000l	// ���X�e�B�b�N�{�^��
#define RSTICK_BUTTON	0x00800000l	// �E�X�e�B�b�N�{�^��

#define BUTTON_CAP		0x00000100l	// �L���v�`���[�{�^��[13]
#define BUTTON_HOME		0x00000200l	// Home�{�^��[28]

// Joy-con(R)�g���p
#define R_BUTTON_Y		0x01000000l	// Y�{�^��
#define R_BUTTON_X		0x02000000l	// X�{�^��
#define R_BUTTON_B		0x04000000l	// B�{�^��
#define R_BUTTON_A		0x08000000l	// A�{�^��
#define R_BUTTON_R		0x10000000l	// R�{�^��
#define R_BUTTON_ZR		0x20000000l	// ZR�{�^��
#define R_BUTTON_PLUS	0x40000000l	// PLUS�{�^��
#define R_BUTTON_LEFT	0x80000000l	// LEFT�{�^��


//#define RSTICK_MARGIN	50			// �E�X�e�B�b�N�p�}�[�W��
#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

#define PAD_SLIDER_MARGIN	(15)
#define PAD_SLIDER_SPEED	(0.0001f)
#define PAD_SLIDER_DEFAULT	(5)
#define PAD_SLIDER_MIN		(1)
#define PAD_SLIDER_MAX		(9)

enum
{
	PAD_STICK_L_X,
	PAD_STICK_L_Y,
	PAD_STICK_R_X,
	PAD_STICK_R_Y
};

enum
{
	PAD_SLIDER_V,
	PAD_SLIDER_H
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mobUse
BOOL IsMobUseLeftPressed(void);			// ���N���b�N�������
BOOL IsMobUseLeftTriggered(void);		// ���N���b�N�����u��
BOOL IsMobUseLeftReleased(void);		// ���N���b�N�𗣂����u��
BOOL IsMobUseRightPressed(void);		// �E�N���b�N�������
BOOL IsMobUseRightTriggered(void);		// �E�N���b�N�����u��
BOOL IsMobUseRightReleased(void);		// �E�N���b�N�𗣂����u��
BOOL IsMobUseCenterPressed(void);		// ���N���b�N�������
BOOL IsMobUseCenterTriggered(void);		// ���N���b�N�����u��
BOOL IsMobUseCenterReleased(void);		// ���N���b�N�𗣂����u��
long GetMobUseX(void);					// �}�E�X��X�����ɓ��������Βl
long GetMobUseY(void);					// �}�E�X��Y�����ɓ��������Βl
long GetMobUseZ(void);					// �}�E�X�z�C�[�������������Βl

									 //---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
BOOL IsButtonReleased(int padNo, DWORD button);

float GetButtonlZ(int padNo);
float GetButtonlRz(int padNo);
float GetStick(int padNo, int nStick);
float GetRglSlider(int nSlider);

#endif