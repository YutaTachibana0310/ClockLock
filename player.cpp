//=============================================================================
//
// �v���C���[���� [Player.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "playerBullet.h"
#include "bulletFire.h"
#include "bg.h"
#include "input.h"
#include "enemyBullet.h"
#include "playerGauge.h"
#include "playerExplosion.h"
#include "transition.h"
#include "gameOver.h"
#include "gameClear.h"
#include "playerShield.h"
#include "playerBomber.h"
#include "GUI.h"
#include "sePlayer.h"
#include "playerParticle.h"
#include "playerShield.h"

/******************************************************************************
�}�N����`
*******************************************************************************/
#define PLAYER_SPEED			(6.0f)									//�v���C���[�̈ړ����x					
#define PLAYER_SPEED_SLOW		(3.0f)

#define PLAYER_SHOTPOS_L		(D3DXVECTOR3(-20.0f, -50.0f, 0.0f))		//�v���C���[�o���b�g�̔��ˈʒu�i���j
#define PLAYER_SHOTPOS_R		(D3DXVECTOR3(20.0f, -50.0f, 0.0f))		//�v���C���[�o���b�g�̔��ˈʒu�i�E�j
#define PLAYER_SIDESHOTPOS_L	(D3DXVECTOR3(-40.0f, -20.0f, 0.0f))		//�v���C���[�o���b�g�̉����ˈʒu�i���j
#define PLAYER_SIDESHOTPOS_R	(D3DXVECTOR3(40.0f, -20.0f, 0.0f))		//�v���C���[�o���b�g�̉����ˈʒu�i�E�j

#define PLAYER_FIREPOS_L		(D3DXVECTOR3(-15.0f, -50.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i���j
#define PLAYER_FIREPOS_R		(D3DXVECTOR3(15.0f, -50.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�E�j
#define PLAYER_SIDEFIREPOS_L	(D3DXVECTOR3(-35.0f, -20.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�T�u���j
#define PLAYER_SIDEFIREPOS_R	(D3DXVECTOR3(35.0f, -20.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�T�u�E�j

#define PLAYER_SIZE_X			(PLAYER_TEXTURE_SIZE_X)
#define PLAYER_SIZE_Y			(PLAYER_TEXTURE_SIZE_Y-20)

#define PLAYER_SHOTINTERBAL		(1)										//�v���C���[�o���b�g�̔��ˊԊu
#define PLAYER_SHOTANGLE		(2.5f)									//�v���C���[�o���b�g�̔��ˊp�x�i�T�u�V���b�g�p�j
#define PLAYER_SHOTNUM_MAX		(6)										//shotNum�̍ő�l

#define PLAYER_TIMEEFFECT_DECREASEEASY		(1.0f)						//���ԑ��쒆��1�t���[�����ɃQ�[�W����������l(EASY)
#define PLAYER_TIMEEFFECT_DECREASENORMAL	(1.2f)						//���ԑ��쒆��1�t���[�����ɃQ�[�W����������l(NORMAL)
#define PLAYER_TIMEEFFECT_DECREASEHARD		(1.5f)						//���ԑ��쒆��1�t���[�����ɃQ�[�W����������l(HARD)

#define PLAYER_SHADOW_POS		(D3DXVECTOR3(30.0f, 100.0f, 0.0f))		//�v���C���[�̉e�̍��W
#define PLAYER_SHADOW_SCALE		(0.5f)									//�v���C���[�̉e�̃X�P�[��

#define PLAYER_INITIALIZED_LENGTH	(20.0f)								//INITIALIZED��Ԃňړ����鋗��
#define PLAYER_INITIALIZED_FRAME	(30)								//INITIALIZED��Ԃ���J�ڂ��鎞��

#define PLAYER_INITZANKI			(2)									//�v���C���[�̏����c�@
#define PLAYER_INITZANKI_DEMO		(89)								//�f���p�̏����c�@

#define PLAYER_BOMBER_OFFSETPOS		(D3DXVECTOR3(0.0f, -500.0f, 0.0f))	//�{���o�[�����ʒu
#define PLAYER_BOMBER_INITCNT		(3)									//�����{���o�[��

