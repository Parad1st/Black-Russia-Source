#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "../gui/gui.h"
#include "../util/CJavaWrapper.h"

extern CGUI* pGUI;

RwTexture* splashTexture = nullptr;

#define COLOR_WHITE		0xFFFFFFFF
#define COLOR_BLACK 	0xFF000000
#define COLOR_ORANGE 	0xFFFAA500
#define COLOR_PURPLE	0xFFCE39DF
#define COLOR_ROSE		0xFFFF99FF
#define COLOR_GREY		0xFF5d5d5d
#define COLOR_BRED		0xFF9933FF
#define COLOR_BLUE		0xFF6C2713
#define COLOR_CYAN		0xFFCE6816
#define COLOR_1			0xFFB58891
#define COLOR_2			0xFF673F40
#define COLOR_PIZDEC	0xFFff008b

struct stRect
{
	int x1;	// left
	int y1;	// top
	int x2;	// right
	int y2;	// bottom
};

struct stfRect
{
	float x1;
	float y1;
	float x2;
	float y2;
};

#define MAX_SCHEMAS 4
uint32_t colors[MAX_SCHEMAS][2] = {
	{ COLOR_BLACK,	COLOR_WHITE },
	{ COLOR_BLACK, 	COLOR_ORANGE },
	{ COLOR_BLACK,	COLOR_PIZDEC },
	{ COLOR_BLACK,	COLOR_BLUE }
};
unsigned int color_scheme = 0;

void LoadSplashTexture()
{
	//Log("Loading splash texture..");
	//splashTexture = (RwTexture*)LoadTextureFromDB("samp", "br-screen");

	//color_scheme = 1;
}

void Draw(stRect* rect, uint32_t color, RwRaster* raster = nullptr, stfRect* uv = nullptr)
{
	/*static RwIm2DVertex vert[4];
	const RwReal nearScreenZ = *(RwReal*)(g_libGTASA + 0x9DAA60);	// CSprite2d::NearScreenZ
	const RwReal recipNearClip = *(RwReal*)(g_libGTASA + 0x9DAA64);	// CSprite2d::RecipNearClip

	RwIm2DVertexSetScreenX(&vert[0], rect->x1);
	RwIm2DVertexSetScreenY(&vert[0], rect->y2);
	RwIm2DVertexSetScreenZ(&vert[0], nearScreenZ);
	RwIm2DVertexSetRecipCameraZ(&vert[0], recipNearClip);
	vert[0].emissiveColor = color;
	RwIm2DVertexSetU(&vert[0], uv ? uv->x1 : 0.0f, recipNearClip);
	RwIm2DVertexSetV(&vert[0], uv ? uv->y2 : 0.0f, recipNearClip);

	RwIm2DVertexSetScreenX(&vert[1], rect->x2);
	RwIm2DVertexSetScreenY(&vert[1], rect->y2);
	RwIm2DVertexSetScreenZ(&vert[1], nearScreenZ);
	RwIm2DVertexSetRecipCameraZ(&vert[1], recipNearClip);
	vert[1].emissiveColor = color;
	RwIm2DVertexSetU(&vert[1], uv ? uv->x2 : 0.0f, recipNearClip);
	RwIm2DVertexSetV(&vert[1], uv ? uv->y2 : 0.0f, recipNearClip);

	RwIm2DVertexSetScreenX(&vert[2], rect->x1);
	RwIm2DVertexSetScreenY(&vert[2], rect->y1);
	RwIm2DVertexSetScreenZ(&vert[2], nearScreenZ);
	RwIm2DVertexSetRecipCameraZ(&vert[2], recipNearClip);
	vert[2].emissiveColor = color;
	RwIm2DVertexSetU(&vert[2], uv ? uv->x1 : 0.0f, recipNearClip);
	RwIm2DVertexSetV(&vert[2], uv ? uv->y1 : 0.0f, recipNearClip);

	RwIm2DVertexSetScreenX(&vert[3], rect->x2);
	RwIm2DVertexSetScreenY(&vert[3], rect->y1);
	RwIm2DVertexSetScreenZ(&vert[3], nearScreenZ);
	RwIm2DVertexSetRecipCameraZ(&vert[3], recipNearClip);
	vert[3].emissiveColor = color;
	RwIm2DVertexSetU(&vert[3], uv ? uv->x2 : 0.0f, recipNearClip);
	RwIm2DVertexSetV(&vert[3], uv ? uv->y1 : 0.0f, recipNearClip);

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)raster);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vert, 4);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)0);*/
}


void RenderSplash()
{
	/*stRect rect;
	stfRect uv;

	// background
	rect.x1 = 0;
	rect.y1 = 0;
	rect.x2 = RsGlobal->maximumWidth;
	rect.y2 = RsGlobal->maximumHeight;
	Draw(&rect, colors[color_scheme][0]);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)1);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

	// texture
	rect.x1 = 0;
	rect.y1 = 0;
	rect.x2 = RsGlobal->maximumWidth;
	rect.y2 = RsGlobal->maximumHeight;
	uv.x1 = 0.0f;
	uv.y1 = 0.0f;
	uv.x2 = 1.0;
	uv.y2 = 1.0;
	if (splashTexture)
	{
		Draw(&rect, COLOR_WHITE, splashTexture->raster, &uv);
	}*/

	const float percent = *(float*)(g_libGTASA + 0x8F08C0);
	if (percent <= 0.0f) return;
	float mult = percent / 100.0f;

	int intMult = (int)percent;

	if (intMult == 51) intMult = 100;

	g_pJavaWrapper->UpdateSplash(intMult);

	/*rect.x1 = RsGlobal->maximumWidth * 0.05f;
	rect.y1 = RsGlobal->maximumHeight * 0.95f;
	rect.x2 = (RsGlobal->maximumWidth * 0.95f) * mult;
	rect.y2 = RsGlobal->maximumHeight * 0.97f;

	Draw(&rect, colors[color_scheme][1]);*/
}

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);
void ImGui_ImplRenderWare_NewFrame();

void RenderSplashScreen()
{
	RenderSplash();
	g_pJavaWrapper->ShowSplash();

	if (!pGUI) return;

	ImGui_ImplRenderWare_NewFrame();
	ImGui::NewFrame();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());
}