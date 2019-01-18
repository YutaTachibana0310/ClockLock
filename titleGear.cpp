//=============================================================================
//
// �^�C�g���M�A���� [titleGear.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "titleGear.h"

/*****************************************************************************
�}�N����`
*****************************************************************************/
#define TITLEGEAR_SCALE_MAX			(1.3f)			//�^�C�g���M�A�̃X�P�[���ő�l
#define TITLEGEAR_SCALE_MIN			(0.7f)			//�^�C�g���M�A�̃X�P�[���ŏ��l
#define TITLEGEAR_ROTVALUE			(RADIAN(0.1f))	//�|���S���̉�]��

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/
HRESULT MakeVertexTitleGear(void);				//���_�쐬�֐�
void SetTextureTitleGear(void);					// �e�N�X�`�����W�̌v�Z����
void SetVertexTitleGear(TITLEGEAR *ptr);		// ���_�̌v�Z����

/*****************************************************************************
�\���̒�`
*****************************************************************************/

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				//�e�N�X�`���ւ̃|�C���^
static VERTEX_2D vertexWk[NUM_VERTEX];					//���_���i�[���[�N
static float angle, radius;								//�|���S���̔��a�A�p�x

TITLEGEAR titleGear[TITLEGEAR_MAX];						//�^�C�g���M�A�z��

/******************************************************************************
����������
******************************************************************************/
HRESULT InitTitleGear(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEGEAR* ptr = GetTitleGearAdr(0);
	int i;

	angle = atan2f(TITLEGEAR_TEXTURE_SIZE_Y, TITLEGEAR_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TITLEGEAR_TEXTURE_SIZE_X, TITLEGEAR_TEXTURE_SIZE_Y));

	// ���_���̍쐬
	MakeVertexTitleGear();

	if (num == 0)
	{
		// �e�N�X�`���̓ǂݍ���
		texture = CreateTextureFromFile((LPSTR)TITLEGEAR_TEXTURE_NAME, pDevice);

		//�e�p�����[�^�̏�����
		for (i = 0; i < TITLEGEAR_MAX; i++, ptr++)
		{
			ptr->pos = VECTOR3_ZERO;
			ptr->pos.x = RandomRange(0.0f, (float)SCREEN_WIDTH);
			ptr->pos.y = RandomRange(0.0f, (float)SCREEN_HEIGHT);
			ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ptr->scale = RandomRange(TITLEGEAR_SCALE_MIN, TITLEGEAR_SCALE_MAX);
			ptr->countAnim = 0;
			ptr->rotDir = (i % 2 == 0) ? -1 : 1;
		}
	}

	return S_OK;
}

/******************************************************************************
�I������
******************************************************************************/
void UninitTitleGear(void)
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
void UpdateTitleGear(void)
{
	TITLEGEAR *ptr = &titleGear[0];

	for (int i = 0; i < TITLEGEAR_MAX; i++, ptr++)
	{
		ptr->rot.z += ptr->rotDir * TITLEGEAR_ROTVALUE * ptr->scale;
	}
}

/******************************************************************************
�`�揈��
******************************************************************************/
void DrawTitleGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TITLEGEAR *ptr = GetTitleGearAdr(0);
	int i;

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, texture);

	//�^�C�g���M�A��`��
	for (i = 0; i < TITLEGEAR_MAX; i++, ptr++)
	{

		//���_���W��ݒ�
		SetVertexTitleGear(ptr);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
}


/******************************************************************************
���_�̍쐬
******************************************************************************/
HRESULT MakeVertexTitleGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhw�̐ݒ�
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(128, 128, 128, 255);

	SetTextureTitleGear();
	return S_OK;
}

/******************************************************************************
�e�N�X�`�����W�̐ݒ�
******************************************************************************/
void SetTextureTitleGear(void)
{
	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
���_���W�̐ݒ�
******************************************************************************/
void SetVertexTitleGear(TITLEGEAR *ptr)
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx.x = ptr->pos.x - cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(angle - ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(angle + ptr->rot.z) * radius * ptr->scale;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(angle + ptr->rot.z) * radius * ptr->scale;
}

/******************************************************************************
//�֐���	�FTITLEGEAR *GetTitleGearAdr(int pno)
//����		�Fint pno�F�擾�������^�C�g���M�A�̃C���f�b�N�X
//�߂�l	�F����Ȉ����F�^�C�g���M�A�̃A�h���X	�ُ�Ȉ����FNULL
//����		�F�^�C�g���M�A�̃A�h���X�擾�֐�
******************************************************************************/
TITLEGEAR *GetTitleGearAdr(int pno)
{
	if (pno < TITLEGEAR_MAX)
	{
		return &titleGear[pno];
	}
	else
	{
		return NULL;
	}
}