//=============================================================================
//
// ���͏��� [input.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad�p�ݒ�l
#define DEADZONE		5000			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

#define MYINPUT_MAX		1				// ���͑��u�̍ő吔

#define AXIS_UP			1				//Y�������
#define AXIS_DOWN		-1				//Y��������
#define AXIS_RIGHT		1				//X���E����
#define AXIS_LEFT		-1				//X��������

#define INPUT_SHORTWAIT (6)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMouse();						// �}�E�X�̏I������
HRESULT UpdateMouse();					// �}�E�X�̍X�V����

HRESULT InitializePad(void);			// �p�b�h������
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

int GetPadAxisXPressed(int padNo);
int GetPadAxisXTriggered(int padNo);
int GetPadAxisYPressed(int padNo);
int GetPadAxisYTriggered(int padNo);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2   mouseTrigger;	// �����ꂽ�u�Ԃ���ON

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// ���o�����p�b�h�̐�

//�X�e�B�b�N��Repeat��Ԍ��o�p
static int		padAxisXRepeat[GAMEPADMAX];
static int		padAxisYRepeat[GAMEPADMAX];
static int		lastAxisX[GAMEPADMAX];
static int		lastAxisY[GAMEPADMAX];
static int		axisXRepeatCnt[GAMEPADMAX];
static int		axisYRepeatCnt[GAMEPADMAX];

//-------------------------------- MYINPUT
static MYINPUT myInput[MYINPUT_MAX];

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;
	int i;

	if (!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitializeMouse(hInst, hWnd);

	// �p�b�h�̏�����
	InitializePad();

	//MYINPUT�̏�����
	for (i = 0; i < MYINPUT_MAX; i++)
	{
		myInput[i].button0Key = DIK_Z;
		myInput[i].button1Key = DIK_X;
		myInput[i].button2Key = DIK_C;
		myInput[i].pauseKey = DIK_P;
		myInput[i].slowButtonKey = DIK_LSHIFT;
		myInput[i].button0Pad = BUTTON_A;
		myInput[i].button1Pad = BUTTON_Z;
		myInput[i].button2Pad = BUTTON_B;
		myInput[i].pausePad = BUTTON_START;
		myInput[i].slowButtonPad = BUTTON_Y;

		//myInput[i].button0 = false;
		//myInput[i].button1 = false;
		//myInput[i].button2 = false;
		myInput[i].axisX = 0;
		myInput[i].axisY = 0;
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �p�b�h�̏I������
	UninitPad();

	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	int i;

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �p�b�h�̍X�V
	UpdatePad();

	//MYINPUT�̍X�V
	for (i = 0; i < MYINPUT_MAX; i++)
	{
		//�e�{�^���̓��͏�Ԃ𔻒�
		//myInput[i].button0 = GetKeyboardPress(myInput[i].button0Key) || IsButtonPressed(i, myInput[0].button0Pad);
		//myInput[i].button1 = GetKeyboardPress(myInput[i].button1Key) || IsButtonPressed(i, myInput[0].button1Pad);
		//myInput[i].button2 = GetKeyboardPress(myInput[i].button2Key) || IsButtonPressed(i, myInput[0].button2Pad);
		//myInput[i].pause = GetKeyboardPress(myInput[i].pauseKey) || IsButtonPressed(i, myInput[0].pausePad);

		//�����L�[�ƃA�i���O�X�e�B�b�N�̓��͏�Ԃ𔻒�
		//Y����
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(i, BUTTON_UP))
		{
			myInput[i].axisY = -1;
		}
		else if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(i, BUTTON_DOWN))
		{
			myInput[i].axisY = 1;
		}
		else
		{
			myInput[i].axisY = 0;
		}

		//X����
		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(i, BUTTON_RIGHT))
		{
			myInput[i].axisX = 1;
		}
		else if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(i, BUTTON_LEFT))
		{
			myInput[i].axisX = -1;
		}
		else
		{
			myInput[i].axisX = 0;
		}
	}
}

