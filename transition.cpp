//=============================================================================
//
// �g�����W�V�������� [transition.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "transition.h"
#include "sePlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TRANSITION_TAKEFRAME				(10)				//�J�ڂɂ�����t���[����
#define TRANSITION_WAITFRAME				(20)				//�ҋ@�ɂ�����t���[����
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
void UpdateTransitionClose(void);								//�X�V�����i�܂�ꍇ�j
void UpdateTransitionWait(void);								//�X�V�����i������ԁj
void UpdateTransitionOpen(void);								//�X�V�����i�J���ꍇ�j

HRESULT MakeVertexTransition(void);								//���_�쐬�֐�
void SetTextureTransition(int cntPattern);						// �e�N�X�`�����W�̌v�Z����
void SetVertexTopTransition(float offset);						//���_�̌v�Z�����i�㕔�j
void SetVertexBottomTransition(float offset);					//���_�̌v�Z�����i�����j

/*****************************************************************************
�\���̒�`
*****************************************************************************/
//�J�ڎ��ɕ\�������|���S���̒�`
enum TRANSITION_SHUTTER
{
	SHUTTER_TOP,
	SHUTTER_BOTTOM,
	SHUTTER_MAX
};

//�J�ڏ�Ԃ̒�`
enum TRANSITION_STATE
{
	TRANSITION_DEFAULT,
	TRANSITION_CLOSE,
	TRANSITION_WAIT,
	TRANSITION_OPEN,
	TRASITIONSTATE_MAX
};

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				//�e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, radius;								//�|���S���̊p�x�A���a

static TRANSITION transition;						//�g�����W�V�����z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTransition(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	angle = atan2f(TRANSITION_TEXTURE_SIZE_Y, TRANSITION_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TRANSITION_TEXTURE_SIZE_X, TRANSITION_TEXTURE_SIZE_Y));

	transition.cntFrame = 0;
	transition.state = TRANSITION_DEFAULT;

	// ���_���̍쐬
	MakeVertexTransition();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)TRANSITION_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTransition(void)
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
void UpdateTransition(void)
{
	switch (transition.state)
	{
	case TRANSITION_CLOSE:
		UpdateTransitionClose();
		break;

	case TRANSITION_WAIT:
		UpdateTransitionWait();
		break;

	case TRANSITION_OPEN:
		UpdateTransitionOpen();
		break;
	}
}

/******************************************************************************
�X�V����(�܂�ꍇ)
******************************************************************************/
void UpdateTransitionClose(void)
{
	transition.cntFrame++;

	if (transition.cntFrame > TRANSITION_TAKEFRAME)
	{
		transition.cntFrame = 0;
		PlaySE(SE_SHUTTER);
		transition.state = TRANSITION_WAIT;
	}
}

/******************************************************************************
�X�V�����i������ԁj
******************************************************************************/
void UpdateTransitionWait(void)
{
	transition.cntFrame++;

	if (transition.cntFrame > TRANSITION_WAITFRAME)
	{
		transition.cntFrame = TRANSITION_TAKEFRAME;
		SetScene(transition.nextScene);
		transition.state = TRANSITION_OPEN;
	}
}

/******************************************************************************
�X�V�����i�J���ꍇ�j
******************************************************************************/
void UpdateTransitionOpen(void)
{
	transition.cntFrame--;

	if (transition.cntFrame <= 0)
	{
		transition.cntFrame = 0;
		transition.state = TRANSITION_DEFAULT;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTransition(void)
{
	TRANSITION *ptr = &transition;

	if (ptr->state == TRANSITION_DEFAULT)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float offset, rad;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);


	//�g�����W�V������`��
	if (ptr->state == TRANSITION_WAIT)
	{
		SetVertexTopTransition(SCREEN_CENTER_Y);
		SetTextureTransition(SHUTTER_TOP);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexBottomTransition(-SCREEN_CENTER_Y);
		SetTextureTransition(SHUTTER_BOTTOM);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
	else
	{
		rad = RADIAN((float)ptr->cntFrame / TRANSITION_TAKEFRAME * 90);
		offset = SCREEN_CENTER_Y * sinf(rad);

		SetVertexTopTransition(offset);
		SetTextureTransition(SHUTTER_TOP);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

		SetVertexBottomTransition(-offset);
		SetTextureTransition(SHUTTER_BOTTOM);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));;
	}
}

/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTransition(void)
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
void SetTextureTransition(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TRANSITION_TEXTURE_DIVIDE_X;
	int y = cntPattern / TRANSITION_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TRANSITION_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TRANSITION_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexTopTransition(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TRANSITION_INITPOS_X, TRANSITION_TOP_INITPOS_Y, 0.0f);
	pos.y += offset;

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
���_���W�̐ݒ�
******************************************************************************/
void SetVertexBottomTransition(float offset)
{
	D3DXVECTOR3 pos = D3DXVECTOR3(TRANSITION_INITPOS_X, TRANSITION_BOTTOM_INITPOS_Y, 0.0f);
	pos.y += offset;

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
//�֐���	�FTRANSITION *GetTransitionAdr(void)
//����		�Fvoid
//�߂�l	�F�g�����W�V�����\���̂̃A�h���X
//����		�F�g�����W�V�����̃A�h���X�擾�֐�
******************************************************************************/
TRANSITION *GetTransitionAdr(void)
{
	return &transition;
}

/******************************************************************************
//�֐���	�Fvoid SetTransition(int nextScene)
//����		�Fint nextScene		�F�J�ڂ���V�[��
//�߂�l	�Fvoid
//����		�F�J�ڂ̊J�n�֐�
******************************************************************************/
void SetTransition(int nextScene)
{
	transition.nextScene = nextScene;
	transition.cntFrame = 0;
	transition.state = TRANSITION_CLOSE;
}