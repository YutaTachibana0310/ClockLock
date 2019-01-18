//=============================================================================
//
// �X�R�A�A�C�e���w�b�_ [ScoreItem.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SCOREITEM_H_
#define _SCOREITEM_H_


// �}�N����`
#define SCOREITEM_TEXTURE_NAME			_T("data/TEXTURE/player/item.png")			// �X�R�A�A�C�e���̃e�N�X�`��
#define SCOREITEMLUMINE_TEXTURE_NAME	_T("data/TEXTURE/player/itemLumine.png")	// �X�R�A�A�C�e�����̃e�N�X�`��

#define SCOREITEM_TEXTURE_NUM						(2)							//�X�R�A�A�C�e���̃e�N�X�`���̐�

#define SCOREITEM_TEXTURE_SIZE_X					(120 / 2)					// �e�N�X�`���T�C�YX
#define SCOREITEM_TEXTURE_SIZE_Y					(120 / 2)					// �e�N�X�`���T�C�YY

#define SCOREITEM_TEXTURE_DIVIDE_X					(8)							// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define SCOREITEM_TEXTURE_DIVIDE_Y					(2)							// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_SCOREITEM_NUM							(7)							// �A�j���[�V�����p�^�[����
#define TIMESCOREITEM_ANIMATION						(2)							// �A�j���[�V�����̐؂�ւ��J�E���g

#define SCOREITEM_MAX								(4096)						//�X�R�A�A�C�e���̍ő吔

#define SCOREITEM_INITPOS_X							(-100)						//�X�R�A�A�C�e���̏���X���W
#define SCOREITEM_INITPOS_Y							(-100)						//�X�R�A�A�C�e���̏���Y���W

#define SCOREITEM_ADDVALUE							(500)						//�X�R�A�A�C�e���擾���̃X�R�A���Z�l

//�X�R�A�A�C�e���̃o�E���f�B���O�{�b�N�X�̒�`
#define SCOREITEM_BB_TOP							(0.0f)
#define SCOREITEM_BB_LEFT							(0.0f)
#define SCOREITEM_BB_WIDTH							(15.0f)
#define SCOREITEM_BB_HIGHT							(15.0f)

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
	float					baseRadius;						//�|���S���̔��a�i�ω������Ȃ��j
	float					angle;							// �|���S���̊p�x

	D3DXVECTOR3				velocity;						//�A�C�e���̐i�s����
	int						cntFrame;						//�t���[���J�E���g
	int						state;							//�A�C�e���̏��

	float					scale;

}SCOREITEM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScoreItem(int num);								//�X�R�A�A�C�e������������
void UninitScoreItem(void);									//�X�R�A�A�C�e���I���֐�
void UpdateScoreItem(void);									//�X�R�A�A�C�e���X�V����
void DrawScoreItem(void);									//�X�R�A�A�C�e���`�揈��
SCOREITEM *GetScoreItemAdr(int pno);						//�X�R�A�A�C�e���A�h���X�擾�֐�
void SetScoreItem(D3DXVECTOR3 pos, float range, int num);	//�X�R�A�A�C�e���Z�b�g�֐�
int GetScoreItemCount(void);								//�X�R�A�A�C�e���擾���̃Q�b�g�֐�
void SetScoreItemCount(int value);							//�X�R�A�A�C�e���擾���̃Z�b�g�֐�
float GetScoreItemScale(void);							//�X�P�[���X�e�[�g�擾�֐�

#ifdef _DEBUG
int GetActiveItemCount(void);								//�A�N�e�B�u�ȃX�R�A�A�C�e�����擾�֐�
void SetActiveItemCount(int value);							//�X�R�A�A�C�e��������֐�
#endif

#endif
