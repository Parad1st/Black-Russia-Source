#include "../main.h"
#include "game.h"
#include "util.h"
#include "RW/RenderWare.h"
#include "../gui/gui.h"

extern CGUI* pGUI;
extern CSnapShotHelper* pSnapShotHelper;

extern uintptr_t TextDrawTexture[];

// 0.3.7
CTextDraw::CTextDraw(TEXT_DRAW_TRANSMIT* pTextDrawTransmit, const char* szText)
{
	memset(&m_TextDrawData, 0, sizeof(TEXT_DRAW_DATA));

	m_TextDrawData.fLetterWidth = pTextDrawTransmit->fLetterWidth;
	m_TextDrawData.fLetterHeight = pTextDrawTransmit->fLetterHeight;

	m_TextDrawData.dwLetterColor = pTextDrawTransmit->dwLetterColor;
	m_TextDrawData.byteUnk12 = 0;
	m_TextDrawData.byteCentered = pTextDrawTransmit->byteCenter;
	m_TextDrawData.byteBox = pTextDrawTransmit->byteBox;

	m_TextDrawData.fLineWidth = pTextDrawTransmit->fLineWidth;
	m_TextDrawData.fLineHeight = pTextDrawTransmit->fLineHeight;

	m_TextDrawData.dwBoxColor = pTextDrawTransmit->dwBoxColor;
	m_TextDrawData.byteProportional = pTextDrawTransmit->byteProportional;
	m_TextDrawData.dwBackgroundColor = pTextDrawTransmit->dwBackgroundColor;
	m_TextDrawData.byteShadow = pTextDrawTransmit->byteShadow;
	m_TextDrawData.byteOutline = pTextDrawTransmit->byteOutline;
	m_TextDrawData.byteAlignLeft = pTextDrawTransmit->byteLeft;
	m_TextDrawData.byteAlignRight = pTextDrawTransmit->byteRight;
	m_TextDrawData.dwStyle = pTextDrawTransmit->byteStyle;

	m_TextDrawData.fX = pTextDrawTransmit->fX;
	m_TextDrawData.fY = pTextDrawTransmit->fY;

	m_TextDrawData.dwParam1 = 0xFFFFFFFF;
	m_TextDrawData.dwParam2 = 0xFFFFFFFF;
	m_TextDrawData.byteSelectable = pTextDrawTransmit->byteSelectable;
	m_TextDrawData.wModelID = pTextDrawTransmit->wModelID;
	m_TextDrawData.vecRot.X = pTextDrawTransmit->vecRot.X;
	m_TextDrawData.vecRot.Y = pTextDrawTransmit->vecRot.Y;
	m_TextDrawData.vecRot.Z = pTextDrawTransmit->vecRot.Z;
	m_TextDrawData.fZoom = pTextDrawTransmit->fZoom;
	m_TextDrawData.wColor1 = pTextDrawTransmit->wColor1;
	m_TextDrawData.wColor2 = pTextDrawTransmit->wColor2;
	m_TextDrawData.bHasKeyCode = false;
	m_TextDrawData.iTextureSlot = -1;
	SetText(szText);

	if (m_TextDrawData.dwStyle == 4) {
		m_TextDrawData.iTextureSlot = GetFreeTextDrawTextureSlot();
		LoadTexture();
	}

	m_TextDrawData.bHasRectArea = false;
	m_rectArea.fLeft = 0.0f;
	m_rectArea.fRight = 0.0f;
	m_rectArea.fBottom = 0.0f;
	m_rectArea.fTop = 0.0f;
	m_bHovered = false;
	m_dwHoverColor = 0;

}
// 0.3.7
CTextDraw::~CTextDraw()
{
	DestroyTextDrawTexture(m_TextDrawData.iTextureSlot);
}

void CTextDraw::Draw()
{
	if (m_TextDrawData.iTextureSlot != -1) {
		return DrawTextured();
	}

	if (m_TextDrawData.dwStyle == 4) {
		return DrawTextured();
	}

	return DrawDefault();
}

