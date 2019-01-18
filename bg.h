//=============================================================================
//
// �w�i�w�b�_�[ [bg.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "enemyBullet.h"

// �}�N����`
#define BG_TEXTURENAME_STAGE1_0	_T("data/TEXTURE/bg/bg_stage1_0.png")	// �e�N�X�`����
#define BG_TEXTURENAME_STAGE1_1	_T("data/TEXTURE/bg/bg_stage1_1.png")	// �e�N�X�`����
#define BG_TEXTURENAME_STAGE1_2	_T("data/TEXTURE/bg/bg_stage1_2.png")	// �e�N�X�`����

#define BG_TEXTURENAME_EFFECT1 _T("data/TEXTURE/bg/effect1.png")	// �e�N�X�`����

#define BG_TEXTURESIZE_X		(780)								// �e�N�X�`���T�C�Y 4:5
#define BG_TEXTURESIZE_Y		(960)								// �e�N�X�`���T�C�Y 

#define BG_POS_X			((SCREEN_WIDTH-BG_TEXTURESIZE_X)/2)		//�w�i�e�N�X�`����X���W

#define BG_LEFT					(BG_POS_X)							//�Q�[����ʍ��[
#define BG_RIGHT				(BG_POS_X+BG_TEXTURESIZE_X)			//�Q�[����ʉE�[

#define BG_MAX					(3)									//�w�i�̍ő吔

#define BG_STAGE1_SPEED1		(-0.0005f)							//�w�i�̃X�N���[���X�s�[�h�i���i�j
#define BG_STAGE1_SPEED2		(-0.0007f)							//�w�i�̃X�N���[���X�s�[�h�i���i�j
#define BG_STAGE1_SPEED3		(-0.0012f)							//�w�i�̃X�N���[���X�s�[�h�i�ߌi�j

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	tex;							//�e�N�X�`���ւ̃|�C���^
	float				offsetSpeedX;					//�w�i�̃X�N���[�����x	
	float				offsetSpeedY;						
	float				offsetX;						//�w�i�̃I�t�Z�b�g�l
	float				offsetY;
}BG;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBG(int num);					//�w�i�̏���������
void UninitBG(void);						//�w�i�̏I������
void UpdateBG(void);						//�w�i�̍X�V����
void DrawBG(void);							//�w�i�̕`�揈��
void SetTimeEffect(SPEEDTYPE type);			//���ԑ���G�t�F�N�g�p�̔w�i�Z�b�g�֐�
void DrawTimeEffect(void);					//���ԑ���G�t�F�N�g�̕`�揈��
#endif
