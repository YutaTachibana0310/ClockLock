//=============================================================================
//
// �~�h���^���N�w�b�_ [MiddleTank.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _MIDDLETANK_H_
#define _MIDDLETANK_H_


// �}�N����`
#define MIDDLETANK_TEXTURE_NAME	_T("data/TEXTURE/Enemy/Tank.png")				// �~�h���^���N�̃e�N�X�`��
#define MIDDLETANK_TURRETTEX_NAME _T("data/TEXTURE/Enemy/tankTurret.png")		//�~�h���^���N�̃e�N�X�`���i�C���j

#define MIDDLETANK_TEXTURE_SIZE_X					(56)						// �e�N�X�`���T�C�YX
#define MIDDLETANK_TEXTURE_SIZE_Y					(56)						// �e�N�X�`���T�C�YY

#define MIDDLETANK_TEXTURE_NUM						(2)							//�e�N�X�`���̐�

#define MIDDLETANK_TEXTURE_DIVIDE_X					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define MIDDLETANK_TEXTURE_DIVIDE_Y					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define MIDDLETANK_ANIM_NUM			(MIDDLETANK_TEXTURE_DIVIDE_X*MIDDLETANK_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define MIDDLETANK_TIME_ANIMATION					(4)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define MIDDLETANK_MAX								(56)						//�~�h���^���N�̍ő吔

#define MIDDLETANK_INITPOS_X						(-100)						//�~�h���^���N�̏���X���W
#define MIDDLETANK_INITPOS_Y						(-100)						//�~�h���^���N�̏���Y���W

#define MIDDLETANK_SETTER_MAX						(10)						//�~�h���^���N�Z�b�^�[�̍ő吔

//�~�h���^���N�̃o�E���f�B���O�{�b�N�X�̒�`
#define MIDDLETANK_BB_TOP							(0.0f)
#define MIDDLETANK_BB_LEFT							(0.0f)
#define MIDDLETANK_BB_WIDTH							(40.0f)
#define MIDDLETANK_BB_HIGHT							(40.0f)

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
	int						shotWay;						//���˂���e��WAY��
}MIDDLETANK;

typedef struct {
	bool					active;							//�g�por���g�p
	D3DXVECTOR3				pos;							//���W
	D3DXVECTOR3				targetPos;						//�ڕW���W
	int						cntFrame;						//�t���[���J�E���g
	int						cntSet;							//�~�h���^���N���Z�b�g������
}MIDDLETANK_SETTER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMiddleTank(int num);								//�~�h���^���N����������
void UninitMiddleTank(void);									//�~�h���^���N�I������
void UpdateMiddleTank(void);									//�~�h���^���N�X�V����
void DrawMiddleTank(void);										//�~�h���^���N�`�揈��
MIDDLETANK *GetMiddleTankAdr(int pno);							//�~�h���^���N�A�h���X�擾�֐�
void SetMiddleTank(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos);	//�~�h���^���N�̃Z�b�g�֐�
void CreateMiddleTankSetter(D3DXVECTOR3 initPos, D3DXVECTOR3 target);	//�~�h���^���N�Z�b�^�[�ݒ�֐�
#endif
