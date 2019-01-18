//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "GUI.h"
#include "score.h"
#include "saveData.h"
#include "stageManager.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexScore(void);							//���_�쐬�֐�
void SetTextureScore(void);								//�e�N�X�`�����W�̌v�Z����
void SetTextureScoreNum(int cntPattern);				//�e�N�X�`�����W�̌v�Z����
void SetVertexScore(void);								//���_�̌v�Z����
void SetVertexScoreNum(float offset);					//���_�̌v�Z����
void SetVertexScoreBack(void);							//���_�̌v�Z����

enum SCORETEXTURE_INDEX
{
	TEXTURE_SCOREBACK,
	TEXTURE_SCORENUM,
};

enum {
	DEFAULT,
	ENLARGEMENT,
	REDUCTION
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SCORE_TEXTURE_NUM] = { NULL, NULL };					// �X�R�A�e�N�X�`���ւ̃|�C���^

static VERTEX_2D				vertexWk[NUM_VERTEX];						//���_���i�[���[�N

static float					radius, radiusNum, radiusBack;				//�|���S���̒��_�ւ̒���
static float					angle, angleNum, angleBack;					//�|���S���̒��_�ւ̊p�x

static float					scale;										//�|���S���̃X�P�[��
static int						state;										//�`��X�e�[�g
static int						digitMax;									//�\�����錅�̍ő吔
static D3DXVECTOR3				basePos;									//����W

SCOREDATA						scoreData;									//�X�R�A�f�[�^�\����

