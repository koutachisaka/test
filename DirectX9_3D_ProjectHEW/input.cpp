//=============================================================================
//
// ���͏��� [input.cpp]
// Author :  GP11B243 24 �l���F��
//
//=============================================================================
#include "input.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMobUse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMobUse();						// �}�E�X�̏I������
HRESULT UpdateMobUse();					// �}�E�X�̍X�V����

HRESULT InitializePad(HWND hWnd);			// �p�b�h������
										//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- mobUse
static LPDIRECTINPUTDEVICE8 pMobUse = NULL; // mobUse

static DIMOUSESTATE2   mobUseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2   mobUseTrigger;	// �����ꂽ�u�Ԃ���ON
static DIMOUSESTATE2   mobUseRelease;	// �����ꂽ�u�Ԃ���OFF

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;			// ���o�����p�b�h�̐�

float			padlX;
float			padlY;
LONG			padlRx;
float			padlRy;
float			padlZ;
float			padlRz;
float			g_rglSlider[2];
DIDEVCAPS		g_diDevCaps;

int				g_nJoyconSlider;

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	g_nJoyconSlider = PAD_SLIDER_DEFAULT;

	if (!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitializeMobUse(hInst, hWnd);

	// �p�b�h�̏�����
	InitializePad(hWnd);

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMobUse();

	// �p�b�h�̏I������
	UninitPad();

	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// Joy-con�̓K�p�����x�ύX
	if (IsButtonTriggered(0, R_BUTTON_PLUS))
	{
		g_nJoyconSlider++;
		if(g_nJoyconSlider > PAD_SLIDER_MAX)
		{
			g_nJoyconSlider = PAD_SLIDER_MAX;
		}
	}
	else if (IsButtonTriggered(0, BUTTON_MINUS))
	{
		g_nJoyconSlider--;
		if (g_nJoyconSlider < PAD_SLIDER_MIN)
		{
			g_nJoyconSlider = PAD_SLIDER_MIN;
		}
	}
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMobUse();

	// �p�b�h�̍X�V
	UpdatePad();
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���m�F�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitializeMobUse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	RECT rectMove;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMobUse, NULL);
	if (FAILED(result) || pMobUse == NULL)
	{
		MessageBox(hWindow, "No mobUse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMobUse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mobUse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMobUse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "MobUse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}


	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMobUse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "MobUse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
	pMobUse->Acquire();
	ShowCursor(false);

	SetRect(&rectMove, 0, 0, 1280 * SCREEN_SCALE, 720 * SCREEN_SCALE);
	return result;
}
//---------------------------------------------------------
void UninitMobUse()
{
	if (pMobUse)
	{
		ShowCursor(true);
		pMobUse->Unacquire();
		pMobUse->Release();
		pMobUse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMobUse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMobUseState = mobUseState;
	// �f�[�^�擾
	result = pMobUse->GetDeviceState(sizeof(mobUseState), &mobUseState);
	if (SUCCEEDED(result))
	{
		mobUseTrigger.lX = mobUseState.lX;
		mobUseTrigger.lY = mobUseState.lY;
		mobUseTrigger.lZ = mobUseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i<8; i++)
		{
			mobUseTrigger.rgbButtons[i] = ((lastMobUseState.rgbButtons[i] ^
				mobUseState.rgbButtons[i]) & mobUseState.rgbButtons[i]);

			mobUseRelease.rgbButtons[i] = ((lastMobUseState.rgbButtons[i] ^
				mobUseState.rgbButtons[i]) & ~mobUseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMobUse->Acquire();
	}

	// �}�E�X�|�C���^�̍X�V���W���擾
	POINT lpMouse;
	GetCursorPos(&lpMouse);


	// �f�o�b�O�p
#ifdef _DEBUG
	PrintDebugProc("Mouse.X[%l]  Y[%l]\n", lpMouse.x, lpMouse.y);
#endif

	return result;
}


//----------------------------------------------
BOOL IsMobUseLeftPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL IsMobUseLeftTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMobUseLeftReleased(void)
{
	return (BOOL)(mobUseRelease.rgbButtons[0] & 0x80);
}
BOOL IsMobUseRightPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[1] & 0x80);
}
BOOL IsMobUseRightTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMobUseRightReleased(void)
{
	return (BOOL)(mobUseRelease.rgbButtons[1] & 0x80);
}
BOOL IsMobUseCenterPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[2] & 0x80);
}
BOOL IsMobUseCenterTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[2] & 0x80);
}
BOOL IsMobUseCenterReleased(void)
{
	return (BOOL)(mobUseRelease.rgbButtons[2] & 0x80);
}
//------------------
long GetMobUseX(void)
{
	return mobUseState.lX;
}
long GetMobUseY(void)
{
	return mobUseState.lY;
}
long GetMobUseZ(void)
{
	return mobUseState.lZ;
}

