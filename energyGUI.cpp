//=============================================================================
//
// ���� [energyGUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "player.h"
#include "energyGUI.h"
#include "myLibrary.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define ENERGYGUI_NUM_LOOPMAX				(3)		//�����`�揈���̃��[�v�ő吔

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexEnergyGUI(void);				//���_�쐬�֐�
void SetTextureEnergyGUI(void);					//�e�N�X�`�����W�̌v�Z����
void SetTextureEnergyGUIGauge(void);			//�e�N�X�`�����W�̌v�Z�����i�Q�[�W�p�j
void SetVertexEnergyGUI(void);					//���_�̌v�Z����
void SetVertexEnergyGUIGauge(void);				//���_�̌v�Z�����i�Q�[�W�p�j
void SetVertexEnergyGUINum(float offset);		//���_�̌v�Z�����i�����p�j			

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum TEX_INDEX
{
	BACK,
	GAUGE,
	FRAME,
	TEX_MAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[TEX_MAX];				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D		vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, angleNum;									//�|���S���̊p�x
static float radius, radiusNum;									//�|���S���̔��a
static D3DXVECTOR3 basePos;										//����W

/******************************************************************************
����������
******************************************************************************/
HRESULT InitEnergyGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexEnergyGUI();

	//����W��ݒ�
	basePos = D3DXVECTOR3(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y, 0.0f);

	//�e�p�����[�^���v�Z
	angle = atan2f(ENERGYGUI_BACKTEX_SIZE_Y, ENERGYGUI_BACKTEX_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(ENERGYGUI_BACKTEX_SIZE_X, ENERGYGUI_BACKTEX_SIZE_Y));

	angleNum = atan2f(ENERGYGUI_NUMTEX_SIZE_Y, ENERGYGUI_NUMTEX_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(ENERGYGUI_NUMTEX_SIZE_X, ENERGYGUI_NUMTEX_SIZE_Y));

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BACK] = CreateTextureFromFile((LPSTR)ENERGYGUI_BACKTEX_NAME, pDevice);
		texture[GAUGE] = CreateTextureFromFile((LPSTR)ENERGYGUI_TEXTURE_NAME, pDevice);
		texture[FRAME] = CreateTextureFromFile((LPSTR)ENERGYGUI_FRAMETEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitEnergyGUI(void)
{
	for (int i = 0; i < TEX_MAX; i++)
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
void UpdateEnergyGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y, 0.0f);

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
void DrawEnergyGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �Q�[�W��`��
	pDevice->SetTexture(0, texture[GAUGE]);
	SetVertexEnergyGUIGauge();
	SetTextureEnergyGUIGauge();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�Q�[�W�t���[����`��
	pDevice->SetTexture(0, texture[FRAME]);
	SetVertexEnergyGUI();
	SetTextureEnergyGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//���l��`��
	DrawEnergyGUINum();


}

/******************************************************************************
�`�揈���i�w�i�p�j
******************************************************************************/
void DrawEnergyGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[BACK]);

	//���_���W��ݒ�
	SetVertexEnergyGUI();

	// �e�N�X�`�����W��ݒ�
	SetTextureEnergyGUI();

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈���i�����p�j
******************************************************************************/
void DrawEnergyGUINum(void)
{
	PLAYER *player = GetPlayerAdr(0);

	float ratio = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	int percent = (int)(ratio * 100);
	int digit = (percent != 0) ? (int)log10((double)percent) + 1 : 1;
	
	int num;
	for (int i = 0; i < digit; i++, percent /= 10)
	{
		num = percent % 10;
		SetVertexEnergyGUINum(i * ENERGYGUI_NUM_OFFSET);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexEnergyGUI(void)
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
�e�N�X�`�����W�̐ݒ�i�w�i�p�j
******************************************************************************/
void SetTextureEnergyGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�Q�[�W�p)
******************************************************************************/
void SetTextureEnergyGUIGauge(void)
{
	PLAYER *player = GetPlayerAdr(0);

	float sizeX = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(sizeX, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�i�w�i�p�j
******************************************************************************/
void SetVertexEnergyGUI(void)
{
	//static D3DXVECTOR2 pos = D3DXVECTOR2(ENERGYGUI_INITPOS_X, ENERGYGUI_INITPOS_Y);
	D3DXVECTOR3 pos = basePos;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = pos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = pos.y + sinf(angle) * radius;
}

/******************************************************************************
���_���W�̐ݒ�i�Q�[�W�p�j
******************************************************************************/
void SetVertexEnergyGUIGauge(void)
{
	//static D3DXVECTOR3 pos = D3DXVECTOR3(ENERGYGUI_GAUGE_INITPOS_X, ENERGYGUI_GAUGE_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ENERGYGUI_GAUGE_INITPOS_X, ENERGYGUI_GAUGE_INITPOS_Y, 0.0f);

	PLAYER *player = GetPlayerAdr(0);
	float sizeX = player->timeEffect / PLAYER_TIMEEFFECT_MAX;
	
	vertexWk[0].vtx.x = pos.x;
	vertexWk[0].vtx.y = pos.y;
	vertexWk[1].vtx.x = pos.x + ENERGYGUI_TEXTURE_SIZE_X * sizeX;
	vertexWk[1].vtx.y = pos.y;
	vertexWk[2].vtx.x = pos.x;
	vertexWk[2].vtx.y = pos.y + ENERGYGUI_TEXTURE_SIZE_Y;
	vertexWk[3].vtx.x = pos.x + ENERGYGUI_TEXTURE_SIZE_X * sizeX;
	vertexWk[3].vtx.y = pos.y + ENERGYGUI_TEXTURE_SIZE_Y;
}

/******************************************************************************
���_���W�̐ݒ�i�����p�j
******************************************************************************/
void SetVertexEnergyGUINum(float offset)
{
	//D3DXVECTOR2 pos = D3DXVECTOR2(ENERGYGUI_NUM_INITPOS_X + offset, ENERGYGUI_NUM_INITPOS_Y);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ENERGYGUI_NUM_INITPOS_X, ENERGYGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += offset;

	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum;
}