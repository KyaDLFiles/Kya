#include "ActorWolfen.h"
#include "MemoryStream.h"
#include "ActorHero.h"
#include "ActorCommander.h"
#include "ActorWeapon.h"
#include "CinematicManager.h"
#include "WayPoint.h"
#include "MathOps.h"
#include "EventManager.h"
#include "ActorProjectile.h"

WolfenConfig CActorWolfen::_gStateCfg_WLF[68]
{
	WolfenConfig(0x0, 0x104, 0x5),
	WolfenConfig(0x6, 0x4, 0x1),
	WolfenConfig(0xA3, 0x904, 0xD),
	WolfenConfig(0x0, 0x104, 0x1),
	WolfenConfig(0x8D, 0x104, 0x45),
	WolfenConfig(0x6, 0x4, 0x1),
	WolfenConfig(0x6, 0x4, 0x1),
	WolfenConfig(0xFFFFFFFF, 0x1, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x1, 0x0),
	WolfenConfig(0x87, 0x1, 0x0),
	WolfenConfig(0x87, 0x1, 0x0),
	WolfenConfig(0x87, 0x1, 0x0),
	WolfenConfig(0x87, 0x1, 0x0),
	WolfenConfig(0x87, 0x1, 0x0),
	WolfenConfig(0x0, 0x1, 0x0),
	WolfenConfig(0x7, 0x0, 0x0),
	WolfenConfig(0x9C, 0x4, 0x0),
	WolfenConfig(0x9C, 0x4, 0x1),
	WolfenConfig(0x9C, 0x4, 0x1),
	WolfenConfig(0x9C, 0x4, 0x5),
	WolfenConfig(0x9D, 0x4, 0x1),
	WolfenConfig(0x9E, 0x4, 0x5),
	WolfenConfig(0x9F, 0x4, 0x1),
	WolfenConfig(0xA0, 0x4, 0x1),
	WolfenConfig(0x82, 0x4, 0x0),
	WolfenConfig(0x83, 0x4, 0x0),
	WolfenConfig(0x7, 0x4, 0x1),
	WolfenConfig(0x0, 0x4, 0x1),
	WolfenConfig(0x0, 0x4, 0x1),
	WolfenConfig(0x0, 0x4, 0x1),
	WolfenConfig(0x0, 0x4, 0x1),
	WolfenConfig(0xA9, 0x2, 0x51),
	WolfenConfig(0xAA, 0x4, 0x51),
	WolfenConfig(0xAA, 0x4, 0x55),
	WolfenConfig(0xAA, 0x2, 0x15),
	WolfenConfig(0xA8, 0x0, 0x55),
	WolfenConfig(0xAD, 0x0, 0x1),
	WolfenConfig(0xAE, 0x0, 0x1),
	WolfenConfig(0x88, 0x0, 0x0),
	WolfenConfig(0x89, 0x0, 0x4),
	WolfenConfig(0x8B, 0x2, 0x2),
	WolfenConfig(0x8C, 0x2, 0x3),
	WolfenConfig(0x84, 0x0, 0x23),
	WolfenConfig(0x8E, 0x2, 0x6),
	WolfenConfig(0x8D, 0x0, 0x2),
	WolfenConfig(0x8F, 0x0, 0x6),
	WolfenConfig(0x91, 0x0, 0x2),
	WolfenConfig(0x92, 0x4, 0x0),
	WolfenConfig(0x93, 0x4, 0x0),
	WolfenConfig(0x6, 0x4, 0x0),
	WolfenConfig(0x0, 0x4, 0x4),
	WolfenConfig(0x94, 0x4, 0x0),
	WolfenConfig(0xA4, 0x4, 0x1),
	WolfenConfig(0xA5, 0x4, 0x1),
	WolfenConfig(0xA6, 0x4, 0x0),
	WolfenConfig(0x6, 0x4, 0x0),
	WolfenConfig(0x6, 0x4, 0x1),
	WolfenConfig(0x84, 0x4, 0x21),
	WolfenConfig(0x94, 0x4, 0x4),
	WolfenConfig(0xFFFFFFFF, 0x900, 0x0),
	WolfenConfig(0xA7, 0x2000101, 0x22),
	WolfenConfig(0xFFFFFFFF, 0x2000101, 0x22),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
	WolfenConfig(0xFFFFFFFF, 0x0, 0x0),
};

bool CActorWolfen::IsKindOfObject(ulong kind)
{
	return (kind & 0x1f) != 0;
}

