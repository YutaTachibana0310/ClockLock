//=============================================================================
//
// �`���[�g���A���v���C���[�w�b�_ [TutoPlayer.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _TUTOPLAYER_H_
#define _TUTOPLAYER_H_

#include "bulletFire.h"

// �}�N����`
#define TUTOPLAYER_TEXTURE_NAME	_T("data/TEXTURE/player/playerMainBullet.png")	// �`���[�g���A���v���C���[�̃e�N�X�`��

#define TUTOPLAYER_TEXTURE_SIZE_X					(70 / 2)					// �e�N�X�`���T�C�YX
#define TUTOPLAYER_TEXTURE_SIZE_Y					(80 / 2)					// �e�N�X�`���T�C�YY

#define TUTOPLAYER_TEXTURE_DIVIDE_X				(1)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TUTOPLAYER_TEXTURE_DIVIDE_Y				(1)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define TUTOPLAYER_ANIM_NUM			(TUTOPLAYER_TEXTURE_DIVIDE_X*TUTOPLAYER_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TUTOPLAYER_TIME_ANIMATION					(4)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define TUTOPLAYER_MAX						(56)							//�`���[�g���A���v���C���[�̍ő吔

#define TUTOPLAYER_INITPOS_X						(-100)					//�`���[�g���A���v���C���[�̏���X���W
#define TUTOPLAYER_INITPOS_Y						(-100)					//�`���[�g���A���v���C���[�̏���Y���W

#define TUTOPLAYER_BULLETFIRE_NUM					(4)
//�`���[�g���A���v���C���[�̃o�E���f�B���O�{�b�N�X�̒�`
#define TUTOPLAYER_BB_TOP							(0.0f)
#define TUTOPLAYER_BB_LEFT						(0.0f)
#define TUTOPLAYER_BB_WIDTH						(10.0f)
#define TUTOPLAYER_BB_HIGHT						(35.0f)

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

	int						cntFrame;						//�J�E���g�t���[��
	bool					flgBomb;						//�{���g�p�t���O

	int						shotInterbal;					//�V���b�g�C���^�[�o��
	int						numFireShot;					//�T�u�V���b�g�̔��ˉ�

	BULLETFIRE *fire[TUTOPLAYER_BULLETFIRE_NUM];			//�o���b�g���˂̃G�t�F�N�g


}TUTOPLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutoPlayer(int num);							//�`���[�g���A���v���C���[����������
void UninitTutoPlayer(void);								//�`���[�g���A���v���C���[�I������
void UpdateTutoPlayer(void);								//�`���[�g���A���v���C���[�X�V����
void DrawTutoPlayer(void);									//�`���[�g���A���v���C���[�`�揈��
TUTOPLAYER *GetTutoPlayerAdr(int pno);						//�`���[�g���A���v���C���[�A�h���X�擾�֐�

#endif