#define PLAYER_TIMEEFFECT_SEPERIOD	(30)								//���ԑ��쒆�Ɍ��ʉ���炷����

#define PLAYER_PARTICLE_EMITNUM		(100)
#define PLAYER_PARTICLE_MAXSCALE	(5.0f)
#define PLAYER_PARTICLE_DURATION	(15)

#define PLAYER_INVINCIBLE_FRAME		(300)

#define PLAYER_TRAIL_SETFRAME		(10)
/******************************************************************************
�\���̒�`
*******************************************************************************/
enum PLAYER_DIR
{
	PLAYER_LEFT,
	PLAYER_FORWARD,
	PLAYER_RIGHT
};

enum PLAYER_TEXTUREINDEX
{
	PLAYER_BODY,
	PLAYER_COLLIDER,
	PLAYER_TEXMAX
};

/******************************************************************************
�v���g�^�C�v�錾
*******************************************************************************/
void UpdateInitializedPlayer(PLAYER *ptr);					//������Ԃ̃v���C���[�X�V����
void UpdateDefaultPlayer(PLAYER *ptr, int i);				//�ʏ��Ԃ̃v���C���[�X�V����
void UpdateDestroyedPlayer(PLAYER *ptr);					//�j���Ԃ̃v���C���[�X�V����
void UpdateInvinciblePlayer(PLAYER *ptr);					//���G��Ԃ̃v���C���[�X�V����

HRESULT MakeVertexPlayer(void);								//���_�̍쐬
void SetTexturePlayer(int cntPattern, PLAYER *ptr);			//�e�N�X�`���̌v�Z���� 
void SetTexturePlayerCore(PLAYER *ptr);						//�e�N�X�`���̌v�Z�����i�R�A�p�j
void SetVertexPlayer(PLAYER *ptr);							//���_�̌v�Z����
void SetVertexPlayerCore(PLAYER *ptr);						//���_�̌v�Z�����i�R�A�p�j
void MovePlayer(PLAYER* ptr, int i);						//�v���C���[�̈ړ�����
void PlayerAttack(PLAYER *ptr);								//�v���C���[�̍U������
void ProcessTimeEffect(PLAYER *ptr, int i);					//���ԑ��쏈��
void PlayerAnimation(PLAYER *ptr);							//�A�j���[�V��������
void ProcessBulletFire(PLAYER *ptr);						//�o���b�g�t�@�C�A����

void SetPlayerTrail(PLAYER *ptr);							//�g���C���X�V����
void SetVertexPlayerTrail(PLAYER *ptr, int trailIndex);		//�g���C�����_�ݒ菈��

/******************************************************************************
�O���[�o���ϐ�
*******************************************************************************/
static LPDIRECT3DTEXTURE9		texture[PLAYER_TEXMAX];		// �e�N�X�`���ւ̃|�C���^

PLAYER					player[PLAYER_MAX];					// �v���C���[�\����

D3DXVECTOR3				rotPlayer;							// �|���S���̉�]��

float					fRadiusPlayer;						// �|���S���̔��a
float					fBaseAnglePlayer;					// �|���S���̊p�x
int						playerDir;							// �v���C���[�̍��E�̈ړ�����

