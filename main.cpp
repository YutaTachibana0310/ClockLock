//=============================================================================
//
// Main���� [main.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "playerBullet.h"
#include "GUI.h"
#include "particle.h"
#include "bulletFire.h"
#include "enemyBullet.h"
#include "collider.h"
#include "playerGauge.h"
#include "enemyManager.h"
#include "PBulletExplosion.h"
#include "scoreItem.h"
#include "stageManager.h"
#include "playerExplosion.h"
#include "pauseMenu.h"
#include "title.h"
#include "saveData.h"
#include "transition.h"
#include "charChip.h"
#include "nameEntry.h"
#include "gameOver.h"
#include "sound.h"
#include "bgmPlayer.h"
#include "sePlayer.h"
#include "energyItem.h"
#include "gameClear.h"
#include "playerBomber.h"
#include "EnemyBulletExplosion.h"
#include "warning.h"
#include "tutorial.h"
#include "groundFire.h"
#include "playerParticle.h"
#include "enemyParticle.h"
#include "playerShield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			_T("AppClass")				// �E�C���h�E�̃N���X��
#define WINDOW_NAME			_T("�N���b�N���b�N")		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void DrawGameScene(void);
void UpdateGameScene(void);

void InitTutorialScene(void);
void DrawTutorialScene(void);
void UpdateTutorialScene(void);

#ifdef _DEBUG
void DrawDebug(void);
#endif

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9				g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;		// Device�I�u�W�F�N�g(�`��ɕK�v)
D3DPRESENT_PARAMETERS	g_D3DPP;					// �f�o�C�X�̐ݒ�



static int currentScene = SCENE_TITLE;				// �V�[��
static bool isPause = false;						// �|�[�Y�����ۂ�
static int difficulty = 2;							// ��Փx
static bool masterPause = false;					// �X�N�V���B�e�p�|�[�Y�t���O
static bool demoMode = false;						// �f�����[�h�t���O
static bool fullScreenMode = false;					// �t���X�N���[�����[�h����

#ifdef _DEBUG
LPD3DXFONT				g_pD3DXFont = NULL;			// �t�H���g�ւ̃|�C���^
int						g_nCountFPS;				// FPS�J�E���^
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	srand((unsigned)time(NULL));

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		50, //CW_USEDEFAULT,																	// �E�B���h�E�̍����W
		0, //CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,										// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hWnd, true)))
	{
		return -1;
	}

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

				Update();			// �X�V����
				Draw();				// �`�揈��

				dwFrameCount++;		// �����񐔂̃J�E���g�����Z
			}
		}
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�t���T�C�Y���擾
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));				// ���[�N���[���N���A

	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�

	if (bWindow)
	{
		d3dpp.BackBufferWidth = SCREEN_WIDTH;			// �Q�[����ʃT�C�Y(��)�E�B���h�E���[�h
		d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)�E�B���h�E���[�h
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g�͌��ݐݒ肳��Ă�����̂��g��
	}
	else
	{
		d3dpp.BackBufferWidth = screenX;
		d3dpp.BackBufferHeight = screenY;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;			// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	//d3dpp.BackBufferFormat = d3ddm.Format;			// �J���[���[�h�̎w��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,							// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,								// �f�B�X�v���C�^�C�v
		hWnd,										// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,		// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,										// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))							// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���s��Ȃ�
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�

	// ���_�t�H�[�}�b�g�̐ݒ�
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���u�����f�B���O�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

#ifdef _DEBUG
	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pD3DXFont);
