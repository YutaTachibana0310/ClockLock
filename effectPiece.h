//=============================================================================
//
// �G�t�F�N�g�s�[�X�w�b�_ [effectPiece.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _EFFECTPIECE_H_
#define _EFFECTPIECE_H_

#include "effect.h"

// �}�N����`
#define EFFECTPIECE_TEXTURE_NAME _T("data/TEXTURE/effect/enemyPiece.png")	// �e�N�X�`����

#define EFFECTPIECE_TEXTURE_SIZE_X				(5.0f)
#define EFFECTPIECE_TEXTURE_SIZE_Y				(5.0f)

#define EFFECTPIECE_TEXTURE_DIVIDE_X			(8)								// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define EFFECTPIECE_TEXTURE_DIVIDE_Y			(2)								// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define EFFECTPIECE_ANIM_NUM					(16)	// �A�j���[�V�����p�^�[����
#define EFFECTPIECE_ANIM_TIME					(1)								// �A�j���[�V�����̐؂�ւ��J�E���g

#define EFFECTPIECE_INITPOS_X					(-100)							//�������W�iX���W�j
#define EFFECTPIECE_INITPOS_Y					(-100)							//�������W�iY���W�j

#define EFFECTPIECE_MAX							(32*32)							//�e�G�t�F�N�g�s�[�X�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffectPiece(int num);									//�G�t�F�N�g�s�[�X����������
void UninitEffectPiece(void);										//�G�t�F�N�g�s�[�X�I������
void UpdateEffectPiece(void);										//�G�t�F�N�g�s�[�X�X�V����
void DrawEffectPiece(void);											//�G�t�F�N�g�s�[�X�`�揈��
EFFECT *GetEffectPieceAdr(int pno);									//�G�t�F�N�g�s�[�X�̃A�h���X�擾�֐�
EFFECT *SetEffectPiece(D3DXVECTOR3 pos, D3DXVECTOR3 dir);			//�G�t�F�N�g�s�[�X�̃Z�b�g�֐�

#endif
