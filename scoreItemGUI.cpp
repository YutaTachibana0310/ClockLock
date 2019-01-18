//=============================================================================
//
// ���� [scoreItemGUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "scoreItem.h"
#include "scoreItemGUI.h"
#include "GUI.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SCOREITEMGUI_SCALE_MAX			(1.5f)		//�X�P�[���̍ő�l
#define SCOREITEMGUI_SCALE_MIN			(1.0f)		//�X�P�[���̍ŏ��l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexScoreItemGUI(void);				//���_�쐬�֐�
void SetTextureScoreItemGUI(void);					// �e�N�X�`�����W�̌v�Z�����i�w�i�p�j
void SetTextureScoreItemNum(int cntPattern);		// �e�N�X�`�����W�̌v�Z�����i�����p�j
void SetVertexScoreItemGUI(void);					// ���_�̌v�Z�����i�w�i�p�j
void SetVertexScoreItemNum(float offset);			// ���_�̌v�Z�����i�����p�j
/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum TEXTURE
{
	BACK,
	NUM
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[SCOREITEMGUI_TEXTURE_NUM]; // �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
//static D3DXVECTOR3 posPolygon;							//�|���S���̍��W
static float angleNum, radiusNum, scale;				//�|���S���̊p�x�A���a�A�X�P�[���i�����p�j
static float angle, radius;								//�|���S���̊p�x�A���a�i�w�i�p�j
static D3DXVECTOR3 basePos;								//����W

/******************************************************************************
����������
******************************************************************************/
HRESULT InitScoreItemGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	angle = atan2f(SCOREITEMGUI_TEXTURE_SIZE_Y, SCOREITEMGUI_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(SCOREITEMGUI_TEXTURE_SIZE_X, SCOREITEMGUI_TEXTURE_SIZE_Y));
	angleNum = atan2f(SCOREITEMGUI_NUMTEX_SIZE_Y, SCOREITEMGUI_NUMTEX_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(SCOREITEMGUI_NUMTEX_SIZE_X, SCOREITEMGUI_NUMTEX_SIZE_Y));
	scale = 1.0f;

	//����W�̐ݒ�
	basePos = D3DXVECTOR3(SCOREITEMGUI_INITPOS_X, SCOREITEMGUI_INITPOS_Y, 0.0f);

	// ���_���̍쐬
	MakeVertexScoreItemGUI();

	if(num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BACK] = CreateTextureFromFile((LPSTR)SCOREITEMGUI_TEXTURE_NAME, pDevice);
		texture[NUM] = CreateTextureFromFile((LPSTR)SCOREITEMGUI_NUMTEX_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitScoreItemGUI(void)
{
	for (int i = 0; i < SCOREITEMGUI_TEXTURE_NUM; i++)
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
void UpdateScoreItemGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(SCOREITEMGUI_INITPOS_X, SCOREITEMGUI_INITPOS_Y, 0.0f);

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
void DrawScoreItemGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, data, digitMax;
	int cntItem = GetScoreItemCount();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//������`��
	pDevice->SetTexture(0, texture[NUM]);
	digitMax = (cntItem > 0) ? (int)log10((double)cntItem) + 1 : 1;
	for (i = 0; i < digitMax; i++, cntItem /= 10)
	{
		data = cntItem % 10;
		SetTextureScoreItemNum(data);
		SetVertexScoreItemNum(SCOREITEMGUI_NUM_OFFSETPOS * i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
�`�揈��(�w�i�p)
******************************************************************************/
void DrawScoreItemGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//GUI�w�i��`��
	pDevice->SetTexture(0, texture[BACK]);
	SetTextureScoreItemGUI();
	SetVertexScoreItemGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexScoreItemGUI(void)
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
�e�N�X�`�����W�̐ݒ�(�w�i�p)
******************************************************************************/
void SetTextureScoreItemGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�����p)
******************************************************************************/
void SetTextureScoreItemNum(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SCOREITEMGUI_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCOREITEMGUI_TEXTURE_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�(�w�i�p)
******************************************************************************/
void SetVertexScoreItemGUI(void)
{
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
void SetVertexScoreItemNum(float offset)
{
	// ���_���W�̐ݒ�
	float scale = GetScoreItemScale();
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(SCOREITEMGUI_NUM_INITPOS_X, SCOREITEMGUI_NUM_INITPOS_Y, 0.0f);
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