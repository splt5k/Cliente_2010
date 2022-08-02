242
%{
	#include "StdH.h"
	#include "EntitiesMP/WorldSettingsController.h"
	//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	#include <engine/Effect/CEffectGroup.h>
	#include <engine/Effect/CEffectGroupManager.h>
	#include <Engine/Effect/CSkaTag.h>
	//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
%}

uses "EntitiesMP/ModelDestruction";
uses "EntitiesMP/AnimationChanger";
uses "EntitiesMP/BloodSpray";

// network model activate/deactivate
event EModel3Activate {
};
event EModel3Deactivate {
};

%{
	// #define MIPRATIO 0.003125f //(2*tan(90/2))/640
%}

class CModelHolder3 : CRationalEntity {
	name      "ModelHolder3";
	thumbnail "Thumbnails\\ModelHolder3.tbn";
	features "HasName", "HasDescription";

properties:
	1 CTFileName m_fnModel      "Model file (.smc)" 'M' = CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"), //("Data\\Models\\Editor\\Axis.mdl"),
		2 CTFileName m_fnTexture    "Texture" 'T' = CTFILENAME("Data\\Models\\Editor\\Vector.tex"),
		// 22 CTFileName m_fnReflection "Reflection" =CTString(""),
		// 23 CTFileName m_fnSpecular   "Specular" =CTString(""),
		// 24 CTFileName m_fnBump       "Bump" =CTString(""),
		3 FLOAT m_fStretchAll       "StretchAll" 'S' = 1.0f,
		4 ANGLE3D m_vStretchXYZ     "StretchXYZ" 'X' = FLOAT3D(1.0f, 1.0f, 1.0f),
		//5 FLOAT m_fStretchY         "StretchY"   'Y' = 1.0f,
		//6 FLOAT m_fStretchZ         "StretchZ"   'Z' = 1.0f,
		7 CTString m_strName        "Name" 'N' = "",
		12 CTString m_strDescription = "",
		8 BOOL m_bColliding       "Collision" 'L' = FALSE,    // set if model is not immatierial
		9 CTString m_strModelAnimation   "Model animation (SKA)" = "",
		// 10 ANIMATION m_iTextureAnimation "Texture animation" = 0,
		11 enum ShadowType m_stClusterShadows "Shadows" 'W' = ST_CLUSTER,   // set if model uses cluster shadows
		13 BOOL m_bBackground     "Background" 'B' = FALSE,   // set if model is rendered in background
		21 BOOL m_bTargetable     "Targetable" = FALSE, // st if model should be targetable

		// parameters for custom shading of a model (overrides automatic shading calculation)
		14 enum CustomShadingType m_cstCustomShading "Shading mode" 'H' = CST_NONE,
		15 ANGLE3D m_aShadingDirection "Shade. Light direction" 'D' = ANGLE3D(AngleDeg(45.0f), AngleDeg(45.0f), AngleDeg(45.0f)),
		16 COLOR m_colLight            "Shade. Light color" 'O' = C_WHITE,
		17 COLOR m_colAmbient          "Shade. Ambient color" 'A' = C_BLACK,
		18 FLOAT3D m_vShadingDirection = FLOAT3D(0, 1, 0),
		// 18 CTFileName m_fnmLightAnimation "Light animation file" = CTString(""),
		// 19 ANIMATION m_iLightAnimation "Light animation" = 0,
		// 20 CAnimObject m_aoLightAnimation,
		// 25 BOOL m_bAttachments      "Attachments" = TRUE, // set if model should auto load attachments*/
		26 BOOL m_bActive "Active" = TRUE features(EPROPF_NETSEND),
		28 COLOR m_colModelColor		"Model Color" = 0xFFFFFFFF,
		// 31 FLOAT m_fMipAdd "Mip Add" = 0.0f,
		// 32 FLOAT m_fMipMul "Mip Mul" = 1.0f,
		//33 FLOAT m_fMipFadeDist "Mip Fade Dist" = 0.0f,
		//34 FLOAT m_fMipFadeLen  "Mip Fade Len" = 0.0f,
		// 33 FLOAT m_fMipFadeDist = 0.0f,
		// 34 FLOAT m_fMipFadeLen  = 0.0f,
		35 RANGE m_rMipFadeDistMetric "Mip Fade Dist (Metric)" = -1.0f,
		36 FLOAT m_fMipFadeLenMetric  "Mip Fade Len (Metric)" = -1.0f,

		// random values variables
		50 BOOL m_bRandomStretch   "Apply RND stretch" = FALSE, // apply random stretch
		52 ANGLE3D m_fStretchRndXYZ    "Stretch RND XYZ (%)" = FLOAT3D(0.2f, 0.2f, 0.2f), // random stretch width
		//51 FLOAT m_fStretchRndY    "Stretch RND Y (%)"   =  0.2f, // random stretch height
		//53 FLOAT m_fStretchRndZ    "Stretch RND Z (%)"   =  0.2f, // random stretch depth
		54 FLOAT m_fStretchRndAll  "Stretch RND All (%)" = 0.0f, // random stretch all
		55 FLOAT3D m_fStretchRandom = FLOAT3D(1, 1, 1),

