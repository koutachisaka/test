//=============================================================================
//
// ���͏��� [sound.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "sound.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

// ���܂��Ȃ�
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void		SetSound(void);
long		GetVol(LPDIRECTSOUNDBUFFER8 pBuffer);
void		SetBgmVol(int no, int type);
void		UpdateSoundBgm(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECTSOUNDBUFFER8	g_pBgm[SOUND_BGM_MAX];
LPDIRECTSOUNDBUFFER8	g_pSe[SOUND_SE_MAX];
LPDIRECTSOUNDBUFFER8	g_pVoice[SOUND_VOICE_MAX];
int						g_nBgmFlag;
bool					g_bBgmBoss;
long g_nVol[SOUND_BGM_MAX];


// �T�E���h�t�@�C���̃p�X�isound.h�̒ʂ��i���o�[�Ə��Ԃ����킹�邱�Ɓj
const TCHAR* c_soundFilename[] = {
	// Atmospheres
	_T("data/SOUND/BGM/Atmospheres/ATM 004 D# 138BPM.wav"),
	_T("data/SOUND/BGM/Atmospheres/ATM 013 F# 138BPM.wav"),//
	// Basslines
	_T("data/SOUND/BGM/Basslines/MUFX1 LOOP 027 138BPM.wav"),
	_T("data/SOUND/BGM/Basslines/BA 028 G#.wav"),//
	_T("data/SOUND/BGM/Basslines/BA 043 B.wav"),//
	_T("data/SOUND/BGM/Basslines/BA 072 A#.wav"),//
	_T("data/SOUND/BGM/Basslines/BL 010 G 138BPM.wav"),//
	_T("data/SOUND/BGM/Basslines/BASS 010 F# 138BPM.wav"),//
	_T("data/SOUND/BGM/Basslines/BA 014 C.wav"),//
	// Drumloops
	_T("data/SOUND/BGM/Drumloops/DL 003 138BPM.wav"),
	_T("data/SOUND/BGM/Drumloops/MSD2 Drum Loop 138 BPM 036.wav"),
	// Sequences
	_T("data/SOUND/BGM/Sequences/SEQ 104 C.wav"),
	_T("data/SOUND/BGM/Sequences/SEQ 144 D#.wav"),
	_T("data/SOUND/BGM/Sequences/SEQ 131 B.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 013 A# 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 017 G 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SEQ 098 B.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 001 F# 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 016 A# 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 035 C 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 062 G 138BPM.wav"),//
	_T("data/SOUND/BGM/Sequences/SQ 065 G# 138BPM.wav"),//
	// TCE
	_T("data/SOUND/BGM/TCE/TCE3 06 Piano 138 BPM G# Wet.wav"),//
	// SE
	_T("data/SOUND/SE/close.wav"),
	_T("data/SOUND/SE/select.wav"),
	_T("data/SOUND/SE/decision.wav"),
	_T("data/SOUND/SE/pause.wav"),
	_T("data/SOUND/SE/nogravity.wav"),
	_T("data/SOUND/SE/gravity.wav"),
	_T("data/SOUND/SE/enemybullet00.wav"),
	_T("data/SOUND/SE/result.wav"),
	_T("data/SOUND/SE/playerbullet00.wav"),
	_T("data/SOUND/SE/alert.wav"),
	_T("data/SOUND/SE/jump.wav"),
	_T("data/SOUND/SE/hit.wav"),
	_T("data/SOUND/SE/enemyfalse.wav"),
	_T("data/SOUND/SE/enemybulletreversal.wav"),
	_T("data/SOUND/SE/se_maoudamashii_onepoint33.wav"),
	_T("data/SOUND/SE/TC/Crash.wav"),
	_T("data/SOUND/SE/TC/MRE Open Hat 009.wav"),
	_T("data/SOUND/SE/TC/MUFX1 EXPL 045.wav"),
	_T("data/SOUND/SE/TC/MRE Clap 009.wav"),
	// VOICE
	_T("data/SOUND/VOICE/fire_01.wav"),
	_T("data/SOUND/VOICE/thunder_01.wav"),
	_T("data/SOUND/VOICE/ice_01.wav"),
	_T("data/SOUND/VOICE/storm_01.wav"),
	_T("data/SOUND/VOICE/water_01.wav"),
	_T("data/SOUND/VOICE/daichiyo_01.wav"),
	_T("data/SOUND/VOICE/powerup_02.wav"),
	_T("data/SOUND/VOICE/warenichikarawo_01.wav"),
	_T("data/SOUND/VOICE/stageclear_01.wav"),
	_T("data/SOUND/VOICE/gameover_02.wav"),
	_T("data/SOUND/VOICE/ittaa_01.wav")
};

// �O���[�o���ϐ�
IDirectSound8 *g_pDirectSound = NULL;	// �T�E���h�C���^�[�t�F�[�X

// �T�E���h�̏�����
// hWnd:�E�B���h�E�n���h��
HRESULT InitSound( HWND hWnd )
{
	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;
	
	// �������x���ݒ� 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;
	g_nBgmFlag = SOUND_BGM_TITLE;
	g_bBgmBoss = false;

	for (int i = 0; i < SOUND_BGM_MAX; i++)
	{
		g_nVol[i] = VOLUME_MIN;
	}

	SetSound();

	return S_OK;
}

void UpdateSound(void)
{
	UpdateSoundBgm();
#ifdef _DEBUG
	PrintDebugProc("�y SOUND �z  BGM FLAG[%d]\n", g_nBgmFlag);
	for (int i = 0; i < SOUND_BGM_MAX; i++)
	{
		PrintDebugProc("BGM00%d[%l]\n", i, GetVol(g_pBgm[i]));
	}
#endif
}

// ��Еt��
void UninitSound()
{
	if( g_pDirectSound != NULL )
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}

// �T�E���h�̃��[�h
// no:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
LPDIRECTSOUNDBUFFER8 LoadSound( int no )
{
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;	// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;						// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;						// MMIO�n���h��
	MMIOINFO mmioInfo;						// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID pBlock;				// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;				// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');				// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);	// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);	// �����������Ƃɓǂݍ���
	
	if(size != formatChunk.cksize) 		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.�t�@�C����͇B �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');					// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR) 		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���
	char *pData = new char[dataChunk.cksize];					// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// �f�[�^��ǂݍ���
	
	if(size != dataChunk.cksize)  		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		delete[] pData;
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);			// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// ���o�����瑍���o�b�t�@���̂Ă�
	pBaseBuffer->Release();

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
	delete[] pData;					// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return pBuffer;
}

