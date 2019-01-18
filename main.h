//=============================================================================
//
// Main�w�b�_�[ [main.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~

#include "windows.h"
#include <time.h>
#include "d3dx9.h"
#include <tchar.h>


#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#if 1	// [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

#define SCREEN_WIDTH	(1280)												// �E�C���h�E�̕�4
#define SCREEN_HEIGHT	(960)												// �E�C���h�E�̍���3
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)									// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)									// �E�C���h�E�̒��S�x���W
#define SCREEN_END_LEFT	(BG_POS_X)
#define SCREEN_END_RIGHT (BG_POS_X + BG_TEXTURESIZE_X)

#define	NUM_VERTEX		(4)													// ���_��
#define NUM_POLYGON		(2)

#define VECTOR3_UP		(D3DXVECTOR3(0.0f, -1.0f, 0.0f))					//��ʏ�����ւ̒P�ʃx�N�g��
#define VECTOR3_RIGHT	(D3DXVECTOR3(1.0f, 0.0f, 0.0f))						//��ʉE�����ւ̒P�ʃx�N�g��
#define VECTOR3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))						//0�x�N�g��

#define PLAYERNAME_MAX	(5)													//�v���C���[�������ő吔

#ifdef _DEBUG
#define COLLIDER_COLOR		(D3DCOLOR_RGBA(255, 0, 0, 255))					//�����蔻��̕`��F
#define NUM_BB_VERTEX	(5)													//�o�E���f�B���O�{�b�N�X��`�悷��ۂ̒��_��
#endif

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//�p�x�����W�A���ɕϊ���������t���}�N��(x���p�x�j
#define RADIAN(x)					((x) * 0.01745329f)				

// ��L���_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float rhw;				// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR diffuse;		// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

enum DEFINE_SCENE
{
	SCENE_TITLE,
	SCENE_TUTORIAL,
	SCENE_GAME,
	SCENE_PAUSE,
	SCENE_CLEAR,
	SCENE_GAMEOVER,
	SCENE_NAMEENTRY
};

enum DEFINE_DIFFICULTY
{
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_MAX
};

enum EMITTER_SHAPE
{
	EMITTER_CONE,
	EMITTER_CIRCLE,
	EMITTER_SHAPEMAX
};

#include "myLibrary.h"

/*******************************************************
�v���g�^�C�v�錾
********************************************************/
LPDIRECT3DDEVICE9	GetDevice(void);				// �f�o�C�X�擾�֐�
void Pause(bool state);								//�|�[�Y�؂�ւ��֐�
void SetScene(int scene);							//�V�[���؂�ւ��֐�
void InitGame(int stage);							//�X�e�[�W�J�n�O�̏���������								
int GetDifficulty(void);							//��Փx�擾�֐�
void SetDifficulty(int n);							//��Փx�ݒ�֐�
void ResumeGame(void);								//�Q�[���ĊJ�֐�
int GetCurrentScene(void);							//�V�[���擾�֐�
bool isDemoMode(void);								//�f�����[�h����֐�

#ifdef _DEBUG
void DrawBoxCollider(D3DXVECTOR3 basePos, float width, float hight);					//�o�E���f�B���O�{�b�N�X����������֐�
#endif

#endif