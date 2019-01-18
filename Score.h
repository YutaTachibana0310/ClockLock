//=============================================================================
//
// �X�R�A�w�b�_ [score.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//#include "main.h"

// �}�N����`
#define SCORE_TEXTURE_NAME	_T("data/TEXTURE/GUI/score.png")				// �X�R�A�̃e�N�X�`��
#define SCORENUM_TEXTURE_NAME _T("data/TEXTURE/GUI/scoreNum.png")			//�X�R�A�̐����̃e�N�X�`��

#define SCORE_TEXTURE_NUM					(2)								//�X�R�A�e�N�X�`���̐�

#define SCORENUM_TEXTURE_SIZE_X				(15)							//�����e�N�X�`���T�C�YX
#define SCORENUM_TEXTURE_SIZE_Y				(20)							//�����e�N�X�`���T�C�YY

#define SCOREBACK_TEXTURE_SIZE_X			(65)							//�X�R�A�w�i�e�N�X�`���T�C�YX
#define SCOREBACK_TEXTURE_SIZE_Y			(20)							//�X�R�A�w�i�e�N�X�`���T�C�YY

#define SCORENUM_TEXTURE_DIVIDE_X			(10)							// �A�j���p�^�[���̃e�N�X�`�����������iX)

#define SCORE_MAX							(8)								//�X�R�A�̍ő包��

#define SCORE_INITPOS_X						(310)							//�X�R�A��X���W
#define SCORE_INITPOS_Y						(25)							//�X�R�A��Y���W

#define SCORENUM_INITPOS_X					(70)							//�X�R�A������X���W
#define SCORENUM_INITPOS_Y					(0)							//�X�R�A������Y���W
#define SCORENUM_OFFSETPOS_X				(20.0f)						//�X�R�A�����̃I�t�Z�b�g���WX

#define SCOREBACK_INITPOS_X					(0.0f)							//�X�R�A�w�i��X���W
#define SCOREBACK_INITPOS_Y					(30)							//�X�R�A�w�i��Y���W

#define HISCORE_MAX							(10)							//�ۑ�����n�C�X�R�A�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int						score;										 //���݂̃n�C�X�R�A
	int						HiScore[HISCORE_MAX];						 //�n�C�X�R�A
	int						playerName[HISCORE_MAX][PLAYERNAME_MAX];	 //�n�C�X�R�A��B�������v���C���[��
}SCOREDATA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(int num);											//�X�R�A����������
void UninitScore(void);												//�X�R�A�I������
void UpdateScore(void);												//�X�R�A�X�V����
void DrawScore(void);												//�X�R�A�`�揈���i�����p�j
void DrawScoreBack(void);											//�X�R�A�`�揈���i�w�i�p�j
SCOREDATA *GetScoreDataAdr(void);									//�X�R�A�f�[�^�A�h���X�擾�֐�	
void AddScore(int value);											//�X�R�A���Z����
void CheckHiscoreUpdate(int *name);									//�n�C�X�R�A�X�V�m�F�֐�
void ShiftHiscoreName(int start);									//�n�C�X�R�A�̃l�[�����V�t�g����֐�
bool isUpdateHighScore(void);										//�n�C�X�R�A���X�V����Ă��邩�`�F�b�N
#endif
