//=============================================================================
//
// �A�C�e���L�����A�w�b�_ [ItemCareer.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ITEMCAREER_H_
#define _ITEMCAREER_H_

// �}�N����`
#define ITEMCAREER_TEXTURE_NAME	_T("data/TEXTURE/Enemy/ItemCareer.png")		// �A�C�e���L�����A�̃e�N�X�`��
#define ITEMCAREER_SHADOWTEX_NAME _T("data/TEXTURE/Shadow/ItemCareer.png")	//�e�̃e�N�X�`��

#define ITEMCAREER_TEXTURE_SIZE_X					(84)					// �e�N�X�`���T�C�YX
#define ITEMCAREER_TEXTURE_SIZE_Y					(84)					// �e�N�X�`���T�C�YY

#define ITEMCAREER_TEXTURE_DIVIDE_X					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ITEMCAREER_TEXTURE_DIVIDE_Y					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ITEMCAREER_ANIM_NUM			(ITEMCAREER_TEXTURE_DIVIDE_X*ITEMCAREER_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ITEMCAREER_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define ITEMCAREER_MAX								(4)						//�A�C�e���L�����A�̍ő吔

#define ITEMCAREER_INITPOS_X						(-100)					//�A�C�e���L�����A�̏���X���W
#define ITEMCAREER_INITPOS_Y						(-100)					//�A�C�e���L�����A�̏���Y���W

//�A�C�e���L�����A�̃o�E���f�B���O�{�b�N�X�̒�`
#define ITEMCAREER_BB_TOP							(0.0f)
#define ITEMCAREER_BB_LEFT						(0.0f)
#define ITEMCAREER_BB_WIDTH						(40.0f)
#define ITEMCAREER_BB_HIGHT						(70.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[
	int						state;							// ���

	D3DXVECTOR3				targetPos;						//�ڕW���W
	int						cntFrame;						//�J�E���g�t���[��

	int						hp;
}ITEMCAREER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItemCareer(int num);							//�A�C�e���L�����A����������
void UninitItemCareer(void);								//�A�C�e���L�����A�I������
void UpdateItemCareer(void);								//�A�C�e���L�����A�X�V����
void DrawItemCareer(void);									//�A�C�e���L�����A�`�揈��
ITEMCAREER *GetItemCareerAdr(int pno);						//�A�C�e���L�����A�A�h���X�擾�֐�
void SetItemCareer(D3DXVECTOR3 pos);						//�A�C�e���L�����A�Z�b�g�֐�
#endif