/******************************************************************************
����������
******************************************************************************/
HRESULT InitPlayer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int i, j;
	PLAYER *ptr = GetPlayerAdr(0);

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(PLAYER_INITPOS_X, PLAYER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(PLAYER_TEXTURE_SIZE_Y / 2.0f, PLAYER_TEXTURE_SIZE_X / 2.0f);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(PLAYER_TEXTURE_SIZE_Y / 2.0f, PLAYER_TEXTURE_SIZE_X / 2.0f));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->zanki = (isDemoMode()) ? PLAYER_INITZANKI_DEMO : PLAYER_INITZANKI;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		ptr->playerDir = PLAYER_FORWARD;

		ptr->angleCore = atan2f(PLAYER_CORE_SIZE_Y, PLAYER_CORE_SIZE_X);
		ptr->radiusCore = D3DXVec2Length(&D3DXVECTOR2(PLAYER_CORE_SIZE_X, PLAYER_CORE_SIZE_Y));

		ptr->timeEffect = PLAYER_TIMEEFFECT_MAX;
		ptr->cntFrame = 0;

		int difficulty = GetDifficulty();
		ptr->gaugeDeltaValue = (difficulty == DIFFICULTY_EASY) ? PLAYER_TIMEEFFECT_DECREASEEASY : (difficulty == DIFFICULTY_NORMAL) ? PLAYER_TIMEEFFECT_DECREASENORMAL : PLAYER_TIMEEFFECT_DECREASEHARD;

		for (j = 0; j < PLAYER_NUM_BULLETFIRE; j++)
		{
			ptr->fire[j] = GetBulletFireAdr(j + i * PLAYER_NUM_BULLETFIRE);
			ptr->fire[j]->active = false;
		}

		ptr->state = PLAYER_INITIALIZED;
		ptr->bomCnt = PLAYER_BOMBER_INITCNT;
		ptr->corePattern = 0;

		//�e�@�\�̃��b�N�i�`���[�g���A���p�j
		if (num == 2)
		{
			ptr->lockBomber = true;
			ptr->lockTimeEffect = true;
		}
		else
		{
			ptr->lockBomber = false;
			ptr->lockTimeEffect = false;
		}

		ptr->active = true;
	}

	// ���_���̍쐬
	MakeVertexPlayer();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture[PLAYER_COLLIDER] = CreateTextureFromFile((LPSTR)PLAYER_CORETEX_NAME, pDevice);
		texture[PLAYER_BODY] = CreateTextureFromFile((LPSTR)PLAYER_TEXTURE, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitPlayer(void)
{
	for (int i = 0; i < PLAYER_TEXMAX; i++)
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
void UpdatePlayer(void)
{
	PLAYER *ptr = GetPlayerAdr(0);

	int i;

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->countAnim++;

		switch (ptr->state)
		{
		case PLAYER_INITIALIZED:
			UpdateInitializedPlayer(ptr);
			break;

		case PLAYER_INVINCIBLE:
			UpdateInvinciblePlayer(ptr);

		case PLAYER_DEFAULT:
			UpdateDefaultPlayer(ptr, i);
			break;

		case PLAYER_DESTROYED:
			UpdateDestroyedPlayer(ptr);
			break;
		}
	}
}

/******************************************************************************
�X�V����(�������)
******************************************************************************/
void UpdateInitializedPlayer(PLAYER *ptr)
{
	float rad = RADIAN((float)ptr->countAnim / PLAYER_INITIALIZED_FRAME * 90);
	ptr->pos.y -= PLAYER_INITIALIZED_LENGTH * cosf(rad);

	if (player->countAnim >= PLAYER_INITIALIZED_FRAME)
	{
		player->countAnim = 0;
		player->state = PLAYER_INVINCIBLE;
	}
}

/******************************************************************************
�X�V�����i���G��ԁj
******************************************************************************/
void UpdateInvinciblePlayer(PLAYER *ptr)
{
	if (ptr->countAnim >= PLAYER_INVINCIBLE_FRAME)
	{
		player->state = PLAYER_DEFAULT;
	}
}

/******************************************************************************
�X�V�����i�ʏ��ԁj
******************************************************************************/
void UpdateDefaultPlayer(PLAYER *ptr, int i)
{
	ptr->cntFrame++;

	//�g���C���X�V����
	SetPlayerTrail(ptr);

	//�ړ�����
	MovePlayer(ptr, i);

	// �A�j���[�V����
	PlayerAnimation(ptr);

	//�o���b�g�t�@�C�A�̈ʒu�X�V
	ProcessBulletFire(ptr);

	//�U������
	ptr->shotInterbal++;
	if (GetMyButton0Down(i) && ptr->shotInterbal > PLAYER_SHOTINTERBAL)
	{
		PlayerAttack(ptr);
		ptr->shotInterbal = 0;
		ptr->numFireShot = (ptr->numFireShot + 1) % PLAYER_SHOTNUM_MAX;
	}

	//�{���o�[���ˏ���
	if (GetMyButton2Trigger(0) && ptr->bomCnt > 0 && !ptr->lockBomber)
	{
		//�{���o�[���˂ɐ���������
		if (SetPlayerBomber(ptr->pos + PLAYER_BOMBER_OFFSETPOS))
		{
			PlaySE(SE_BOMBER);
			ptr->bomCnt--;
			DisableAllEnemyBullet();
			SetGUIShake();
		}
	}

	//���ԑ��쏈��
	ProcessTimeEffect(ptr, i);
	
}

/******************************************************************************
�X�V�����i�j���ԁj
******************************************************************************/
void UpdateDestroyedPlayer(PLAYER *ptr)
{
	if (ptr->countAnim >= 60)
	{
		RespornPlayer(0);
	}
}

/********************************************************************************
�`�揈��
*******************************************************************************/
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *ptr = GetPlayerAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, texture[PLAYER_BODY]);
		SetTexturePlayer(ptr->patternAnim, ptr);

		//�g���C���̕`��
		//if (GetSpeedType() == SLOW_GREEN)
		//{
		//	for (int j = 1; j < PLAYER_TRAILPOS_NUM; j*= 10)
		//	{
		//		SetVertexPlayerTrail(ptr, j);
		//		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));
		//	}
		//}

		// ���_�̌v�Z����
		SetVertexPlayer(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->shadowWk, sizeof(VERTEX_2D));
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));

		//�����蔻��\���p�̕`��
		pDevice->SetTexture(0, texture[PLAYER_COLLIDER]);
		SetVertexPlayerCore(ptr);
		SetTexturePlayerCore(ptr);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, ptr->vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
		//�����蔻��̕`��
		//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYER_BB_LEFT, PLAYER_BB_TOP, 0.0f), PLAYER_BB_WIDTH, PLAYER_BB_HIGHT);
		//DrawBoxCollider(ptr->pos + D3DXVECTOR3(PLAYER_BB_LEFT, PLAYER_BB_TOP, 0.0f), PLAYER_ITEMBB_WIDTH, PLAYER_ITEMBB_HIGHT);
