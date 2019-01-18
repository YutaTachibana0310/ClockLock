//=============================================================================
//
// �X���[���U�R1�w�b�_ [SmallZako1.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SMALLZAKO1_H_
#define _SMALLZAKO1_H_


// �}�N����`
#define SMALLZAKO1_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sZako.png")			// �X���[���U�R1�̃e�N�X�`��
#define SMALLZAKO1_SHADOWTEX_NAME	_T("data/TEXTURE/Shadow/sZako1.png")	// �X���[���U�R1�̃e�N�X�`��(�e)

#define SMALLZAKO1_TEXTURE_SIZE_X					(56)					// �e�N�X�`���T�C�YX
#define SMALLZAKO1_TEXTURE_SIZE_Y					(56)					// �e�N�X�`���T�C�YY

#define SMALLZAKO1_TEXTURE_NUM						(2)						//�X���[���U�R�̃e�N�X�`���̐�

#define SMALLZAKO1_TEXTURE_DIVIDE_X					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SMALLZAKO1_TEXTURE_DIVIDE_Y					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define SMALLZAKO1_ANIM_NUM			(SMALLZAKO1_TEXTURE_DIVIDE_X*SMALLZAKO1_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define SMALLZAKO1_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define SMALLZAKO1_MAX								(56)					//�X���[���U�R1�̍ő吔

#define SMALLZAKO1_INITPOS_X						(-100)					//�X���[���U�R1�̏���X���W
#define SMALLZAKO1_INITPOS_Y						(-100)					//�X���[���U�R1�̏���Y���W

//�X���[���U�R1�̃o�E���f�B���O�{�b�N�X�̒�`
#define SMALLZAKO1_BB_TOP							(0.0f)
#define SMALLZAKO1_BB_LEFT							(0.0f)
#define SMALLZAKO1_BB_WIDTH							(25.0f)
#define SMALLZAKO1_BB_HIGHT							(25.0f)

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
}SMALLZAKO1;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSmallZako1(int num);							//�X���[���U�R1����������
void UninitSmallZako1(void);								//�X���[���U�R1�I������
void UpdateSmallZako1(void);								//�X���[���U�R1�X�V����
void DrawSmallZako1(void);									//�X���[���U�R1�`�揈��
SMALLZAKO1 *GetSmallZako1Adr(int pno);						//�X���[���U�R1�A�h���X�擾�֐�
void SetSmallZako1(D3DXVECTOR3 initPos);					//�X���[���U�R1�Z�b�g�֐�

#endif
