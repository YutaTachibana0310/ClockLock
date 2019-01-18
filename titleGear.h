//=============================================================================
//
// �^�C�g���M�A�w�b�_ [TitleGear.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TITLEGEAR_H_
#define _TITLEGEAR_H_


// �}�N����`
#define TITLEGEAR_TEXTURE_NAME	_T("data/TEXTURE/bg/gear.png")	// �^�C�g���M�A�̃e�N�X�`��

#define TITLEGEAR_TEXTURE_SIZE_X				(100)					// �e�N�X�`���T�C�YX
#define TITLEGEAR_TEXTURE_SIZE_Y				(100)					// �e�N�X�`���T�C�YY
#define TITLEGEAR_MAX							(64)					//�^�C�g���M�A�̍ő吔

#define TITLEGEAR_INITPOS_X						(-100)					//�^�C�g���M�A�̏���X���W
#define TITLEGEAR_INITPOS_Y						(-100)					//�^�C�g���M�A�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							// �g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	float					scale;							// �|���S���̃X�P�[��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						rotDir;							// ���Ԃ̉�]����
}TITLEGEAR;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitleGear(int num);							//�^�C�g���M�A����������
void UninitTitleGear(void);								//�^�C�g���M�A�I������
void UpdateTitleGear(void);								//�^�C�g���M�A�X�V����
void DrawTitleGear(void);								//�^�C�g���M�A�`�揈��
TITLEGEAR *GetTitleGearAdr(int pno);					//�^�C�g���M�A�A�h���X�擾�֐�

#endif
