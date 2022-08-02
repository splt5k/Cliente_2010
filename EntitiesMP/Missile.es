//Temp (Only for Test) ����
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
1005
%{
#include "stdH.h"
#include <Engine/Math/Quaternion.h>
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CRefCountPtr.h>
#include <Engine/Effect/CEntityTag.h>
#include <Engine/Effect/CTagManager.h>
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Effect/CEffectGroup.h>

#include "EntitiesMP/Player.h"
#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/CharacterBase.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Effect/CSampleSpline.h>
#include <Engine/Effect/CMdlEffect.h>
#include <Engine/Entities/Skill.h>
#include <Engine/World/WorldRayCasting.h>
#include <Engine/Network/MessageDefine.h>


typedef CSampleSpline< FLOAT3D, CCubicSplineInterpolate<FLOAT3D> > cubic_spline;
#define GetSpline() (*((cubic_spline*)m_pssPath))

// NOTE : ���ϰ� �и��� �̱۴���...
#define	EXTREAM_SINGLEDUNGEON		(10)

%}

uses "EntitiesMP/Light";
uses "EntitiesMP/Player";
//������ ���� ���� ���� ���� �۾�	08.22
uses "EntitiesMP/Enemy";
uses "EntitiesMP/Character";
//������ ���� �� ���� ���� �۾�		08.22

enum MissileType{                
	0 MLT_NORMALARROW	"NormalArrow",		//�Ϲ� ȭ��, ska����
	1 MLT_SOMETHING		"Something",		//�Ϲ� ȭ��� �����ϳ� ska����.
	2 MLT_FALLDOWN		"FallDown",			//ska����. �ϴÿ��� ������.
	3 MLT_GOWAITBACK	"Go, Wait, Back",	//ska����. �ϴ� ���ٰ� ���� �ð� ��ٸ��� �ٽ� ���ƿ�.
	4 MLT_CONNECT		"Connect",			//ska����. �߻��� ��ü�� ��ǥ���� ���� �ð� ���� ������.(tag�̿�)
};