//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;
	hr = pGamePad[0]->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//---------------------------------------- ������
HRESULT InitializePad(HWND hWnd)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)SearchGamePadCallback,
		NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i<padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(result))
			return false;	// �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

						//// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
						//result = pGamePad[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						//if ( FAILED(result) )
						//	return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// RX���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RY���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RZ���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);


		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		// X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// RX���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RY���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RZ���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
		pGamePad[i]->GetCapabilities(&g_diDevCaps);
		pGamePad[i]->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_ABSAXIS);
		pGamePad[i]->Poll();
		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- �I������
void UninitPad(void)
{
	for (int i = 0; i<GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

#ifdef _DEBUG
	PrintDebugProc("�yPAD�z\n");
#endif

	for (i = 0; i<padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE2), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���

		//* �����L�[��
		if (dijs.rgdwPOV[0] == 0)		padState[i] |= BUTTON_UP;
		//* �����L�[��
		if (dijs.rgdwPOV[0] == 18000)	padState[i] |= BUTTON_DOWN;
		//* �����L�[��
		if (dijs.rgdwPOV[0] == 27000)	padState[i] |= BUTTON_LEFT;
		//* �����L�[�E
		if (dijs.rgdwPOV[0] == 9000)	padState[i] |= BUTTON_RIGHT;

		//* �����L�[�E��
		if (dijs.rgdwPOV[0] == 4500)	padState[i] |= BUTTON_UP, padState[i] |= BUTTON_RIGHT;
		//* �����L�[�E��
		if (dijs.rgdwPOV[0] == 13500)	padState[i] |= BUTTON_DOWN, padState[i] |= BUTTON_RIGHT;
		//* �����L�[����
		if (dijs.rgdwPOV[0] == 22500)	padState[i] |= BUTTON_DOWN, padState[i] |= BUTTON_LEFT;
		//* �����L�[����
		if (dijs.rgdwPOV[0] == 31500)	padState[i] |= BUTTON_UP, padState[i] |= BUTTON_LEFT;

		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		////* �x�{�^��
		//if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		////* �y�{�^��
		//if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;

		//* �L���v�`���[�{�^��[13]
		if (dijs.rgbButtons[13] & 0x80)	padState[i] |= BUTTON_CAP;
		//* Home�{�^��[28]
		if (dijs.rgbButtons[28] & 0x80)	padState[i] |= BUTTON_HOME;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_MINUS;

		//*
		if (dijs.rgbButtons[16] & 0x80)	padState[i] |= R_BUTTON_Y;
		//*
		if (dijs.rgbButtons[17] & 0x80)	padState[i] |= R_BUTTON_X;
		//*
		if (dijs.rgbButtons[18] & 0x80)	padState[i] |= R_BUTTON_B;
		//*
		if (dijs.rgbButtons[19] & 0x80)	padState[i] |= R_BUTTON_A;
		//*
		if (dijs.rgbButtons[22] & 0x80)	padState[i] |= R_BUTTON_R;
		//*
		if (dijs.rgbButtons[23] & 0x80)	padState[i] |= R_BUTTON_ZR;
		//*
		if (dijs.rgbButtons[25] & 0x80)	padState[i] |= R_BUTTON_PLUS;
		//*
		if (dijs.rgbButtons[21] & 0x80)	padState[i] |= R_BUTTON_LEFT;

		//* y-axis (forward)
		if (dijs.lY < 0)				padState[i] |= LSTICK_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)				padState[i] |= LSTICK_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)				padState[i] |= LSTICK_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)				padState[i] |= LSTICK_RIGHT;


		//* �E�X�e�B�b�N��
		if (dijs.lRy < 0)				padState[i] |= RSTICK_UP;
		//* �E�X�e�B�b�N��
		if (dijs.lRy > 0)				padState[i] |= RSTICK_DOWN;
		//* �E�X�e�B�b�N��
		if (dijs.lRx < 0 )				padState[i] |= RSTICK_LEFT;
		//* �E�X�e�B�b�N�E
		if (dijs.lRx > 0 )				padState[i] |= RSTICK_RIGHT;


		////* �E�X�e�B�b�N��
		//if (dijs.lRz < 0x8000 - RSTICK_MARGIN)	padState[i] |= RSTICK_UP;
		////* �E�X�e�B�b�N��
		//if (dijs.lRz > 0x8000 + RSTICK_MARGIN)	padState[i] |= RSTICK_DOWN;
		////* �E�X�e�B�b�N��
		//if (dijs.lZ < 0x8000 - RSTICK_MARGIN)	padState[i] |= RSTICK_LEFT;
		////* �E�X�e�B�b�N�E
		//if (dijs.lZ > 0x8000 + RSTICK_MARGIN)	padState[i] |= RSTICK_RIGHT;

		//* ���X�e�B�b�N�{�^��
		if (dijs.rgbButtons[10] & 0x80)	padState[i] |= LSTICK_BUTTON;
		//* �E�X�e�B�b�N�{�^��
		if (dijs.rgbButtons[11] & 0x80)	padState[i] |= RSTICK_BUTTON;

		
		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& padState[i]);					// ��������ON�̂��
		// Release�ݒ�
		padRelease[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& ~padState[i]);					// ��������OFF�̂��

		if (i == 0)
		{
			// �E�X�e�B�b�N�̌X���ʂ�ۊ�
			padlRx = dijs.lRx;
			padlRy = dijs.lRy;

			padlZ = dijs.lZ;
			padlRz = dijs.lRz;

			g_rglSlider[0] = (float)dijs.rglSlider[0];
			g_rglSlider[1] = (float)dijs.rglSlider[1];
		}

