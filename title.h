//=============================================================================
//
// �^�C�g���w�b�_ [Title.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_


// �}�N����`
#define TITLE_TEXTURE_NAME	_T("data/TEXTURE/bg/titlelogo.png")				//�^�C�g���w�i�̃e�N�X�`��
#define TITLE_ANIMTEX0_NAME	_T("data/TEXTURE/bg/title00.png")				//�^�C�g���A�j���[�V�����p�̃e�N�X�`��
#define TITLE_ANIMTEX1_NAME	_T("data/TEXTURE/bg/title01.png")				//�^�C�g���A�j���[�V�����p�̃e�N�X�`��
#define TITLE_ANIMTEX2_NAME _T("data/TEXTURE/bg/title02.png")				//�^�C�g���A�j���[�V�����p�̃e�N�X�`��
#define TITLE_BGTEX_NAME		_T("data/TEXTURE/GUI/titleBG.png")		//�^�C�g���w�i�̃e�N�X�`��
#define TITLE_DIFFICULTY_NAME	_T("data/TEXTURE/GUI/difficultySelect.png")	//��Փx�I���̃e�N�X�`��
#define TITLE_GEARTEX_NAME	_T("data/TEXTURE/bg/gear.png")					//���Ԃ̃e�N�X�`��
#define TITLE_GUITEX_NAME	_T("data/TEXTURE/GUI/titleGUI.png")				//�^�C�g��GUI�̃e�N�X�`��
#define TITLE_COPYRIGHT_NAME	_T("data/TEXTURE/bg/copyright.png")			//�R�s�[���C�g�̃e�N�X�`��

#define TITLE_TEXTURE_SIZE_X				(SCREEN_WIDTH / 2)		// �e�N�X�`���T�C�YX
#define TITLE_TEXTURE_SIZE_Y				(SCREEN_HEIGHT / 2)		// �e�N�X�`���T�C�YY

#define TITLE_INITPOS_X						(SCREEN_CENTER_X)		//�^�C�g���̏���X���W
#define TITLE_INITPOS_Y						(SCREEN_CENTER_Y)		//�^�C�g���̏���Y���W

#define TITLE_LOGO_INITPOS_X				(SCREEN_CENTER_X)		//�^�C�g���w�i�̏������W
#define TITLE_LOGO_INITPOS_Y				(SCREEN_CENTER_Y-150)	//�^�C�g���w�i�̏������W
#define TITLE_LOGO_SIZE_X					(350.0f)				//�^�C�g�����S�̏������W
#define TITLE_LOGO_SIZE_Y					(350.0f)				//�^�C�g�����S�̏������W

#define TITLE_DIFFICULTY_DIVIDE_Y			(4)						//��Փx�I���̃e�N�X�`��������
#define TITLE_DIFFICULTY_INITPOS_X			(SCREEN_CENTER_X)		//��Փx�I���̏������W
#define TITLE_DIFFICULTY_INITPOS_Y			(820)					//��Փx�I���̏������W
#define TITLE_DIFFICULTY_SIZE_X				(300)					//��Փx�I���̃e�N�X�`���T�C�Y
#define TITLE_DIFFICULTY_SIZE_Y				(100)					//��Փx�I���̃e�N�X�`���T�C�Y

#define TITLE_GEARCENTER_SIZE_X				(500)					//�傫�����Ԃ̃e�N�X�`���T�C�Y
#define TITLE_GEARCENTER_SIZE_Y				(500)					//�傫�����Ԃ̃e�N�X�`���T�C�Y

#define TITLE_GUITEX_SIZE_X					(400)					//GUI�e�N�X�`���̃T�C�Y
#define TITLE_GUITEX_SIZE_Y					(200)					//GUI�e�N�X�`���̃T�C�Y
#define TITLE_GUITEX_DIVIDE_Y				(2)						//GUI�e�N�X�`����Y������

#define TITLE_COPYRIGHT_SIZE_X				(400)					//�R�s�[���C�g�̃e�N�X�`���T�C�Y
#define TITLE_COPYRIGHT_SIZE_Y				(100)					//�R�s�[���C�g�̃e�N�X�`���T�C�Y
#define TITLE_COPYRIGHT_INITPOS_X			(880)					//�R�s�[���C�g�̏������W
#define TITLE_COPYRIGHT_INITPOS_Y			(870)					//�R�s�[���C�g�̏������W
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	int						state;
	int						cntFrame;

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	float					radius;						// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

	float					radiusLogo;					//���S�̃|���S�����a
	float					angleLogo;					//���S�̃|���S���p�x

	float					radiusDifficulty;			//��Փx�I���̃|���S�����a
	float					angleDifficulty;			//��Փx�I���̃|���S���p�x
	int						difficulty;					//��Փx

	float					radiusGearSmall;			//���������Ԃ̃|���S�����a
	float					angleGearSmall;				//���������Ԃ̃|���S���p�x

	float					radiusGearBig;				//�傫�����Ԃ̃|���S�����a
	float					angleGearBig;				//�傫�����Ԃ̃|���S���p�x

	float					radiusGUI;					//GUI�̃|���S�����a
	float					angleGUI;					//GUI�̃|���S���p�x
}TITLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(int num);							//�^�C�g������������
void UninitTitle(void);								//�^�C�g���I������
void UpdateTitle(void);								//�^�C�g���X�V����
void DrawTitle(void);								//�^�C�g���`�揈��
TITLE *GetTitleAdr(void);						//�^�C�g���A�h���X�擾�֐�

#endif
