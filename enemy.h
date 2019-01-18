//=============================================================================
//
// �G�l�~�[���� [Enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_


// �}�N����`
#define TEXTURE_ENEMY	_T("data/TEXTURE/runningman002.png")	// �T���v���p�摜
#define TEXTURE_ENEMY_SIZE_X	(100)	// �e�N�X�`���T�C�Y
#define TEXTURE_ENEMY_SIZE_Y	(200)	// 

#define TEXTURE_PATTERN_DIVIDE_X_ENEMY	(5)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_ENEMY	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_ENEMY			(TEXTURE_PATTERN_DIVIDE_X_ENEMY*TEXTURE_PATTERN_DIVIDE_Y_ENEMY)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_ENEMY				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g


#define	RADIUS_MIN_ENEMY				(50.0f)							// �|���S���̔��a�ŏ��l
#define	RADIUS_MAX_ENEMY				(300.0f)						// �|���S���̔��a�ő�l
#define	VALUE_ROTATION_ENEMY			(D3DX_PI * 0.01f)				// �|���S���̉�]��
#define	VALUE_SCALE_ENEMY				(2.0f)							// �|���S���̃X�P�[���ω���

#define ENEMY_MAX				(20)
#define NUM_ENEMY				(2)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	D3DXVECTOR3				position;						// �|���S���̍��W
	D3DXVECTOR3				rotation;						// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							// �|���S���̔��a
	float					baseAngle;						// �|���S���̊p�x
	VERTEX_2D		vertexWk[NUM_VERTEX];			// ���_���i�[���[�N


}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);


#endif