WolfenCollisionSphere CActorWolfen::_pDefCollisions[4] =
{
	{ { 0.4f, 0.8f, 0.4f, 0.0f }, { 0.0f, 0.8f, 0.0f, 1.0f } },
	{ { 0.5f, 1.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.6f, 1.0f, 0.6f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ { 0.6f, 1.0f, 0.6f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
};

void CActorWolfen::Create(ByteCode* pByteCode)
{
	CCollision* pCVar1;
	edColPRIM_OBJECT* peVar2;
	uint uVar3;
	CVision* pPerception;
	EnemyComponent80* pEVar4;
	edF32VECTOR4* peVar5;
	int iVar6;
	float fVar7;

	this->field_0xb80 = 0;
	CActorFighter::Create(pByteCode);
	this->field_0xb74 = pByteCode->GetU32();

	assert(this->field_0xb74 < 4); // Need to ensure that there is valid table entries below for higher values.
	ChangeCollisionSphere(0.0f, &_pDefCollisions[this->field_0xb74].field_0x0, &_pDefCollisions[this->field_0xb74].field_0x10);
	StoreCollisionSphere();

	GetVision()->Create(this, pByteCode);

	//uVar3 = this->field_0xb74;
	//if ((uVar3 == 1) || (uVar3 == 0)) {
	//	if (g_EnemyComponent80_0049c6d0.field_0x0 == (StaticEnemy90*)0x0 || g_EnemyComponent80_0049c6d0.field_0x4 == 0) {
	//		(*(code*)(g_EnemyComponent80_0049c6d0.pVTable)->setObjCounts)(0x49c6d0, 4, 0);
	//		INT_004497e8 = 0;
	//	}
	//	this->pEnemyComponent80_0xd34 = &g_EnemyComponent80_0049c6d0;
	//	INT_004497e8 = INT_004497e8 + 1;
	//}
	//else {
	//	if (uVar3 == 3) {
	//		pEVar4 = (EnemyComponent80*)operator.new(0x80);
	//		if (pEVar4 != (EnemyComponent80*)0x0) {
	//			EnemyComponent80::Constructor_003fdca0(pEVar4);
	//			pEVar4->pVTable = &EnemyComponent80::VTable_004460b0;
	//		}
	//		this->pEnemyComponent80_0xd34 = pEVar4;
	//		(*(code*)this->pEnemyComponent80_0xd34->pVTable->setObjCounts)(this->pEnemyComponent80_0xd34, 0x1a, 0x1d);
	//		(*(code*)this->pEnemyComponent80_0xd34->pVTable->setupObjects)(this->pEnemyComponent80_0xd34, this);
	//	}
	//	else {
	//		this->pEnemyComponent80_0xd34 = (EnemyComponent80*)0x0;
	//	}
	//}

	this->field_0xb8c = pByteCode->GetS32();

	this->hearingDetectionProps.Create(pByteCode);
	this->visionDetectionProps.Create(pByteCode);

	this->field_0xcf4 = pByteCode->GetF32();

	this->walkSpeed = pByteCode->GetF32();
	this->walkAcceleration = pByteCode->GetF32();
	this->walkRotSpeed = pByteCode->GetF32();

	this->field_0xc6c = pByteCode->GetF32();
	fVar7 = pByteCode->GetF32();
	this->field_0xc5c = fVar7;

	this->runSpeed = fVar7;
	this->runAcceleration = pByteCode->GetF32();
	this->rotRunSpeed = pByteCode->GetF32();

	uVar3 = pByteCode->GetU32();
	if ((int)uVar3 < 0) {
		fVar7 = (float)(uVar3 >> 1 | uVar3 & 1);
		fVar7 = fVar7 + fVar7;
	}
	else {
		fVar7 = (float)uVar3;
	}
	this->field_0xb84 = fVar7;

	this->field_0xd30 = pByteCode->GetU32();
	//uVar3 = this->nbComboRoots;
	//if (uVar3 == 0) {
	//	this->field_0xbe8 = (edF32VECTOR4*)0x0;
	//}
	//else {
	//	this->count_0xbd4 = uVar3;
	//	if (this->count_0xbd4 == 0) {
	//		this->field_0xbd0 = (edF32VECTOR4*)0x0;
	//	}
	//	else {
	//		peVar5 = (edF32VECTOR4*)operator.new.array((long)(this->count_0xbd4 << 4));
	//		this->field_0xbd0 = peVar5;
	//	}
	//	peVar5 = (edF32VECTOR4*)operator.new.array((long)(int)(this->nbComboRoots << 4));
	//	this->field_0xbe8 = peVar5;
	//}

	this->field_0xb90 = pByteCode->GetF32();
	this->field_0xb94 = pByteCode->GetF32();
	this->field_0xb98 = pByteCode->GetF32();
	this->field_0xd1c = 10.0f;
	this->field_0xd20 = 20.0f;

	Create_FightParam(pByteCode);

	this->pTargetActor_0xc80 = (CActorFighter*)0x0;
	this->pCommander = (CActorCommander*)0x0;
	this->combatFlags_0xb78 = 0;
	this->combatMode_0xb7c = ECM_None;

	pCVar1 = this->pCollisionData;
	if (pCVar1 == (CCollision*)0x0) {
		this->field_0xcf0 = 1.5f;
	}
	else {
		peVar2 = pCVar1->pObbPrim;
		this->field_0xcf0 = (peVar2->field_0xb0).y + (peVar2->field_0x90).y * 0.6f;
	}

	SkipToNextActorNoBase(pByteCode);

	return;
}

void CActorWolfen::Init()
{
	int* piVar1;
	CShadow* pCVar2;
	s_fighter_combo* psVar3;
	CLifeInterface* pCVar4;
	int iVar5;
	uint uVar6;
	int iVar7;
	int iVar8;
	uint uVar9;
	edF32VECTOR4* peVar10;
	char* pcVar11;
	int iVar12;
	float fVar13;
	float fVar14;
	float fVar15;
	float fVar16;
	float fVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	float fVar21;
	float fVar22;
	float fVar23;
	float fVar24;
	float fVar25;
	float fVar26;
	undefined4 uVar27;

	CActorFighter::Init();
	this->combatFlags_0xb78 = 0;

	//pCVar2 = this->pShadow;
	//if (pCVar2 != (CShadow*)0x0) {
	//	(pCVar2->base).field_0x48 = (float)&DAT_3f4ccccd;
	//	(this->pShadow)->field_0x50 = (float)&DAT_3f4ccccd;
	//}
	//if (PTR_DAT_004574a0 == (undefined*)0x0) {
	//	FUN_001fbaa0((int*)&PTR_DAT_004574a0);
	//}
	uVar6 = this->nbComboRoots;
	uVar9 = 0;
	fVar17 = 3.402823e+38;
	fVar13 = -3.402823e+38;
	fVar14 = fVar17;
	fVar24 = fVar17;
	fVar15 = fVar17;
	fVar18 = fVar17;
	fVar22 = fVar13;
	fVar25 = fVar13;
	if (uVar6 != 0) {
		iVar7 = 0;
		iVar5 = 0;
		fVar19 = fVar13;
		fVar16 = fVar13;
		fVar20 = fVar13;
		fVar26 = fVar13;
		//do {
		//	fVar23 = 0.0;
		//	fVar21 = 0.0;
		//	iVar8 = (int)&(this->aCombos)->field_0x0 + iVar7;
		//	fVar25 = *(float*)(*(int*)(iVar8 + 0x18) + 0x50);
		//	fVar22 = *(float*)(*(int*)(iVar8 + 0x18) + 0x4c);
		//	do {
		//		piVar1 = (int*)(iVar8 + 0x18);
		//		fVar23 = fVar23 + *(float*)(*piVar1 + 0x14);
		//		if (*(int*)(iVar8 + 0x20) == 0) {
		//			iVar8 = 0;
		//			fVar25 = fVar25 + *(float*)(*piVar1 + 0x54);
		//		}
		//		else {
		//			iVar8 = **(int**)(iVar8 + 0x1c);
		//		}
		//		fVar21 = fVar21 + 1.0;
		//	} while (iVar8 != 0);
		//	*(float*)((int)&this->field_0xbe8->x + iVar5) = fVar25;
		//	*(float*)((int)&this->field_0xbe8->y + iVar5) = fVar22;
		//	*(float*)((int)&this->field_0xbe8->z + iVar5) = fVar23;
		//	*(float*)((int)&this->field_0xbe8->w + iVar5) = fVar21;
		//	fVar13 = fVar22;
		//	if (fVar22 <= fVar19) {
		//		fVar13 = fVar19;
		//	}
		//	if (fVar18 <= fVar22) {
		//		fVar22 = fVar18;
		//	}
		//	fVar18 = fVar22;
		//	fVar22 = fVar25;
		//	if (fVar25 <= fVar20) {
		//		fVar22 = fVar20;
		//	}
		//	if (fVar15 <= fVar25) {
		//		fVar25 = fVar15;
		//	}
		//	fVar15 = fVar25;
		//	fVar25 = fVar23;
		//	if (fVar23 <= fVar26) {
		//		fVar25 = fVar26;
		//	}
		//	if (fVar14 <= fVar23) {
		//		fVar23 = fVar14;
		//	}
		//	fVar14 = fVar23;
		//	fVar20 = fVar21;
		//	if (fVar21 <= fVar16) {
		//		fVar20 = fVar16;
		//	}
		//	if (fVar21 < fVar24) {
		//		uVar6 = this->nbComboRoots;
		//		fVar24 = fVar21;
		//	}
		//	else {
		//		uVar6 = this->nbComboRoots;
		//	}
		//	uVar9 = uVar9 + 1;
		//	iVar7 = iVar7 + 0x24;
		//	iVar5 = iVar5 + 0x10;
		//	fVar19 = fVar13;
		//	fVar16 = fVar20;
		//	fVar20 = fVar22;
		//	fVar26 = fVar25;
		//} while (uVar9 < uVar6);
	}
	uVar9 = 0;
	fVar19 = fVar17;
	if (uVar6 != 0) {
		//iVar8 = 0;
		//iVar7 = 0;
		//iVar5 = 0;
		//fVar16 = fVar17;
		//fVar20 = fVar17;
		//do {
		//	psVar3 = this->aCombos;
		//	fVar26 = 1.0;
		//	if (fVar13 - fVar18 != 0.0) {
		//		fVar26 = (*(float*)((int)&this->field_0xbe8->y + iVar7) - fVar18) / (fVar13 - fVar18);
		//	}
		//	fVar23 = 1.0;
		//	if (fVar22 - fVar15 != 0.0) {
		//		fVar23 = 1.0 - (*(float*)((int)&this->field_0xbe8->x + iVar7) - fVar15) / (fVar22 - fVar15);
		//	}
		//	uVar27 = 0x3f800000;
		//	fVar21 = 1.0;
		//	if (fVar25 - fVar14 == 0.0) {
		//		peVar10 = this->field_0xbe8;
		//	}
		//	else {
		//		fVar21 = (*(float*)((int)&this->field_0xbe8->z + iVar7) - fVar14) / (fVar25 - fVar14);
		//		peVar10 = this->field_0xbe8;
		//	}
		//	iVar12 = (int)&peVar10->x + iVar7;
		//	if (fVar24 != *(float*)(iVar12 + 0xc)) {
		//		uVar27 = 0;
		//	}
		//	fVar17 = fVar16;
		//	fVar19 = fVar20;
		//	if (0.5 <= fVar26) {
		//		fVar17 = *(float*)(iVar12 + 4);
		//		if (fVar16 <= *(float*)(iVar12 + 4)) {
		//			fVar17 = fVar16;
		//		}
		//	}
		//	else {
		//		fVar19 = *(float*)(iVar12 + 4);
		//		if (fVar20 <= fVar19) {
		//			fVar19 = fVar20;
		//		}
		//	}
		//	*(float*)(iVar12 + 4) = fVar26;
		//	*(float*)((int)&this->field_0xbe8->x + iVar7) = fVar23;
		//	*(float*)((int)&this->field_0xbe8->z + iVar7) = fVar21;
		//	*(undefined4*)((int)&this->field_0xbe8->w + iVar7) = uVar27;
		//	*(int*)((int)&this->field_0xbd0->w + iVar5) = (int)&psVar3->field_0x0 + iVar8;
		//	this->field_0xbd8 = 0;
		//	*(float*)((int)&this->field_0xbd0->y + iVar5) = fVar23 + 0.3;
		//	this->field_0xbd8 = 0;
		//	pcVar11 = (char*)((int)&this->field_0xbd0->x + iVar5);
		//	if (*pcVar11 != '\x01') {
		//		*pcVar11 = '\x01';
		//		this->field_0xbd8 = 0;
		//	}
		//	uVar9 = uVar9 + 1;
		//	iVar5 = iVar5 + 0x10;
		//	iVar8 = iVar8 + 0x24;
		//	iVar7 = iVar7 + 0x10;
		//	fVar16 = fVar17;
		//	fVar20 = fVar19;
		//} while (uVar9 < this->nbComboRoots);
	}
	//this->field_0xbe4 = 2;
	//uVar6 = 0;
	//if (this->count_0xbd4 != 0) {
	//	iVar5 = 0;
	//	do {
	//		uVar6 = uVar6 + 1;
	//		*(undefined*)((int)&this->field_0xbd0->x + iVar5) = 1;
	//		iVar5 = iVar5 + 0x10;
	//	} while (uVar6 < (uint)this->count_0xbd4);
	//}
	//this->field_0xbe0 = 0;
	//this->field_0xbd8 = 0;
	//if (fVar13 - fVar18 == 0.0) {
	//	this->field_0xbf0 = fVar13;
	//	this->field_0xbec = fVar13;
	//}
	//else {
	//	this->field_0xbec = fVar19;
	//	this->field_0xbf0 = fVar17 * 0.9;
	//}
	//fVar14 = this->field_0xbec;
	//fVar24 = ((this->pCollisionData)->pObbPrim->field_0x90).z +
	//	(((CActorHero::_gThis->character).characterBase.base.base.pCollisionData)->pObbPrim->field_0x90).z + 0.1;
	//if (fVar24 < fVar14) {
	//	this->field_0xbec = fVar14;
	//}
	//else {
	//	this->field_0xbec = fVar24;
	//	fVar14 = fVar24;
	//}
	//if (this->field_0xbf0 < fVar14) {
	//	this->field_0xbf0 = fVar14;
	//}
	//fVar14 = edFIntervalUnitSrcLERP(this->field_0xa80, fVar24, this->field_0xbec);
	//this->field_0xbec = fVar14;
	//fVar14 = edFIntervalUnitSrcLERP(this->field_0xa80, (float)&DAT_3f333333, 0.0);
	//this->field_0xbf0 = this->field_0xbf0 - fVar14;
	this->field_0xb80 = 1;
	this->field_0xb88 = 0;
	pCVar4 = GetLifeInterfaceOther();
	//pCVar4->field_0x10 = this;
	ClearLocalData();
	return;
}

void CActorWolfen::Term()
{
	uint uVar1;
	EnemyComponent80* pAlloc;
	CActorWeapon* pCVar2;
	CActor* pCVar3;

	//if (this->field_0xbe8 != (edF32VECTOR4*)0x0) {
	//	delete(this->field_0xbe8);
	//	this->field_0xbe8 = (edF32VECTOR4*)0x0;
	//}
	//if (PTR_DAT_004574a0 != (undefined*)0x0) {
	//	FreeFunc_001fba40((int*)&PTR_DAT_004574a0);
	//}
	//uVar1 = this->field_0xb74;
	//if (uVar1 == 3) {
	//	if (this->pEnemyComponent80_0xd34 != (EnemyComponent80*)0x0) {
	//		(*(code*)this->pEnemyComponent80_0xd34->pVTable->field_0x10)();
	//		pAlloc = this->pEnemyComponent80_0xd34;
	//		if (pAlloc != (EnemyComponent80*)0x0) {
	//			pAlloc->pVTable = &EnemyComponent80::VTable_004460b0;
	//			if (pAlloc != (EnemyComponent80*)0x0) {
	//				pAlloc->pVTable = &EnemyComponent80VTable_00447ec0;
	//			}
	//			Free(pAlloc);
	//		}
	//		this->pEnemyComponent80_0xd34 = (EnemyComponent80*)0x0;
	//	}
	//}
	//else {
	//	if (((uVar1 == 1) || (uVar1 == 0)) && (this->pEnemyComponent80_0xd34 != (EnemyComponent80*)0x0)) {
	//		this->pEnemyComponent80_0xd34 = (EnemyComponent80*)0x0;
	//		INT_004497e8 = INT_004497e8 + -1;
	//		if (INT_004497e8 < 1) {
	//			(*(code*)(g_EnemyComponent80_0049c6d0.pVTable)->field_0x10)();
	//		}
	//	}
	//}

	if (((this->flags & 0x2000000) == 0) && (GetWeapon() != (CActorWeapon*)0x0)) {
		pCVar3 = GetWeapon()->GetLinkFather();
		if (pCVar3 == this) {
			GetWeapon()->UnlinkWeapon();
		}
	}

	CActorFighter::Term();

	//if (*(void**)&this->field_0xb64 != (void*)0x0) {
	//	Free(*(void**)&this->field_0xb64);
	//	*(undefined4*)&this->field_0xb64 = 0;
	//}

	//if (this->pWolfenKnowledge != (CActorWolfenKnowledge*)0x0) {
	//	CActorWolfenKnowledge::Term(this->pWolfenKnowledge);
	//	Free(this->pWolfenKnowledge);
	//	this->pWolfenKnowledge = (CActorWolfenKnowledge*)0x0;
	//}

	return;
}

void CActorWolfen::Manage()
{
	bool bVar1;
	CEventManager* pCVar2;
	CActorFighter* pCVar3;
	ed_zone_3d* peVar4;
	int iVar5;
	CLifeInterface* pCVar6;
	float fVar7;

	CActorFighter::Manage();

	ManageKnowledge();

	if (this->field_0xd30 != -1) {
		iVar5 = 2;
		pCVar3 = this->pCommander->GetIntruder();
		pCVar2 = CScene::ptable.g_EventManager_006f5080;
		if (pCVar3 != (CActorFighter*)0x0) {
			peVar4 = (ed_zone_3d*)0x0;
			if (this->field_0xd30 != 0xffffffff) {
				peVar4 = edEventGetChunkZone((CScene::ptable.g_EventManager_006f5080)->activeChunkId, this->field_0xd30);
			}

			iVar5 = edEventComputeZoneAgainstVertex(pCVar2->activeChunkId, peVar4, &pCVar3->currentLocation, 0);
		}

		if (iVar5 == 2) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffffdf;
			if ((~this->combatFlags_0xb78 & 0x30) == 0x30) {
				this->pTargetActor_0xc80 = (CActorFighter*)0x0;
			}
		}
		else {
			if (iVar5 == 1) {
				if ((~this->combatFlags_0xb78 & 0x30) == 0x30) {
					this->pTargetActor_0xc80 = pCVar3;
				}
				this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x20;
			}
		}

		pCVar2 = CScene::ptable.g_EventManager_006f5080;
		peVar4 = (ed_zone_3d*)0x0;
		if (this->field_0xd30 != 0xffffffff) {
			peVar4 = edEventGetChunkZone((CScene::ptable.g_EventManager_006f5080)->activeChunkId, this->field_0xd30);
		}

		iVar5 = edEventComputeZoneAgainstVertex(pCVar2->activeChunkId, peVar4, &this->currentLocation, 0);
		if (iVar5 == 2) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffbff;
		}
		else {
			if (iVar5 == 1) {
				this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x400;
			}
		}
	}

	if ((0 < (int)this->combatMode_0xb7c) && (pCVar3 = this->pTargetActor_0xc80, pCVar3 != (CActorHero*)0x0)) {
		fVar7 = pCVar3->GetLifeInterface()->GetValue();
		if ((fVar7 <= 0.0f) && (this->combatMode_0xb7c != ECM_None)) {
			this->combatMode_0xb7c = ECM_None;
		}
	}

	bVar1 = false;
	if ((this->field_0xb80 != 2) &&
		(((~this->combatFlags_0xb78 & 0x600) != 0x600 ||
			(bVar1 = true, 50.0f <= this->field_0x7c8 - this->field_0x7d0)))) {
		bVar1 = false;
	}

	if (bVar1) {
		if ((this->combatFlags_0xb78 & 0x10000) == 0) {
			this->flags = this->flags | 2;
			this->flags = this->flags & 0xfffffffe;
			this->flags = this->flags | 0x80;
			this->flags = this->flags & 0xffffffdf;
			EvaluateDisplayState();
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x10000;
			this->pCommander->RemoveTracked();
		}
	}
	else {
		if ((this->combatFlags_0xb78 & 0x10000) != 0) {
			this->pCommander->AddTracked();
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffeffff;
			if ((this->flags & 2) != 0) {
				this->flags = this->flags & 0xfffffffc;
			}
			if ((this->flags & 0x80) != 0) {
				this->flags = this->flags & 0xffffff5f;
				EvaluateDisplayState();
			}
		}
	}

	return;
}

void CActorWolfen::Draw()
{
	IMPLEMENTATION_GUARD_LOG();
}

void CActorWolfen::Reset()
{
	this->field_0xb80 = 1;
	this->field_0xb88 = 0;

	this->flags = this->flags & 0xffffff5f;
	EvaluateDisplayState();
	this->flags = this->flags & 0xfffffffc;

	ClearLocalData();
	CActorFighter::Reset();

	return;
}

void CActorWolfen::CheckpointReset()
{
	if (this->field_0xb80 == 2) {
		CActorAutonomous::CheckpointReset();
		ClearLocalData();
	}
	else {
		CActor::PreReset();
		Reset();
	}

	return;
}

void CActorWolfen::SectorChange(int oldSectorId, int newSectorId)
{
	if ((this->combatFlags_0xb78 & 0x10000) != 0) {
		IMPLEMENTATION_GUARD(
		FUN_00171a80(this->pCommander);
		this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffeffff;
		if ((this->flags & 2) != 0) {
			this->flags = this->flags & 0xfffffffc;
		}
		if ((this->flags & 0x80) != 0) {
			this->flags = this->flags & 0xffffff5f;
			CActor::EvaluateDisplayState((CActor*)this);
		}
		(*(code*)(this->pVTable)->CheckpointReset)(this);)
	}

	CActor::SectorChange(oldSectorId, newSectorId);

	return;
}

CBehaviour* CActorWolfen::BuildBehaviour(int behaviourType)
{
	CBehaviour* pBehaviour;

	switch (behaviourType) {
	case WOLFEN_BEHAVIOUR_WATCH_DOG:
		pBehaviour = new CBehaviourWatchDog;
		break;
	case WOLFEN_BEHAVIOUR_GUARD_AREA:
		pBehaviour = new CBehaviourGuardArea;
		break;
	case WOLFEN_BEHAVIOUR_SLEEP:
		pBehaviour = new CBehaviourSleep;
		break;
	case WOLFEN_BEHAVIOUR_ESCAPE:
		pBehaviour = new CBehaviourEscape;
		break;
	case WOLFEN_BEHAVIOUR_EXORCISM:
		pBehaviour = &behaviourExorcism;
		break;
	case WOLFEN_BEHAVIOUR_TRACK:
		pBehaviour = &behaviourTrack;
		break;
	case WOLFEN_BEHAVIOUR_TRACK_WEAPON:
		pBehaviour = new CBehaviourTrackWeapon;
		break;
	case WOLFEN_BEHAVIOUR_TRACK_WEAPON_STAND:
		pBehaviour = new CBehaviourTrackWeaponStand;
		break;
	case WOLFEN_BEHAVIOUR_TRACK_WEAPON_SNIPE:
		pBehaviour = new CBehaviourTrackWeaponSnipe;
		break;
	case WOLFEN_BEHAVIOUR_SNIPE:
		pBehaviour = new CBehaviourSnipe;
		break;
	case WOLFEN_BEHAVIOUR_LOST:
		pBehaviour = &behaviourLost;
		break;
	case WOLFEN_BEHAVIOUR_WOLFEN_UNKNOWN:
		pBehaviour = new CBehaviourWolfenUnknown;
		break;
	case WOLFEN_BEHAVIOUR_DCA:
		pBehaviour = new CBehaviourDCA;
		break;
	case WOLFEN_BEHAVIOUR_AVOID:
		pBehaviour = new CBehaviourAvoid;
		break;
	case WOLFEN_BEHAVIOUR_TRACK_STAND:
		pBehaviour = new CBehaviourTrackStand;
		break;
	default:
		assert(behaviourType < 7);
		pBehaviour = CActorFighter::BuildBehaviour(behaviourType);
		break;
	}
	return pBehaviour;
}

StateConfig* CActorWolfen::GetStateCfg(int state)
{
	StateConfig* pWVar1;

	if (state < 0x72) {
		pWVar1 = CActorFighter::GetStateCfg(state);
	}
	else {
		assert((state - 0x72) < 68);
		pWVar1 = _gStateCfg_WLF + state + -0x72;
	}

	return pWVar1;
}

uint _gBehaviourFlags_WLF[27] =
{
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x1,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
};

uint CActorWolfen::GetBehaviourFlags(int state)
{
	uint behaviourFlags;

	if (state < 7) {
		behaviourFlags = CActorFighter::GetBehaviourFlags(state);
	}
	else {
		behaviourFlags = _gBehaviourFlags_WLF[state];
	}

	return behaviourFlags;
}

void CActorWolfen::UpdatePostAnimEffects()
{
	uint uVar1;
	EnemyComponent80* this_00;

	uVar1 = this->field_0xb74;
	if (uVar1 == 3) {
		if (this->pEnemyComponent80_0xd34 != (EnemyComponent80*)0x0) {
			IMPLEMENTATION_GUARD(
			this->pEnemyComponent80_0xd34->FUN_003c1400();)
		}
	}
	else {
		if ((((uVar1 == 1) || (uVar1 == 0)) && (this_00 = this->pEnemyComponent80_0xd34, this_00 != (EnemyComponent80*)0x0)) && (this_00->pEnemy_0x60 == this)) {
			IMPLEMENTATION_GUARD(
			this_00->FUN_003c2b20();)
		}
	}

	return;
}

void CActorWolfen::SetState(int newState, int animType)
{
	int iVar1;
	ulong uVar2;

	if (newState == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
		animType = 7;

		if (this->combatMode_0xb7c == ECM_InCombat) {
			animType = 6;
		}
	}
	else {
		if (newState == WOLFEN_STATE_COME_BACK) {
			animType = 6;
			if ((this->combatFlags_0xb78 & 0x100000) == 0) {
				animType = 7;
			}
		}
		else {
			if (((newState == 0x99) && (iVar1 = GetIdMacroAnim(0x8a), iVar1 != -1)) && (uVar2 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3, CScene::_pinstance->field_0x38 = uVar2,
					((uint)(uVar2 >> 0x10) & 0x7fff) < 0x4000)) {
				animType = 0x8a;
			}
		}
	}

	CActorMovable::SetState(newState, animType);

	return;
}

void CActorWolfen::ChangeManageState(int state)
{
	int iVar1;
	CBehaviour* pCVar2;
	CActorWeapon* pCVar3;
	CActor* pCVar4;

	CActorAutonomous::ChangeManageState(state);

	iVar1 = this->curBehaviourId;
	if (iVar1 == 5) {
		IMPLEMENTATION_GUARD(
		pCVar2 = CActor::GetBehaviour((CActor*)this, this->curBehaviourId);
		(*(code*)pCVar2->pVTable[1].field_0x0.field_0x4)(pCVar2, state);)
	}
	else {
		if (iVar1 == 4) {
			IMPLEMENTATION_GUARD(
			pCVar2 = CActor::GetBehaviour((CActor*)this, this->curBehaviourId);
			(*(code*)pCVar2->pVTable[1].field_0x0.field_0x4)(pCVar2, state);)
		}
		else {
			if (iVar1 == 3) {
				IMPLEMENTATION_GUARD(
				pCVar2 = CActor::GetBehaviour((CActor*)this, this->curBehaviourId);
				(*(code*)pCVar2->pVTable[1].field_0x0._dt)(pCVar2, state);)
			}
			else {
				if (iVar1 == 0xe) {
					IMPLEMENTATION_GUARD(
					pCVar2 = CActor::GetBehaviour((CActor*)this, this->curBehaviourId);
					(*(code*)pCVar2->pVTable->SaveContext)(pCVar2, state);)
				}
				else {
					if (this->actorState == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
						if (state == 0) {
							IMPLEMENTATION_GUARD_AUDIO(
							FUN_001844c0(CScene::ptable.g_AudioManager_00451698);)
						}
						else {
							IMPLEMENTATION_GUARD_AUDIO(
							GlobalSound::Func_001844f0(CScene::ptable.g_AudioManager_00451698);)
						}
					}
				}
			}
		}
	}

	if (GetWeapon() != (CActorWeapon*)0x0) {
		pCVar3 = GetWeapon();
		pCVar4 = pCVar3->GetLinkFather();
		if (pCVar4 == this) {
			if (state == 0) {
				pCVar3 = GetWeapon();
				pCVar3->flags = pCVar3->flags & 0xfffffffc;
			}
			else {
				pCVar3 = GetWeapon();
				pCVar3->flags = pCVar3->flags | 2;
				pCVar3->flags = pCVar3->flags & 0xfffffffe;
			}
		}
	}

	this->lifeInterface.SetPriority(0);

	return;
}

void CActorWolfen::AnimEvaluate(uint layerId, edAnmMacroAnimator* pAnimator, uint newAnim)
{
	if (newAnim == 0xa2) {
		AnimEvaluate_0xa2(layerId);
	}
	else {
		if (((newAnim == 0xaa) || (newAnim == 0xa9)) || (newAnim == 0xa8)) {
			AnimEvaluate_0017c930(layerId, pAnimator);
		}
		else {
			CActorFighter::AnimEvaluate(layerId, pAnimator, newAnim);
		}
	}
	return;
}

void CActorWolfen::CinematicMode_Enter(bool bSetState)
{
	CActorWeapon* pCVar1;
	CAnimation* pAnimation;

	CActor::CinematicMode_Enter(bSetState);

	if (this->field_0xb74 == 3) {
		pAnimation = this->pAnimationController;
		if (pAnimation != (CAnimation*)0x0) {
			pAnimation->RemoveDisabledBone(0xae8f8ef8);
		}

		pCVar1 = GetWeapon();
		if (pCVar1 != (CActorWeapon*)0x0) {
			pCVar1 = GetWeapon();
			pCVar1->flags = pCVar1->flags & 0xffffff7f;
			pCVar1->flags = pCVar1->flags | 0x20;
			pCVar1->EvaluateDisplayState();
		}
	}

	return;
}

void CActorWolfen::CinematicMode_Leave(int behaviourId)
{
	CActorWeapon* pCVar1;
	CAnimation* pAnimation;

	CActor::CinematicMode_Leave(behaviourId);

	if (behaviourId == 0xe) {
		PlayAnim(_SV_ANM_GetTwoSidedAnim(0x75, this->field_0x7dc));
	}

	if (this->field_0xb74 == 3) {
		pAnimation = this->pAnimationController;
		if (pAnimation != (CAnimation*)0x0) {
			pAnimation->AddDisabledBone(0xae8f8ef8);
		}

		pCVar1 = CActorFighter::GetWeapon();
		if (pCVar1 != (CActorWeapon*)0x0) {
			pCVar1 = GetWeapon();
			pCVar1->flags = pCVar1->flags & 0xffffff5f;
			pCVar1->EvaluateDisplayState();
		}
	}

	return;
}

bool CActorWolfen::CarriedByActor(CActor* pActor, edF32MATRIX4* m0)
{
	int iVar1;
	EnemyComponent80* pEVar2;
	CBehaviour* pCVar3;

	CActorFighter::CarriedByActor(pActor, m0);

	iVar1 = this->curBehaviourId;
	if (iVar1 == 0xe) {
		IMPLEMENTATION_GUARD(
		pCVar3 = GetBehaviour(this->curBehaviourId);
		FUN_001ee4e0((int)pCVar3, (long)(int)pActor, m0);)
	}
	else {
		if (iVar1 == 3) {
			IMPLEMENTATION_GUARD(
			pCVar3 = CActor::GetBehaviour((CActor*)this, this->curBehaviourId);
			(*(code*)pCVar3->pVTable[1].field_0x0.Create)(pCVar3, (long)(int)pActor, m0);)
		}
	}

	iVar1 = this->field_0xb74;
	if (iVar1 == 3) {
		IMPLEMENTATION_GUARD(
		if (this->pEnemyComponent80_0xd34 != (EnemyComponent80*)0x0) {
			FUN_003c1350((int*)this->pEnemyComponent80_0xd34, m0);
		})
	}
	else {
		if ((((iVar1 == 1) || (iVar1 == 0)) && (pEVar2 = this->pEnemyComponent80_0xd34, pEVar2 != (EnemyComponent80*)0x0)) && (pEVar2->pEnemy_0x60 == this)) {
			IMPLEMENTATION_GUARD(
			FUN_003c2980((int*)pEVar2, m0);)
		}
	}

	return true;
}

bool CActorWolfen::IsMakingNoise()
{
	int iVar1;
	bool bVar2;
	StateConfig* pSVar2;

	iVar1 = this->actorState;
	if (iVar1 == 6) {
		bVar2 = true;
	}
	else {
		bVar2 = (GetStateFlags(iVar1) & 2);
	}

	return bVar2;
}

CVision* CActorWolfen::GetVision()
{
	return &vision;
}

int CActorWolfen::InterpretMessage(CActor* pSender, int msg, void* pMsgParam)
{
	//CBehaviourMovingPlatformVTable* pCVar1;
	edColPRIM_OBJECT* peVar2;
	CActorFighter* pCVar3;
	CActorCommander* pCVar4;
	CEventManager* pCVar5;
	bool bVar6;
	char cVar7;
	CBehaviour* pCVar8;
	CLifeInterface* pCVar9;
	CActorHero* pCVar10;
	ed_zone_3d* peVar11;
	uint uVar12;
	CVision* pCVar13;
	CActor* pCVar14;
	CPathFinderClient* pPathFindingClient;
	int iVar15;
	long lVar16;
	undefined8 uVar17;
	float fVar18;
	float fVar19;
	float fVar20;
	edF32VECTOR4 local_130;
	edF32VECTOR4 local_120;
	edF32VECTOR4 local_110;
	float local_100;
	float fStack252;
	float fStack248;
	float fStack244;
	edF32VECTOR4 local_f0;
	float local_e0;
	float fStack220;
	float local_d8;
	float fStack212;
	float local_d0;
	float local_cc;
	float local_c8;
	float local_c4;
	edF32VECTOR4 local_c0;
	float local_b0;
	float local_ac;
	float local_a8;
	float local_a4;
	edF32VECTOR4 local_a0;
	undefined4 auStack144[8];
	edF32VECTOR4 aeStack112[6];
	undefined4* local_4;

	if (msg == 0x25) {
		IMPLEMENTATION_GUARD(
		FUN_001fbaf0(this, 0);)
	LAB_0017c6e8:
		iVar15 = CActorFighter::InterpretMessage(pSender, msg, pMsgParam);
		return iVar15;
	}

	if (msg == 0x26) {
		IMPLEMENTATION_GUARD(
		FUN_001fbaf0(this, 1);
		goto LAB_0017c6e8;)
	}

	if (msg != 0x1a) {
		if (msg == 0x3f) {
			return 2;
		}

		if (msg == 99) {
			IMPLEMENTATION_GUARD(
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x8000;)
			goto LAB_0017c6e8;
		}

		if (msg == 0x3a) {
			bVar6 = false;
			if (pSender->IsMakingNoise() != 0) {
				
				local_b0 = this->currentLocation.x;
				local_a8 = this->currentLocation.z;
				local_a4 = this->currentLocation.w;
				local_ac = this->currentLocation.y + this->field_0xcf0;
				pSender->SV_GetActorColCenter(&local_c0);
				local_e0 = local_b0 - local_c0.x;
				fStack220 = local_ac - local_c0.y;
				local_d8 = local_a8 - local_c0.z;
				fStack212 = local_a4 - local_c0.w;
				if ((local_e0 * local_e0 + local_d8 * local_d8 < this->hearingDetectionProps.rangeSquared) &&
					(fabs(local_ac - local_c0.y) < this->hearingDetectionProps.maxHeightDifference)) {
					bVar6 = true;
				}
			}

			if (!bVar6) {
				iVar15 = this->actorState;
				uVar12 = 0;
	
				if ((iVar15 != -1) && (uVar12 = 0, 0x71 < iVar15)) {
					uVar12 = _gStateCfg_WLF[iVar15 + -0x72].field_0x8;
				}

				if ((uVar12 & 0x20) == 0) {
					GetVision()->location = this->currentLocation;
					GetVision()->rotationQuat = this->rotationQuat;
				}

				pCVar14 = GetVision()->ScanForTarget(pSender, 1);

				if (pCVar14 == (CActor*)0x0) {
					if (this->visionDetectionProps.field_0x0 <= sqrtf(edF32Vector4DotProductHard_I(&pSender->currentLocation, &this->currentLocation))) {
						if (!IsAlive(pSender)) {
							return 1;
						}

						EnterCombatState(pSender);

						UpdateCombatMode();

						return 1;
					}
				}
			}

		
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x100;

			if (pSender->typeID != 9) {
				if (pSender->typeID != 0x1c) {
					return 1;
				}
				this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x80;
				return 1;
			}

			if (!IsAlive(static_cast<CActor*>(pMsgParam))) {
				return 1;
			}

			EnterCombatState(static_cast<CActor*>(pMsgParam));

			UpdateCombatMode();

			return 1;
		}

		if (msg == 0xc) {
			IMPLEMENTATION_GUARD(
			*(float*)pMsgParam = this->field_0xc5c;
			return 1;)
		}

		if ((msg == 0x66) || (msg == 0x27)) {
			IMPLEMENTATION_GUARD(
			if ((this->field_0xb74 != 0) && (this->field_0xb74 != 1)) {
				return 0;
			}
			iVar15 = CActorFighter::InterpretMessage((CActorFighter*)this, pSender, msg, pMsgParam);
			return iVar15;)
		}

		if (msg == 0x86) {
			IMPLEMENTATION_GUARD(
			if ((this->field_0xb80 != 2) && (this->field_0xb80 != 0)) {
				(*(code*)(this->pVTable)->LifeAnnihilate)();
				SetBehaviour(0xe, 0x7f, -1);
			}
			return 1;)
		}

		if (msg != 2) {
			if (msg == 1) {
				IMPLEMENTATION_GUARD(
				pCVar9 = (*(this->pVTable)->GetLifeInterface)((CActor*)this);
				fVar18 = (*pCVar9->pVtable->GetValue)((CInterface*)pCVar9);
				if (0.0 < fVar18) {
					return 1;
				})
			}
			else {
				if (msg == 0x16) {
					IMPLEMENTATION_GUARD(
					this->field_0x6a0.x = g_xVector.x;
					this->field_0x6a0.y = g_xVector.y;
					this->field_0x6a0.z = g_xVector.z;
					this->field_0x6a0.w = g_xVector.w;
					this->field_0x6b0 = 0.001;
					SetBehaviour(4, -1, -1);
					return 1;)
				}
				if (msg == 7) {
					IMPLEMENTATION_GUARD(
					/* WARNING: Load size is inaccurate */
					if ((*pMsgParam == 1) || (*pMsgParam == 0)) {
						peVar2 = (this->pCollisionData)->pObbPrim;
						*(undefined4*)((int)pMsgParam + 0x20) = 0;
						*(float*)((int)pMsgParam + 0x24) = (peVar2->field_0x90).y + (peVar2->field_0xb0).y;
						*(undefined4*)((int)pMsgParam + 0x28) = 0;
						*(undefined4*)((int)pMsgParam + 0x2c) = 0;
						return 1;
					})
				}
				else {
					if (msg == 0x36) {
						IMPLEMENTATION_GUARD(
						if (pMsgParam == (void*)0x0) {
							this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffdfff;
						}
						else {
							this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x2000;
						}
						return 1;)
					}

					if (msg == 3) {
						IMPLEMENTATION_GUARD(
						if (pSender->typeID == 0x33) {
							pCVar8 = CActor::GetBehaviour((CActor*)this, 0x18);
							if ((pCVar8 != (CBehaviour*)0x0) && (this->curBehaviourId != 4)) {
								pCVar8 = CActor::GetBehaviour((CActor*)this, 0x18);
								FUN_001ec780((int)pCVar8, auStack144, (int)pSender);
								local_4 = auStack144;
								CActor::DoMessage(pSender, (CActor*)this, 2, (uint)local_4);
								CActorFighter::PlayImpactFx
								((CActorFighter*)this,
									(long)(int)&(this->pCollisionData)->field_0x90, aeStack112, 1,
									'\0');
							}
						}
						else {
							(*(code*)(this->pVTable)->LifeAnnihilate)();
							pCVar1 = (CBehaviourMovingPlatformVTable*)this->curBehaviourId;
							pCVar8 = CActor::GetBehaviour((CActor*)this, 0xe);
							pCVar8[2].pVTable = pCVar1;
							SetBehaviour(4, 0x5a, -1);
						})
					}
				}
			}

			goto LAB_0017c6e8;
		}

		IMPLEMENTATION_GUARD(
		/* WARNING: Load size is inaccurate */
		if ((*pMsgParam != 8) && (*pMsgParam != 7)) goto LAB_0017ba38;
		this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x100;
		bVar6 = (*pSender->pVTable->IsKindOfObject)(pSender, 8);
		if (bVar6 == false) {
			return 1;
		}
		if (this->field_0x350 == (CActor*)0x0) {
			(*(code*)this->pVTable[1].actorBase)();
		}

		if (IsAlive(pSender)) {
			EnterCombatState(pSender);
			
			UpdateCombatMode();
		}
		
		iVar15 = this->actorState;
		if ((iVar15 == 0xa1) || (iVar15 == 0xa2)) {
			edF32Vector4GetNegHard(&local_a0, &this->rotationQuat);
			this->rotationQuat.x = local_a0.x;
			this->rotationQuat.y = local_a0.y;
			this->rotationQuat.z = local_a0.z;
			this->rotationQuat.w = local_a0.w;
		}
		iVar15 = CActorFighter::InterpretMessage((CActorFighter*)this, pSender, 2, pMsgParam);
		return iVar15;)
	}

	InitPathfindingClientMsgParams* pMsgParams = reinterpret_cast<InitPathfindingClientMsgParams*>(pMsgParam);
	/* WARNING: Load size is inaccurate */
	switch (pMsgParams->msgId) {
	case 0:
		IMPLEMENTATION_GUARD(
		SetBehaviour(*(int*)((int)pMsgParam + 4), -1, -1);)
		break;
	case 1:
		pCVar3 = pMsgParams->pActor;
		this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x10;
		this->pTargetActor_0xc80 = pCVar3;
		break;
	case 2:
		this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffffef;
		if ((this->combatFlags_0xb78 & 0x20) == 0) {
			this->pTargetActor_0xc80 = (CActorHero*)0x0;
		}
		break;
	case 3:
		pCVar3 = pMsgParams->pActor;
		if (this->field_0xd30 == -1) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x20;
			this->pTargetActor_0xc80 = pCVar3;
		}
		break;
	case 4:
		if ((this->field_0xd30 == -1) &&
			(this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffffdf, (this->combatFlags_0xb78 & 0x10) == 0)) {
			this->pTargetActor_0xc80 = (CActorHero*)0x0;
		}
		break;
	case 5:
		pCVar3 = pMsgParams->pActor;

		if (!IsAlive(pCVar3)) {
			return 1;
		}

		EnterCombatState(pCVar3);

		UpdateCombatMode();
		break;
	case 6:
		if (this->field_0xd30 == -1) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x400;
		}
		break;
	case 7:
		if (this->field_0xd30 == -1) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffbff;
		}
		break;
	case 8:
		this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x200;
		break;
	case 9:
		this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffdff;
		break;
	case 10:
	{
		pPathFindingClient = GetPathfinderClient();
		pPathFindingClient->Init();
		pPathFindingClient->ChangePathfindingId(pMsgParams->pActor, pMsgParams->newId, &this->currentLocation);
	}
		break;
	case 0xf:
		if (this->actorState == 6) {
			SetState(0xad, -1);
		}
	}

	return 1;
}

float CActorWolfen::GetWalkSpeed()
{
	return this->walkSpeed;
}

float CActorWolfen::GetWalkRotSpeed()
{
	return this->walkRotSpeed;
}

float CActorWolfen::GetWalkAcceleration()
{
	return this->walkAcceleration;
}

float CActorWolfen::GetRunSpeed()
{
	return this->runSpeed;
}

float CActorWolfen::GetRunRotSpeed()
{
	return this->rotRunSpeed;
}

float CActorWolfen::GetRunAcceleration()
{
	return this->runAcceleration;
}

CPathFinderClient* CActorWolfen::GetPathfinderClient()
{
	return &pathFinderClient;
}

CPathFinderClient* CActorWolfen::GetPathfinderClientAlt()
{
	return &pathFinderClient;
}

bool CActorWolfen::Func_0x19c()
{
	bool uVar1;

	uVar1 = false;
	if (this->pInputAnalyser != (CInputAnalyser*)0x0) {
		uVar1 = CActorFighter::Func_0x19c();
	}

	return uVar1;
}

bool CActorWolfen::Func_0x1ac()
{
	IMPLEMENTATION_GUARD();
}

void CActorWolfen::_Std_OnFightActionSuccess()
{
	IMPLEMENTATION_GUARD();
}

void CActorWolfen::Func_0x204(CActorFighter* pOther)
{
	CActorFighter* pCVar1;
	int iVar2;
	float fVar3;
	edF32VECTOR4 eStack32;
	float* local_8;
	float local_4;

	SetAdversary(pOther);

	this->runSpeed = 0.0f;
	pCVar1 = this->pAdversary;
	if (pCVar1 == (CActorFighter*)0x0) {
		this->adversaryDistance = this->field_0x358;
		this->adversaryAngleDiff = GetAngleYFromVector(&this->rotationQuat);
		Func_0x20c(this->field_0xc5c);
	}
	else {
		edF32Vector4SubHard(&eStack32, &pCVar1->currentLocation, &this->currentLocation);
		this->adversaryAngleDiff = GetAngleYFromVector(&eStack32);
		this->adversaryDistance = edF32Vector4GetDistHard(&eStack32);
		local_8 = &local_4;
		iVar2 = DoMessage(this->pAdversary, (ACTOR_MESSAGE)0xc, (void*)local_8);
		if (iVar2 != 0) {
			IMPLEMENTATION_GUARD(
			(*(code*)(this->pVTable)->field_0x20c)(local_4 * this->field_0xc6c, this);)
		}
	}
	return;
}

void CActorWolfen::Func_0x20c(float param_1)
{
	this->runSpeed = param_1;
	return;
}

void CActorWolfen::SetCombatMode(EEnemyCombatMode newCombatMode)
{
	if (this->combatMode_0xb7c != newCombatMode) {
		this->combatMode_0xb7c = newCombatMode;
	}

	return;
}

uint CActorWolfen::GetStateWolfenFlags(int state)
{
	uint uVar1;

	uVar1 = 0;
	if (state != -1) {
		if (state < 0x72) {
			uVar1 = 0;
		}
		else {
			uVar1 = _gStateCfg_WLF[state + -0x72].field_0x8;
		}
	}

	return uVar1;
}

void CActorWolfen::AnimEvaluate_0xa2(uint layerId)
{
	edANM_HDR* peVar1;
	int index;
	edAnmLayer* peVar2;
	CAnimation* pAnim;

	pAnim = this->pAnimationController;
	index = pAnim->PhysicalLayerFromLayerId(layerId);
	peVar2 = (pAnim->anmBinMetaAnimator).aAnimData + index;

	peVar2->field_0x4 = this->field_0xd24; // This can be read before it is initialized... Might be worth fixing

	if (0.0f < this->field_0xd24) {
		pAnim->anmBinMetaAnimator.SetLayerTimeWarper(this->field_0xd24, index);

		peVar1 = (peVar2->currentAnimDesc).state.pAnimKeyTableEntry;
		if ((peVar1->keyIndex_0x8.asKey == 2) && (peVar1->field_0x4.asKey == 1)) {
			char* pBase = (char*)peVar1;
			AnimKeySomething* pValue = (AnimKeySomething*)(pBase + (peVar2->currentAnimDesc).state.pAnimKeyTableEntry->keyIndex_0x8.asKey * 4);

			pValue->field_0xc = 1.0f - this->field_0xd28;
			pValue->field_0x10 = this->field_0xd28;
		}
	}

	return;
}