		// destruction values
		60 CEntityPointer m_penDestruction "Destruction" 'Q' COLOR(C_BLACK | 0x20),    // model destruction entity
		61 FLOAT3D m_vDamage = FLOAT3D(0, 0, 0),    // current damage impact
		62 FLOAT m_tmLastDamage = -1000.0f,
		63 CEntityPointer m_penDestroyTarget "Destruction Target" COLOR(C_WHITE | 0xFF), // targeted when destroyed
		64 CEntityPointer m_penLastDamager,
		65 FLOAT m_tmSpraySpawned = 0.0f,   // time when damage has been applied
		66 FLOAT m_fSprayDamage = 0.0f,     // total ammount of damage
		67 CEntityPointer m_penSpray,       // the blood spray
		68 FLOAT m_fMaxDamageAmmount = 0.0f, // max ammount of damage recived in in last xxx ticks

		70 FLOAT m_fClassificationStretch  "Classification stretch" = 1.0f, // classification box multiplier
		80 COLOR m_colBurning = COLOR(C_WHITE | CT_OPAQUE), // color applied when burning

		90 enum DamageType m_dmtLastDamageType = DMT_CHAINSAW,
		91 FLOAT m_fChainSawCutDamage    "Chain saw cut dammage" 'C' = 300.0f,
		//93 INDEX m_iFirstRandomAnimation "First random animation" 'R' = -1,
		100 FLOAT m_fMaxTessellationLevel "Max tessellation level" = 0.0f,

		110 BOOL m_bDoNotLoopAnimation "Animation looping OFF" = FALSE,
		//115 BOOL m_bEnemy "Is Enemy?" = FALSE,
		116 BOOL	m_bRenderReflection	"Render reflection?" = FALSE,
		117 BOOL		m_bTerrainShadow "Terrain Shadow?" = FALSE,
		118 CTString	m_strEffectGroupNameEvent "Model Effect Name(Event)" = "",
		119 BOOL		m_bDecorationObject "Decoration Object" = FALSE,
		120 BOOL		m_bHasRootTag	"Has __ROOT & __TOP tag" = FALSE,
		121 CTString	m_strEffectGroupName "Model Effect Name" = "",
	{
		CEffectGroup * m_pModelEffectGroupEvent;
		CEffectGroup* m_pModelEffectGroup;
	}

		/*{
			CTFileName m_fnOldModel;  // used for remembering last selected model (not saved at all)
		}*/

	components:
	1 class    CLASS_BLOOD_SPRAY "Classes\\BloodSpray.ecl",
		20 skamodel EDITOR_AXIS_MODEL "Data\\Models\\Editor\\Ska\\Axis.smc", // just used for safety precaching

