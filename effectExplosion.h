//=============================================================================
//
// �G�t�F�N�g�G�N�X�v���[�W�����w�b�_ [effectPiece.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _EFFECTEXPLOSION_H_
#define _EFFECTEXPLOSION_H_

#include "effect.h"

// �}�N����`
#define EFFECTEXPLOSION_TEXTURE_NAME _T("data/TEXTURE/effect/explosion1.png")	// �e�N�X�`����

#define EFFECTEXPLOSION_TEXTURE_SIZE_X				(30.0f)
#define EFFECTEXPLOSION_TEXTURE_SIZE_Y				(30.0f)

#define EFFECTEXPLOSION_TEXTURE_DIVIDE_X			(8)								// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECTEXPLOSION_TEXTURE_DIVIDE_Y			(2)								// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define EFFECTEXPLOSION_ANIM_NUM			(EFFECTEXPLOSION_TEXTURE_DIVIDE_X*EFFECTEXPLOSION_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define EFFECTEXPLOSION_ANIM_TIME					(2)								// �A�j���[�V�����̐؂�ւ��J�E���g

#define EFFECTEXPLOSION_INITPOS_X					(-100)							//�������W�iX���W�j
#define EFFECTEXPLOSION_INITPOS_Y					(-100)							//�������W�iY���W�j

#define EFFECTEXPLOSION_MAX							(32*32)							//�e�G�t�F�N�g�G�N�X�v���[�W�����̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffectExplosion(int num);									//�G�t�F�N�g�G�N�X�v���[�W��������������
void UninitEffectExplosion(void);										//�G�t�F�N�g�G�N�X�v���[�W�����I������
void UpdateEffectExplosion(void);										//�G�t�F�N�g�G�N�X�v���[�W�����X�V����
void DrawEffectExplosion(void);											//�G�t�F�N�g�G�N�X�v���[�W�����`�揈��
EFFECT *GetEffectExplosionAdr(int pno);									//�G�t�F�N�g�G�N�X�v���[�W�����̃A�h���X�擾�֐�
EFFECT *SetEffectExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//�G�t�F�N�g�G�N�X�v���[�W�����̃Z�b�g�֐�

#endif