%{
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CTagManager.h>
#include <Engine/Ska/ModelInstance.h>
void ShotMissile(CEntity *penShoter, const char *szShotPosTagName //Maykom Fun��o bugada
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX iArrowType = 0, const char *szMissileModel = NULL)		// iArrowType=0 : normal arrow, iArrowType=1 : magic arrow, iArrowType=etc : ??
{
	//�Է� ���� �˻�.
	ASSERT(penShoter != NULL && penShoter->en_pmiModelInstance != NULL);
	ASSERT(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f);
	ASSERT(szShotPosTagName != NULL && szHitEffectName != NULL && szArrowEffectName != NULL);
	ASSERT(fHorizonalOffset <= 1.0f && fHorizonalOffset >= -1.0f);
	ASSERT(fVerticalOffset <= 1.0f && fVerticalOffset >= -1.0f);
	if(!(penShoter != NULL && penShoter->en_pmiModelInstance != NULL)) return;
	if(!(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f)) return;
	if(!(szShotPosTagName != NULL && szHitEffectName != NULL && szArrowEffectName != NULL)) return;

	//missile ����.
	CTag *pTag = penShoter->en_pmiModelInstance->m_tmSkaTagManager.Find(szShotPosTagName);
	//if(pTag == NULL) pTag = penShoter->m_ptmTagManager->Find(szShotPosTagName);
	ASSERT(pTag != NULL);
	if(pTag == NULL) return;
	CPlacement3D plMissile;
	plMissile.pl_PositionVector = pTag->CurrentTagInfo().m_vPos;
	plMissile.pl_OrientationAngle = GetEulerAngleFromDir(penTarget->en_plPlacement.pl_PositionVector - penShoter->en_plPlacement.pl_PositionVector);
	CMissile *penMissile = (CMissile *)penShoter->en_pwoWorld->CreateEntity_t(plMissile, "classes\\missile.ecl");

	//missile ���� ����.
	penMissile->m_epSource = penShoter;
	penMissile->m_epTarget = penTarget;	//target entity
	penMissile->m_fMoveSpeed = fMoveSpeed;
	penMissile->m_fHorizonalOffset = fHorizonalOffset;
	penMissile->m_fVerticalOffset = fVerticalOffset;
	penMissile->m_strHitEffect = szHitEffectName;
	if(iArrowType == 0)
	{
		penMissile->m_eMissileType = MLT_NORMALARROW;
		penMissile->m_fnModel = CTFileName("data\\effect\\ska\\ArrowNormal.smc");	//Hardcoding
	}
	else if(iArrowType == 1 )
	{
		penMissile->m_eMissileType = MLT_SOMETHING;
		penMissile->m_fAfterDelay = 0.0f;
		penMissile->m_fnModel = CTFileName("data\\effect\\ska\\magic_arr.smc");	//Hardcoding
	}
	else
	{
		penMissile->m_eMissileType = MLT_SOMETHING;
		penMissile->m_fAfterDelay = 0.0f;
		penMissile->m_fnModel = szMissileModel;
	}
	penMissile->m_bNoPeak = FALSE;
	penMissile->Initialize();
	
	penMissile->m_bNormalHit = TRUE;
	penMissile->m_bCritical = bCritical;
	penMissile->m_ulTargetID = penMissile->en_ulID;

	//Tag �� Effect ���� ����
	if(penMissile->en_pmiModelInstance != NULL)
	{
		penMissile->en_pmiModelInstance->RefreshTagManager();
		penMissile->en_pmiModelInstance->m_tmSkaTagManager.SetOwner(penMissile);
		penMissile->m_pMyEffectGroup = StartEffectGroup(szArrowEffectName
			, &penMissile->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

//orbit type
//-1 : nothing
//0 : Default(invisible)
//1 : Cutter
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE)
{
	//�Է� ���� �˻�.
	ASSERT(penShoter != NULL && penShoter->en_pmiModelInstance != NULL);
	ASSERT(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f);
	if(!(penShoter != NULL && penShoter->en_pmiModelInstance != NULL)) return;
	if(!(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f)) return;
	if(!(szHitEffectName != NULL && szMagicEffectName != NULL)) return;

	//missile ����.
	CPlacement3D plMissile;
	plMissile.pl_PositionVector = vStartPos;
	plMissile.pl_OrientationAngle = GetEulerAngleFromQuaternion(qStartRot);
	//plMissile.pl_OrientationAngle = GetEulerAngleFromDir(penTarget->en_plPlacement.pl_PositionVector - penShoter->en_plPlacement.pl_PositionVector);
	CMissile *penMissile = (CMissile *)penShoter->en_pwoWorld->CreateEntity_t(plMissile, "classes\\missile.ecl");

	//missile ���� ����.
	penMissile->m_epSource = penShoter;
	penMissile->m_epTarget = penTarget;	//target entity
	penMissile->m_fMoveSpeed = fMoveSpeed;
	penMissile->m_strHitEffect = szHitEffectName;
	penMissile->m_eMissileType = MLT_SOMETHING;
	penMissile->m_bDirectTag = bDirectTag;
	penMissile->m_iFireObjectType = iOrbitType; // ��ų ������Ʈ Ÿ��
	if(iOrbitType == CSkill::MT_MAGECUTTER)
	{
		static FLOAT angleSpeed = 90.0f;
		penMissile->m_aAngleSpeed = angleSpeed;
		penMissile->m_fnModel = CTFileName("data\\effect\\ska\\mCutter.smc");	//Hardcoding
		penMissile->m_fAfterDelay = 0.0f;
	}
	else	//Default
	{
		penMissile->m_fnModel = CTFileName("data\\effect\\ska\\invisible.smc");	//Hardcoding
		penMissile->m_fAfterDelay = 0.0f;
	}

	penMissile->m_bNormalHit = FALSE;
	penMissile->m_fHorizonalOffset = 0;
	penMissile->m_fVerticalOffset = 0.15f;
	penMissile->m_bNoPeak = TRUE;

	penMissile->Initialize();

	if(iOrbitType == CSkill::MT_MAGECUTTER && penMissile->en_pmiModelInstance)
	{
		static FLOAT3D qRot(0,0,0);
		penMissile->en_pmiModelInstance->mi_qvOffset.qRot.FromEuler(qRot);
		penMissile->en_pmiModelInstance->mi_vStretch *= 0.35f; 
		penMissile->en_pmiModelInstance->mi_colModelColor = 0x828282FF;
		penMissile->en_pmiModelInstance->AddAnimation(
			penMissile->en_pmiModelInstance->FindFirstAnimationID(), AN_LOOPING|AN_CLEAR, 1, 0);
	}

	penMissile->m_ulTargetID = penMissile->en_ulID;
	penMissile->m_bCritical = bCritical;

	//Tag �� Effect ���� ����
	if(penMissile->en_pmiModelInstance != NULL)
	{
		penMissile->en_pmiModelInstance->RefreshTagManager();
		penMissile->en_pmiModelInstance->m_tmSkaTagManager.SetOwner(penMissile);
		penMissile->m_pMyEffectGroup = StartEffectGroup(szMagicEffectName
			, &penMissile->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

void ShotFallDown(FLOAT3D vStartPos, FLOAT3D vToTargetDir, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szFallObjEffectName
				 , bool bCritical)
{
	ASSERT(szHitEffectName != NULL && szFallObjEffectName != NULL);
	ASSERT(vToTargetDir.Length() > 0.99f && vToTargetDir.Length() < 1.01f);

	//Target ��ġ ��� by ray cast
	FLOAT3D vTargetPos;
	FLOAT fMaxY = -9999999.0f;
	BOOL bFloorHitted = FALSE;
	FLOAT3D vSource = vStartPos;
	FLOAT3D vTarget = vStartPos - vToTargetDir * 100.0f;
	CCastRay crRay(NULL, vSource, vTarget);
	crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
	crRay.cr_bHitTranslucentPortals = TRUE;
	crRay.cr_bPhysical = TRUE;
	CEntity *pen = CEntity::GetPlayerEntity(0);
	pen->GetWorld()->CastRay(crRay);
	if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
	{
		bFloorHitted = TRUE;
	}
	if(bFloorHitted)
	{
		vTargetPos = crRay.cr_vHit;
	}
	else
	{
		return;
	}
	
	//missile ����.
	CPlacement3D plMissile;
	plMissile.pl_PositionVector = vStartPos;
	plMissile.pl_OrientationAngle = GetEulerAngleFromDir(vTargetPos - vStartPos);
	CMissile *penMissile = (CMissile *)pen->en_pwoWorld->CreateEntity_t(plMissile, "classes\\missile.ecl");

	//missile ���� ����.
	penMissile->m_fnModel = CTFileName("data\\effect\\ska\\invisible.smc");	//Hardcoding
	penMissile->m_bEntityTarget = FALSE;
	penMissile->m_vTargetPos = vTargetPos;
	penMissile->m_fMoveSpeed = fMoveSpeed;
	penMissile->m_strHitEffect = szHitEffectName;
	penMissile->m_eMissileType = MLT_FALLDOWN;
	penMissile->m_bNormalHit = FALSE;
	penMissile->m_fHorizonalOffset = 0;
	penMissile->m_fVerticalOffset = 0.15f;
	penMissile->m_bNoPeak = TRUE;
	penMissile->Initialize();

	penMissile->m_bCritical = bCritical;
	penMissile->m_ulTargetID = penMissile->en_ulID;

	//Tag �� Effect ���� ����
	if(penMissile->en_pmiModelInstance != NULL)
	{
		penMissile->en_pmiModelInstance->RefreshTagManager();
		penMissile->en_pmiModelInstance->m_tmSkaTagManager.SetOwner(penMissile);
		penMissile->m_pMyEffectGroup = StartEffectGroup(szFallObjEffectName
			, &penMissile->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

void ShotGoWaitBack(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szMoveEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 )
{
	//�Է� ���� �˻�.
	ASSERT(penShoter != NULL && penShoter->en_pmiModelInstance != NULL);
	ASSERT(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f);
	ASSERT(szHitEffectName != NULL && szMoveEffectName != NULL);
	ASSERT(fHorizonalOffset <= 1.0f && fHorizonalOffset >= -1.0f);
	ASSERT(fVerticalOffset <= 1.0f && fVerticalOffset >= -1.0f);
	if(!(penShoter != NULL && penShoter->en_pmiModelInstance != NULL)) return;
	if(!(penTarget != NULL && penTarget->en_pmiModelInstance != NULL && fMoveSpeed > 0.0f)) return;
	if(!(szHitEffectName != NULL && szMoveEffectName != NULL)) return;

	//missile ����.
	CPlacement3D plMissile;
	plMissile.pl_PositionVector = vStartPos;
	plMissile.pl_OrientationAngle = GetEulerAngleFromQuaternion(qStartRot);
	CMissile *penMissile = (CMissile *)penShoter->en_pwoWorld->CreateEntity_t(plMissile, "classes\\missile.ecl");

	//missile ���� ����.
	penMissile->m_epSource = penShoter;
	penMissile->m_epTarget = penTarget;	//target entity
	penMissile->m_fMoveSpeed = fMoveSpeed;
	penMissile->m_fHorizonalOffset = fHorizonalOffset;
	penMissile->m_fVerticalOffset = fVerticalOffset;
	penMissile->m_strHitEffect = szHitEffectName;
	penMissile->m_fAfterDelay = fWaitTime;
	penMissile->m_eMissileType = MLT_GOWAITBACK;
	penMissile->m_strMoveEffect = szMoveEffectName;

	penMissile->m_bNoPeak = FALSE;
	penMissile->Initialize();
	
	penMissile->m_bNormalHit = TRUE;
	penMissile->m_bCritical = bCritical;
	penMissile->m_ulTargetID = penMissile->en_ulID;

	//Tag �� Effect ���� ����
	if(penMissile->en_pmiModelInstance != NULL)
	{
		penMissile->en_pmiModelInstance->RefreshTagManager();
		penMissile->en_pmiModelInstance->m_tmSkaTagManager.SetOwner(penMissile);
		penMissile->m_pMyEffectGroup = StartEffectGroup(szMoveEffectName
			, &penMissile->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
	}
}

void ShotConnect(CEntity *penShoter, CEntity *penTarget, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szConnectEffectName)
{
	//�Է� ���� �˻�.
	ASSERT(penShoter != NULL && penShoter->en_pmiModelInstance != NULL);
	ASSERT(penTarget != NULL && penTarget->en_pmiModelInstance != NULL);
	ASSERT(szHitEffectName != NULL && szConnectEffectName != NULL);
	if(!(penShoter != NULL && penShoter->en_pmiModelInstance != NULL)) return;
	if(!(penTarget != NULL && penTarget->en_pmiModelInstance != NULL)) return;
	if(!(szHitEffectName != NULL && szConnectEffectName != NULL)) return;

	//missile ����.
	CPlacement3D plNew;
	FLOAT3D vSourcePos = penShoter->GetPlacement().pl_PositionVector;
	plNew.pl_PositionVector = (vSourcePos + penTarget->GetPlacement().pl_PositionVector) * 0.5f;
	FLOAT3D vDir = vSourcePos - plNew.pl_PositionVector;
	FLOAT fLen = vDir.Length();
	vDir /= fLen;
	fLen *= 2;
	plNew.pl_OrientationAngle = GetEulerAngleFromDir(vDir);
	plNew.pl_PositionVector(2) += 1.0f;//�̰� ���ϸ� �ٴڿ� �پ ����Ʈ�� ����.

	CMissile *penMissile = (CMissile *)penShoter->en_pwoWorld->CreateEntity_t(plNew, "classes\\missile.ecl");

	//missile ���� ����.
	penMissile->m_epSource = penShoter;
	penMissile->m_epTarget = penTarget;	//target entity
	penMissile->m_strHitEffect = szHitEffectName;
	penMissile->m_fAfterDelay = fWaitTime;
	penMissile->m_eMissileType = MLT_CONNECT;
	penMissile->m_pMyEffectGroup = NULL;

	penMissile->Initialize();
	penMissile->m_pMyEffectGroup = NULL;

	penMissile->m_ulTargetID = penMissile->en_ulID;

	//Tag �� Effect ���� ����
	if(penMissile->en_pmiModelInstance != NULL)
	{
		penMissile->en_pmiModelInstance->RefreshTagManager();
		penMissile->en_pmiModelInstance->m_tmSkaTagManager.SetOwner(penMissile);
		penMissile->m_pMyEffectGroup = StartEffectGroup(szConnectEffectName
			, &penMissile->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

		if(penMissile->m_pMyEffectGroup)
		{
			for(INDEX i=0; i<penMissile->m_pMyEffectGroup->GetEffectKeyVector().size(); ++i)
			{
				CEffect *pEffect = penMissile->m_pMyEffectGroup->GetEffectKeyVector()[i].m_pCreatedEffect;
				if(pEffect != NULL && pEffect->GetType() == ET_MDL)
				{
					FLOAT3D vNewStretch = ((CMdlEffect*)pEffect)->GetStretch();
					vNewStretch(2) = fLen;
					((CMdlEffect*)pEffect)->SetStretch(vNewStretch);
				}
			}
		}
	}
}

%}

//���󰡴� Effect������ �� ��ƼƼ
//CProjectile�� �ֱ��ϳ� �ʹ� ��ġ�� Ŀ�� ���� ����.
//CProjectile�� �����Ͽ� ��������Ƿ� ���� ������ �ƴ϶�� �̰ɷ� ��� ���� ��ü�� ������.
//����, �, ���� �� ���� Ÿ���� �����ϸ鼭 ������ ���� ����.
//�̰��� ���� �߿����� ���̰� ������� ���� �ʴ´�.
class CMissile : CRationalEntity
{
name      "Missile";
thumbnail "Missile.tbn";
features  "IsTargetable";

properties:
	 1 BOOL				m_bInit = FALSE,
	 2 enum MissileType	m_eMissileType		"Missile Type" = MLT_NORMALARROW,
	14 BOOL				m_bEntityTarget		"Entity Target" = TRUE,
	 3 CEntityPointer	m_epTarget			"Target",
	17 CEntityPointer	m_epSource			"Source",
	15 FLOAT3D			m_vTargetPos		"Target Pos" = FLOAT3D(0,0,0),
	 4 FLOAT			m_fMoveSpeed		"Move Speed" = 1.0f,
	 5 FLOAT			m_fBeforeDelay		"Delay Before Fire" = 0.0f,
	 6 FLOAT			m_fAfterDelay		"Delay After Fire" = 3.5f,
	 7 CTString			m_strHitEffect		"Hit Effect Name" = "",
	18 CTString			m_strMoveEffect		"Move Effect Name" = "",
	 8 CTFileName		m_fnModel			"Model file (.smc)" = CTFILENAME(""),
	 9 FLOAT			m_fHorizonalOffset	"Horizonal Offset" = 0.0f,
	10 FLOAT			m_fVerticalOffset	"Vertical Offset" = 0.0f,
	11 CSoundObject		m_soSound,
	12 BOOL				m_bCritical	= FALSE,
	13 ANGLE			m_aAngleSpeed = 0.0f,
	16 BOOL				m_bDirectTag = TRUE,
	19 INDEX			m_iFireObjectType = 0, // CSkill::MT_NONE
	{
		BOOL			m_bReached;
		BOOL			m_bReached2;
		FLOAT			m_fDeltaTime;
		FLOAT			m_fLastTime;
		FLOAT			m_fLeftDistance;
		CEffectGroup	*m_pMyEffectGroup;
		CTString		m_strHitTag;
		INDEX			m_iIndex;
		FLOAT3D			m_vInitPos;
		void			*m_pssPath;
		FLOAT			m_fRatio;
		FLOAT			m_fTimeRatio;
		FLOAT3D			m_vLastPos;
		BOOL			m_bNoPeak;
		FLOAT			m_fTimeFromStart;
		BOOL			m_bNormalHit;
		ULONG			m_ulTargetID;
		FLOAT3D			m_vTargetCenterPos;
		BOOL			m_bMoveFail;
	}
components:
	1 sound SOUND_BOW_BLOW                "Data\\Sounds\\Character\\public\\C_blow_bow01.wav",
	2 sound SOUND_BOW_CRITICAL            "Data\\Sounds\\Character\\public\\C_critical_blow_bow01.wav",
	3 sound SOUND_CROSSBOW_BLOW           "Data\\Sounds\\Character\\public\\C_blow_cross01.wav",
	
functions:
	void RemakeSplinePath()
	{
		if(!m_bEntityTarget) {return;}
		if(m_bNoPeak)
		{
			GetSpline().RemoveSample(1);
		}
		else
		{
			GetSpline().RemoveSample(2);
		}
/*
		const FLOAT3D vTargetBSInfo = GetBoundingSphereInfo(m_epTarget);
		//�������� ���� �ٿ�� �ڽ��� ��� ���̸� �������ΰ��� ����, HEIGHT_FACTOR�� �߰�
		const FLOAT3D vTargetPos = m_epTarget->en_plPlacement.pl_PositionVector
									+ FLOAT3D(0, (vTargetBSInfo(3) - vTargetBSInfo(2)) * 0.4f, 0);
*/
		FLOAT3D vTargetPos = m_epTarget->en_plPlacement.pl_PositionVector;
		vTargetPos(2) += m_vTargetCenterPos(2);
		GetSpline().AddSample(1.0f, vTargetPos);
		GetSpline().Prepare();
	}

	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(m_bCritical)
		{
			PlaySound(m_soSound, SOUND_BOW_CRITICAL, SOF_3D|SOF_VOLUMETRIC);	
			m_bCritical = FALSE;	
		}
		else
		{
			PlaySound(m_soSound, SOUND_BOW_BLOW, SOF_3D|SOF_VOLUMETRIC);			
		}

		fDamageAmmount = 0;		
		if( penToDamage )
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }
			_pUIMgr->ShowDamage( penToDamage->en_ulID );
			INDEX preHealth, curHealth;
			preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

			if(preHealth!= -1 && curHealth > preHealth)
			{
				fDamageAmmount = 1;
				((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;
				
			}
			else fDamageAmmount = 0;
			if(penToDamage ==_pNetwork->_TargetInfo.pen_pEntity)
			{
				if( penToDamage->IsCharacter() )
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth,((CCharacter*) penToDamage)->m_nPkMode,((CCharacter*) penToDamage)->m_nPkState,((CCharacter*) penToDamage)->m_nLegit);		
				}
				else
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth);
				}
			}

			if(((CUnit*)penToDamage)->m_nCurrentHealth ==0)//0827
			{
				if(penToDamage == _pNetwork->_TargetInfo.pen_pEntity)
				{
					_pNetwork->_TargetInfo.Init();
				}
			}	
		}	

		// FIXME : �̱� ����̱�... Ư�� ���� �϶�... HardCoding
		// FIXME : �̱� �����϶��� ���� �����ϹǷ�...
		// FIXME : ���� �ȵ��...��.��
		if( _pNetwork->m_bSingleMode )
		{
			// FIXME : 10�� ���� Ư���ϰ� ó��...��.��
			// FIXME : ���� �и����� �ؾ� ��.
			if( g_slZone == EXTREAM_SINGLEDUNGEON && !((CEnemy*)penToDamage)->m_bBoss )
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION, 30.0f, vHitPoint, vDirection);
			}
			else
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION, fDamageAmmount, vHitPoint, vDirection);
			}
		}
		else
		{
			CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType, fDamageAmmount,vHitPoint, vDirection);
		}

		if(penToDamage && penToDamage->IsEnemy())
		{
			const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;
			// ������ �����϶�....
			if( iMobIndex == LORD_SYMBOL_INDEX)
			{
				CMobData& MD = _pNetwork->GetMobData(iMobIndex);
				INDEX iCurHealth = ((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth;
				static INDEX iType = 0;
				float fMaxHealth = MD.GetHealth();
				if( iCurHealth <= fMaxHealth * 0.25f)
				{
					if(iType != 1 )
					{
						iType = 1;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword04.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else if( iCurHealth <= fMaxHealth * 0.50f)
				{
					if( iType != 2 )
					{
						iType = 2;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword03.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else if( iCurHealth <= fMaxHealth * 0.75f)
				{
					if( iType != 3 )
					{
						iType = 3;
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword02.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}
				else
				{
					if( iType != 0 )
					{
						iType = 0;
						penToDamage->SetSkaModel(MD.GetMobSmcFileName());
						penToDamage->GetModelInstance()->RefreshTagManager();
					}
				}

			}
		}	

		if(penToDamage->IsEnemy())
		{
			if(((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0)
			{
				CMobData& MD = _pNetwork->GetMobData(((CEnemy*)(CEntity*)penToDamage)->m_nMobDBIndex);
				if(!MD.IsCastleTower())
				{
					((CUnit*)((CEntity*)penToDamage))->DeathNow();
				}
				else
				{
					// WSS_DRATAN_SEIGEWARFARE 2007/08/31
					// ���� ��ȣ���� ���ź Ÿ�� ����ó�� ( ������ Ÿ�� ���� )
					if(MD.GetMobIndex()>=352 && MD.GetMobIndex()<=399)
					{
						((CUnit*)((CEntity*)penToDamage))->DeathNow();
					}
					else 
					{
						((CUnit*)((CEntity*)penToDamage))->SetFlagOff(ENF_ALIVE);
					}
				}
			}
		}
		else if(penToDamage)
		{
			if(((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0)
			{
				((CUnit*)((CEntity*)penToDamage))->DeathNow();
			}
		}
	};

	void HitTarget()
	{
		if ( m_iFireObjectType == CSkill::MT_DASH )
		{// ����Ʈ ��ų �뽬
			if ( m_epSource->GetNetworkType() == MSG_CHAR_PC && 
				m_epSource->GetNetworkID() == _pNetwork->MyCharacterInfo.index && !m_bMoveFail)
			{// �ڱ� �ڽ�
				CPlacement3D plSource = GetPlacement();
				// ��ų��� ��ƼƼ ��ġ ����
				m_epSource->SetPlacement(plSource);
				// ������ �߻� �޼��� ����
				_pNetwork->SendSkillMessage(401, m_epTarget, m_epTarget->GetNetworkID(), TRUE, 1 );
			}

			if (m_bMoveFail)
			{
				m_epSource->SetPlacement(GetPlacement());
			}

			FLOATmatrix3D matrix;
			MakeRotationMatrixFast(matrix, this->en_plPlacement.pl_OrientationAngle);
			FLOAT3D vDir(0,1,0);
			RotateVector(vDir, matrix);
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDir % vAxisY);
			FLOAT3D axis = vAxisY * vDir;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);
			StartEffectGroup(m_strHitEffect, _pTimer->GetLerpedCurrentTick(), this->en_plPlacement.pl_PositionVector, quat);
		}
		else if( (CEntity*)m_epTarget != NULL)
		{
			FLOATmatrix3D matrix;
			MakeRotationMatrixFast(matrix, this->en_plPlacement.pl_OrientationAngle);
			FLOAT3D vDir(0,1,0);
			RotateVector(vDir, matrix);
			this->InflictDirectDamage(m_epTarget, this, DMT_NONE, 1, this->en_plPlacement.pl_PositionVector, vDir);					
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDir % vAxisY);
			FLOAT3D axis = vAxisY * vDir;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);
			StartEffectGroup(m_strHitEffect, _pTimer->GetLerpedCurrentTick(), this->en_plPlacement.pl_PositionVector, quat);
		}
		else
		{
			FLOAT3D vAxisY(0,1,0);
			FLOAT3D vDir = (m_vInitPos - m_vTargetPos).SafeNormalize();
			FLOAT angle = acos(vDir % vAxisY);
			FLOAT3D axis = vAxisY * vDir;
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);
			StartEffectGroup(m_strHitEffect, _pTimer->GetLerpedCurrentTick(), this->en_plPlacement.pl_PositionVector, quat);
		}
	}
	FLOAT3D GetBoundingSphereInfo(CEntity *pen)
	{
		if( pen == NULL ) {return FLOAT3D(1,0,1);}
		
		if(pen->GetRenderType() == RT_SKAMODEL || pen->GetRenderType() == RT_SKAEDITORMODEL)
		{
			if(pen->en_pmiModelInstance != NULL)
			{
				FLOATaabbox3D bbox;
				pen->en_pmiModelInstance->GetCurrentColisionBox(bbox);
				pen->en_pmiModelInstance->mi_vStretch;
				FLOAT3D vLength = bbox.Max() - bbox.Min();
				vLength(2) = 0;
				FLOAT fStretchXY = Max(pen->en_pmiModelInstance->mi_vStretch(1), pen->en_pmiModelInstance->mi_vStretch(3));
				return FLOAT3D(vLength.Length() * 0.40f * fStretchXY
					, bbox.Min()(2) * pen->en_pmiModelInstance->mi_vStretch(2)
					, bbox.Max()(2) * pen->en_pmiModelInstance->mi_vStretch(2));
			}
		}
		else if(pen->GetRenderType() == RT_MODEL || pen->GetRenderType() == RT_EDITORMODEL)
		{
			if(pen->en_pmoModelObject != NULL)
			{
				FLOATaabbox3D bbox;
				pen->en_pmoModelObject->GetCurrentFrameBBox(bbox);
				FLOAT3D vLength = bbox.Max() - bbox.Min();
				vLength(2) = 0;
				return FLOAT3D(vLength.Length() * 0.40f, bbox.Min()(2), bbox.Max()(2));
				FLOAT fStretchXY = Max(pen->en_pmoModelObject->mo_Stretch(1), pen->en_pmoModelObject->mo_Stretch(3));
				return FLOAT3D(vLength.Length() * 0.40f * fStretchXY
					, bbox.Min()(2) * pen->en_pmoModelObject->mo_Stretch(2)
					, bbox.Max()(2) * pen->en_pmoModelObject->mo_Stretch(2));
			}
		}
		return FLOAT3D(1.0f, 0.0f, 1.0f);
	}
	void CalcPalacementOnPath(FLOAT curRatio, FLOAT oldRatio, FLOAT processedTime, CPlacement3D &newPlace)
	{
		FLOAT3D vCurrentPos = GetSpline().Value(curRatio);
		FLOAT3D vCurrentDir = vCurrentPos - GetSpline().Value(oldRatio);
		
		if ( m_iFireObjectType == CSkill::MT_DASH )
		{// ����Ʈ ��ų �뽬(�̵��� �Ÿ� ����)
			m_fLeftDistance += vCurrentDir.Length();

			FLOAT3D vSource = GetSpline().Value(oldRatio);
			FLOAT3D vTarget = vCurrentPos;
			CCastRay crRay( this, vSource, vTarget);
			crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
			crRay.cr_bHitTranslucentPortals = TRUE;
			crRay.cr_bPhysical = TRUE;
			GetWorld()->CastRay(crRay);
			if(crRay.cr_penHit != NULL)
			{
				m_bMoveFail = TRUE;
			}
		}

		ANGLE3D aCurrentRot;// = GetEulerAngleFromDir(vCurrentDir);
		if(m_aAngleSpeed != 0.0f)
		{
			FLOATquat3D quatCurrent;//, quatRot;
			//quatCurrent.FromEuler(aCurrentRot);
			quatCurrent.FromAxisAngle(vCurrentDir, RadAngle(m_aAngleSpeed * processedTime));
			//quatCurrent *= quatRot;
			aCurrentRot = GetEulerAngleFromQuaternion(quatCurrent);
		}
		else
		{
			aCurrentRot = GetEulerAngleFromDir(vCurrentDir);
		}

		newPlace.pl_OrientationAngle = aCurrentRot;
		newPlace.pl_PositionVector = vCurrentPos;
	}
	BOOL PathMove(FLOAT fDeltaTime)
	{
		//path spline �籸��
		RemakeSplinePath();

		CPlacement3D plNew;
		FLOAT fOldRatio = m_fRatio;
		m_fRatio += m_fTimeRatio * fDeltaTime;
		m_fTimeFromStart += fDeltaTime;
		CalcPalacementOnPath(m_fRatio, fOldRatio - m_fTimeRatio * 0.001f, m_fTimeFromStart, plNew);

		if ( m_iFireObjectType == CSkill::MT_DASH )
		{ // ����Ʈ ��ų �뽬
			/*
			CSkill &SkillData = _pNetwork->GetSkillData(401);
			
			if ( m_fLeftDistance > SkillData.GetFireRange2() )
			{// ���� �Ÿ���ŭ�� �̵�
				return TRUE;
			}
			*/
			if (m_bMoveFail)
			{
				return TRUE;
			}

			// ���� ���� �浹 ó��
			if ( m_fRatio >= 0.8f ) 
			{
				SetPlacement(plNew);
				return TRUE;
			}
		}

		SetPlacement(plNew);

		//�浹ó��
		if(m_fRatio >= 1.0f)	{ return TRUE; }
		else					{ return FALSE; }
	}
procedures:
	PreBehavior()
	{
		autowait(0.01f);
		if(!m_bInit)
		{
			m_fLastTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
			m_bInit = TRUE;
		}

		if(m_fBeforeDelay <= _pTimer->TickQuantum) {m_fBeforeDelay = _pTimer->TickQuantum * 0.01f;}
		autowait(m_fBeforeDelay);

		jump DoBehavior();

		return;
	}
	PostBehavior()
	{
		autowait(0.001f);

		//destroy self
		if(m_pMyEffectGroup != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pMyEffectGroup))
		{
			m_pMyEffectGroup->Stop(0);
		}
		else m_pMyEffectGroup = NULL;

		
		if(m_epTarget.ep_pen != NULL && m_epTarget->en_pmiModelInstance != NULL)
		{
			if(m_eMissileType == MLT_NORMALARROW
				&& ((CEnemy*)(CEntity*)m_epTarget)->m_nMobDBIndex != 82)//�̽�ƽ�� ȭ���� ������ ����.
			{
				//ȭ���� ������ ȿ�� ó��.
				CSkaTag *pSkaTag = NULL;
				INDEX temp = m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.GetTempValue();
				for(m_iIndex=0; m_iIndex<31; ++m_iIndex) {if( !(temp & (0x00000001 << m_iIndex)) ) {break;}}
				if(m_iIndex != 31)
				{
					m_strHitTag.PrintF("TEMP%x", 0x00000001 << m_iIndex);
					m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.SetTempValue(temp | (0x00000001 << m_iIndex));
					pSkaTag = (CSkaTag*)m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.Find(m_strHitTag.str_String);
					if(pSkaTag == NULL)
					{
						//test ray cast
/*
						const FLOAT3D vTargetBSInfo = GetBoundingSphereInfo(m_epTarget);
						const FLOAT3D vTarget = m_epTarget->en_plPlacement.pl_PositionVector
													+ FLOAT3D(0, (vTargetBSInfo(3) - vTargetBSInfo(2)) * HEIGHT_FACTOR, 0);
*/
						FLOAT3D vTarget = m_epTarget->en_plPlacement.pl_PositionVector;
						vTarget(2) += m_vTargetCenterPos(2);
						FLOAT3D vDist = vTarget - m_vInitPos;
						FLOAT dist = vDist.Length();
						INDEX iBoneID;
						MeshLOD *pmlod = NULL;
						pSkaTag = new CSkaTag;
						FLOAT fDist = RM_TestRayCastHit(
							*m_epTarget->en_pmiModelInstance
							, m_epTarget->en_mRotation
							, m_epTarget->en_plPlacement.pl_PositionVector
							, m_vInitPos
							, vTarget
							, dist
							, &iBoneID
							, pSkaTag );
						if(fDist == 1e6)
						{
							delete pSkaTag;
							pSkaTag = NULL;
						}
						else
						{
							pSkaTag->SetName(m_strHitTag.str_String);
							FLOATquat3D quatDir, quatEn, quatEnMi;
							quatEn.FromEuler(m_epTarget->en_plPlacement.pl_OrientationAngle);
							quatEnMi = quatEn.Inv() * m_epTarget->en_pmiModelInstance->mi_qvOffset.qRot.Inv();
							quatDir.FromEuler( GetEulerAngleFromDir(vDist.SafeNormalize()) );

							pSkaTag->SetOffsetRot( quatEnMi * quatDir );

							FLOAT3D pos = pSkaTag->GetOffsetPos() - m_epTarget->en_plPlacement.pl_PositionVector - m_epTarget->en_pmiModelInstance->mi_qvOffset.vPos;
							pos = VectorRotate(pos, quatEnMi);
							pSkaTag->SetOffsetPos( pos );
							//((CWorldTag*)pSkaTag)->AddNewTagInfo(0, FLOATquat3D(1,0,0,0), FLOAT3D(0,0,0), FLOAT3D(1,1,1));
							m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.RegisterNoCopy(pSkaTag);
						}
					}
				}
				if(pSkaTag != NULL)
				{
					CEffectGroup *pEG = StartEffectGroup(
						"Normal Arrow"
						, _pTimer->GetLerpedCurrentTick()
						, ptr_tag((CTag*)pSkaTag) );
					if(pEG) {pEG->Stop(m_fAfterDelay);}
					this->SetFlagOn(ENF_HIDDEN);
				}
			}
			else if(m_eMissileType == MLT_GOWAITBACK)
			{
			}
		}

		if(m_fAfterDelay <= _pTimer->TickQuantum) {m_fAfterDelay = _pTimer->TickQuantum * 0.01f;}
		autowait(m_fAfterDelay);

		if(m_eMissileType == MLT_GOWAITBACK && m_epSource != NULL && m_epSource->en_pmiModelInstance != NULL)
		{
			m_bReached = FALSE;
			m_bReached2 = FALSE;
			m_fRatio = 0.0f;
			m_fTimeFromStart = 0.0f;
			m_bNoPeak = TRUE;
			m_eMissileType = MLT_SOMETHING;
			m_fAfterDelay = 0.0f;
			m_strHitEffect += "\'";

			m_vInitPos = m_epTarget->GetPlacement().pl_PositionVector;
			FLOAT3D vDist = m_vInitPos - m_epSource->GetPlacement().pl_PositionVector;
			FLOAT fDist = vDist.Length();
			m_fTimeRatio = m_fMoveSpeed / fDist;
			GetSpline().Clear();
			GetSpline().AddSample(0.0f, m_vInitPos);
			GetSpline().AddSample(1.0f, m_epSource->GetPlacement().pl_PositionVector);
			m_epTarget = m_epSource;
			GetSpline().Prepare();
			RemakeSplinePath();
			CPlacement3D plNew;
			m_fRatio += m_fTimeRatio * 0.001f;
			CalcPalacementOnPath(m_fRatio, 0, 0.001f, plNew);
			SetPlacement(plNew);
			m_strMoveEffect += "\'";
			m_pMyEffectGroup = StartEffectGroup(m_strMoveEffect, &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			m_bInit = FALSE;
			jump PreBehavior();
		}

		this->SetFlagOn(ENF_HIDDEN);
		if(m_strHitTag != "" && m_epTarget->en_RenderType != RT_NONE && m_epTarget->en_pmiModelInstance)
		{
			m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.Unregister(m_strHitTag.str_String);
			m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.SetTempValue(
				m_epTarget->en_pmiModelInstance->m_tmSkaTagManager.GetTempValue() &
				(~(0x00000001 << m_iIndex)) );
		}

		if ( m_epSource != NULL )
		{
			if ( m_epSource->GetNetworkID() == _pNetwork->MyCharacterInfo.index )
			{// ����ڰ� �ڽ��̰�, INVISIBLE���°� ON�̸� OFF�Ѵ�.
				if ( m_epSource->IsFlagOn(ENF_INVISIBLE) )
				{
					m_epSource->SetFlagOff(ENF_INVISIBLE);
				}
			}
		}

		m_bInit = FALSE;
		SetFlagOff(ENF_ALIVE);
		SetFlagOn(ENF_CLIENTHANDLING);
		delete m_pssPath;
		m_pssPath = NULL;
		Destroy(FALSE);

		return;
	}
	DoBehavior()
	{
		while(!m_bReached)// && !m_bReached2)
		{
			autowait(0.01f);
			if(m_bEntityTarget && m_epTarget.ep_pen == NULL)
			{
				jump PostBehavior();
			}

			FLOAT fCurrent = _pTimer->GetHighPrecisionTimer().GetSeconds();
			m_fDeltaTime = fCurrent - m_fLastTime;
			m_fLastTime = fCurrent;

			if(!m_bReached)
			{
				m_bReached = PathMove(m_fDeltaTime);
			}
			//else
			//{
				//m_bReached2 = PathMove(m_fDeltaTime);
			//}
		}

		HitTarget();
		
		jump PostBehavior();

		return;
	}
	Connect()
	{
		autowait(0.1f);
		
		m_fLastTime = _pTimer->GetLerpedCurrentTick();
		StartEffectGroup(m_strHitEffect, &m_epTarget->GetModelInstance()->m_tmSkaTagManager, m_fLastTime);

		autowait(0.1f);
		while(_pTimer->GetLerpedCurrentTick() - m_fLastTime < m_fAfterDelay)
		{
			CPlacement3D plNew;
			FLOAT3D vSourcePos = m_epSource->GetPlacement().pl_PositionVector;
			plNew.pl_PositionVector = (vSourcePos + m_epTarget->GetPlacement().pl_PositionVector) * 0.5f;
			FLOAT3D vDir = vSourcePos - plNew.pl_PositionVector;
			FLOAT fLen = vDir.Length();
			vDir /= fLen;
			fLen *= 2;
			plNew.pl_OrientationAngle = GetEulerAngleFromDir(vDir);
			plNew.pl_PositionVector(2) += 1.0f;//�̰� ���ϸ� �ٴڿ� �پ ����Ʈ�� ����.
			SetPlacement(plNew);
			
			if(m_pMyEffectGroup)
			{
				for(INDEX i=0; i<m_pMyEffectGroup->GetEffectKeyVector().size(); ++i)
				{
					CEffect *pEffect = m_pMyEffectGroup->GetEffectKeyVector()[i].m_pCreatedEffect;
					if(pEffect != NULL && pEffect->GetType() == ET_MDL)
					{
						FLOAT3D vNewStretch = ((CMdlEffect*)pEffect)->GetStretch();
						vNewStretch(2) = fLen;
						((CMdlEffect*)pEffect)->SetStretch(vNewStretch);
					}
				}
			}

			autowait(0.1f);
		}

		if(m_pMyEffectGroup)
		{
			StopEffectGroupIfValid(m_pMyEffectGroup, 0.1f);
		}
		Destroy(FALSE);
		return;
	}

	Main()
	{
		if(m_bEntityTarget && m_epTarget.ep_pen == NULL)
		{
			InitAsVoid();
			SetFlagOff(ENF_ALIVE);
			SetFlagOn(ENF_CLIENTHANDLING);
			Destroy(FALSE);
			return;
		}
		InitAsSkaModel();
		m_soSound.Set3DParameters(30.0f, 5.0f, 1.0f, 1.0f);
		
		// try to load the model
		try {
			if(m_fnModel.GetName() != "") { SetSkaModel_t(m_fnModel);			}
			else
			{
				m_fnModel = CTFileName("data\\effect\\ska\\invisible.smc");
				SetSkaModel_t(m_fnModel);
			}	//Hardcoding
			if(m_eMissileType == MLT_NORMALARROW)
			{
				en_pmiModelInstance->mi_qvOffset.vPos = FLOAT3D(0,0,0);
				en_pmiModelInstance->mi_qvOffset.qRot = FLOATquat3D(1,0,0,0);
			}
			// if failed
		}
		catch(char *)
		{
			ASSERTALWAYS("Loading Fail.");
		}
		
		// set model stretch
		ModelChangeNotify();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);
		if(en_pmiModelInstance) {en_pmiModelInstance->SetCurrentColisionBoxIndex(0);}

		if(m_eMissileType == MLT_CONNECT)
		{
			//�ʱ�ȭ
			if(m_epSource.ep_pen == NULL)
			{
				SetFlagOff(ENF_ALIVE);
				SetFlagOn(ENF_CLIENTHANDLING);
				Destroy(FALSE);
				return;
			}
			m_fTimeFromStart = 0.0f;

			if(GetModelInstance())
			{
				CSkaTag tag;
				tag.SetName("__ROOT");
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			}

			jump Connect();
		}
		else
		{
			//�ʱ�ȭ

			if ( m_iFireObjectType == CSkill::MT_DASH )
			{ // ����Ʈ�� �뽬 ��ų���� ĳ���Ͱ� Invisible �ȴ�.
				if ( m_epSource->IsFlagOff(ENF_INVISIBLE) )
				{
					m_epSource->SetFlagOn(ENF_INVISIBLE);
				}
			}

			m_bReached = FALSE;
			m_bReached2 = FALSE;
			m_fRatio = 0.0f;
			m_fTimeFromStart = 0.0f;
			m_fLeftDistance = 0.0f;
			m_bMoveFail = FALSE;

			if(GetModelInstance())
			{
				CSkaTag tag;
				tag.SetName("__ROOT");
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			}

			//�ʱ� ���� ó��.
			m_fHorizonalOffset = Clamp(m_fHorizonalOffset, -1.0f, 1.0f);
			m_fVerticalOffset = Clamp(m_fVerticalOffset, -1.0f, 1.0f);
			m_vInitPos = en_plPlacement.pl_PositionVector;
			if(m_bEntityTarget)
			{
				CTag *pTag = m_epTarget->GetModelInstance()->m_tmSkaTagManager.Find("CENTER");
				if(pTag)
				{
					m_vTargetCenterPos = pTag->GetOffsetPos();
					RotateVector(m_vTargetCenterPos, pTag->GetOffsetRot());
				}
				else
				{
					m_vTargetCenterPos = m_epTarget->en_plPlacement.pl_PositionVector;
				}
			}
			
			//�ʱ� �˵� ó��.
			FLOAT3D vDist;
			if(m_bEntityTarget)
			{
				vDist = m_epTarget->en_plPlacement.pl_PositionVector - this->en_plPlacement.pl_PositionVector;
			}
			else
			{
				vDist = m_vTargetPos - this->en_plPlacement.pl_PositionVector;
			}
			FLOAT fDist = vDist.Length();
			m_fTimeRatio = m_fMoveSpeed / fDist;//1.0f / (fDist / m_fMoveSpeed);
			static FLOAT fNoPeakLimit = 3.0f;
			if(!m_bNoPeak) { m_bNoPeak = fDist < fNoPeakLimit; }
			m_pssPath = new cubic_spline;
			GetSpline().AddSample(0.0f, m_vInitPos);
			if(!m_bNoPeak)
			{
				vDist *= 0.3f;
				static FLOAT fDistRatio = 0.10f;
				vDist(2) += fDist * fDistRatio * (m_fVerticalOffset * 0.5f + 0.5f);
				if(m_fHorizonalOffset != 0.0f)
				{
					FLOATquat3D qRot;
					static FLOAT fAngle = 15.0f;
					qRot.FromEuler(ANGLE3D(fAngle * m_fHorizonalOffset,0,0));
					vDist = VectorRotate(vDist, qRot);
				}
				GetSpline().AddSample(0.4f, vDist + this->en_plPlacement.pl_PositionVector);
			}
			if(m_bEntityTarget)
			{
				RemakeSplinePath();
			}
			else
			{
				GetSpline().AddSample(1.0f, m_vTargetPos);
				GetSpline().Prepare();
			}

			CPlacement3D plNew;
			m_fRatio += m_fTimeRatio * 0.001f;
			CalcPalacementOnPath(m_fRatio, 0, 0.001f, plNew);
			SetPlacement(plNew);
			
			jump PreBehavior();
		}
		
		delete m_pssPath;
		m_pssPath = NULL;
		Destroy(FALSE);
		return;
	}
};
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//Temp (Only for Test) ��
