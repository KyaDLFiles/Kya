#include "FrontendDisp.h"
#include "edBankBuffer.h"
#include "edVideo/VideoB.h"
#include "edVideo/VideoD.h"
#include "Pause.h"
#include "edVideo/Viewport.h"

#include <string.h>
#include "edVideo/VideoA.h"
#include "ed3D.h"
#include "CameraViewManager.h"
#include "edStr.h"
#include "BootData.h"
#include "MathOps.h"
#include "FrontEndBank.h"
#include "FrontEndLife.h"
#include "TimeController.h"
#include "LevelScheduleManager.h"

struct Bank {
	Bank() {}
	edCBankBufferEntry* pAccessObj;
	edCBankBuffer bankData;
	void Init(int param_2, int size);
	bool BankLoad(char* path, char* fileName, edCBankCallback* pTypePairData);
	char* GetResource(char* fileName, edBANK_ENTRY_INFO* param_3);
	bool BankUnload();
	void Term();
};

void Bank::Init(int param_2, int size)
{
	edCBankInstall BStack32;

	memset(&BStack32, 0, sizeof(edCBankInstall));
	this->bankData.initialize(size, param_2, &BStack32);
	return;
}

bool Bank::BankLoad(char* path, char* fileName, edCBankCallback* pTypePairData)
{
	bool bVar1;
	edCBankBufferEntry* peVar2;
	edCBankInstall local_a0;
	char filePath[128];

	if (pTypePairData != (edCBankCallback*)0x0) {
		this->bankData.bank_buffer_setcb(pTypePairData);
	}

	memset(&local_a0, 0, sizeof(edCBankInstall));
	/* +.bnk */
	edStrCatMulti(filePath, path, fileName, ".bnk", 0);
	local_a0.filePath = filePath;
	peVar2 = this->bankData.get_free_entry();
	this->pAccessObj = peVar2;
	bVar1 = this->pAccessObj->load(&local_a0);
	return bVar1;
}

char* Bank::GetResource(char* fileName, edBANK_ENTRY_INFO* param_3)
{
	bool bVar1;
	int fileIndex;
	char* fileBuffer;

	fileIndex = this->pAccessObj->get_index(fileName);
	if (fileIndex == -1) {
		/* \r\nFile: %s\r\n */
		edDebugPrintf("\r\nFile: %s\r\n", fileName);
		fileBuffer = (char*)0x0;
	}
	else {
		fileBuffer = (char*)0x0;
		if (param_3 == (edBANK_ENTRY_INFO*)0x0) {
			fileBuffer = this->pAccessObj->get_element(fileIndex);
		}
		else {
			bVar1 = this->pAccessObj->get_info(fileIndex, param_3, (char*)0x0);
			if (bVar1 != false) {
				fileBuffer = param_3->fileBufferStart;
			}
		}
	}
	return fileBuffer;
}

bool Bank::BankUnload()
{
	bool bVar1;

	bVar1 = this->pAccessObj->close();
	this->pAccessObj = (edCBankBufferEntry*)0x0;
	return bVar1;
}

void Bank::Term()
{
	this->bankData.terminate();
	return;
}

char* g_FrontendSoundFiles_0040ec90[5] =
{
	"valid.vag",
	"changeSelection.vag",
	"changeValue.vag",
	"cancel.vag",
	"takeMoney.vag",
};

CFrontendLifeGauge gFrontendLifeGauge;
CFrontendMagicGauge gMagicGauge;
CFrontendAction gFrontendAction;
CFrontendMoney gFrontendMoney;

CFrontendDisplay::CFrontendDisplay()
{
	this->pViewport = (ed_viewport*)0x0;
	//FUN_001dcc10((long)&this->field_0x54);
	this->bHideHUD = 0;
	this->pHealthBar = &gFrontendLifeGauge;
	this->pMagicOrbs = &gMagicGauge;
	this->pMoney = &gFrontendMoney;
	//this->pFreedWolfun = &HNootiesWolfun_00452ae0;
	this->pFrontendAction = &gFrontendAction;
	//this->pMenuObj_0x6c = &MenuObj_Big_00452ea0;
	//this->pMenuObj_0x74 = (undefined*)&PTR_DAT_004544c0;
	//this->pMenuObj_0x70 = (undefined*)&PTR_DAT_00454b80;
	//this->field_0x78 = (char*)&FLOAT_00456220;
}

