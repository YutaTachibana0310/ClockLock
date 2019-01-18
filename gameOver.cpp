//=============================================================================
//
// �Q�[���I�[�o�[���� [gameOver.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "gameOver.h"
#include "transition.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define GAMEOVER_DIFFUSE_ALPHA			(3)
#define GAMEOVER_FADEIN_FRAMEMAX		(85)
#define GAMEOVER_TRANSITION_FRAMEMAX	(120)
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexGameOver(void);				//���_�쐬�֐�
void SetTextureGameOver(void);	// �e�N�X�`�����W�̌v�Z����
void SetVertexGameOver(void);					// ���_�̌v�Z����
void SetDiffuseGameOver(void);

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum GAMEOVER_STATE
{
	GAMEOVER_FADEIN,
	GAMEOVER_TRANSITION,
	GAMEOVER_STATEMAX
};
/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

GAMEOVER gameOver;										//�Q�[���I�[�o�[

/******************************************************************************
����������
******************************************************************************/
HRESULT InitGameOver(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	GAMEOVER* ptr = &gameOver;

	ptr->pos = D3DXVECTOR3(GAMEOVER_INITPOS_X, GAMEOVER_INITPOS_Y, 0.0f);

	ptr->angle = atan2f(GAMEOVER_TEXTURE_SIZE_Y, GAMEOVER_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(GAMEOVER_TEXTURE_SIZE_X, GAMEOVER_TEXTURE_SIZE_Y));

	ptr->active = false;
	ptr->state = GAMEOVER_FADEIN;

	// ���_���̍쐬
	MakeVertexGameOver();
	SetTextureGameOver();
	SetVertexGameOver();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)GAMEOVER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitGameOver(void)
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
void UpdateGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	if (!ptr->active)
	{
		return;
	}

	if (ptr->state == GAMEOVER_FADEIN)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMEOVER_FADEIN_FRAMEMAX);
		if (ptr->cntFrame == GAMEOVER_FADEIN_FRAMEMAX)
		{
			ptr->cntFrame = 0;
			ptr->state = GAMEOVER_TRANSITION;
		}
	}
	else if (ptr->state == GAMEOVER_TRANSITION)
	{
		ptr->cntFrame = Min(ptr->cntFrame + 1, GAMEOVER_TRANSITION_FRAMEMAX);
		if (ptr->cntFrame == GAMEOVER_TRANSITION_FRAMEMAX)
		{
			SetTransition(SCENE_NAMEENTRY);
			ptr->state = GAMEOVER_STATEMAX;
		}
	}


}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawGameOver(void)
{
	if (!gameOver.active)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//diffuse��ݒ�
	SetDiffuseGameOver();

	//�e�N�X�`���u�����f�B���O��ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//�e�N�X�`���u�����f�B���O���Đݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexGameOver(void)
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
void SetTextureGameOver(void)
{
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

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
�f�B�t���[�Y�̐ݒ�
******************************************************************************/
void SetDiffuseGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	if (ptr->state != GAMEOVER_FADEIN)
	{
		return;
	}

	vertexWk[0].diffuse =
		vertexWk[1].diffuse =
		vertexWk[2].diffuse =
		vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, ptr->cntFrame * GAMEOVER_DIFFUSE_ALPHA);
}

/******************************************************************************
//�֐���	�FGAMEOVER *GetGameOverAdr(int pno)
//����		�Fint pno�F�擾�������Q�[���I�[�o�[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�Q�[���I�[�o�[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�Q�[���I�[�o�[�̃A�h���X�擾�֐�
******************************************************************************/
GAMEOVER *GetGameOverAdr(void)
{

	return &gameOver;
}

/******************************************************************************
//�֐���	�Fvoid SetGameOver(void)
//����		�Fvoid
//�߂�l	�Fvoid
//����		�F�Q�[���I�[�o�[��set�֐�
******************************************************************************/
void SetGameOver(void)
{
	GAMEOVER *ptr = &gameOver;

	ptr->active = true;
	ptr->cntFrame = 0;
	SetScene(SCENE_GAMEOVER);
}