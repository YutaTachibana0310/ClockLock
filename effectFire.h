//=============================================================================
//
// �G�t�F�N�g�t�@�C�A�w�b�_ [effectFire.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _EFFECTFIRE_H_
#define _EFFECTFIRE_H_

#include "effect.h"

// �}�N����`
#define EFFECTFIRE_TEXTURE_NAME		_T("data/TEXTURE/effect/smallexplosion.png")		// �e�N�X�`����

#define EFFECTFIRE_TEXTURE_SIZE_X				(25.0f)
#define EFFECTFIRE_TEXTURE_SIZE_Y				(25.0f)

#define EFFECTFIRE_TEXTURE_DIVIDE_X				(4)								// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECTFIRE_TEXTURE_DIVIDE_Y				(2)								// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define EFFECTFIRE_ANIM_NUM			(EFFECTFIRE_TEXTURE_DIVIDE_X*EFFECTFIRE_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define EFFECTFIRE_ANIM_TIME					(3)								// �A�j���[�V�����̐؂�ւ��J�E���g

#define EFFECTFIRE_INITPOS_X					(-100)							//�������W�iX���W�j
#define EFFECTFIRE_INITPOS_Y					(-100)							//�������W�iY���W�j

#define EFFECTFIRE_MAX							(32*32)							//�e�G�t�F�N�g�t�@�C�A�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffectFire(int num);									//�G�t�F�N�g�t�@�C�A����������
void UninitEffectFire(void);										//�G�t�F�N�g�t�@�C�A�I������
void UpdateEffectFire(void);										//�G�t�F�N�g�t�@�C�A�X�V����
void DrawEffectFire(void);											//�G�t�F�N�g�t�@�C�A�`�揈��
EFFECT *GetEffectFireAdr(int pno);									//�G�t�F�N�g�t�@�C�A�̃A�h���X�擾�֐�
EFFECT *SetEffectFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//�G�t�F�N�g�t�@�C�A�̃Z�b�g�֐�
EFFECT *SetBossFire(D3DXVECTOR3 pos, D3DXVECTOR3 dir, float speed);	//�{�X�t�@�C�A�Z�b�g�֐�
#endif
