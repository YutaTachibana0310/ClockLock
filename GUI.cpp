//=============================================================================
//
// GUI���� [GUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "GUI.h"
#include "Score.h"
#include "scoreItemGUI.h"
#include "zankiGUI.h"
#include "energyGUI.h"
#include "hiScoreGUI.h"
#include "bomGUI.h"
#include "comboGUI.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define GUI_QUAKE_FRAME			(60)					//GUI���h��鎞��

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexGUIback(void);						// GUI�w�i�̒��_�쐬
void SetTextureGUIback(void);							// GUI�w�i�̃e�N�X�`���Z�b�g
void SetVertexGUIback(D3DXVECTOR3 pos);					// ���_�̌v�Z����

/*****************************************************************************
 �O���[�o���ϐ�
*****************************************************************************/
static VERTEX_2D				vertexWk[NUM_VERTEX];		// ���_���i�[���[�N	
static LPDIRECT3DTEXTURE9		texture = NULL;		// �e�N�X�`���ւ̃|�C���^
static float					g_fRadius;					// �|���S���̔��a
static float					g_fBaseAngle;				// �|���S���̊p�x

GUIback back;												// GUI�w�i

/******************************************************************************
����������
******************************************************************************/
HRESULT InitGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUIback *back = GetGUIbackAdr();

	//�w�i�̍��ɂ��ď�����
	back->posL = D3DXVECTOR3(BG_POS_X, 0.0f, 0.0f);

	//�w�i�E�ɂ��ď�����
	back->posR = D3DXVECTOR3(BG_POS_X + BG_TEXTURESIZE_X + GUI_TEXTURE_SIZE_X, 0.0f, 0.0f);

	// ���_���̍쐬
	MakeVertexGUIback();

	back->state = GUI_DEFAULT;

	if (num == 0)
	{
		texture = CreateTextureFromFile((LPSTR)GUI_TEXTURE_BACK, pDevice);
	}

	InitScore(num);
	InitScoreItemGUI(num);
	InitZankiGUI(num);
	InitEnergyGUI(num);
	InitHiScoreGUI(num);
	InitBomGUI(num);
	InitComboGUI(num);

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitGUI(void)
{
	if (texture != NULL)
	{	// �e�N�X�`���̊J��
		texture->Release();
		texture = NULL;
	}

	UninitScore();
	UninitScoreItemGUI();
	UninitZankiGUI();
	UninitEnergyGUI();
	UninitHiScoreGUI();
	UninitBomGUI();
	UninitComboGUI();
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateGUI(void)
{
	//QUAKE��Ԃ�GUI�̂Ƃ��̓t���[�����J�E���g
	GUIback *ptr = &back;
	if (ptr->state == GUI_QUAKE)
	{
		ptr->cntFrame++;

		if (ptr->cntFrame == GUI_QUAKE_FRAME)
		{
			ptr->state = GUI_DEFAULT;
		}
	}

	//�eGUI�̍X�V
	UpdateScore();
	UpdateScoreItemGUI();
	UpdateZankiGUI();
	UpdateEnergyGUI();
	UpdateHiScoreGUI();
	UpdateBomGUI();
	UpdateComboGUI();
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GUIback *back = GetGUIbackAdr();
	pDevice->SetTexture(0, texture);

	// ���u�����h���s��Ȃ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_���̐ݒ�
	SetVertexGUIback(back->posR);

	// �w�i�E�̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	// ���_���̐ݒ�
	SetVertexGUIback(back->posL);

	// �w�i���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//DrawScoreBack();
	DrawScoreItemGUIBack();
	//DrawZankiGUIBack();
	DrawEnergyGUIBack();
	DrawHiScoreGUIBack();
	//DrawBomGUIBack();
	DrawComboGUIBack();

	//���u�����h���Đݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	DrawScoreItemGUI();

	DrawEnergyGUI();
	DrawHiscoreNum();
	DrawComboGUI();
}

/******************************************************************************
�Q�[����ʏ�̃p�����[�^�\��
******************************************************************************/
void DrawParameter(void)
{
	DrawScore();
	DrawZankiGUI();
	DrawBomGUI();
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexGUIback(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureGUIback();

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureGUIback(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexGUIback(D3DXVECTOR3 pos)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y, pos.z);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x - GUI_TEXTURE_SIZE_X, pos.y, pos.z);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y + GUI_TEXTURE_SIZE_Y, pos.z);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x - GUI_TEXTURE_SIZE_X, pos.y + GUI_TEXTURE_SIZE_Y, pos.z);
}

/****************************************************************
�֐���	�FGUIback *GetGUIbackAdr(void)
����	�FGUI�w�i�A�h���X�擾�֐�
����	�Fvoidint pno�F�A�h���X���擾������GUI�w�i�̃C���f�b�N�X
�߂�l	�FGUIback�̃A�h���X
*****************************************************************/
GUIback *GetGUIbackAdr(void)
{
	return &back;
}

/****************************************************************
�֐���	�Fvoid SetGUIShake(void)
����	�FGUI��h�炷��ԂɃZ�b�g����֐�
����	�Fvoid
�߂�l	�Fvoid
*****************************************************************/
void SetGUIShake(void)
{
	GUIback *ptr = &back;

	ptr->cntFrame = 0;
	ptr->state = GUI_QUAKE;
}