/******************************************************************************
����������
******************************************************************************/
HRESULT InitScore(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREDATA *ptr = GetScoreDataAdr();
	SAVEDATA *saveData = GetSaveDataAdr();

	ptr->score = 0;

	//����W�̐ݒ�
	basePos = D3DXVECTOR3(SCORE_INITPOS_X, SCORE_INITPOS_Y, 0.0f);

	//�e�p�����[�^�̏�����
	radiusNum = D3DXVec2Length(&D3DXVECTOR2(SCORENUM_TEXTURE_SIZE_X, SCORENUM_TEXTURE_SIZE_Y));
	radiusBack = D3DXVec2Length(&D3DXVECTOR2(SCOREBACK_TEXTURE_SIZE_X, SCOREBACK_TEXTURE_SIZE_Y));

	angleNum = atan2f(SCORENUM_TEXTURE_SIZE_Y / 2, SCORENUM_TEXTURE_SIZE_X / 2);
	angleBack = atan2f(SCOREBACK_TEXTURE_SIZE_Y / 2, SCOREBACK_TEXTURE_SIZE_X / 2);

	scale = 1.0f;
	state = DEFAULT;
	digitMax = 1;

	// ���_���̍쐬
	MakeVertexScore();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[TEXTURE_SCORENUM] = CreateTextureFromFile((LPSTR)SCORENUM_TEXTURE_NAME, pDevice);
		texture[TEXTURE_SCOREBACK] = CreateTextureFromFile((LPSTR)SCORE_TEXTURE_NAME, pDevice);

		//�Z�[�u�f�[�^����n�C�X�R�A���̓Ǎ�
		for (int i = 0; i < HISCORE_MAX; i++)
		{
			ptr->HiScore[i] = saveData->highScore[i];
			//strcpy(&ptr->playerName[i][0], &saveData->hiScore[i].name[0]);
			//memcpy(&ptr->playerName[i][0], &saveData->name[i][0], sizeof(ptr->playerName[i]));
			ArrayCopy(&ptr->playerName[i][0], &saveData->name[i][0], PLAYERNAME_MAX);
		}
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitScore(void)
{
	int i;
	for (i = 0; i < SCORE_TEXTURE_NUM; i++)
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
void UpdateScore(void)
{
	//�h�炷����
	basePos = D3DXVECTOR3(SCORE_INITPOS_X, SCORE_INITPOS_Y, 0.0f);
	
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

	//�g�又��
	if (state == ENLARGEMENT)
	{
		scale += 0.05f;

		if (scale >= 1.5f)
		{
			state = REDUCTION;
			return;
		}
	}
	//�k������
	else if (state == REDUCTION)
	{
		scale -= 0.05f;
		if (scale <= 1.0f)
		{
			state = DEFAULT;
			return;
		}
	}
}

/******************************************************************************
�`�揈��(�����p�j
******************************************************************************/
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 offsetPos = D3DXVECTOR3(SCORENUM_INITPOS_X, SCORENUM_INITPOS_Y, 0.0f);
	SCOREDATA *ptr = GetScoreDataAdr();
	int i, tmpScore, data;
	float offset = 0.0f;

	DrawScoreBack();

	//�X�R�A�����̕`��
	pDevice->SetTexture(0, texture[TEXTURE_SCORENUM]);
	tmpScore = ptr->score;
	for (i = 0; i < digitMax; i++, tmpScore /= 10)
	{
		data = tmpScore % 10;
		//offsetPos.x = (float)SCORENUM_INITPOS_X - i * SCORENUM_OFFSETPOS_X;
		offset = i * SCORENUM_OFFSETPOS_X;

		//���_���W��ݒ�
		SetVertexScoreNum(offset);

		// �e�N�X�`�����W��ݒ�
		SetTextureScoreNum(data);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}

/******************************************************************************
�`�揈��(�w�i�p�j
******************************************************************************/
void DrawScoreBack(void)
{
	PDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�X�R�A�w�i�̕`��
	pDevice->SetTexture(0, texture[TEXTURE_SCOREBACK]);
	SetTextureScore();
	SetVertexScoreBack();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexScore(void)
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
void SetTextureScore(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

void SetTextureScoreNum(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SCORENUM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCORENUM_TEXTURE_DIVIDE_X;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexScore(void)
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
void SetVertexScoreNum(float offset)
{
	D3DXVECTOR3 pos = basePos + D3DXVECTOR3(SCORENUM_INITPOS_X, SCORENUM_INITPOS_Y, 0.0f);
	pos.x += SCORENUM_OFFSETPOS_X * (digitMax - 1);
	pos.x -= offset;

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

/******************************************************************************
���_���W�̐ݒ�(�w�i�p)
******************************************************************************/
void SetVertexScoreBack(void)
{
	//���_���W�̐ݒ�
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
//�֐���	�FSCORE *GetScoreAdr(void)
//����		�Fvoid
//�߂�l	�F�X�R�A�f�[�^�̃A�h���X
//����		�F�X�R�A�f�[�^�A�h���X�擾�֐�
******************************************************************************/
SCOREDATA *GetScoreDataAdr(void)
{
	return &scoreData;
}

/******************************************************************************
//�֐���	�FSCORE *GetScoreAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
void AddScore(int value)
{
	SCOREDATA *ptr = GetScoreDataAdr();

	ptr->score += value;

	if (ptr->score >= pow(10, SCORE_MAX))
	{
		ptr->score = (int)pow(10, SCORE_MAX) - 1;
	}

	if (state == DEFAULT)
	{
		state = ENLARGEMENT;
	}

	//�\�����̏���
	int digit = (int)log10((double)ptr->score) + 1;
	digitMax = digit;
}

/******************************************************************************
//�֐���	�Fvoid CheckHiscoreUpdate(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F�n�C�X�R�A�̍X�V�����邩�ǂ����̃`�F�b�N
******************************************************************************/
void CheckHiscoreUpdate(int *name)
{
	SCOREDATA *ptr = GetScoreDataAdr();
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		if (ptr->score != Max(ptr->score, ptr->HiScore[i]))
		{
			continue;
		}

		ArrayShiftDown(&ptr->HiScore[0], i, HISCORE_MAX);
		ptr->HiScore[i] = ptr->score;

		ShiftHiscoreName(i);
		ArrayCopy(&ptr->playerName[i][0], &name[0], PLAYERNAME_MAX);
		break;

	}
}

/******************************************************************************
//�֐���	�Fbool isUpdateHighScore(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F�X�R�A���n�C�X�R�A���X�V���Ă��邩�`�F�b�N
******************************************************************************/
bool isUpdateHighScore(void)
{
	SCOREDATA *ptr = &scoreData;

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		if (ptr->score == Max(ptr->score, ptr->HiScore[i]))
		{
			return true;
		}
	}

	return false;
}

/******************************************************************************
//�֐���	�Fvoid ShiftRankingName(int start)
//����		�Fint start		�F�V�t�g�̊J�n�ʒu
//�߂�l	�Fvoid
//����		�F�n�C�X�R�A�̃l�[�����J�n�ʒu���牺�ɃV�t�g����֐�
******************************************************************************/
void ShiftHiscoreName(int start)
{
	SCOREDATA *ptr = GetScoreDataAdr();

	for (int i = HISCORE_MAX - 1; i > start; i--)
	{
		for (int j = 0; j < PLAYERNAME_MAX; j++)
		{
			ptr->playerName[i][j] = ptr->playerName[i - 1][j];
		}
	}
}