void CTextDraw::DrawDefault()
{
	if (!m_szText || !strlen(m_szText)) return;

	//CFont::AsciiToGxtChar(m_szText, m_gxtString);
	int iScreenWidth = RsGlobal->maximumWidth;
	int iScreenHeight = RsGlobal->maximumHeight;
	float fHorizHudScale = 1.0f / 640.0f;
	float fVertHudScale = 1.0f / 448.0f;

	float fScaleY = (float)iScreenHeight * fVertHudScale * m_TextDrawData.fLetterHeight * 0.5f;
	float fScaleX = (float)iScreenWidth * fHorizHudScale * m_TextDrawData.fLetterWidth;

	CFont::SetScale(fScaleX, fScaleY);

	if (m_bHovered)
	{
		uint32_t dwReversed = __builtin_bswap32(m_dwHoverColor | (0x000000FF));
		CFont::SetColor(&dwReversed);
	}
	else
	{
		CFont::SetColor(&m_TextDrawData.dwLetterColor);
	}

	CFont::SetJustify(0);

	if (m_TextDrawData.byteAlignRight) CFont::SetOrientation(2);
	else if (m_TextDrawData.byteCentered) CFont::SetOrientation(0);
	else CFont::SetOrientation(1);

	float fLineWidth = iScreenWidth * m_TextDrawData.fLineWidth * fHorizHudScale;
	CFont::SetWrapX(fLineWidth);

	float fLineHeight = iScreenWidth * m_TextDrawData.fLineHeight * fHorizHudScale;
	CFont::SetCentreSize(fLineHeight);

	CFont::SetBackground(m_TextDrawData.byteBox, 0);
	CFont::SetBackgroundColor(&m_TextDrawData.dwBoxColor);
	CFont::SetProportional(m_TextDrawData.byteProportional);
	CFont::SetDropColor(&m_TextDrawData.dwBackgroundColor);

	if (m_TextDrawData.byteOutline) {
		CFont::SetEdge(m_TextDrawData.byteOutline);
	}
	else {
		//CFont::SetEdge(m_TextDrawData.byteShadow);
		CFont::SetDropShadowPosition(m_TextDrawData.byteShadow);
	}

	CFont::SetFontStyle(m_TextDrawData.dwStyle);

	if (m_TextDrawData.bHasKeyCode) {
		//  CMessages_InsertPlayerControlKeysInString(m_szString);
	}

	float fUseY = iScreenHeight - ((448.0 - m_TextDrawData.fY) * (iScreenHeight * fVertHudScale));
	float fUseX = iScreenWidth - ((640.0 - m_TextDrawData.fX) * (iScreenWidth * fHorizHudScale));
	CFont::PrintString(fUseX, fUseY, (const char*)m_szText);

	CFont::SetEdge(0);

	if (m_TextDrawData.byteAlignRight)
	{
		m_rectArea.fLeft = fUseX - (fLineWidth - fUseX);
		m_rectArea.fRight = fUseX;
		m_rectArea.fBottom = fUseY;
		m_rectArea.fTop = fUseY + fLineHeight;
	}
	else if (m_TextDrawData.byteCentered)
	{
		m_rectArea.fLeft = fUseX - (fLineHeight * 0.5f);
		m_rectArea.fRight = m_rectArea.fLeft + fLineHeight;
		m_rectArea.fBottom = fUseY;
		m_rectArea.fTop = fUseY + fLineWidth;
	}
	else
	{
		m_rectArea.fLeft = fUseX;
		m_rectArea.fRight = fLineWidth - fUseX + fUseX;
		m_rectArea.fBottom = fUseY;
		m_rectArea.fTop = fUseY + fLineHeight;
	}
}

