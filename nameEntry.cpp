//=============================================================================
//
// �l�[���G���g���[���� [nameEntry.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "charChip.h"
#include "nameEntry.h"
#include "input.h"
#include "saveData.h"
#include "Score.h"
#include "transition.h"
#include "sePlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
//���_�쐬�֐�
HRESULT MakeVertexNameEntry(void);

// �e�N�X�`�����W�̌v�Z����
void SetTextureNameEntry(void);					
void SetTextureNameEntryCursor(void);
void SetTextureNameEntryGuide(void);

// ���_�̌v�Z����
void SetVertexNameEntryChar(int charID);		
void SetVertexNameEntryCursor(int charID);
void SetVertexNameEntryName(int wordCnt);
void SetVertexNameEntryBG(void);
void SetVertexNameEntryScore(float offsetX, float offsetY);
void SetVertexNameEntryScoreName(float offsetX, float offsetY);
void SetVertexNameEntryCurrentScore(float offset);
void SetVertexNameEntryGuide(void);

//�l�[���G���g���[�`�揈���i�����p�j
void DrawNameEntryChar(void);					
void DrawNameEntryName(void);					
void DrawNameEntryCursor(void);
void DrawNameEntryBG(void);
void DrawNameEntryHighScore(void);
void DrawNameEntryScoreName(void);
void DrawNameEntryCurrentScore(void);
void DrawNameEntryGuide(void);

