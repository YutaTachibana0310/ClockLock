//=============================================================================
//
// �{��GUI���� [bomGUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "bomGUI.h"
#include "player.h"
#include "stageManager.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define BOMGUI_SPEED (-40.0f)

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexBomGUI(void);				// ���_�쐬�֐�
void SetTextureBomGUI(void);					// �e�N�X�`�����W�̌v�Z����
void SetVertexBomGUI(float offset);			// ���_�̌v�Z�����i�A�C�R���p�j
void SetVertexBomGUIBack(void);				// ���_�̌v�Z�����i�w�i�p�j

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum 
{
	BACK,
	ICON
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[BOMGUI_TEXTURE_NUM];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D		vertexWk[NUM_VERTEX];				//���_���i�[���[�N

static float angleBack, angleIcon;							//�|���S���̊p�x
static float radiusBack, radiusIcon;						//�|���S���̔��a

static D3DXVECTOR3 basePos;									//����W

/******************************************************************************
����������
******************************************************************************/
HRESULT InitBomGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�p�����[�^���v�Z
	angleBack = atan2f(BOMGUI_BACKTEX_SIZE_Y, BOMGUI_BACKTEX_SIZE_X);
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(BOMGUI_BACKTEX_SIZE_X, BOMGUI_BACKTEX_SIZE_Y));

	angleIcon = atan2f(BOMGUI_TEXTURE_SIZE_Y, BOMGUI_TEXTURE_SIZE_X);
	radiusIcon = D3DXVec2Length(&D3DXVECTOR2(BOMGUI_TEXTURE_SIZE_X, BOMGUI_TEXTURE_SIZE_Y));

	// ���_���̍쐬
	MakeVertexBomGUI();
	SetTextureBomGUI();

	//����W��ݒ�
	basePos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BACK] = CreateTextureFromFile((LPSTR)BOMGUI_BACKTEX_NAME, pDevice);
		texture[ICON] = CreateTextureFromFile((LPSTR)BOMGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitBomGUI(void)
{
	for (int i = 0; i < BOMGUI_TEXTURE_NUM; i++)
	{
		if (texture[i] != NULL)
		{	// �e�N�X�`���̊J��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateBomGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);
	if (GetStageManagerAdr()->state == STAGE_BOSSBATTLE)
	{
		basePos.y += BOSSGAUGE_OFFSET;
	}

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawBomGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayerAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[ICON]);

	for (i = 0; i < player->bomCnt; i++)
	{
		SetVertexBomGUI(BOMGUI_OFFSETPOS * i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
�`�揈��(�w�i�p)
******************************************************************************/
void DrawBomGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexBomGUIBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexBomGUI(void)
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
void SetTextureBomGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�(�A�C�R���p)
******************************************************************************/
void SetVertexBomGUI(float offset)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(BOMGUI_INITPOS_X + offset, BOMGUI_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(BOMGUI_INITPOS_X, BOMGUI_INITPOS_Y, 0.0f);
	pos.x += offset;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angleIcon) * radiusIcon;
	vertexWk[0].vtx.y = pos.y - sinf(angleIcon) * radiusIcon;
	vertexWk[1].vtx.x = pos.x + cosf(angleIcon) * radiusIcon;
	vertexWk[1].vtx.y = pos.y - sinf(angleIcon) * radiusIcon;
	vertexWk[2].vtx.x = pos.x - cosf(angleIcon) * radiusIcon;
	vertexWk[2].vtx.y = pos.y + sinf(angleIcon) * radiusIcon;
	vertexWk[3].vtx.x = pos.x + cosf(angleIcon) * radiusIcon;
	vertexWk[3].vtx.y = pos.y + sinf(angleIcon) * radiusIcon;
}

/******************************************************************************
���_���W�̐ݒ�(�w�i�p)
******************************************************************************/
void SetVertexBomGUIBack(void)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(BOMGUI_BACK_INITPOS_X, BOMGUI_BACK_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angleBack) * radiusBack;
	vertexWk[0].vtx.y = pos.y - sinf(angleBack) * radiusBack;
	vertexWk[1].vtx.x = pos.x + cosf(angleBack) * radiusBack;
	vertexWk[1].vtx.y = pos.y - sinf(angleBack) * radiusBack;
	vertexWk[2].vtx.x = pos.x - cosf(angleBack) * radiusBack;
	vertexWk[2].vtx.y = pos.y + sinf(angleBack) * radiusBack;
	vertexWk[3].vtx.x = pos.x + cosf(angleBack) * radiusBack;
	vertexWk[3].vtx.y = pos.y + sinf(angleBack) * radiusBack;
}