#endif

	//�Z�[�u�f�[�^������
	InitSaveData(0);

	//�T�E���h������
	InitSound(hWnd);
	InitBgmPlayer(0);
	InitSEplayer(0);

	//�G�l�~�[�֘A������
	InitStageManager(0);
	InitEnemyBullet(0);
	InitEnemyManager(0);
	InitEnemyBulletExplosion(0);

	//plyaer�֘A������
	InitPlayer(0);
	InitPlayerGauge(0);
	InitPBulletExplosion(0);
	InitScoreItem(0);
	InitPlayerExplosion(0);
	InitBulletFire(0);
	InitPlayerBomber(0);
	InitEnergyItem(0);
	InitPlayerBullet(0);
	InitPlayerShield(0);

	//�G�t�F�N�g������
	InitTransition(0);
	InitBG(0);
	InitParticle(0);

	//�V�X�e��������
	InitGUI(0);
	InitMyLibrary(0);
	InitPauseMenu(0);
	InitTitle(0);
	InitCharChip(0);
	InitNameEntry(0);
	InitGameOver(0);
	InitGameClear(0);
	InitWarning(0);
	InitTutorial(0);
	InitGroundFire(0);
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	if (g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
	}

	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
	}

	//�Z�[�u�f�[�^�ۑ�
	UninitSaveData();

	//�G�l�~�[�֘A�I������
	UninitPlayer();
	UninitStageManager();
	UninitPlayerBullet();
	UninitTransition();
	UninitEnemyBullet();
	UninitEnemyManager();
	UninitEnemyBulletExplosion();

	//player�I������
	UninitBulletFire();
	UninitPlayerGauge();
	UninitPBulletExplosion();
	UninitScoreItem();
	UninitPlayerExplosion();
	UninitEnergyItem();
	UninitPlayerBomber();
	UninitPlayerShield();

	//�G�t�F�N�g�I������
	UninitParticle();
	UninitGroundFire();

	//�V�X�e���I��
	UninitBG();
	UninitGUI();
	UninitMyLibrary();
	UninitPauseMenu();
	UninitTitle();
	UninitCharChip();
	UninitNameEntry();
	UninitGameOver();
	UninitGameClear();
	UninitWarning();
	UninitTutorial();

	// ���͏����̏I������
	UninitInput();

	//�T�E���h�I������
	UninitBgmPlayer();
	UninitSEplayer();
	UninitSound();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	UpdateInput();

	//�X�N���[���V���b�g�B�e�p�|�[�Y
	if (GetKeyboardTrigger(DIK_V))
	{
		masterPause = !masterPause;
	}
	if (masterPause)
	{
		return;
	}
	

	// ���͂̍X�V����
	UpdateTransition();
	UpdateBgmPlayer();
	UpdateSEplayer();

	switch (currentScene)
	{
	case SCENE_TITLE:			//�^�C�g����ʍX�V����
		UpdateTitle();
		break;

	case SCENE_TUTORIAL:		//�`���[�g���A����ʍX�V����
		UpdateTutorialScene();
		break;

	case SCENE_GAME:			//�Q�[����ʍX�V����
		if (GetMyButtonPuaseTrigger(0))
		{
			//�|�[�Y����
			Pause(true);
			break;
		}

		UpdateGameScene();
		break;

	case SCENE_PAUSE:			//�|�[�Y��ʍX�V����
		UpdatePauseMenu();
		break;

	case SCENE_GAMEOVER:		//�Q�[���I�[�o�[��ʍX�V����
		UpdateGameScene();
		UpdateGameOver();
		break;

	case SCENE_CLEAR:			//�N���A��ʍX�V����
		UpdateGameScene();
		UpdateGameClear();
		break;

	case SCENE_NAMEENTRY:		//�l�[���G���g���[��ʍX�V����
		UpdateNameEntry();
		break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (currentScene)
		{
		case SCENE_TITLE:
			DrawTitle();
			break;

		case SCENE_TUTORIAL:
			DrawTutorialScene();
			break;

		case SCENE_GAME:
			DrawGameScene();
			DrawGUI();
			break;

		case SCENE_PAUSE:
			DrawGameScene();
			DrawPauseMenu();
			DrawGUI();
			break;

		case SCENE_CLEAR:
			DrawGameScene();
			DrawGameClear();
			DrawGUI();
			break;

		case SCENE_GAMEOVER:
			DrawGameScene();
			DrawGameOver();
			DrawGUI();
			break;

		case SCENE_NAMEENTRY:
			DrawNameEntry();
			break;
		}

		DrawTransition();

#ifdef _DEBUG
		// FPS�\��
		DrawDebug();
#endif

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�擾�֐�
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return(g_pD3DDevice);
}