bool isDrawSelectedChar(int i);					//�I�𒆂̕����̕`�攻��
void SelectPlayerName();						//�v���C���[�����͏���
void CheckUpdateRanking(void);					//�n�C�X�R�A�z��̍X�V

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum NAMEENTRY_TEXTURE
{
	NAMEENTRY_BACK,
	NAMEENTRY_CURSOR,
	NAMEENTRY_GUIDE,
	NAMEENTRY_TEXMAX
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[NAMEENTRY_TEXMAX];		// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];						//���_���i�[���[�N
static int buffHighScore[HISCORE_MAX];						//�`��p�n�C�X�R�A�z��	
static int buffHighScoreName[HISCORE_MAX][PLAYERNAME_MAX];	//�`��p�v���C���[���z��

NAMEENTRY nameEntry;										//�l�[���G���g���[�\����

/******************************************************************************
����������
******************************************************************************/
HRESULT InitNameEntry(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	NAMEENTRY *ptr = &nameEntry;

	//�e�|���S���p�p�����[�^��������
	ptr->angleChar = atan2f(NAMEENTRY_CHARTEX_SIZE_Y, NAMEENTRY_CHARTEX_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_CHARTEX_SIZE_X, NAMEENTRY_CHARTEX_SIZE_Y));

	ptr->angleCursor = atan2f(NAMEENTRY_CURSORTEX_SIZE_Y, NAMEENTRY_CURSORTEX_SIZE_X);
	ptr->radiusCursor = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_CURSORTEX_SIZE_X, NAMEENTRY_CURSORTEX_SIZE_Y));

	ptr->angleName = atan2f(NAMEENTRY_NAMETEX_SIZE_Y, NAMEENTRY_NAMETEX_SIZE_X);
	ptr->radiusName = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_NAMETEX_SIZE_X, NAMEENTRY_NAMETEX_SIZE_Y));

	ptr->angleScore = atan2f(NAMEENTRY_SCORETEX_SIZE_Y, NAMEENTRY_SCORETEX_SIZE_X);
	ptr->radiusScore = D3DXVec2Length(&D3DXVECTOR2(NAMEENTRY_SCORETEX_SIZE_X, NAMEENTRY_SCORETEX_SIZE_Y));

	ptr->cntFrame = 0;
	ptr->wordCnt = 0;
	ptr->selectedChar = 0;

	ptr->state = NAMEENTRY_INPUT;

	//���͗p�v���C���[����������
	for (int i = 0; i < PLAYERNAME_MAX; i++)
	{
		ptr->nameBuff[i] = CHARCHIP_NONE;
	}

	// ���_���̍쐬
	MakeVertexNameEntry();

	//�T���v���[�X�e�[�g�p�����[�^��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�

	//���݂̃n�C�X�R�A��Ǎ�
	//SAVEDATA *save = GetSaveDataAdr();
	SCOREDATA *data = GetScoreDataAdr();
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		//buffHighScore[i] = save->highScore[i];
		buffHighScore[i] = data->HiScore[i];
		ArrayCopy(&buffHighScoreName[i][0], &data->playerName[i][0], PLAYERNAME_MAX);
	}

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[NAMEENTRY_BACK] = CreateTextureFromFile((LPSTR)NAMEENTRY_TEXTURE_NAME, pDevice);
		texture[NAMEENTRY_CURSOR] = CreateTextureFromFile((LPSTR)NAMEENTRY_CURSORTEX_NAME, pDevice);
		texture[NAMEENTRY_GUIDE] = CreateTextureFromFile((LPSTR)NAMEENTRY_GUIDETEX_NAME, pDevice);
	}

	//�n�C�X�R�A�̍X�V���Ȃ���Ζ��O���͂��s��Ȃ�
	ptr->state = (isUpdateHighScore()) ? NAMEENTRY_INPUT : NAMEENTRY_WAIT;


	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitNameEntry(void)
{
	for (int i = 0; i < NAMEENTRY_TEXMAX; i++)
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
void UpdateNameEntry(void)
{
	NAMEENTRY *ptr = &nameEntry;

	ptr->cntFrame = WrapAround(0, NAMEENTRY_CURSORTEX_ANIMAX, ptr->cntFrame + 1);

	//���O����
	if (ptr->state == NAMEENTRY_INPUT)
	{
		SelectPlayerName();
		return;
	}

	//�{�^���������ꂽ��^�C�g���֑J��
	bool input = GetMyButton0Trigger(0) | GetMyButton1Trigger(0) | GetMyButton2Trigger(0);
	if(ptr->state == NAMEENTRY_WAIT && input)
	{
		SetTransition(SCENE_TITLE);
		ptr->state = NAMEENTRY_STATEMAX;
	}
}

/******************************************************************************
�v���C���[�����͏���
******************************************************************************/
void SelectPlayerName()
{
	NAMEENTRY *ptr = &nameEntry;

	//�I������Ă��镶���̍X�V
	int x = ptr->selectedChar % CHARCHIP_TEXTURE_DIVIDE_X;
	int y = ptr->selectedChar / CHARCHIP_TEXTURE_DIVIDE_X;

	x = WrapAround(0, CHARCHIP_TEXTURE_DIVIDE_X, x + GetMyAxisXRepeat(0));
	y = WrapAround(0, CHARCHIP_TEXTURE_DIVIDE_Y, y + GetMyAxisYRepeat(0));
	ptr->selectedChar = x + y * CHARCHIP_TEXTURE_DIVIDE_X;

	//�J�[�\���ړ�����SE�Đ�
	if (GetMyAxisXRepeat(0) != 0 || GetMyAxisYRepeat(0) != 0)
	{
		PlaySE(SE_CURSOR);
	}

	//����
	if (GetMyButton0Trigger(0))
	{
		PlaySE(SE_DECIDE);
		if (ptr->selectedChar == CHARCHIP_END)
		{
			if (ptr->wordCnt > 0)
			{
				//�l�[���G���g���[�C��
				ptr->state = NAMEENTRY_WAIT;
				CheckUpdateRanking();
			}
			return;
		}

		//�������v���C���[���ɓ���A���̕�����
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX - 1, ptr->wordCnt);
		ptr->nameBuff[ptr->wordCnt] = ptr->selectedChar;
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX, ptr->wordCnt + 1);

		//���������ő�Ȃ�END�֕�������
		if (ptr->wordCnt == PLAYERNAME_MAX)
		{
			ptr->selectedChar = CHARCHIP_END;
		}
		return;
	}

	//�ꕶ���߂�
	if (GetMyButton2Trigger(0))
	{
		ptr->nameBuff[ptr->wordCnt] = CHARCHIP_NONE;
		ptr->wordCnt = Clamp(0, PLAYERNAME_MAX - 1, ptr->wordCnt - 1);
		return;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawNameEntry(void)
{
	DrawNameEntryBG();
	DrawNameEntryHighScore();
	DrawNameEntryCurrentScore();
	DrawNameEntryName();
	DrawNameEntryScoreName();
	DrawNameEntryGuide();
	if (nameEntry.state == NAMEENTRY_INPUT)
	{
		DrawNameEntryChar();
		DrawNameEntryCursor();
	}
}

/******************************************************************************
�����Z�b�g�`�揈��
******************************************************************************/
void DrawNameEntryChar(void)
{
	for (int i = 0; i < CHARCHIP_MAX; i++)
	{
		SetVertexNameEntryChar(i);
		DrawCharChip(vertexWk, i);
	}
}

/******************************************************************************
�w�i�`�揈��
******************************************************************************/
void DrawNameEntryBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���u�����h���s��Ȃ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	pDevice->SetTexture(0, texture[NAMEENTRY_BACK]);

	SetTextureNameEntry();
	SetVertexNameEntryBG();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//���u�����h���Đݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

/******************************************************************************
�J�[�\���`�揈��
******************************************************************************/
void DrawNameEntryCursor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture[NAMEENTRY_CURSOR]);

	SetVertexNameEntryCursor(nameEntry.selectedChar);

	SetTextureNameEntryCursor();

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}

