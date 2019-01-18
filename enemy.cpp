//=============================================================================
//
// �v���C���[���� [Enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemy(int i);
void SetTextureEnemy( int cntPattern , int i);	// 
void SetVertexEnemy(int i);					// ���_�̌v�Z����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^

static VERTEX_2D				g_vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

D3DXVECTOR3				g_posEnemy;						// �|���S���̍��W
D3DXVECTOR3				g_rotEnemy;						// �|���S���̉�]��
int						g_nCountEnemyAnim;						// �A�j���[�V�����J�E���g
int						g_nPatternAnim;						// �A�j���[�V�����p�^�[���i���o�[

float					g_fRadiusEnemy;					// �|���S���̔��a
float					g_fBaseAngleEnemy;					// �|���S���̊p�x

int enemyDir;												// �v���C���[�̌���

ENEMY enemy[ENEMY_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	int i;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (i = 0; i < ENEMY_MAX; i++)
	{
		//g_posEnemy = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
		//g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].position = D3DXVECTOR3(i * 50.0f, 100.0f, 0.0f);
		enemy[i].rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//g_nCountEnemyAnim = 0;
		//g_nPatternAnim = 0;
		enemy[i].countAnim = 0;
		enemy[i].patternAnim = i;

		// ���_���̍쐬
		MakeVertexEnemy(i);

	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice,		// �f�o�C�X�̃|�C���^
		TEXTURE_ENEMY,						// �t�@�C���̖��O
		&g_pD3DTexture );					// �ǂݍ��ރ������̃|�C���^

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if( g_pD3DTexture != NULL )
	{	// �e�N�X�`���̊J��
		g_pD3DTexture->Release();

		g_pD3DTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	//
	//					// �A�j���[�V����
	//enem++;

	//// �A�j���[�V����Wait�`�F�b�N
	//if ((g_nCountEnemyAnim % TIME_ANIMATION_ENEMY) == 0)
	//{
	//	// �p�^�[���̐؂�ւ�
	//	g_nPatternAnim = (g_nPatternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

	//	// �e�N�X�`�����W��ݒ�
	//	SetTextureEnemy(g_nPatternAnim);
	//}
	int i;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].countAnim++;

		if (enemy[i].countAnim % TIME_ANIMATION_ENEMY == 0)
		{
			enemy[i].patternAnim = (enemy[i].patternAnim + 1) % ANIM_PATTERN_NUM_ENEMY;

			SetTextureEnemy(enemy[i].patternAnim, i);
		}


		enemy[i].position.x += (float)(rand() % 11 - 5);
		enemy[i].position.y += (float)(rand() % 11 - 5);
		SetVertexEnemy(i);
	}
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTexture);

	// �|���S���̕`��
	int i;
	for (i = 0; i < ENEMY_MAX; i++)
	{
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_ENEMY, enemy[i].vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemy(int i)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�	
	SetVertexEnemy(i);	// ���_�̌v�Z����

	// rhw�̐ݒ�
	//g_vertexWk[0].rhw =
	//g_vertexWk[1].rhw =
	//g_vertexWk[2].rhw =
	//g_vertexWk[3].rhw = 1.0f;

	enemy[i].vertexWk[0].rhw =
	enemy[i].vertexWk[1].rhw =
	enemy[i].vertexWk[2].rhw =
	enemy[i].vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	enemy[i].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	enemy[i].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTextureEnemy(enemy[i].patternAnim, i);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureEnemy(int cntPattern, int i)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_ENEMY;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_ENEMY;

	enemy[i].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	enemy[i].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	enemy[i].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	enemy[i].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEnemy(int i)
{
	// ���_���W�̐ݒ�
	enemy[i].vertexWk[0].vtx = D3DXVECTOR3(enemy[i].position.x, enemy[i].position.y, enemy[i].position.z);
	enemy[i].vertexWk[1].vtx = D3DXVECTOR3(enemy[i].position.x+TEXTURE_ENEMY_SIZE_X, enemy[i].position.y, enemy[i].position.z);
	enemy[i].vertexWk[2].vtx = D3DXVECTOR3(enemy[i].position.x, enemy[i].position.y+TEXTURE_ENEMY_SIZE_Y, enemy[i].position.z);
	enemy[i].vertexWk[3].vtx = D3DXVECTOR3(enemy[i].position.x+TEXTURE_ENEMY_SIZE_X, enemy[i].position.y+TEXTURE_ENEMY_SIZE_Y, enemy[i].position.z);
}


