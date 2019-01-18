//=============================================================================
//
// �X���[���^���N�w�b�_ [SmallTank.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SMALLTANK_H_
#define _SMALLTANK_H_


// �}�N����`
#define SMALLTANK_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sTank.png")				// �X���[���^���N�̃e�N�X�`��
#define SMALLTANK_TURRETTEX_NAME _T("data/TEXTURE/Enemy/sTankTurret.png")		//�X���[���^���N�̃e�N�X�`���i�C���j

#define SMALLTANK_TEXTURE_SIZE_X					(56)						// �e�N�X�`���T�C�YX
#define SMALLTANK_TEXTURE_SIZE_Y					(56)						// �e�N�X�`���T�C�YY

#define SMALLTANK_TEXTURE_NUM						(2)							//�e�N�X�`���̐�

#define SMALLTANK_TEXTURE_DIVIDE_X					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SMALLTANK_TEXTURE_DIVIDE_Y					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define SMALLTANK_ANIM_NUM			(SMALLTANK_TEXTURE_DIVIDE_X*SMALLTANK_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define SMALLTANK_TIME_ANIMATION					(4)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define SMALLTANK_MAX								(56)						//�X���[���^���N�̍ő吔

#define SMALLTANK_INITPOS_X							(-100)						//�X���[���^���N�̏���X���W
#define SMALLTANK_INITPOS_Y							(-100)						//�X���[���^���N�̏���Y���W

#define SMALLTANK_SETTER_MAX						(10)						//�Z�b�^�[�ő吔
//�X���[���^���N�̃o�E���f�B���O�{�b�N�X�̒�`
#define SMALLTANK_BB_TOP							(0.0f)
#define SMALLTANK_BB_LEFT							(0.0f)
#define SMALLTANK_BB_WIDTH							(40.0f)
#define SMALLTANK_BB_HIGHT							(40.0f)

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

	float					turretRot;						//�C���̉�]��
	int						countFrame;						//�J�E���g�t���[��
	D3DXVECTOR3				moveDir;						//�i�s����
	int						state;							//�s�����
	int						hp;								//HP
	int						attackNum;						//�U����
}SMALLTANK;

typedef struct {
	bool					active;							//�g�por���g�p
	D3DXVECTOR3				pos;							//���W
	D3DXVECTOR3				targetPos;						//�ڕW���W
	int						cntFrame;						//�t���[���J�E���g
	int						cntSet;							//�~�h���^���N���Z�b�g������
}SMALLTANK_SETTER;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSmallTank(int num);								//�X���[���^���N����������
void UninitSmallTank(void);									//�X���[���^���N�I������
void UpdateSmallTank(void);									//�X���[���^���N�X�V����
void DrawSmallTank(void);										//�X���[���^���N�`�揈��
SMALLTANK *GetSmallTankAdr(int pno);							//�X���[���^���N�A�h���X�擾�֐�
void SetSmallTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);	//�X���[���^���N�̃Z�b�g�֐�
void CreateSmallTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target); //�X���[���^���N�̃Z�b�^�[�ݒ�֐�
#endif
