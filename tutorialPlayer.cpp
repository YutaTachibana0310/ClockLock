//=============================================================================
//
// �`���[�g���A���v���C���[���� [tutoPlayer.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "playerBullet.h"
#include "playerShield.h"
#include "tutorialPlayer.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TUTOPLAYER_SPEED			(6.0f)						//�v���C���[�̈ړ����x					

#define TUTOPLAYER_INITIALIZED_FRAME	(90.0f/30)				//INIT��Ԃ���J�ڂ��鎞��
#define TUTOPLAYER_INITIALIZED_LENGTH	(20.0f)					//INIT��Ԃňړ����鋗��

#define TUTOPLAYER_MOVE_FRAME			(240)					//�ړ��`���[�g���A���ɂ����鎞��

#define TUTOPLAYER_SHIELD_FRAME			(240)					//�V�[���h�`���[�g���A���ɂ����鎞��

#define TUTOPLAYER_ENERGY_FRAME			(240)					//�G�l���M�[�`���[�g���A���ɂ����鎞��

#define TUTOPLAYER_BOMB_FRAME			(240)					//�{���`���[�g���A���ɂ����鎞��

#define TUTOPLAYER_SHOT_FRAME			(240)					//�V���b�g�`���[�g���A���ɂ����鎞��

#define TUTOPLAYER_SHOTPOS_L		(D3DXVECTOR3(-20.0f, -50.0f, 0.0f))		//�v���C���[�o���b�g�̔��ˈʒu�i���j
#define TUTOPLAYER_SHOTPOS_R		(D3DXVECTOR3(20.0f, -50.0f, 0.0f))		//�v���C���[�o���b�g�̔��ˈʒu�i�E�j
#define TUTOPLAYER_SIDESHOTPOS_L	(D3DXVECTOR3(-40.0f, -20.0f, 0.0f))		//�v���C���[�o���b�g�̉����ˈʒu�i���j
#define TUTOPLAYER_SIDESHOTPOS_R	(D3DXVECTOR3(40.0f, -20.0f, 0.0f))		//�v���C���[�o���b�g�̉����ˈʒu�i�E�j

#define TUTOPLAYER_FIREPOS_L		(D3DXVECTOR3(-15.0f, -50.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i���j
#define TUTOPLAYER_FIREPOS_R		(D3DXVECTOR3(15.0f, -50.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�E�j
#define TUTOPLAYER_SIDEFIREPOS_L	(D3DXVECTOR3(-35.0f, -20.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�T�u���j
#define TUTOPLAYER_SIDEFIREPOS_R	(D3DXVECTOR3(35.0f, -20.0f, 0.0f))		//�o���b�g�t�@�C�A�̃Z�b�g�ʒu�i�T�u�E�j

#define TUTOPLAYER_SHOTINTERBAL		(1)										//�v���C���[�o���b�g�̔��ˊԊu
#define TUTOPLAYER_SHOTANGLE		(2.5f)									//�v���C���[�o���b�g�̔��ˊp�x�i�T�u�V���b�g�p�j
#define TUTOPLAYER_SHOTNUM_MAX		(6)										//shotNum�̍ő�l

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexTutoPlayer(void);							//���_�쐬�֐�
void SetTextureTutoPlayer(int cntPattern);					// �e�N�X�`�����W�̌v�Z����
void SetVertexTutoPlayer(TUTOPLAYER *ptr);					// ���_�̌v�Z����

//�e��X�V����
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerMove(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr);

//�ړ�����
void MoveTutoPlayer(TUTOPLAYER *ptr);

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// �e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N

