//=============================================================================
//
// GUI�w�b�_ [GUI.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _GUI_H_
#define _PLAYERBYLLET_H_


// �}�N����`
#define GUI_TEXTURE_BACK _T("data/TEXTURE/GUI/GUIback.jpg")		// GUI�w�i�e�N�X�`��
#define GUI_TEXTURE_SIZE_X			(960)						// �e�N�X�`���T�C�Y
#define GUI_TEXTURE_SIZE_Y			(960)						// �e�N�X�`���T�C�Y

#define GUI_QUAKE_LENGTHMAX			(10.0f)						//GUI���h���Ƃ��̍ő啝
#define BOSSGAUGE_OFFSET			(50.0f)
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	D3DXVECTOR3				posR;						// �|���S���̍��W(�E)
	D3DXVECTOR3				posL;						// �|���S���̍��W(��)
	int						cntFrame;					//�J�E���g�t���[��
	int						state;						//���
}GUIback;

enum GUI_STATE
{
	GUI_DEFAULT,
	GUI_QUAKE,
	GUI_STATEMAX
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGUI(int num);								//GUI����������
void UninitGUI(void);									//GUI�I������
void UpdateGUI(void);									//GUI�X�V����
void DrawGUI(void);										//GUI�`�揈��
GUIback *GetGUIbackAdr(void);							//GUI�A�h���X�擾�֐�
void SetGUIShake(void);									//GUI��h�炷�֐�
void DrawParameter(void);
#endif
