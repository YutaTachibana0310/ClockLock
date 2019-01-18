//=============================================================================
//
// �X�R�A�A�C�e������ [scoreItem.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bg.h"
#include "scoreItem.h"
#include "sePlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define SCOREITEM_SCALEUP_FRAME				(30)				//�X�R�A�A�C�e���̃|�b�v�ɂ�����t���[��
#define SCOREITEM_SPEED						(7.0f)				//�X�R�A�A�C�e���̃X�s�[�h
#define SCOREITEM_SPEED_HOMING				(15.0f)				//�X�R�A�A�C�e���̃X�s�[�h�i�ǔ���ԁj
#define SCOREITEM_LENGTH_HOMING				(500*500)			//�X�R�A�A�C�e�����ǔ���ԂɑJ�ڂ��鋗��

#define SCOREITEM_SCALE_MAX					(1.5f)				//�X�P�[���ő�l
#define SCOREITEM_SCALE_MIN					(1.0f)				//�X�P�[���ŏ��l

/*****************************************************************************
�\���̒�`
*****************************************************************************/
enum SCOREITEM_STATE
{
	SCOREITEM_POP,
	SCOREITEM_DOWN,
	SCOREITEM_HOMING
};

enum SCALE_STATE
{
	DEFAULT,
	ENLARGEMENT,
	REDUCTION
};
/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexScoreItem(void);							//���_�쐬�֐�
void SetTextureScoreItem(int cntPattern, SCOREITEM *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexScoreItem(SCOREITEM *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture[SCOREITEM_TEXTURE_NUM];			// �e�N�X�`���ւ̃|�C���^�i���C���p�j

SCOREITEM scoreItem[SCOREITEM_MAX];				//�X�R�A�A�C�e���z��(���C���V���b�g)

static VERTEX_2D vertexWk[NUM_VERTEX];			//���_���i�[���[�N
static int getItemCount;						//�A�C�e���擾��
static int scaleState;
static float scale;


#ifdef _DEBUG
static int				cntScoreItem = 0;						//�A�N�e�B�u�ȃX�R�A�A�C�e���̐�
#endif

/******************************************************************************
����������
******************************************************************************/
HRESULT InitScoreItem(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREITEM* ptr = GetScoreItemAdr(0);
	int i;

	getItemCount = 0;
	scaleState = DEFAULT;
	scale = 1.0f;

	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(SCOREITEM_INITPOS_X, SCOREITEM_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(SCOREITEM_TEXTURE_SIZE_Y, SCOREITEM_TEXTURE_SIZE_Y);
		ptr->baseRadius = ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SCOREITEM_TEXTURE_SIZE_X, SCOREITEM_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->scale = 1.0f;

		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexScoreItem();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[0] = CreateTextureFromFile((LPSTR)SCOREITEMLUMINE_TEXTURE_NAME, pDevice);
		texture[1] = CreateTextureFromFile((LPSTR)SCOREITEM_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitScoreItem(void)
{
	for(int i = 0; i < SCOREITEM_TEXTURE_NUM; i++)
	if (texture[i] != NULL)
	{	// �e�N�X�`���̊J��
		texture[i]->Release();
		texture[i] = NULL;
	}
}

/******************************************************************************
�X�V����
******************************************************************************/
void UpdateScoreItem(void)
{
	SCOREITEM *ptr = GetScoreItemAdr(0);
	PLAYER *player = GetPlayerAdr(0);
	D3DXVECTOR3 tmp;								//�v���C���[�ƃA�C�e���̋��������߂�̂Ɏg�p����x�N�g��
	int i;
	float length = SCOREITEM_LENGTH_HOMING + 1.f;	//�v���C���[�ƃA�C�e���̋���

	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		//��A�N�e�B�u�̃A�C�e���ł���Ώ������s��Ȃ�
		if (!ptr->active)
		{
			continue;
		}

		//�A�C�e���̏�Ԃɂ���ď����𕪊�
		switch (ptr->state)
		{
			//��ʓ��ɃZ�b�g���ꂽ����̏��
		case SCOREITEM_POP:
			ptr->cntFrame++;

			//���X�Ɍ������Ȃ��������փ|�b�v���g��
			ptr->pos += ptr->velocity * SCOREITEM_SPEED * (float)(SCOREITEM_SCALEUP_FRAME - ptr->cntFrame) / SCOREITEM_SCALEUP_FRAME;
			ptr->radius = ptr->baseRadius * ptr->cntFrame / SCOREITEM_SCALEUP_FRAME;

			//�|�b�v���I������玟�̏�Ԃ֑J��
			if (ptr->cntFrame >= SCOREITEM_SCALEUP_FRAME)
			{
				ptr->state = SCOREITEM_DOWN;
				ptr->velocity = VECTOR3_UP * -1;
			}
			break;

			//��ʓ����ォ�牺�ֈړ�������
		case SCOREITEM_DOWN:
			ptr->pos += ptr->velocity * SCOREITEM_SPEED;

			//�v���C���[�Ƃ̋������v�Z���߂�������ǔ���Ԃ֑J��
			if (player->active)
			{
				tmp = player->pos - ptr->pos;
				length = D3DXVec3LengthSq(&tmp);
				if (length < SCOREITEM_LENGTH_HOMING)
				{
					ptr->state = SCOREITEM_HOMING;
				}
			}
			break;

			//�߂��̃v���C���[�̕����֌������Ĉړ�������
		case SCOREITEM_HOMING:
			tmp = player->pos - ptr->pos;
			D3DXVec3Normalize(&ptr->velocity, &tmp);

			ptr->pos += ptr->velocity * SCOREITEM_SPEED_HOMING;

			ptr->scale = Maxf(0.2f, ptr->scale - 0.01f);
			//�v���C���[����A�N�e�B�u�ł����DOWN��Ԃ֑J��
			if(!player->active)
			{
				ptr->state = SCOREITEM_DOWN;
			}
			break;
		}

		//�X�V��̍��W����ʊO�ł���Δ�A�N�e�B�u��
		if (ptr->pos.x + SCOREITEM_TEXTURE_SIZE_X < BG_LEFT || ptr->pos.x - SCOREITEM_TEXTURE_SIZE_X > BG_RIGHT)
		{
			ptr->active = false;
#ifdef _DEBUG
			cntScoreItem--;
#endif
		}
		else if (ptr->pos.y + SCOREITEM_TEXTURE_SIZE_Y < 0 || ptr->pos.y - SCOREITEM_TEXTURE_SIZE_Y > SCREEN_HEIGHT)
		{
			ptr->active = false;
#ifdef _DEBUG
			cntScoreItem--;
#endif
		}

		//�A�j���[�V����
		ptr->countAnim++;
		if (ptr->countAnim % TIMESCOREITEM_ANIMATION == 0)
		{
			ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_SCOREITEM_NUM;
		}
	}

	//�X�P�[���̐ݒ�
	if (scaleState == ENLARGEMENT)
	{
		scale += 0.1f;
		if (scale >= SCOREITEM_SCALE_MAX)
		{
			scaleState = REDUCTION;
		}
	}
	else if (scaleState == REDUCTION)
	{
		scale -= 0.1f;
		if (scale <= SCOREITEM_SCALE_MIN)
		{
			scaleState = DEFAULT;
		}
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawScoreItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREITEM *ptr = GetScoreItemAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, texture[0]);

	//�X�R�A�A�C�e���̌���`��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexScoreItem(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureScoreItem(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
		}
	}
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�X�R�A�A�C�e����`��
	ptr = GetScoreItemAdr(0);
	pDevice->SetTexture(0, texture[1]);
	for (i = 0; i < SCOREITEM_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexScoreItem(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureScoreItem(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(SCOREITEM_BB_LEFT, SCOREITEM_BB_TOP, 0.0f), SCOREITEM_BB_WIDTH, SCOREITEM_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexScoreItem(void)
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
void SetTextureScoreItem(int cntPattern, SCOREITEM *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % SCOREITEM_TEXTURE_DIVIDE_X;
	int y = cntPattern / SCOREITEM_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCOREITEM_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SCOREITEM_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexScoreItem(SCOREITEM *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FSCOREITEM *GetScoreItemAdr(int pno)
//����		�Fint pno�F�擾�������X�R�A�A�C�e���̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�X�R�A�A�C�e���̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�X�R�A�A�C�e���̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
SCOREITEM *GetScoreItemAdr(int pno)
{
	if (pno < SCOREITEM_MAX)
	{
		return &scoreItem[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid SetScoreItem(D3DXVECTOR3 pos, float range, int num)
//����1		�FD3DXVECTOR3 pos	�F�X�R�A�A�C�e�����Z�b�g����������W
//����2		�Ffloat range		�F����W����̃Z�b�g�͈�
//����3		�Fint num			�F�Z�b�g�������A�C�e���̐�
//�߂�l	�Fvoid
//����		�F�X�R�A�A�C�e���̃Z�b�g�֐�
******************************************************************************/
void SetScoreItem(D3DXVECTOR3 pos, float range, int num)
{
	int i, j;
	SCOREITEM *ptr = GetScoreItemAdr(0);

	for (i = 0; i < num; i++, ptr = GetScoreItemAdr(0))
	{
		for (j = 0; j < SCOREITEM_MAX; j++, ptr++)
		{
			//��A�N�e�B�u�ȃA�C�e��������΃Z�b�g
			if (!ptr->active)
			{
				ptr->pos = pos + D3DXVECTOR3(RandomRange(-1 * range, range), RandomRange(-1 * range, range), 0.0f);
				ptr->velocity = VECTOR3_UP;
				ptr->cntFrame = 0;
				ptr->patternAnim = 0;
				ptr->countAnim = 0;
				ptr->state = SCOREITEM_POP;
				ptr->patternAnim = rand() % ANIM_SCOREITEM_NUM;
				ptr->scale = 1.0f;
				ptr->active = true;
#ifdef _DEBUG
				cntScoreItem++;
#endif
				break;
			}
		}
	}
}

/******************************************************************************
//�֐���	�Fint GetScoreItemCount(void)
//����1		�Fvoid
//�߂�l	�Fint		�F�擾�����X�R�A�A�C�e���̐�
//����		�F�擾�����X�R�A�A�C�e���̐��̃Q�b�g�֐�
******************************************************************************/
int GetScoreItemCount(void)
{
	return getItemCount;
}

/******************************************************************************
//�֐���	�Fint SetScoreItemCount(void)
//����1		�Fint value	�F���Z�l
//�߂�l	�Fvoid
//����		�F�擾�����X�R�A�A�C�e���̐���set�֐�
******************************************************************************/
void SetScoreItemCount(int value)
{
	getItemCount += value;

	if (scaleState == DEFAULT)
	{
		scaleState = ENLARGEMENT;
	}
}

/******************************************************************************
//�֐���	�Fint GetScoreItemScaleState(void)
//����1		�Fvoid
//�߂�l	�Fint scaleState	�F���݂̃X�P�[���X�e�[�g
//����		�F�X�R�A�A�C�e���̃X�P�[���X�e�[�g�擾�֐�
******************************************************************************/
float GetScoreItemScale(void)
{
	return scale;
}

#ifdef _DEBUG
/******************************************************************************
//�֐���	�Fint GetScoreItemCount(void)
//����1		�Fvoid
//�߂�l	�Fint		�F�A�N�e�B�u�ȃX�R�A�A�C�e���̐�
//����		�F�X�R�A�A�C�e���̃A�N�e�B�u���擾�֐�
******************************************************************************/
int GetActiveItemCount(void)
{
	return cntScoreItem;
}

/******************************************************************************
//�֐���	�Fvoid AddScoreItemCount(int value)
//����1		�Fint value	�F���Z����l
//�߂�l	�Fvoid
//����		�F�X�R�A�A�C�e���̃A�N�e�B�u������֐�
******************************************************************************/
void SetActiveItemCount(int value)
{
	cntScoreItem += value;
}
#endif