void CActorWolfen::AnimEvaluate_0017c930(uint layerId, edAnmMacroAnimator* pAnimator)
{
	edANM_HDR* peVar1;
	CBehaviour* pCVar2;
	uint uVar3;
	int iVar4;
	float fVar5;
	uint uVar6;
	float fVar7;
	float fVar8;
	float fVar9;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 local_10;

	peVar1 = pAnimator->pAnimKeyTableEntry;
	if (1 < peVar1->keyIndex_0x8.asKey) {
		CActor::SV_GetActorColCenter(&eStack32);
		iVar4 = this->curBehaviourId;
		if ((((iVar4 == WOLFEN_BEHAVIOUR_TRACK_WEAPON_STAND) || (iVar4 == WOLFEN_BEHAVIOUR_TRACK_WEAPON)) || (iVar4 == WOLFEN_BEHAVIOUR_TRACK_WEAPON_SNIPE)) || (iVar4 == WOLFEN_BEHAVIOUR_SNIPE)) {
			CBehaviourWolfen* pWolfenBehaviour = static_cast<CBehaviourWolfen*>(GetBehaviour(iVar4));
			edF32Vector4SubHard(&local_10, &pWolfenBehaviour->rotationEuler, &eStack32);
		}
		else {
			if (this->pTargetActor_0xc80 == (CActorFighter*)0x0) {
				local_10 = this->rotationQuat;
			}
			else {
				this->pTargetActor_0xc80->SV_GetActorColCenter(&eStack48);
				edF32Vector4SubHard(&local_10, &eStack48, &eStack32);
			}
		}

		edF32Vector4NormalizeHard(&local_10, &local_10);
		fVar7 = edF32Vector4DotProductHard(&local_10, &g_xVector);
		uVar3 = peVar1->keyIndex_0x8.asKey - 1;
		fVar7 = 0.5f - fVar7 * 0.5f;
		if ((int)uVar3 < 0) {
			fVar8 = (float)(uVar3 >> 1 | uVar3 & 1);
			fVar8 = fVar8 + fVar8;
		}
		else {
			fVar8 = (float)uVar3;
		}

		fVar8 = 1.0f / fVar8;
		fVar5 = fVar7 / fVar8;
		if (fVar5 < 2.147484e+09f) {
			uVar6 = (uint)fVar5;
		}
		else {
			uVar6 = (int)(fVar5 - 2.147484e+09f) | 0x80000000;
		}

		if (peVar1->keyIndex_0x8.asKey <= uVar6) {
			uVar6 = uVar3;
		}

		if ((int)uVar6 < 0) {
			fVar5 = (float)(uVar6 >> 1 | uVar6 & 1);
			fVar5 = fVar5 + fVar5;
		}
		else {
			fVar5 = (float)uVar6;
		}

		uVar3 = uVar6 + 1;
		if ((int)uVar3 < 0) {
			fVar9 = (float)(uVar3 >> 1 | uVar3 & 1);
			fVar9 = fVar9 + fVar9;
		}
		else {
			fVar9 = (float)uVar3;
		}

		char* pBase = (char*)peVar1;
		AnimKeySomething* pValue = (AnimKeySomething*)(pBase + peVar1->keyIndex_0x8.asKey * 4);

		fVar7 = edFIntervalLERP(fVar7, fVar5 * fVar8, fVar9 * fVar8, 0.0f, 1.0f);
		uVar3 = 0;
		if (peVar1->keyIndex_0x8.asKey != 0) {
			do {
				pValue->field_0xc_array[uVar3];
				uVar3 = uVar3 + 1;
			} while (uVar3 < peVar1->keyIndex_0x8.asKey);
		}

		pValue->field_0xc_array[uVar6] = fVar7;
		pValue->field_0xc_array[uVar6] = (1.0f - fVar7);
	}

	return;
}

void CActorWolfen::Create_FightParam(ByteCode* pByteCode)
{
	uint uVar1;
	uint uVar2;
	void* pvVar3;
	uint uVar4;
	uint uVar5;
	CActorWolfenKnowledge* pKnowledge;
	undefined8 uVar6;
	int iVar7;
	char* pcVar8;
	uint* puVar9;
	uint uVar10;
	int iVar11;
	float fVar12;
	float fVar13;
	float fVar14;
	uint local_2c0;
	uint local_2b0;
	uint local_290;
	uint local_280[4];
	undefined4 local_270;
	undefined4 local_26c;
	undefined4 local_268;
	undefined4 local_264;
	undefined4 local_260;
	undefined4 local_25c;
	undefined4 local_258;
	undefined4 local_254;
	undefined4 local_250;
	undefined4 local_24c;
	undefined4 local_248;
	undefined4 local_244;
	undefined4 local_240;
	undefined4 local_23c;
	undefined4 local_238;
	undefined4 local_234;
	undefined4 local_230;
	undefined4 local_22c;
	undefined4 local_228;
	undefined4 local_224;
	undefined4 local_220;
	undefined4 local_21c;
	undefined4 local_218;
	undefined4 local_214;
	undefined4 local_210;
	undefined4 local_20c;
	undefined4 local_208;
	undefined4 local_204;
	undefined4 local_200;
	undefined4 local_1fc;
	undefined4 local_1f8;
	undefined4 local_1f4;
	undefined4 local_1f0;
	undefined4 local_1ec;
	undefined4 local_1e8;
	undefined4 local_1e4;
	uint local_1e0[120];

	//ByteCode::GetU32(pByteCode);
	//*(undefined4*)&this->field_0xa90 = 0;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xa94 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xa98 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xa9c = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xaa0 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xaa4 = uVar1;
	//*(undefined4*)&this->field_0xaf8 = 1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xafc = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb00 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb04 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb08 = uVar1;
	//uVar1 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb0c = uVar1;
	//memcpy((void*)&this->field_0xac4, (void*)&this->field_0xa90, 0x18);
	//*(undefined4*)&this->field_0xb30 = 3;
	this->field_0xb2c = 3;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//this->field_0xa80 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xa84 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xa88 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xa8c = fVar12;
	//local_260 = 0x20000;
	//local_25c = 2;
	//local_24c = 3;
	//local_280[0] = 1;
	//local_23c = 4;
	//uVar1 = 10;
	//local_280[2] = 1;
	//local_22c = 5;
	//local_268 = 0xffffffff;
	//local_220 = 0x40000;
	//local_26c = 1;
	//local_21c = 6;
	//local_258 = 0xffffffff;
	//local_210 = 0x4000;
	//local_250 = 0x80000;
	//local_20c = 7;
	//local_248 = 0xffffffff;
	//local_200 = 0x400;
	//local_240 = 0x80000;
	//local_1f0 = 0x10000;
	//local_230 = 0x80000;
	//local_1ec = 9;
	//local_238 = 0xffffffff;
	//local_228 = 0xffffffff;
	//local_218 = 0xffffffff;
	//local_208 = 8;
	//local_1fc = 8;
	//local_204 = 1;
	//local_1f4 = 1;
	//local_1f8 = 0xffffffff;
	//local_1e8 = 0xffffffff;
	//local_280[1] = 0;
	//local_280[3] = 0;
	//local_270 = 0;
	//local_264 = 0;
	//local_254 = 0;
	//local_244 = 0;
	//local_234 = 0;
	//local_224 = 0;
	//local_214 = 0;
	//local_1e4 = 0;
	//uVar2 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xac0 = uVar2;
	//*(undefined4*)&this->field_0xaac = *(undefined4*)&this->field_0xac0;
	//if (*(int*)&this->field_0xaac == 0) {
	//	*(undefined4*)&this->field_0xaa8 = 0;
	//}
	//else {
	//	pvVar3 = operator.new.array((long)(*(int*)&this->field_0xaac * 0x14));
	//	*(void**)&this->field_0xaa8 = pvVar3;
	//}
	//local_290 = 0;
	//puVar9 = local_1e0;
	//if (*(int*)&this->field_0xac0 != 0) {
	//	iVar11 = 0;
	//	uVar2 = uVar1;
	//	do {
	//		uVar4 = ByteCode::GetU32(pByteCode);
	//		uVar10 = 0;
	//		uVar1 = uVar2;
	//		if (uVar4 != 0) {
	//			do {
	//				uVar5 = ByteCode::GetU32(pByteCode);
	//				*puVar9 = uVar5;
	//				if (uVar10 < uVar4 - 1) {
	//					uVar5 = uVar1 + 1;
	//				}
	//				else {
	//					uVar5 = 0xffffffff;
	//				}
	//				puVar9[2] = uVar5;
	//				uVar10 = uVar10 + 1;
	//				puVar9[1] = uVar1;
	//				puVar9 = puVar9 + 4;
	//				uVar1 = uVar1 + 1;
	//			} while (uVar10 < uVar4);
	//		}
	//		fVar13 = ByteCode::GetF32(pByteCode);
	//		fVar14 = ByteCode::GetF32(pByteCode);
	//		fVar12 = *(float*)&this->field_0xa8c;
	//		iVar7 = *(int*)&this->field_0xaa8 + iVar11;
	//		*(uint*)(iVar7 + 0xc) = uVar4;
	//		*(uint*)(iVar7 + 0x10) = uVar2;
	//		*(undefined4*)&this->field_0xab0 = 0;
	//		*(float*)(*(int*)&this->field_0xaa8 + iVar11 + 4) = fVar14 + fVar12 * (fVar13 - fVar14);
	//		*(undefined4*)&this->field_0xab0 = 0;
	//		pcVar8 = (char*)(*(int*)&this->field_0xaa8 + iVar11);
	//		if (*pcVar8 != '\x01') {
	//			*pcVar8 = '\x01';
	//			*(undefined4*)&this->field_0xab0 = 0;
	//		}
	//		local_290 = local_290 + 1;
	//		iVar11 = iVar11 + 0x14;
	//		uVar2 = uVar1;
	//	} while (local_290 < *(uint*)&this->field_0xac0);
	//}
	//uVar2 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xaf4 = uVar2;
	//*(undefined4*)&this->field_0xae0 = *(undefined4*)&this->field_0xaf4;
	//if (*(int*)&this->field_0xae0 == 0) {
	//	*(undefined4*)&this->field_0xadc = 0;
	//}
	//else {
	//	pvVar3 = operator.new.array((long)(*(int*)&this->field_0xae0 * 0x14));
	//	*(void**)&this->field_0xadc = pvVar3;
	//}
	//local_2c0 = 0;
	//if (*(int*)&this->field_0xaf4 != 0) {
	//	iVar11 = 0;
	//	puVar9 = local_280 + uVar1 * 4;
	//	uVar2 = uVar1;
	//	do {
	//		uVar4 = ByteCode::GetU32(pByteCode);
	//		uVar10 = 0;
	//		uVar1 = uVar2;
	//		if (uVar4 != 0) {
	//			do {
	//				uVar5 = ByteCode::GetU32(pByteCode);
	//				*puVar9 = uVar5;
	//				if (uVar10 < uVar4 - 1) {
	//					uVar5 = uVar1 + 1;
	//				}
	//				else {
	//					uVar5 = 0xffffffff;
	//				}
	//				puVar9[2] = uVar5;
	//				uVar10 = uVar10 + 1;
	//				puVar9[1] = uVar1;
	//				puVar9 = puVar9 + 4;
	//				uVar1 = uVar1 + 1;
	//			} while (uVar10 < uVar4);
	//		}
	//		fVar13 = ByteCode::GetF32(pByteCode);
	//		fVar14 = ByteCode::GetF32(pByteCode);
	//		fVar12 = *(float*)&this->field_0xa8c;
	//		iVar7 = *(int*)&this->field_0xadc + iVar11;
	//		*(uint*)(iVar7 + 0xc) = uVar4;
	//		*(uint*)(iVar7 + 0x10) = uVar2;
	//		*(undefined4*)&this->field_0xae4 = 0;
	//		*(float*)(*(int*)&this->field_0xadc + iVar11 + 4) = fVar14 + fVar12 * (fVar13 - fVar14);
	//		*(undefined4*)&this->field_0xae4 = 0;
	//		pcVar8 = (char*)(*(int*)&this->field_0xadc + iVar11);
	//		if (*pcVar8 != '\x01') {
	//			*pcVar8 = '\x01';
	//			*(undefined4*)&this->field_0xae4 = 0;
	//		}
	//		local_2c0 = local_2c0 + 1;
	//		iVar11 = iVar11 + 0x14;
	//		uVar2 = uVar1;
	//	} while (local_2c0 < *(uint*)&this->field_0xaf4);
	//}
	//uVar2 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb28 = uVar2;
	//*(undefined4*)&this->field_0xb14 = *(undefined4*)&this->field_0xb28;
	//if (*(int*)&this->field_0xb14 == 0) {
	//	*(undefined4*)&this->field_0xb10 = 0;
	//}
	//else {
	//	pvVar3 = operator.new.array((long)(*(int*)&this->field_0xb14 * 0x14));
	//	*(void**)&this->field_0xb10 = pvVar3;
	//}
	//local_2b0 = 0;
	//if (*(int*)&this->field_0xb28 != 0) {
	//	iVar11 = 0;
	//	puVar9 = local_280 + uVar1 * 4;
	//	uVar2 = uVar1;
	//	do {
	//		uVar4 = ByteCode::GetU32(pByteCode);
	//		uVar10 = 0;
	//		uVar1 = uVar2;
	//		if (uVar4 != 0) {
	//			do {
	//				uVar5 = ByteCode::GetU32(pByteCode);
	//				*puVar9 = uVar5;
	//				if (uVar10 < uVar4 - 1) {
	//					uVar5 = uVar1 + 1;
	//				}
	//				else {
	//					uVar5 = 0xffffffff;
	//				}
	//				puVar9[2] = uVar5;
	//				uVar10 = uVar10 + 1;
	//				puVar9[1] = uVar1;
	//				puVar9 = puVar9 + 4;
	//				uVar1 = uVar1 + 1;
	//			} while (uVar10 < uVar4);
	//		}
	//		fVar13 = ByteCode::GetF32(pByteCode);
	//		fVar14 = ByteCode::GetF32(pByteCode);
	//		fVar12 = *(float*)&this->field_0xa8c;
	//		iVar7 = *(int*)&this->field_0xb10 + iVar11;
	//		*(uint*)(iVar7 + 0xc) = uVar4;
	//		*(uint*)(iVar7 + 0x10) = uVar2;
	//		*(undefined4*)&this->field_0xb18 = 0;
	//		*(float*)(*(int*)&this->field_0xb10 + iVar11 + 4) = fVar14 + fVar12 * (fVar13 - fVar14);
	//		*(undefined4*)&this->field_0xb18 = 0;
	//		pcVar8 = (char*)(*(int*)&this->field_0xb10 + iVar11);
	//		if (*pcVar8 != '\x01') {
	//			*pcVar8 = '\x01';
	//			*(undefined4*)&this->field_0xb18 = 0;
	//		}
	//		local_2b0 = local_2b0 + 1;
	//		iVar11 = iVar11 + 0x14;
	//		uVar2 = uVar1;
	//	} while (local_2b0 < *(uint*)&this->field_0xb28);
	//}
	//uVar4 = ByteCode::GetU32(pByteCode);
	//uVar2 = uVar1;
	//if (uVar4 == 0) {
	//	uVar2 = 0;
	//}
	//uVar10 = 0;
	//if (uVar4 != 0) {
	//	puVar9 = local_280 + uVar1 * 4;
	//	do {
	//		uVar5 = ByteCode::GetU32(pByteCode);
	//		*puVar9 = uVar5;
	//		if (uVar10 < uVar4 - 1) {
	//			uVar5 = uVar1 + 1;
	//		}
	//		else {
	//			uVar5 = 0xffffffff;
	//		}
	//		puVar9[2] = uVar5;
	//		uVar10 = uVar10 + 1;
	//		puVar9[1] = uVar1;
	//		puVar9 = puVar9 + 4;
	//		uVar1 = uVar1 + 1;
	//	} while (uVar10 < uVar4);
	//}
	//*(uint*)&this->field_0xb34 = uVar4;
	//*(uint*)&this->field_0xb38 = uVar2;
	//uVar4 = ByteCode::GetU32(pByteCode);
	//uVar2 = uVar1;
	//if (uVar4 == 0) {
	//	uVar2 = 0;
	//}
	//uVar10 = 0;
	//if (uVar4 != 0) {
	//	puVar9 = local_280 + uVar1 * 4;
	//	do {
	//		uVar5 = ByteCode::GetU32(pByteCode);
	//		*puVar9 = uVar5;
	//		if (uVar10 < uVar4 - 1) {
	//			uVar5 = uVar1 + 1;
	//		}
	//		else {
	//			uVar5 = 0xffffffff;
	//		}
	//		puVar9[2] = uVar5;
	//		uVar10 = uVar10 + 1;
	//		puVar9[1] = uVar1;
	//		puVar9 = puVar9 + 4;
	//		uVar1 = uVar1 + 1;
	//	} while (uVar10 < uVar4);
	//}
	//*(uint*)&this->field_0xb3c = uVar4;
	//*(uint*)&this->field_0xb40 = uVar2;
	//ByteCode::GetU32(pByteCode);
	//uVar2 = ByteCode::GetU32(pByteCode);
	//*(uint*)&this->field_0xb44 = uVar2;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb48 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb4c = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb50 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb54 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb58 = fVar12;
	//fVar12 = ByteCode::GetF32(pByteCode);
	//*(float*)&this->field_0xb5c = fVar12;
	//*(uint*)&this->field_0xb60 = uVar1;
	//pvVar3 = operator.new.array((long)(int)(uVar1 << 4));
	//*(void**)&this->field_0xb64 = pvVar3;
	//memcpy(*(void**)&this->field_0xb64, local_280, uVar1 << 4);
	//uVar1 = ByteCode::GetU32(pByteCode);
	//uVar2 = ByteCode::GetU32(pByteCode);
	//uVar4 = ByteCode::GetU32(pByteCode);
	//uVar10 = ByteCode::GetU32(pByteCode);
	//if (uVar2 == 0) {
	if (true) {
		this->pWolfenKnowledge = (CActorWolfenKnowledge*)0x0;
	}
	else {
		//pKnowledge = (CActorWolfenKnowledge*)operator.new(0x30);
		//if (pKnowledge != (CActorWolfenKnowledge*)0x0) {
		//	uVar6 = CActorWolfenKnowledge::CActorWolfenKnowledge(pKnowledge);
		//	pKnowledge = (CActorWolfenKnowledge*)uVar6;
		//}
		//this->pWolfenKnowledge = pKnowledge;
		//CActorWolfenKnowledge::Init(this->pWolfenKnowledge, uVar1, uVar4, uVar10, uVar2, 0x80);
	}

	return;
}

void CActorWolfen::ManageKnowledge()
{
	s_fighter_combo* pFighterCombo;
	float fVar1;
	float fVar2;
	bool bVar3;
	CActorFighter* pAdv;
	CActorWolfenKnowledge* pKnowledge;

	if (((this->pWolfenKnowledge != (CActorWolfenKnowledge*)0x0) &&
		(pAdv = this->pAdversary, pAdv != (CActorFighter*)0x0)) && ((CActorWolfen*)pAdv->pAdversary == this)) {
		IMPLEMENTATION_GUARD(
		if (((pAdv->pFighterCombo != (s_fighter_combo*)0x0) &&
			(bVar3 = CActorFighter::FUN_0031b790(pAdv, (pAdv->characterBase).base.base.actorState), bVar3 != false)) &&
			(pFighterCombo = pAdv->pFighterCombo,
				fVar1 = (pAdv->characterBase).base.base.currentLocation.x -
				this->currentLocation.x,
				fVar2 = (pAdv->characterBase).base.base.currentLocation.z -
				this->currentLocation.z,
				fVar1 * fVar1 + fVar2 * fVar2 <=
				*(float*)(pFighterCombo->actionHash + 0x4c) * *(float*)(pFighterCombo->actionHash + 0x4c) * 1.25)) {
			pKnowledge = this->pWolfenKnowledge;
			if (pKnowledge->field_0x1c == 0) {
				CActorWolfenKnowledge::BeginMemory(pKnowledge, pFighterCombo);
			}
			else {
				CActorWolfenKnowledge::NextStage(pKnowledge, pFighterCombo);
			}

			if (this->pWolfenKnowledge->field_0x2c == 1) {
				this->field_0xb70 = 0;
			}
		}

		if ((this->pWolfenKnowledge->field_0x1c != 0) &&
			(bVar3 = CActorFighter::FUN_0031b790(pAdv, (pAdv->characterBase).base.base.actorState), bVar3 == false)) {
			CActorWolfenKnowledge::EndMemory(this->pWolfenKnowledge);
		})
	}

	return;
}

void CActorWolfen::BehaviourStand_Manage(CBehaviourWolfen* pBehaviour)
{
	CActorWolfen* pCVar1;
	CActorFighter* pCVar2;
	bool bVar3;
	uint uVar4;
	CLifeInterface* pCVar5;
	int iVar6;
	int iVar7;
	float fVar8;

	iVar6 = this->actorState;
	if (iVar6 == 0x72) {
	LAB_001f46f0:
		StateStandGuard();
	}
	else {
		if (iVar6 == 0x9c) {
			SetState(0x72, -1);
			goto LAB_001f46f0;
		}
	}

	pCVar1 = pBehaviour->pOwner;
	if (((pCVar1->combatFlags_0xb78 & 0x800) != 0) &&
		(uVar4 = pCVar1->GetStateWolfenFlags(pCVar1->actorState), (uVar4 & 2) == 0)) {
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 | 0x1000;
	}

	uVar4 = pBehaviour->pOwner->GetStateWolfenFlags(pBehaviour->pOwner->actorState);
	if ((uVar4 & 1) == 0) goto LAB_001f47e8;

	if ((pBehaviour->pOwner->combatFlags_0xb78 & 0x10) == 0) {
	LAB_001f47b8:
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 & 0xfffffff8;
	}
	else {
		pCVar2 = pBehaviour->pOwner->pTargetActor_0xc80;
		fVar8 = pCVar2->GetLifeInterface()->GetValue();
		if (fVar8 <= 0.0f) goto LAB_001f47b8;

		pBehaviour->CheckDetection();
	}

	pBehaviour->pOwner->UpdateInRange_001744a0((pBehaviour->pOwner->combatFlags_0xb78 & 7) != 0);

LAB_001f47e8:
	if (((~pBehaviour->pOwner->combatFlags_0xb78 & 0x1800) == 0x1800) &&
		(iVar6 = pBehaviour->switchBehaviour.Test(pBehaviour->pOwner), iVar6 != 0)) {
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 | 0x800;
	}

	pCVar1 = pBehaviour->pOwner;
	iVar6 = -1;
	if ((pCVar1->combatFlags_0xb78 & 0x20000) == 0) {
		if (((pCVar1->pCollisionData)->flags_0x4 & 2) != 0) {
			pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 | 0x20000;
		}
	}
	else {
		if ((pCVar1->fightFlags & 2) != 0) {
			iVar6 = pCVar1->FUN_0030a6a0();
		}
	}

	if (iVar6 == -1) {
		pCVar1 = pBehaviour->pOwner;
		iVar6 = -1;
		if (((((~pCVar1->combatFlags_0xb78 & 0x30) == 0x30) &&
			(pCVar1->curBehaviourId != (pCVar1->subObjA)->defaultBehaviourId)) &&
			(pCVar2 = pCVar1->pAdversary, iVar6 = -1, pCVar2 != (CActorFighter*)0x0)) &&
			(bVar3 = pCVar2->IsKindOfObject(0x10), bVar3 == false))
		{
			pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 & 0xffffe7ff;
			pBehaviour->pOwner->SetCombatMode(ECM_None);
			iVar6 = pBehaviour->pOwner->subObjA->defaultBehaviourId;
		}

		iVar7 = -1;
		if (iVar6 == -1) {
			pCVar1 = pBehaviour->pOwner;
			if ((pCVar1->combatFlags_0xb78 & 0x1000) != 0) {
				pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 & 0xffffe7ff;
				iVar7 = pBehaviour->switchBehaviour.Execute(pBehaviour->pOwner);
			}

			if (iVar7 != -1) {
				SetBehaviour(iVar7, -1, -1);
			}
		}
		else {
			SetBehaviour(iVar6, -1, -1);
		}
	}
	else {
		SetBehaviour(iVar6, -1, -1);
	}

	return;
}

