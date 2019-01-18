//=============================================================================
//
// �e�X�g�G�l�~�[���� [testEnemy.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "enemyBullet.h"
#include "testEnemy.h"
#include "player.h"
#include "myLibrary.h"
#include "effect.h"
#include "particle.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
enum
{
	MOVE_TARGET,
	ATTACK,
	ESCAPE
};

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexTestEnemy(TESTENEMY *ptr);				//���_�쐬�֐�
void SetTextureTestEnemy(int cntPattern, TESTENEMY *ptr);	// �e�N�X�`�����W�̌v�Z����
void SetVertexTestEnemy(TESTENEMY *ptr);					// ���_�̌v�Z����

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9		texture = NULL;						// �e�N�X�`���ւ̃|�C���^�i���C���p�j

TESTENEMY				testEnemy[TESTENEMY_MAX];				//�v���C���[�o���b�g�z��(���C���V���b�g)

static VERTEX_2D		vertexWk[NUM_VERTEX];						//���_���i�[���[�N

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTestEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TESTENEMY* ptr = GetTestEnemyAdr(0);
	int i;

	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(TESTENEMY_INITPOS_X, TESTENEMY_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(TESTENEMY_TEXTURE_SIZE_Y, TESTENEMY_TEXTURE_SIZE_Y);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TESTENEMY_TEXTURE_SIZE_X, TESTENEMY_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->state = 0;
		ptr->active = false;

		// ���_���̍쐬
		MakeVertexTestEnemy(ptr);
	}

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)TESTENEMY_TEXTURE_NAME, pDevice);
	}

	ptr = GetTestEnemyAdr(0);
	ptr->pos = D3DXVECTOR3(-50.0f, -50.0f, 0.0f);
	//ptr->active = true;

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTestEnemy(void)
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
void UpdateTestEnemy(void)
{
	TESTENEMY *ptr = GetTestEnemyAdr(0);
	PLAYER *player = GetPlayerAdr(0);
	ENEMYATTACK param = { ORANGE_CIRCLE, ptr->pos, 5.0f, ENEMYBULLET_DAFAULT_ANGLE };
	int i;

	//�X�V
	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���Ĕ���
			if (ptr->hp <= 0)
			{
				ptr->active = false;
			}

			ptr->rot.z += RotateToTarget(&ptr->pos, &player->pos, ptr->rot.z, RADIAN(10.0f));
			switch (ptr->state)
			{
			case MOVE_TARGET:
				if (MoveToTarget(&ptr->pos, &D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), 10.0f))
				{
					ptr->state = ATTACK;
				}
				break;
			case ATTACK:
				ptr->interbal++;
				if (ptr->interbal % 10 == 0)
				{
					param.angle = ENEMYBULLET_DAFAULT_ANGLE + ptr->rot.z;
					SetEnemyBullet(&param, &ptr->hp);
					ptr->attackNum++;
					if (ptr->attackNum > 10)
					{
						ptr->state = ESCAPE;
						ptr->interbal = 0;
					}
				}
				break;
			case ESCAPE:
				ptr->interbal++;
				StartAccel(&ptr->pos, &VECTOR3_UP, 8.0f, 60, ptr->interbal);
				break;
			}
			//ptr->rot.z += RotateToTarget(&(ptr->pos), &(player->pos), ptr->rot.z, RADIAN(5.0f));
		}
	}
}


/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTestEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TESTENEMY *ptr = GetTestEnemyAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�v���C���[�o���b�g��`��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TESTENEMY_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexTestEnemy(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureTestEnemy(ptr->patternAnim, ptr);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			DrawBoxCollider(ptr->pos + D3DXVECTOR3(TESTENEMY_BB_LEFT, TESTENEMY_BB_TOP, 0.0f), TESTENEMY_BB_WIDTH, TESTENEMY_BB_HIGHT);
#endif
		}
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTestEnemy(TESTENEMY *ptr)
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
void SetTextureTestEnemy(int cntPattern, TESTENEMY *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TESTENEMY_TEXTURE_DIVIDE_X;
	int y = cntPattern / TESTENEMY_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TESTENEMY_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TESTENEMY_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexTestEnemy(TESTENEMY *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}

/******************************************************************************
//�֐���	�FTESTENEMY *GetTestEnemyAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�o���b�g�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�v���C���[�o���b�g�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�v���C���[�o���b�g�̃A�h���X�擾�֐��i���C���V���b�g�j
******************************************************************************/
TESTENEMY *GetTestEnemyAdr(int pno)
{
	if (pno < TESTENEMY_MAX)
	{
		return &testEnemy[pno];
	}
	else
	{
		return NULL;
	}
}