		functions :
		void Precache(void) {
		PrecacheSkaModel(EDITOR_AXIS_MODEL);
		PrecacheClass(CLASS_BLOOD_SPRAY, 0);
		if (en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
		if (en_pmiModelInstance->mi_pmisSerial
			&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance
			&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile != "")
		{
			en_pmiModelInstance->ReadAnimEffectFile(en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile);
			//en_pmiModelInstance->m_vectorAnimEffect = ((CModelInstance*)en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance)->m_vectorAnimEffect;
		}
		GetModelInstance()->RefreshTagManager();
		if (m_bHasRootTag && GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
		if (m_strModelAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) {
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				if (m_bDoNotLoopAnimation) {
					ulFlags = 0;
				}
				AddAnimation(iAnim, ulFlags, 1, 1);
				OffSetAnimationQueue(FRnd() * 10.0f);
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strModelAnimation, GetName());
			}
		}
		if (GetModelInstance() && m_strEffectGroupName != "")
		{
			if (CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroup))
			{
				m_pModelEffectGroup->Stop(2); // WSS_DRATAN_SEIGEWARFARE 2007/08/31 ����
			}
			m_pModelEffectGroup = StartEffectGroup(m_strEffectGroupName
				, &GetModelInstance()->m_tmSkaTagManager
				, _pTimer->GetLerpedCurrentTick() + rand() / RAND_MAX);
		}
		else
		{
			m_pModelEffectGroup = NULL;
		}
	};
	/*
		void Read_t(CTStream *istr, BOOL bNetwork)
		{
			CRationalEntity::Read_t(istr,bNetwork);
			if(m_bHasRootTag && GetModelInstance())
			{
				CSkaTag tag;
				tag.SetName("__ROOT");
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
				tag.SetName("__TOP");
				FLOATaabbox3D aabb;
				GetModelInstance()->GetAllFramesBBox(aabb);
				tag.SetOffsetPos(0, aabb.Size()(2), 0);
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			}
			if( m_strModelAnimation != "")
			{
				INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
				INDEX iDummy1, iDummy2;
				if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) {
					NewClearState(0.05f);
					ULONG ulFlags = AN_LOOPING;
					if (m_bDoNotLoopAnimation) {
						ulFlags = 0;
					}
					AddAnimation(iAnim, ulFlags, 1, 1);
					OffSetAnimationQueue(FRnd()*10.0f);
				} else {
					CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
						m_strModelAnimation, GetName());
				}
			}
		}
	*/

	/* Fill in entity statistics - for AI purposes only */
	BOOL FillEntityStatistics(EntityStats* pes)
	{
		//pes->es_strName = m_fnModel.FileName()+", "+m_fnTexture.FileName();
		pes->es_strName = m_fnModel.FileName();

		pes->es_ctCount = 1;
		pes->es_ctAmmount = 1;
		if (m_penDestruction != NULL) {
			pes->es_strName += " (destroyable)";
			pes->es_fValue = GetDestruction()->m_fHealth;
			//			pes->es_iScore = 0;
		} else {
			pes->es_fValue = 0;
			//			pes->es_iScore = 0;
		}
		return TRUE;
	}

	// classification box multiplier
	FLOAT3D GetClassificationBoxStretch(void)
	{
		return FLOAT3D(m_fClassificationStretch, m_fClassificationStretch, m_fClassificationStretch);
	}


	// maximum allowed tessellation level for this model (for Truform/N-Patches support)
	FLOAT GetMaxTessellationLevel(void)
	{
		return m_fMaxTessellationLevel;
	}


	/* Receive damage */
	void ReceiveDamage(CEntity* penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D& vHitPoint, const FLOAT3D& vDirection)
	{
		FLOAT fNewDamage = fDamageAmmount;

		// if not destroyable
		if (m_penDestruction == NULL) {
			// do nothing
			return;
		}
		if (dmtType == DMT_BURNING)
		{
			UBYTE ubR, ubG, ubB, ubA;
			ColorToRGBA(m_colBurning, ubR, ubG, ubB, ubA);
			ubR = ClampDn(ubR - 4, 32);
			m_colBurning = RGBAToColor(ubR, ubR, ubR, ubA);
		}

		CModelDestruction* penDestruction = GetDestruction();
		// adjust damage
		fNewDamage *= DamageStrength(penDestruction->m_eibtBodyType, dmtType);
		// if no damage
		if (fNewDamage == 0) {
			// do nothing
			return;
		}
		FLOAT fKickDamage = fNewDamage;
		if ((dmtType == DMT_EXPLOSION) || (dmtType == DMT_IMPACT)/* || (dmtType == DMT_CANNONBALL_EXPLOSION) */)
		{
			fKickDamage *= 1.5f;
		}
		if (dmtType == DMT_CLOSERANGE) {
			fKickDamage = 0.0f;
		}
		if (dmtType == DMT_CHAINSAW) {
			fKickDamage = 0.0f;
		}
		if ((dmtType == DMT_BULLET || dmtType == DMT_SNIPER || dmtType == DMT_SHOTGUN)
			&& penDestruction->m_eibtBodyType == EIBT_ROCK) {
			fKickDamage = 0.0f;
		}
		if (dmtType == DMT_BURNING)
		{
			fKickDamage = 0.0f;
		}

		// get passed time since last damage
		TIME tmNow = _pTimer->CurrentTick();
		TIME tmDelta = tmNow - m_tmLastDamage;
		m_tmLastDamage = tmNow;

		// remember who damaged you
		m_penLastDamager = penInflictor;

		// fade damage out
		if (tmDelta >= _pTimer->TickQuantum * 3) {
			m_vDamage = FLOAT3D(0, 0, 0);
		}
		// add new damage
		FLOAT3D vDirectionFixed;
		if (vDirection.ManhattanNorm() > 0.5f) {
			vDirectionFixed = vDirection;
		} else {
			vDirectionFixed = FLOAT3D(0, 1, 0);
		}
		FLOAT3D vDamageOld = m_vDamage;
		m_vDamage += vDirectionFixed * fKickDamage;

		// NOTE: we don't receive damage here, but handle death differently
		if (m_vDamage.Length() > GetHealth()) {
			if (!penDestruction->m_bRequireExplosion ||
				dmtType == DMT_EXPLOSION/* || dmtType==DMT_CANNONBALL || dmtType==DMT_CANNONBALL_EXPLOSION*/)
			{
				EDeath eDeath;  // we don't need any extra parameters
				SendEvent(eDeath);
				//remember last dammage type
				m_dmtLastDamageType = dmtType;
			}
		}

		if (m_fMaxDamageAmmount < fDamageAmmount) {
			m_fMaxDamageAmmount = fDamageAmmount;
		}

		// if it has no spray, or if this damage overflows it
		if ((dmtType != DMT_BURNING) && (dmtType != DMT_CHAINSAW) &&
			(m_tmSpraySpawned <= _pTimer->CurrentTick() - _pTimer->TickQuantum * 8 ||
				m_fSprayDamage + fNewDamage > 50.0f))
		{
			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D(vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity(plSpray, CLASS_BLOOD_SPRAY);
			m_penSpray->SetParent(this);
			ESpawnSpray eSpawnSpray;

			// adjust spray power
			if (m_fMaxDamageAmmount > 10.0f) {
				eSpawnSpray.fDamagePower = 3.0f;
			} else if(m_fSprayDamage+fNewDamage>50.0f) {
				eSpawnSpray.fDamagePower = 2.0f;
			} else {
				eSpawnSpray.fDamagePower = 1.0f;
			}

			eSpawnSpray.sptType = penDestruction->m_sptType;
			eSpawnSpray.fSizeMultiplier = penDestruction->m_fParticleSize;

			// get your down vector (simulates gravity)
			FLOAT3D vDn(-en_mRotation(1, 2), -en_mRotation(2, 2), -en_mRotation(3, 2));

			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent(vHitPointRelative, vDn, vReflectingNormal);
			vReflectingNormal.Normalize();

			FLOAT3D vProjectedComponent = vReflectingNormal * (vDirection % vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection - vProjectedComponent * 2.0f - vDn * 0.5f;

			eSpawnSpray.vDirection = vSpilDirection;
			eSpawnSpray.eidOwner = this;
			eSpawnSpray.colCentralColor = penDestruction->m_colParticles;
			eSpawnSpray.colBurnColor = m_colBurning;
			eSpawnSpray.fLaunchPower = penDestruction->m_fParticleLaunchPower;
			// initialize spray
			m_penSpray->Initialize(eSpawnSpray);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}

		if (dmtType == DMT_CHAINSAW && m_fChainSawCutDamage > 0)
		{
			m_fChainSawCutDamage -= fDamageAmmount;
			if (m_fChainSawCutDamage <= 0)
			{
				EDeath eDeath;  // we don't need any extra parameters
				SendEvent(eDeath);
				//remember last dammage type
				m_dmtLastDamageType = dmtType;
			}
		}

		m_fSprayDamage += fNewDamage;
	};

	// Entity info
	void* GetEntityInfo(void) {
		CModelDestruction* pmd = GetDestruction();
		if (pmd != NULL)
		{
			return GetStdEntityInfo(pmd->m_eibtBodyType);
		}
		return CEntity::GetEntityInfo();
	};

	class CModelDestruction* GetDestruction(void)
	{
		ASSERT(m_penDestruction == NULL || IsOfClass(m_penDestruction, &CModelDestruction_DLLClass));
		return (CModelDestruction*)&*m_penDestruction;
	}
	BOOL IsTargetable(void) const
	{
		return m_bTargetable;
	}

	/* Adjust model mip factor if needed. */
	void AdjustMipFactor(FLOAT& fMipFactor)
	{
		// if should fade last mip
		//if (m_rMipFadeDistMetric>0) {
		// this was changed due to an error in conversion of mipfactors <=0.0f
		// if it causes any problems, return to previous value
		if (m_rMipFadeDistMetric > 0.5f) {
			CModelInstance* pmi = GetModelInstance();
			if (pmi == NULL) {
				return;
			}

			ENGINE_API extern FLOAT ska_fLODMul;
			ENGINE_API extern FLOAT ska_fLODAdd;

			// adjust for stretch
			FLOAT fMipDistNow = fMipFactor;
			FLOAT fMipFadeDist = m_rMipFadeDistMetric * ska_fLODMul + ska_fLODAdd;

			// if not visible
			if (fMipDistNow > fMipFadeDist) {
				// set mip factor so that model is never rendered
				fMipFactor = UpperLimit(0.0f);
				return;
			}

			// adjust fading
			FLOAT fFade = (fMipFadeDist - fMipDistNow);
			if (m_fMipFadeLenMetric > 0) {
				fFade /= m_fMipFadeLenMetric;
			} else {
				if (fFade > 0) {
					fFade = 1.0f;
				}
			}

			fFade = Clamp(fFade, 0.0f, 1.0f);
			// make it invisible
			pmi->SetModelColor((pmi->GetModelColor() & ~255) | UBYTE(255 * fFade));
		}
	}

	/* Adjust model shading parameters if needed. */
	BOOL AdjustShadingParameters(FLOAT3D& vLightDirection, COLOR& colLight, COLOR& colAmbient)
	{
		switch (m_cstCustomShading)
		{
		case CST_FULL_CUSTOMIZED:
		{
			colLight = m_colLight;
			colAmbient = m_colAmbient;
			// obtain world settings controller
			CWorldSettingsController* pwsc = GetWSC(this);
			if (pwsc != NULL && pwsc->m_bApplyShadingToModels) {
				// apply animating shading
				COLOR colShade = GetWorld()->wo_atbTextureBlendings[9].tb_colMultiply;
				colLight = MulColors(colLight, colShade);
				colAmbient = MulColors(colAmbient, colShade);
			}
			vLightDirection = m_vShadingDirection;
			SetFlagOn(ENF_CUSTOMSHADING); // signal to engine
			break;
		}
		case CST_CONSTANT_SHADING:
		{
			// combine colors with clamp
			UBYTE lR, lG, lB, aR, aG, aB, rR, rG, rB;
			ColorToRGB(colLight, lR, lG, lB);
			ColorToRGB(colAmbient, aR, aG, aB);
			colLight = 0;
			rR = (UBYTE)Clamp((ULONG)lR + aR, (ULONG)0, (ULONG)255);
			rG = (UBYTE)Clamp((ULONG)lG + aG, (ULONG)0, (ULONG)255);
			rB = (UBYTE)Clamp((ULONG)lB + aB, (ULONG)0, (ULONG)255);
			colAmbient = RGBToColor(rR, rG, rB);
			SetFlagOff(ENF_CUSTOMSHADING); // signal to engine
			break;
		}
		case CST_NONE:
		{
			SetFlagOff(ENF_CUSTOMSHADING); // signal to engine
			break;
		}
		}

		if (m_colBurning != COLOR(C_WHITE | CT_OPAQUE)) {
			colAmbient = MulColors(colAmbient, m_colBurning);
			colLight = MulColors(colLight, m_colBurning);
			return TRUE;
		}
		// done
		return m_stClusterShadows != ST_NONE;
	};


	// apply mirror and stretch to the entity
	void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
	{
		m_fStretchAll *= fStretch;
		if (bMirrorX) {
			m_vStretchXYZ(1) = -m_vStretchXYZ(1);
		}
	}


	// Stretch model
	void StretchModel(void) {
		// stretch factors must not have extreme values
		if (Abs(m_vStretchXYZ(1)) < 0.01f) { m_vStretchXYZ(1) = 0.01f; }
		if (Abs(m_vStretchXYZ(2)) < 0.01f) { m_vStretchXYZ(2) = 0.01f; }
		if (Abs(m_vStretchXYZ(3)) < 0.01f) { m_vStretchXYZ(3) = 0.01f; }
		if (m_fStretchAll < 0.01f) { m_fStretchAll = 0.01f; }

		if (Abs(m_vStretchXYZ(1)) > 1000.0f) { m_vStretchXYZ(1) = 1000.0f * Sgn(m_vStretchXYZ(1)); }
		if (Abs(m_vStretchXYZ(2)) > 1000.0f) { m_vStretchXYZ(2) = 1000.0f * Sgn(m_vStretchXYZ(2)); }
		if (Abs(m_vStretchXYZ(3)) > 1000.0f) { m_vStretchXYZ(3) = 1000.0f * Sgn(m_vStretchXYZ(3)); }
		if (m_fStretchAll > 1000.0f) { m_fStretchAll = 1000.0f; }

		if (m_bRandomStretch) {
			m_bRandomStretch = FALSE;
			// stretch
			m_fStretchRndXYZ = FLOAT3D(Clamp(m_fStretchRndXYZ(1), 0.0f, 1.0f),
				Clamp(m_fStretchRndXYZ(2), 0.0f, 1.0f),
				Clamp(m_fStretchRndXYZ(3), 0.0f, 1.0f));
			m_fStretchRndAll = Clamp(m_fStretchRndAll, 0.0f, 1.0f);

			m_fStretchRandom(1) = (FRnd() * m_fStretchRndXYZ(1) * 2 - m_fStretchRndXYZ(1)) + 1;
			m_fStretchRandom(2) = (FRnd() * m_fStretchRndXYZ(2) * 2 - m_fStretchRndXYZ(2)) + 1;
			m_fStretchRandom(3) = (FRnd() * m_fStretchRndXYZ(3) * 2 - m_fStretchRndXYZ(3)) + 1;

			FLOAT fRNDAll = (FRnd() * m_fStretchRndAll * 2 - m_fStretchRndAll) + 1;
			m_fStretchRandom(1) *= fRNDAll;
			m_fStretchRandom(2) *= fRNDAll;
			m_fStretchRandom(3) *= fRNDAll;
		}
		//������ ���� ����	//(Easy Use World Editor)(0.1)
		FLOAT3D vStretch;
		vStretch(1) = m_vStretchXYZ(1) * m_fStretchAll * m_fStretchRandom(1);
		vStretch(2) = m_vStretchXYZ(2) * m_fStretchAll * m_fStretchRandom(2);
		vStretch(3) = m_vStretchXYZ(3) * m_fStretchAll * m_fStretchRandom(3);

		GetModelInstance()->StretchModel(vStretch);
		//������ ���� ��	//(Easy Use World Editor)(0.1)
		ModelChangeNotify();
	};


	/* Init model holder*/
	void InitModelHolder(void) {

		// must not crash when model is removed
		if (m_fnModel == "") {
			m_fnModel = CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc");
		}

		if (m_bActive) {
			InitAsSkaModel();
		} else {
			InitAsSkaEditorModel();
		}

		BOOL bLoadOK = TRUE;
		// try to load the model
		try {
			SetSkaModel_t(m_fnModel);
			// if failed
		} catch(char *) {            
			bLoadOK = FALSE;
			// set colision info for default model
			//SetSkaColisionInfo();
		}
		if (!bLoadOK) {
			SetSkaModel(CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"));
			WarningMessage(TRANS("Cannot load ska model '%s'"), (CTString&)m_fnModel);
		}

		// set model stretch
		StretchModel();
		ModelChangeNotify();

		if (m_bColliding && m_bActive) {
			SetPhysicsFlags(EPF_MODEL_FIXED);
			SetCollisionFlags(ECF_MODEL_HOLDER);
		} else {
			SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
			SetCollisionFlags(ECF_IMMATERIAL);
		}

		switch (m_stClusterShadows) {
		case ST_NONE:
		{
			SetFlags(GetFlags() & ~ENF_CLUSTERSHADOWS);
			//������ ���� ����
			SetFlags(GetFlags() & ~ENF_PROJECTIONSHADOWS);
			//������ ���� ��
			//SetFlags(GetFlags()&~ENF_POLYGONALSHADOWS);
			break;
		}
		case ST_CLUSTER:
		{
			SetFlags(GetFlags() | ENF_CLUSTERSHADOWS);
			//������ ���� ����
			SetFlags(GetFlags() & ~ENF_PROJECTIONSHADOWS);
			//������ ���� ��
			//SetFlags(GetFlags()&~ENF_POLYGONALSHADOWS);
			break;
		}
		case ST_POLYGONAL:
		{
			//SetFlags(GetFlags()|ENF_POLYGONALSHADOWS);
			//������ ���� ����
			SetFlags(GetFlags() & ~ENF_PROJECTIONSHADOWS);
			//������ ���� ��
			SetFlags(GetFlags() & ~ENF_CLUSTERSHADOWS);
			break;
		}
		//������ ���� ����
		case ST_PROJECTION:
		{
			//������ ���� ����	//(Add Sun Moon Entity and etc)(0.2)
			GetModelInstance()->mi_bRenderShadow = TRUE;
			//������ ���� ��	//(Add Sun Moon Entity and etc)(0.2)
			SetFlags(GetFlags() | ENF_PROJECTIONSHADOWS);
			break;
		}
		//������ ���� ��
		}

		if (m_bBackground) {
			SetFlags(GetFlags() | ENF_BACKGROUND);
		} else {
			SetFlags(GetFlags() & ~ENF_BACKGROUND);
		}

		if (m_penDestruction == NULL) {
			m_strDescription.PrintF("%s undestroyable", (CTString&)m_fnModel.FileName());
		} else {
			m_strDescription.PrintF("%s -> %s", (CTString&)m_fnModel.FileName(), m_penDestruction->GetName());
		}

		m_strDescription.PrintF("%s", (CTString&)m_fnModel.FileName());

		AnglesToDirectionVector(m_aShadingDirection, m_vShadingDirection);
		m_vShadingDirection = -m_vShadingDirection;

		return;
	};
	//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	void SetModelColor(const COLOR color)
	{
		GetModelInstance()->SetModelColor(color);
	}
	//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)

procedures:
	Main()
	{
		//������ ���� ���� ���� ������ �۾�	05.20
		/*
				if(m_bEnemy)
				{
					SetFlagOn(ENF_ENEMY);
					m_bActive = FALSE;
				}
				*/
				//������ ���� �� ���� ������ �۾�	05.20
		SetFlagOn(ENF_MARKDESTROY);
		SetFlagOn(ENF_NONETCONNECT);
		SetFlagOff(ENF_PROPSCHANGED);
		//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.30
		SetFlagOff(ENF_RENDERREFLECTION);
		//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.30
		//������ ���� ���� �α��� ó�� �۾�	07.20
		SetFirstExtraFlagOff(ENF_EX1_TERRAINSHADOW);
		//������ ���� �� �α��� ó�� �۾�		07.20

		// initialize the model
		InitModelHolder();

		//������ ���� ����	//(For Performance)(0.1)
		SetFirstExtraFlagOff(ENF_EX1_DECORATION);
		if (m_bDecorationObject) { SetFirstExtraFlagOn(ENF_EX1_DECORATION); }
		//������ ���� ��	//(For Performance)(0.1)

		//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.30
		if (m_bRenderReflection)
		{
			SetFlagOn(ENF_RENDERREFLECTION);
		}
		//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.30

		//������ ���� ���� �α��� ó�� �۾�	07.20
		if (m_bTerrainShadow)
		{
			SetFirstExtraFlagOn(ENF_EX1_TERRAINSHADOW);
		}
		//������ ���� �� �α��� ó�� �۾�		07.20

		//������ ���� ����	//(Add Sun Moon Entity and etc)(0.2)
		GetModelInstance()->SetModelColor(m_colModelColor);
		//������ ���� ��	//(Add Sun Moon Entity and etc)(0.2)

		//������ ���� ����	//(Open beta)(2004-12-13)
		if (m_bHasRootTag && GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
		//������ ���� ��	//(Open beta)(2004-12-13)

				// check your destruction pointer
		if (m_penDestruction != NULL && !IsOfClass(m_penDestruction, &CModelDestruction_DLLClass)) {
			WarningMessage("Destruction '%s' is wrong class!", m_penDestruction->GetName());
			m_penDestruction = NULL;
		}

		// warning! because of this auto wait domino effect destruction with chainsaw didn't work
		//autowait(0.05f);

		// start ska animations if needed
		if (m_strModelAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) {
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				if (m_bDoNotLoopAnimation) {
					ulFlags = 0;
				}
				AddAnimation(iAnim, ulFlags, 1, 1);
				OffSetAnimationQueue(FRnd() * 10.0f);
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strModelAnimation, GetName());
			}
		}

		// [080201: Su-won] �ٸ� ����Ʈ�� �����ϸ� ���ο� ����Ʈ�� �����ϱ� ���� ����.
		if (m_pModelEffectGroup)
		{
			DestroyEffectGroup(m_pModelEffectGroup);
		}

		if (GetModelInstance() && m_strEffectGroupName != "")
		{
			if (CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroup))
			{
				m_pModelEffectGroup->Stop(0);
			}
			m_pModelEffectGroup = StartEffectGroup(m_strEffectGroupName
				, &GetModelInstance()->m_tmSkaTagManager
				, _pTimer->GetLerpedCurrentTick() + rand() / RAND_MAX);
		}
		else
		{
			m_pModelEffectGroup = NULL;
		}
		
		// wait forever
		wait() {
			// on the beginning
			on(EBegin) : {
				// set your health
				if (m_penDestruction != NULL) {
					SetHealth(GetDestruction()->m_fHealth);
				}
				resume;
			}
			//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
			on(EStart) :
			{
				if (m_strEffectGroupNameEvent != "" && en_pmiModelInstance != NULL)
				{
					DestroyEffectGroup(m_pModelEffectGroupEvent);
					m_pModelEffectGroupEvent = StartEffectGroup(m_strEffectGroupNameEvent, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				resume;
			}
			on(EStop) :
			{
				if (m_strEffectGroupNameEvent != "" && en_pmiModelInstance != NULL)
				{
					if (CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroupEvent))
					{
						m_pModelEffectGroupEvent->Stop(0); // WSS_DRATAN_SEIGEWARFARE 2007/08/30 �ٷ� ������� ó�� 10 -> 0
						m_pModelEffectGroupEvent = NULL;
					}
				}
				resume;
			}
			//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
						// activate/deactivate shows/hides model
			on(EActivate) : {
				if (_pNetwork->IsServer()) {
					SendEvent(EModel3Activate(), TRUE);
				}
				resume;
			}
			on(EModel3Activate) : {
				SetFlagOn(ENF_PROPSCHANGED);
				SwitchToModel();
				m_bActive = TRUE;
				if (m_bColliding) {
					SetPhysicsFlags(EPF_MODEL_FIXED);
					SetCollisionFlags(ECF_MODEL_HOLDER);
				}
				resume;
			}
			on(EDeactivate) : {
				if (_pNetwork->IsServer()) {
					SendEvent(EModel3Deactivate(), TRUE);
				}
				resume;
			}
			on(EModel3Deactivate) : {
				SetFlagOn(ENF_PROPSCHANGED);
				SwitchToEditorModel();
				SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
				SetCollisionFlags(ECF_IMMATERIAL);
				m_bActive = FALSE;
				resume;
			}
			// when your parent is destroyed
			on(ERangeModelDestruction) : {
				// for each child of this entity
				{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
					// send it destruction event
					itenChild->SendEvent(ERangeModelDestruction(), TRUE);
				}}
				// destroy yourself
				Destroy(FALSE);
				resume;
			}
			// when dead
			on(EDeath) : {
				SendEvent(EModelDeath(), TRUE);
				if (_pNetwork->IsServer()) {
					if (m_penDestruction != NULL) {
						jump Die();
					}
				}
				resume;
			}
			on(EModelDeath) : {
				SendEvent(EModelDeath(), TRUE);
				if (!_pNetwork->IsServer()) {
					if (m_penDestruction != NULL) {
						jump Die();
					}
				}
				resume;
			}
			// when animation should be changed
			on(EChangeAnim eChange) : {
				m_strModelAnimation = eChange.strAnimation;
				if (m_strModelAnimation != "")
				{
					INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
					INDEX iDummy1, iDummy2;
					if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) {
						NewClearState(eChange.fBlendTime);
						ULONG ulFlags = 0;
						if (eChange.bModelLoop) {
							ulFlags = AN_LOOPING;
						}
						AddAnimation(iAnim, ulFlags, 1, 1);
					} else {
						CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
							m_strModelAnimation, GetName());
					}
				}
				resume;
			}
			otherwise() : {
				resume;
			}
		};
	}

	Die()
	{
		// for each child of this entity
		{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
			// send it destruction event
			itenChild->SendEvent(ERangeModelDestruction(), TRUE);
		}}

		// spawn debris 
		CModelDestruction* pmd = GetDestruction();
		pmd->SpawnDebris(this);
		// if there is another phase in destruction
		CEntity* penNext = pmd->GetNextPhase();
		if (penNext != NULL) {
			// copy it here
			CEntity* penNew = GetWorld()->CopyEntityInWorld(*penNext, GetPlacement());
			if (IsOfClass(penNew, &CModelHolder2_DLLClass)) {
				penNew->GetModelObject()->StretchModel(GetModelInstance()->mi_vStretch);
				penNew->ModelChangeNotify();
				((CModelHolder2*)penNew)->m_colBurning = m_colBurning;
				((CModelHolder2*)penNew)->m_fChainSawCutDamage = m_fChainSawCutDamage;

				if (pmd->m_iStartAnim != -1)
				{
					penNew->GetModelObject()->PlayAnim(pmd->m_iStartAnim, 0);
				}

				// copy custom shading parameters
				CModelHolder2& mhNew = *((CModelHolder2*)penNew);
				mhNew.m_cstCustomShading = m_cstCustomShading;
				mhNew.m_colLight = m_colLight;
				mhNew.m_colAmbient = m_colAmbient;
				{ mhNew.m_fMipFadeDist = MipFactor_MetricToLog(m_rMipFadeDistMetric);
				mhNew.m_fMipFadeLen = MipFactor_MetricToLog(m_fMipFadeLenMetric + m_rMipFadeDistMetric)
					- mhNew.m_fMipFadeDist;
				/*mhNew.m_fMipAdd = m_fMipAdd;*/
				/*mhNew.m_fMipMul = m_fMipMul;*/ }
				// domino death for chainsaw
				if (m_dmtLastDamageType == DMT_CHAINSAW)
				{
					EDeath eDeath;  // we don't need any extra parameters
					mhNew.m_fChainSawCutDamage = 0.0f;
					mhNew.m_dmtLastDamageType = DMT_CHAINSAW;
					penNew->SendEvent(eDeath);
				}
			}
			else if (IsOfClass(penNew, &CModelHolder3_DLLClass)) {
				penNew->GetModelInstance()->StretchModel(GetModelInstance()->mi_vStretch);
				penNew->ModelChangeNotify();

				if (pmd->m_strStartAnim != "")
				{
					INDEX iAnim = ska_GetIDFromStringTable(pmd->m_strStartAnim);
					INDEX iDummy1, iDummy2;
					if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) {
						penNew->NewClearState(0.05f);
						penNew->AddAnimation(iAnim, 0, 1, 1);
					}
				}

				// copy custom shading parameters
				CModelHolder3& mhNew = *((CModelHolder3*)penNew);
				mhNew.m_colBurning = m_colBurning;
				mhNew.m_fChainSawCutDamage = m_fChainSawCutDamage;
				mhNew.m_cstCustomShading = m_cstCustomShading;
				mhNew.m_colLight = m_colLight;
				mhNew.m_colAmbient = m_colAmbient;
				mhNew.m_rMipFadeDistMetric = m_rMipFadeDistMetric;
				mhNew.m_fMipFadeLenMetric = m_fMipFadeLenMetric;
				// domino death for chainsaw
				if (m_dmtLastDamageType == DMT_CHAINSAW)
				{
					EDeath eDeath;  // we don't need any extra parameters
					mhNew.m_fChainSawCutDamage = 0.0f;
					mhNew.m_dmtLastDamageType = DMT_CHAINSAW;
					penNew->SendEvent(eDeath);
				}
			}
		}

		/* currently, environment destruction does not yield score.
		update statistics, if score is re-enabled!
		// send score to who killed you
		if (m_penLastDamager!=NULL) {
		EReceiveScore eScore;
		eScore.fPoints = 10.0f;
		m_penLastDamager->SendEvent(eScore);
	}*/

	// if there is a destruction target
		if (m_penDestroyTarget != NULL) {
			// notify it
			SendToTarget(m_penDestroyTarget, EET_TRIGGER, m_penLastDamager);
		}

		// destroy yourself
		Destroy(FALSE);
		return;
	}
};
