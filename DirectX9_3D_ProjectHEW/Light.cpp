//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : GP11B243 24 �l���F��
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9				g_aLight[LIGHT_MAX];		// ���C�g���

//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;

	/********** ���C�g0 **********/

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ���C�g0�̃^�C�v�̐ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g0�̊g�U���̐ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g0�̋��ʔ��ˌ��̐ݒ�
	g_aLight[0].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g0�̊����̐ݒ�
	g_aLight[0].Ambient = D3DXCOLOR(LIGHT_AMBIENT, LIGHT_AMBIENT, LIGHT_AMBIENT, 1.0f);

	// ���C�g0�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.01f, -0.54f, -0.842f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight(0, &g_aLight[0]);

	// ���C�g0���g�p��Ԃ�
	pDevice->LightEnable(0, TRUE);


		/********** ���C�g1 **********/
	
		// D3DLIGHT9�\���̂�0�ŃN���A����
		ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));
	
		// ���C�g1�̃^�C�v�̐ݒ�
		g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;
	
		// ���C�g1�̊g�U���̐ݒ�
		g_aLight[1].Diffuse = D3DXCOLOR(LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, LIGHT_DIFFUSE_1, 1.0f);
	
		// ���C�g1�̋��ʔ��ˌ��̐ݒ�
		//	g_aLight[1].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
		// ���C�g1�̊����̐ݒ�
		//	g_aLight[1].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	    // ���C�g1�̕����̐ݒ�
		vecDir = D3DXVECTOR3(-0.01f, 0.54f, -0.842f);
		//vecDir = D3DXVECTOR3(-0.0f, -0.6f, -1.0f);
		//vecDir = D3DXVECTOR3(-0.0f, -1.0f, 0.8f);
		D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);
	
		// ���C�g1�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetLight(1, &g_aLight[1]);
	
		// ���C�g1���g�p��Ԃ�
		pDevice->LightEnable(1, TRUE);


	///********** ���C�g2 **********/

	//// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	//// ���C�g2�̃^�C�v�̐ݒ�
	//g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	//// ���C�g2�̊g�U���̐ݒ�
	//g_aLight[2].Diffuse = D3DXCOLOR(LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, 1.0f);

	//// ���C�g2�̋��ʔ��ˌ��̐ݒ�
	////	g_aLight[2].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g2�̊����̐ݒ�
	////	g_aLight[2].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g2�̕����̐ݒ�
	//vecDir = D3DXVECTOR3(LIGHT_SUB_XZ, LIGHT_SUB_Y, 0.0f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir);

	//// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	//pDevice->SetLight(2, &g_aLight[2]);

	//// ���C�g2���g�p��Ԃ�
	//pDevice->LightEnable(2, TRUE);


	///********** ���C�g3 **********/

	//// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&g_aLight[3], sizeof(D3DLIGHT9));

	//// ���C�g3�̃^�C�v�̐ݒ�
	//g_aLight[3].Type = D3DLIGHT_DIRECTIONAL;

	//// ���C�g3�̊g�U���̐ݒ�
	//g_aLight[3].Diffuse = D3DXCOLOR(LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, 1.0f);

	//// ���C�g3�̋��ʔ��ˌ��̐ݒ�
	////	g_aLight[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g3�̊����̐ݒ�
	////	g_aLight[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g3�̕����̐ݒ�
	//vecDir = D3DXVECTOR3(-LIGHT_SUB_XZ, LIGHT_SUB_Y, 0.0f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction, &vecDir);

	//// ���C�g3�������_�����O�p�C�v���C���ɐݒ�
	//pDevice->SetLight(3, &g_aLight[3]);

	//// ���C�g3���g�p��Ԃ�
	//pDevice->LightEnable(3, TRUE);


	///********** ���C�g4 **********/

	//// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&g_aLight[4], sizeof(D3DLIGHT9));

	//// ���C�g4�̃^�C�v�̐ݒ�
	//g_aLight[4].Type = D3DLIGHT_DIRECTIONAL;

	//// ���C�g4�̊g�U���̐ݒ�
	//g_aLight[4].Diffuse = D3DXCOLOR(LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, 1.0f);

	//// ���C�g4�̋��ʔ��ˌ��̐ݒ�
	////	g_aLight[4].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g4�̊����̐ݒ�
	////	g_aLight[4].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g4�̕����̐ݒ�
	//vecDir = D3DXVECTOR3(0.0f, LIGHT_SUB_Y, LIGHT_SUB_XZ);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[4].Direction, &vecDir);

	//// ���C�g4�������_�����O�p�C�v���C���ɐݒ�
	//pDevice->SetLight(4, &g_aLight[4]);

	//// ���C�g4���g�p��Ԃ�
	//pDevice->LightEnable(4, TRUE);


	///********** ���C�g5 **********/

	//// D3DLIGHT9�\���̂�0�ŃN���A����
	//ZeroMemory(&g_aLight[5], sizeof(D3DLIGHT9));

	//// ���C�g5�̃^�C�v�̐ݒ�
	//g_aLight[5].Type = D3DLIGHT_DIRECTIONAL;

	//// ���C�g5�̊g�U���̐ݒ�
	//g_aLight[5].Diffuse = D3DXCOLOR(LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, LIGHT_SUB_DIFFUSE, 1.0f);

	//// ���C�g5�̋��ʔ��ˌ��̐ݒ�
	////	g_aLight[5].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g5�̊����̐ݒ�
	////	g_aLight[5].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//// ���C�g5�̕����̐ݒ�
	//vecDir = D3DXVECTOR3(0.0f, LIGHT_SUB_Y, -LIGHT_SUB_XZ);
	//D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[5].Direction, &vecDir);

	//// ���C�g5�������_�����O�p�C�v���C���ɐݒ�
	//pDevice->SetLight(5, &g_aLight[5]);

	//// ���C�g5���g�p��Ԃ�
	//pDevice->LightEnable(5, TRUE);


	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}