#endif
	}
}

/******************************************************************************
���_�̍쐬
*******************************************************************************/
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *ptr = GetPlayerAdr(0);
	int i;

	for (i = 0; i < PLAYER_MAX; i++, ptr++)
	{
		// ���_���W�̐ݒ�	
		SetVertexPlayer(ptr);

		// rhw�̐ݒ�
		ptr->vertexWk[0].rhw =
			ptr->vertexWk[1].rhw =
			ptr->vertexWk[2].rhw =
			ptr->vertexWk[3].rhw = 1.0f;

		ptr->shadowWk[0].rhw =
			ptr->shadowWk[1].rhw =
			ptr->shadowWk[2].rhw =
			ptr->shadowWk[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		ptr->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		ptr->shadowWk[0].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[1].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[2].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);
		ptr->shadowWk[3].diffuse = D3DCOLOR_RGBA(0, 0, 0, 128);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePlayer(ptr->patternAnim, ptr);

	}

	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTexturePlayer(int cntPattern, PLAYER *ptr)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = ptr->playerDir;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	ptr->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ptr->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ptr->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ptr->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

	ptr->shadowWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	ptr->shadowWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	ptr->shadowWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	ptr->shadowWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�(�R�A�p)
******************************************************************************/
void SetTexturePlayerCore(PLAYER *ptr)
{
	int x = ptr->corePattern;
	int y = 0;

	float sizeX = 1.0f / PLAYER_CORE_DIVIDE_X;
	float sizeY = 1.0f / PLAYER_CORE_DIVIDE_Y;

	ptr->vertexWk[0].tex = D3DXVECTOR2(x * sizeX, 0.0f);
	ptr->vertexWk[1].tex = D3DXVECTOR2(x * sizeX + sizeX, 0.0f);
	ptr->vertexWk[2].tex = D3DXVECTOR2(x * sizeX, 1.0f);
	ptr->vertexWk[3].tex = D3DXVECTOR2(x * sizeX + sizeX, 1.0f);
}