/******************************************************************************
�v���C���[���`�揈��
******************************************************************************/
void DrawNameEntryName(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	NAMEENTRY *ptr = &nameEntry;

	for (int i = 0; i < PLAYERNAME_MAX; i++)
	{
		SetVertexNameEntryName(i);

		//if (i == ptr->wordCnt && ptr->selectedChar != CHARCHIP_END && ptr->wordCnt < PLAYERNAME_MAX - 1)
		if (isDrawSelectedChar(i))
		{
			DrawCharChip(vertexWk, ptr->selectedChar);
		}
		else
		{
			DrawCharChip(vertexWk, ptr->nameBuff[i]);
		}
	}

}

/******************************************************************************
�I�𒆂̕����ƃv���C���[���̂ǂ�����`�悷�邩
******************************************************************************/
bool isDrawSelectedChar(int i)
{
	NAMEENTRY *ptr = &nameEntry;

	if (i != ptr->wordCnt)
	{
		return false;
	}

	if (ptr->selectedChar == CHARCHIP_END)
	{
		return false;
	}

	return true;
}

/******************************************************************************
�n�C�X�R�A�`�揈��
******************************************************************************/
void DrawNameEntryHighScore(void)
{
	int digit, score, num;

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		score = buffHighScore[i];
		digit = (score > 0) ? (int)log10((double)score) + 1 : 1;
		for (int j = 0; j < digit; j++, score /= 10)
		{
			num = score % 10;
			SetVertexNameEntryScore(NAMEENTRY_HIGHSCORE_OFFSET_X * j, NAMEENTRY_HIGHSCORE_OFFSET_Y * i);
			DrawYellowNum(vertexWk, num);
		}
	}
}

/******************************************************************************
�n�C�X�R�A�l�[���`�揈��
******************************************************************************/
void DrawNameEntryScoreName(void)
{
	for (int i = 0; i < HISCORE_MAX; i++)
	{
		for (int j = 0; j < PLAYERNAME_MAX; j++)
		{
			if (buffHighScoreName[i][j] == CHARCHIP_NONE)
			{
				break;
			}

			SetVertexNameEntryScoreName(-NAMEENTRY_HIGHSCORE_OFFSET_X * j, NAMEENTRY_HIGHSCORE_OFFSET_Y * i);
			DrawCharChip(vertexWk, buffHighScoreName[i][j]);
		}
	}
}

/******************************************************************************
����̃X�R�A�`�揈��
******************************************************************************/
void DrawNameEntryCurrentScore(void)
{
	int score, digit, num;
	SCOREDATA *data = GetScoreDataAdr();

	score = data->score;
	digit = (score > 0) ? (int)log10((double)score) + 1 : 1;

	for (int i = 0; i < digit; i++, score /= 10)
	{
		num = score % 10;
		SetVertexNameEntryCurrentScore(NAMEENTRY_HIGHSCORE_OFFSET_X * i);
		DrawYellowNum(vertexWk, num);
	}
}