void CFrontendDisplay::Game_Init()
{
	CFrontendBank* pMVar1;
	char* soundFileBuffer;
	edVideo_Globals* peVar2;
	edSurface* pVidModeDataA;
	edSurface* pVidModeDataB;
	ed_viewport* pCVar3;
	char* pFileBuffer;
	undefined8 uVar4;
	char** fileToLoad;
	CSprite* pTexture;
	int iVar5;
	char** wolfenFileName;
	char* puVar6;
	Bank frontendBank;
	CameraObjParams local_8;

	puVar6 = this->field_0x78;
	frontendBank.Init(1, 0x6000);
	/* Init the frontend IOP Bnk */
	uVar4 = 0;
	frontendBank.BankLoad("CDEURO/Frontend/", "frntdiop", NULL);
	iVar5 = 0;
	fileToLoad = g_FrontendSoundFiles_0040ec90;
	do {
		soundFileBuffer = frontendBank.GetResource(*fileToLoad, (edBANK_ENTRY_INFO*)0x0);
		IMPLEMENTATION_GUARD_AUDIO(
			edSoundSampleLoad(soundFileBuffer, (ed_sound_sample*)(puVar6 + 4), 0);)
			iVar5 = iVar5 + 1;
		fileToLoad = fileToLoad + 1;
		puVar6 = puVar6 + 0x18;
	} while (iVar5 < 5);
	frontendBank.BankUnload();
	frontendBank.Term();

	peVar2 = edVideoGetInfo();
	local_8.posY = 0;
	local_8.posX = 0;
	local_8.screenWidth = peVar2->pActiveVidParams->screenWidth;
	local_8.screenHeight = peVar2->pActiveVidParams->screenHeight;
	pVidModeDataA = VideoGetDisplaybuffer();
	pVidModeDataB = VideoGetZbuffer();
	pCVar3 = edViewportNew(&local_8, pVidModeDataA, pVidModeDataB, 2);
	this->pViewport = pCVar3;
	edViewportSetBackgroundColor(this->pViewport, 0, 0, 0);
	CFrontend::_scene_handle = ed3DSceneCreate(&CCameraManager::_gFrontEndCamera, this->pViewport, 1);

	edF32Matrix4SetIdentityHard(&this->field_0x10);
	iVar5 = 0;
	wolfenFileName = MenuElementsBitmapNames;
	pTexture = MenuBitmaps;
	do {
		NAME_NEXT_OBJECT(*wolfenFileName);
		pTexture->Install(CScene::ptable.g_FrontEndBank_00451674->GetResource(*wolfenFileName, (edBANK_ENTRY_INFO*)0x0));
		iVar5 = iVar5 + 1;
		wolfenFileName = wolfenFileName + 1;
		pTexture = pTexture + 1;
	} while (iVar5 < 0xd);

	this->pHealthBar->Init();
	//(*(code*)(this->pMagicOrbs->pVTable->pVTable).pVTable.field_0x20)();
	//(*(code*)this->pNooties->pVTable->field_0x20)();
	//(*(code*)this->pFreedWolfun->pVTable->field_0x20)();
	//(*(code*)this->pMenuObj_0x68->field_0x0->field_0x20)();
	//(**(code**)(*(int*)this->pMenuObj_0x70 + 8))();
	//(*(code*)this->pMenuObj_0x6c->pVTable->field_0x20)();
	//(**(code**)(*(int*)this->pMenuObj_0x74 + 0x20))();

	if (this->bHideHUD == 0) {
		this->bHideHUD = 1;
		this->pMagicOrbs->Magic_SetDisplay(false);
	}
	return;
}

void CFrontendDisplay::Game_Term()
{
	return;
}

int Global_00448814;

void CFrontendDisplay::Level_Init()
{
	if (this->bHideHUD == 1) {
		this->bHideHUD = 0;
		this->pMagicOrbs->Magic_SetDisplay(true);
	}

	if (Global_00448814 != 0) {
		//CFrontendMagicGauge::ShowMagic(this->pMagicOrbs);
		this->pHealthBar->ShowLife();
		//FUN_00371cb0(this->pNooties);
		//FUN_00408c60(this->pFreedWolfun);
		Global_00448814 = 0;
	}

	//FUN_003cb180(this->pMenuObj_0x6c);
	//FUN_003c9370((long)(int)this->pMenuObj_0x74);

	//(*(code*)this->pManagerFunctionData[1].field_0x0)(this, 4, &this->field_0x54);

	ed3DSceneComputeCameraToScreenMatrix(CFrontend::_scene_handle, &this->field_0x10);

	return;
}

void CFrontendDisplay::Level_Term()
{
	//(*(code*)this->pManagerFunctionData[1].field_0x0)(this, 4, 0);

	if (this->bHideHUD == 0) {
		this->bHideHUD = 1;
		this->pMagicOrbs->Magic_SetDisplay(false);
	}

	//FUN_003cb000(this->pMenuObj_0x6c);
	//FUN_001d8220((int)this->pFrontendEnemyList);
	//FUN_003c9330((int)this->pMenuObj_0x74);

	return;
}

void CFrontendDisplay::Level_Manage()
{
	float fVar2;

	if (this->bHideHUD == 0) {
		fVar2 = Timer::GetTimer()->totalTime;
		//(*(code*)(this->pMagicOrbs->pVTable->pVTable).pVTable.Update)(fVar2);
		//(*(code*)this->pNooties->pVTable->Update)(fVar2);
		//(*(code*)this->pFreedWolfun->pVTable->Update)(fVar2);
		pHealthBar->Update(fVar2);
		//(*(code*)((this->pFrontendAction->base).pVTable)->Update)(fVar2);
		//FUN_001d8340(fVar2, (int*)this->pFrontendEnemyList);
		//(*(code*)this->pMenuObj_0x6c->pVTable->Update)(fVar2);
		//(**(code**)(*(int*)this->pMenuObj_0x74 + 0x38))(fVar2);
	}

	return;
}

