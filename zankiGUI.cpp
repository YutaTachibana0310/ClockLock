//=============================================================================
//
// �c�@GUI���� [zankiGUI.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "zankiGUI.h"
#include "GUI.h"
#include "player.h"
#include "charChip.h"
#include "stageManager.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/


/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexZankiGUI(void);							// ���_�쐬�֐�
void SetTextureZankiGUI(void);								// �e�N�X�`�����W�̌v�Z����
void SetVertexZankiGUI(float offset);						// ���_�̌v�Z�����i�A�C�R���p�j
void SetVertexZankiGUIBack(void);							// ���_�̌v�Z�����i�w�i�p�j
void SetVertexZankiGUIChar(void);							// ���_�̌v�Z�����i�����p�j
void SetVertexZankiGUINum(float offset, int digitMax);		// ���_�̌v�Z�����i�����p�j

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
static LPDIRECT3DTEXTURE9 texture[ZANKIGUI_TEXTURE_NUM];	// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D		vertexWk[NUM_VERTEX];				//���_���i�[���[�N

static float angleBack, angleIcon, angleChar;				//�|���S���̊p�x
static float radiusBack, radiusIcon, radiusChar;			//�|���S���̔��a
static float radiusNum, angleNum;
static D3DXVECTOR3 basePos;									//����W

/******************************************************************************
����������
******************************************************************************/
HRESULT InitZankiGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//����W��ݒ�
	basePos = D3DXVECTOR3(ZANKIGUI_BACK_INITPOS_X, ZANKIGUI_BACK_INITPOS_Y, 0.0f);

	//�e�p�����[�^���v�Z
	angleBack = atan2f(ZANKIGUI_BACKTEX_SIZE_Y, ZANKIGUI_BACKTEX_SIZE_X);
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_BACKTEX_SIZE_X, ZANKIGUI_BACKTEX_SIZE_Y));

	angleIcon = atan2f(ZANKIGUI_TEXTURE_SIZE_Y, ZANKIGUI_TEXTURE_SIZE_X);
	radiusIcon = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_TEXTURE_SIZE_X, ZANKIGUI_TEXTURE_SIZE_Y));

	angleChar = atan2f(ZANKIGUI_CHAR_SIZE_Y, ZANKIGUI_CHAR_SIZE_X);
	radiusChar = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_CHAR_SIZE_X, ZANKIGUI_CHAR_SIZE_Y));

	angleNum = atan2f(ZANKIGUI_NUM_SIZE_Y, ZANKIGUI_NUM_SIZE_X);
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(ZANKIGUI_NUM_SIZE_X, ZANKIGUI_NUM_SIZE_Y));

	// ���_���̍쐬
	MakeVertexZankiGUI();
	SetTextureZankiGUI();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[BACK] = CreateTextureFromFile((LPSTR)ZANKIGUI_BACKTEX_NAME, pDevice);
		texture[ICON] = CreateTextureFromFile((LPSTR)ZANKIGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitZankiGUI(void)
{
	for (int i = 0; i < ZANKIGUI_TEXTURE_NUM; i++)
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
void UpdateZankiGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(ZANKIGUI_BACK_INITPOS_X, ZANKIGUI_BACK_INITPOS_Y, 0.0f);

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
void DrawZankiGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayerAdr(0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[ICON]);

	//�A�C�R���̕`��
	SetVertexZankiGUI(0);
	SetTextureZankiGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�~�̕`��
	SetVertexZankiGUIChar();

	DrawCharChip(vertexWk, CHARCHIP_X);

	//���̕`��
	int digitMax = (player->zanki == 0) ? 1 : (int)log10f((float)player->zanki) + 1;
	int count = player->zanki;
	for (int i = 0; i < digitMax; i++, count /= 10)
	{
		SetVertexZankiGUINum(i * ZANKIGUI_NUM_OFFSETPOS, digitMax);
		DrawYellowNum(vertexWk, count % 10);
	}
	
}

/******************************************************************************
�`�揈��(�w�i�p)
******************************************************************************/
void DrawZankiGUIBack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[BACK]);

	SetVertexZankiGUIBack();
	SetTextureZankiGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexZankiGUI(void)
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
void SetTextureZankiGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�(�A�C�R���p)
******************************************************************************/
void SetVertexZankiGUI(float offset)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(ZANKIGUI_INITPOS_X + offset, ZANKIGUI_INITPOS_Y, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_INITPOS_X, ZANKIGUI_INITPOS_Y, 0.0f);
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
void SetVertexZankiGUIBack(void)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[0].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[1].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[1].vtx.y = basePos.y - sinf(angleBack) * radiusBack;
	vertexWk[2].vtx.x = basePos.x - cosf(angleBack) * radiusBack;
	vertexWk[2].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
	vertexWk[3].vtx.x = basePos.x + cosf(angleBack) * radiusBack;
	vertexWk[3].vtx.y = basePos.y + sinf(angleBack) * radiusBack;
}

/******************************************************************************
���_���W�̐ݒ�(�����p)
******************************************************************************/
void SetVertexZankiGUIChar(void)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_CHAR_INITPOS_X, ZANKIGUI_CHAR_INITPOS_Y, 0.0f);

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angleChar) * radiusChar;
	vertexWk[0].vtx.y = pos.y - sinf(angleChar) * radiusChar;
	vertexWk[1].vtx.x = pos.x + cosf(angleChar) * radiusChar;
	vertexWk[1].vtx.y = pos.y - sinf(angleChar) * radiusChar;
	vertexWk[2].vtx.x = pos.x - cosf(angleChar) * radiusChar;
	vertexWk[2].vtx.y = pos.y + sinf(angleChar) * radiusChar;
	vertexWk[3].vtx.x = pos.x + cosf(angleChar) * radiusChar;
	vertexWk[3].vtx.y = pos.y + sinf(angleChar) * radiusChar;
}

/******************************************************************************
���_���W�̐ݒ�(�����p)
******************************************************************************/
void SetVertexZankiGUINum(float offset, int digitMax)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(ZANKIGUI_NUM_INITPOS_X, ZANKIGUI_NUM_INITPOS_Y, 0.0f);
	pos.x += ZANKIGUI_NUM_OFFSETPOS * (digitMax - 1);
	pos.x -= offset;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum;
}