//=============================================================================
//
// �^�C�g������ [titlelogo.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "titlelogo.h"
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTitlelogo(int no);
void SetVertexTitlelogo(int no);
void SetDiffuseTitlelogo(int no, float fAlpha);
void SetTextureTitlelogo(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
TITLELOGO					titlelogoWk[TITLELOGO_MAX];

// �e�N�X�`���ւ̃|���S��
LPDIRECT3DTEXTURE9		pD3DTextureTitlelogo[TITLELOGO_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitlelogo(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[0];

	if (nType == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TITLELOGO_00,				// �t�@�C���̖��O
			&pD3DTextureTitlelogo[0]);			// �ǂݍ��ރ������̃|�C���^

		D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�̃|�C���^
			TEXTURE_GAME_TITLELOGO_01,				// �t�@�C���̖��O
			&pD3DTextureTitlelogo[1]);			// �ǂݍ��ރ������̃|�C���^
	}

	// �^�C�g���̏�����
	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		titlelogo->bUse = true;
		titlelogo->bAlpha = true;
		titlelogo->pos = D3DXVECTOR3(TITLELOGO_POS_X, TITLELOGO_POS_Y, 0.0f);
		titlelogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		titlelogo->fAlpha = TITLELOGO_ALPHA_MAX;

		titlelogo->Texture = pD3DTextureTitlelogo[i];			// �e�N�X�`���ւ̃G�l�~�[
		titlelogo->size = D3DXVECTOR2(TEXTURE_TITLELOGO_SIZE_X, TEXTURE_TITLELOGO_SIZE_Y);

		// ���_���̍쐬
		MakeVertexTitlelogo(i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitlelogo(void)
{
	TITLELOGO *titlelogo = &titlelogoWk[0];

	for (int i = 0; i < TITLELOGO_MAX; i++)
	{
		// ���������
		if (&pD3DTextureTitlelogo[i] != NULL)
		{
			pD3DTextureTitlelogo[i]->Release();
			pD3DTextureTitlelogo[i] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitlelogo(void)
{
	TITLELOGO *titlelogo = &titlelogoWk[0];

	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		if (titlelogo->bUse)
		{
			// �^�C�g���̊O�g�̃��l��ύX
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
				PrintDebugProc("�y TITLELOGO �z\n");
				PrintDebugProc("Alpha:%f\n", titlelogo->fAlpha);
#endif
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitlelogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[0];

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TITLELOGO_MAX; i++, titlelogo++)
	{
		if (titlelogo->bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, titlelogo->Texture);

			// �^�C�g���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_TITLELOGO, titlelogo->vertexWk, sizeof(VERTEX_2D));
		}
	}

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexTitlelogo(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// ���_���W�̐ݒ�
	SetVertexTitlelogo(no);

	// rhw�̐ݒ�
	titlelogo->vertexWk[0].rhw =
	titlelogo->vertexWk[1].rhw =
	titlelogo->vertexWk[2].rhw =
	titlelogo->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	titlelogo->vertexWk[0].diffuse =
	titlelogo->vertexWk[1].diffuse =
	titlelogo->vertexWk[2].diffuse =
	titlelogo->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	titlelogo->vertexWk[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	titlelogo->vertexWk[1].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLELOGO, 0.0f );
	titlelogo->vertexWk[2].tex = D3DXVECTOR2( 0.0f, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO );
	titlelogo->vertexWk[3].tex = D3DXVECTOR2( 1.0f/TEXTURE_PATTERN_DIVIDE_X_TITLELOGO, 1.0f/TEXTURE_PATTERN_DIVIDE_Y_TITLELOGO );

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�֐�
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
// ���ˌ��̐ݒ�֐�
//=============================================================================
void SetDiffuseTitlelogo(int no, float fAlpha)
{
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// ���ˌ��̐ݒ�
	titlelogo->vertexWk[0].diffuse =
	titlelogo->vertexWk[1].diffuse =
	titlelogo->vertexWk[2].diffuse =
	titlelogo->vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f,1.0f, fAlpha);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureTitlelogo( int no, int cntPattern )
{
	TITLELOGO *titlelogo = &titlelogoWk[no];

	// �e�N�X�`�����W�̐ݒ�
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
// �^�C�g���擾�֐�
//=============================================================================
TITLELOGO *GetTitlelogo(int no)
{
	return(&titlelogoWk[no]);
}

