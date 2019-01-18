//=============================================================================
//
// �R���{GUI���� [comboGUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "comboGUI.h"
#include "GUI.h"
#include "stageManager.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define COMBOGUI_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexComboGUI(void);				//���_�쐬�֐�
void SetTextureComboGUI(void);					// �e�N�X�`�����W�̌v�Z����
void SetVertexComboGUI(void);					// ���_�̌v�Z����
void SetVertexComboGUINum(float offset);		// ���_�̌v�Z�����i�����p�j

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

static float radius;									// �|���S���̔��a
static float angle;										// �|���S���̊p�x

static float radiusNum;									// �����|���S���̔��a
static float angleNum;									// �����|���S���̊p�x

static D3DXVECTOR3 basePos;								//����W

/******************************************************************************
����������
******************************************************************************/
HRESULT InitComboGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�p�����[�^�̌v�Z
	angle = atan2f(COMBOGUI_TEXTURE_SIZE_Y, COMBOGUI_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(COMBOGUI_TEXTURE_SIZE_X, COMBOGUI_TEXTURE_SIZE_Y));

	angleNum = atan2f(COMBOGUI_NUM_SIZE_Y, COMBOGUI_NUM_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(COMBOGUI_NUM_SIZE_X, COMBOGUI_NUM_SIZE_Y));

	//����W�̐ݒ�
	basePos = D3DXVECTOR3(COMBOGUI_INITPOS_X, COMBOGUI_INITPOS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexComboGUI();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)COMBOGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitComboGUI(void)
{
	if (texture != NULL)
	{	// �e�N�X�`���̊J��
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateComboGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(COMBOGUI_INITPOS_X, COMBOGUI_INITPOS_Y, 0.0f);

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}
}

/******************************************************************************
�`�揈��(�����p)
******************************************************************************/
void DrawComboGUI(void)
{
	STAGEMANAGER *manager = GetStageManagerAdr();

	int comboCnt = manager->combo;
	int digitMax = (comboCnt == 0) ? 1 : (int)log10f((float)comboCnt) + 1;
	int num;

	for (int i = 0; i < digitMax; i++, comboCnt /= 10)
	{
		num = comboCnt % 10;
		SetVertexComboGUINum(COMBOGUI_NUM_OFFSET * i);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
�`�揈���i�w�i�p�j
******************************************************************************/
void DrawComboGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);
	SetTextureComboGUI();
	SetVertexComboGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexComboGUI(void)
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

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureComboGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexComboGUI(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = basePos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = basePos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = basePos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = basePos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = basePos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = basePos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = basePos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = basePos.y + sinf(angle) * radius;
}

/******************************************************************************
���_���W�̐ݒ�(�����p)
******************************************************************************/
void SetVertexComboGUINum(float offset)
{
	// ���_���W�̐ݒ�
	float scale = GetStageManagerAdr()->comboScale;
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(COMBOGUI_NUM_INITPOS_X, COMBOGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
}