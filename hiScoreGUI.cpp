//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "score.h"
#include "hiScoreGUI.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexHiScoreGUI(void);							//���_�쐬�֐�
void SetTextureHiScoreGUI(void);								//�e�N�X�`�����W�̌v�Z����
void SetVertexHiScoreGUINum(float offsetX, float offsetY);		//���_�̌v�Z����(�����p)
void SetVertexHiScoreGUIBack(void);							//���_�̌v�Z�����i�w�i�p�j

enum HISCORE_TEXTURE
{
	TEXTURE_BACK,
	TEXTURE_MAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[TEXTURE_MAX];				// �X�R�A�e�N�X�`���ւ̃|�C���^

static VERTEX_2D				vertexWk[NUM_VERTEX];				//���_���i�[���[�N

static float					radiusNum, radiusBack;				//�|���S���̒��_�ւ̒���
static float					angleNum, angleBack;				//�|���S���̒��_�ւ̊p�x
static float					scale;								//�|���S���̃X�P�[��
static int						scoreBuff[HISCORE_DRAW_MAX];		//�`�悷��X�R�A�̔z��
static D3DXVECTOR3				basePos;							//����W
//static int					state;								//�`��X�e�[�g

/******************************************************************************
����������
******************************************************************************/
HRESULT InitHiScoreGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	scale = 1.0f;

	//�e�p�����[�^�̏�����
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(HISCORENUM_TEXTURE_SIZE_X, HISCORENUM_TEXTURE_SIZE_Y));
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(HISCOREBACK_TEXTURE_SIZE_X, HISCOREBACK_TEXTURE_SIZE_Y));

	angleNum = atan2f(HISCORENUM_TEXTURE_SIZE_Y, HISCORENUM_TEXTURE_SIZE_X);
	angleBack = atan2f(HISCOREBACK_TEXTURE_SIZE_Y, HISCOREBACK_TEXTURE_SIZE_X);

	//����W�̐ݒ�
	basePos = D3DXVECTOR3(HISCORE_INITPOS_X, HISCORE_INITPOS_Y, 0.0f);
	
	// ���_���̍쐬
	MakeVertexHiScoreGUI();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[TEXTURE_BACK] = CreateTextureFromFile((LPSTR)HISCORE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitHiScoreGUI(void)
{
	int i;
	for (i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i] != NULL)
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateHiScoreGUI(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(HISCORE_INITPOS_X, HISCORE_INITPOS_Y, 0.0f);

	GUIback *gui = GetGUIbackAdr();
	if (gui->state == GUI_QUAKE)
	{
		basePos.x += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
		basePos.y += RandomRange(-GUI_QUAKE_LENGTHMAX, GUI_QUAKE_LENGTHMAX);
	}

	SCOREDATA *data = GetScoreDataAdr();

	//���݂̃n�C�X�R�A��z��ɕۑ�
	for (int i = 0; i < HISCORE_DRAW_MAX; i++)
	{
		scoreBuff[i] = data->HiScore[i];
	}

	//�n�C�X�R�A�̑}�����K�v���m�F
	for (int i = 0; i < HISCORE_DRAW_MAX; i++)
	{
		if (data->score == Max(data->score, scoreBuff[i]))
		{
			ArrayShiftDown(&scoreBuff[0], i, HISCORE_DRAW_MAX);
			scoreBuff[i] = data->score;
			break;
		}
	}
}

/******************************************************************************
�`�揈��(�����p�j
******************************************************************************/
void DrawHiScoreGUI(void)
{

}

/******************************************************************************
�`�揈��(�w�i�p�j
******************************************************************************/
void DrawHiScoreGUIBack(void)
{
	PDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�X�R�A�w�i�̕`��
	pDevice->SetTexture(0, texture[TEXTURE_BACK]);
	SetTextureHiScoreGUI();
	SetVertexHiScoreGUIBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}

/******************************************************************************
�`�揈��(�����p�j
******************************************************************************/
void DrawHiscoreNum(void)
{
	SCOREDATA *scoreData = GetScoreDataAdr();
	int digitMax, hiScore, num;

	for (int i = 0, drawNum = 0; i < HISCORE_DRAW_MAX; i++)
	{
		hiScore = scoreBuff[i];
		digitMax = (hiScore > 0) ? (int)log10((double)hiScore) + 1 : 1;

		for (int j = 0; j < digitMax; j++, hiScore /= 10)
		{
			num = hiScore % 10;
			SetVertexHiScoreGUINum(j * HISCORENUM_OFFSETPOS_X, i * HISCORENUM_OFFSETPOS_Y);
			DrawYellowNum(vertexWk, num);
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexHiScoreGUI(void)
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
void SetTextureHiScoreGUI(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�(�w�i�p)
******************************************************************************/
void SetVertexHiScoreGUIBack(void)
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
void SetVertexHiScoreGUINum(float offsetX, float offsetY)
{
	//D3DXVECTOR3 pos = D3DXVECTOR3(HISCORENUM_INITPOS_X + offsetX, HISCORENUM_INITPOS_Y + offsetY, 0.0f);
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(HISCORENUM_INITPOS_X, HISCORENUM_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[0].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[1].vtx.y = pos.y - sinf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.x = pos.x - cosf(angleNum) * radiusNum * scale;
	vertexWk[2].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.x = pos.x + cosf(angleNum) * radiusNum * scale;
	vertexWk[3].vtx.y = pos.y + sinf(angleNum) * radiusNum * scale;
}