/******************************************************************************
//�֐���	�Fbool GetMyButton0Down(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��0�i�V���b�g�{�^���j�̓��͏��
//����		�F�{�^��0�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton0Down(int no)
{
	return GetKeyboardPress(myInput[no].button0Key) || IsButtonPressed(no, myInput[no].button0Pad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButton1Down(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��1�i���ԑ���{�^��1�j�̓��͏��
//����		�F�{�^��1�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton1Down(int no)
{
	return GetKeyboardPress(myInput[no].button1Key) || IsButtonPressed(no, myInput[no].button1Pad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButton2Down(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton2Down(int no)
{
	return GetKeyboardPress(myInput[no].button2Key) || IsButtonPressed(no, myInput[no].button2Pad);
}

/******************************************************************************
//�֐���	�Fbool GetSlowButton2Down(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�X���E�{�^���̓��͏��
//����		�F�X���E�{�^���̏�Ԏ擾�֐�
******************************************************************************/
bool GetMySlowButtonDown(int no)
{
	return GetKeyboardPress(myInput[no].slowButtonKey) || IsButtonPressed(no, myInput[no].slowButtonPad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButton0Trigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton0Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button0Key) || IsButtonTriggered(no, myInput[no].button0Pad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButton1Trigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton1Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button1Key) || IsButtonTriggered(no, myInput[no].button1Pad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButton2Trigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButton2Trigger(int no)
{
	return GetKeyboardTrigger(myInput[no].button2Key) || IsButtonTriggered(no, myInput[no].button2Pad);
}
/******************************************************************************
//�֐���	�Fbool GetMyButtonPuseTrigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetMyButtonPuaseTrigger(int no)
{
	return GetKeyboardTrigger(myInput[no].pauseKey);// || IsButtonPressed(no, myInput[no].pausePad);
}

/******************************************************************************
//�֐���	�Fbool GetMyButtonPuseTrigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fbool		�F�{�^��2�i���ԑ���{�^��2�j�̓��͏��
//����		�F�{�^��2�̏�Ԏ擾�֐�
******************************************************************************/
bool GetSlowButtonTrigger(int no)
{
	return GetKeyboardTrigger(myInput[no].slowButtonKey) || IsButtonPressed(no, myInput[no].slowButtonPad);
}

/******************************************************************************
//�֐���	�Fint GetMyAxisX(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FX�������̓��͏��
//����		�FX�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisX(int no)
{
	return myInput[no].axisX;
}

/******************************************************************************
//�֐���	�Fint GetMyAxisY(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FY�������̓��͏��
//����		�FY�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisY(int no)
{
	return myInput[no].axisY;
}

/******************************************************************************
//�֐���	�Fint GetMyAxisYTrigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FY�������̓��͏��
//����		�FY�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisYTrigger(int no)
{
	if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(no, BUTTON_UP))
	{
		return AXIS_UP;
	}

	if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(no, BUTTON_DOWN))
	{
		return AXIS_DOWN;
	}

	else
	{
		return 0;
	}
}

/******************************************************************************
//�֐���	�Fint GetMyAxisXTrigger(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FX�������̓��͏��
//����		�FY�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisXTrigger(int no)
{
	if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(no, BUTTON_RIGHT))
	{
		return AXIS_RIGHT;
	}

	if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(no, BUTTON_LEFT))
	{
		return AXIS_LEFT;
	}

	else
	{
		return 0;
	}
}

/******************************************************************************
//�֐���	�Fint GetMyAxisXRepeat(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FX�������̓��͏��
//����		�FY�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisXRepeat(int no)
{
	if (padAxisXRepeat[no] != 0)
	{
		return padAxisXRepeat[no];
	}

	if (GetKeyboardRepeat(DIK_RIGHT))
	{
		return AXIS_RIGHT;
	}

	if (GetKeyboardRepeat(DIK_LEFT))
	{
		return AXIS_LEFT;
	}

	return 0;
}

/******************************************************************************
//�֐���	�Fint GetMyAxisXTRepeat(int no)
//����1		�Fint no	�F�擾������myInput[]�̃C���f�b�N�X
//�߂�l	�Fint		�FX�������̓��͏��
//����		�FY�������̏�Ԏ擾�֐�
******************************************************************************/
int GetMyAxisYRepeat(int no)
{
	if (padAxisYRepeat[no] != 0)
	{
		return padAxisYRepeat[no];
	}

	if (GetKeyboardRepeat(DIK_UP))
	{
		return AXIS_UP;
	}

	if (GetKeyboardRepeat(DIK_DOWN))
	{
		return AXIS_DOWN;
	}

	return 0;
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20 && g_keyStateRepeatCnt[cnt] % INPUT_SHORTWAIT == 0)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void UninitMouse()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMouse->Acquire();
	}
	return result;

}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}
//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
//---------------------------------------- ������
HRESULT InitializePad(void)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i < padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- �I������
void UninitPad(void)
{
	for (int i = 0; i < GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* �x�{�^��
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* �y�{�^��
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& padState[i]);					// ��������ON�̂��

		//�X�e�B�b�N��Repeat���菈��
		padAxisYRepeat[i] = GetPadAxisYTriggered(i);

		if (BUTTON_UP & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = 1;
			}
		}
		else if (BUTTON_DOWN & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = -1;
			}
		}
		else
		{
			axisYRepeatCnt[i] = 0;
			padAxisYRepeat[i] = 0;
		}

		padAxisXRepeat[i] = GetPadAxisXTriggered(i);

		if (BUTTON_RIGHT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = 1;
			}
		}
		else if (BUTTON_LEFT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = -1;
			}
		}
		else
		{
			axisXRepeatCnt[i] = 0;
			padAxisXRepeat[i] = 0;
		}

	}

}

//----------------------------------------------- ����
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

void CheckAxisXRepeat(int padNo)
{

}

//�X�e�B�b�N�̓��͌��o
int GetPadAxisXTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_RIGHT))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_LEFT))
		return -1;

	else
		return 0;
}

int GetPadAxisYTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_UP))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_DOWN))
		return -1;

	else
		return 0;
}

//�p�b�h�̐����o�֐�
int GetPadCount(void)
{
	return padCount;
}