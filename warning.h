//=============================================================================
//
// ���[�j���O�w�b�_ [Warning.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _WARNING_H_
#define _WARNING_H_


// �}�N����`
#define WARNING_TEXTURE_NAME	_T("data/TEXTURE/GUI/warning.png")		// ���[�j���O�̃e�N�X�`��
#define WARNING_ROLLTEX_NAME	_T("data/TEXTURE/GUI/warningBelt.png")	// ���[�j���O�̑т̃e�N�X�`��
#define WARNING_BG_NAME			_T("data/TEXTURE/GUI/warningBG.png")	// ���[�j���O�̔w�i�e�N�X�`��

#define WARNING_TEXTURE_SIZE_X					(320)					// �e�N�X�`���T�C�YX
#define WARNING_TEXTURE_SIZE_Y					(65)					// �e�N�X�`���T�C�YY
#define WARNING_TEXTURE_DIVIDE_Y				(2)						// �e�N�X�`����Y������

#define WARNING_ROLL_SIZE_X						(320)					//���[�j���O�т̃T�C�YX
#define WARNING_ROLL_SIZE_Y						(15)					//���[�j���O�т̃T�C�YY

#define WARNING_INITPOS_X						(SCREEN_CENTER_X)		//���[�j���O�̏���X���W
#define WARNING_INITPOS_Y						(300)					//���[�j���O�̏���Y���W

#define WARNING_BG_SIZE_X						(390)					//���[�j���O�w�i�̃T�C�YX
#define WARNING_BG_SIZE_Y						(480)					//���[�j���O�w�i�̃T�C�YY
#define WARNING_BG_INITPOS_X					(SCREEN_CENTER_X)		//���[�j���O�w�i�̏������WX
#define WARNING_BG_INITPOS_Y					(SCREEN_CENTER_Y)		//���[�j���O�w�i�̏������WY

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���
	int						countFrame;						// �t���[���J�E���g
	int						state;							// ���
	float					radius;							// �|���S���̔��a
	float					angle;							// �|���S���̊p�x

	float					radiusRoll;						// �у|���S���̔��a
	float					angleRoll;						// �у|���S���̊p�x

	float					radiusBG;						//�w�i�|���S���̔��a
	float					angleBG;						//�w�i�|���S���̊p�x

	float					offsetUV;						// �т̃e�N�X�`���I�t�Z�b�g�l
	float					offsetPos;						// �т̒��_�I�t�Z�b�g�l

	int						countDiffuse;
	bool					flgDiffuse;
}WARNING;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitWarning(int num);							//���[�j���O����������
void UninitWarning(void);								//���[�j���O�I������
void UpdateWarning(void);								//���[�j���O�X�V����
void DrawWarning(void);									//���[�j���O�`�揈��
WARNING *GetWarningAdr(void);							//���[�j���O�A�h���X�擾�֐�
void SetWarning(void);									//���[�j���O�Z�b�g�֐�

#endif
