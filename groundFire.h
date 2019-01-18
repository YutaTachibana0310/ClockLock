//=============================================================================
//
// �O�����h�t�@�C�A�w�b�_ [GroundFire.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _GROUNDFIRE_H_
#define _GROUNDFIRE_H_


// �}�N����`
#define GROUNDFIRE_TEXTURE_NAME	_T("data/TEXTURE/effect/groundFire.png")	// �O�����h�t�@�C�A�̃e�N�X�`��

#define GROUNDFIRE_TEXTURE_SIZE_X				(36)					// �e�N�X�`���T�C�YX
#define GROUNDFIRE_TEXTURE_SIZE_Y				(36)					// �e�N�X�`���T�C�YY

#define GROUNDFIRE_TEXTURE_DIVIDE_X				(4)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define GROUNDFIRE_TEXTURE_DIVIDE_Y				(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define GROUNDFIRE_ANIM_NUM						(8)						// �A�j���[�V�����p�^�[����
#define GROUNDFIRE_TIME_ANIMATION				(6)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define GROUNDFIRE_MAX							(56)					//�O�����h�t�@�C�A�̍ő吔

#define GROUNDFIRE_INITPOS_X					(-100)					//�O�����h�t�@�C�A�̏���X���W
#define GROUNDFIRE_INITPOS_Y					(-100)					//�O�����h�t�@�C�A�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	int						countAnim;					// �A�j���[�V�����J�E���g
	int						patternAnim;				// �A�j���[�V�����p�^�[���i���o�[

}GROUNDFIRE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGroundFire(int num);							//�O�����h�t�@�C�A����������
void UninitGroundFire(void);								//�O�����h�t�@�C�A�I������
void UpdateGroundFire(void);								//�O�����h�t�@�C�A�X�V����
void DrawGroundFire(void);									//�O�����h�t�@�C�A�`�揈��
GROUNDFIRE *GetGroundFireAdr(int pno);						//�O�����h�t�@�C�A�A�h���X�擾�֐�
void SetGroundFire(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�O�����h�t�@�C�A�Z�b�g�֐�
#endif