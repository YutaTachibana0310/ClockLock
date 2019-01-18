//=============================================================================
//
// �|�[�Y���j���[�w�b�_ [PauseMenu.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_


// �}�N����`
#define PAUSEMENU_TEXTURE_NAME	_T("data/TEXTURE/GUI/pausemenu.png")		// �|�[�Y���j���[�̃e�N�X�`��
#define PAUSEMENU_CURSORTEX_NAME _T("data/TEXTURE/GUI/pauseLumine.png")		//�|�[�Y���j���[�̃J�[�\���e�N�X�`��

#define PAUSEMENU_TEXTURE_SIZE_X					(150)					// �e�N�X�`���T�C�YX
#define PAUSEMENU_TEXTURE_SIZE_Y					(150)					// �e�N�X�`���T�C�YY

#define PAUSEMENU_CURSORTEX_DIVIDE_X				(3)						// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define PAUSEMENU_MAX								(1)						//�|�[�Y���j���[�̍ő吔

#define PAUSEMENU_INITPOS_X							(SCREEN_CENTER_X)		//�|�[�Y���j���[�̏���X���W
#define PAUSEMENU_INITPOS_Y							(SCREEN_CENTER_Y)		//�|�[�Y���j���[�̏���Y���W

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	bool					active;						//�g�por���g�p�̔���

	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	int						select;						//�I������Ă��郁�j���[
	float					scale;						//�X�P�[��
	int						state;						//���
	int						cntFrame;					//�J�E���g�t���[��
}PAUSEMENU;

//�|�[�Y���j���[�̏�ԗ񋓌^
enum PAUSEMENU_STATE
{
	PAUSEMENU_OPEN,
	PAUSEMENU_SELECT,
	PAUSEMENU_CLOSE,
	PAUSEMENU_NONE,
	PAUSEMENU_STATEMAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPauseMenu(int num);							//�|�[�Y���j���[����������
void UninitPauseMenu(void);								//�|�[�Y���j���[�I������
void UpdatePauseMenu(void);								//�|�[�Y���j���[�X�V����
void DrawPauseMenu(void);								//�|�[�Y���j���[�`�揈��
PAUSEMENU *GetPauseMenuAdr(int pno);					//�|�[�Y���j���[�A�h���X�擾�֐�
void SetPauseMenuState(PAUSEMENU_STATE state);			//�|�[�Y���j���[�̏�ԃZ�b�g�֐�

#endif
