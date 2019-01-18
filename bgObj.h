//=============================================================================
//
// BG�I�u�W�F�w�b�_ [BgObj.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _BGOBJ_H_
#define _BGOBJ_H_

// �}�N����`
#define BGOBJ_TEXTURE_NAME	_T("data/TEXTURE/bg/stage1_bg_obj.png")	// BG�I�u�W�F�̃e�N�X�`��

#define BGOBJ_TEXTURE_SIZE_X					(BG_TEXTURESIZE_X)		// �e�N�X�`���T�C�YX
#define BGOBJ_TEXTURE_SIZE_Y					(BG_TEXTURESIZE_Y)		// �e�N�X�`���T�C�YY

#define BGOBJ_TEXTURE_DIVIDE_X					(4)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define BGOBJ_TEXTURE_DIVIDE_Y					(3)						// �A�j���p�^�[���̃e�N�X�`�����������iY)

#define BGOBJ_MAX								(12)					//BG�I�u�W�F�̍ő吔

#define BGOBJ_INITPOS_X							(BG_LEFT)				//BG�I�u�W�F�̏���X���W
#define BGOBJ_INITPOS_Y							(0.0f)					//BG�I�u�W�F�̏���Y���W

#define BGOBJ_SPEED				(SCREEN_HEIGHT * -BG_STAGE1_SPEED3)		//�X�N���[���X�s�[�h
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	D3DXVECTOR3				pos;							// �|���S���̍��W
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[
}BGOBJ;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBgObj(int num);							//BG�I�u�W�F����������
void UninitBgObj(void);								//BG�I�u�W�F�I������
void UpdateBgObj(void);								//BG�I�u�W�F�X�V����
void DrawBgObj(void);								//BG�I�u�W�F�`�揈��
BGOBJ *GetBgObjAdr(int pno);						//BG�I�u�W�F�A�h���X�擾�֐�

#endif
