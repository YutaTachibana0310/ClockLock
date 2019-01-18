//=============================================================================
//
// �X�e�[�W1�{�X�w�b�_ [Stage1Boss.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _STAGE1BOSS_H_
#define _STAGE1BOSS_H_


// �}�N����`
#define STAGE1BOSS_TEXTURE_NAME	_T("data/TEXTURE/Enemy/boss1.png")			// �X�e�[�W1�{�X�̃e�N�X�`��
#define STAGE1BOSS_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/boss1Shadow.png")

#define STAGE1BOSS_TEXTURE_SIZE_X					(336)					// �e�N�X�`���T�C�YX
#define STAGE1BOSS_TEXTURE_SIZE_Y					(196)					// �e�N�X�`���T�C�YY

#define STAGE1BOSS_TEXTURE_DIVIDE_X					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define STAGE1BOSS_TEXTURE_DIVIDE_Y					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define STAGE1BOSS_ANIM_NUM							(2)						// �A�j���[�V�����p�^�[����
#define STAGE1BOSS_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define STAGE1BOSS_INITPOS_X						(-100)					//�X�e�[�W1�{�X�̏���X���W
#define STAGE1BOSS_INITPOS_Y						(-100)					//�X�e�[�W1�{�X�̏���Y���W

//�X�e�[�W1�{�X�̃o�E���f�B���O�{�b�N�X�̒�`
#define STAGE1BOSS_BB_TOP						(-30.0f)
#define STAGE1BOSS_BB_LEFT						(0.0f)
#define STAGE1BOSS_BB_WIDTH						(330.0f)
#define STAGE1BOSS_BB_HIGHT						(70.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	int						cntFrame;						//�J�E���g�t���[��

	float					radius;							// �|���S���̔��a
	float					angle;							// �|���S���̊p�x

	int						form;							//�`��
	int						state;							//���
	int						nextState;						//���ɑJ�ڂ�����

	int						hp;								//HP
	D3DXVECTOR3				targetPos;						//�ڕW���W
	int						cntMove;						//���[�u�J�E���g
	int						atkNum;							//�U����

	BOSSGAUGE				*gauge;							//�{�X�Q�[�W
}STAGE1BOSS;

enum STAGE1BOSS_STATE
{
	STAGE1BOSS_INIT,		//�������
	STAGE1BOSS_INTERBAL,	//�U����̃C���^�[�o��
	STAGE1BOSS_WAY1,		//WAY1�U��
	STAGE1BOSS_WIP1,		//WIP�U��
	STAGE1BOSS_WIP2,
	STAGE1BOSS_CIRCLE,		//CIRCLE�U��
	STAGE1BOSS_DESTROYED,	//���ď��
	STAGE1BOSS_STATEMAX		
};

enum STAGE1BOSS_FORM
{
	STAGE1BOSS_FIRST,		//��1�`��
	STAGE1BOSS_SECOND,		//��2�`��
	STAGE1BOSS_FORMMAX
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStage1Boss(int num);							//�X�e�[�W1�{�X����������
void UninitStage1Boss(void);								//�X�e�[�W1�{�X�I������
void UpdateStage1Boss(void);								//�X�e�[�W1�{�X�X�V����
void DrawStage1Boss(void);									//�X�e�[�W1�{�X�`�揈��
STAGE1BOSS *GetStage1BossAdr(void);							//�X�e�[�W1�{�X�A�h���X�擾�֐�
void SetStage1Boss(D3DXVECTOR3 pos);						//�X�e�[�W1�{�X�̃Z�b�g�֐�

#endif