#ifdef _DEBUG
			PrintDebugProc("LStick[X:%l  Y:%l  Z:%l]  RStick[X:%l  Y:%l  Z:%l]\n",
				dijs.lX, dijs.lY, dijs.lZ, dijs.lRx, dijs.lRy, dijs.lRz);
			PrintDebugProc("POV[UP:%d  RIGHT:%d  DOWN:%d  LEFT:%d]\n",
				dijs.rgdwPOV[0], dijs.rgdwPOV[1], dijs.rgdwPOV[2], dijs.rgdwPOV[3]);
			PrintDebugProc("lV[X:%l  Y:%l  Z:%l]  lVR[X:%l  Y:%l  Z:%l]\n",
				dijs.lVX, dijs.lVY, dijs.lVZ, dijs.lVRx, dijs.lVRy, dijs.lVRz);
			PrintDebugProc("lA[X:%l  Y:%l  Z:%l]  lAR[X:%l  Y:%l  Z:%l]\n",
				dijs.lAX, dijs.lAY, dijs.lAZ, dijs.lARx, dijs.lARy, dijs.lARz);
			PrintDebugProc("lF[X:%l  Y:%l  Z:%l]  lFR[X:%l  Y:%l  Z:%l]\n",
				dijs.lFX, dijs.lFY, dijs.lFZ, dijs.lFRx, dijs.lFRy, dijs.lFRz);
			PrintDebugProc("rglSlider[0:%l  1:%l]  rglVSlider[0:%l  1:%l]\n",
				dijs.rglSlider[0], dijs.rglSlider[1], dijs.rglVSlider[0], dijs.rglVSlider[1]);
			PrintDebugProc("rglASlider[0:%l  1:%l]  rglFSlider[0:%l  1:%l]\n",
				dijs.rglASlider[0], dijs.rglASlider[1], dijs.rglFSlider[0], dijs.rglFSlider[1]);
			PrintDebugProc("rgbButtons\n");
			for (int i = 0; i < 128; i++)
			{
				PrintDebugProc("%d", dijs.rgbButtons[i]);
				if (i % 32 == 0 && i != 0)
				{
					PrintDebugProc("\n");
				}
			}
			PrintDebugProc("[%f] [%f]\n", (float)padlZ, (float)padlRz);
			PrintDebugProc("\n");
#endif
	}
	//GetButtonlZ(0);
}
//----------------------------------------------- ����
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

BOOL IsButtonReleased(int padNo, DWORD button)
{
	return (button & padRelease[padNo]);
}

float GetButtonlZ(int padNo)
{
	if (padlZ < 0.0f) padlZ *= -1.0f;
	padlZ = padlZ / RANGE_MAX;
#ifdef _DEBUG
	PrintDebugProc("padlZ[%f]\n", padlZ);
#endif
	return (padlZ);
}

float GetButtonlRz(int padNo)
{
	if (padlRz < 0.0f) padlRz *= -1.0f;
	//padlRz = padlRz / RANGE_MAX;
#ifdef _DEBUG
	PrintDebugProc("padlRz[%f]\n", padlRz);
#endif
	return (padlRz);
}

float GetStick(int padNo, int nStick)
{
	float fStick = 0.0f;
	switch (nStick)
	{
	case PAD_STICK_R_X:
		fStick = (float)padlRx / RANGE_MAX;
		break;
	}
	return (fStick);
}

float GetRglSlider(int nSlider)
{
	float fSlider = 0.0f;
	if (g_rglSlider[nSlider] > PAD_SLIDER_MARGIN || g_rglSlider[nSlider] < -PAD_SLIDER_MARGIN)
	fSlider = g_rglSlider[nSlider] * PAD_SLIDER_SPEED * g_nJoyconSlider;
	return (fSlider);
}