void CTextDraw::DrawTextured()
{
	float scaleX = (float)(RsGlobal->maximumWidth) * (1.0f / 640.0f);
	float scaleY = (float)(RsGlobal->maximumHeight) * (1.0f / 448.0f);

	m_rectArea.fLeft = m_TextDrawData.fX * scaleX;
	m_rectArea.fBottom = m_TextDrawData.fY * scaleY;
	m_rectArea.fRight = (m_TextDrawData.fX + m_TextDrawData.fLineWidth) * scaleX;
	m_rectArea.fTop = (m_TextDrawData.fY + m_TextDrawData.fLineHeight) * scaleY;

	static float uv_reflected[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f };

	static float uv_normal[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f };

	RECT rect;
	rect.fLeft = m_rectArea.fLeft;
	rect.fBottom = m_rectArea.fTop;
	rect.fRight = m_rectArea.fRight;
	rect.fTop = m_rectArea.fBottom;
	if (m_bHovered) {
		DrawTextureUV(TextDrawTexture[m_TextDrawData.iTextureSlot], &rect, m_dwHoverColor,
			m_TextDrawData.dwStyle == 5 ? uv_reflected : uv_normal);
	}
	else {
		DrawTextureUV(TextDrawTexture[m_TextDrawData.iTextureSlot], &rect, m_TextDrawData.dwLetterColor,
			m_TextDrawData.dwStyle == 5 ? uv_reflected : uv_normal);
	}
}

void CTextDraw::SetText(const char* szText)
{
	memset(m_szText, 0, 800);
	strncpy(m_szText, szText, 800);
	m_szText[800] = 0;

	if (m_TextDrawData.dwStyle == 4 && m_TextDrawData.iTextureSlot != -1)
	{
		DestroyTextDrawTexture(m_TextDrawData.iTextureSlot);
		LoadTexture();
	}
}

void CTextDraw::LoadTexture()
{
	char txdname[64 + 1];
	memset(txdname, 0, sizeof(txdname));
	char texturename[64 + 1];
	memset(texturename, 0, sizeof(texturename));

	char* szTexture = strchr(m_szText, ':');
	if (szTexture == nullptr) return;

	if (strlen(m_szText) < 64 && strchr(m_szText, '\\') == nullptr && strchr(m_szText, '/') == nullptr)
	{
		strncpy(txdname, m_szText, (size_t)(szTexture - m_szText));
		strcpy(texturename, ++szTexture);

		if (m_TextDrawData.iTextureSlot != -1) {
			TextDrawTexture[m_TextDrawData.iTextureSlot] = LoadTextureFromDB("txd", texturename);
		}
	}
}

void CTextDraw::SnapshotProcess()
{
	if (m_TextDrawData.dwStyle != 5 || m_TextDrawData.iTextureSlot != -1) {
		return;
	}

	uintptr_t snapshot = 0;

	// PED MODEL
	if (IsPedModel(m_TextDrawData.wModelID))
	{
		/*if (!IsExistinfoForModel(m_TextDrawData.wModelID))
			m_TextDrawData.wModelID = 0;*/

		snapshot = pSnapShotHelper->CreatePedSnapShot(
			m_TextDrawData.wModelID,
			m_TextDrawData.dwBackgroundColor,
			&m_TextDrawData.vecRot,
			m_TextDrawData.fZoom);
	}
	// VEHICLE MODEL
	else if (m_TextDrawData.wModelID >= 400 && m_TextDrawData.wModelID <= 611)
	{
		snapshot = pSnapShotHelper->CreateVehicleSnapShot(
			m_TextDrawData.wModelID,
			m_TextDrawData.dwBackgroundColor,
			&m_TextDrawData.vecRot,
			m_TextDrawData.fZoom,
			m_TextDrawData.wColor1,
			m_TextDrawData.wColor2
		);
	}
	// OBJECT MODEL
	else
	{
		if (!GetModelInfoByID(m_TextDrawData.wModelID))
			m_TextDrawData.wModelID = 18631;
		snapshot = pSnapShotHelper->CreateObjectSnapShot(
			m_TextDrawData.wModelID,
			m_TextDrawData.dwBackgroundColor,
			&m_TextDrawData.vecRot,
			m_TextDrawData.fZoom
		);
	}

	if (snapshot)
	{
		m_TextDrawData.iTextureSlot = GetFreeTextDrawTextureSlot();
		TextDrawTexture[m_TextDrawData.iTextureSlot] = snapshot;
	}
}