/******************************************************************************
�K�C�h�`�揈��
******************************************************************************/
void DrawNameEntryGuide(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetVertexNameEntryGuide();
	SetTextureNameEntryGuide();
	pDevice->SetTexture(0, texture[NAMEENTRY_GUIDE]);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexNameEntry(void)
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
void SetTextureNameEntry(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�J�[�\���p)
******************************************************************************/
void SetTextureNameEntryCursor(void)
{
	int pattern = nameEntry.cntFrame % NAMEENTRY_CURSORTEX_ANIMAX;

	// �e�N�X�`�����W�̐ݒ�
	int x = pattern % NAMEENTRY_CURSORTEX_DIVIDE_X;
	int y = pattern / NAMEENTRY_CURSORTEX_DIVIDE_X;
	float sizeX = 1.0f / NAMEENTRY_CURSORTEX_DIVIDE_X;
	float sizeY = 1.0f / NAMEENTRY_CURSORTEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�K�C�h�p)
******************************************************************************/
void SetTextureNameEntryGuide(void)
{
	int pattern = nameEntry.state;

	// �e�N�X�`�����W�̐ݒ�
	int y = pattern;
	float sizeY = 1.0f / NAMEENTRY_GUIDETEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�(���͕����p)
******************************************************************************/
void SetVertexNameEntryChar(int charID)
{
	float posX = NAMEENTRY_CHAR_INITPOS_X + (charID % CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_X;
	float posY = NAMEENTRY_CHAR_INITPOS_Y + (charID / CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_Y;
	NAMEENTRY *ptr = &nameEntry;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = posX - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleChar) * ptr->radiusChar;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleChar) * ptr->radiusChar;
}

/******************************************************************************
���_���W�̐ݒ�i�J�[�\���p�j
******************************************************************************/
void SetVertexNameEntryCursor(int charID)
{
	float posX = NAMEENTRY_CHAR_INITPOS_X + (charID % CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_X;
	float posY = NAMEENTRY_CHAR_INITPOS_Y + (charID / CHARCHIP_TEXTURE_DIVIDE_X) * NAMEENTRY_CHAR_OFFSETPOS_Y;

	NAMEENTRY *ptr = &nameEntry;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = posX - cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleCursor) * ptr->radiusCursor;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleCursor) * ptr->radiusCursor;
}

/******************************************************************************
���_���W�̐ݒ�i�v���C���[���p�j
******************************************************************************/
void SetVertexNameEntryName(int wordCnt)
{
	float posX = NAMEENTRY_NAME_INITPOS_X + wordCnt * NAMEENTRY_NAME_OFFSETPOS;
	float posY = NAMEENTRY_NAME_INITPOS_Y;

	NAMEENTRY *ptr = &nameEntry;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = posX - cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[0].vtx.y = posY - sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[1].vtx.x = posX + cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[1].vtx.y = posY - sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[2].vtx.x = posX - cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[2].vtx.y = posY + sinf(ptr->angleName) * ptr->radiusName;
	vertexWk[3].vtx.x = posX + cosf(ptr->angleName) * ptr->radiusName;
	vertexWk[3].vtx.y = posY + sinf(ptr->angleName) * ptr->radiusName;
}

/******************************************************************************
���_���W�̐ݒ�i�w�i�p�j
******************************************************************************/
void SetVertexNameEntryBG(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
}

/******************************************************************************
���_���W�̐ݒ�i�X�R�A�p�j
******************************************************************************/
void SetVertexNameEntryScore(float offsetX, float offsetY)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_HIGHSCORE_INITPOS_X, NAMEENTRY_HIGHSCORE_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
���_���W�̐ݒ�i�n�C�X�R�A�v���C���[���p�j
******************************************************************************/
void SetVertexNameEntryScoreName(float offsetX, float offsetY)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_SCORENAME_INITPOS_X, NAMEENTRY_SCORENAME_INITPOS_Y, 0.0f);
	pos.x += offsetX;
	pos.y += offsetY;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
���_���W�̐ݒ�i�X�R�A�p�j
******************************************************************************/
void SetVertexNameEntryCurrentScore(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(NAMEENTRY_CURRENTSCORE_INITPOS_X, NAMEENTRY_CURRENTSCORE_INITPOS_Y, 0.0f);
	pos.x += offset;

	NAMEENTRY *ptr = &nameEntry;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angleScore) * ptr->radiusScore;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angleScore) * ptr->radiusScore;
}

/******************************************************************************
���_���W�̐ݒ�i�K�C�h�p�j
******************************************************************************/
void SetVertexNameEntryGuide(void)
{
	vertexWk[0].vtx = D3DXVECTOR3(0.0f, NAMEENTRY_GUIDE_INITPOS_Y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(NAMEENTRY_GUIDETEX_SIZE_X, NAMEENTRY_GUIDE_INITPOS_Y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(0.0f, NAMEENTRY_GUIDE_INITPOS_Y + NAMEENTRY_GUIDETEX_SIZE_Y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(NAMEENTRY_GUIDETEX_SIZE_X, NAMEENTRY_GUIDE_INITPOS_Y + NAMEENTRY_GUIDETEX_SIZE_Y, 0.0f);
}

/******************************************************************************
//�֐���	�FNAMEENTRY *GetNameEntryAdr(int pno)
//����		�Fint pno�F�擾�������l�[���G���g���[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�l�[���G���g���[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�l�[���G���g���[�̃A�h���X�擾�֐�
******************************************************************************/
NAMEENTRY *GetNameEntryAdr(void)
{

	return &nameEntry;

}

/******************************************************************************
//�֐���	�Fvoid CheckUpdateRanking(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F�����L���O�̍X�V
******************************************************************************/
void CheckUpdateRanking(void)
{
	SCOREDATA *data = GetScoreDataAdr();
	
	//�����L���O���X�V
	CheckHiscoreUpdate(&nameEntry.nameBuff[0]);

	//�X�V���������L���O��`��p�z��ɃR�s�[
	ArrayCopy(&buffHighScore[0], &data->HiScore[0], HISCORE_MAX);

	for (int i = 0; i < HISCORE_MAX; i++)
	{
		ArrayCopy(&buffHighScoreName[i][0], &data->playerName[i][0], PLAYERNAME_MAX);
	}
}