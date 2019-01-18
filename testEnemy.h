//=============================================================================
//
// �e�X�g�G�l�~�[�w�b�_ [TestEnemy.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TESTENEMY_H_
#define _TESTENEMY_H_

// �}�N����`
#define TESTENEMY_TEXTURE_NAME	_T("data/TEXTURE/Enemy/wideEnemy.png")	// �e�N�X�`��

#define TESTENEMY_TEXTURE_SIZE_X					(140 / 2)			// �e�N�X�`���T�C�YX
#define TESTENEMY_TEXTURE_SIZE_Y					(120 / 2)			// �e�N�X�`���T�C�YY

#define TESTENEMY_TEXTURE_DIVIDE_X				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TESTENEMY_TEXTURE_DIVIDE_Y				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_TESTENEMY_NUM			(TESTENEMY_TEXTURE_DIVIDE_X*TESTENEMY_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIMETESTENEMY_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define TESTENEMY_MAX						(56)					//�ő吔�i���C���V���b�g�j

#define TESTENEMY_INITPOS_X						(-100)					//����X���W
#define TESTENEMY_INITPOS_Y						(-100)					//����Y���W

//�o�E���f�B���O�{�b�N�X�̒�`
#define TESTENEMY_BB_TOP						(0.0f)
#define TESTENEMY_BB_LEFT						(0.0f)
#define TESTENEMY_BB_WIDTH						(60.0f)
#define TESTENEMY_BB_HIGHT						(50.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							// �|���S���̔��a
	float					angle;							// �|���S���̊p�x

	D3DXVECTOR3				targetPos;
	int state;
	int interbal;
	int attackNum;

	int hp = 100;
}TESTENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTestEnemy(int num);
void UninitTestEnemy(void);
void UpdateTestEnemy(void);
void DrawTestEnemy(void);
TESTENEMY *GetTestEnemyAdr(int pno);

#endif