/*******************************************************************************
���_���W�̐ݒ�
*******************************************************************************/
void SetVertexPlayer(PLAYER *ptr)
{
	// ���_���W�̐ݒ�
	ptr->vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	ptr->shadowWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius * PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;
	ptr->shadowWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.x;
	ptr->shadowWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius* PLAYER_SHADOW_SCALE + PLAYER_SHADOW_POS.y;

	//�f�B�t���[�Y�̐ݒ�
	ptr->vertexWk[0].diffuse =
		ptr->vertexWk[1].diffuse =
		ptr->vertexWk[2].diffuse =
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
}

/*******************************************************************************
���_���W�̐ݒ�(�R�A�p)
*******************************************************************************/
void SetVertexPlayerCore(PLAYER *ptr)
{
	D3DXVECTOR3 pos = ptr->pos + D3DXVECTOR3(0.0f, PLAYER_CORE_OFFSET_Y, 0.0f);
	//���_���W�̐ݒ�
	ptr->vertexWk[0].vtx.x = pos.x - cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[0].vtx.y = pos.y - sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[1].vtx.x = pos.x + cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[1].vtx.y = pos.y - sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[2].vtx.x = pos.x - cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[2].vtx.y = pos.y + sinf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[3].vtx.x = pos.x + cosf(ptr->angleCore) * ptr->radiusCore;
	ptr->vertexWk[3].vtx.y = pos.y + sinf(ptr->angleCore) * ptr->radiusCore;
}

/******************************************************************************
//�֐���	�FPLAYER *GetPlayerAdr(int pno)
//����		�Fint pno�F�擾�������v���C���[�̃C���f�b�N�X
//�߂�l	�F�v���C���[�̃A�h���X
//����		�F�v���C���[�̃A�h���X�擾�֐�
******************************************************************************/
PLAYER *GetPlayerAdr(int pno)
{
	if (pno < PLAYER_MAX)
	{
		return &player[pno];
	}
	else
	{
		return NULL;
	}
}

