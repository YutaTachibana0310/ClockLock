//=============================================================================
//
// �v���C���[�o���b�g�̔����w�b�_ [PBulletExplosion.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PBULLETEXPLOSION_H_
#define _PBULLETEXPLOSION_H_


// �}�N����`
#define PBULLETEXPLOSION_TEXTURE_NAME	_T("data/TEXTURE/effect/bluefire.png")	// �v���C���[�o���b�g�̔����̃e�N�X�`��

#define PBULLETEXPLOSION_TEXTURE_SIZE_X					(20)					// �e�N�X�`���T�C�YX
#define PBULLETEXPLOSION_TEXTURE_SIZE_Y					(20)					// �e�N�X�`���T�C�YY

#define PBULLETEXPLOSION_TEXTURE_DIVIDE_X				(4)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define PBULLETEXPLOSION_TEXTURE_DIVIDE_Y				(2)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PBULLETEXPLOSION_NUM			(PBULLETEXPLOSION_TEXTURE_DIVIDE_X*PBULLETEXPLOSION_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIMEPBULLETEXPLOSION_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define PBULLETEXPLOSION_MAX						(128)					//�v���C���[�o���b�g�̔����̍ő吔�i���C���V���b�g�j

#define PBULLETEXPLOSION_INITPOS_X						(-100)					//�v���C���[�o���b�g�̔����̏���X���W
#define PBULLETEXPLOSION_INITPOS_Y						(-100)					//�v���C���[�o���b�g�̔����̏���Y���W

//�v���C���[�o���b�g�̔����̃o�E���f�B���O�{�b�N�X�̒�`
#define PBULLETEXPLOSION_BB_TOP							(0.0f)
#define PBULLETEXPLOSION_BB_LEFT						(0.0f)
#define PBULLETEXPLOSION_BB_WIDTH						(10.0f)
#define PBULLETEXPLOSION_BB_HIGHT						(35.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	float					radius;							// �|���S���̔��a
	float					angle;						// �|���S���̊p�x

}PBULLETEXPLOSION;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPBulletExplosion(int num);
void UninitPBulletExplosion(void);
void UpdatePBulletExplosion(void);
void DrawPBulletExplosion(void);
PBULLETEXPLOSION *GetPBulletExplosionAdr(int pno);
void SetPBulletExplosion(D3DXVECTOR3 pos);

#endif
