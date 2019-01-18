//=============================================================================
//
// �X���[���U�R2�w�b�_ [SmallZako2.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SMALLZAKO2_H_
#define _SMALLZAKO2_H_


// �}�N����`
#define SMALLZAKO2_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sZako.png")			// �X���[���U�R2�̃e�N�X�`��
#define SMALLZAKO2_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/sZako1.png")	// �X���[���U�R2�̃e�N�X�`��(�e)

#define SMALLZAKO2_TEXTURE_SIZE_X					(56)					// �e�N�X�`���T�C�YX
#define SMALLZAKO2_TEXTURE_SIZE_Y					(56)					// �e�N�X�`���T�C�YY

#define SMALLZAKO2_TEXTURE_NUM						(2)						//�X���[���U�R�̃e�N�X�`���̐�

#define SMALLZAKO2_TEXTURE_DIVIDE_X					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SMALLZAKO2_TEXTURE_DIVIDE_Y					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define SMALLZAKO2_ANIM_NUM			(SMALLZAKO2_TEXTURE_DIVIDE_X*SMALLZAKO2_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define SMALLZAKO2_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define SMALLZAKO2_MAX								(56)					//�X���[���U�R2�̍ő吔

#define SMALLZAKO2_INITPOS_X						(-100)					//�X���[���U�R2�̏���X���W
#define SMALLZAKO2_INITPOS_Y						(-100)					//�X���[���U�R2�̏���Y���W

//�X���[���U�R2�̃o�E���f�B���O�{�b�N�X�̒�`
#define SMALLZAKO2_BB_TOP							(0.0f)
#define SMALLZAKO2_BB_LEFT							(0.0f)
#define SMALLZAKO2_BB_WIDTH							(25.0f)
#define SMALLZAKO2_BB_HIGHT							(25.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	int						hp;								//HP
	D3DXVECTOR3				moveDir;						//�i�s����
	float					speed;							//�ړ��X�s�[�h
	int						state;							//�s�����
	int						cntFrame;						//�J�E���g�t���[��
	int						cntAttack;						//�U����
}SMALLZAKO2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSmallZako2(int num);							//�X���[���U�R2����������
void UninitSmallZako2(void);								//�X���[���U�R2�I������
void UpdateSmallZako2(void);								//�X���[���U�R2�X�V����
void DrawSmallZako2(void);									//�X���[���U�R2�`�揈��
SMALLZAKO2 *GetSmallZako2Adr(int pno);						//�X���[���U�R2�A�h���X�擾�֐�
void SetSmallZako2(D3DXVECTOR3 initPos);					//�X���[���U�R2�Z�b�g�֐�

#endif
