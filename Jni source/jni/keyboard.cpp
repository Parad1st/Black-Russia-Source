#include "main.h"

#include "keyboardhistory.h"
#include "util/CJavaWrapper.h"
#include "gui/gui.h"
#include "game/game.h"
#include "keyboard.h"
#include "scrollbar.h"

extern CGUI* pGUI;

CKeyBoard::CKeyBoard()
{
	Log("Initalizing KeyBoard..");

	Log("KeyBoard inited");
}

CKeyBoard::~CKeyBoard()
{
}

void CKeyBoard::Render()
{
}

void CKeyBoard::Open(keyboard_callback* handler, bool bHiden)
{
	if (handler == nullptr) return;

}

void CKeyBoard::Close()
{
	return;
}
#include "util/CJavaWrapper.h"
bool CKeyBoard::OnTouchEvent(int type, bool multi, int x, int y)
{

	if (!m_bEnable) return true;

}

void CKeyBoard::HandleInput(kbKey& key)
{
	
}

void CKeyBoard::AddCharToInput(char sym)
{
}

void CKeyBoard::DeleteCharFromInput()
{
	}

void CKeyBoard::Send()
{
}

kbKey* CKeyBoard::GetKeyFromPos(int x, int y)
{
	return nullptr;
}

void CKeyBoard::InitENG()
{
	return;
}

void CKeyBoard::InitRU()
{
	

	return;
}

void CKeyBoard::InitNUM()
{
	

	return;
}

void CKeyBoard::Flush()
{
	if (!m_sInput.length()) return;
}

void CKeyBoard::EnableNewKeyboard()
{
	m_bNewKeyboard = true;
}

void CKeyBoard::EnableOldKeyboard()
{
	m_bNewKeyboard = false;
}

bool CKeyBoard::IsNewKeyboard()
{
	return m_bNewKeyboard;
}

void CKeyBoard::ProcessInputCommands()
{
}

void CKeyBoard::OnNewKeyboardInput(JNIEnv* pEnv, jobject thiz, jbyteArray str)
{
	
}