/******************************************************************************
//�֐���	�FLPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
//����1		�FLPSTR szName�F�ǂݍ��݂����t�@�C����
//����2		�FLPDIRECT3DDEVICE9 lpD3DDevice�F�f�o�C�X�I�u�W�F�N�g
//�߂�l	�F�ǂݍ��񂾃e�N�X�`���ւ̃|�C���^
//����		�F�T�C�Y���w�肵�ăe�N�X�`���t�@�C����ǂݍ��ފ֐�
******************************************************************************/
LPDIRECT3DTEXTURE9 CreateTextureFromFile(LPSTR szName, LPDIRECT3DDEVICE9 lpD3DDevice)
{
	HRESULT             hRet;

	//�߂�l�̃e�N�X�`��
	LPDIRECT3DTEXTURE9  lpTex = NULL;
	if (lpD3DDevice) {

		//D3DXGetImageInfoFromFile�Ŏg�p����摜�t�@�C���̏��
		D3DXIMAGE_INFO      iinfo;

		//�摜�t�@�C���̏����擾
		hRet = D3DXGetImageInfoFromFile(szName, &iinfo);
		if (hRet == D3D_OK)
		{
			//�e�N�X�`�����t�@�C������쐬
			hRet = D3DXCreateTextureFromFileEx(
				lpD3DDevice,
				szName,
				iinfo.Width, iinfo.Height,			//�擾�����摜�t�@�C���̃T�C�Y���w�肷��
				1, 0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0xFF000000,
				NULL, NULL,
				&lpTex);
		}
	}
	return lpTex;
}

//=============================================================================
//�|�[�Y�؂�ւ��֐�
//=============================================================================
void Pause(bool state)
{
	isPause = state;
	
	if (state)
	{
		currentScene = SCENE_PAUSE;
		SetPauseMenuState(PAUSEMENU_OPEN);
	}
}

//=============================================================================
//�V�[���؂�ւ��֐�
//=============================================================================
void SetScene(int scene)
{
	currentScene = scene;

	switch (scene)
	{
	case SCENE_TITLE:
		InitTitle(1);
		PlayBGM(BGM_TITLE);
		break;

	case SCENE_GAME:
		InitGame(0);
		PlayBGM(BGM_STAGE1);
		break;

	case SCENE_NAMEENTRY:
		InitNameEntry(1);
		PlayBGM(BGM_NAMEENTRY);
		break;

	case SCENE_TUTORIAL:
		InitTutorialScene();
		PlayBGM(BGM_TUTORIAL);
		//�`���[�g���A��BGM�Đ�
		break;
	}
}

//=============================================================================
//�Q�[���ĊJ�֐�
//=============================================================================
void ResumeGame()
{
	//PlayBGM(BGM_STAGE1);
	currentScene = SCENE_GAME;
}

//=============================================================================
//�X�e�[�W�J�n�O�̏���������
//=============================================================================
void InitGame(int num)
{
	//difficulty = num;

	InitStageManager(1);
	InitBG(1);
	InitGUI(1);
	InitPlayer(1);
	InitPlayerBullet(1);
	InitPlayerBomber(1);
	InitParticle(1);
	InitBulletFire(1);
	InitPlayerGauge(1);
	InitEnemyBullet(1);
	InitPBulletExplosion(1);
	InitEnemyManager(1);
	InitScoreItem(1);
	InitPlayerExplosion(1);
	InitEnergyItem(1);
	InitGameOver(1);
	InitEnemyBulletExplosion(1);
	InitWarning(1);
	InitEnemyBullet(1);
	InitGroundFire(1);
	InitPlayerShield(1);

	//SetTransition(SCENE_GAME);
}

//=============================================================================
//�Q�[���V�[���`�揈��
//=============================================================================
void DrawGameScene(void)
{
	DrawBG();
	DrawGroundFire();
	DrawStageManager();
	DrawPlayerBomber();
	DrawEnemyManager();
	DrawPlayerBullet();
	DrawScoreItem();
	DrawParticle();
	DrawPlayerGauge();
	DrawPBulletExplosion();
	DrawBulletFire();
	DrawPlayer();
	DrawPlayerShield();
	DrawEnergyItem();
	DrawPlayerExplosion();
	DrawEnemyBulletExplosion();

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DrawEnemyParticle();
	DrawPlayerParticle();
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DrawEnemyBullet();
	DrawParameter();
	DrawWarning();
	//GUI�`�揈��
	DrawBossGauge();
	DrawEnemyGauge();
	DrawGameOver();
}

