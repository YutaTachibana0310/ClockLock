//=============================================================================
//
// �X���[���v���y���w�b�_ [SmallPropeller.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SMALLPROPELLER_H_
#define _SMALLPROPELLER_H_


// �}�N����`
#define SMALLPROPELLER_TEXTURE_NAME	_T("data/TEXTURE/Enemy/sPropela.png")			// �X���[���v���y���̃e�N�X�`��
#define SMALLPROPELLER_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/sPropela.png")		//�X���[���v���y���̃e�N�X�`���i�e�p�j

#define SMALLPROPELLER_TEXTURE_NUM		(2)											//�X���[���v���y���̃e�N�X�`���̐�

#define SMALLPROPELLER_TEXTURE_SIZE_X					(56)						// �e�N�X�`���T�C�YX
#define SMALLPROPELLER_TEXTURE_SIZE_Y					(56)						// �e�N�X�`���T�C�YY

#define SMALLPROPELLER_TEXTURE_DIVIDE_X					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SMALLPROPELLER_TEXTURE_DIVIDE_Y					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define SMALLPROPELLER_ANIM_NUM			(SMALLPROPELLER_TEXTURE_DIVIDE_X*SMALLPROPELLER_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define SMALLPROPELLER_TIME_ANIMATION					(2)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define SMALLPROPELLER_MAX								(32)						//�X���[���v���y���̍ő吔

#define SMALLPROPELLER_INITPOS_X						(-100)						//�X���[���v���y���̏���X���W
#define SMALLPROPELLER_INITPOS_Y						(-100)						//�X���[���v���y���̏���Y���W

//�X���[���v���y���̃o�E���f�B���O�{�b�N�X�̒�`
#define SMALLPROPELLER_BB_TOP							(0.0f)
#define SMALLPROPELLER_BB_LEFT							(0.0f)
#define SMALLPROPELLER_BB_WIDTH							(55.0f)
#define SMALLPROPELLER_BB_HIGHT							(55.0f)

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

	D3DXVECTOR3				targetPos;						//�ڕW���W

	int						hp;								//HP
	int						state;							//�s�����

	int						attackNum;						//�U��������
	int						countFrame;						//�t���[���̃J�E���g��

	float					shotPosRadius;					//�o���b�g���ˈʒu�̔��a
	float					shotPosAngle;					//�o���b�g���ˈʒu�̊p�x
}SMALLPROPELLER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSmallPropeller(int num);						//�X���[���v���y������������
void UninitSmallPropeller(void);							//�X���[���v���y���I������
void UpdateSmallPropeller(void);							//�X���[���v���y���X�V����
void DrawSmallPropeller(void);								//�X���[���v���y���`�揈��
SMALLPROPELLER *GetSmallPropellerAdr(int pno);				//�X���[���v���y���A�h���X�擾�֐�
void SetSmallPropeller(D3DXVECTOR3 initPos, D3DXVECTOR3 targetPos); //�X���[���v���y�����o��������֐�

#endif
