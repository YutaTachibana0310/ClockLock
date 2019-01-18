//=============================================================================
//
// �Q�[���I�[�o�[���� [gameClear.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "gameClear.h"
#include "transition.h"
#include "bgmPlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define GAMECLEAR_DIFFUSE_ALPHA			(3)
#define GAMECLEAR_FADEIN_FRAMEMAX		(85)
#define GAMECLEAR_ANIMATION_FRAMEMAX	(10)
#define GAMECLEAR_TRANSITION_FRAMEMAX	(180)
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexGameClear(void);				//���_�쐬�֐�

void SetTextureGameClear(void);					// �e�N�X�`�����W�̌v�Z����
void SetTextureGameClearText(int i);				

void SetVertexGameClear(void);					// ���_�̌v�Z����
void SetVertexGameClearText(int i, GAMECLEAR *ptr);

void SetDiffuseGameClear(void);					//�f�B�t���[�Y�̐ݒ菈��

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum GAMECLEAR_STATE
{
	GAMECLEAR_FADEIN,
	GAMECLEAR_ANIMATION,
	GAMECLEAR_TRANSITION,
	GAMECLEAR_STATEMAX
};

enum GAMECLEAR_TEXT
{
	GAMECLEAR_STAGE,
	GAMECLEAR_CLEAR,
	GAMECLEAR_TEXTMAX
};

enum GAMECLEAR_TEXTURE
{
	GAMECLEAR_BG,
	GAMECLEAR_TEXT,
	GAMECLEAR_TEXMAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture[GAMECLEAR_TEXMAX];				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

GAMECLEAR gameClear;										//�Q�[���I�[�o�[

/******************************************************************************
����������
******************************************************************************/
HRESULT InitGameClear(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAMECLEAR* ptr = &gameClear;

	ptr->pos = D3DXVECTOR3(GAMECLEAR_INITPOS_X, GAMECLEAR_INITPOS_Y, 0.0f);

	ptr->angle = atan2f(GAMECLEAR_TEXTURE_SIZE_Y, GAMECLEAR_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(GAMECLEAR_TEXTURE_SIZE_X, GAMECLEAR_TEXTURE_SIZE_Y));

	ptr->angleChar = atan2f(GAMECLEAR_CHARTEX_SIZE_Y, GAMECLEAR_CHARTEX_SIZE_X);
	ptr->radiusChar = D3DXVec2Length(&D3DXVECTOR2(GAMECLEAR_CHARTEX_SIZE_X, GAMECLEAR_CHARTEX_SIZE_Y));

	ptr->active = false;
	ptr->state = GAMECLEAR_FADEIN;

	// ���_���̍쐬
	MakeVertexGameClear();
	SetTextureGameClear();
	SetVertexGameClear();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[GAMECLEAR_TEXT] = CreateTextureFromFile((LPSTR)GAMECLEAR_CHARTEX_NAME, pDevice);
		texture[GAMECLEAR_BG] = CreateTextureFromFile((LPSTR)GAMECLEAR_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitGameClear(void)
{
	for (int i = 0; i < GAMECLEAR_TEXMAX; i++)
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
void UpdateGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (!ptr->active)
	{
		return;
	}

	//�t�F�[�h�C�����̍X�V����
	if (ptr->state == GAMECLEAR_FADEIN)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_FADEIN_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_FADEIN_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMECLEAR_ANIMATION;
		}
	}
	//�������A�j���[�V����������X�V����
	else if (ptr->state == GAMECLEAR_ANIMATION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_ANIMATION_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_ANIMATION_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMECLEAR_TRANSITION;
		}

	}
	//�J�ڑҋ@���̍X�V����
	else if (ptr->state == GAMECLEAR_TRANSITION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMECLEAR_TRANSITION_FRAMEMAX);
		if (ptr->cntFrame == GAMECLEAR_TRANSITION_FRAMEMAX)
		{
			FadeoutBGM(10);
			SetTransition(SCENE_NAMEENTRY);
			ptr->state = GAMECLEAR_STATEMAX;
		}
	}


}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (!ptr->active)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture[GAMECLEAR_BG]);

	//���_��ݒ�
	SetDiffuseGameClear();
	SetTextureGameClear();
	SetVertexGameClear();

	//�e�N�X�`���u�����f�B���O��ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	if (ptr->state == GAMECLEAR_FADEIN)
	{
		return;
	}

	//�e�N�X�`���u�����f�B���O���Đݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//�e�L�X�g��`��
	pDevice->SetTexture(0, texture[GAMECLEAR_TEXT]);
	for (int i = 0; i < GAMECLEAR_TEXTMAX; i++)
	{
		SetTextureGameClearText(i);
		SetVertexGameClearText(i, ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexGameClear(void)
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
void SetTextureGameClear(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�e�L�X�g�p)
******************************************************************************/
void SetTextureGameClearText(int pattern)
{
	float sizeY = 1.0f / GAMECLEAR_CHATEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2(0.0f, pattern * sizeY);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, pattern * sizeY);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, pattern * sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, pattern * sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle) * ptr->radius;
}