TUTOPLAYER tutoPlayer;				//�`���[�g���A���v���C���[�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTutoPlayer(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTOPLAYER* ptr = GetTutoPlayerAdr(0);
	int i;

	for (i = 0; i < TUTOPLAYER_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(TUTOPLAYER_INITPOS_X, TUTOPLAYER_INITPOS_Y, 0.0f);
		ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		ptr->angle = atan2f(TUTOPLAYER_TEXTURE_SIZE_Y, TUTOPLAYER_TEXTURE_SIZE_X);
		ptr->radius = D3DXVec2Length(&D3DXVECTOR2(TUTOPLAYER_TEXTURE_SIZE_X, TUTOPLAYER_TEXTURE_SIZE_Y));

		ptr->countAnim = 0;
		ptr->patternAnim = 0;
		ptr->cntFrame = 0;

		ptr->shotInterbal = 0;
		ptr->numFireShot = 0;
		
		ptr->flgBomb = false;
		ptr->active = false;
	}

	// ���_���̍쐬
	MakeVertexTutoPlayer();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)TUTOPLAYER_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTutoPlayer(void)
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
void UpdateTutoPlayer(void)
{

}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTutoPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TUTOPLAYER *ptr = GetTutoPlayerAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�`���[�g���A���v���C���[�����Z�����ŕ`��
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	for (i = 0; i < TUTOPLAYER_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			//���_���W��ݒ�
			SetVertexTutoPlayer(ptr);

			// �e�N�X�`�����W��ݒ�
			SetTextureTutoPlayer(ptr->patternAnim);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
#ifdef _DEBUG
			//�����蔻��̕`��
			//DrawBoxCollider(ptr->pos + D3DXVECTOR3(TUTOPLAYER_BB_LEFT, TUTOPLAYER_BB_TOP, 0.0f), TUTOPLAYER_BB_WIDTH, TUTOPLAYER_BB_HIGHT);
#endif
		}
	}
	//�`����@��ʏ�̕��@�ɍĐݒ�
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTutoPlayer(void)
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
void SetTextureTutoPlayer(int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TUTOPLAYER_TEXTURE_DIVIDE_X;
	int y = cntPattern / TUTOPLAYER_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / TUTOPLAYER_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / TUTOPLAYER_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexTutoPlayer(TUTOPLAYER *ptr)
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
//�֐���	�FTUTOPLAYER *GetTutoPlayerAdr(int pno)
//����		�Fint pno�F�擾�������`���[�g���A���v���C���[�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�`���[�g���A���v���C���[�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�`���[�g���A���v���C���[�̃A�h���X�擾�֐�
******************************************************************************/
TUTOPLAYER *GetTutoPlayerAdr(int pno)
{
	return &tutoPlayer;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iINIT��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerInit(TUTOPLAYER *ptr)
{
	float rad = RADIAN((float)ptr->countAnim * TUTOPLAYER_INITIALIZED_FRAME);
	ptr->pos.y -= TUTOPLAYER_INITIALIZED_LENGTH * cosf(rad);

	if (ptr->cntFrame >= TUTOPLAYER_INITIALIZED_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iMOVE��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerMove(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_MOVE_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}
	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iSHOT��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerShot(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_MOVE_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iATTACK��ԁj
******************************************************************************/
bool TutoPlayerAttack(TUTOPLAYER *ptr)
{
	int i;

	MoveTutoPlayer(ptr);

	//SHOTPOS����v���C���[�o���b�g�𔭎�
	FirePlayerBullet(ptr->pos + TUTOPLAYER_SHOTPOS_R);
	FirePlayerBullet(ptr->pos + TUTOPLAYER_SHOTPOS_L);

	SetBulletFire(ptr->fire[0]);
	SetBulletFire(ptr->fire[1]);

	if (!GetMyButton1Down(0))
	{
		//SIDESHOTPOS����T�u�o���b�g�𔭎�
		for (i = ptr->numFireShot; i < ptr->numFireShot + 3; i++)
		{
			FirePlayerSubBullet(ptr->pos + TUTOPLAYER_SIDESHOTPOS_L, 90 - (i + 1) * TUTOPLAYER_SHOTANGLE);
			FirePlayerSubBullet(ptr->pos + TUTOPLAYER_SIDESHOTPOS_R, 90 + (i + 1) * TUTOPLAYER_SHOTANGLE);
		}

		SetBulletFire(ptr->fire[2]);
		SetBulletFire(ptr->fire[3]);
	}

	ptr->cntFrame++;
	if (ptr->cntFrame == TUTOPLAYER_SHOT_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�ړ�����
******************************************************************************/
void MoveTutoPlayer(TUTOPLAYER *ptr)
{
	int x = GetMyAxisX(0);
	int y = GetMyAxisY(0);

	D3DXVECTOR3 moveDir = D3DXVECTOR3((float)x, (float)y, 0.0f);
	D3DXVec3Normalize(&moveDir, &moveDir);

	ptr->pos += moveDir * TUTOPLAYER_SPEED;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iSHIELD��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerShield(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	PLAYERSHIELD *shield = GetPlayerShieldAdr();

	if (shield->active)
	{
		ptr->cntFrame++;
	}

	if (ptr->cntFrame == TUTOPLAYER_SHIELD_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iENERGY��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerEnergy(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	ptr->cntFrame++;

	if (ptr->cntFrame == TUTOPLAYER_ENERGY_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}

/******************************************************************************
�`���[�g���A���v���C���[�X�V�����iBOMBER��ԁj
******************************************************************************/
bool OnUpdateTutoPlayerBomber(TUTOPLAYER *ptr)
{
	MoveTutoPlayer(ptr);

	if (GetMyButton0Down(0))
	{
		ptr->flgBomb = true;
	}

	if (ptr->flgBomb)

	{
		ptr->cntFrame++;
	}

	if (ptr->cntFrame == TUTOPLAYER_BOMB_FRAME)
	{
		ptr->cntFrame = 0;
		return true;
	}

	return false;
}