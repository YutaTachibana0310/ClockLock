//=============================================================================
//
// �v���C���[�Q�[�W���� [tmplate.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "playerGauge.h"
#include "input.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define PLAYERGAUGE_TEXTURE_CENTER_X	(0.5f)									//�e�N�X�`�����W�̒��S�iX)
#define PLAYERGAUGE_TEXTURE_CENTER_Y	(0.5f)									//�e�N�X�`�����W�̒��S�iY)
#define PLAYERGAUGE_INITANGLE			(90)									//�Q�[�W�̏����p�x
#define PLAYERGAUGE_PERCENT_DEVIDENUM	(720 / (PLAYERGAUGE_VERTEX_NUM - 2))	//�v���C���[�Q�[�W�̃|���S���������߂�̂Ɏg�p����l
#define PLAYERGAUGE_OFFSETPOS			(D3DXVECTOR3(0.0f, -10.0f, 0.0f))		//�v���C���[�̈ʒu����Q�[�W�����炵�ĕ\��������W

#define PLAYERGAUGE_SCALE_MAX			(1.5f)				//�v���C���[�Q�[�W�X�P�[���ő�l
#define PLAYERGAUGE_ADDSCALE			(0.2f)				//�v���C���[�Q�[�W�X�P�[���ω��l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexPlayerGauge(void);							//���_�쐬�֐�
void SetTexturePlayerGauge(PLAYERGAUGE *ptr);					//�e�N�X�`�����W�̌v�Z����
void SetTexturePlayerGaugeBack(PLAYERGAUGE *ptr);				//�e�N�X�`�����W�̌v�Z�����i�o�b�N�p�j
void SetVertexPlayerGauge(PLAYERGAUGE *ptr);					//���_�̌v�Z����
void SetVertexPlayerGaugeBack(PLAYERGAUGE *ptr);				//���_�̌v�Z�����i�o�b�N�p�j

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;							// �e�N�X�`���ւ̃|�C���^�i���C���p�j
static LPDIRECT3DTEXTURE9		texBack = NULL;							// �o�b�N�p�e�N�X�`��

PLAYERGAUGE						playerGauge[PLAYERGAUGE_MAX];			//�v���C���[�Q�[�W�z��

static VERTEX_2D				vertexWk[PLAYERGAUGE_VERTEX_NUM];		//���_���i�[���[�N
static VERTEX_2D				backVertexWk[NUM_VERTEX];				//�o�b�N�p���_���i�[���[�N
static D3DXVECTOR2				vertexOffset[PLAYERGAUGE_VERTEX_NUM];	//�g���_�̒��S����̃I�t�Z�b�g���W					

