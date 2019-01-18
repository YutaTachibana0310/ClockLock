//=============================================================================
//
// �`���[�g���A���G�l�~�[�w�b�_ [TutorialEnemy.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TUTORIALENEMY_H_
#define _TUTORIALENEMY_H_


// �}�N����`
#define TUTORIALENEMY_TEXTURE_NAME	_T("data/TEXTURE/Enemy/TutorialEnemy.png")	// �`���[�g���A���G�l�~�[�̃e�N�X�`��

#define TUTORIALENEMY_TEXTURE_SIZE_X				(154)					// �e�N�X�`���T�C�YX
#define TUTORIALENEMY_TEXTURE_SIZE_Y				(70)					// �e�N�X�`���T�C�YY

#define TUTORIALENEMY_TEXTURE_DIVIDE_X				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TUTORIALENEMY_TEXTURE_DIVIDE_Y				(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define TUTORIALENEMY_ANIM_NUM			(TUTORIALENEMY_TEXTURE_DIVIDE_X*TUTORIALENEMY_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TUTORIALENEMY_TIME_ANIMATION				(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define TUTORIALENEMY_MAX							(2)						//�`���[�g���A���G�l�~�[�̍ő吔

//�`���[�g���A���G�l�~�[�̃o�E���f�B���O�{�b�N�X�̒�`
#define TUTORIALENEMY_BB_TOP						(-10.0f)
#define TUTORIALENEMY_BB_LEFT						(0.0f)
#define TUTORIALENEMY_BB_WIDTH						(140.0f)
#define TUTORIALENEMY_BB_HIGHT						(20.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	int						countAnim;					// �A�j���[�V�����J�E���g
	int						patternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int						id;

	D3DXVECTOR3				targetPos;					// �ڕW���W
	int						state;						// ���
	int						hp;							// HP

}TUTORIALENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorialEnemy(int num);							//�`���[�g���A���G�l�~�[����������
void UninitTutorialEnemy(void);								//�`���[�g���A���G�l�~�[�I������
void UpdateTutorialEnemy(void);								//�`���[�g���A���G�l�~�[�X�V����
void DrawTutorialEnemy(void);								//�`���[�g���A���G�l�~�[�`�揈��
TUTORIALENEMY *GetTutorialEnemyAdr(int pno);				//�`���[�g���A���G�l�~�[�A�h���X�擾�֐�
void SetTutorialEnemy(void);								//�`���[�g���A���G�l�~�[�Z�b�g�֐�
void DisableTutorialEnemy(void);							//���ׂẴ`���[�g���A���G�l�~�[���A�N�e�B�u�ɂ���֐�
#endif
