//=============================================================================
//
// �l�[���G���g���[�w�b�_ [NameEntry.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _NAMEENTRY_H_
#define _NAMEENTRY_H_


// �}�N����`
#define NAMEENTRY_TEXTURE_NAME	_T("data/TEXTURE/GUI/nameEntryBG.png")			// �l�[���G���g���[�̃e�N�X�`��
#define NAMEENTRY_CURSORTEX_NAME _T("data/TEXTURE/GUI/nameEntryCursor.png")		//�J�[�\���̃e�N�X�`��
#define NAMEENTRY_GUIDETEX_NAME _T("data/TEXTURE/GUI/nameEntryGuide.png")		//�K�C�h�̃e�N�X�`��

#define NAMEENTRY_CHARTEX_SIZE_X		(30.0f)					//�����Z�b�g�̃e�N�X�`���T�C�Y
#define NAMEENTRY_CHARTEX_SIZE_Y		(30.0f)					

#define NAMEENTRY_NAMETEX_SIZE_X		(20.0f)					//�v���C���[���̃e�N�X�`���T�C�Y
#define NAMEENTRY_NAMETEX_SIZE_Y		(20.0f)

#define NAMEENTRY_SCORETEX_SIZE_X		(20.0f)					//�X�R�A�̃e�N�X�`���T�C�Y
#define NAMEENTRY_SCORETEX_SIZE_Y		(20.0f)

#define NAMEENTRY_CURSORTEX_SIZE_X		(NAMEENTRY_CHARTEX_SIZE_X*1.5f)	//�J�[�\���̃e�N�X�`���T�C�Y
#define NAMEENTRY_CURSORTEX_SIZE_Y		(NAMEENTRY_CHARTEX_SIZE_Y*1.5f)
#define NAMEENTRY_CURSORTEX_DIVIDE_X	(2)								//�J�[�\���̃e�N�X�`��������	
#define NAMEENTRY_CURSORTEX_DIVIDE_Y	(2)
#define NAMEENTRY_CURSORTEX_ANIMAX		(NAMEENTRY_CURSORTEX_DIVIDE_X*NAMEENTRY_CURSORTEX_DIVIDE_X) //�J�[�\���̃A�j���[�V�����ő吔

#define NAMEENTRY_GUIDETEX_SIZE_X		(1280)					//�����e�L�X�g�̃e�N�X�`���T�C�Y
#define NAMEENTRY_GUIDETEX_SIZE_Y		(180)
#define NAMEENTRY_GUIDETEX_DIVIDE_Y		(2)						//�����e�L�X�g�̕�����

#define NAMEENTRY_CAHR_MAX				(5)						//���͂ł��镶���̍ő吔
#define NAMEENTRY_CHAR_INITPOS_X		(150.0f)				//�����Z�b�g�̏������W
#define NAMEENTRY_CHAR_INITPOS_Y		(830)
#define NAMEENTRY_CHAR_OFFSETPOS_X		(NAMEENTRY_CHARTEX_SIZE_X*2)	//�����Z�b�g�̃I�t�Z�b�g�l
#define NAMEENTRY_CHAR_OFFSETPOS_Y		(NAMEENTRY_CHARTEX_SIZE_Y*2)

#define NAMEENTRY_NAME_INITPOS_X		(805.0f)				//�l�[���̏������W
#define NAMEENTRY_NAME_INITPOS_Y		(740.0f)
#define NAMEENTRY_NAME_OFFSETPOS		(42.0f)					//�l�[���̃I�t�Z�b�g�l

#define NAMEENTRY_HIGHSCORE_INITPOS_X	(650.0f)				//�n�C�X�R�A�̏������W
#define NAMEENTRY_HIGHSCORE_INITPOS_Y	(170.0f)
#define NAMEENTRY_HIGHSCORE_OFFSET_X	(-30.0f)				//�n�C�X�R�A�̃I�t�Z�b�g�l
#define NAMEENTRY_HIGHSCORE_OFFSET_Y	(55.0f)

#define NAMEENTRY_SCORENAME_INITPOS_X	(800.0f)				//�n�C�X�R�A�̃v���C���[���̏������W
#define NAMEENTRY_SCORENAME_INITPOS_Y	(NAMEENTRY_HIGHSCORE_INITPOS_Y)

#define NAMEENTRY_CURRENTSCORE_INITPOS_X (650)					//����̃X�R�A�\���̏������W
#define NAMEENTRY_CURRENTSCORE_INITPOS_Y (NAMEENTRY_NAME_INITPOS_Y)

#define NAMEENTRY_GUIDE_INITPOS_X		(SCREEN_CENTER_X)		//�����e�L�X�g�̏������W
#define NAMEENTRY_GUIDE_INITPOS_Y		(780)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct {
	int						state;						//���

	int						cntFrame;					//�J�E���g�t���[��
	D3DXVECTOR3				pos;						// �|���S���̍��W
	D3DXVECTOR3				rot;						// �|���S���̉�]��

	float					radiusChar;					// �|���S���̔��a
	float					angleChar;					// �|���S���̊p�x
	int						selectedChar;				//���I�΂�Ă��镶��
	int						nameBuff[NAMEENTRY_CAHR_MAX];	//�v���C���[��
	int						wordCnt;					//���݂̃v���C���[���̕�����

	float					radiusCursor;				//�J�[�\���̃|���S�����a
	float					angleCursor;				//�J�[�\���̃|���S���p�x

	float					radiusName;					//�l�[���̃|���S�����a
	float					angleName;					//�l�[���̃|���S���p�x

	float					radiusScore;				//�X�R�A�̃|���S�����a
	float					angleScore;					//�X�R�A�̃|���S���p�x
}NAMEENTRY;

//�l�[���G���g���[��ʂ̏�ԗ񋓎q
enum NAMEENTRY_STATE
{
	NAMEENTRY_INPUT,
	NAMEENTRY_WAIT,
	NAMEENTRY_STATEMAX
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitNameEntry(int num);							//�l�[���G���g���[����������
void UninitNameEntry(void);								//�l�[���G���g���[�I������
void UpdateNameEntry(void);								//�l�[���G���g���[�X�V����
void DrawNameEntry(void);								//�l�[���G���g���[�`�揈��
NAMEENTRY *GetNameEntryAdr(void);						//�l�[���G���g���[�A�h���X�擾�֐�

#endif