void UpdateSoundBgm(void)
{
	switch (g_nBgmFlag)
	{
	case SOUND_BGM_TITLE:
		SetBgmVol(BGM_ATM_00, true);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, true);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_NORMAL:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, true);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_FIRE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, true);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, true);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_05, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_THUNDER:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, true);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, true);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_07, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_ICE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, true);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_10, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, true);
		break;
	case SOUND_BGM_GAME_WIND:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, true);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, true);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_06, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_WATER:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, true);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, true);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_08, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_EARTH:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, true);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, true);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, true);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		if (g_bBgmBoss) SetBgmVol(BGM_SEQ_09, true);
		if (!g_bBgmBoss) SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_GAME_PAUSE:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, VOLUME_FADE_HALF);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	case SOUND_BGM_RESULT:
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, true);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, true);
		SetBgmVol(BGM_ATM_00, false);
		SetBgmVol(BGM_ATM_01, false);
		SetBgmVol(BGM_BASS_00, false);
		SetBgmVol(BGM_BASS_01, false);
		SetBgmVol(BGM_BASS_02, false);
		SetBgmVol(BGM_BASS_03, false);
		SetBgmVol(BGM_BASS_04, false);
		SetBgmVol(BGM_BASS_05, false);
		SetBgmVol(BGM_BASS_06, false);
		SetBgmVol(BGM_DRUM_00, false);
		SetBgmVol(BGM_DRUM_01, false);
		SetBgmVol(BGM_SEQ_00, false);
		SetBgmVol(BGM_SEQ_01, false);
		SetBgmVol(BGM_SEQ_02, false);
		SetBgmVol(BGM_SEQ_03, false);
		SetBgmVol(BGM_SEQ_04, false);
		SetBgmVol(BGM_SEQ_05, false);
		SetBgmVol(BGM_SEQ_06, false);
		SetBgmVol(BGM_SEQ_07, false);
		SetBgmVol(BGM_SEQ_08, false);
		SetBgmVol(BGM_SEQ_09, false);
		SetBgmVol(BGM_SEQ_10, false);
		SetBgmVol(BGM_TCE_00, false);
		break;
	}
}

void SetSoundBgm(int nBgm)
{
	g_nBgmFlag = nBgm;
}

void SetSoundBgmBoss(bool bBgm)
{
	g_bBgmBoss = bBgm;
}