/******************************************************************************
//�֐���	�Fvoid MovePlayer(D3DXVECTOR3 *dir, PLAYER *ptr)
//����1		�FD3DXVECTOR3 *dir�F�ړ������ւ̃x�N�g��
//����2		�FPLAYER *ptr�F�ړ�������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�v���C���[�̈ړ�����
******************************************************************************/
void MovePlayer(PLAYER *ptr, int i)
{

	D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��������i�[����x�N�g��

	dir.y = (float)GetMyAxisY(i);
	dir.x = (float)GetMyAxisX(i);
	D3DXVec3Normalize(&dir, &dir);

	ptr->playerDir = GetMyAxisX(i) + 1;

	//�x�N�g���̊e�����Ɉړ����x�������č��W���X�V
	float speed = (GetMySlowButtonDown(i)) ? PLAYER_SPEED_SLOW : PLAYER_SPEED;
	ptr->pos.x += dir.x * speed;
	ptr->pos.y += dir.y * speed;


	//X�������ɂ͂ݏo�Ă��邩�̔���
	if (ptr->pos.x > SCREEN_END_RIGHT - PLAYER_SIZE_X / 2.0f)
	{
		ptr->pos.x = SCREEN_END_RIGHT - PLAYER_SIZE_X / 2.0f;
	}
	else if (ptr->pos.x < SCREEN_END_LEFT + PLAYER_SIZE_X / 2.0f)
	{
		ptr->pos.x = SCREEN_END_LEFT + PLAYER_SIZE_X / 2.0f;
	}

	//Y�������ɂ͂ݏo�Ă��邩�̔���
	if (ptr->pos.y > SCREEN_HEIGHT - PLAYER_SIZE_Y / 2.0f + 20.0f)
	{
		ptr->pos.y = SCREEN_HEIGHT - PLAYER_SIZE_Y / 2.0f + 20.0f;
	}
	else if (ptr->pos.y < PLAYER_SIZE_Y / 2.0f)
	{
		ptr->pos.y = PLAYER_SIZE_Y / 2.0f;
	}
}

/******************************************************************************
//�֐���	�Fvoid PlayerAttack(PLAYER *ptr)
//����1		�FPLAYER *ptr�F�U������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�v���C���[�̍U������
******************************************************************************/
void PlayerAttack(PLAYER *ptr)
{
	int i;

	//���ʉ��Đ�
	PlaySE(SE_PLAYERSHOT);


	//SHOTPOS����v���C���[�o���b�g�𔭎�
	FirePlayerBullet(ptr->pos + PLAYER_SHOTPOS_R);
	FirePlayerBullet(ptr->pos + PLAYER_SHOTPOS_L);

	SetBulletFire(ptr->fire[0]);
	SetBulletFire(ptr->fire[1]);

	//SIDESHOTPOS����T�u�o���b�g�𔭎�
	for (i = ptr->numFireShot; i < ptr->numFireShot + 3; i++)
	{
		FirePlayerSubBullet(ptr->pos + PLAYER_SIDESHOTPOS_L, 90 - (i + 1) * PLAYER_SHOTANGLE);
		FirePlayerSubBullet(ptr->pos + PLAYER_SIDESHOTPOS_R, 90 + (i + 1) * PLAYER_SHOTANGLE);
	}

	SetBulletFire(ptr->fire[2]);
	SetBulletFire(ptr->fire[3]);
}

/******************************************************************************
//�֐���	�Fvoid ProcessTimeEffect(PLAYER *ptr)
//����1		�FPLAYER *ptr�F��������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F���ԑ��쏈��
******************************************************************************/
void ProcessTimeEffect(PLAYER *ptr, int i)
{
	//���ԑ���@�\�����b�N���ł���΃��^�[��
	if (ptr->lockTimeEffect)
	{
		return;
	}

	//���ʉ��Đ�
	//if (ptr->cntFrame % PLAYER_TIMEEFFECT_SEPERIOD == 0)
	//{
	//	//PlaySE(SE_CLOCK);
	//}

	PLAYERGAUGE *gauge = GetPlayerGaugeAdr(i);

	if (ptr->timeEffect > 0.0f && GetMyButton1Down(i))
	{

		gauge->state = GAUGESTATE_OPEN;
		ptr->timeEffect = Clampf(0.0f, PLAYER_TIMEEFFECT_MAX, ptr->timeEffect - ptr->gaugeDeltaValue);
		SetSpeedType(SLOW_GREEN);
		gauge->percent = ptr->timeEffect;
		return;
	}

	SetSpeedType(DEFAULT_SPEED);
	gauge->state = GAUGESTATE_CLOSE;
}

/******************************************************************************
//�֐���	�Fvoid PlayerAnimation(PLAYER *ptr)
//����1		�FPLAYER *ptr�F��������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�A�j���[�V��������
******************************************************************************/
void PlayerAnimation(PLAYER *ptr)
{
	// �A�j���[�V����Wait�`�F�b�N
	if ((ptr->countAnim % TIME_ANIMATION) == 0)
	{
		// �p�^�[���̐؂�ւ�
		ptr->patternAnim = (ptr->patternAnim + 1) % ANIM_PATTERN_NUM;
		ptr->corePattern = (ptr->corePattern + 1) % PLAYER_CORE_PATTERNMAX;
	}
}

/******************************************************************************
//�֐���	�Fvoid ProcessBulletFire(PLAYER *ptr)
//����1		�FPLAYER *ptr�F��������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�o���b�g�t�@�C�A����
******************************************************************************/
void ProcessBulletFire(PLAYER *ptr)
{
	ptr->fire[0]->pos = ptr->pos + PLAYER_FIREPOS_R;
	ptr->fire[1]->pos = ptr->pos + PLAYER_FIREPOS_L;
	ptr->fire[2]->pos = ptr->pos + PLAYER_SIDEFIREPOS_L;
	ptr->fire[3]->pos = ptr->pos + PLAYER_SIDEFIREPOS_R;
}

/******************************************************************************
//�֐���	�Fvoid DestroyPlayer(PLAYER *ptr)
//����1		�FPLAYER *ptr�F��������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F���ď���
******************************************************************************/
void DestroyPlayer(int i)
{
	PLAYER *player = GetPlayerAdr(i);

	SetPlayerExplosion(player->pos);
	SetEmitterPlayerParticle(player->pos, PLAYER_PARTICLE_DURATION, PLAYER_PARTICLE_EMITNUM, EMITTER_CIRCLE, PLAYER_PARTICLE_MAXSCALE);

	player->state = PLAYER_DESTROYED;
	player->pos = D3DXVECTOR3(PLAYER_INITPOS_X, PLAYER_INITPOS_Y, 0.0f);
	player->countAnim = 0;

	PLAYERGAUGE *gauge = GetPlayerGaugeAdr(0);
	gauge->scale = 0.0f;
	gauge->state = GAUGESTATE_NORMAL;
	SetSpeedType(DEFAULT_SPEED);
}

/******************************************************************************
//�֐���	�Fvoid RespornPlayer(PLAYER *ptr)
//����1		�FPLAYER *ptr�F��������v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F���X�|�[������
******************************************************************************/
void RespornPlayer(int i)
{
	PLAYER *ptr = GetPlayerAdr(i);

	//�c�@���c���Ă���ꍇ
	if (ptr->zanki > 0)
	{
		//�`���[�g���A���V�[���ł͎c�@�����炳�Ȃ�
		if (GetCurrentScene() != SCENE_TUTORIAL)
		{
			ptr->zanki -= 1;
		}

		ptr->countAnim = 0;
		ptr->patternAnim = 0;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		ptr->playerDir = 1;
		ptr->cntFrame = 0;

		ptr->timeEffect = PLAYER_TIMEEFFECT_MAX;

		for (int j = 0; j < PLAYER_NUM_BULLETFIRE; j++)
		{
			ptr->fire[j]->active = false;
		}

		ptr->bomCnt = PLAYER_BOMBER_INITCNT;
		SetPlayerShield();
		ptr->state = PLAYER_INITIALIZED;
	}
	//�c�@�[���̏ꍇ
	else
	{
		SetGameOver();
		ptr->state = PLAYER_GAMEOVER;
	}
}

/******************************************************************
//�֐���	�FSetPlayerTrail(PLAYER *ptr)
//����		�FPLAYER *ptr�F�g���C����ۑ�����v���C���[�ւ̃|�C���^
//�߂�l	�Fvoid
//����		�F�v���C���[�̃g���C�����X�V���鏈��
*******************************************************************/
void SetPlayerTrail(PLAYER *ptr)
{
	//�g���C�����W�z����V�t�g
	VectorArrayShiftDown(&ptr->trailPos[0], 0, PLAYER_TRAILPOS_NUM);

	//���݂̍��W���g���C���z��ɕۑ�
  	ptr->trailPos[0] = ptr->pos;
}

/******************************************************************
//�֐���	�FSetVertexPlayerTrail(PLAYER *ptr, int trailIndex)
//����1		�FPLAYER *ptr�F�g���C����`�悷��v���C���[�ւ̃|�C���^
//����2		�Fint trailIndex�F�`�悷��g���C���̃C���f�b�N�X
//�߂�l	�Fvoid
//����		�F�v���C���[�̃g���C���̒��_��ݒ肷�鏈��
*******************************************************************/
void SetVertexPlayerTrail(PLAYER *ptr, int trailIndex)
{
	//���_���W�̐ݒ�
	ptr->vertexWk[0].vtx.x = ptr->trailPos[trailIndex].x - cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[0].vtx.y = ptr->trailPos[trailIndex].y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.x = ptr->trailPos[trailIndex].x + cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[1].vtx.y = ptr->trailPos[trailIndex].y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.x = ptr->trailPos[trailIndex].x - cosf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[2].vtx.y = ptr->trailPos[trailIndex].y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.x = ptr->trailPos[trailIndex].x + cosf(ptr->angle + ptr->rot.z) * ptr->radius;
	ptr->vertexWk[3].vtx.y = ptr->trailPos[trailIndex].y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;

	//�f�B�t���[�Y�̐ݒ�
	int alpha = (int)(255 * ((float)PLAYER_TRAILPOS_NUM - trailIndex) / PLAYER_TRAILPOS_NUM);
	ptr->vertexWk[0].diffuse =
		ptr->vertexWk[1].diffuse =
		ptr->vertexWk[2].diffuse =
		ptr->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, alpha);
}