void CFrontendDisplay::Level_ManagePaused()
{
	this->Level_Manage();

	return;
}

void CFrontendDisplay::Level_Draw()
{
	if (this->bHideHUD == 0) {
		//(**(code**)(*(int*)this->pMenuObj_0x74 + 0x3c))();
		this->pHealthBar->Draw();
		//(*(code*)(this->pMagicOrbs->pVTable->pVTable).pVTable.Draw)();
		//this->pMoney->Draw();
		//(*(code*)this->pFreedWolfun->pVTable->Draw)();
		//(*(code*)((this->pFrontendAction->base).pVTable)->Draw)();
		//FUN_001d89d0((int)this->pFrontendEnemyList);
		//(*(code*)this->pMenuObj_0x6c->pVTable->Draw)();
	}

	return;
}

void CFrontendDisplay::Level_Reset()
{
	this->pHealthBar->Reset();
	//(*(this->pMagicOrbs->pVTable->pVTable).pVTable.Reset)((CWidget*)this->pMagicOrbs);
	//this->pMoney->Reset();
	//(*this->pFreedWolfun->pVTable->Reset)((CWidget*)this->pFreedWolfun);
	//(*((this->pFrontendAction->base).pVTable)->Reset)((CWidget*)this->pFrontendAction);
	//(**(code**)(*(int*)this->pFrontendEnemyList + 0x10))();
	//(*this->pMenuObj_0x6c->pVTable->Reset)((CWidget*)this->pMenuObj_0x6c);
	//(**(code**)(*(int*)this->pMenuObj_0x74 + 0x18))();

	return;
}

void CFrontendDisplay::Level_CheckpointReset()
{
	this->pHealthBar->Reset();
	this->pHealthBar->ShowLife();

	//(*(this->pMagicOrbs->pVTable->pVTable).pVTable.Reset)((CWidget*)this->pMagicOrbs);
	//CFrontendMagicGauge::ShowMagic(this->pMagicOrbs);
	//(*this->pNooties->pVTable->Reset)((CWidget*)this->pNooties);
	//FUN_00371cb0(this->pNooties);
	//(*this->pFreedWolfun->pVTable->Reset)((CWidget*)this->pFreedWolfun);
	//FUN_00408c60(this->pFreedWolfun);
	//(*((this->pFrontendAction->base).pVTable)->Reset)((CWidget*)this->pFrontendAction);
	//(**(code**)(*(int*)this->pFrontendEnemyList + 0x10))();
	//(*(code*)this->pMenuObj_0x6c->pVTable->CheckpointReset)();
	//(**(code**)(*(int*)this->pMenuObj_0x74 + 0x1c))();

	return;
}

void CFrontendDisplay::Level_PauseChange(bool bPaused)
{
	if (bPaused == 0) {
		this->pHealthBar->HideLifeAlways();
		//CFrontendMagicGauge::HideMagicAlways(this->pMagicOrbs, 0);
		//CFrontendMoney::HideMoneyAlways(this->pNooties);
	}
	else {
		this->pHealthBar->ShowLifeAlways();
		//CFrontendMagicGauge::ShowMagicAlways((int)this->pMagicOrbs);
		//CFrontendMoney::ShowMoneyAlways((int)this->pNooties);
	}
	return;
}

void CFrontendDisplay::Level_LoadContext()
{
	if (CLevelScheduler::gThis->field_0x78 != 0) {
		//CFrontendMagicGauge::ShowMagic(this->pMagicOrbs);
		this->pHealthBar->ShowLife();
		//FUN_00371cb0(this->pNooties);
		//FUN_00408c60(this->pFreedWolfun);
	}

	return;
}

void CFrontendDisplay::DeclareInterface(FRONTEND_INTERFACE interfaceType, CInterface* pInterface)
{
	switch (interfaceType) {
	case FRONTEND_INTERFACE_LIFE:
		this->pHealthBar->SetInterface(pInterface);
		break;
	case FRONTEND_INTERFACE_MAGIC:
		//this->pMagicOrbs->SetInterface(pInterface);
		break;
	default:
		//IMPLEMENTATION_GUARD();
		break;
	}

	return;
}

void CFrontendDisplay::SetHeroActionA(int actionId)
{
	this->pFrontendAction->SetActionA(actionId);
}

void CFrontendDisplay::SetHeroActionB(int actionId)
{
	this->pFrontendAction->SetActionB(actionId);
}

void CFrontendDisplay::SetGlobalFunc_001cf8e0(void)
{
	Global_00448814 = 1;
	return;
}

void CFrontendDisplay::SetActive(bool bActive)
{
	if ((uint)this->bHideHUD == (bActive & 0xffU)) {
		this->bHideHUD = bActive == 0;
		this->pMagicOrbs->Magic_SetDisplay(bActive);
	}
	return;
}

void CFrontendMagicGauge::Magic_SetDisplay(unsigned char bNewVisible)
{
	if (bNewVisible != this->bVisible) {
		this->bVisible = bNewVisible;
	}
	return;
}
