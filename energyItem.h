//=============================================================================
//
// �G�i�W�[�A�C�e���w�b�_ [EnergyItem.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _ENERGYITEM_H_
#define _ENERGYITEM_H_


// �}�N����`
#define ENERGYITEM_TEXTURE_NAME	_T("data/TEXTURE/player/energyItem.png")	// �G�i�W�[�A�C�e���̃e�N�X�`��
#define ENERGYITEM_SHADOWTEX_NAME _T("data/TEXTURE/player/energyItemShadow.png") //�e�̃e�N�X�`��

#define ENERGYITEM_TEXTURE_SIZE_X					(64)					// �e�N�X�`���T�C�YX
#define ENERGYITEM_TEXTURE_SIZE_Y					(64)					// �e�N�X�`���T�C�YY

#define ENERGYITEM_TEXTURE_DIVIDE_X					(2)						// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define ENERGYITEM_TEXTURE_DIVIDE_Y					(1)						// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ENERGYITEM_ANIM_NUM			(ENERGYITEM_TEXTURE_DIVIDE_X*ENERGYITEM_TEXTURE_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ENERGYITEM_TIME_ANIMATION					(2)						// �A�j���[�V�����̐؂�ւ��J�E���g

#define ENERGYITEM_MAX								(8)					//�G�i�W�[�A�C�e���̍ő吔

#define ENERGYITEM_INITPOS_X						(-100)					//�G�i�W�[�A�C�e���̏���X���W
#define ENERGYITEM_INITPOS_Y						(-100)					//�G�i�W�[�A�C�e���̏���Y���W

#define ENERGYITEM_ADDVALUE							(PLAYER_TIMEEFFECT_MAX/2)	//�擾���ɉ��Z�����G�i�W�[

//�G�i�W�[�A�C�e���̃o�E���f�B���O�{�b�N�X�̒�`
#define ENERGYITEM_BB_TOP							(0.0f)
#define ENERGYITEM_BB_LEFT							(0.0f)
#define ENERGYITEM_BB_WIDTH							(10.0f)
#define ENERGYITEM_BB_HIGHT							(35.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;							//�g�por���g�p�̔���

	D3DXVECTOR3				pos;							// �|���S���̍��W
	D3DXVECTOR3				rot;							// �|���S���̉�]��
	int						countAnim;						// �A�j���[�V�����J�E���g
	int						patternAnim;					// �A�j���[�V�����p�^�[���i���o�[

	D3DXVECTOR3				moveDir;						//�i�s����
	int						state;

}ENERGYITEM;

enum ENEGYITEM_STATE
{
	ENERGYITEM_DEFAULT,
	ENERGYITEM_REDUCTION,
	ENERGYITEM_STATEMAX
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnergyItem(int num);							//�G�i�W�[�A�C�e������������
void UninitEnergyItem(void);								//�G�i�W�[�A�C�e���I������
void UpdateEnergyItem(void);								//�G�i�W�[�A�C�e���X�V����
void DrawEnergyItem(void);									//�G�i�W�[�A�C�e���`�揈��
ENERGYITEM *GetEnergyItemAdr(int pno);						//�G�i�W�[�A�C�e���A�h���X�擾�֐�
void SetEnergyItem(D3DXVECTOR3 pos);						//�G�i�W�[�A�C�e���Z�b�g�֐�

#endif