// ����炷
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
// flag   :1(E_DS8_FLAG_LOOP)�Ȃ烋�[�v�Đ�
void PlaySound(  LPDIRECTSOUNDBUFFER8 pBuffer, int flag/*=0*/ )
{	// ��������炷�̂ŁA�ŏ�����炵�����ꍇ��SetCurrentPosition(0)�����邱��
	pBuffer->Play(0, 0, flag);
}

void PlayContinuitySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag)
{
	pBuffer->SetCurrentPosition(0);
	PlaySound(pBuffer, flag);
}

// �����~�߂�
void StopSound( LPDIRECTSOUNDBUFFER8 pBuffer )
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// ���Ă�����
	{
		pBuffer->Stop();	// �Ӗ��I�ɂ�PabUse�ɂȂ�B
	}
}

// �Đ������ǂ������ׂ�
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
bool IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer )
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}

HRESULT SetVol( LPDIRECTSOUNDBUFFER8 pBuffer, long nVol)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	pBuffer->SetVolume(nVol);
	return DS_OK;
}

long GetVol(LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;
	long lVol;
	pBuffer->GetStatus(&status);
	pBuffer->GetVolume(&lVol);
	return lVol;
}

//=============================================================================
// �T�E���h�ݒ�֐�
//=============================================================================
void SetSound(void)
{
	// BGM������Ȃ��悤�Ƀ��[�h�ƕ�����
	for (int i = BGM_ATM_00; i < SOUND_BGM_MAX; i++)
	{
		// �eBGM���[�h
		g_pBgm[i] = LoadSound(i);
	}
	for (int i = BGM_ATM_00; i < SOUND_BGM_MAX; i++)
	{
		// �eBGM�{�����[���Z�b�g
		PlaySound(g_pBgm[i], E_DS8_FLAG_LOOP);
		SetVol(g_pBgm[i], VOLUME_MIN);
	}

	for (int i = 0; i < SOUND_SE_MAX; i++)
	{	
		// �eSE���[�h
		g_pSe[i] = LoadSound(i + SE_00);
		//// �eSE�{�����[���Z�b�g
		//if (i != SE_00 || i != SE_11 || i != SE_13)
		//{
		//	SetVol(g_pSe[i], VOLUME_SE);
		//}
	}

	//SetVol(g_pSe[11], SE_11_VOLUME_SE);

	for (int i = 0; i < SOUND_VOICE_MAX; i++)
	{
		// �eSE���[�h
		g_pVoice[i] = LoadSound(i + VOICE_00);
	}
}

//=============================================================================
// SE�Đ��֐�
//=============================================================================
void SetSe(int nVoice, int nFlag, bool bContinuity)
{
	if (bContinuity)
	{
		PlayContinuitySound(g_pSe[nVoice], nFlag);
	}
	else
	{
		PlaySound(g_pSe[nVoice], nFlag);
	}
}

//=============================================================================
// Voice�Đ��֐�
//=============================================================================
void SetVoice(int nVoice, int nFlag, bool bContinuity)
{
	if (bContinuity)
	{
		PlayContinuitySound(g_pVoice[nVoice], nFlag);
	}
	else
	{
		PlaySound(g_pVoice[nVoice], nFlag);
	}
}

//=============================================================================
// BGM�{�����[���R���g���[��
//=============================================================================
void SetBgmVol(int no, int type)
{
	// type 0=�t�F�[�h�A�E�g 1=�t�F�[�h�C��
	if (type == VOLUME_FADE_OUT)
	{
		if (g_nVol[no] > VOLUME_MIN)
		{
			g_nVol[no] -= VOLUME_CONTROL_DOWN;
			if (g_nVol[no] < VOLUME_MIN)
			{
				g_nVol[no] = VOLUME_MIN;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
	else if (type == VOLUME_FADE_IN)
	{
		if (g_nVol[no] < VOLUME_MAX)
		{
			g_nVol[no] += VOLUME_CONTROL_UP;
			if (g_nVol[no] > VOLUME_MAX)
			{
				g_nVol[no] = VOLUME_MAX;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
	else if (type == VOLUME_FADE_HALF)
	{
		if (g_nVol[no] > VOLUME_HALF)
		{
			g_nVol[no] -= VOLUME_CONTROL_DOWN;
			if (g_nVol[no] < VOLUME_HALF)
			{
				g_nVol[no] = VOLUME_HALF;
			}
			SetVol(g_pBgm[no], g_nVol[no]);
		}
	}
}