static int par = 0;

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayerGauge(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERGAUGE* ptr = GetPlayerGaugeAdr(0);
	int i;

	for (i = 0; i < PLAYERGAUGE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYERGAUGE_INITPOS_X, PLAYERGAUGE_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYERGAUGE_TEXTURE_SIZE_Y, PLAYERGAUGE_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYERGAUGE_TEXTURE_SIZE_X, PLAYERGAUGE_TEXTURE_SIZE_Y));

		ptr->percent = PLAYER_TIMEEFFECT_MAX;
		ptr->scale = 0.0f;
		ptr->active = true;

		ptr->state = GAUGESTATE_NORMAL;

		// ���_���̍쐬
		MakeVertexPlayerGauge();
	}

	//���_�̃I�t�Z�b�g���W�̌v�Z
	float angle = 360.0f / (PLAYERGAUGE_VERTEX_NUM - 2);
	vertexOffset[0] = D3DXVECTOR2(0.0f, 0.0f);
	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexOffset[i] = D3DXVECTOR2(cosf(RADIAN(-90 + angle * (i - 1))), sinf(RADIAN(-90 + angle * (i - 1))));
	}

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)PLAYERGAUGE_TEXTURE_NAME, pDevice);
		texBack = CreateTextureFromFile((LPSTR)PLAYERGAUGE_BACKTEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayerGauge(void)
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
void UpdatePlayerGauge(void)
{
	PLAYERGAUGE *ptr = GetPlayerGaugeAdr(0);
	PLAYER *player = GetPlayerAdr(0);

	ptr->pos = player->pos + PLAYERGAUGE_OFFSETPOS;
	ptr->rot.z += 0.005f;

	//�Q�[�W�̏�Ԃ�OPEN or CLOSE��Ԃł����scale��ω�������
	if (ptr->state == GAUGESTATE_OPEN)
	{
		ptr->scale = Clampf(0.0f, PLAYERGAUGE_SCALE_MAX, ptr->scale + PLAYERGAUGE_ADDSCALE);
	}

	else if (ptr->state == GAUGESTATE_CLOSE)
	{
		ptr->scale = Clampf(0.0f, PLAYERGAUGE_SCALE_MAX, ptr->scale - PLAYERGAUGE_ADDSCALE);
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawPlayerGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYERGAUGE *ptr = GetPlayerGaugeAdr(0);
	int i, numPolygon;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�v���C���[�Q�[�W��`��
	for (i = 0; i < PLAYERGAUGE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			// �Q�[�W�w�i�`��
			pDevice->SetTexture(0, texBack);
			SetVertexPlayerGaugeBack(ptr);
			SetTexturePlayerGaugeBack(ptr);
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, backVertexWk, sizeof(VERTEX_2D));

			// �Q�[�W�`��
			pDevice->SetTexture(0, texture);
			SetVertexPlayerGauge(ptr);
			SetTexturePlayerGauge(ptr);
			numPolygon = (int)ptr->percent / PLAYERGAUGE_PERCENT_DEVIDENUM;
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numPolygon, vertexWk, sizeof(VERTEX_2D));
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexPlayerGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i;

	for (i = 0; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		// rhw�̐ݒ�
		vertexWk[i].rhw = 1.0f;
		// ���ˌ��̐ݒ�
		vertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	for (i = 0; i < NUM_VERTEX; i++)
	{
		// rhw�̐ݒ�
		backVertexWk[i].rhw = 1.0f;
		// ���ˌ��̐ݒ�
		backVertexWk[i].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	}

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTexturePlayerGauge(PLAYERGAUGE *ptr)
{
	int i;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(PLAYERGAUGE_TEXTURE_CENTER_X, PLAYERGAUGE_TEXTURE_CENTER_Y);

	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexWk[i].tex.x = 0.5f * (1 + vertexOffset[i].x);
		vertexWk[i].tex.y = 0.5f * (1 + vertexOffset[i].y);
	}
}

void SetTexturePlayerGaugeBack(PLAYERGAUGE *ptr)
{
	//�e�N�X�`�����W�̐ݒ�
	backVertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	backVertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	backVertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	backVertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexPlayerGauge(PLAYERGAUGE *ptr)
{
	int i;

	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x;
	vertexWk[0].vtx.y = ptr->pos.y;
	for (i = 1; i < PLAYERGAUGE_VERTEX_NUM; i++)
	{
		vertexWk[i].vtx.x = ptr->pos.x + vertexOffset[i].x * PLAYERGAUGE_TEXTURE_SIZE_X * ptr->scale;
		vertexWk[i].vtx.y = ptr->pos.y + vertexOffset[i].y * PLAYERGAUGE_TEXTURE_SIZE_Y * ptr->scale;
	}
}

void SetVertexPlayerGaugeBack(PLAYERGAUGE *ptr)
{
	backVertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	backVertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FPLAYERGAUGE *GetPlayerGaugeAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�Q�[�W�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�Q�[�W�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�Q�[�W�̃A�h���X�擾�֐�
******************************************************************************/
PLAYERGAUGE *GetPlayerGaugeAdr(int pno)
{
	if (pno < PLAYERGAUGE_MAX)
	{
		return &playerGauge[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetGaugeState(GAUGESTATE state)
//����1		�FGAUGESTATE state	�F�Z�b�g�������Q�[�W�̏��
//����2		�Fint no			�F�Z�b�g�������Q�[�W�̃C���f�b�N�X
//�߂�l	�Fvoid
//����		�F�v���C���[�Q�[�W�̏�ԃZ�b�g�֐�
******************************************************************************/
void SetGaugeState(GAUGESTATE state, int no)
{
	if (no < PLAYERGAUGE_MAX)
	{
		PLAYERGAUGE *ptr = GetPlayerGaugeAdr(no);

		ptr->state = state;
		if (state == GAUGESTATE_OPEN)
		{
			ptr->active = true;
		}
	}
}