void CActorWolfen::BehaviourWatchDog_Manage(CBehaviourWatchDog* pBehaviour)
{
	CActorWolfen* pCVar1;
	CActorFighter* pCVar2;
	//CBehaviourMovingPlatformVTable* pCVar3;
	bool bVar4;
	uint uVar5;
	CLifeInterface* pCVar6;
	undefined4 uVar7;
	CBehaviour* pCVar8;
	S_TARGET_STREAM_REF* pSVar9;
	long lVar10;
	int iVar11;
	int iVar12;
	float fVar13;

	iVar12 = this->actorState;
	if (iVar12 == 0xa5) {
		IMPLEMENTATION_GUARD(
		StateWolfenBombShoot();)
	}
	else {
		if (iVar12 == 0xa4) {
			IMPLEMENTATION_GUARD(
			StateWolfenBombOrientTo(pBehaviour);)
		}
		else {
			if (iVar12 == 0xa3) {
				IMPLEMENTATION_GUARD(
				StateWolfenBombWalkTo(pBehaviour);)
			}
			else {
				if (iVar12 == 0xa2) {
					IMPLEMENTATION_GUARD(
					StateWolfenBombStand();)
				}
				else {
					if (iVar12 == 0xa1) {
						IMPLEMENTATION_GUARD(
						StateWolfenBombFlip();)
					}
					else {
						if (iVar12 == 0xa0) {
							IMPLEMENTATION_GUARD(
							StateWolfenInsultEnd(pBehaviour);)
						}
						else {
							if (iVar12 == 0x9f) {
								IMPLEMENTATION_GUARD(
								StateWolfenInsultReceive(pBehaviour);)
							}
							else {
								if (iVar12 == 0x9e) {
									IMPLEMENTATION_GUARD(
									StateWolfenInsultStand();)
								}
								else {
									if (iVar12 == 0x9d) {
										IMPLEMENTATION_GUARD(
										StateWolfenInsult(pBehaviour);)
									}
									else {
										if (iVar12 == 0x9b) {
											StateWolfen_00179db0(pBehaviour);
										}
										else {
											if (iVar12 == WOLFEN_STATE_SURPRISE) {
												StateWolfenSurprise(pBehaviour);
											}
											else {
												if (iVar12 == 0x98) {
													IMPLEMENTATION_GUARD(
													StateWolfenBoomyHit();)
												}
												else {
													if (iVar12 == WOLFEN_STATE_LOCATE) {
														StateWolfenLocate(pBehaviour);
													}
													else {
														if (iVar12 == 0x74) {
															IMPLEMENTATION_GUARD(
															ActorFunc_0035eac0();)
														}
														else {
															if (iVar12 == WOLFEN_STATE_COME_BACK) {
																StateWolfenComeBack(pBehaviour);
															}
															else {
																if (iVar12 == WOLFEN_STATE_WATCH_DOG_GUARD) {
																	StateWatchDogGuard(pBehaviour);
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	pCVar1 = pBehaviour->pOwner;
	if (((pCVar1->combatFlags_0xb78 & 0x800) != 0) &&
		(uVar5 = pCVar1->GetStateWolfenFlags(pCVar1->actorState), (uVar5 & 2) == 0)) {
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 | 0x1000;
	}

	uVar5 = pBehaviour->pOwner->GetStateWolfenFlags(pBehaviour->pOwner->actorState);

	if ((uVar5 & 1) == 0) goto LAB_001f42d8;

	if ((pBehaviour->pOwner->combatFlags_0xb78 & 0x10) == 0) {
	LAB_001f42a8:
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 & 0xfffffff8;
	}
	else {
		pCVar2 = pBehaviour->pOwner->pTargetActor_0xc80;
		fVar13 = pCVar2->GetLifeInterface()->GetValue();
		if (fVar13 <= 0.0f) goto LAB_001f42a8;

		pBehaviour->CheckDetection();
	}

	pBehaviour->pOwner->UpdateInRange_001744a0((pBehaviour->pOwner->combatFlags_0xb78 & 7) != 0);

LAB_001f42d8:
	bVar4 = (this->combatFlags_0xb78 & 4) != 0;
	if (bVar4 != pBehaviour->bool_0x68) {
		if ((bVar4) && (pBehaviour->bool_0x68 == false)) {
			for (int i = 0; i < pBehaviour->pTargetStreamRef->entryCount; i++) {
				pBehaviour->pTargetStreamRef->aEntries[i].SwitchOn(pBehaviour->pOwner);
			}

			pBehaviour->pCameraStreamEvent->SwitchOn(pCVar1);
		}

		if ((!bVar4) && (pBehaviour->bool_0x68 == true)) {
			for (int i = 0; i < pBehaviour->pTargetStreamRef->entryCount; i++) {
				pBehaviour->pTargetStreamRef->aEntries[i].SwitchOff(pBehaviour->pOwner);
			}
		}
	}

	pBehaviour->bool_0x68 = bVar4;
	if (((~pBehaviour->pOwner->combatFlags_0xb78 & 0x1800) == 0x1800) &&
		(iVar12 = pBehaviour->switchBehaviour.Test(pBehaviour->pOwner), iVar12 != 0)) {
		pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 | 0x800;
	}

	pCVar1 = pBehaviour->pOwner;
	iVar12 = -1;

	if ((pCVar1->combatFlags_0xb78 & 0x20000) == 0) {
		if (((pCVar1->pCollisionData)->flags_0x4 & 2) != 0) {
			pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 | 0x20000;
		}
	}
	else {
		if ((pCVar1->fightFlags & 2) != 0) {
			iVar12 = pCVar1->FUN_0030a6a0();
		}
	}

	if (iVar12 == -1) {
		pCVar1 = pBehaviour->pOwner;
		iVar12 = -1;
		if (((((~pCVar1->combatFlags_0xb78 & 0x30) == 0x30) && (pCVar1->curBehaviourId != (pCVar1->subObjA)->defaultBehaviourId)) &&
			(pCVar2 = pCVar1->pAdversary, iVar12 = -1, pCVar2 != (CActorFighter*)0x0)) && (bVar4 = pCVar2->IsKindOfObject(0x10), bVar4 == false)) {
			pBehaviour->pOwner->combatFlags_0xb78 = pBehaviour->pOwner->combatFlags_0xb78 & 0xffffe7ff;
			pBehaviour->pOwner->SetCombatMode(ECM_None);
			iVar12 = pBehaviour->pOwner->subObjA->defaultBehaviourId;
		}

		iVar11 = -1;
		if (iVar12 == -1) {
			pCVar1 = pBehaviour->pOwner;
			if ((pCVar1->combatFlags_0xb78 & 0x1000) != 0) {
				pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 & 0xffffe7ff;
				iVar11 = pBehaviour->switchBehaviour.Execute(pBehaviour->pOwner);
			}
			if (iVar11 == -1) {
				iVar12 = this->actorState;
				if (iVar12 == 0xb0) {
					SetBehaviour(3, -1, -1);
				}
				else {
					if (iVar12 == 0xb1) {
						SetBehaviour(pBehaviour->GetTrackBehaviour(), -1, -1);
					}
					else {
						if (iVar12 == 0xb4) {
							SetBehaviour(0x18, -1, -1);
						}
						else {
							if (iVar12 == 0xb5) {
								IMPLEMENTATION_GUARD(
								pCVar3 = (CBehaviourMovingPlatformVTable*)this->curBehaviourId;
								pCVar8 = CActor::GetBehaviour((CActor*)this, 0xe);
								pCVar8[2].pVTable = pCVar3;
								SetBehaviour(4, 0x5a, -1);)
							}
						}
					}
				}
			}
			else {
				SetBehaviour(iVar11, -1, -1);
			}
		}
		else {
			SetBehaviour(iVar12, -1, -1);
		}
	}
	else {
		SetBehaviour(iVar12, -1, -1);
	}

	return;
}

void CActorWolfen::BehaviourTrackWeapon_Manage(CBehaviourTrackWeapon* pBehaviour)
{
	CAnimation* pCVar1;
	edAnmLayer* peVar2;
	CActorFighter* pCVar3;
	//CBehaviourMovingPlatformVTable* pCVar4;
	bool bVar5;
	int iVar6;
	CLifeInterface* pCVar7;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* pCVar8;
	CBehaviour* pCVar9;
	float fVar10;

	iVar6 = this->actorState;

	if (iVar6 == 0x97) {
		this->dynamic.speed = 0.0f;
		ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
		pCVar1 = this->pAnimationController;
		peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
		bVar5 = false;

		if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
			if (peVar2->animPlayState == 0) {
				bVar5 = false;
			}
			else {
				bVar5 = (peVar2->field_0xcc & 2) != 0;
			}
		}

		if (bVar5) {
			SetState(this->field_0xcf8, -1);
		}
		else {
			iVar6 = pBehaviour->GetState_001f0930();
			if (iVar6 != -1) {
				SetState(iVar6, -1);
			}
		}
	}
	else {
		if (iVar6 == 0x96) {
			this->dynamic.speed = 0.0f;
			ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
			pCVar1 = this->pAnimationController;
			peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
			bVar5 = false;
			if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
				if (peVar2->animPlayState == 0) {
					bVar5 = false;
				}
				else {
					bVar5 = (peVar2->field_0xcc & 2) != 0;
				}
			}
			if (bVar5) {
				SetState(this->field_0xcf8, -1);
			}
			else {
				iVar6 = pBehaviour->GetState_001f0930();
				if (iVar6 != -1) {
					SetState(iVar6, -1);
				}
			}
		}
		else {
			if (iVar6 == WOLFEN_STATE_TRACK_WEAPON_CHECK_POSITION) {
				StateTrackCheckPosition(pBehaviour);
			}
			else {
				if (iVar6 == 0xaa) {
					IMPLEMENTATION_GUARD(
					FUN_0035d950(this, (CBehaviourWolfen*)pBehaviour);)
				}
				else {
					if (iVar6 == 0xa9) {
						IMPLEMENTATION_GUARD(
						FUN_0035dbb0(this, (CBehaviourWolfen*)pBehaviour);)
					}
					else {
						if (iVar6 == WOLFEN_STATE_RELOAD) {
							StateTrackWeaponReload(pBehaviour);
						}
						else {
							if (iVar6 == WOLFEN_STATE_FIRE) {
								StateTrackWeaponStandFire(pBehaviour);
							}
							else {
								if (iVar6 == WOLFEN_STATE_AIM) {
									StateTrackWeaponAim(pBehaviour);
								}
								else {
									if (iVar6 == 0x78) {
										IMPLEMENTATION_GUARD(
										FUN_0035e560(this, (CBehaviourWolfen*)pBehaviour);)
									}
									else {
										if (iVar6 == WOLFEN_STATE_TRACK_DEFEND) {
											StateTrackWeaponDefend(pBehaviour);
										}
										else {
											if (iVar6 == 0x74) {
												IMPLEMENTATION_GUARD(
												ActorFunc_0035eac0(this);)
											}
											else {
												if (iVar6 == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
													StateTrackWeaponChase(pBehaviour);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	pBehaviour->CheckDetection_Intruder();
	iVar6 = pBehaviour->FUN_001f0ab0();
	if (iVar6 == -1) {
		pCVar3 = this->pTargetActor_0xc80;
		if (pCVar3 != (CActorFighter*)0x0) {
			fVar10 = GetLifeInterface()->GetValue();
			if (fVar10 <= 0.0f) {
				SetBehaviour((this->subObjA)->defaultBehaviourId, -1, -1);
				return;
			}
		}
		iVar6 = this->actorState;
		if (iVar6 == 0xb3) {
			pCVar8 = pBehaviour->GetNotificationTargetArray();
			SetBehaviour(pCVar8->field_0x34, -1, -1);
		}
		else {
			if ((iVar6 == 0x9a) || (iVar6 == 0x9b)) {
				pCVar8 = pBehaviour->GetNotificationTargetArray();
				SetBehaviour(pCVar8->field_0x34, this->actorState, -1);
			}
			else {
				if (iVar6 == 0xb4) {
					SetBehaviour(0x18, -1, -1);
				}
				else {
					if (iVar6 == 0xb5) {
						IMPLEMENTATION_GUARD(
							pCVar4 = (CBehaviourMovingPlatformVTable*)this->curBehaviourId;
						pCVar9 = CActor::GetBehaviour((CActor*)this, 0xe);
						pCVar9[2].pVTable = pCVar4;
						SetBehaviour(4, 0x5a, -1););
					}
					else {
						if (iVar6 == 0xb0) {
							SetBehaviour(3, -1, -1);
						}
					}
				}
			}
		}
	}
	else {
		SetBehaviour(iVar6, -1, -1);
	}

	return;
}

void CActorWolfen::BehaviourTrackWeaponStand_Manage(CBehaviourTrackWeaponStand* pBehaviour)
{
	CAnimation* pCVar1;
	edAnmLayer* peVar2;
	CActorFighter* pCVar3;
	//CBehaviourMovingPlatformVTable* pCVar4;
	bool bVar5;
	int iVar6;
	CLifeInterface* pCVar7;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* pCVar8;
	CBehaviour* pCVar9;
	float fVar10;

	iVar6 = this->actorState;

	if (iVar6 == WOLFEN_STATE_TRACK_WEAPON_CHECK_POSITION) {
		StateTrackWeaponCheckPosition(pBehaviour);
	}
	else {
		if (iVar6 == 0x97) {
			this->dynamic.speed = 0.0f;
			ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
			pCVar1 = this->pAnimationController;
			peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
			bVar5 = false;

			if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
				if (peVar2->animPlayState == 0) {
					bVar5 = false;
				}
				else {
					bVar5 = (peVar2->field_0xcc & 2) != 0;
				}
			}

			if (bVar5) {
				SetState(this->field_0xcf8, -1);
			}
			else {
				iVar6 = pBehaviour->GetState_001f0930();
				if (iVar6 != -1) {
					SetState(iVar6, -1);
				}
			}
		}
		else {
			if (iVar6 == 0x96) {
				this->dynamic.speed = 0.0f;
				ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
				pCVar1 = this->pAnimationController;
				peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
				bVar5 = false;
				if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
					if (peVar2->animPlayState == 0) {
						bVar5 = false;
					}
					else {
						bVar5 = (peVar2->field_0xcc & 2) != 0;
					}
				}
				if (bVar5) {
					SetState(this->field_0xcf8, -1);
				}
				else {
					iVar6 = pBehaviour->GetState_001f0930();
					if (iVar6 != -1) {
						SetState(iVar6, -1);
					}
				}
			}
			else {
				if (iVar6 == WOLFEN_STATE_RELOAD) {
					StateTrackWeaponReload(pBehaviour);
				}
				else {
					if (iVar6 == WOLFEN_STATE_FIRE) {
						StateTrackWeaponStandFire(pBehaviour);
					}
					else {
						if (iVar6 == WOLFEN_STATE_AIM) {
							StateTrackStandAim(pBehaviour);
						}
						else {
							if (iVar6 == 0x78) {
								IMPLEMENTATION_GUARD(
									FUN_0035e560(this, (CBehaviourWolfen*)pBehaviour);)
							}
							else {
								if (iVar6 == WOLFEN_STATE_TRACK_DEFEND) {
									StateTrackWeaponStandDefend(pBehaviour);
								}
							}
						}
					}
				}
			}
		}
	}

	pBehaviour->CheckDetection_Intruder();
	iVar6 = pBehaviour->FUN_001f0ab0();
	if (iVar6 == -1) {
		pCVar3 = this->pTargetActor_0xc80;
		if (pCVar3 != (CActorFighter*)0x0) {
			fVar10 = GetLifeInterface()->GetValue();
			if (fVar10 <= 0.0f) {
				SetBehaviour((this->subObjA)->defaultBehaviourId, -1, -1);
				return;
			}
		}
		iVar6 = this->actorState;
		if (iVar6 == 0xb3) {
			pCVar8 = pBehaviour->GetNotificationTargetArray();
			SetBehaviour(pCVar8->field_0x34, -1, -1);
		}
		else {
			if ((iVar6 == 0x9a) || (iVar6 == 0x9b)) {
				pCVar8 = pBehaviour->GetNotificationTargetArray();
				SetBehaviour(pCVar8->field_0x34, this->actorState, -1);
			}
			else {
				if (iVar6 == 0xb4) {
					SetBehaviour(0x18, -1, -1);
				}
				else {
					if (iVar6 == 0xb5) {
						IMPLEMENTATION_GUARD(
							pCVar4 = (CBehaviourMovingPlatformVTable*)this->curBehaviourId;
						pCVar9 = CActor::GetBehaviour((CActor*)this, 0xe);
						pCVar9[2].pVTable = pCVar4;
						SetBehaviour(4, 0x5a, -1););
					}
					else {
						if (iVar6 == 0xb0) {
							SetBehaviour(3, -1, -1);
						}
					}
				}
			}
		}
	}
	else {
		SetBehaviour(iVar6, -1, -1);
	}

	return;
}

int CActorWolfen::_waitStandAnimArray[8] =
{
	0x96,
	0x97,
	0x98,
	0x99,
	0x9a,
	0x9b,
	-1
};

int CActorWolfen::_waitDefendAnimArray[4] =
{
	0x8e,
	0x8f,
	0x90,
	-1
};

void CActorWolfen::WaitingAnimation_Defend()
{
	int inAnimType;
	int iVar1;
	int iVar2;
	ulong uVar3;

	uVar3 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
	CScene::_pinstance->field_0x38 = uVar3;

	inAnimType = _waitDefendAnimArray[((uint)(uVar3 >> 0x10) & 0x7fff) % 3];
	iVar1 = GetIdMacroAnim(inAnimType);
	if (iVar1 != -1) {
		iVar1 = rand();
		iVar2 = rand();
		PlayWaitingAnimation(((float)iVar2 / 2.147484e+09f) * 3.0f + 2.0f, ((float)iVar1 / 2.147484e+09f) * 0.7f + 0.8f, inAnimType, -1, 1);
	}
	return;
}

void CActorWolfen::WaitingAnimation_Guard()
{
	ulong uVar3 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
	CScene::_pinstance->field_0x38 = uVar3;

	int newState = _waitStandAnimArray[(((uint)(uVar3 >> 0x10) & 0x7fff) % 7)];
	int iVar1 = GetIdMacroAnim(newState);
	if (iVar1 != -1) {
		iVar1 = rand();
		int iVar2 = rand();
		PlayWaitingAnimation(((float)iVar2 / 2.147484e+09f) * 10.0f + 5.0f, ((float)iVar1 / 2.147484e+09f) * 0.7f + 0.8f, newState, -1, 1);
	}
}

void CActorWolfen::StateStandGuard()
{
	int inAnimType;
	int iVar1;
	int iVar2;
	ulong uVar3;

	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
	uVar3 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
	CScene::_pinstance->field_0x38 = uVar3;
	inAnimType = _waitStandAnimArray[((uint)(uVar3 >> 0x10) & 0x7fff) % 7];
	iVar1 = GetIdMacroAnim(inAnimType);
	if (iVar1 != -1) {
		iVar1 = rand();
		iVar2 = rand();
		PlayWaitingAnimation(((float)iVar2 / 2.147484e+09f) * 10.0f + 5.0f, ((float)iVar1 / 2.147484e+09f) * 0.7f + 0.8f, inAnimType, -1, 1);
	}

	return;
}

void CActorWolfen::StateTrackWeaponStandDefend(CBehaviourTrackWeaponStand* pBehaviour)
{
	CActorCommander* pCommander;
	float fVar2;
	float fVar3;
	bool bVar4;
	int iVar7;
	float fVar8;
	edF32VECTOR4 targetDirection;

	fVar8 = GetWalkRotSpeed();
	edF32Vector4SubHard(&targetDirection, &this->pCommander->targetPosition, &this->currentLocation);
	edF32Vector4NormalizeHard(&targetDirection, &targetDirection);
	bVar4 = SV_WLF_UpdateOrientation2D(fVar8, &targetDirection, 0);
	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	if (0 < this->pCommander->field_0x194) {
		WaitingAnimation_Defend();
		pBehaviour->field_0xec = Timer::GetTimer()->scaledTotalTime;
	}

	if ((bVar4 != true) || (Timer::GetTimer()->scaledTotalTime - (float)pBehaviour->field_0xec <= 4.0f)) {
		pCommander = this->pCommander;
		if ((pCommander->field_0x194 < 1) ||
			(fVar8 = (pCommander->targetPosition).x - this->currentLocation.x,
				fVar2 = (pCommander->targetPosition).y - this->currentLocation.y,
				fVar3 = (pCommander->targetPosition).z - this->currentLocation.z,
				pBehaviour->field_0x90 <= sqrtf(fVar8 * fVar8 + fVar2 * fVar2 + fVar3 * fVar3))) {
			iVar7 = pBehaviour->GetState_001f0930();
			if (iVar7 != -1) {
				SetState(iVar7, -1);
			}
		}
		else {
			SetState(pBehaviour->Func_0x74(), -1);
		}
	}
	else {
		SetState(pBehaviour->Func_0x78(), -1);
	}

	return;
}

void CActorWolfen::StateTrackWeaponStandFire(CBehaviourTrackWeaponStand* pBehaviour)
{
	uint uVar1;
	CAnimation* pCVar2;
	edAnmLayer* peVar3;
	bool bVar4;
	CActorWeapon* pCVar5;
	CActor* pCVar6;
	Timer* pTVar7;
	int iVar8;
	CActor* pOtherActor;
	long nextAnim;
	long nextState;
	float fVar11;
	float fVar12;
	edF32VECTOR4 eStack96;
	edF32VECTOR4 local_50;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 local_20;
	edF32VECTOR4 eStack16;

	nextState = -1;

	pOtherActor = this->pTargetActor_0xc80;
	if (pOtherActor != (CActor*)0x0) {
		fVar11 = GetRunRotSpeed();
		edF32Vector4SubHard(&eStack16, &pOtherActor->currentLocation, &this->currentLocation);
		edF32Vector4NormalizeHard(&eStack16, &eStack16);
		SV_WLF_UpdateOrientation2D(fVar11, &eStack16, 0);
		uVar1 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0;

		pCVar5 = GetWeapon();
		SV_GetActorHitPos(pOtherActor, &local_20);

		if ((uVar1 & 1) != 0) {
			edF32Vector4SubHard(&eStack48, &local_20, &pCVar5->currentLocation);
			fVar11 = edF32Vector4GetDistHard(&eStack48);
			fVar12 = pCVar5->FUN_002d5710();
			pCVar6 = pOtherActor->GetCollidingActor();
			do {
				if (pCVar6 == pOtherActor) {
					pOtherActor = (CActor*)0x0;
				}
				else {
					bVar4 = pOtherActor->IsKindOfObject(2);
					if (bVar4 != false) {
						CActorMovable* pMovable = static_cast<CActorMovable*>(pOtherActor);
						edF32Vector4ScaleHard((fVar11 / fVar12) * pMovable->dynamic.linearAcceleration, &eStack96, &pMovable->dynamic.velocityDirectionEuler);
						edF32Vector4AddHard(&local_20, &local_20, &eStack96);
					}

					pOtherActor = pOtherActor->pTiedActor;
				}
			} while (pOtherActor != (CActor*)0x0);
		}

		fVar11 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x8;
		local_50 = local_20;

		if ((pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0 & 2) != 0) {
			edF32Vector4SubHard(&eStack64, &local_50, &pBehaviour->field_0x80);
			fVar12 = edF32Vector4GetDistHard(&eStack64);
			pTVar7 = Timer::GetTimer();
			fVar11 = fVar11 * pTVar7->cutsceneDeltaTime;
			if (fVar11 <= fVar12) {
				fVar12 = fVar11;
			}

			fVar11 = edF32Vector4SafeNormalize0Hard(&eStack64, &eStack64);
			if (fVar11 != 0.0f) {
				edF32Vector4ScaleHard(fVar12, &eStack64, &eStack64);
				edF32Vector4AddHard(&local_50, &pBehaviour->field_0x80, &eStack64);
			}
		}

		pBehaviour->field_0x80 = local_50;
	}

	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	iVar8 = GetWeapon()->GetBurstState();
	if (iVar8 == 3) {
		nextState = 0x91;
		nextAnim = -1;
	}
	else {
		if (iVar8 == 1) {
			nextState = WOLFEN_STATE_AIM;
			nextAnim = 0xa8;
		}
		else {
			nextAnim = nextState;
			if (pBehaviour->field_0x94 == 2) {
				pCVar2 = this->pAnimationController;
				peVar3 = (pCVar2->anmBinMetaAnimator).aAnimData;
				bVar4 = false;
				if ((peVar3->currentAnimDesc).animType == pCVar2->currentAnimType_0x30) {
					if (peVar3->animPlayState == 0) {
						bVar4 = false;
					}
					else {
						bVar4 = (peVar3->field_0xcc & 2) != 0;
					}
				}

				if (bVar4) {
					PlayAnim(0xaa);
					nextAnim = -1;
				}
			}
		}
	}
	if (nextState == -1) {
		iVar8 = pBehaviour->GetState_001f08a0();
		if (iVar8 != -1) {
			SetState(iVar8, -1);
		}
	}
	else {
		SetState(nextState, nextAnim);
	}

	return;
}

void CActorWolfen::StateTrackStandAim(CBehaviourTrackWeaponStand* pBehaviour)
{
	CActorCommander* pCVar1;
	uint uVar2;
	CAnimation* pCVar3;
	edAnmLayer* peVar4;
	float fVar5;
	bool bVar6;
	bool bVar7;
	bool bVar8;
	int iVar9;
	CActorWeapon* pWeapon;
	CActor* pCVar10;
	Timer* pTVar11;
	ulong uVar12;
	edF32VECTOR4* v2;
	CActor* pOtherActor;
	float fVar13;
	float fVar14;
	edF32VECTOR4 eStack128;
	edF32VECTOR4 local_70;
	edF32VECTOR4 eStack96;
	edF32VECTOR4 eStack80;
	edF32VECTOR4 local_40;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 local_10;

	GetVision();
	pOtherActor = this->pTargetActor_0xc80;
	bVar7 = false;
	fVar13 = GetRunRotSpeed();
	edF32Vector4SubHard(&eStack32, &this->pCommander->targetPosition, &this->currentLocation);
	edF32Vector4NormalizeHard(&eStack32, &eStack32);
	bVar6 = SV_WLF_UpdateOrientation2D(fVar13, &eStack32, 0);

	if ((bVar6 == true) && (this->pCommander->field_0x194 < 1)) {
		SetState(pBehaviour->GetStateWolfenWeapon(), -1);
	}
	else {
		if (pOtherActor != (CActorFighter*)0x0) {
			int iVar8 = pBehaviour->GetState_001f0930();
			if (iVar8 == -1) {
				bVar6 = CanSwitchToFight_Area(pOtherActor);
				if (bVar6 == false) {
					bVar7 = true;
				}
				else {
					bVar6 = this->pCommander->BeginFightIntruder(this, pOtherActor);
					if (bVar6 != false) {
						Func_0x204(static_cast<CActorFighter*>(pOtherActor));
						bVar6 = SetBehaviour(3, -1, -1);
						if (bVar6 == false) {
							this->pCommander->EndFightIntruder(this);
						}
					}
				}
			}
			else {
				SetState(iVar8, -1);
			}
		}
	}

	if ((((bVar7) && ((this->combatFlags_0xb78 & 4) != 0)) && (pOtherActor != (CActorFighter*)0x0)) &&
		(pCVar1 = this->pCommander,
			fVar13 = (pCVar1->targetPosition).x - this->currentLocation.x,
			fVar14 = (pCVar1->targetPosition).y - this->currentLocation.y,
			fVar5 = (pCVar1->targetPosition).z - this->currentLocation.z,
			sqrtf(fVar13 * fVar13 + fVar14 * fVar14 + fVar5 * fVar5) <= pBehaviour->field_0x90)) {
		uVar2 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0;
		pWeapon = GetWeapon();
		SV_GetActorHitPos(pOtherActor, &local_40);
		if ((uVar2 & 1) != 0) {
			edF32Vector4SubHard(&eStack80, &local_40, &pWeapon->currentLocation);
			fVar13 = edF32Vector4GetDistHard(&eStack80);
			fVar14 = pWeapon->FUN_002d5710();
			pCVar10 = pOtherActor->GetCollidingActor();

			do {
				if (pCVar10 == pOtherActor) {
					pOtherActor = (CActorFighter*)0x0;
				}
				else {
					bVar8 = pOtherActor->IsKindOfObject(2);
					if (bVar8 != false) {
						CActorMovable* pMovable = static_cast<CActorMovable*>(pOtherActor);
						edF32Vector4ScaleHard((fVar13 / fVar14) * pMovable->dynamic.linearAcceleration, &eStack128, &pMovable->dynamic.velocityDirectionEuler);
						edF32Vector4AddHard(&local_40, &local_40, &eStack128);
					}

					pOtherActor = pOtherActor->pTiedActor;
				}
			} while (pOtherActor != (CActorFighter*)0x0);
		}

		local_70 = local_40;

		fVar13 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x8;
		if ((pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0 & 2) != 0) {
			local_10 = local_70;

			edF32Vector4SubHard(&eStack96, &local_70, &pBehaviour->field_0x80);
			fVar14 = edF32Vector4GetDistHard(&eStack96);
			fVar13 = fVar13 * Timer::GetTimer()->cutsceneDeltaTime;
			if (fVar13 <= fVar14) {
				fVar14 = fVar13;
			}

			fVar13 = edF32Vector4SafeNormalize0Hard(&eStack96, &eStack96);
			if (fVar13 != 0.0f) {
				edF32Vector4ScaleHard(fVar14, &eStack96, &eStack96);
				edF32Vector4AddHard(&local_70, &pBehaviour->field_0x80, &eStack96);
			}
		}

		local_10 = local_70;
		pBehaviour->field_0x80 = local_70;

		fVar14 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x4;
		GetWeapon();
		fVar13 = fVar14 * 0.5f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.x = (local_10.x - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.y = (local_10.y - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.w = 1.0f;
		local_10.z = (local_10.z - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;

		iVar9 = GetWeapon()->GetBurstState();
		if (iVar9 == 1) {
			iVar9 = GetWeapon()->Action(&local_10, this);
			if (iVar9 != 0) {
				SetState(WOLFEN_STATE_FIRE, -1);
			}
		}
	}

	pCVar3 = this->pAnimationController;
	peVar4 = (pCVar3->anmBinMetaAnimator).aAnimData;
	if ((peVar4->currentAnimDesc).animType == pCVar3->currentAnimType_0x30) {
		bVar8 = false;
		if (peVar4->animPlayState != 0) {
			bVar8 = (peVar4->field_0xcc & 2) != 0;
		}
	}
	else {
		bVar8 = false;
	}

	if (bVar8) {
		PlayAnim(0xaa);
	}

	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	return;
}

void CActorWolfen::StateTrackWeaponReload(CBehaviourTrackWeaponStand* pBehaviour)
{
	uint uVar1;
	CAnimation* pCVar2;
	edAnmLayer* peVar3;
	bool bVar4;
	CActorWeapon* pCVar5;
	CActor* pCVar6;
	Timer* pTVar7;
	int iVar8;
	CActor* pOtherActor;
	long nextState;
	float fVar11;
	float fVar12;
	edF32VECTOR4 eStack96;
	edF32VECTOR4 local_50;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 local_20;
	edF32VECTOR4 eStack16;

	pOtherActor = this->pTargetActor_0xc80;

	nextState = -1;

	if (pOtherActor != (CActor*)0x0) {
		fVar11 = GetRunRotSpeed();
		SV_UpdateOrientationToPosition2D(fVar11, &pOtherActor->currentLocation);
		uVar1 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0;

		pCVar5 = GetWeapon();
		SV_GetActorHitPos(pOtherActor, &local_20);

		if ((uVar1 & 1) != 0) {
			edF32Vector4SubHard(&eStack48, &local_20, &pCVar5->currentLocation);
			fVar11 = edF32Vector4GetDistHard(&eStack48);
			fVar12 = pCVar5->FUN_002d5710();
			pCVar6 = pOtherActor->GetCollidingActor();
			do {
				if (pCVar6 == pOtherActor) {
					pOtherActor = (CActor*)0x0;
				}
				else {
					bVar4 = pOtherActor->IsKindOfObject(2);
					if (bVar4 != false) {
						CActorMovable* pMovable = static_cast<CActorMovable*>(pOtherActor);
						edF32Vector4ScaleHard((fVar11 / fVar12) * pMovable->dynamic.linearAcceleration, &eStack96, &pMovable->dynamic.velocityDirectionEuler);
						edF32Vector4AddHard(&local_20, &local_20, &eStack96);
					}

					pOtherActor = pOtherActor->pTiedActor;
				}
			} while (pOtherActor != (CActor*)0x0);
		}

		fVar11 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x8;
		local_50 = local_20;

		if ((pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0 & 2) != 0) {
			edF32Vector4SubHard(&eStack64, &local_50, &pBehaviour->field_0x80);
			fVar12 = edF32Vector4GetDistHard(&eStack64);
			pTVar7 = Timer::GetTimer();
			fVar11 = fVar11 * pTVar7->cutsceneDeltaTime;
			if (fVar11 <= fVar12) {
				fVar12 = fVar11;
			}

			fVar11 = edF32Vector4SafeNormalize0Hard(&eStack64, &eStack64);
			if (fVar11 != 0.0f) {
				edF32Vector4ScaleHard(fVar12, &eStack64, &eStack64);
				edF32Vector4AddHard(&local_50, &pBehaviour->field_0x80, &eStack64);
			}
		}

		pBehaviour->field_0x80 = local_50;
	}

	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	iVar8 = GetWeapon()->GetBurstState();
	if (iVar8 == 1) {
		nextState = WOLFEN_STATE_AIM;

		if (this->pCommander->field_0x194 < 1) {
			if (this->curBehaviourId == 0x11) {
				nextState = pBehaviour->Func_0x70();
			}
			else {
				nextState = pBehaviour->GetStateWolfenWeapon();
			}
		}
	}
	else {
		if (pBehaviour->field_0x94 == 2) {
			pCVar2 = this->pAnimationController;
			peVar3 = (pCVar2->anmBinMetaAnimator).aAnimData;
			bVar4 = false;
			if ((peVar3->currentAnimDesc).animType == pCVar2->currentAnimType_0x30) {
				if (peVar3->animPlayState == 0) {
					bVar4 = false;
				}
				else {
					bVar4 = (peVar3->field_0xcc & 2) != 0;
				}
			}

			if (bVar4) {
				PlayAnim(0xaa);
			}
		}
	}

	if (nextState == -1) {
		iVar8 = pBehaviour->GetState_001f08a0();
		if (iVar8 != -1) {
			SetState(iVar8, -1);
		}
	}
	else {
		SetState(nextState, -1);
	}

	return;
}

void CActorWolfen::StateWolfenComeBack(CBehaviourWolfen* pBehaviour)
{
	char cVar1;
	bool bVar2;
	edF32VECTOR4* pComeBackPosition;
	int nextState;
	undefined4 uVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	CActorsTable actorsTable;
	CActorMovParamsIn actorMovParamsIn;
	CActorMovParamsOut actorMovParamsOut;

	actorMovParamsOut.flags = 0;
	actorMovParamsIn.flags = 0;
	actorMovParamsIn.pRotation = (edF32VECTOR4*)0x0;
	actorMovParamsIn.speed = 0.0f;

	pComeBackPosition = pBehaviour->GetComeBackPosition();

	if (sqrtf(edF32Vector4DotProductHard_I(pComeBackPosition, &this->currentLocation)) < 0.5f) goto LAB_0017ae10;

	actorMovParamsIn.flags = actorMovParamsIn.flags | 0x150;
	if (this->currentAnimType == 7) {
		actorMovParamsIn.rotSpeed = GetWalkRotSpeed();
		actorMovParamsIn.flags = actorMovParamsIn.flags | 2;
		actorMovParamsIn.acceleration = GetWalkAcceleration();
		actorMovParamsIn.speed = GetWalkSpeed();
	}
	else {
		actorMovParamsIn.rotSpeed = GetRunRotSpeed();
		actorMovParamsIn.flags = actorMovParamsIn.flags | 2;
		actorMovParamsIn.acceleration = GetRunAcceleration();
		actorMovParamsIn.speed = GetRunSpeed();
	}

	actorMovParamsIn.flags = actorMovParamsIn.flags | 0x400;

	pComeBackPosition = pBehaviour->GetComeBackPosition();
	if ((this->combatFlags_0xb78 & 0x400) == 0) {
	LAB_0017ad88:
		bVar2 = false;
	}
	else {
		if (GetPathfinderClientAlt()->id != -1) {
			if (GetPathfinderClientAlt()->IsValidPosition(&this->currentLocation) == false) goto LAB_0017ad88;
		}

		bVar2 = true;
	}

	if (bVar2) {
		if ((this->combatFlags_0xb78 & 0x80000) == 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x80000;
		}

		this->pathOriginPosition = this->currentLocation;
	}

	SV_AUT_MoveTo(&actorMovParamsOut, &actorMovParamsIn, pComeBackPosition);

	if ((actorMovParamsOut.flags & 2) != 0) {
		this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x40000;
	}
LAB_0017ae10:
	actorsTable.entryCount = 0;
	ManageDyn(4.0f, 0x1002023b, &actorsTable);

	nextState = SV_WLF_CheckBoxOnWay(&actorsTable);
	if (nextState == -1) {
		nextState = pBehaviour->TestState_001f0a90();
		if (nextState == -1) {
			nextState = pBehaviour->TestState_001f0a70();
			if (nextState == -1) {
				nextState = pBehaviour->TestState_001f0a30();
				if (nextState == -1) {
					pComeBackPosition = pBehaviour->GetComeBackPosition();
					if (0.5f <= sqrtf(edF32Vector4DotProductHard_I(pComeBackPosition, &this->currentLocation))) {
						if ((this->pTargetActor_0xc80 != (CActorFighter*)0x0) &&
							(nextState = pBehaviour->TestState_001f09b0(), nextState != -1)) {
							SetState(nextState, -1);
						}
					}
					else {
						SetState(pBehaviour->GetStateWolfenGuard(), -1);
					}
				}
				else {
					SetState(nextState, -1);
				}
			}
			else {
				SetState(nextState, -1);
			}
		}
		else {
			SetState(nextState, -1);
		}
	}
	else {
		SetState(nextState, -1);
	}

	return;
}

void CActorWolfen::StateWolfen_00179db0(CBehaviourWolfen* pBehaviour)
{
	CAnimation* pCVar1;
	edAnmLayer* peVar2;
	bool bVar3;
	int iVar4;

	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	iVar4 = pBehaviour->GetState_001f0930();
	if (iVar4 == -1) {
		if ((int)this->combatMode_0xb7c < 2) {
			pCVar1 = this->pAnimationController;
			peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
			bVar3 = false;
			if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
				if (peVar2->animPlayState == 0) {
					bVar3 = false;
				}
				else {
					bVar3 = (peVar2->field_0xcc & 2) != 0;
				}
			}

			if (bVar3) {
				SetState(WOLFEN_STATE_LOCATE, -1);
			}
		}
		else {
			SetState(0x9a, -1);
		}
	}
	else {
		SetState(iVar4, -1);
	}

	return;
}

void CActorWolfen::StateWolfenSurprise(CBehaviourWolfen* pBehaviour)
{
	CAnimation* pCVar1;
	edAnmLayer* peVar2;
	bool bVar3;
	int iVar4;
	StateConfig* pSVar5;
	int iVar6;
	edF32VECTOR4* v1;
	float fVar7;
	float fVar8;
	edF32VECTOR4 eStack80;
	undefined4 local_40;
	undefined4 local_38;
	undefined4 local_34;
	undefined4 local_c;

	local_c = 0;
	local_40 = 0;
	local_38 = 0;
	local_34 = 0;

	v1 = (edF32VECTOR4*)0x0;

	if ((int)this->combatMode_0xb7c < 1) {
		if (((this->combatFlags_0xb78 & 0x80) == 0) || (this->pTrackedProjectile == (CActorProjectile*)0x0)) {
			if (this->field_0xd04 != (CActor*)0x0) {
				v1 = &this->field_0xd04->currentLocation;
			}
		}
		else {
			v1 = &this->pTrackedProjectile->currentLocation;
		}
	}
	else {
		if (this->pTargetActor_0xc80 != (CActorFighter*)0x0) {
			v1 = &this->pTargetActor_0xc80->currentLocation;
		}
	}

	if ((v1 != (edF32VECTOR4*)0x0) && (this->actorState == 0x9a)) {
		edF32Vector4SubHard(&eStack80, v1, &this->currentLocation);
		edF32Vector4NormalizeHard(&eStack80, &eStack80);
		fVar7 = GetAngleYFromVector(&eStack80);
		fVar7 = edF32Between_Pi(fVar7 - this->field_0xcfc);
		pCVar1 = this->pAnimationController;
		iVar4 = GetIdMacroAnim(this->currentAnimType);
		if (iVar4 < 0) {
			fVar8 = 0.0f;
		}
		else {
			fVar8 = pCVar1->GetAnimLength(iVar4, 1);
		}

		if (fVar7 <= 0.0f) {
			fVar7 = -fVar7;
		}

		SV_UpdateOrientationToPosition2D(fVar7 / fVar8, v1);
	}

	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	iVar4 = pBehaviour->GetState_001f0930();
	if (iVar4 == -1) {
		pCVar1 = this->pAnimationController;
		peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;

		if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
			bVar3 = false;
			if (peVar2->animPlayState != 0) {
				bVar3 = (peVar2->field_0xcc & 2) != 0;
			}
		}
		else {
			bVar3 = false;
		}

		if (bVar3) {
			if ((int)this->combatMode_0xb7c < 1) {
				if (((this->combatFlags_0xb78 & 0x80) == 0) || (v1 == (edF32VECTOR4*)0x0)) {
					if (this->field_0xd04 != (CActor*)0x0) {
						iVar4 = 0x9d;
					}
				}
				else {
					if (this->pTrackedProjectile != (CActorProjectile*)0x0) {
						this->pTrackedProjectile->GetTimeToExplode();

						pCVar1 = this->pAnimationController;
						iVar6 = GetIdMacroAnim(GetStateCfg(0xa5)->animId);
						if (-1 < iVar6) {
							pCVar1->GetAnimLength(iVar6, 0);
						}
						
						GetRunSpeed();

						if ((pBehaviour->flags_0x4 & 0x20) == 0) {
							iVar4 = 0xa1;
						}
						else {
							if (this->pTrackedProjectile->field_0x40c == (CActor*)0x0) {
								iVar4 = 0xa3;
								this->pTrackedProjectile->field_0x40c = this;
							}
						}
					}
				}
			}
			else {
				if (1 < (int)this->combatMode_0xb7c) {
					SV_AUT_WarnActors(this->field_0xcf4, 0.0f, this->pTargetActor_0xc80);
				}

				iVar4 = pBehaviour->GetStateWolfenTrack();
			}

			if (iVar4 == -1) {
				SetState(WOLFEN_STATE_LOCATE, -1);
			}
			else {
				SetState(iVar4, -1);
			}

			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffff7f;
		}
	}
	else {
		SetState(iVar4, -1);
	}

	return;
}

void CActorWolfen::StateWolfenLocate(CBehaviourWolfen* pBehaviour)
{
	CAnimation* pCVar1;
	edAnmLayer* peVar2;
	bool bVar3;
	float fVar4;
	float fVar5;
	CActorWolfen* pScannedWolfen;
	int iVar7;
	int* piVar9;
	Timer* pTVar10;
	int lVar11;
	long lVar12;
	int iVar13;
	float fVar14;
	CActorsTable local_220;
	CActorsTable local_110;

	this->dynamic.speed = 0.0;
	iVar7 = -1;
	
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	InternState_WolfenLocate();

	if (((this->combatFlags_0xb78 & 4) == 0) || ((int)this->combatMode_0xb7c < 1)) {
		if ((this->combatFlags_0xb78 & 0x80) != 0) {
			if ((this->field_0xb74 == 1) || (this->field_0xb74 == 0)) {
				local_110.entryCount = 0;
				GetVision()->ScanFromClassID(PROJECTILE, &local_110, 1);
				iVar13 = 0;
				if (local_110.entryCount != 0) {
					for (; iVar13 < local_110.entryCount; iVar13 = iVar13 + 1) {
						if (local_110.aEntries[iVar13]->actorState == 0xc) {
							this->pTrackedProjectile = static_cast<CActorProjectile*>(local_110.aEntries[iVar13]);
							iVar7 = 0x9a;
							break;
						}
					}
				}
			}
			else {
				this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xffffff7f;
			}
		}
	}
	else {
		iVar7 = pBehaviour->GetState_001f0b30();
	}

	if ((this->combatFlags_0xb78 & 0x40) != 0) {
		local_220.entryCount = 0;
		GetVision()->ScanFromClassID(WOLFEN, &local_220, 1);
		for (iVar13 = 0; pScannedWolfen = static_cast<CActorWolfen*>(local_220.aEntries[0]), iVar13 < local_220.entryCount; iVar13 = iVar13 + 1) {
			fVar14 = pScannedWolfen->GetLifeInterface()->GetValue();
			if ((0.0f < fVar14) &&
				(fVar14 = pScannedWolfen->currentLocation.x - this->currentLocation.x,
					fVar4 = pScannedWolfen->currentLocation.y - this->currentLocation.y,
					fVar5 = pScannedWolfen->currentLocation.z - this->currentLocation.z,
					sqrtf(fVar14 * fVar14 + fVar4 * fVar4 + fVar5 * fVar5) <= 5.0f)) {
				pTVar10 = Timer::GetTimer();
				if ((this->field_0xd04 == (CActor*)0x0) || (15.0f < pTVar10->scaledTotalTime - this->field_0xd08)) {
					pTVar10 = Timer::GetTimer();
					this->field_0xd08 = pTVar10->scaledTotalTime;
				}

				this->field_0xd04 = pScannedWolfen;
			}
		}
	}

	if (iVar7 == -1) {
		pCVar1 = this->pAnimationController;
		peVar2 = (pCVar1->anmBinMetaAnimator).aAnimData;
		if ((peVar2->currentAnimDesc).animType == pCVar1->currentAnimType_0x30) {
			bVar3 = false;
			if (peVar2->animPlayState != 0) {
				bVar3 = (peVar2->field_0xcc & 2) != 0;
			}
		}
		else {
			bVar3 = false;
		}

		if (bVar3) {
			if ((int)this->combatMode_0xb7c < 1) {
				lVar11 = pBehaviour->GetStateWolfenComeBack();
			}
			else {
				lVar11 = pBehaviour->GetStateWolfenTrack();
			}

			if ((((this->combatFlags_0xb78 & 0x40) != 0) && ((pBehaviour->flags_0x4 & 0x10) != 0)) && (this->field_0xd04 != (CActor*)0x0)
				) {
				pTVar10 = Timer::GetTimer();
				if (pTVar10->scaledTotalTime - this->field_0xd08 < this->timeInAir) {
					lVar11 = 0x9a;
				}
				else {
					IMPLEMENTATION_GUARD(
					lVar12 = (*(code*)this->pVTable[1].field_0x4)(this, this->field_0xd04);
					if (lVar12 == 0) {
						(*(code*)this->pVTable[1].actorBase)(this, this->field_0xd04);
						(*(code*)(this->pVTable)->SetFightBehaviour)(this);
						lVar11 = -1;
					}
					this->field_0xd04 = (CActor*)0x0;)
				}
			}

			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffe3f;

			if (lVar11 != -1) {
				SetState((int)lVar11, -1);
			}
		}
		else {
			iVar7 = pBehaviour->GetState_001f0930();
			if (iVar7 != -1) {
				SetState(iVar7, -1);
			}
		}
	}
	else {
		this->field_0xd08 = 0.0f;
		this->field_0xd04 = (CActor*)0x0;
		this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffebf;
		SetState(iVar7, -1);
	}

	return;
}

void CActorWolfen::StateWatchDogGuard(CBehaviourWatchDog* pBehaviour)
{
	edF32VECTOR4* rotEuler;
	int iVar1;
	int iVar2;
	int newState;
	ulong uVar3;
	float fVar4;
	edF32VECTOR4 eStack16;

	if ((pBehaviour->flags_0x4 & 8) != 0) {
		rotEuler = pBehaviour->GetComeBackAngles();
		SetVectorFromAngles(&eStack16, &rotEuler->xyz);
		SV_WLF_UpdateOrientation2D(GetWalkRotSpeed(), &eStack16, 0);
	}

	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	WaitingAnimation_Guard();

	newState = pBehaviour->TestState_001f09b0();
	if (newState == -1) {
		/* Not in combat. */
		newState = pBehaviour->TestState_001f0a90();
		if (newState == -1) {
			newState = pBehaviour->TestState_001f0a70();
			if (newState == -1) {
				newState = pBehaviour->TestState_001f0a30();
				if (newState == -1) {
					newState = pBehaviour->TestState_001f0a00();
					if (newState != -1) {
						SetState(newState, -1);
					}
				}
				else {
					SetState(newState, -1);
				}
			}
			else {
				SetState(newState, -1);
			}
		}
		else {
			SetState(newState, -1);
		}
	}
	else {
		SetState(newState, -1);
	}

	return;
}

void CActorWolfen::StateTrackWeaponAim(CBehaviourTrackWeapon* pBehaviour)
{
	CActorCommander* pCVar1;
	uint uVar2;
	CAnimation* pCVar3;
	edAnmLayer* peVar4;
	float fVar5;
	bool bVar6;
	bool bVar7;
	bool bVar8;
	int iVar9;
	CActorWeapon* pWeapon;
	CActor* pCVar10;
	Timer* pTVar11;
	ulong uVar12;
	edF32VECTOR4* v2;
	CActor* pOtherActor;
	float fVar13;
	float fVar14;
	edF32VECTOR4 eStack128;
	edF32VECTOR4 local_70;
	edF32VECTOR4 eStack96;
	edF32VECTOR4 eStack80;
	edF32VECTOR4 local_40;
	edF32VECTOR4 eStack32;
	edF32VECTOR4 local_10;

	GetVision();
	pOtherActor = this->pTargetActor_0xc80;
	bVar8 = false;
	fVar13 = GetRunRotSpeed();
	edF32Vector4SubHard(&eStack32, &this->pCommander->targetPosition, &this->currentLocation);
	edF32Vector4NormalizeHard(&eStack32, &eStack32);
	bVar6 = SV_WLF_UpdateOrientation2D(fVar13, &eStack32, 0);
	pCVar1 = this->pCommander;
	if (((pCVar1->field_0x194 < 1) ||
		(fVar13 = (pCVar1->targetPosition).x - this->currentLocation.x,
			fVar14 = (pCVar1->targetPosition).y - this->currentLocation.y,
			fVar5 = (pCVar1->targetPosition).z - this->currentLocation.z,
			sqrt(fVar13 * fVar13 + fVar14 * fVar14 + fVar5 * fVar5) <= pBehaviour->field_0x90)) ||
		(bVar7 = SV_WLF_CanMoveTo(&this->pCommander->targetGroundPosition), bVar7 == false)) {
		if ((this->combatFlags_0xb78 & 4) == 0) {
			SetState(pBehaviour->Func_0x70(), -1);
		}
		else {
			if (((bVar6 != true) || (0 < this->pCommander->field_0x194)) ||
				(this->timeInAir <= 1.0f)) {
				if (pOtherActor != (CActorFighter*)0x0) {
					iVar9 = pBehaviour->GetState_001f0930();
					if (iVar9 == -1) {
						iVar9 = pBehaviour->GetState_001f08a0();
						if (iVar9 == -1) {
							bVar8 = true;
						}
						else {
							SetState(iVar9, -1);
						}
					}
					else {
						SetState(iVar9, -1);
					}
				}
			}
			else {
				SetState(pBehaviour->GetStateWolfenWeapon(), -1);
			}
		}
	}
	else {
		SetState(pBehaviour->Func_0x70(), -1);
	}

	if ((((this->combatFlags_0xb78 & 4) != 0) && (pOtherActor != (CActorFighter*)0x0)) && (bVar8)) {
		uVar2 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0;
		pWeapon = GetWeapon();
		SV_GetActorHitPos(pOtherActor, &local_40);
		if ((uVar2 & 1) != 0) {
			edF32Vector4SubHard(&eStack80, &local_40, &pWeapon->currentLocation);
			fVar13 = edF32Vector4GetDistHard(&eStack80);
			fVar14 = pWeapon->FUN_002d5710();
			pCVar10 = pOtherActor->GetCollidingActor();

			do {
				if (pCVar10 == pOtherActor) {
					pOtherActor = (CActorFighter*)0x0;
				}
				else {
					bVar8 = pOtherActor->IsKindOfObject(2);
					if (bVar8 != false) {
						CActorMovable* pMovable = static_cast<CActorMovable*>(pOtherActor);
						edF32Vector4ScaleHard((fVar13 / fVar14) * pMovable->dynamic.linearAcceleration, &eStack128, &pMovable->dynamic.velocityDirectionEuler);
						edF32Vector4AddHard(&local_40, &local_40, &eStack128);
					}

					pOtherActor = pOtherActor->pTiedActor;
				}
			} while (pOtherActor != (CActorFighter*)0x0);
		}

		local_70 = local_40;

		fVar13 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x8;
		if ((pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x0 & 2) != 0) {
			local_10 = local_70;

			edF32Vector4SubHard(&eStack96, &local_70, &pBehaviour->field_0x80);
			fVar14 = edF32Vector4GetDistHard(&eStack96);
			fVar13 = fVar13 * Timer::GetTimer()->cutsceneDeltaTime;
			if (fVar13 <= fVar14) {
				fVar14 = fVar13;
			}

			fVar13 = edF32Vector4SafeNormalize0Hard(&eStack96, &eStack96);
			if (fVar13 != 0.0f) {
				edF32Vector4ScaleHard(fVar14, &eStack96, &eStack96);
				edF32Vector4AddHard(&local_70, &pBehaviour->field_0x80, &eStack96);
			}
		}

		local_10 = local_70;
		pBehaviour->field_0x80 = local_70;

		fVar14 = pBehaviour->aSubObjs[pBehaviour->field_0xe8].field_0x4.field_0x4;
		GetWeapon();
		fVar13 = fVar14 * 0.5f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.x = (local_10.x - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.y = (local_10.y - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;
		uVar12 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar12;
		local_10.w = 1.0f;
		local_10.z = (local_10.z - fVar13) + (fVar14 * (float)((uint)(uVar12 >> 0x10) & 0x7fff)) / 32767.0f;

		iVar9 = GetWeapon()->GetBurstState();
		if (iVar9 == 3) {
			SetState(WOLFEN_STATE_RELOAD, -1);
		}
		else {
			if (iVar9 == 1) {
				iVar9 = GetWeapon()->Action(&local_10, this);
				if (iVar9 != 0) {
					SetState(WOLFEN_STATE_FIRE, -1);
				}
			}
		}
	}

	pCVar3 = this->pAnimationController;
	peVar4 = (pCVar3->anmBinMetaAnimator).aAnimData;
	if ((peVar4->currentAnimDesc).animType == pCVar3->currentAnimType_0x30) {
		bVar8 = false;
		if (peVar4->animPlayState != 0) {
			bVar8 = (peVar4->field_0xcc & 2) != 0;
		}
	}
	else {
		bVar8 = false;
	}

	if (bVar8) {
		PlayAnim(0xaa);
	}

	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	return;
}

void CActorWolfen::StateTrackWeaponChase(CBehaviourTrackWeapon* pBehaviour)
{
	float fVar1;
	float fVar2;
	float fVar3;
	int iVar4;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* pCVar5;
	undefined4 uVar6;
	CActorMovParamsIn movParamsIn;
	CActorMovParamsOut movParamsOut;
	CActorsTable actorsTable;
	CActorCommander* pCommander;

	actorsTable.entryCount = 0;
	movParamsOut.flags = 0;
	movParamsIn.pRotation = (edF32VECTOR4*)0x0;
	movParamsIn.speed = 0.0;
	movParamsIn.flags = 0x150;
	iVar4 = -1;
	if (this->currentAnimType == 7) {
		movParamsIn.rotSpeed = GetWalkRotSpeed();
		movParamsIn.flags = movParamsIn.flags | 2;
		movParamsIn.acceleration = GetWalkAcceleration();
		movParamsIn.speed = GetWalkSpeed();
	}
	else {
		movParamsIn.rotSpeed = GetRunRotSpeed();
		movParamsIn.flags = movParamsIn.flags | 2;
		movParamsIn.acceleration = GetRunAcceleration();
		movParamsIn.speed = GetRunSpeed();
	}

	movParamsIn.flags = movParamsIn.flags | 0x400;
	SV_WLF_MoveTo(&movParamsOut, &movParamsIn, &this->pCommander->targetGroundPosition);

	if ((movParamsOut.flags & 2) != 0) {
		iVar4 = pBehaviour->GetStateWolfenWeapon();
	}

	ManageDyn(4.0f, 0x1002023b, &actorsTable);

	if (iVar4 == -1) {
		iVar4 = SV_WLF_CheckBoxOnWay(&actorsTable);
	}

	if (iVar4 == -1) {
		if ((this->combatMode_0xb7c == ECM_InCombat) &&
			(pCVar5 = pBehaviour->GetNotificationTargetArray(), (int)pCVar5->combatMode < 2)) {
			SetState(pBehaviour->GetState_001f0b30(), -1);
		}
		else {
			if ((~this->combatFlags_0xb78 & 0x420) == 0x420) {
				SetState(pBehaviour->GetStateWolfenWeapon(), -1);
			}
			else {
				iVar4 = pBehaviour->GetState_001f08a0();
				if (iVar4 == -1) {
					if (((this->combatFlags_0xb78 & 4) == 0) ||
						(pCommander = this->pCommander,
							fVar1 = (pCommander->targetPosition).x - this->currentLocation.x,
							fVar2 = (pCommander->targetPosition).y - this->currentLocation.y,
							fVar3 = (pCommander->targetPosition).z - this->currentLocation.z,
							pBehaviour->field_0xf0 < sqrtf(fVar1 * fVar1 + fVar2 * fVar2 + fVar3 * fVar3))) {
						pCommander = this->pCommander;
	
							fVar1 = (pCommander->targetGroundPosition).x - this->currentLocation.x;
						fVar2 = (pCommander->targetGroundPosition).y - this->currentLocation.y;
						fVar3 = (pCommander->targetGroundPosition).z - this->currentLocation.z;
						if (0.5f <= sqrtf(fVar1 * fVar1 + fVar2 * fVar2 + fVar3 * fVar3)) {
							iVar4 = FUN_0035f1e0(&actorsTable, &this->pCommander->targetGroundPosition);
							if (iVar4 == 0) {
								iVar4 = pBehaviour->GetState_001f0930();
								if (iVar4 != -1) {
									SetState(iVar4, -1);
								}
							}
							else {
								IMPLEMENTATION_GUARD(
								SetState(pBehaviour->field_0x78(), -1);)
							}
						}
						else {
							if ((int)this->combatMode_0xb7c < 1) {
								IMPLEMENTATION_GUARD(
								SetState(pBehaviour->field_0x78(), -1);)
							}
							else {
								SetState(pBehaviour->GetStateWolfenWeapon(), -1);
							}
						}
					}
					else {
						pCVar5 = pBehaviour->GetNotificationTargetArray();
						if ((int)pCVar5->combatMode < 2) {
							SetState(pBehaviour->GetState_001f0b30(), -1);
						}
						else {
							SetState(pBehaviour->Func_0x74(), -1);
						}
					}
				}
				else {
					SetState(iVar4, -1);
				}
			}
		}
	}
	else {
		SetState(iVar4, -1);
	}

	return;
}

void CActorWolfen::StateTrackWeaponCheckPosition(CBehaviourTrackWeaponStand* pBehaviour)
{
	CActorCommander* pCVar1;
	CAnimation* pCVar2;
	edAnmLayer* peVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	bool bVar7;
	undefined4 uVar8;
	int iVar9;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* this_00;

	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
	InternState_WolfenLocate();

	pCVar1 = this->pCommander;
	if (pCVar1->field_0x194 < 1) {
		pCVar2 = this->pAnimationController;
		peVar3 = (pCVar2->anmBinMetaAnimator).aAnimData;
		if ((peVar3->currentAnimDesc).animType == pCVar2->currentAnimType_0x30) {
			bVar7 = false;
			if (peVar3->animPlayState != 0) {
				bVar7 = (peVar3->field_0xcc & 2) != 0;
			}
		}
		else {
			bVar7 = false;
		}
		if (bVar7) {
			bVar7 = pBehaviour->GetNotificationTargetArray()->FUN_003c38c0(this);
			if (bVar7 == false) {
				SetState(0xb3, -1);
			}
			else {
				SetState(pBehaviour->GetStateWolfenWeapon(), -1);
			}
		}
		else {
			iVar9 = pBehaviour->GetState_001f0b30();
			if (iVar9 != -1) {
				SetState(iVar9, -1);
			}
		}
	}
	else {
		if (((this->combatFlags_0xb78 & 4) == 0) ||
			(fVar4 = (pCVar1->targetPosition).x - this->currentLocation.x,
				fVar5 = (pCVar1->targetPosition).y - this->currentLocation.y,
				fVar6 = (pCVar1->targetPosition).z - this->currentLocation.z,
				pBehaviour->field_0x90 < sqrtf(fVar4 * fVar4 + fVar5 * fVar5 + fVar6 * fVar6))) {
			SetState(pBehaviour->GetState_001f0b30(), -1);
		}
		else {
			SetState(pBehaviour->Func_0x74(), -1);
		}
	}

	return;
}

void CActorWolfen::StateTrackWeaponDefend(CBehaviourTrackWeapon* pBehaviour)
{
	CActorCommander* pCVar1;
	float fVar2;
	float fVar3;
	bool bVar4;
	int iVar5;
	undefined4 uVar6;
	float fVar7;
	edF32VECTOR4 eStack16;

	fVar7 = GetWalkRotSpeed();
	edF32Vector4SubHard(&eStack16, &this->pCommander->targetPosition, &this->currentLocation);
	edF32Vector4NormalizeHard(&eStack16, &eStack16);
	bVar4 = SV_WLF_UpdateOrientation2D(fVar7, &eStack16, 0);
	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	if (0 < this->pCommander->field_0x194) {
		WaitingAnimation_Defend();
	}

	if ((((this->combatFlags_0xb78 & 0x420) == 0) || (pCVar1 = this->pCommander,
			fVar7 = (pCVar1->targetGroundPosition).x - this->currentLocation.x,
			fVar2 = (pCVar1->targetGroundPosition).y - this->currentLocation.y,
			fVar3 = (pCVar1->targetGroundPosition).z - this->currentLocation.z,
			sqrtf(fVar7 * fVar7 + fVar2 * fVar2 + fVar3 * fVar3) < 0.5f)) ||
		(iVar5 = SV_WLF_CanMoveTo(&this->pCommander->targetGroundPosition), iVar5 == 0)) {
		if (((bVar4 != true) || (0 < this->pCommander->field_0x194)) || (this->timeInAir <= 1.0f)) {
			if (((this->pCommander->field_0x194 < 1) || ((this->combatFlags_0xb78 & 4) == 0)) ||
				(pCVar1 = this->pCommander,
					fVar7 = (pCVar1->targetPosition).x - this->currentLocation.x,
					fVar2 = (pCVar1->targetPosition).y - this->currentLocation.y,
					fVar3 = (pCVar1->targetPosition).z - this->currentLocation.z,
					pBehaviour->field_0x90 <= sqrtf(fVar7 * fVar7 + fVar2 * fVar2 + fVar3 * fVar3))) {
				iVar5 = pBehaviour->GetState_001f0930();
				if (iVar5 != -1) {
					SetState(iVar5, -1);
				}
			}
			else {
				SetState(pBehaviour->Func_0x74(), -1);
			}
		}
		else {
			SetState(pBehaviour->Func_0x78(), -1);
		}
	}
	else {
		SetState(pBehaviour->Func_0x70(), -1);
	}

	return;
}

void CActorWolfen::StateTrackCheckPosition(CBehaviourTrackWeapon* pBehaviour)
{
	CActorCommander* pCVar1;
	CAnimation* pCVar2;
	edAnmLayer* peVar3;
	float fVar4;
	float fVar5;
	float fVar6;
	bool bVar7;
	undefined4 uVar8;
	int iVar9;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* this_00;

	this->dynamic.speed = 0.0f;
	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);
	InternState_WolfenLocate();

	pCVar1 = this->pCommander;
	if (pCVar1->field_0x194 < 1) {
		pCVar2 = this->pAnimationController;
		peVar3 = (pCVar2->anmBinMetaAnimator).aAnimData;
		if ((peVar3->currentAnimDesc).animType == pCVar2->currentAnimType_0x30) {
			bVar7 = false;
			if (peVar3->animPlayState != 0) {
				bVar7 = (peVar3->field_0xcc & 2) != 0;
			}
		}
		else {
			bVar7 = false;
		}
		if (bVar7) {
			bVar7 = pBehaviour->GetNotificationTargetArray()->FUN_003c38c0(this);
			if (bVar7 == false) {
				SetState(0xb3, -1);
			}
			else {
				SetState(pBehaviour->GetStateWolfenWeapon(), -1);
			}
		}
		else {
			iVar9 = pBehaviour->GetState_001f0b30();
			if (iVar9 != -1) {
				SetState(iVar9, -1);
			}
		}
	}
	else {
		if (((this->combatFlags_0xb78 & 4) == 0) ||
			(fVar4 = (pCVar1->targetPosition).x - this->currentLocation.x,
				fVar5 = (pCVar1->targetPosition).y - this->currentLocation.y,
				fVar6 = (pCVar1->targetPosition).z - this->currentLocation.z,
				pBehaviour->field_0x90 < sqrtf(fVar4 * fVar4 + fVar5 * fVar5 + fVar6 * fVar6))) {
			SetState(pBehaviour->GetState_001f0b30(), -1);
		}
		else {
			SetState(pBehaviour->Func_0x74(), -1);
		}
	}

	return;
}

void CActorWolfen::StateTrackWeaponDefend(CBehaviourTrackWeaponStand* pBehaviour)
{
	CActorCommander* pCVar1;
	float fVar2;
	float fVar3;
	bool bVar4;
	Timer* pTVar5;
	undefined4 uVar6;
	int iVar7;
	float fVar8;
	edF32VECTOR4 eStack16;

	fVar8 = GetWalkRotSpeed();
	edF32Vector4SubHard(&eStack16, &this->pCommander->targetPosition, &this->currentLocation);
	edF32Vector4NormalizeHard(&eStack16, &eStack16);
	bVar4 = SV_WLF_UpdateOrientation2D(fVar8, &eStack16, 0);
	this->dynamic.speed = 0.0f;

	ManageDyn(4.0f, 0x100a023b, (CActorsTable*)0x0);

	if (0 < this->pCommander->field_0x194) {
		WaitingAnimation_Defend();
		pTVar5 = Timer::GetTimer();
		pBehaviour->field_0xec = pTVar5->scaledTotalTime;
	}

	if ((bVar4 != true) || (pTVar5 = Timer::GetTimer(), pTVar5->scaledTotalTime - (float)pBehaviour->field_0xec <= 4.0f)) {
		pCVar1 = this->pCommander;
		if ((pCVar1->field_0x194 < 1) ||
			(fVar8 = (pCVar1->targetPosition).x - this->currentLocation.x,
				fVar2 = (pCVar1->targetPosition).y - this->currentLocation.y,
				fVar3 = (pCVar1->targetPosition).z - this->currentLocation.z,
				pBehaviour->field_0x90 <= sqrtf(fVar8 * fVar8 + fVar2 * fVar2 + fVar3 * fVar3))) {
			iVar7 = pBehaviour->GetState_001f0930();
			if (iVar7 != -1) {
				SetState(iVar7, -1);
			}
		}
		else {
			SetState(pBehaviour->Func_0x74(), -1);
		}
	}
	else {
		SetState(pBehaviour->Func_0x78(), -1);
	}

	return;
}

int CActorWolfen::SV_WLF_CheckBoxOnWay(CActorsTable* pTable)
{
	bool bVar1;
	int iVar2;

	iVar2 = -1;
	if (((this->field_0xb74 != 5) && (this->field_0xd14 == 0)) && ((bVar1 = pTable->IsInList(BOX), bVar1 != false ||
			(bVar1 = pTable->IsInList(BASIC_BOX), bVar1 != false)))) {
		iVar2 = this->field_0xd14;
		while (iVar2 == 0) {
			iVar2 = pTable->aEntries[0]->typeID;
			if ((iVar2 == 0x35) || (iVar2 == 0x39)) {
				this->field_0xd14 = (int)pTable->aEntries[0];
			}
			pTable = (CActorsTable*)pTable->aEntries;
			iVar2 = this->field_0xd14;
		}
		iVar2 = 0x74;
	}
	return iVar2;
}

void CActorWolfen::ClearLocalData()
{
	CinNamedObject30* pCVar1;
	CActorWeapon* pCVar2;
	CActorWolfen* pCVar3;
	int iVar4;
	uint uVar5;
	float fVar6;
	float fVar7;
	CAnimation* pAnim;

	if (((this->flags & 0x2000000) == 0) && (GetWeapon() != (CActorWeapon*)0x0)) {
		if (GetWeapon()->GetLinkFather() == this) {
			GetWeapon()->UnlinkWeapon();
		}
		if (this->field_0xb80 != 2) {
			GetWeapon()->LinkWeapon(this, 0xcc414f1b);
		}
	}

	Func_0x204((CActorFighter*)0x0);

	this->combatFlags_0xb78 = 0;
	this->combatMode_0xb7c = ECM_None;
	//uVar5 = 0;
	//if (this->count_0xbd4 != 0) {
	//	iVar4 = 0;
	//	do {
	//		uVar5 = uVar5 + 1;
	//		*(undefined*)((int)&this->field_0xbd0->x + iVar4) = 1;
	//		iVar4 = iVar4 + 0x10;
	//	} while (uVar5 < (uint)this->count_0xbd4);
	//}
	//this->field_0xbe0 = 0;
	//this->field_0xbd8 = 0;
	this->field_0xd04 = (CActor*)0x0;
	//this->field_0xd0c = 0;
	this->field_0xd08 = 0;
	this->pTrackedProjectile = (CActorProjectile*)0x0;
	this->field_0xd14 = 0;
	this->field_0xd24 = 0.0f;
	this->field_0xd28 = 1.0f;
	this->pTargetActor_0xc80 = (CActorFighter*)0x0;
	LifeRestore();
	UpdatePosition(&this->baseLocation, true);

	pCVar1 = this->pCinData;
	this->rotationEuler.xyz = pCVar1->rotationEuler;

	TermFightAction();

	if (this->pWolfenKnowledge != (CActorWolfenKnowledge*)0x0) {
		//this->pWolfenKnowledge->Reset();
		//this->field_0xb6c = 0xffffffff;
		//this->field_0xb70 = 1;
	}

	if (this->field_0xb74 == 3) {
		pAnim = this->pAnimationController;
		if (pAnim != (CAnimation*)0x0) {
			pAnim->AddDisabledBone(0xae8f8ef8);
		}

		if (GetWeapon() != (CActorWeapon*)0x0) {
			GetWeapon()->flags = GetWeapon()->flags & 0xffffff5f;
			GetWeapon()->EvaluateDisplayState();
		}
	}

	return;
}

void CActorWolfen::SV_WLF_MoveTo(CActorMovParamsOut* pMovParamsOut, CActorMovParamsIn* pMovParamsIn, edF32VECTOR4* pPosition)
{
	bool bValidPosition;

	if ((this->combatFlags_0xb78 & 0x400) == 0) {
	LAB_001739a0:
		bValidPosition = false;
	}
	else {
		if (GetPathfinderClientAlt()->id != -1) {
			bValidPosition = GetPathfinderClientAlt()->IsValidPosition(&this->currentLocation);
			if (bValidPosition == false) goto LAB_001739a0;
		}

		bValidPosition = true;
	}

	if (bValidPosition) {
		if ((this->combatFlags_0xb78 & 0x80000) == 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 0x80000;
		}

		this->pathOriginPosition = this->currentLocation;
	}

	SV_AUT_MoveTo(pMovParamsOut, pMovParamsIn, pPosition);

	return;
}

bool CActorWolfen::SV_WLF_CanMoveTo(edF32VECTOR4* pPosition)
{
	uint zoneId;
	bool bCanMoveTo;
	ed_zone_3d* pZone;
	int iVar2;

	zoneId = this->field_0xd30;
	bCanMoveTo = false;

	if (zoneId == 0xffffffff) {
		iVar2 = this->pCommander->CheckGuardArea(pPosition);
	}
	else {
		pZone = (ed_zone_3d*)0x0;
		if (zoneId != 0xffffffff) {
			pZone = edEventGetChunkZone(CScene::ptable.g_EventManager_006f5080->activeChunkId, zoneId);
		}

		iVar2 = edEventComputeZoneAgainstVertex(CScene::ptable.g_EventManager_006f5080->activeChunkId, pZone, pPosition, 0);
	}

	if ((iVar2 == 1) && (iVar2 = this->pCommander->CheckDetectArea(pPosition), iVar2 == 1)) {
		bCanMoveTo = SV_AUT_CanMoveTo(pPosition);
	}

	return bCanMoveTo;
}

bool CActorWolfen::SV_WLF_UpdateOrientation2D(float param_1, edF32VECTOR4* v0, int rotationType)
{
	edAnmLayer* peVar1;
	bool bVar2;
	int AVar3;
	int iVar4;
	Timer* pTVar5;
	float fVar6;
	float fVar7;
	float fVar8;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;
	CAnimation* pAnim;

	pAnim = this->pAnimationController;
	AVar3 = GetIdMacroAnim(0xa2);
	iVar4 = pAnim->PhysicalLayerFromLayerId(8);
	peVar1 = (pAnim->anmBinMetaAnimator).aAnimData;
	bVar2 = pAnim->IsLayerActive(8);
	if (((bVar2 != false) && (AVar3 != -1)) && (peVar1[iVar4].currentAnimDesc.animType == AVar3)) {
		local_10.x = this->rotationQuat.x;
		local_10.y = 0.0f;
		local_10.z = this->rotationQuat.z;
		local_10.w = 0.0f;

		edF32Vector4NormalizeHard(&local_10, &local_10);

		local_20.x = v0->x;
		local_20.y = 0.0f;
		local_20.z = v0->z;
		local_20.w = 0.0f;

		edF32Vector4NormalizeHard(&local_20, &local_20);
		fVar6 = edF32Vector4DotProductHard(&local_10, &local_20);
		fVar8 = 1.0f;
		if (fVar6 <= 1.0f) {
			fVar8 = fVar6;
		}
		fVar6 = edF32ACosHard(fVar8);
		pTVar5 = Timer::GetTimer();
		fVar8 = pTVar5->cutsceneDeltaTime;
		fVar7 = GetWalkRotSpeed();
		fVar6 = edFIntervalLERP(fabs(fVar6), 0.0f, fVar8 * fVar7 * 0.5f, 0.0f, 1.0f);
		fVar7 = this->field_0xd24;
		if (fVar6 < fVar7) {
			if (fVar7 < fVar6 + 0.2f) {
				this->field_0xd24 = fVar6;
			}
			else {
				this->field_0xd24 = fVar7 - 0.2f;
			}
		}
		else {
			if (fVar7 < fVar6) {
				if (fVar6 - 0.2f < fVar7) {
					this->field_0xd24 = fVar6;
				}
				else {
					this->field_0xd24 = fVar7 + 0.2f;
				}
			}
		}
		if (0.0f < local_10.x * local_20.z - local_20.x * local_10.z) {
			fVar8 = this->field_0xd28 + fVar8;
			this->field_0xd28 = fVar8;
			if (1.0f < fVar8) {
				this->field_0xd28 = 1.0f;
			}
		}
		else {
			fVar8 = this->field_0xd28 - fVar8;
			this->field_0xd28 = fVar8;
			if (fVar8 < 0.0f) {
				this->field_0xd28 = 0.0f;
			}
		}
	}

	return SV_UpdateOrientation2D(param_1, v0, rotationType);
}

bool CActorWolfen::SV_WLF_IsIntruderMakingNoise(CActor* pActor)
{
	bool bMakingNoise;
	int iVar2;
	long lVar3;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;

	bMakingNoise = false;
	if (((pActor != (CActor*)0x0) && (iVar2 = this->pCommander->CheckZone_00170f90(&pActor->currentLocation), iVar2 == 2)) && (pActor->IsMakingNoise() != false)) {
		local_10.x = this->currentLocation.x;
		local_10.z = this->currentLocation.z;
		local_10.w = this->currentLocation.w;
		local_10.y = this->currentLocation.y + this->field_0xcf0;

		pActor->SV_GetActorColCenter(&local_20);

		if (((local_10.x - local_20.x) * (local_10.x - local_20.x) + (local_10.z - local_20.z) * (local_10.z - local_20.z) <
			(this->hearingDetectionProps).rangeSquared) && (fabs(local_10.y - local_20.y) < (this->hearingDetectionProps).maxHeightDifference)) {
			bMakingNoise = true;
		}
	}

	if (bMakingNoise) {
		if ((this->combatFlags_0xb78 & 1) == 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 1;
		}
	}
	else {
		if ((this->combatFlags_0xb78 & 1) != 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffffe;
		}
	}

	return bMakingNoise;
}

bool CActorWolfen::SV_WLF_IsIntruderInVitalSphere(CActor* pActor)
{
	float fVar1;
	float fVar2;
	float fVar3;
	bool bInVitalSphere;

	bInVitalSphere = false;
	if (pActor != (CActor*)0x0) {
		fVar1 = (pActor->currentLocation).x - this->currentLocation.x;
		fVar2 = (pActor->currentLocation).y - this->currentLocation.y;
		fVar3 = (pActor->currentLocation).z - this->currentLocation.z;

		bInVitalSphere = sqrtf(fVar1 * fVar1 + fVar2 * fVar2 + fVar3 * fVar3) < (this->visionDetectionProps).field_0x0;
	}

	if (bInVitalSphere) {
		if ((this->combatFlags_0xb78 & 2) == 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 2;
		}
	}
	else {
		if ((this->combatFlags_0xb78 & 2) != 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffffd;
		}
	}

	return bInVitalSphere;
}

bool CActorWolfen::SV_WLF_IsIntruderInVision(CActor* pActor)
{
	int iVar1;
	uint uVar2;
	bool bInVision;

	bInVision = false;
	if (pActor != (CActor*)0x0) {
		iVar1 = this->actorState;
		uVar2 = 0;
		if ((iVar1 != -1) && (uVar2 = 0, 0x71 < iVar1)) {
			uVar2 = _gStateCfg_WLF[iVar1 + -0x72].field_0x8;
		}

		if ((uVar2 & 0x20) == 0) {
			GetVision()->location.x = this->currentLocation.x;
			GetVision()->location.y = this->currentLocation.y + this->field_0xcf0;
			GetVision()->location.z = this->currentLocation.z;
			GetVision()->location.w = this->currentLocation.w;

			GetVision()->rotationQuat = this->rotationQuat;
		}

		bInVision = GetVision()->ScanForTarget(pActor, 1) != (CActor*)0x0;
	}

	if (bInVision) {
		if ((this->combatFlags_0xb78 & 4) == 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 | 4;
		}
	}
	else {
		if ((this->combatFlags_0xb78 & 4) != 0) {
			this->combatFlags_0xb78 = this->combatFlags_0xb78 & 0xfffffffb;
		}
	}

	return bInVision;
}

bool CActorWolfen::FUN_001738e0(CActor* pActor)
{
	bool bVar1;

	bVar1 = false;
	if (((pActor != (CActor*)0x0) && (pActor->typeID == ACTOR_HERO_PRIVATE)) && (pActor->curBehaviourId == 8)) {
		bVar1 = true;
	}

	return bVar1;
}

bool CActorWolfen::FUN_0035f1e0(CActorsTable* pTable, edF32VECTOR4* pPosition)
{
	CActor* pActor;
	float fVar1;
	float fVar2;
	int curEntryIndex;
	float fVar4;

	curEntryIndex = 0;
	while (true) {
		if (pTable->entryCount <= curEntryIndex) {
			return false;
		}

		pActor = pTable->aEntries[curEntryIndex];
		if (((pActor->typeID == DCA) || (pActor->typeID == PROJECTILE)) && (fVar1 = pPosition->x - (pActor->currentLocation).x, fVar2 = pPosition->z - (pActor->currentLocation).z,
				fVar4 = pActor->GetPosition_00117db0(), sqrtf(fVar1 * fVar1 + 0.0f + fVar2 * fVar2) < fVar4)) break;

		curEntryIndex = curEntryIndex + 1;
	}
	return true;
}

int CActorWolfen::GetState_00174190()
{
	int newState;

	newState = 0x9b;
	if (1 < (int)this->combatMode_0xb7c) {
		newState = WOLFEN_STATE_SURPRISE;
	}

	return newState;
}

void CActorWolfen::EnterCombatState(CActor* pSender)
{
	this->pTargetActor_0xc80 = static_cast<CActorFighter*>(pSender);

	if ((this->pTargetActor_0xc80 == this->pCommander->GetIntruder()) && (this->pCommander != (CActorCommander*)0x0)) {
		edF32VECTOR4 targetPosition;
		this->pTargetActor_0xc80->SV_GetActorColCenter(&targetPosition);
		this->pCommander->targetPosition = targetPosition;

		if (this->pCommander->bInCombat_0x1b0 == 0) {
			edF32VECTOR4 targetGroundPosition;
			this->pTargetActor_0xc80->SV_GetGroundPosition(&targetGroundPosition);
			int iVar15 = this->pCommander->CheckDetectArea(&targetGroundPosition);

			CEventManager* pCVar5 = CScene::ptable.g_EventManager_006f5080;
			if (iVar15 != 1) {
				uint uVar12 = this->field_0xd30;
				if (uVar12 == 0xffffffff) {
					iVar15 = this->pCommander->CheckGuardArea(&targetGroundPosition);
				}
				else {
					ed_zone_3d* pZone = (ed_zone_3d*)0x0;
					if (uVar12 != 0xffffffff) {
						pZone = edEventGetChunkZone((CScene::ptable.g_EventManager_006f5080)->activeChunkId, uVar12);
					}

					iVar15 = edEventComputeZoneAgainstVertex(pCVar5->activeChunkId, pZone, &targetGroundPosition, 0);
				}

				if (iVar15 != 1) return;
			}

			if (GetPathfinderClientAlt()->id != -1) {
				if (GetPathfinderClientAlt()->IsValidPosition(&targetGroundPosition) == false) return;
			}

			this->pCommander->targetGroundPosition = targetGroundPosition;
			this->pCommander->bInCombat_0x1b0 = 1;
		}
	}

	return;
}

void CActorWolfen::UpdateInRange_001744a0(bool bFlag)
{
	if (bFlag == false) {
		return;
	}

	EnterCombatState(this->pTargetActor_0xc80);

	if ((this->combatFlags_0xb78 & 4) == 0) {
		if ((this->combatMode_0xb7c == ECM_None) && (this->combatMode_0xb7c != ECM_Alerted)) {
			this->combatMode_0xb7c = ECM_Alerted;
		}
	}
	else {
		if (this->combatMode_0xb7c != ECM_InCombat) {
			this->combatMode_0xb7c = ECM_InCombat;
		}
	}

	return;
}


bool CActorWolfen::IsAlive(CActor* pActor)
{
	int iVar15 = -1;

	if ((pActor != (CActor*)0x0) && (pActor->typeID == ACTOR_HERO_PRIVATE)) {
		CActorAutonomous* pAutonomous = static_cast<CActorAutonomous*>(pActor);
		if (0.0f < pAutonomous->GetLifeInterface()->GetValue()) {
			iVar15 = 0;
		}
	}

	if (iVar15 < 0) {
		return false;
	}

	return true;
}

bool CActorWolfen::CanSwitchToFight_Area(CActor* pTarget)
{
	float fVar1;
	float fVar2;
	bool bVar3;
	bool bVar4;
	int iVar5;
	CBehaviour* pCVar6;
	long lVar7;
	bool bCanSwitchToFight;

	bCanSwitchToFight = false;
	if ((this->combatFlags_0xb78 & 4) != 0) {
		bVar4 = false;
		bVar3 = pTarget->IsKindOfObject(OBJ_TYPE_FIGHTER);
		if (((bVar3 != false) &&
			(fVar1 = (pTarget->currentLocation).x - this->currentLocation.x,
				fVar2 = (pTarget->currentLocation).z - this->currentLocation.z,
				sqrtf(fVar1 * fVar1 + 0.0f + fVar2 * fVar2) < this->field_0xb90)) &&
			(fabs((this->distanceToGround + this->currentLocation.y) - (pTarget->distanceToGround + (pTarget->currentLocation).y)) < this->field_0xb98 - 0.2f)) {
			bVar4 = true;
		}

		if (bVar4) {
			bVar4 = false;
			if (((this->combatFlags_0xb78 & 0x30) != 0) &&
				(iVar5 = SV_AUT_CanMoveTo(&pTarget->currentLocation), iVar5 != 0)) {
				bVar4 = true;
			}

			IMPLEMENTATION_GUARD(
			if ((bVar4) &&
				((bVar4 = pTarget->IsKindOfObject(OBJ_TYPE_AUTONOMOUS), bVar4 == false ||
					(lVar7 = (*(code*)this->pVTable[1].field_0x4)(this), lVar7 == 0)))) {
				bVar4 = false;
				if ((pTarget != (CActor*)0x0) && ((pTarget->typeID == ACTOR_HERO_PRIVATE && (pTarget->curBehaviourId == 8)))) {
					bVar4 = true;
				}
				if ((!bVar4) || (pCVar6 = CActor::GetBehaviour((CActor*)this, 0x18), pCVar6 == (CBehaviour*)0x0)) {
					bCanSwitchToFight = true;
				}				
			})
		}
	}

	return bCanSwitchToFight;
}

void CActorWolfen::InternState_WolfenLocate()
{
	CAnimation* pAnimationController;
	float fVar2;
	float fVar3;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;

	local_10.x = this->currentLocation.x;
	local_10.z = this->currentLocation.z;
	local_10.w = this->currentLocation.w;
	local_10.y = this->currentLocation.y + this->field_0xcf0;

	pAnimationController = this->pAnimationController;

	if (((pAnimationController->anmBinMetaAnimator).aAnimData)->animPlayState == 1) {
		fVar2 = pAnimationController->anmBinMetaAnimator.GetLayerAnimTime(0, 0);
		fVar3 = fVar2;
		pAnimationController->anmBinMetaAnimator.GetAnimType_00242330(0);

		IMPLEMENTATION_GUARD_LOG(); // IS this correct?
		float cosValue = sinf(fabs(((fVar3 / fVar2) * 6.283185)));
		fVar3 = edF32Between_0_2Pi(-cosValue * (3.141593 - GetVision()->field_0x30 * 2.0f * 57.29578f * 0.01745329f * 0.5f) + this->rotationEuler.y);
		SetVectorFromAngleY(fVar3, &local_20);
		GetVision()->location = local_10;
		GetVision()->rotationQuat = local_20;
	}
	else {
		GetVision()->location = local_10;
		GetVision()->rotationQuat = this->rotationQuat;
	}

	return;
}

void CActorWolfen::TermFightAction()
{
	CActorCommander* this_00;
	undefined4* puVar1;
	long lVar2;

	if (this->field_0xb2c != 3) {
		IMPLEMENTATION_GUARD(
		this_00 = this->pCommander;
		lVar2 = CActorCommander::FUN_00171440(this_00, (int)this);
		if (lVar2 != 0) {
			puVar1 = (undefined4*)0x0;
			if (this->field_0xb2c != 3) {
				puVar1 = (undefined4*)(&this->field_0xa90 + this->field_0xb2c * 0x34);
			}
			CActorCommander::ReleaseSemaphore(this_00, (undefined4*)*puVar1, (int)this);
		})
	}
	return;
}

void CActorWolfen::UpdateCombatMode()
{
	if ((this->combatFlags_0xb78 & 4) == 0) {
		if ((this->combatMode_0xb7c == ECM_None) && (this->combatMode_0xb7c != ECM_Alerted)) {
			this->combatMode_0xb7c = ECM_Alerted;
		}
	}
	else {
		if (this->combatMode_0xb7c != ECM_InCombat) {
			this->combatMode_0xb7c = ECM_InCombat;
		}
	}

	if ((this->combatMode_0xb7c < 2) && (this->combatMode_0xb7c != ECM_InCombat)) {
		this->combatMode_0xb7c = ECM_InCombat;
	}
}

void CBehaviourWatchDog::Create(ByteCode* pByteCode)
{
	S_TARGET_STREAM_REF* piVar1;
	S_STREAM_EVENT_CAMERA* pSVar2;
	uint uVar3;
	CWayPoint* pCVar4;
	int iVar5;

	this->flags_0x4 = pByteCode->GetU32();
	this->field_0x80.index = pByteCode->GetS32();
	this->trackBehaviourId = pByteCode->GetS32();

	this->switchBehaviour.Create(pByteCode);

	this->pTargetStreamRef.Create(pByteCode);
	this->pCameraStreamEvent.Create(pByteCode);

	return;
}

void CBehaviourWatchDog::Init(CActor* pOwner)
{
	CWayPoint* pCVar1;
	S_TARGET_STREAM_REF* pSVar2;
	int iVar3;
	int iVar4;
	float fVar5;
	float fVar6;
	edF32VECTOR3 local_10;

	this->baseLocation = pOwner->baseLocation;
	this->rotationEuler.xyz = pOwner->pCinData->rotationEuler;

	this->pOwner = reinterpret_cast<CActorWolfen*>(pOwner);

	this->field_0x80.Init();

	pCVar1 = this->field_0x80.Get();
	if (pCVar1 == (CWayPoint*)0x0) {
		this->baseLocation = pOwner->baseLocation;
		this->rotationEuler.xyz = pOwner->pCinData->rotationEuler;
	}
	else {
		this->baseLocation.xyz = pCVar1->location;
		(this->baseLocation).w = 1.0f;

		local_10 = pCVar1->rotation;
		this->pOwner->SV_BuildAngleWithOnlyY(&this->rotationEuler.xyz, &local_10);
		this->rotationEuler.w = 0.0f;
	}

	this->switchBehaviour.Init(pOwner);

	this->pTargetStreamRef.Init();

	this->pCameraStreamEvent->Init();
	return;
}

void CBehaviourWatchDog::Manage()
{
	this->pCameraStreamEvent->Manage(this->pOwner);
	this->pOwner->BehaviourWatchDog_Manage(this);

	return;
}

void CBehaviourWatchDog::Begin(CActor* pOwner, int newState, int newAnimationType)
{
	CActorWolfen* pCVar1;
	undefined4 uVar2;
	S_TARGET_STREAM_REF* pSVar3;
	int iVar4;
	int iVar5;

	if (newState == -1) {
		this->pOwner->SetState(this->GetStateWolfenComeBack(), -1);
	}
	else {
		this->pOwner->SetState(newState, newAnimationType);
	}

	this->switchBehaviour.Begin(pOwner);

	this->bool_0x68 = false;

	for (int i = 0; i < this->pTargetStreamRef->entryCount; i++) {
		this->pTargetStreamRef->aEntries[i].Reset();
	}

	this->pCameraStreamEvent->Reset(pOwner);

	return;
}

void CBehaviourWatchDog::End(int newBehaviourId)
{
	CActorWolfen* pActor;
	int iVar3;

	this->pOwner->SV_AUT_PathfindingEnd();

	if ((this->bool_0x68 != false) && (this->bool_0x68 == true)) {
		pActor = this->pOwner;
		iVar3 = 0;
		if (0 < this->pTargetStreamRef->entryCount) {
			do {
				this->pTargetStreamRef->aEntries[iVar3].SwitchOff(this->pOwner);
				iVar3 = iVar3 + 1;
			} while (iVar3 < this->pTargetStreamRef->entryCount);
		}
	}
	this->bool_0x68 = false;
	return;
}

int CBehaviourWatchDog::InterpretMessage(CActor* pSender, int msg, void* pMsgParam)
{
	int iVar1;

	iVar1 = this->switchBehaviour.InterpretMessage(this->pOwner, pSender, msg, pMsgParam);
	if (iVar1 == 0) {
		iVar1 = CBehaviourWolfen::InterpretMessage(pSender, msg, pMsgParam);
	}
	return iVar1;
}

int CBehaviourWatchDog::GetTrackBehaviour()
{
	this->pOwner->GetBehaviour(this->trackBehaviourId);
	return this->trackBehaviourId;
}

void CBehaviourWolfen::Create(ByteCode* pByteCode)
{
	this->flags_0x4 = pByteCode->GetU32();

	return;
}

void CBehaviourWolfen::Init(CActor* pOwner)
{

	this->baseLocation = pOwner->baseLocation;
	this->rotationEuler.xyz = pOwner->pCinData->rotationEuler;

	this->pOwner = reinterpret_cast<CActorWolfen*>(pOwner);

	return;
}

void CBehaviourWolfen::End(int newBehaviourId)
{	
	if (this->pOwner->GetPathfinderClient() != (CPathFinderClient*)0x0) {
		this->pOwner->GetPathfinderClient()->CleanPathDynamic();
	}
	return;
}

void CBehaviourWolfen::InitState(int newState)
{
	CAnimation* pCVar1;
	bool bVar2;
	StateConfig* pSVar3;
	CActorWolfen* pCVar4;
	uint uVar5;
	int iVar6;
	int mode;
	ulong uVar7;
	float fVar8;

	if (newState == 0x74) {
		pSVar3 = this->pOwner->GetStateCfg(0x74);
		iVar6 = this->pOwner->GetIdMacroAnim(pSVar3->animId);
		if (iVar6 < 0) {
			fVar8 = 0.0f;
			pCVar4 = this->pOwner;
		}
		else {
			fVar8 = this->pOwner->pAnimationController->GetAnimLength(iVar6, 0);
		}

		this->pOwner->field_0xd18 = fVar8;
	}
	else {
		if (newState != WOLFEN_STATE_LOCATE) {
			if (newState != WOLFEN_STATE_TRACK_WEAPON_CHECK_POSITION) {
				if (newState == 0xb3) {
					this->pOwner->SetCombatMode(ECM_None);
				}
				else {
					if (newState == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
						IMPLEMENTATION_GUARD_AUDIO(
						GlobalSound::Func_001844f0(CScene::ptable.g_AudioManager_00451698);)
					}
					else {
						if ((newState != WOLFEN_STATE_COME_BACK) && (newState == WOLFEN_STATE_SURPRISE)) {
							this->pOwner->field_0xcfc = this->pOwner->rotationEuler.y;
							pCVar4 = this->pOwner;
							pCVar1 = pCVar4->pAnimationController;
							iVar6 = pCVar4->GetIdMacroAnim(pCVar4->currentAnimType);
							if (iVar6 < 0) {
								fVar8 = 0.0f;
							}
							else {
								fVar8 = pCVar1->GetAnimLength(iVar6, 1);
							}

							this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(fVar8 / 0.3f, 0);
						}
					}
				}

				goto LAB_001f1738;
			}

			this->pOwner->SetCombatMode(ECM_None);
		}

		uVar7 = CScene::_pinstance->field_0x38 * 0x343fd + 0x269ec3;
		CScene::_pinstance->field_0x38 = uVar7;
		fVar8 = ((float)((uint)(uVar7 >> 0x10) & 0x7fff) * 0.4f) / 32767.0f + 0.8f;

		if ((this->pOwner->combatFlags_0xb78 & 0x100000) == 0) {
			fVar8 = fVar8 * 0.7f;
		}

		this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(fVar8, 0);
	}

LAB_001f1738:
	uVar5 = this->pOwner->GetStateWolfenFlags(newState);
	if ((uVar5 & 4) != 0) {
		pCVar1 = this->pOwner->pAnimationController;
		iVar6 = pCVar1->PhysicalLayerFromLayerId(8);
		mode = this->pOwner->GetIdMacroAnim(0xa2);
		bVar2 = pCVar1->IsLayerActive(8);
		if ((bVar2 != false) && (mode != -1)) {
			pCVar1->anmBinMetaAnimator.SetLayerBlendingOp(iVar6, 3);
			pCVar1->anmBinMetaAnimator.SetAnimOnLayer(mode, iVar6, 0xa2);
			pCVar4 = this->pOwner;
			pCVar4->field_0xd24 = 0.0f;
			pCVar4->field_0xd28 = 1.0f;
		}
	}
	return;
}

void CBehaviourWolfen::TermState(int oldState, int newState)
{
	edANM_HDR* peVar1;
	CActorWolfen* pCVar2;
	bool bVar3;
	uint uVar4;
	int iVar5;
	CActorProjectile* pProjectile;
	int AVar6;
	edAnmLayer* peVar7;
	CAnimation* pAnim;

	uVar4 = this->pOwner->GetStateWolfenFlags(oldState);
	if ((uVar4 & 4) != 0) {
		pAnim = this->pOwner->pAnimationController;
		iVar5 = pAnim->PhysicalLayerFromLayerId(8);
		AVar6 = this->pOwner->GetIdMacroAnim(0xa2);
		peVar7 = (pAnim->anmBinMetaAnimator).aAnimData + iVar5;
		bVar3 = pAnim->IsLayerActive(8);
		if (((bVar3 != false) && (AVar6 != -1)) && ((peVar7->currentAnimDesc).animType == AVar6)) {
			peVar1 = (peVar7->currentAnimDesc).state.pAnimKeyTableEntry;
			if ((peVar1->field_0x4.asKey == 1) && (peVar1->keyIndex_0x8.asKey == 2)) {
				AnimKeySomething* peVar8 = reinterpret_cast<AnimKeySomething*>(peVar1);
				peVar8->field_0x14 = 0.5f;
				peVar8->field_0x1c = 0.5f;
			}

			pAnim->anmBinMetaAnimator.SetAnimOnLayer(-1, iVar5, 0xffffffff);
		}
	}

	if (oldState == 0xa5) {
		pCVar2 = this->pOwner;
		pProjectile = pCVar2->pTrackedProjectile;
		if (pCVar2 == (CActorWolfen*)0x0) {
			bVar3 = pProjectile->field_0x40c != (CActor*)0x0;
		}
		else {
			bVar3 = pCVar2 == pProjectile->field_0x40c;
		}
		if (bVar3) {
			pProjectile->field_0x40c = (CActor*)0x0;
		}

		this->pOwner->pTrackedProjectile = (CActorProjectile*)0x0;
	}
	else {
		if (oldState == 0xa4) {
			if (newState != 0xa5) {
				this->pOwner->pTrackedProjectile->field_0x40c = (CActor*)0x0;
				this->pOwner->pTrackedProjectile = (CActorProjectile*)0x0;
			}
		}
		else {
			if (oldState == 0xa3) {
				if ((newState != 0xa4) && (newState != 0xa5)) {
					this->pOwner->pTrackedProjectile->field_0x40c = (CActor*)0x0;
					this->pOwner->pTrackedProjectile = (CActorProjectile*)0x0;
				}
			}
			else {
				if ((oldState == WOLFEN_STATE_LOCATE) || (oldState == WOLFEN_STATE_TRACK_WEAPON_CHECK_POSITION)) {
					this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(1.0f, 0);
				}
				else {
					if (oldState == WOLFEN_STATE_COME_BACK) {
						this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 & 0xffefffff;
					}
					else {
						if (oldState == WOLFEN_STATE_SURPRISE) {
			
							this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(1.0f, 0);
							if ((newState != 0xa1) && (newState != 0xa3)) {
								pCVar2 = this->pOwner;
								pProjectile = pCVar2->pTrackedProjectile;
								if (pProjectile != (CActorProjectile*)0x0) {
									if (pCVar2 == (CActorWolfen*)0x0) {
										bVar3 = pProjectile->field_0x40c != (CActor*)0x0;
									}
									else {
										bVar3 = pCVar2 == pProjectile->field_0x40c;
									}
									if (bVar3) {
										pProjectile->field_0x40c = (CActor*)0x0;
									}
								}

								this->pOwner->pTrackedProjectile = (CActorProjectile*)0x0;
							}
						}
						else {
							if (oldState == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
								IMPLEMENTATION_GUARD_AUDIO(
								FUN_001844c0(CScene::ptable.g_AudioManager_00451698);)
								this->pOwner->SV_AUT_PathfindingEnd();
							}
						}
					}
				}
			}
		}
	}

	return;
}

int CBehaviourWolfen::InterpretMessage(CActor* pSender, int msg, void* pMsgParam)
{
	CActorWolfen* pCVar1;
	bool bVar2;
	CLifeInterface* pCVar3;
	int iVar4;
	long lVar5;
	ulong uVar6;
	float fVar7;
	edF32VECTOR4 local_b0;
	float local_a0;
	float local_9c;
	float local_98;
	float local_94;
	undefined4 local_90[2];
	undefined4 local_88;
	undefined4 local_84;
	undefined4 local_80;
	edF32VECTOR4 eStack112;
	undefined4 local_60;
	undefined4 local_50;
	undefined4 local_4c;
	undefined4 local_48;
	undefined4 local_44;
	undefined2 local_40;
	float local_30;
	float local_2c;
	float local_28;
	float local_24;
	undefined4 local_20;
	undefined4* local_4;

	if (msg == 0xb) {
		IMPLEMENTATION_GUARD(
		pCVar1 = this->pOwner;
		bVar2 = false;
		lVar5 = (*(code*)pSender->pVTable->IsMakingNoise)((long)(int)pSender);
		if (lVar5 != 0) {
			local_a0 = pCVar1->currentLocation.x;
			local_98 = pCVar1->currentLocation.z;
			local_94 = pCVar1->currentLocation.w;
			local_9c = pCVar1->currentLocation.y + pCVar1->field_0xcf0;
			CActor::SV_GetActorColCenter(pSender, &local_b0);
			if (((local_a0 - local_b0.x) * (local_a0 - local_b0.x) + (local_98 - local_b0.z) * (local_98 - local_b0.z) <
				*(float*)&pCVar1->field_0xc84) && (fabs(local_9c - local_b0.y) < *(float*)&pCVar1->field_0xc88)) {
				bVar2 = true;
			}
		}
		if ((!bVar2) && (uVar6 = FUN_00173550((long)(int)this->pOwner, (long)(int)pSender), uVar6 == 0)) {
			return 0;
		}
		this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 | 0x4000;
		*(CActor**)&this->pOwner->field_0xd0c = pSender;)
		return 1;
	}

	if (msg == 7) {
		IMPLEMENTATION_GUARD(
		pCVar3 = (*((this->pOwner->base).characterBase.base.base.pVTable)->GetLifeInterface)((CActor*)this->pOwner);
		fVar7 = (*pCVar3->pVtable->GetValue)((CInterface*)pCVar3);
		if (fVar7 <= 0.0) {
			return 0;
		}
		/* WARNING: Load size is inaccurate */
		if ((*pMsgParam != 1) && (*pMsgParam != 0)) {
			return 0;
		}
		*(undefined4*)((int)pMsgParam + 0x20) = 0;
		*(undefined4*)((int)pMsgParam + 0x24) = 0x3fc00000;
		*(undefined4*)((int)pMsgParam + 0x28) = 0;
		*(undefined4*)((int)pMsgParam + 0x2c) = 0;)
		return 1;
	}

	if (msg != 2) {
		return 0;
	}

	/* WARNING: Load size is inaccurate */
	int* pMsgParamInt = (int*)pMsgParam;
	iVar4 = *pMsgParamInt;
	if (iVar4 == 4) {
		IMPLEMENTATION_GUARD(
		this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 | 0x40;)
		return 1;
	}
	if (((iVar4 != 7) && (iVar4 != 8)) && (iVar4 != 10)) {
		IMPLEMENTATION_GUARD(
		pCVar3 = (*((this->pOwner->base).characterBase.base.base.pVTable)->GetLifeInterface)((CActor*)this->pOwner);
		fVar7 = (*pCVar3->pVtable->GetValue)((CInterface*)pCVar3);
		if (0.0 < fVar7) {
			/* WARNING: Load size is inaccurate */
			if ((pSender->typeID != 0x20) && (*pMsgParam != 4)) {
				(*((this->pOwner->base).characterBase.base.base.pVTable)->LifeDecrease)
					(*(float*)((int)pMsgParam + 0xc), (CActorAutonomous*)this->pOwner);
			}
			pCVar3 = (*((this->pOwner->base).characterBase.base.base.pVTable)->GetLifeInterface)((CActor*)this->pOwner);
			fVar7 = (*pCVar3->pVtable->GetValue)();
			if (fVar7 <= 0.0) {
				local_90[0] = 7;
				local_88 = 0;
				local_84 = 0;
				local_80 = 0;
				local_20 = 0;
				local_30 = g_xVector.x;
				local_2c = g_xVector.y;
				local_28 = g_xVector.z;
				local_24 = g_xVector.w;
				local_40 = 1;
				local_50 = *(undefined4*)((int)pMsgParam + 0x40);
				local_4c = *(undefined4*)((int)pMsgParam + 0x44);
				local_48 = *(undefined4*)((int)pMsgParam + 0x48);
				local_44 = *(undefined4*)((int)pMsgParam + 0x4c);
				local_60 = 0x41800000;
				edF32Vector4SubHard(&eStack112, &(this->pOwner->base).characterBase.base.base.currentLocation,
					&pSender->currentLocation);
				edF32Vector4SafeNormalize1Hard(&eStack112, &eStack112);
				local_4 = local_90;
				CActor::DoMessage((CActor*)this->pOwner, (CActor*)this->pOwner, 2, (uint)local_4);
			}
			iVar4 = 1;
			goto LAB_001f0ee8;
		})
	}

	iVar4 = 0;
LAB_001f0ee8:
	this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 | 0x100;

	return iVar4;
}

edF32VECTOR4* CBehaviourWolfen::GetComeBackPosition()
{
	edF32MATRIX4 eStack64;
	CActor* pActor;

	pActor = this->pOwner->pTiedActor;
	if (pActor == (CActor*)0x0) {
		this->comeBackPosition = this->baseLocation;
	}
	else {
		pActor->SV_ComputeDiffMatrixFromInit(&eStack64);
		edF32Matrix4MulF32Vector4Hard(&this->comeBackPosition, &eStack64, &this->baseLocation);
	}

	return &this->comeBackPosition;
}

edF32VECTOR4* CBehaviourWolfen::GetComeBackAngles()
{
	float fVar1;
	float fVar2;
	float fVar3;
	edF32VECTOR4 eStack80;
	edF32MATRIX4 eStack64;
	CActor* pActor;

	pActor = this->pOwner->pTiedActor;
	if (pActor == (CActor*)0x0) {
		this->comeBackAngles = this->rotationEuler;
	}
	else {
		pActor->SV_ComputeDiffMatrixFromInit(&eStack64);
		SetVectorFromAngles(&eStack80, &this->rotationEuler.xyz);
		edF32Matrix4MulF32Vector4Hard(&eStack80, &eStack64, &eStack80);
		GetAnglesFromVector(&this->comeBackAngles.xyz, &eStack80);
		(this->comeBackAngles).w = 0.0f;
	}

	return &this->comeBackAngles;
}

int CBehaviourWolfen::GetTrackBehaviour()
{
	return -1;
}

CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* CBehaviourWolfen::GetNotificationTargetArray()
{
	return 0;
}

int CBehaviourWolfen::GetStateWolfenComeBack()
{
	edF32VECTOR4 eStack16;

	edF32Vector4SubHard_I(&eStack16, GetComeBackPosition(), &this->pOwner->currentLocation);
	float magnitude = edF32Vector4DotProductHard_I(&eStack16, &eStack16);

	int iVar6;

	if (0.5f <= sqrtf(magnitude)) {
		iVar6 = WOLFEN_STATE_COME_BACK;
	}
	else {
		iVar6 = GetStateWolfenGuard();
	}

	return iVar6;
}

int CBehaviourWolfen::GetStateWolfenTrack()
{
	int stateTrack;

	stateTrack = -1;

	if (GetTrackBehaviour() != this->pOwner->curBehaviourId) {
		stateTrack = 0xb1;
	}

	return stateTrack;
}

int CBehaviourWolfen::GetStateWolfenGuard()
{
	return WOLFEN_STATE_WATCH_DOG_GUARD;
}

int CBehaviourWolfen::GetStateWolfenWeapon(void)
{
	return WOLFEN_STATE_TRACK_DEFEND;
}

void CBehaviourWolfen::CheckDetection()
{
	CActorWolfen* pWolfen;
	CActorFighter* pTarget;

	pWolfen = this->pOwner;
	pTarget = pWolfen->pTargetActor_0xc80;

	if ((this->flags_0x4 & 1) == 0) {
		if ((pWolfen->combatFlags_0xb78 & 1) != 0) {
			pWolfen->combatFlags_0xb78 = pWolfen->combatFlags_0xb78 & 0xfffffffe;
		}
	}
	else {
		pWolfen->SV_WLF_IsIntruderMakingNoise(pTarget);
	}

	if ((this->flags_0x4 & 2) == 0) {
		pWolfen = this->pOwner;
		if ((pWolfen->combatFlags_0xb78 & 2) != 0) {
			pWolfen->combatFlags_0xb78 = pWolfen->combatFlags_0xb78 & 0xfffffffd;
		}
	}
	else {
		pWolfen->SV_WLF_IsIntruderInVitalSphere(pTarget);
	}

	if ((this->flags_0x4 & 4) == 0) {
		pWolfen = this->pOwner;
		if ((pWolfen->combatFlags_0xb78 & 4) != 0) {
			pWolfen->combatFlags_0xb78 = pWolfen->combatFlags_0xb78 & 0xfffffffb;
		}
	}
	else {
		pWolfen->SV_WLF_IsIntruderInVision(pTarget);
	}

	return;
}

void CBehaviourWolfen::CheckDetection_Intruder()
{
	uint uVar2;
	float fVar4;

	uVar2 = this->pOwner->GetStateWolfenFlags(this->pOwner->actorState);
	if ((uVar2 & 1) == 0) {
		return;
	}

	if ((this->pOwner->combatFlags_0xb78 & 0x10) != 0) {
		fVar4 = this->pOwner->pTargetActor_0xc80->GetLifeInterface()->GetValue();
		if (0.0f < fVar4) {
			CheckDetection();
			goto LAB_001f0688;
		}
	}

	this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 & 0xfffffff8;

LAB_001f0688:
	this->pOwner->UpdateInRange_001744a0((this->pOwner->combatFlags_0xb78 & 7) != 0);
	return;
}

int CBehaviourWolfen::FUN_001f0ab0()
{
	int iVar1;
	CActorWolfen* pWolfen;

	pWolfen = this->pOwner;
	iVar1 = -1;
	if ((pWolfen->combatFlags_0xb78 & 0x20000) == 0) {
		if ((pWolfen->pCollisionData->flags_0x4 & 2) != 0) {
			pWolfen->combatFlags_0xb78 = pWolfen->combatFlags_0xb78 | 0x20000;
		}
	}
	else {
		if ((pWolfen->fightFlags & 2) != 0) {
			iVar1 = pWolfen->FUN_0030a6a0();
		}
	}

	return iVar1;
}

int CBehaviourWolfen::TestState_001f09b0()
{
	CActorWolfen* pEnemy;
	int iVar1;

	pEnemy = this->pOwner;
	iVar1 = -1;
	if (((pEnemy->combatFlags_0xb78 & 0x400) != 0) && (0 < (int)pEnemy->combatMode_0xb7c)) {
		iVar1 = pEnemy->GetState_00174190();
	}

	return iVar1;
}

int CBehaviourWolfen::TestState_001f0a00()
{
	CActorWolfen* pCVar1;
	int iVar2;

	pCVar1 = this->pOwner;
	iVar2 = -1;
	if ((pCVar1->combatFlags_0xb78 & 0x4000) != 0) {
		iVar2 = 0x9f;
		pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 & 0xffffbfff;
	}

	return iVar2;
}

int CBehaviourWolfen::TestState_001f0a30()
{
	CActorWolfen* pCVar1;
	int iVar2;

	pCVar1 = this->pOwner;
	iVar2 = -1;
	if ((pCVar1->combatFlags_0xb78 & 0x8000) != 0) {
		iVar2 = 0xb5;
		pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 & 0xffff7fff;
	}

	return iVar2;
}

int CBehaviourWolfen::TestState_001f0a70()
{
	int iVar1;

	iVar1 = -1;
	if ((this->pOwner->combatFlags_0xb78 & 0x100) != 0) {
		iVar1 = WOLFEN_STATE_LOCATE;
	}

	return iVar1;
}

int CBehaviourWolfen::TestState_001f0a90()
{
	int iVar1;

	iVar1 = -1;
	if ((this->pOwner->combatFlags_0xb78 & 0x40) != 0) {
		iVar1 = 0x98;
	}

	return iVar1;
}

int CBehaviourWolfen::GetState_001f0930()
{
	bool bVar1;
	CBehaviour* pBehaviour;
	int newState;

	newState = -1;

	if ((((this->pOwner->pTargetActor_0xc80 != (CActorFighter*)0x0) && (0 < (int)this->pOwner->combatMode_0xb7c)) &&
		(bVar1 = this->pOwner->FUN_001738e0(this->pOwner->pTargetActor_0xc80), bVar1 != false)) &&
		(pBehaviour = this->pOwner->GetBehaviour(WOLFEN_BEHAVIOUR_DCA), pBehaviour != (CBehaviour*)0x0)) {
		newState = 0xb4;
	}

	return newState;
}

int CBehaviourWolfen::GetState_001f0b30()
{
	int newState;

	newState = 0x9b;
	if (1 < (int)this->pOwner->combatMode_0xb7c) {
		newState = 0x9a;
	}

	return newState;
}

int CBehaviourWolfen::GetState_001f08a0()
{
	CActorFighter* pIntruder;
	bool bVar1;
	int iVar2;
	int newState;

	pIntruder = this->pOwner->pTargetActor_0xc80;
	newState = -1;

	if (((pIntruder != (CActorFighter*)0x0) && (bVar1 = this->pOwner->CanSwitchToFight_Area(pIntruder), bVar1 != false)) &&
		(iVar2 = this->pOwner->pCommander->BeginFightIntruder(this->pOwner, pIntruder), iVar2 != 0)) {
		IMPLEMENTATION_GUARD(
		(*(code*)(this->pOwner->base).characterBase.base.base.pVTable[1].actorBase)(this->pOwner, pIntruder);)
		newState = 0xb0;
	}

	return newState;
}

void CBehaviourTrack::Create(ByteCode* pByteCode)
{
	CBehaviourWolfen::Create(pByteCode);

	GetNotificationTargetArray()->field_0x0 = pByteCode->GetF32();
}

void HearingDetectionProps::Create(ByteCode* pByteCode)
{
	const float range = pByteCode->GetF32();
	this->rangeSquared = range * range;
	this->maxHeightDifference = pByteCode->GetF32();

	return;
}

void VisionDetectionProps::Create(ByteCode* pByteCode)
{
	this->field_0x0 = pByteCode->GetF32();
	return;
}

void CBehaviourWolfenWeapon::Begin(CActor* pOwner, int newState, int newAnimationType)
{
	this->pOwner->GetWeapon();
	if (newState == -1) {
		this->pOwner->SetState(0x77, -1);
	}
	else {
		this->pOwner->SetState(newState, newAnimationType);
	}

	this->pOwner->flags = this->pOwner->flags | 0x400;
	
	return;
}

void CBehaviourWolfenWeapon::End(int newBehaviourId)
{
	this->pOwner->flags = this->pOwner->flags & 0xfffffbff;

	CBehaviourWolfen::End(newBehaviourId);

	return;
}

void CBehaviourTrackWeapon::Create(ByteCode* pByteCode)
{
	CBehaviourTrackWeaponStand::Create(pByteCode);

	this->field_0xf0 = pByteCode->GetF32();
	GetNotificationTargetArray()->field_0x0 = pByteCode->GetF32();
}

void CBehaviourTrackWeapon::Init(CActor* pOwner)
{
	CBehaviourTrackWeaponStand::Init(pOwner);

	return;
}

void CBehaviourTrackWeapon::Manage()
{
	int iVar1;
	int iVar3;

	this->pOwner->BehaviourTrackWeapon_Manage(this);
	iVar1 = FUN_002faf40();
	if (iVar1 != -1) {
		if (this->field_0xe8 != iVar1) {
			this->field_0xe8 = iVar1;
		}

		iVar1 = this->aSubObjs[this->field_0xe8].field_0x10;
		iVar3 = this->pOwner->GetWeapon()->FUN_002d57c0();
		if (iVar1 != iVar3) {
			this->pOwner->GetWeapon()->FUN_002d57e0(iVar1);
		}
	}

	return;
}

void CBehaviourTrackWeapon::Begin(CActor* pOwner, int newState, int newAnimationType)
{
	CActorWolfen* pWolfen;
	bool bVar2;
	int iVar3;

	if (newState == -1) {
		pWolfen = this->pOwner;
		bVar2 = pWolfen->IsFightRelated(pWolfen->prevBehaviourId);
		if ((bVar2 == false) || (pWolfen = this->pOwner, (pWolfen->combatFlags_0xb78 & 0x80000) == 0)) {
			newState = Func_0x70();
			newAnimationType = 6;
			if ((this->pOwner)->combatMode_0xb7c != ECM_InCombat) {
				newAnimationType = 7;
			}
		}
		else {
			IMPLEMENTATION_GUARD(
			iVar3 = pWolfen->FUN_00173a20();
			if (iVar3 == 0) {
				newState = 0x78;
			}
			else {
				newState = Func_0x70();
			})
		}
	}

	CBehaviourTrackWeaponStand::Begin(pOwner, newState, newAnimationType);

	return;
}

void CBehaviourTrackWeapon::TermState(int oldState, int newState)
{
	if (oldState == WOLFEN_STATE_TRACK_WEAPON_CHASE) {
		this->pOwner->SV_AUT_PathfindingEnd();
	}

	if ((oldState == WOLFEN_STATE_RELOAD) || (oldState == WOLFEN_STATE_FIRE)) {
		this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(1.0f, 0);
	}

	CBehaviourWolfen::TermState(oldState, newState);

	return;
}

int CBehaviourTrackWeapon::Func_0x70()
{
	if (this->pOwner->GetWeapon()->FUN_002d58a0() != 0) {
		if (this->pOwner->GetWeapon()->FUN_002d5830() == 0) {
			this->pOwner->field_0xcf8 = WOLFEN_STATE_TRACK_WEAPON_CHASE;
			return 0x97;
		}
	};
	return WOLFEN_STATE_TRACK_WEAPON_CHASE;
}

void CBehaviourTrackWeapon::Func_0x80(int* param_2, int* param_3, CActor* pTarget)
{
	CActorWolfen* pCVar1;
	float fVar2;
	float fVar3;

	pCVar1 = this->pOwner;
	fVar3 = (pTarget->currentLocation).x - pCVar1->currentLocation.x;
	fVar2 = (pTarget->currentLocation).z - pCVar1->currentLocation.z;
	if (this->field_0xf0 < sqrtf(fVar3 * fVar3 + 0.0f + fVar2 * fVar2)) {
		*param_3 = *param_3 + 1;
	}
	else {
		fVar3 = pTarget->SV_GetDirectionalAlignmentToTarget(&this->pOwner->currentLocation);
		if (0.001f < fVar3) {
			*param_2 = *param_2 + 1;
		}
		else {
			if (fVar3 < 0.001f) {
				*param_3 = *param_3 + 1;
			}
		}
	}

	return;
}

void CBehaviourTrackWeaponStand::Create(ByteCode* pByteCode)
{
	uint count;
	int iVar1;
	int* pBase;
	TrackSubObj* pTVar2;
	int iVar3;
	float fVar5;

	CBehaviourWolfen::Create(pByteCode);

	this->field_0x90 = pByteCode->GetF32();
	this->field_0x94 = pByteCode->GetS32();
	this->field_0x98 = pByteCode->GetS32();
	this->nbSubObjs = pByteCode->GetS32();

	this->aSubObjs = new TrackSubObj[this->nbSubObjs];

	iVar1 = 0;
	if (0 < this->nbSubObjs) {
		do {
			this->aSubObjs[iVar1].field_0x0 = pByteCode->GetS32();
			this->aSubObjs[iVar1].field_0x4.Create(pByteCode);
			this->aSubObjs[iVar1].field_0x10 = pByteCode->GetS32();
			iVar1 = iVar1 + 1;
		} while (iVar1 < this->nbSubObjs);
	}

	return;
}

void CBehaviourTrackWeaponStand::Init(CActor* pOwner)
{
	CBehaviourWolfen::Init(pOwner);

	if (this->pOwner->field_0xb90 <= this->field_0x90) {
		this->pOwner->GetVision();
	}

	return;
}

void CBehaviourTrackWeaponStand::Term()
{
	TrackSubObj* pTVar1;

	pTVar1 = this->aSubObjs;
	if (pTVar1 != (TrackSubObj*)0x0) {
		if (pTVar1 != (TrackSubObj*)0x0) {
			IMPLEMENTATION_GUARD_LOG(); // Diff from expected code.
			delete this->aSubObjs;
		}

		this->aSubObjs = (TrackSubObj*)0x0;
	}

	return;
}

void CBehaviourTrackWeaponStand::Manage()
{
	int iVar1;
	int iVar3;

	this->pOwner->BehaviourTrackWeaponStand_Manage(this);
	iVar1 = FUN_002faf40();
	if (iVar1 != -1) {
		if (this->field_0xe8 != iVar1) {
			this->field_0xe8 = iVar1;
		}

		iVar1 = this->aSubObjs[this->field_0xe8].field_0x10;
		iVar3 = this->pOwner->GetWeapon()->FUN_002d57c0();
		if (iVar1 != iVar3) {
			this->pOwner->GetWeapon()->FUN_002d57e0(iVar1);
		}
	}

	return;
}

void CBehaviourTrackWeaponStand::Begin(CActor* pOwner, int newState, int newAnimationType)
{
	CActorWolfen* pCVar1;
	uint uVar2;
	EEnemyCombatMode EVar3;
	bool bVar4;
	CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* paVar5;
	int iVar6;
	CActorWeapon* pActor;
	CActor* pCVar7;
	CActor* pOtherActor;
	float fVar8;
	float fVar9;
	edF32VECTOR4 eStack64;
	edF32VECTOR4 eStack48;
	edF32VECTOR4 local_20;
	edF32VECTOR4 local_10;

	this->pOwner->GetWeapon();

	if (newState == -1) {
		pCVar1 = this->pOwner;
		pCVar1->SetState(0x77, -1);
	}
	else {
		pCVar1 = this->pOwner;
		pCVar1->SetState(newState, newAnimationType);
	}
	pCVar1 = this->pOwner;
	pCVar1->flags = pCVar1->flags | 0x400;

	this->field_0xe8 = 0;
	this->field_0xec = 0;

	pCVar1 = this->pOwner;
	bVar4 = pCVar1->IsFightRelated(pCVar1->prevBehaviourId);
	if (bVar4 == false) {
		GetNotificationTargetArray()->field_0x34 = this->pOwner->prevBehaviourId;
	}

	if (newState == -1) {
		pCVar1 = this->pOwner;
		bVar4 = pCVar1->IsFightRelated(pCVar1->prevBehaviourId);
		if ((bVar4 == false) || (pCVar1 = this->pOwner, (pCVar1->combatFlags_0xb78 & 0x80000) == 0)) {
			pCVar1 = this->pOwner;
			pCVar1->SetState(GetStateWolfenWeapon(), -1);
		}
		else {
			IMPLEMENTATION_GUARD(
			iVar6 = FUN_00173a20((long)(int)pCVar1);
			if (iVar6 == 0) {
				pCVar1 = this->pOwner;
				pCVar1->SetState(0x78, -1);
			}
			else {
				pCVar1 = this->pOwner;
				iVar6 = (*(code*)(this->pVTable)->field_0x6c)(this);
				pCVar1->SetState(iVar6, -1);
			})
		}
	}
	else {
		pCVar1 = this->pOwner;
		pCVar1->SetState(newState, newAnimationType);
	}

	pCVar1 = this->pOwner;
	pOtherActor = pCVar1->pTargetActor_0xc80;
	if (pOtherActor == (CActor*)0x0) {
		edF32Vector4ScaleHard(3.0f, &local_10, &pCVar1->rotationQuat);
		edF32Vector4AddHard(&local_10, &local_10, &this->pOwner->currentLocation);
		this->field_0x80 = local_10;
	}
	else {
		uVar2 = this->aSubObjs[this->field_0xe8].field_0x4.field_0x0;
		pActor = pCVar1->GetWeapon();
		this->pOwner->SV_GetActorHitPos(pOtherActor, &local_20);
		if ((uVar2 & 1) != 0) {
			edF32Vector4SubHard(&eStack48, &local_20, &pActor->currentLocation);
			fVar8 = edF32Vector4GetDistHard(&eStack48);
			fVar9 = pActor->FUN_002d5710();
			pCVar7 = pOtherActor->GetCollidingActor();
			do {
				if (pCVar7 == pOtherActor) {
					pOtherActor = (CActor*)0x0;
				}
				else {
					bVar4 = pOtherActor->IsKindOfObject(2);
					if (bVar4 != false) {
						CActorMovable* pMovable = static_cast<CActorMovable*>(pOtherActor);
						edF32Vector4ScaleHard((fVar8 / fVar9) * pMovable->dynamic.linearAcceleration, &eStack64, &pMovable->dynamic.velocityDirectionEuler);
						edF32Vector4AddHard(&local_20, &local_20, &eStack64);
					}

					pOtherActor = pOtherActor->pTiedActor;
				}
			} while (pOtherActor != (CActor*)0x0);
		}

		local_10 = local_20;
		this->field_0x80 = local_10;
	}

	GetNotificationTargetArray()->FUN_003c3a30();
	GetNotificationTargetArray()->combatMode = (this->pOwner)->combatMode_0xb7c;

	pCVar1 = this->pOwner;
	if ((int)pCVar1->combatMode_0xb7c < 2) {
		pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 & 0xffefffff;
	}
	else {
		pCVar1->combatFlags_0xb78 = pCVar1->combatFlags_0xb78 | 0x100000;
	}

	return;
}

void CBehaviourTrackWeaponStand::End(int newBehaviourId)
{
	this->pOwner->flags = this->pOwner->flags & 0xfffffbff;

	CBehaviourWolfen::End(newBehaviourId);

	if ((this->pOwner->combatFlags_0xb78 & 0x140) != 0) {
		this->pOwner->combatFlags_0xb78 = this->pOwner->combatFlags_0xb78 & 0xfffffebf;
	}

	return;
}

void CBehaviourTrackWeaponStand::InitState(int newState)
{
	CActorWeapon* pWeapon;
	StateConfig* pConfig;
	int iVar1;
	float fVar2;
	float fVar3;
	CAnimation* pAnim;
	CActorWolfen* pWolfen;

	if (newState == 0x97) {
		IMPLEMENTATION_GUARD(
		pWeapon = CActorFighter::GetWeapon((CActorFighter*)this->pOwner);
		FUN_002d5860((int)pWeapon, 1);)
	}
	else {
		if (newState == 0x96) {
			IMPLEMENTATION_GUARD(
			pWeapon = CActorFighter::GetWeapon((CActorFighter*)this->pOwner);
			FUN_002d5860((int)pWeapon, 0);)
		}
		else {
			if (newState == WOLFEN_STATE_AIM) {
				this->pOwner->SV_AUT_WarnActors(this->pOwner->field_0xcf4, 0.0f, (CActor*)0x0);
			}
			else {
				if (newState == WOLFEN_STATE_RELOAD) {
					if (this->field_0x98 == 1) {
						fVar2 = this->pOwner->GetWeapon()->GetLaunchSpeed(3);
						pWolfen = this->pOwner;
						pAnim = pWolfen->pAnimationController;
						pConfig = pWolfen->GetStateCfg(WOLFEN_STATE_RELOAD);
						iVar1 = pWolfen->GetIdMacroAnim(pConfig->animId);
						if (iVar1 < 0) {
							fVar3 = 0.0f;
						}
						else {
							fVar3 = pAnim->GetAnimLength(iVar1, 0);
						}

						this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(fVar3 / fVar2, 0);
					}
				}
				else {
					if (newState == WOLFEN_STATE_FIRE) {
						if (this->field_0x94 == 1) {
							fVar2 = this->pOwner->GetWeapon()->GetLaunchSpeed(2);
							pWolfen = this->pOwner;
							pAnim = pWolfen->pAnimationController;
							pConfig = pWolfen->GetStateCfg(WOLFEN_STATE_FIRE);
							iVar1 = pWolfen->GetIdMacroAnim(pConfig->animId);
							if (iVar1 < 0) {
								fVar3 = 0.0f;
							}
							else {
								fVar3 = pAnim->GetAnimLength(iVar1, 0);
							}

							this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(fVar3 / fVar2, 0);
						}
					}
					else {
						if (newState == WOLFEN_STATE_TRACK_DEFEND) {
							this->field_0xec = Timer::GetTimer()->scaledTotalTime;
						}
					}
				}
			}
		}
	}

	CBehaviourWolfen::InitState(newState);

	return;
}

void CBehaviourTrackWeaponStand::TermState(int oldState, int newState)
{
	if ((oldState == WOLFEN_STATE_RELOAD) || (oldState == WOLFEN_STATE_FIRE)) {
		this->pOwner->pAnimationController->anmBinMetaAnimator.SetLayerTimeWarper(1.0f, 0);
	}

	CBehaviourWolfen::TermState(oldState, newState);
	return;
}

CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* CBehaviourTrackWeaponStand::GetNotificationTargetArray()
{
	return &this->notificationTargetArray;
}

int CBehaviourTrackWeaponStand::GetStateWolfenWeapon()
{
	if (this->pOwner->GetWeapon()->FUN_002d58a0() != 0) {
		if (this->pOwner->GetWeapon()->FUN_002d5830() == 0) {
			(this->pOwner)->field_0xcf8 = WOLFEN_STATE_TRACK_DEFEND;
			return 0x97;
		}
	}

	return WOLFEN_STATE_TRACK_DEFEND;
}

int CBehaviourTrackWeaponStand::Func_0x74()
{
	if (this->pOwner->GetWeapon()->FUN_002d58a0() != 0) {
		if (this->pOwner->GetWeapon()->FUN_002d5830() == 0) {
			(this->pOwner)->field_0xcf8 = WOLFEN_STATE_AIM;
			return 0x96;
		}
	}

	return WOLFEN_STATE_AIM;
}

int CBehaviourTrackWeaponStand::Func_0x78()
{
	return notificationTargetArray.GetState_003c37c0(this->pOwner);
}

void CBehaviourTrackWeaponStand::Func_0x80(int* param_2, int* param_3, CActor* pTarget)
{
	float fVar1;

	fVar1 = pTarget->SV_GetDirectionalAlignmentToTarget(&this->pOwner->currentLocation);

	if (0.001f < fVar1) {
		*param_2 = *param_2 + 1;
	}
	else {
		if (fVar1 < 0.001f) {
			*param_3 = *param_3 + 1;
		}
	}

	return;
}

int CBehaviourTrackWeaponStand::FUN_002faf40()
{
	int iVar1;
	TrackSubObj* pTVar2;
	int iVar3;
	int iVar4;
	int local_8;
	int local_4;

	local_4 = 0;
	local_8 = 0;
	if (this->pOwner->pTargetActor_0xc80 != (CActorFighter*)0x0) {
		Func_0x80(&local_4, &local_8, this->pOwner->pTargetActor_0xc80);
	}
	iVar1 = -1;
	iVar4 = 0;
	iVar3 = 0;
	if (0 < this->nbSubObjs) {
		pTVar2 = this->aSubObjs;
		do {
			if ((pTVar2->field_0x0 == 0) && (iVar4 < local_4)) {
				iVar1 = iVar3;
				iVar4 = local_4;
			}
			if ((pTVar2->field_0x0 == 1) && (iVar4 < local_8)) {
				iVar1 = iVar3;
				iVar4 = local_8;
			}
			iVar3 = iVar3 + 1;
			pTVar2 = pTVar2 + 1;
		} while (iVar3 < this->nbSubObjs);
	}
	return iVar1;
}

astruct_16::astruct_16()
{
	this->field_0x0 = 0;
	this->field_0x4 = 1.0;
	this->field_0x8 = 5.0;

	return;
}

void astruct_16::Create(ByteCode* pByteCode)
{
	this->field_0x0 = pByteCode->GetU32();
	this->field_0x4 = pByteCode->GetF32();
	this->field_0x8 = pByteCode->GetF32();

	return;
}

CNotificationTargetArray<S_STREAM_NTF_TARGET_ONOFF>* CBehaviourTrackStand::GetNotificationTargetArray()
{
	return &this->notificationTargetArray;
}

void CBehaviourLost::Create(ByteCode* pByteCode)
{
	CBehaviourWolfen::Create(pByteCode);

	this->switchBehaviour.Create(pByteCode);

	return;
}

void CBehaviourLost::Init(CActor* pOwner)
{
	CBehaviourWolfen::Init(pOwner);

	this->switchBehaviour.Init(pOwner);

	return;
}

void CBehaviourLost::Manage()
{
	this->pOwner->BehaviourStand_Manage(this);
}

void CBehaviourLost::Begin(CActor* pOwner, int newState, int newAnimationType)
{
	if (newState == -1) {
		this->pOwner->SetState(0x72, -1);
	}
	else {
		this->pOwner->SetState(newState, newAnimationType);
	}

	this->switchBehaviour.Begin(pOwner);

	return;
}

int CBehaviourLost::InterpretMessage(CActor* pSender, int msg, void* pMsgParam)
{
	int iVar1 = this->switchBehaviour.InterpretMessage(this->pOwner, pSender, msg, pMsgParam);

	if (iVar1 == 0) {
		iVar1 = CBehaviourWolfen::InterpretMessage(pSender, msg, pMsgParam);
	}

	return iVar1;
}