/******************************************************************************
���_���W�̐ݒ�(�e�L�X�g�p)
******************************************************************************/
void SetVertexGameClearText(int pattern, GAMECLEAR *ptr)
{
	D3DXVECTOR3 pos;

	switch (pattern)
	{
	case GAMECLEAR_STAGE:
		pos = D3DXVECTOR3(GAMECLEAR_STAGETEX_INITPOS_X, GAMECLEAR_STAGETEX_INITPOS_Y, 0.0f);
		break;

	case GAMECLEAR_CLEAR:
		pos = D3DXVECTOR3(GAMECLEAR_CLEARTEX_INITPOS_X, GAMECLEAR_CLEARTEX_INITPOS_Y, 0.0f);
		break;

	default:
		pos = D3DXVECTOR3(GAMECLEAR_CLEARTEX_INITPOS_X, GAMECLEAR_CLEARTEX_INITPOS_Y, 0.0f);
		break;
	}

	if (ptr->state == GAMECLEAR_ANIMATION)
	{
		pos.x += (SCREEN_CENTER_X - pos.x) / GAMECLEAR_ANIMATION_FRAMEMAX * ptr->cntFrame;

		vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	}
	else
	{
		pos.x = SCREEN_CENTER_X;

		vertexWk[0].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[0].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[1].vtx.y = pos.y - sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.x = pos.x - cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[2].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.x = pos.x + cosf(ptr->angleChar) * ptr->radiusChar;
		vertexWk[3].vtx.y = pos.y + sinf(ptr->angleChar) * ptr->radiusChar;
	}
}

/******************************************************************************
�f�B�t���[�Y�̐ݒ�
******************************************************************************/
void SetDiffuseGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	if (ptr->state != GAMECLEAR_FADEIN)
	{
		return;
	}

	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, ptr->cntFrame * GAMECLEAR_DIFFUSE_ALPHA);
}

/******************************************************************************
//�֐���	�FGAMECLEAR *GetGameClearAdr(int pno)
//����		�Fint pno�F�擾�������Q�[���I�[�o�[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�Q�[���I�[�o�[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�Q�[���I�[�o�[�̃A�h���X�擾�֐�
******************************************************************************/
GAMECLEAR *GetGameClearAdr(void)
{

	return &gameClear;
}

/******************************************************************************
//�֐���	�Fvoid SetGameClear(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F�Q�[���I�[�o�[��set�֐�
******************************************************************************/
void SetGameClear(void)
{
	GAMECLEAR *ptr = &gameClear;

	ptr->active = true;
	ptr->state = GAMECLEAR_FADEIN;
	ptr->cntFrame = 0;
	PlayBGM(BGM_CLEAR);
	SetScene(SCENE_CLEAR);
}