//=============================================================================
//�Q�[���V�[���X�V����
//=============================================================================
void UpdateGameScene(void)
{
	UpdateStageManager();
	UpdateBG();
	UpdateGroundFire();
	UpdatePlayerBullet();
	UpdateParticle();
	UpdateBulletFire();
	UpdatePlayer();
	UpdatePlayerShield();
	UpdatePlayerGauge();
	UpdateEnemyBullet();
	UpdateEnemyManager();
	UpdateScoreItem();
	UpdateEnergyItem();
	UpdatePlayerBomber();
	UpdatePlayerExplosion();
	UpdateEnemyBulletExplosion();
	UpdateWarning();

	//GUI�̍X�V����
	UpdateGUI();
	UpdatePBulletExplosion();
	UpdateCollider();
}

//=============================================================================
//�`���[�g���A���V�[������������
//=============================================================================
void InitTutorialScene(void)
{
	InitPlayerBullet(1);
	InitParticle(1);
	InitBulletFire(1);
	InitPlayer(2);
	InitPlayerGauge(1);
	InitEnemyBullet(1);
	InitScoreItem(1);
	InitPlayerBomber(1);
	InitPlayerExplosion(1);
	InitEnemyBulletExplosion(1);
	InitGUI(1);
	InitPBulletExplosion(1);
	InitEnemyBullet(1);
	InitWarning(1);
	InitTutorial(1);
	InitCollider();
}

//=============================================================================
//�`���[�g���A���V�[���X�V����
//=============================================================================
void UpdateTutorialScene(void)
{
	UpdatePlayerBullet();
	UpdateParticle();
	UpdateBulletFire();
	UpdatePlayer();
	UpdatePlayerGauge();
	UpdateEnemyBullet();
	UpdateScoreItem();
	UpdatePlayerBomber();
	UpdatePlayerExplosion();
	UpdateEnemyBulletExplosion();
	UpdateGUI();
	UpdatePBulletExplosion();
	UpdateTutorial();
	UpdateCollider();
}

//=============================================================================
//�`���[�g���A���V�[���`�揈��
//=============================================================================
void DrawTutorialScene(void)
{
	DrawTutorialBG();
	DrawTimeEffect();
	DrawPlayerBomber();
	DrawPlayerBullet();
	DrawParticle();
	DrawBulletFire();
	DrawPlayerGauge();
	DrawPlayer();
	DrawEnemyBullet();
	DrawScoreItem();
	DrawPlayerExplosion();
	DrawEnemyBulletExplosion();
	DrawPBulletExplosion();

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	DrawEnemyParticle();
	DrawPlayerParticle();
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DrawParameter();
	DrawGUI();
	DrawTutorial();
	DrawCollider();


}

//=============================================================================
//��Փx�擾�֐�
//=============================================================================
int GetDifficulty(void)
{
	return difficulty;
}

//=============================================================================
//��Փx�擾�֐�
//=============================================================================
void SetDifficulty(int n)
{
	//�ʏ�̓�Փx�ݒ�
	if (n < DIFFICULTY_MAX)
	{
		difficulty = n;
		demoMode = false;
	}
	//�f���p��Փx
	else
	{
		difficulty = DIFFICULTY_EASY;
		demoMode = true;
	}
}

//=============================================================================
//�V�[���擾�֐�
//=============================================================================
int GetCurrentScene(void)
{
	return currentScene;
}

//=============================================================================
//�f�����[�h����֐�
//=============================================================================
bool isDemoMode(void)
{
	return demoMode;
}

//=============================================================================
//�f�B�X�v���C���[�h�ύX�֐�
//=============================================================================
void ChangeDisplayMode(void)
{
	fullScreenMode = !fullScreenMode;

	//�t���X�N���[�����[�h�ɐݒ�
	if (fullScreenMode)
	{
	
	}
}


#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�\������
//=============================================================================
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS:%d\nEnemyBullet:%d\nScoreItem:%d\nBGM:%d"), g_nCountFPS, GetEnemyBulletCnt(), GetActiveItemCount(), (int)GetBgmPlayerAdr()->bgmVolume);

	// �e�L�X�g�`��
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif