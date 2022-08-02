310
%{
#include "StdH.h"
#include "EntitiesMP/Common/PathFinding.h"
#include "EntitiesMP/NavigationMarker.h"
#include "EntitiesMP/TacticsHolder.h"

#include "EntitiesMP/MovingBrush.h"
#include "EntitiesMP/DestroyableArchitecture.h"

//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/EffectCommon.h>
#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Base/Timer.h>
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)

//extern void JumpFromBouncer(CEntity *penToBounce, CEntity *penBouncer);
extern INDEX ent_bReportBrokenChains;
extern INDEX dbg_bShowEnemyBodyPoint;
extern INDEX gam_iBlood;
extern INDEX gam_bGibs;
%}

uses "EntitiesMP/Player";
uses "EntitiesMP/Watcher";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/Projectile";
uses "EntitiesMP/Debris";
uses "EntitiesMP/EnemyMarker";
uses "EntitiesMP/MusicHolder";
uses "EntitiesMP/BloodSpray";
uses "EntitiesMP/Unit";

// ���ʹ� ���� �̺�Ʈ��...
// self sent in Active loop to reconsider what enemy should do
event EReconsiderBehavior {
};

// force wound
event EForceWound {
};

// entity base sends this event to itself uppon receiving EDeath from the engine
event EEnemyBaseDeath {
	CEntityID eidInflictor,
};

// entity has been damaged
event EEnemyBaseDamage {  
	CEntityID eidInflictor,    // entity that inflicted the damage
	FLOAT3D vDirection,             // where the damage came from (in absolute space)
	FLOAT3D vHitPoint,              // where the damage hit the entity (in absolute space)
	FLOAT fAmount,                  // amount of damage done
	enum DamageType dmtType,        // type of damage
	FLOAT   fHealth,
};

// entity should blow up (spawns debris)
event EBlowUp {
	FLOAT3D vDamage,
	FLOAT3D vTranslation,
};

class export CEnemyBase : CUnit {
name      "Enemy Base";
thumbnail "";
features  "HasName", "IsTargetable";

properties:
	1 CEntityPointer m_penWatcher,      // watcher
	2 FLOAT3D m_vStartPosition = FLOAT3D(0,0,0),         // start position
	3 CEntityPointer m_penEnemy,        // current enemy
	//4 enum TargetType m_ttTarget = TT_NONE, // type of target
	5 CTString m_strDescription = "Enemy base",
	6 CTString m_strName  "Name" 'N' = "Enemy base",
	7 CSoundObject m_soSound,

 43 FLOAT3D m_vDamage = FLOAT3D(0,0,0),       // current damage impact
 44 FLOAT m_tmLastDamage = -1000.0f,
 46 BOOL m_bRobotBlowup = FALSE,    // set for robots parts blowup, otherwise blowup flesh
 47 FLOAT m_fBlowUpSize = 2.0f,

 // logic temporary variables -> DO NOT USE
 //53 enum DestinationType m_dtDestination =  DT_PLAYERCURRENT, // type of current desired position
 59 CEntityPointer m_penPathMarker,   // current path finding marker
 18 FLOAT3D m_vPlayerSpotted =  FLOAT3D(0,0,0), // where player was last spotted
 57 FLOAT m_fLockStartTime = 0.0f,
 58 FLOAT m_fRangeLast = 0.0f,
130 BOOL m_bFadeOut = FALSE,
131 FLOAT m_fFadeStartTime = 0.0f,
132 FLOAT m_fFadeTime = 0.0f,

 // attack temporary -> DO NOT USE 

 // editor variables
 83 CEntityPointer m_penSpawnerTarget,                 // for re-spawning
 84 CEntityPointer m_penDeathTarget "Death target" 'D',                 // death target
 85 enum EventEType m_eetDeathType  "Death event type" 'F' = EET_TRIGGER, // death event type
 86 BOOL m_bTemplate "Template" = FALSE,                  // template enemy for spawning new enemies 
 89 COLOR m_colColor "Color" 'L' = 0x00,    // color
 90 BOOL  m_bDeaf "Deaf" = FALSE,    // deaf
 91 BOOL  m_bBlind "Blind" = FALSE,    // blind
 92 FLOAT m_tmGiveUp "Give up time" = 5.0f,    // how fast enemy gives up attack
 93 FLOAT m_tmReflexMin "Reflex Min" = 0.0f,  // how much to wait before reacting on spotting the player
 94 FLOAT m_tmReflexMax "Reflex Max" = 0.0f, 

 // random values variables
106 BOOL m_bApplyRandomStretch "Apply random stretch" = FALSE,       // apply random stretch
107 FLOAT m_fRandomStretchFactor "Random stretch factor" = 0.1f,     // random stretch
108 FLOAT m_fStretchMultiplier "Stretch multiplier" =  1.0f,         // stretch multiplier
109 FLOAT m_fRandomStretchMultiplier = 1.0f,                         // calculated random stretch

 // marker variables
120 CEntityPointer m_penMarker "Marker" 'M' COLOR(C_RED|0xFF),       // enemy marker pointer

// fuss variables
140 CEntityPointer m_penMainMusicHolder,
141 FLOAT m_tmLastFussTime = 0.0f,

144 BOOL  m_bBoss = FALSE,           // set for bosses (for health display)

145 FLOAT m_fSpiritStartTime = 0.0f, // time when spirit effect has started

146 FLOAT m_tmSpraySpawned = 0.0f,   // time when damage has been applied
147 FLOAT m_fSprayDamage = 0.0f,     // total ammount of damage
148 CEntityPointer m_penSpray,       // the blood spray
149 FLOAT m_fMaxDamageAmmount  = 0.0f, // max ammount of damage received in in last few ticks
150 FLOAT3D m_vLastStain  = FLOAT3D(0,0,0), // where last stain was left
151 enum SprayParticlesType m_sptType = SPT_LAVA_STONES, //SPT_BLOOD, // type of particles

160 CEntityPointer m_penTacticsHolder "Tactics Holder",
161 BOOL  m_bTacticActive = FALSE,
162 FLOAT m_tmTacticsActivation = 0.0f,
// warning! tactic variables are also used for dust spawning
163 FLOAT3D m_vTacticsStartPosition = FLOAT3D(0,0,0),
//165 FLOAT m_fTacticVar1 = 0.0f,
//166 FLOAT m_fTacticVar2 = 0.0f,
167 FLOAT m_fTacticVar3 = 0.0f,
168 FLOAT m_fTacticVar4 = 0.0f,
169 FLOAT m_fTacticVar5 = 0.0f,      
170 BOOL  m_bTacticsStartOnSense "Tactics start on sense" = FALSE, 
180 COLOR m_colBurning = COLOR(C_WHITE|CT_OPAQUE), // color applied when burning

181 BOOL  m_bResizeAttachments "Stretch attachments" = FALSE, // for small enemies with big guns

// internal variable
200 FLOAT m_tmStopJumpAnim = 0.0f,

201 BOOL  m_bMoving = FALSE,

205 FLOAT m_fAttackStartTime = 0.0f,
206 FLOAT m_fAttackFrequency = 0.0f,
207 BOOL m_bStop = FALSE,
208 FLOAT m_fImpactTime = 0.0f,
213 BOOL  m_bNpc = FALSE,
214 INDEX	m_nAttackCnt = 0,
215 CEntityPointer	m_penKillEnemy,        //�׾���� ���ʹ�

components:

	1 class   CLASS_WATCHER         "Classes\\Watcher.ecl",
	2 class   CLASS_PROJECTILE      "Classes\\Projectile.ecl",
	3 class   CLASS_DEBRIS          "Classes\\Debris.ecl",
	4 class   CLASS_BASIC_EFFECT    "Classes\\BasicEffect.ecl",
	5 class   CLASS_BLOOD_SPRAY     "Classes\\BloodSpray.ecl",

// ************** FLESH PARTS **************
 10 model   MODEL_FLESH          "Data\\Models\\Effects\\Debris\\Flesh\\Flesh.mdl",
 11 model   MODEL_FLESH_APPLE    "Data\\Models\\Effects\\Debris\\Fruits\\Apple.mdl",
 12 model   MODEL_FLESH_BANANA   "Data\\Models\\Effects\\Debris\\Fruits\\Banana.mdl",
 13 model   MODEL_FLESH_BURGER   "Data\\Models\\Effects\\Debris\\Fruits\\CheeseBurger.mdl",
 14 model   MODEL_FLESH_LOLLY    "Data\\Models\\Effects\\Debris\\Fruits\\LollyPop.mdl",
 15 model   MODEL_FLESH_ORANGE   "Data\\Models\\Effects\\Debris\\Fruits\\Orange.mdl",

 20 texture TEXTURE_FLESH_RED    "Data\\Models\\Effects\\Debris\\Flesh\\FleshRed.tex",
 21 texture TEXTURE_FLESH_GREEN  "Data\\Models\\Effects\\Debris\\Flesh\\FleshGreen.tex",
 22 texture TEXTURE_FLESH_APPLE  "Data\\Models\\Effects\\Debris\\Fruits\\Apple.tex",       
 23 texture TEXTURE_FLESH_BANANA "Data\\Models\\Effects\\Debris\\Fruits\\Banana.tex",      
 24 texture TEXTURE_FLESH_BURGER "Data\\Models\\Effects\\Debris\\Fruits\\CheeseBurger.tex",
 25 texture TEXTURE_FLESH_LOLLY  "Data\\Models\\Effects\\Debris\\Fruits\\LollyPop.tex",
 26 texture TEXTURE_FLESH_ORANGE "Data\\Models\\Effects\\Debris\\Fruits\\Orange.tex",

// ************** MACHINE PARTS **************
 31 model   MODEL_MACHINE        "Data\\Models\\Effects\\Debris\\Stone\\Stone.mdl",
 32 texture TEXTURE_MACHINE      "Data\\Models\\Effects\\Debris\\Stone\\Stone.tex",


functions:

	void CEnemyBase(void)
	{
	}

	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("%s killed %s"), GetClass()->ec_pdecDLLClass->dec_strName, strPlayerName);
		return str;
	}

	virtual FLOAT GetCrushHealth(void)
	{
		return 0.0f;
	}

	// overridable function for access to different properties of derived classes (flying/diving)
	virtual FLOAT &GetProp(FLOAT &m_fBase)
	{
		return m_fBase;
	}

	// check if we maybe switch to some other player (for large beasts in coop)
	// NOTE : ���� �ø���� ��Ʈ��ũ ��忡���� ����� �κ�.
	void MaybeSwitchToAnotherPlayer(void)
	{
		// if in single player
		if (GetSP()->sp_bSinglePlayer) 
		{
			// no need to check
			return;
		}

		// if current player is inside threat distance
		if (CalcDist(m_penEnemy)<GetThreatDistance()) 
		{
			// do not switch
			return;
		}
		if (_pNetwork->IsServer()) 
		{
			// maybe switch
			CEntity *penNewEnemy = GetWatcher()->CheckAnotherPlayer(m_penEnemy);
			if (penNewEnemy!=m_penEnemy && penNewEnemy!=NULL) 
			{
				m_penEnemy = penNewEnemy;
				SendEvent(EReconsiderBehavior());
			}
		}
	}

	void AssignWatcher(CEntity* penWatcher)
	{
		ASSERT(penWatcher!=NULL);
		m_penWatcher = penWatcher;
	}

	class CWatcher *GetWatcher(void)
	{
		//ASSERT(m_penWatcher!=NULL);
		return (CWatcher*)&*m_penWatcher;
	}

	export void Copy(CEntity &enOther, ULONG ulFlags)
	{
		CMovableModelEntity::Copy(enOther, ulFlags);
		CEnemyBase *penOther = (CEnemyBase *)(&enOther);
	}

	void Precache(void)
	{
		PrecacheModel(MODEL_FLESH);
		PrecacheModel(MODEL_FLESH_APPLE);
		PrecacheModel(MODEL_FLESH_BANANA);
		PrecacheModel(MODEL_FLESH_BURGER);
		PrecacheModel(MODEL_MACHINE);
		PrecacheTexture(TEXTURE_MACHINE);
		PrecacheTexture(TEXTURE_FLESH_RED);
		PrecacheTexture(TEXTURE_FLESH_GREEN);
		PrecacheTexture(TEXTURE_FLESH_APPLE); 
		PrecacheTexture(TEXTURE_FLESH_BANANA);
		PrecacheTexture(TEXTURE_FLESH_BURGER);
		PrecacheTexture(TEXTURE_FLESH_LOLLY); 
		PrecacheTexture(TEXTURE_FLESH_ORANGE); 
		PrecacheClass(CLASS_BASIC_EFFECT, BET_BLOODSPILL);
		PrecacheClass(CLASS_BASIC_EFFECT, BET_BLOODSTAIN);
		PrecacheClass(CLASS_BASIC_EFFECT, BET_BLOODSTAINGROW);
		PrecacheClass(CLASS_BASIC_EFFECT, BET_BLOODEXPLODE);
		PrecacheClass(CLASS_BASIC_EFFECT, BET_BOMB);
		PrecacheClass(CLASS_BASIC_EFFECT, BET_EXPLOSIONSTAIN);
		PrecacheClass(CLASS_DEBRIS);
		PrecacheClass(CLASS_WATCHER);
	}

	// get position you would like to go to when following player
	virtual FLOAT3D PlayerDestinationPos(void)
	{
		return m_penEnemy->GetPlacement().pl_PositionVector;
	}

	BOOL IfTargetCrushed(CEntity *penOther, const FLOAT3D &vDirection)
	{
		if( IsOfClass( penOther, &CModelHolder2_DLLClass)
		 || IsOfClass( penOther, &CModelHolder3_DLLClass)) 
		{
			FLOAT fCrushHealth = GetCrushHealth();
			if( fCrushHealth>((CRationalEntity &)*penOther).GetHealth())
			{
				InflictDirectDamage(penOther, this, 
					DMT_EXPLOSION, fCrushHealth, GetPlacement().pl_PositionVector, vDirection);
				return TRUE;
			}
		}
		return FALSE;
	}

	virtual BOOL IsSkillAttack( )
	{
		return FALSE;
	};

	virtual BOOL IsBossMob()
	{
		return FALSE;
	};

	// get movement animation for given flags with current movement type
	// ���� �̵� �÷��׿� ���� �ش��ϴ� �ִϸ��̼��� ������.
	virtual void MovementAnimation(ULONG ulFlags)
	{
		if (ulFlags&MF_MOVEZ) 
		{	
			if( m_fMoveSpeed==GetProp(m_fAttackRunSpeed) || 
				m_fMoveSpeed==GetProp(m_fCloseRunSpeed) || 
				m_fMoveSpeed > GetProp(m_fWalkSpeed ) ) 
			{
				RunningAnim();
			}
			else
			{
				WalkingAnim();
			}			
		} 
		else if (ulFlags&MF_ROTATEH) 
		{		
			if(m_bAttack)
			{
				AttackAnim();
			}
			else
			{
				RotatingAnim();
			}
		} 
		else 
		{
			if (m_penEnemy!=NULL) 
			{
				if(m_bAttack)
				{
					AttackAnim();
				}
				else
				{				
					StandingAnim();
				}
			} 
			else 
			{						
				StandingAnim();
			}
		}
	}

	
	/* Check if entity is moved on a route set up by its targets. */
	BOOL MovesByTargetedRoute(CTString &strTargetProperty) const {
		strTargetProperty = "Marker";
		return TRUE;
	};
	/* Check if entity can drop marker for making linked route. */
	BOOL DropsMarker(CTFileName &fnmMarkerClass, CTString &strTargetProperty) const 
	{
		fnmMarkerClass = CTFILENAME("Classes\\EnemyMarker.ecl");
		strTargetProperty = "Marker";
		return TRUE;
	}
	const CTString &GetDescription(void) const 
	{
		((CTString&)m_strDescription).PrintF("-><none>");
		if (m_penMarker!=NULL) {
			((CTString&)m_strDescription).PrintF("->%s", m_penMarker->GetName());
		}
		return m_strDescription;
	}

	virtual const CTFileName &GetComputerMessageName(void) const 
	{
		static CTFileName fnm(CTString(""));
		return fnm;
	}

	/* Read from stream. */
	void Read_t( CTStream *istr,BOOL bNetwork) 
	{
		CMovableModelEntity::Read_t(istr,bNetwork);

		// add to fuss if needed
		if (m_penMainMusicHolder!=NULL) 
		{
			((CMusicHolder&)*m_penMainMusicHolder).m_cenFussMakers.Add(this);
		}
	};

	/* Fill in entity statistics - for AI purposes only */
	BOOL FillEntityStatistics(EntityStats *pes)
	{
		pes->es_strName = GetClass()->ec_pdecDLLClass->dec_strName;
		if (m_bTemplate) 
		{
			pes->es_ctCount = 0;
		} 
		else 
		{
			pes->es_ctCount = 1;
		}
		pes->es_ctAmmount = 1;
		pes->es_fValue = GetHealth();
//		pes->es_iScore = m_iScore;
		return TRUE;
	}

	/* Receive damage(�������� �޾�����..) */
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
	{
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
		if(dmtType == DMT_BURNING) {return;}
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)

		// if template
		if (m_bTemplate) 
		{
			// do nothing
			return;
		}

		// adjust damage(������ ����)
/*		FLOAT fNewDamage = fDamageAmmount * DamageStrength( ((EntityInfo*)GetEntityInfo())->Eeibt, dmtType);
		// apply game extra damage per enemy and per player
		fNewDamage *=GetGameDamageMultiplier();*/
		FLOAT fNewDamage = fDamageAmmount;

		// if no damage(�������� �ȹ޾�����...)
		if (fNewDamage==0) 
		{
			// do nothing
			return;
		}		

		SetHealth(1000.0f);

		FLOAT fKickDamage = fNewDamage;
		if( (dmtType == DMT_EXPLOSION) || (dmtType == DMT_IMPACT)/* || (dmtType == DMT_CANNONBALL_EXPLOSION) */)
		{
			fKickDamage*=1.5;
		}
		if (dmtType==DMT_DROWNING || dmtType==DMT_CLOSERANGE || dmtType==DMT_CHAINSAW) 
		{
			fKickDamage /= 10;
		}
		if (dmtType==DMT_BURNING)
		{
			fKickDamage /= 100000;
			UBYTE ubR, ubG, ubB, ubA;
			FLOAT fColorFactor=fNewDamage/m_fMaxHealth*255.0f;
			ColorToRGBA(m_colBurning, ubR, ubG, ubB, ubA);
			ubR=ClampDn(ubR-fColorFactor, 32.0f);
			m_colBurning=RGBAToColor(ubR, ubR, ubR, ubA);
		}
		if(dmtType == DMT_NONE)
		{
			fKickDamage /= 100000;
		}

		// get passed time since last damage
		TIME tmNow = _pTimer->CurrentTick();
		TIME tmDelta = tmNow-m_tmLastDamage;
		m_tmLastDamage = tmNow;

		// fade damage out
		if (tmDelta>=_pTimer->TickQuantum*3) 
		{
			m_vDamage=FLOAT3D(0,0,0);
		}
		// add new damage
		FLOAT3D vDirectionFixed;
		if (vDirection.ManhattanNorm()>0.5f) 
		{
			vDirectionFixed = vDirection;
		} 
		else 
		{
			vDirectionFixed = -en_vGravityDir;
		}
		FLOAT3D vDamageOld = m_vDamage;
/*		if( (dmtType == DMT_EXPLOSION) || (dmtType == DMT_CANNONBALL_EXPLOSION) )
		{
			m_vDamage+=(vDirectionFixed/2-en_vGravityDir/2)*fKickDamage;
		}
		else*/
		{
			m_vDamage+=(vDirectionFixed-en_vGravityDir/2)*fKickDamage;
		}
		
		FLOAT fOldLen = vDamageOld.Length();
		FLOAT fNewLen = m_vDamage.Length();
		FLOAT fOldRootLen = Sqrt(fOldLen);
		FLOAT fNewRootLen = Sqrt(fNewLen);

		FLOAT fMassFactor = 300.0f/((EntityInfo*)GetEntityInfo())->fMass;
		
		if( !(en_ulFlags & ENF_ALIVE))
		{
			fMassFactor /= 3;
		}

		if(fOldLen != 0.0f)
		{
			// cancel last push
			GiveImpulseTranslationAbsolute( -vDamageOld/fOldRootLen*fMassFactor);
		}

		//-en_vGravityDir*fPushStrength/10

		// push it back
		GiveImpulseTranslationAbsolute( m_vDamage/fNewRootLen*fMassFactor);
		
		if( m_fMaxDamageAmmount<fDamageAmmount)
		{
			m_fMaxDamageAmmount = fDamageAmmount;
		}

		// if it has no spray, or if this damage overflows it, and not already disappearing
		if ((m_tmSpraySpawned<=_pTimer->CurrentTick()-_pTimer->TickQuantum*8 || 
			m_fSprayDamage+fNewDamage>50.0f)
			&& m_fSpiritStartTime==0 &&
			dmtType!=DMT_CHAINSAW && 
			!(dmtType==DMT_BURNING && GetHealth()<0) ) 
		{
			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D( vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity( plSpray, CLASS_BLOOD_SPRAY,WLD_AUTO_ENTITY_ID,FALSE);
			if(m_sptType != SPT_ELECTRICITY_SPARKS)
			{
				m_penSpray->SetParent( this);
			}

			ESpawnSpray eSpawnSpray;
			eSpawnSpray.colBurnColor=C_WHITE|CT_OPAQUE;
			
			if( m_fMaxDamageAmmount > 10.0f)
			{
				eSpawnSpray.fDamagePower = 3.0f;
			}
			else if(m_fSprayDamage+fNewDamage>50.0f)
			{
				eSpawnSpray.fDamagePower = 2.0f;
			}
			else
			{
				eSpawnSpray.fDamagePower = 1.0f;
			}

			eSpawnSpray.sptType = m_sptType;
			eSpawnSpray.fSizeMultiplier = 1.0f;

			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent( vHitPointRelative, en_vGravityDir, vReflectingNormal);
			vReflectingNormal.SafeNormalize();
			
			vReflectingNormal(1)/=5.0f;
		
			FLOAT3D vProjectedComponent = vReflectingNormal*(vDirection%vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection-vProjectedComponent*2.0f-en_vGravityDir*0.5f;

			eSpawnSpray.vDirection = vSpilDirection;
			eSpawnSpray.eidOwner = this;

			// initialize spray
			m_penSpray->Initialize( eSpawnSpray,FALSE);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}
		m_fSprayDamage+=fNewDamage;

		CMovableModelEntity::ReceiveDamage(penInflictor, 
			dmtType, fNewDamage, vHitPoint, vDirection);
	};


/************************************************************
 *                        FADE OUT                          *
 ************************************************************/

	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		colAmbient = AddColors( colAmbient, m_colColor);
		if( m_bFadeOut) 
		{
			FLOAT fTimeRemain = m_fFadeStartTime + m_fFadeTime - _pTimer->CurrentTick();
			if( fTimeRemain < 0.0f) { fTimeRemain = 0.0f; }
			COLOR colAlpha;
			if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
			{
				colAlpha = GetModelInstance()->GetModelColor();
				colAlpha = (colAlpha&0xFFFFFF00) + (COLOR(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelInstance()->SetModelColor(colAlpha);
			}
			else 
			{
				colAlpha = GetModelObject()->mo_colBlendColor;
				colAlpha = (colAlpha&0xFFFFFF00) + (COLOR(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelObject()->mo_colBlendColor = colAlpha;
			}
		} 
/*		else 
		{
			if (GetSP()->sp_bMental) 
			{
				if (GetHealth()<=0) 
				{
					if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
					{
						GetModelInstance()->SetModelColor(C_WHITE&0xFF);
					} 
					else 
					{
						GetModelObject()->mo_colBlendColor = C_WHITE&0xFF;
					}
				} 
				else 
				{
					extern FLOAT ent_tmMentalIn  ;
					extern FLOAT ent_tmMentalOut ;
					extern FLOAT ent_tmMentalFade;
					FLOAT tmIn   = ent_tmMentalIn  ;
					FLOAT tmOut  = ent_tmMentalOut ;
					FLOAT tmFade = ent_tmMentalFade;
					FLOAT tmExist = tmFade+tmIn+tmFade;
					FLOAT tmTotal = tmFade+tmIn+tmFade+tmOut;
					
					FLOAT tmTime = _pTimer->GetLerpedCurrentTick();
					FLOAT fFactor = 1;
					if (tmTime>0.1f) 
					{
						tmTime += en_ulID*123.456f;
						tmTime = fmod(tmTime, tmTotal);
						fFactor = CalculateRatio(tmTime, 0, tmExist, tmFade/tmExist, tmFade/tmExist);
					}
					
					if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
					{
						GetModelInstance()->SetModelColor(C_WHITE|INDEX(0xFF*fFactor)); 
					} 
					else 
					{
						GetModelObject()->mo_colBlendColor = C_WHITE|INDEX(0xFF*fFactor);
					}
				}
			}
		}
*/

		if(m_colBurning!=COLOR(C_WHITE|CT_OPAQUE))
		{
			colAmbient = MulColors( colAmbient, m_colBurning);
			colLight = MulColors( colLight, m_colBurning);
		}
		//return CMovableModelEntity::AdjustShadingParameters(vLightDirection, colLight, colAmbient); // �׳� TRUE�� ����
		return TRUE;
	};


	// fuss functions
	void AddToFuss(void)
	{
		// if no music holder remembered - not in fuss
		if (m_penMainMusicHolder==NULL) 
		{
			// find main music holder
			//m_penMainMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
			// if no music holder found
			if (m_penMainMusicHolder==NULL) 
			{
				// just remember last fuss time
				m_tmLastFussTime = _pTimer->CurrentTick();
				// cannot make fuss
				return;
			}
			// add to end of fuss list
			((CMusicHolder&)*m_penMainMusicHolder).m_cenFussMakers.Add(this);
			// if boss set as boss
			if (m_bBoss) {
				((CMusicHolder&)*m_penMainMusicHolder).m_penBoss = this;
			}
			// remember last fuss time
			m_tmLastFussTime = _pTimer->CurrentTick();

		// if music holder remembered - still in fuss
		} 
		else 
		{
			// must be in list
			ASSERT(((CMusicHolder&)*m_penMainMusicHolder).m_cenFussMakers.IsMember(this));			
			// just remember last fuss time
			m_tmLastFussTime = _pTimer->CurrentTick();
		}
	}
	void RemoveFromFuss(void)
	{
		// if no music holder remembered
		if (m_penMainMusicHolder==NULL) 
		{
			// not in fuss
			return;
		}
		// just remove from list
		((CMusicHolder&)*m_penMainMusicHolder).m_cenFussMakers.Remove(this);		
		m_penMainMusicHolder = NULL;
	}

	// check if should give up attacking
	// ������ �����ϴ� ���...
	BOOL ShouldCeaseAttack(void)
	{
		// if there is no valid the enemy
		if (m_penEnemy==NULL ||
			!(m_penEnemy->GetFlags()&ENF_ALIVE) || 
			 (m_penEnemy->GetFlags()&ENF_DELETED)) 
		{
			// cease attack
			return TRUE;
		}
		// otherwise, continue
		return FALSE;
	}

	// Stretch model - MUST BE DONE BEFORE SETTING MODEL!
	virtual void SizeModel(void)
	{
		FLOAT3D vStretch = GetModelStretch();
		
		// apply defined stretch
		vStretch *= m_fStretchMultiplier;

		// if should apply random stretch
		if (m_bApplyRandomStretch)
		{
			// will be done only when user clicks "Apply random" switch
			m_bApplyRandomStretch = FALSE;
			// get rnd for random stretch
			m_fRandomStretchMultiplier = (FRnd()-0.5f)*m_fRandomStretchFactor+1.0f;
		}
		
		// apply random stretch
		vStretch *= m_fRandomStretchMultiplier;

		if (m_bResizeAttachments) 
		{
			StretchModel( vStretch);
		} 
		else if (TRUE) 
		{
			StretchSingleModel( vStretch);
		}
		ModelChangeNotify();
	};
	
	// unset target
	void SetTargetNone(void)
	{
		m_ttTarget = TT_NONE;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = NULL;
	}

	// set new player as soft target if possible
	BOOL SetTargetSoft(CEntity *penPlayer)
	{
		// if invalid target
		if (!IsValidForEnemy(penPlayer)) 
		{
			// do nothing
			return FALSE;
		}
		// if we already have any kind of target
		if (m_ttTarget!=TT_NONE) 
		{
			// do nothing
			return FALSE;
		}
		// remember new soft target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_SOFT;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return penOld!=penPlayer;
	}

	// set new player as hard target if possible
	BOOL SetTargetHard(CEntity *penPlayer)
	{
		// if invalid target
		if (!IsValidForEnemy(penPlayer)) 
		{
			// do nothing
			return FALSE;
		}
		// if we already have hard target
		if (m_ttTarget==TT_HARD) 
		{
			// do nothing
			return FALSE;
		}
		// remember new hard target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_HARD;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return penOld!=penPlayer;
	}

	// force new player to be hard target
	BOOL SetTargetHardForce(CEntity *penPlayer)
	{
		// if invalid target

		if (!IsValidForEnemy(penPlayer)) 
		{
			 if(!(penPlayer->IsCharacter())) //0423 kwon
			 {
				// do nothing
				return FALSE;
			}
		}
		// remember new hard target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_HARD;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return penOld!=penPlayer;
	}

	BOOL SetTargetEntity(CEntity *penPlayer) //0628 
	{		
		m_penEnemy = penPlayer;

		return TRUE;
	}

	BOOL SetKillTargetEntity(CEntity *penKill) 
	{		
		m_penKillEnemy = penKill;

		return TRUE;
	}

	void SetNoTargetEntity(void) //0628
	{	
		m_penEnemy = NULL;
	}

	
/************************************************************
 *                     MOVING FUNCTIONS                     *
 ************************************************************/

	// get movement frequency for attack
	virtual FLOAT GetAttackMoveFrequency(FLOAT fEnemyDistance)
	{
		if (fEnemyDistance>GetProp(m_fCloseDistance)) 
		{
			return 0.5f;
		} 
		else 
		{
			return 0.25f;
		}
	}

	// set speeds for movement towards desired position(���ϴ� ��ġ�� �̵��� ���ǵ� ����)
	virtual void SetSpeedsToDesiredPosition(
		const FLOAT3D &vPosDelta,		// ���� ����
		FLOAT fPosDist,					// �Ÿ�.
		BOOL bGoingToPlayer)			// ������ �÷��̾ ���ϰ� �Ǿ��ֳ�?
	{
		//FLOAT fEnemyDistance = CalcDist(m_penEnemy);
		FLOAT fCloseDistance = GetProp(m_fCloseDistance);
		FLOAT fStopDistance = GetProp(m_fStopDistance);
		// find relative direction angle
		FLOAT fCos = GetPlaneFrustumAngle(vPosDelta);

		// if may move and
		if (fCos>CosFast(45.0f) && 
			// not too close
			fPosDist>fStopDistance) 
		{
			// move and rotate towards it
			if (fPosDist<fCloseDistance) 
			{
				m_fMoveSpeed = GetProp(m_fCloseRunSpeed);
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			}
			else
			{
				m_fMoveSpeed = GetProp(m_fAttackRunSpeed);
				m_aRotateSpeed = GetProp(m_aAttackRotateSpeed);
			}
		}
		// ȸ���ؾ���~!
		else if (fCos<CosFast(15.0f)) 
		{
			// just rotate towards it
			m_fMoveSpeed = 0;
			if (fPosDist<fCloseDistance) 
			{
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			} 
			else 
			{
				m_aRotateSpeed = GetProp(m_aAttackRotateSpeed);
			}
		}
		// otherwise (stop range)
		// ���� ������ ������...
		else 
		{
			// if going towards player, or would leave attackradius
			if (bGoingToPlayer ) 
			{
				// stand in place
				m_fMoveSpeed = 0;
				m_aRotateSpeed = 0;

			// if going to some other location (some pathfinding AI scheme)
			} 
			else 
			{
				m_fMoveSpeed = GetProp(m_fCloseRunSpeed);
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			}
		}
	}

/************************************************************
 *                   ATTACK SPECIFIC                        *
 ************************************************************/
	// can attack (shoot) at entity in plane - ground support(�ѽ��...)
	BOOL CanAttackEnemy(CEntity *penTarget, FLOAT fCosAngle) {
		if (IsInPlaneFrustum(penTarget, fCosAngle)) 
		{
			if( !_pNetwork->m_bSingleMode )
			{
				return IsVisibleCheckAll(penTarget);				
			}
			else
			{
				return TRUE;
			}
		}
		return FALSE;
	};

	// close attack if possible(������ ������?)
	virtual BOOL CanHitEnemy(CEntity *penTarget, FLOAT fCosAngle) {
		if (IsInFrustum(penTarget, fCosAngle)) 
		{
			if( !_pNetwork->m_bSingleMode )
			{
				return IsVisibleCheckAll(penTarget);
			}
			else
			{
				return TRUE;
			}
		}
		return FALSE;
	};

	// see entity
	BOOL SeeEntity(CEntity *pen, FLOAT fCosAngle) {
		if (IsInFrustum(pen, fCosAngle)) {
			return IsVisible(pen);
		}
		return FALSE;
	};

	// see entity in plane
	BOOL SeeEntityInPlane(CEntity *pen, FLOAT fCosAngle) {
		CalcPlaneDist(pen);
		if (IsInPlaneFrustum(pen, fCosAngle)) {
			return IsVisible(pen);
		}
		return FALSE;
	};

	BOOL WouldNotLeaveAttackRadius(void)
	{
		if (m_fAttackRadius<=0) 
		{
			return FALSE;
		}
		// test if we are inside radius
		BOOL bInsideNow = (m_vStartPosition-GetPlacement().pl_PositionVector).Length() < m_fAttackRadius;
		// test if going towards enemy leads us to center of attack radius circle
		BOOL bEnemyTowardsCenter = 
			(m_vStartPosition-m_penEnemy->GetPlacement().pl_PositionVector).Length() <
			(GetPlacement().pl_PositionVector-m_penEnemy->GetPlacement().pl_PositionVector).Length();
		return bInsideNow || bEnemyTowardsCenter;
	}

	// check whether may move while attacking(�����ϴ� ���� �����ϼ� �ִ��� üũ��.)
	virtual BOOL MayMoveToAttack(void) 
	{
		// check if enemy is diving
		CMovableEntity *pen = (CMovableEntity *)&*m_penEnemy;
		CContentType &ctUp = pen->en_pwoWorld->wo_actContentTypes[pen->en_iUpContent];
		BOOL bEnemyDiving = !(ctUp.ct_ulFlags&CTF_BREATHABLE_LUNGS);
		// may move if can go to enemy without leaving attack radius, and entity is not diving
		return  WouldNotLeaveAttackRadius() && !bEnemyDiving;
	};


/************************************************************
 *                 BLOW UP FUNCTIONS                        *
 ************************************************************/
	// should this enemy blow up (spawn debris)
	// ���� ������~
	virtual BOOL ShouldBlowUp(void) 
	{
		// blow up if(�׾��ٸ� ����~)
		return
			!(GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) &&
			gam_bGibs && // allowed 
			GetHealth()<=0 &&  // dead and
			m_vDamage.Length()>m_fBlowUpAmount &&  // has received large enough damage lately and
			m_fSpiritStartTime==0; // not already disappearing
	}


	// base function for blowing up(�Ⱦ��̴� ��ƾ)
	void BlowUpBase(void)
	{
		// call derived function
//������ ���� ���� �̱� ���� �۾�	07.23
		//BlowUp();
//������ ���� �� �̱� ���� �۾�		07.23
	}


	// spawn body parts(�Ⱦ��̴� ��ƾ...)
	virtual void BlowUp(void)
	{
		// blow up notify
		BlowUpNotify();
		if (!_pNetwork->IsServer()) 
		{
			CPlacement3D plSpeed;
			plSpeed.pl_PositionVector = en_vCurrentTranslationAbsolute;
			plSpeed.pl_OrientationAngle = en_aCurrentRotationAbsolute;
			AdjustDeadReckoning(en_plPlacement,plSpeed,_pTimer->CurrentTick());
		}

		FLOAT3D vNormalizedDamage = m_vDamage-m_vDamage*(m_fBlowUpAmount/m_vDamage.Length());
		vNormalizedDamage /= Sqrt(vNormalizedDamage.Length());
		vNormalizedDamage *= 0.75f;
		FLOAT3D vBodySpeed = en_vCurrentTranslationAbsolute-en_vGravityDir*(en_vGravityDir%en_vCurrentTranslationAbsolute);

		// if allowed and fleshy
		if( !(GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) && gam_bGibs && !m_bRobotBlowup)
		{
			// clamp blood type
			gam_iBlood = Clamp( gam_iBlood, (INDEX)0, (INDEX)3);
			// determine debris texture (color)
			ULONG ulFleshTexture = TEXTURE_FLESH_GREEN;
			ULONG ulFleshModel   = MODEL_FLESH;
			if( gam_iBlood==2) { ulFleshTexture = TEXTURE_FLESH_RED; }
			// spawn debris
			Debris_Begin(EIBT_FLESH, DPT_BLOODTRAIL, BET_BLOODSTAIN, m_fBlowUpSize, vNormalizedDamage, vBodySpeed, 1.0f, 0.0f);
			for( INDEX iDebris = 0; iDebris<m_fBodyParts; iDebris++) 
			{
				// flowerpower mode?
				if( gam_iBlood==3) 
				{
					switch( IRnd()%5) 
					{
					case 1:  { ulFleshModel = MODEL_FLESH_APPLE;   ulFleshTexture = TEXTURE_FLESH_APPLE;   break; }
					case 2:  { ulFleshModel = MODEL_FLESH_BANANA;  ulFleshTexture = TEXTURE_FLESH_BANANA;  break; }
					case 3:  { ulFleshModel = MODEL_FLESH_BURGER;  ulFleshTexture = TEXTURE_FLESH_BURGER;  break; }
					case 4:  { ulFleshModel = MODEL_FLESH_LOLLY;   ulFleshTexture = TEXTURE_FLESH_LOLLY;   break; }
					default: { ulFleshModel = MODEL_FLESH_ORANGE;  ulFleshTexture = TEXTURE_FLESH_ORANGE;  break; }
					}
				}
				Debris_Spawn( this, this, ulFleshModel, ulFleshTexture, 0, 0, 0, IRnd()%4, 0.5f,
											FLOAT3D(FRnd()*0.6f+0.2f, FRnd()*0.6f+0.2f, FRnd()*0.6f+0.2f));
			}
			// leave a stain beneath
//			LeaveStain(FALSE);
		}

		// if allowed and robot/machine
		if( !(GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) && gam_bGibs && m_bRobotBlowup)
		{
			// spawn debris
			Debris_Begin(EIBT_ROBOT, DPR_SMOKETRAIL, BET_EXPLOSIONSTAIN, m_fBlowUpSize, vNormalizedDamage, vBodySpeed, 1.0f, 0.0f);
			for( INDEX iDebris = 0; iDebris<m_fBodyParts; iDebris++) 
			{
				Debris_Spawn( this, this, MODEL_MACHINE, TEXTURE_MACHINE, 0, 0, 0, IRnd()%4, 0.2f,
											FLOAT3D(FRnd()*0.6f+0.2f, FRnd()*0.6f+0.2f, FRnd()*0.6f+0.2f));
			}
			// spawn explosion
			CPlacement3D plExplosion = GetPlacement();
			CEntityPointer penExplosion = CreateEntity(plExplosion, CLASS_BASIC_EFFECT,WLD_AUTO_ENTITY_ID,FALSE);
			ESpawnEffect eSpawnEffect;
			eSpawnEffect.colMuliplier = C_WHITE|CT_OPAQUE;
			eSpawnEffect.betType = BET_BOMB;
			FLOAT fSize = m_fBlowUpSize*0.3f;
			eSpawnEffect.vStretch = FLOAT3D(fSize,fSize,fSize);
			penExplosion->Initialize(eSpawnEffect,FALSE);
		}

		// hide yourself (must do this after spawning debris)
		SwitchToEditorModel();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);
	}


/************************************************************
 *                CLASS SUPPORT FUNCTIONS                   *
 ************************************************************/

	// leave stain
	virtual void LeaveStain( BOOL bGrow)
	{
		ESpawnEffect ese;
		FLOAT3D vPoint;
		FLOATplane3D vPlaneNormal;
		FLOAT fDistanceToEdge;
		// get your size
		FLOATaabbox3D box;
		GetBoundingBox(box);
	
		// on plane
		if( GetNearestPolygon(vPoint, vPlaneNormal, fDistanceToEdge)) 
		{
			// if near to polygon and away from last stain point
			if( (vPoint-GetPlacement().pl_PositionVector).Length()<0.5f
				&& (m_vLastStain-vPoint).Length()>1.0f ) 
			{
				m_vLastStain = vPoint;
				FLOAT fStretch = box.Size().Length();
				ese.colMuliplier = C_WHITE|CT_OPAQUE;
				// stain
				if (bGrow) 
				{
					ese.betType    = BET_BLOODSTAINGROW;
					ese.vStretch   = FLOAT3D( fStretch*1.5f, fStretch*1.5f, 1.0f);
				} 
				else 
				{
					ese.betType    = BET_BLOODSTAIN;
					ese.vStretch   = FLOAT3D( fStretch*0.75f, fStretch*0.75f, 1.0f);
				}
				ese.vNormal    = FLOAT3D( vPlaneNormal);
				ese.vDirection = FLOAT3D( 0, 0, 0);
				FLOAT3D vPos = vPoint+ese.vNormal/50.0f*(FRnd()+0.5f);
				CEntityPointer penEffect = CreateEntity( CPlacement3D(vPos, ANGLE3D(0,0,0)), CLASS_BASIC_EFFECT,WLD_AUTO_ENTITY_ID,FALSE);
				penEffect->Initialize(ese,FALSE);
			}
		}
	};

	// ���̵� ����.
	virtual void AdjustDifficulty(void)
	{
		FLOAT fMoveSpeed = GetSP()->sp_fEnemyMovementSpeed;
		FLOAT fAttackSpeed = GetSP()->sp_fEnemyMovementSpeed;
//    m_fWalkSpeed *= fMoveSpeed;
//    m_aWalkRotateSpeed *= fMoveSpeed;
		m_fAttackRunSpeed *= fMoveSpeed;
		m_aAttackRotateSpeed *= fMoveSpeed;
		m_fCloseRunSpeed *= fMoveSpeed;
		m_aCloseRotateSpeed *= fMoveSpeed;
		m_fAttackFireTime *= 1/fAttackSpeed;
		m_fCloseFireTime *= 1/fAttackSpeed;
	}


/************************************************************
 *                SOUND VIRTUAL FUNCTIONS                   *
 ************************************************************/

	// wounded -> yell
	void WoundedNotify(const EEnemyBaseDamage &eEBDamage)
	{
		// if no enemy
		if (m_penEnemy==NULL) 
		{
			// do nothing
			return;
		}

		// if not killed from short distance
		if (eEBDamage.dmtType!=DMT_CLOSERANGE && eEBDamage.dmtType!=DMT_CHAINSAW) 
		{
			// yell
			ESound eSound;
			eSound.EsndtSound = SNDT_YELL;
			eSound.penTarget = m_penEnemy;
			SendEventInRange(eSound, FLOATaabbox3D(GetPlacement().pl_PositionVector, 25.0f));
		}
	};

	// see enemy -> shout
	void SeeNotify() 
	{
		// if no enemy
		if (m_penEnemy==NULL) 
		{
			// do nothing
			return;
		}
		// yell
		ESound eSound;
		eSound.EsndtSound = SNDT_SHOUT;
		eSound.penTarget = m_penEnemy;
		SendEventInRange(eSound, FLOATaabbox3D(GetPlacement().pl_PositionVector, 50.0f));
	};

/************************************************************
 *          VIRTUAL FUNCTIONS THAT NEED OVERRIDE            *
 ************************************************************/
	virtual void ChargeAnim(void) {};
	virtual INDEX AnimForDamage(FLOAT fDamage) { return 0; };
	virtual void BlowUpNotify(void) {};
	virtual INDEX AnimForDeath(void) { return 0; };
	virtual FLOAT WaitForDust(FLOAT3D &vStretch) { return -1; };
	virtual void DeathNotify(void) {};
	virtual void IdleSound(void) {};
	virtual void SightSound(void) {};
	virtual void WoundSound(void) {};
	virtual void DeathSound(void) {};
	virtual FLOAT GetLockRotationSpeed(void) { return 2000.0f;};
	virtual void IdleLoop(void) {};

	// render particles
	void RenderParticles(void) 
	{
		// no particles when not existing
		if (GetRenderType()!=CEntity::RT_MODEL && GetRenderType()!=CEntity::RT_SKAMODEL) 
		{
			return;
		}
		// if is dead
		if( m_fSpiritStartTime != 0.0f)
		{
			// �׾����� ��ƼŬ ������...
			 const FLOAT tmNow = _pTimer->CurrentTick();
			// Particles_ModelGlow(this, tmNow + 20,PT_STAR08, 0.15f, 2, 0.03f, 0xff00ff00);
			Particles_Death(this, m_fSpiritStartTime);
		}
	}

	// adjust sound and watcher parameters here if needed
	virtual void EnemyPostInit(void) {};

	/* Handle an event, return false if the event is not handled. */
	// �̺�Ʈ�� ó����.
	BOOL HandleEvent(const CEntityEvent &ee)
	{
		if (ee.ee_slEvent==EVENTCODE_ETouch)
		{
			if( GetCrushHealth() != 0.0f)
			{
				ETouch eTouch = ((ETouch &) ee);
				if( IsOfClass( eTouch.penOther, &CModelHolder2_DLLClass)
				 || IsOfClass( eTouch.penOther, &CModelHolder3_DLLClass)
				 || IsOfClass( eTouch.penOther, &CMovingBrush_DLLClass)
				 || IsOfClass( eTouch.penOther, &CDestroyableArchitecture_DLLClass)) 
				{
					InflictDirectDamage(eTouch.penOther, this, DMT_EXPLOSION, GetCrushHealth(),
						eTouch.penOther->GetPlacement().pl_PositionVector, -(FLOAT3D&)eTouch.plCollision);
				}
			}
		} 
		// Enemy�� �����ؼ� Player�� �������� �Ծ�����...
		else if( ee.ee_slEvent==EVENTCODE_EEnemyBaseDamage) 
		{
			if (!_pNetwork->IsServer()) 
			{
				en_fHealth = ((const EEnemyBaseDamage&)ee).fHealth;
				//SpawnBlood_client((const EEnemyBaseDamage&)ee);
			}
			}
		else if( ee.ee_slEvent==EVENTCODE_EEnemyBaseDeath) 
		{
			if (!CMovableModelEntity::HandleEvent(ee)) 
			{
				if (en_ulFlags&ENF_ALIVE) 
				{
					CPrintF(" Warning: enemy death event not handled! Entity ID: %ld\n",en_ulID);
					if (!_pNetwork->IsServer()) 
					{
						Destroy(FALSE);
						return TRUE;
					}
				}
			}
			return FALSE;
		}
		return CMovableModelEntity::HandleEvent(ee);
	}
	
	// returns length of animation
	FLOAT GetAnimLength(int iAnim)
	{
		if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			return GetModelInstance()->GetAnimLength(iAnim);
		} else {
			return GetModelObject()->GetAnimLength(iAnim);
		}
	}
	
	// returns lenght of current anim length
	FLOAT GetCurrentAnimLength()
	{
		if(en_RenderType==RT_SKAMODEL) {
			return 0.5f;
		} else {
			return GetModelObject()->GetCurrentAnimLength();
		}
	}

	// is animation finished
	BOOL IsAnimFinished()
	{
		if(en_RenderType==RT_SKAMODEL) {
			return TRUE;
		} else {
			return GetModelObject()->IsAnimFinished();
		}
	}

	FLOAT3D &GetModelStretch()
	{
		if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
		{
			return GetModelInstance()->mi_vStretch;
		} 
		else 
		{
			return GetModelObject()->mo_Stretch;
		}
	}

	// Stretch model
	void StretchModel(FLOAT3D vStretch)
	{
		if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			GetModelInstance()->StretchModel( vStretch);
		} else {
			GetModelObject()->StretchModel( vStretch);
		}
	}

	// Stretch single model
	void StretchSingleModel( FLOAT3D vStretch)
	{
		if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			GetModelInstance()->StretchSingleModel( vStretch);
		} else {
			GetModelObject()->StretchSingleModel( vStretch);
		}
	}


	// returns bytes of memory used by this object
	SLONG GetUsedMemory(void)
	{
		// initial
		SLONG slUsedMemory = sizeof(CEnemyBase) - sizeof(CMovableModelEntity) + CMovableModelEntity::GetUsedMemory();
		// add some more
		slUsedMemory += m_strDescription.Length();
		slUsedMemory += m_strName.Length();
		slUsedMemory += 1* sizeof(CSoundObject);
		return slUsedMemory;
	}

	void SpawnBlood_client(EEnemyBaseDamage eEBDamage)		// �Ⱦ��̴� �κ�.
	{
		FLOAT fNewDamage = eEBDamage.fAmount;
		DamageType dmtType = eEBDamage.dmtType;
		FLOAT3D vHitPoint = eEBDamage.vHitPoint;
		FLOAT3D vDirection = eEBDamage.vDirection;

		// if it has no spray, or if this damage overflows it, and not already disappearing
		if ((m_tmSpraySpawned<=_pTimer->CurrentTick()-_pTimer->TickQuantum*8 || 
			m_fSprayDamage+fNewDamage>50.0f)
			&& m_fSpiritStartTime==0 &&
			dmtType!=DMT_CHAINSAW && 
			!(dmtType==DMT_BURNING && GetHealth()<0) ) 
		{

			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D( vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity( plSpray, CLASS_BLOOD_SPRAY,WLD_AUTO_ENTITY_ID,FALSE);
			if(m_sptType != SPT_ELECTRICITY_SPARKS)
			{
				m_penSpray->SetParent( this);
			}

			ESpawnSpray eSpawnSpray;
			eSpawnSpray.colBurnColor=C_WHITE|CT_OPAQUE;
			
			if( m_fMaxDamageAmmount > 10.0f)
			{
				eSpawnSpray.fDamagePower = 3.0f;
			}
			else if(m_fSprayDamage+fNewDamage>50.0f)
			{
				eSpawnSpray.fDamagePower = 2.0f;
			}
			else
			{
				eSpawnSpray.fDamagePower = 1.0f;
			}

			eSpawnSpray.sptType = m_sptType;
			eSpawnSpray.fSizeMultiplier = 1.0f;

			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent( vHitPointRelative, en_vGravityDir, vReflectingNormal);
			vReflectingNormal.SafeNormalize();
			
			vReflectingNormal(1)/=5.0f;
		
			FLOAT3D vProjectedComponent = vReflectingNormal*(vDirection%vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection-vProjectedComponent*2.0f-en_vGravityDir*0.5f;

			eSpawnSpray.vDirection = vSpilDirection;

			eSpawnSpray.eidOwner = this;
		
			// initialize spray
			m_penSpray->Initialize( eSpawnSpray,FALSE);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}
		m_fSprayDamage+=fNewDamage;
	}

//0322 kwon
	void DeathNow()
	{		
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;		
		if(GetFlags()&ENF_ALIVE)
		{
			SendEvent(EDeath());
		}
	}

	void MoveNow()
	{		
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = TRUE;		
		SendEvent(EReconsiderBehavior());
	}
	void AttackNow()
	{
		// NOTE : �̱� ����� ��쿡�� AttackTarget�� �ִϸ��̼� �߿� ȣ���ϱ⶧����,
		// NOTE : �������� �޼����� �ް� ������ ������ �ʴ´�.
		if( _pNetwork->m_bSingleMode )
		{
			return;
		}

		m_bStop = FALSE;
		m_bAttack = TRUE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;

		m_nAttackCnt++;

		SendEvent(EReconsiderBehavior());
	}

	void StopNow()
	{
		m_bStop = TRUE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;

		SendEvent(EReconsiderBehavior());
	}

	void SkillNow()
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;	
		m_bSkilling = TRUE;	
	
		SendEvent(EReconsiderBehavior());
	}

	void StopandTeleport()//1124
	{
		FLOAT3D vDeltaPlane = m_vDesiredPosition - GetPlacement().pl_PositionVector;
		if(vDeltaPlane.Length() > 0.5f)
	    {
			vDeltaPlane(2) = 0;		
			FLOAT3D vWhere = m_vDesiredPosition;
			vWhere(2) += 2.0f;
			vDeltaPlane.Normalize();
			CPlacement3D plWhere=CPlacement3D(vWhere,ANGLE3D(vDeltaPlane(1),0,0));
			this->Teleport(plWhere, FALSE);
		}
		else
		{
	
		}

		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopNow();
		StandingAnim();		
	}

//������ ���� ����	//(5th Closed beta)(0.2)
	virtual void DropDeathItem() {}
//������ ���� ��	//(5th Closed beta)(0.2)


procedures:

//**********************************************************
//                 ATTACK PROCEDURES
//**********************************************************

	// sequence that is activated when a new player is spotted visually or heard
	NewEnemySpotted()
	{
		// calculate reflex time
		FLOAT tmReflex = Lerp(m_tmReflexMin, m_tmReflexMax, FRnd());
		tmReflex = ClampDn(tmReflex, 0.0f);

		// if should wait
		if (tmReflex>=_pTimer->TickQuantum) 
		{
			// stop where you are
			StopMoving();
			StandingAnim();

			// wait the reflex time
			wait(tmReflex) 
			{
				on (ETimer) : { stop; }
				// pass all damage events
				on (EDamage) : { pass; }
				on (EEnemyBaseDamage) : { pass; }
				// pass all death events
				on (EDeath) : { pass; }
				on (EEnemyBaseDeath) : {pass; }
				// pass space beam hit
				//on (EHitBySpaceShipBeam) : { pass;}
				// ignore all other events
				otherwise () : { resume; }
			}
		}

		// play sight sound
		SightSound();

		// return to caller
		return EReturn();
	}

	// stop attack
	StopAttack(EVoid) 
	{
		if (_pNetwork->IsServer()) 
		{
			// start watching
			GetWatcher()->SendEvent(EStart());
		}
		// no damager
		SetTargetNone();
		m_fDamageConfused = 0.0f;
		// stop moving
		StopMoving();

		return EReturn();
	};

	// initial preparation
	InitializeAttack(EVoid) 
	{
		// disable blind/deaf mode
		m_bBlind = FALSE;
		m_bDeaf = FALSE;

		SeeNotify();          // notify seeing
		if (_pNetwork->IsServer()) 
		{
			GetWatcher()->SendEvent(EStop());   // stop looking
		}
		// make fuss
		AddToFuss();
		// remember spotted position
		m_vPlayerSpotted = PlayerDestinationPos();

		// set timers
		if (CalcDist(m_penEnemy)<GetProp(m_fCloseDistance)) 
		{
			m_fShootTime = 0.0f;
		} 
		else 
		{
			m_fShootTime = _pTimer->CurrentTick() + FRnd();
		}
		m_fDamageConfused = m_fDamageWounded;

		return EReturn();
	};

	// attack enemy
	// ���ʹ� ����...
	AttackEnemy(EVoid) 
	{
		// initial preparation
		autocall InitializeAttack() EReturn;

		// while you have some enemy
		// ��ǥ���� ����������???
		while (m_penEnemy != NULL) 
		{
			// attack it
			autocall PerformAttack() EReturn;
		}

		// stop attack
		// ���� ����...
		autocall StopAttack() EReturn;

		// return to Active() procedure
		return EBegin();
	};

	// repeat attacking enemy until enemy is dead or you give up
	// ���� ����...
	PerformAttack()
	{
		// reset last range
		m_fRangeLast = 1E9f;

		// set player's position as destination position
		m_vDesiredPosition = PlayerDestinationPos();
		m_dtDestination = DT_PLAYERCURRENT;

		// repeat
		while(TRUE)
		{
			// if attacking is futile(������ ����������...)
			if (ShouldCeaseAttack()) 
			{
				// cease it	(�׸���)
				SetTargetNone();
				return EReturn();
			}

			// get distance from enemy(��(�÷��̾�~)���� �Ÿ� ����)
			FLOAT fEnemyDistance = CalcDist(m_penEnemy);
			// if just entered close range(��� ����� ������ ������...)
			if (m_fRangeLast>GetProp(m_fCloseDistance) && fEnemyDistance<=GetProp(m_fCloseDistance)) 
			{
				// reset shoot time to make it attack immediately
				m_fShootTime = 0.0f;
			}
			m_fRangeLast = fEnemyDistance;

			// determine movement frequency depending on enemy distance or path finding(�̵��ð�(�� �ð����� �̵���)�� ���ʹ�(�÷��̾�)�� �Ÿ��Ǵ� ��ã��� ����)
			m_fMoveFrequency = GetAttackMoveFrequency(fEnemyDistance);			

			// wait one time interval
			wait(m_fMoveFrequency) 
			{
				on (ETimer) : 
				{ 
					stop; 
				}
				// initially
				on (EBegin) : 
				{
					// if you haven't fired/hit enemy for some time
					if (_pTimer->CurrentTick() > m_fShootTime) 
					{
						//0507 kwon ����. //0531 kwon ����.
						if(!_cmiComm. IsNetworkOn())
						{
							if (_pNetwork->IsServer()) 
							{
								// if you have new player visible closer than current and in threat distance
								CEntity *penNewEnemy = GetWatcher()->CheckCloserPlayer(m_penEnemy, GetThreatDistance());
								if (penNewEnemy!=NULL) 
								{
									// switch to that player
									SetTargetHardForce(penNewEnemy);
									// start new behavior
									SendEvent(EReconsiderBehavior());
									stop;
								}
							}
						}

						// if you can see player(�÷��̾ ���̸�...)
						if (IsVisible(m_penEnemy)) 
						{
							// remember spotted position
							m_vPlayerSpotted = PlayerDestinationPos();
							// if going to player spotted or temporary path position
							if (m_dtDestination==DT_PLAYERSPOTTED||m_dtDestination==DT_PATHTEMPORARY) {
								// switch to player current position(�������� �ش��ϴ� ���� �÷��̾��.)
								m_dtDestination=DT_PLAYERCURRENT;
							}

						// if you cannot see player(�÷��̾ �Ⱥ��̸�...)
						} 
						else 
						{
							// if going to player's current position
							if (m_dtDestination==DT_PLAYERCURRENT) 
							{
								// switch to position where player was last seen
								m_dtDestination=DT_PLAYERSPOTTED;
							}
						}						
						
						if( !IsSkillAttack() )
						{
							// try firing/hitting again now
							call FireOrHit();
						}

					// if between two fire/hit moments
					} 
					else 
					{
						// if going to player spotted or temporary path position and you just seen the player
						if ((m_dtDestination==DT_PLAYERSPOTTED||m_dtDestination==DT_PATHTEMPORARY)
							&& IsVisible(m_penEnemy)) {
							// switch to player current position
							m_dtDestination=DT_PLAYERCURRENT;
							// remember spotted position
							m_vPlayerSpotted = PlayerDestinationPos();
						}
					}

					// if you are not following the player and you are near current destination position
					FLOAT fAllowedError = m_fMoveSpeed*m_fMoveFrequency*2.0f;
					if (m_dtDestination==DT_PATHPERSISTENT||m_dtDestination==DT_PATHTEMPORARY) {
						fAllowedError = ((CNavigationMarker&)*m_penPathMarker).m_fMarkerRange;
					}
					if (m_dtDestination!=DT_PLAYERCURRENT &&
						(CalcDistanceInPlaneToDestination()<fAllowedError || fAllowedError<0.1f)) {
						// if going to where player was last spotted
						if (m_dtDestination==DT_PLAYERSPOTTED) {
							// if you see the player
							if (IsVisible(m_penEnemy)) {
								// switch to following player
								m_dtDestination=DT_PLAYERCURRENT;
							// if you don't see him
							}							
						// if using pathfinding
						} 						
					}

					// if following player
					if (m_dtDestination==DT_PLAYERCURRENT) {
						// set player's position as destination position(�÷��̾��� ��ġ�� ������...)
						m_vDesiredPosition = PlayerDestinationPos();

					// if going to where player was last seen
					} else if (m_dtDestination==DT_PLAYERSPOTTED) {
						// use that as destination position
						m_vDesiredPosition = m_vPlayerSpotted;
					}

					if( IsMovable() )
					{
						// set speeds for movement towards desired position(�������� ������ ��ġ���� �Ÿ� ���)
						FLOAT3D vPosDelta = m_vDesiredPosition-GetPlacement().pl_PositionVector;
						FLOAT fPosDistance = vPosDelta.Length();						
						
						SetSpeedsToDesiredPosition(vPosDelta, fPosDistance, m_dtDestination==DT_PLAYERCURRENT);

						// adjust direction and speed				
						ULONG ulFlags = SetDesiredMovementEx();
						MovementAnimation(ulFlags);
					}
					resume;
				}
				// if touched something
				on (ETouch eTouch) : { 
					if( IfTargetCrushed(eTouch.penOther, (FLOAT3D&)eTouch.plCollision))
					{
						resume;
					}
					else 
					{
						// pass the event
						pass;
					}
				}
				// if came to an edge
				on (EWouldFall eWouldFall) : 
				{
					// pass the event
					pass;
				}

				// pass all death events
				on (EDeath) : { pass; }
				on (EEnemyBaseDeath) : {pass; }

				on (ESound) : { resume; }     // ignore all sounds
				on (EWatch) : { resume; }     // ignore watch
				on (EReturn) : { stop; }  // returned from subprocedure
			}
		}
	}
	
	// fire or hit the enemy if possible
	FireOrHit() 
	{		
		// if player is in close range and in front
		if (CalcDist(m_penEnemy)<GetProp(m_fCloseDistance) && CanHitEnemy(m_penEnemy, Cos(AngleDeg(45.0f)))) 
		{
			// make fuss
			AddToFuss();
			// stop moving (rotation and translation)
			StopMoving();
			// set next shoot time
			m_fShootTime = _pTimer->CurrentTick() + GetProp(m_fCloseFireTime)*(1.0f + FRnd()/3.0f);
			// hit
			autocall Hit() EReturn;

		// else if player is in fire range and in front
		} 
		else if (CalcDist(m_penEnemy)<GetProp(m_fAttackDistance) && CanAttackEnemy(m_penEnemy, Cos(AngleDeg(45.0f)))) 
		{
			// make fuss
			AddToFuss();
			// stop moving (rotation and translation)
			StopMoving();
			// set next shoot time
			if (CalcDist(m_penEnemy)<GetProp(m_fCloseDistance)) {
				m_fShootTime = _pTimer->CurrentTick() + GetProp(m_fCloseFireTime)*(1.0f + FRnd()/3.0f);
			} else {
				m_fShootTime = _pTimer->CurrentTick() + GetProp(m_fAttackFireTime)*(1.0f + FRnd()/3.0f);
			}
			// fire
			autocall Hit() EReturn;

		// if cannot fire nor hit
		} 
		else 
		{
			// make sure we don't retry again too soon
			m_fShootTime = _pTimer->CurrentTick() + 0.25f;
		}

		// return to caller
		return EReturn();
	};

//**********************************************************
//                 COMBAT IMPLEMENTATION
//**********************************************************

	// this is called to hit the player when near
	Hit(EVoid) 
	{ 
		return EReturn(); 
	}

	// this is called to shoot at player when far away or otherwise unreachable
	Fire(EVoid) 
	{ 
		return EReturn(); 
	}

//**********************************************************
//                 COMBAT HELPERS
//**********************************************************

//**********************************************************
//             WOUNDING AND DYING PROCEDURES
//**********************************************************

	// play wounding animation(���ʹ̰� ������)...
	BeWounded(EEnemyBaseDamage eEBDamage)
	{ 				
		StopMoving();
		// determine damage anim and play the wounding(�̵��� ���߰� �ִϸ���Ʈ �ϱ� ��ٸ�)
		INDEX iDamageAnim = AnimForDamage(eEBDamage.fAmount);
		if (iDamageAnim != -1) {
			autowait(GetAnimLength(iDamageAnim));
		}
		StandingAnim();
		return EReturn();
	};

	// we get here once the enemy is dead(���ʹ̰� �׾����)
	Die(EEnemyBaseDeath eEBDeath)
	{
		//�����̻� ������ �ʱ�ȭ�Ѵ�.
		_pNetwork->ResetMobStatus(en_ulID);

		//0810 ���� ������ ��ŷ �ȵǵ���..
		SetCollisionFlags(ECF_IMMATERIAL);

		// not alive anymore
		SetFlagOff(ENF_ALIVE);
		// should be handled on the client side (to fadeout and selfdestroy)
		SetFlagOn(ENF_CLIENTHANDLING);

		// find the one who killed, or other best suitable player(���� �׿���?)
		CEntityPointer penKiller = (CEntity*) eEBDeath.eidInflictor;
		if( penKiller==NULL/* || !IsOfClass( penKiller, &CPlayer_DLLClass)*/) {	// �÷��̾ �׿���.
			penKiller = m_penEnemy;
		}

		if( penKiller==NULL/* || !IsOfClass(penKiller, &CPlayer_DLLClass)*/) {
			penKiller = FixupCausedToPlayer(this, penKiller, /*bWarning=*/FALSE);
		}

		// see what killed the enemy(���� �׿���???)
		KillType ekt = KT_OTHER;
		if (penKiller==NULL) {
			ekt = KT_OTHER;
		} else if( IsOfClass( eEBDeath.eidInflictor, &CPlayer_DLLClass)) {
			ekt = KT_PLAYER;
		} else if( IsDerivedFromClass((CEntity*) eEBDeath.eidInflictor, &CEnemyBase_DLLClass)) {
			ekt = KT_ENEMY;
		} 
	
		MarkEnemyDead(this);		// ���� �� ���ʹ̴� �׾��ٱ� ǥ����)


		if(_pNetwork->m_bSingleMode)
		{		
			if (_pNetwork->IsServer()) 
			{
				// destroy watcher class
				GetWatcher()->SendEvent(EStop());
				GetWatcher()->SendEvent(EEnd());
			}
		}
		
		// send event to death target(�״� ��, �׾���~, ���� ��)
		SendToTarget(m_penDeathTarget, m_eetDeathType, penKiller);
		// send event to spawner if any
		// NOTE: trigger's penCaused has been changed from penKiller to THIS;
		if (m_penSpawnerTarget) {
			SendToTarget(m_penSpawnerTarget, EET_TRIGGER, this);
		}
		
		// wait
		wait() 
		{
			// initially
			on (EBegin) : 
			{
				// if should already blow up
				if (ShouldBlowUp()) 
				{
					// blow up now
					EBlowUp eBlow;
					eBlow.vDamage = m_vDamage;
					eBlow.vTranslation = en_vCurrentTranslationAbsolute;
					SendEvent(eBlow,TRUE);
					if (_pNetwork->IsServer()) 
					{
						BlowUpBase();
					}
					// stop waiting
					resume;                                  
				// if shouldn't blow up yet
				} 
				else 
				{
					// invoke death animation sequence(�׾�����...ó������)
					call DeathSequence();
				}
			}
			// if damaged, send the message to yourself to apply the dammage (so the client side would get hurt too)
			on (EDamage eDamage) : 
			{
				if (_pNetwork->IsServer()) 
				{
					EEnemyBaseDamage eEBDamage;
					eEBDamage.eidInflictor = (CEntity*) eDamage.penInflictor;
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage,TRUE);
				}
				resume;
			}
			 // if received
			on (EEnemyBaseDamage eEBDamage) : 
			{
				// if should already blow up
				if (ShouldBlowUp()) 
				{
					EBlowUp eBlow;
					eBlow.vDamage = m_vDamage;
					eBlow.vTranslation = en_vCurrentTranslationAbsolute;
					SendEvent(eBlow,TRUE);
					if (_pNetwork->IsServer()) {
						BlowUpBase();
						// stop waiting
						stop;
					}
					resume;
				}
				// otherwise, ignore the damage
				resume;
			}
			// if needs to explode
			on (EBlowUp eBlow) : {
				if (!_pNetwork->IsServer()) {
					m_vDamage = eBlow.vDamage;
					en_vCurrentTranslationAbsolute = eBlow.vTranslation;
					BlowUpBase();
					call DeathSequence();
				}
				stop;
			}
			// if death sequence is over
			on (EEnd) : { 
				// stop waiting
				stop; 
			}
		}

		// stop making fuss
		RemoveFromFuss();

		//0830
		_pNetwork->DelMobTarget(en_ulID);

		//Destroy(FALSE);

		// all is over now, entity will be deleted
		return;
	};

	Death(EVoid) 
	{
		StopMoving();     // stop moving
		DeathSound();     // death sound
		LeaveStain(FALSE);

		// set physic flags
		SetPhysicsFlags(EPF_MODEL_CORPSE);
		SetCollisionFlags(ECF_CORPSE);
		SetFlags(GetFlags() | ENF_SEETHROUGH);

		// stop making fuss
		RemoveFromFuss();

		// death notify (usually change collision box and change body density)
		DeathNotify();

		// start death anim
		INDEX iAnim = AnimForDeath();

		// use tactic variables for temporary data
		m_vTacticsStartPosition=FLOAT3D(1,1,1);
		m_fTacticVar4=WaitForDust(m_vTacticsStartPosition);
		// remember start time
		m_fTacticVar5=_pTimer->CurrentTick();
		// mark that we didn't spawned dust yet
		m_fTacticVar3=-1;

			
		autowait(0.5f);
		DropDeathItem();

		// if no dust should be spawned
		if( m_fTacticVar4<0)
		{
	//		autowait(GetAnimLength(iAnim));
		}	
		// should spawn dust
		else if( TRUE)
		{
			while(_pTimer->CurrentTick()<m_fTacticVar5+GetCurrentAnimLength())
			{
				autowait(_pTimer->TickQuantum);
				if(en_penReference!=NULL && _pTimer->CurrentTick()>=m_fTacticVar5+m_fTacticVar4 && m_fTacticVar3<0)
				{
					// spawn dust effect
					CPlacement3D plFX=GetPlacement();
					ESpawnEffect ese;
					ese.colMuliplier = C_WHITE|CT_OPAQUE;
					ese.vStretch = m_vTacticsStartPosition;
					ese.vNormal = FLOAT3D(0,1,0);
					ese.betType = BET_DUST_FALL;
					CPlacement3D plSmoke=plFX;
					plSmoke.pl_PositionVector+=FLOAT3D(0,0.35f*m_vTacticsStartPosition(2),0);
					CEntityPointer penFX = CreateEntity(plSmoke, CLASS_BASIC_EFFECT);
					penFX->Initialize(ese);
					penFX->SetParent(this);
					// mark that we spawned dust
					m_fTacticVar3=1;
				}
			}
		}

		return EEnd();
	};

	DeathSequence(EVoid)
	{
		// not alive anymore
		SetFlagOff(ENF_ALIVE);
		// should be handled on the client side (to fadeout and selfdestroy)
		SetFlagOn(ENF_CLIENTHANDLING);

		// entity death
		autocall Death() EEnd;

		// start bloody stain growing out from beneath the corpse
		//LeaveStain(TRUE);

		// check if you have attached flame
		CEntityPointer penFlame = GetChildOfClass("Flame");
		if (penFlame!=NULL)
		{
			// send the event to stop burning
			EStopFlaming esf;
			esf.m_bNow=FALSE;
			penFlame->SendEvent(esf);
		}

		FLOAT fWaitTime = 2.0f;
		if(IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			fWaitTime = 0.1f;
		}
		autowait(fWaitTime);

		// start fading out and turning into stardust effect
		m_fSpiritStartTime = _pTimer->CurrentTick();
		m_fFadeStartTime = _pTimer->CurrentTick();
		m_fFadeTime = 4.0f,
		m_bFadeOut = TRUE;
		// become passable even if very large corpse
		SetCollisionFlags(ECF_CORPSE&~((ECBI_PROJECTILE_MAGIC|ECBI_PROJECTILE_SOLID)<<ECB_TEST));

		if(IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			m_fFadeTime = 0.1f;
		}

		// wait for fading
		autowait(m_fFadeTime);

		FLOAT fEffectWait = 6.0f;
		if(IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			fEffectWait = 0.1f;			
		}
		// wait for the stardust effect
		autowait(fEffectWait);

		return EEnd();
	}
//**********************************************************
//                MAIN LOOP PROCEDURES
//**********************************************************

	// move
	// ��ƼƼ�� Ȱ��ȭ�� ����.
	// NOTE : �����ϰ� �ٸ� �κ��ε�...
	Active(EVoid) 
	{
		m_fDamageConfused = 0.0f;		

		// logic loop
		wait () 
		{						
			// initially
			on (EBegin) : 
			{
				// start new behavior
				SendEvent(EReconsiderBehavior());
				resume;
			}
			// if new behavior is requested
			// ���ο� �ൿ�� ��û��...
			on (EReconsiderBehavior) : 
			{	
				if(m_penEnemy!=NULL) //���� �Ǵ� ���ݽ� rotate
				{	
					//SetFlags(GetFlags()|ENF_ALIVE);
					//m_penEnemy�� �ִٸ� attack�� �ؾ��Ѵ�.

					if(m_bSkilling)
					{
						m_bSkilling = FALSE;
						call SkillingTarget();
					}
					else if(m_bAttack)
					{
						m_bAttack = FALSE;
						call AttackTarget();
					}					
					else
					{
						SetNoTargetEntity();
					}											
				}
				else if(m_penEnemy==NULL) //0628 Ÿ���� ����.
				{
					if(m_bStop) //���� �Ǿ����� �Ǵ� �̵��� ���⶧
					{
						call StopMovement();
					}
					else if(m_bMoving)
					{
						call MovetoPoint();						
					}					
				}
				resume;
			}
			// on return from some of the sub-procedures
			on (EReturn) : 
			{
				// start new behavior(���ο� �ൿ�� ������)
				SendEvent(EReconsiderBehavior());
				resume;
			}
			// if damaged, send the message to yourself to apply the dammage (so the client side would get hurt too)
			on (EDamage eDamage) : 
			{
				if (_pNetwork->IsServer())	// Player�� �������� �޼���...
				{
					EEnemyBaseDamage eEBDamage;
					eEBDamage.eidInflictor = (CEntity*) eDamage.penInflictor;
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage,TRUE);
					resume;
				}
			}
			 // if received damage
			on (EEnemyBaseDamage eEBDamage) : 
			{
			//0531 kwon ����.			
				// if confused
				m_fDamageConfused -= eEBDamage.fAmount;
				if (m_fDamageConfused < 0.001f) 
				{
					m_fDamageConfused = m_fDamageWounded;
					// notify wounding to others
//					WoundedNotify(eEBDamage);
					// make pain sound
					WoundSound(); 
					// play wounding animation
					if(!m_bAttack && !m_bMoving)//0629
					{
						call BeWounded(eEBDamage);
					}
					else
					{
//������ ���� ����	//(Open beta)(2005-01-03)
						INDEX animSet, animIdx;
						INDEX animId = ska_StringToID("mldam");//HardCoding
						if(this->en_pmiModelInstance
						&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{
							//Ÿ�� ���
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
//������ ���� ��	//(Open beta)(2005-01-03)
					}
				}
				resume;
			}

			on (EForceWound) :
			{
				call BeWounded(EEnemyBaseDamage());
				resume;
			}
			// on touch
			on (ETouch eTouch) : {

				pass;
			}
		}
	};

//������ ���� ���� �̱� ���� �۾�	07.23
	SingleActive(EVoid)
	{
		m_fDamageConfused = 0.0f;
		// logic loop
		wait () 
		{
			// initially
			on (EBegin) : 
			{
				// start new behavior
				SendEvent(EReconsiderBehavior());
				resume;
			}
			// if new behavior is requested
			// ���ο� �ൿ�� ��û��...
			on (EReconsiderBehavior) : 
			{
				if( m_penEnemy != NULL && m_penEnemy->IsFlagOn(ENF_ALIVE) ) //���� �Ǵ� ���ݽ� rotate
				{	
					//SetFlags(GetFlags()|ENF_ALIVE);
					//m_penEnemy�� �ִٸ� attack�� �ؾ��Ѵ�.

					if(m_bSkilling)
					{
						m_bSkilling = FALSE;
						call SkillingTarget();
					}
					else
					{
							// �Ϲ� ����...												
						SetFlagOn(ENF_ALIVE);
						call AttackEnemy();					
					}
				}		
				resume;
			}
			// on return from some of the sub-procedures
			on (EReturn) : 
			{
				// start new behavior(���ο� �ൿ�� ������)
				SendEvent(EReconsiderBehavior());
				resume;
			}
			// if enemy has been seen(�÷��̾ ���δٸ�?)
			on (EWatch eWatch) : 
			{
				// if new enemy
				if (SetTargetSoft((CEntity*)eWatch.eidSeen)) 
				{
					// react to it
					call NewEnemySpotted();
				}
				resume;
			}

			// if damaged, send the message to yourself to apply the dammage (so the client side would get hurt too)
			on (EDamage eDamage) : 
			{
				if (_pNetwork->IsServer())	// Player�� �������� �޼���...
				{
					EEnemyBaseDamage eEBDamage;
					eEBDamage.eidInflictor = (CEntity*) eDamage.penInflictor;
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage,TRUE);
					resume;
				}
			}
			 // if received damage
			on (EEnemyBaseDamage eEBDamage) : 
			{
			//0531 kwon ����.			
				// if confused
				m_fDamageConfused -= eEBDamage.fAmount;
				if (m_fDamageConfused < 0.001f) 
				{
					m_fDamageConfused = m_fDamageWounded;
					// notify wounding to others
//					WoundedNotify(eEBDamage);
					// make pain sound
					WoundSound(); 
					// play wounding animation
					if(!m_bAttack && !m_bMoving && !IsBossMob() )//0629
					{
						call BeWounded(eEBDamage);
					}					
				}
				resume;
			}

			on (EForceWound) :
			{
				call BeWounded(EEnemyBaseDamage());
				resume;
			}
			// on touch
			on (ETouch eTouch) : 
			{
				pass;
			}

			// warn for all obsolete events
			on (EStartAttack) : 
			{
				//CPrintF("%s: StartAttack event is obsolete!\n", GetName());
				resume;
			}
			on (EStopAttack) : 
			{
				//CPrintF("%s: StopAttack event is obsolete!\n", GetName());
				resume;
			}
		}
	};
//������ ���� �� �̱� ���� �۾�		07.23

	// not doing anything, waiting until some player comes close enough to start patroling or similar
	// �ƹ��͵� ���� ����.  � �÷��̾ ������ �ö����� ���.
	Inactive(EVoid) 
	{
		// stop moving
		StopMoving();                 
		StandingAnim();

		// wait forever
		wait() 
		{
			on (EBegin) : 
			{ 
				if(m_bNpc)
				{
					call IdleLoop(); //0125 �̰��� NPC�� ����ó ȣ���ϴ� �κ�
					//jump Active(); 
				}
				else 
				{
					jump Active(); 
				}
			
			}

			// if watcher detects that a player is near
			// watcher�� �÷��̾ �����̿��� ã�Ҵٸ�, �������ֱ�~
			on (EStart) : 
			{ 
				// become active (patroling etc.)
				jump Active(); 
			}

			// if returned from wounding
			// ������ �ִٰ� ������ �ٷ� �����ؾ���~!!!
			on (EReturn) : 
			{ 
				// become active (this will attack the enemy)
				jump Active(); 
			}
			// if triggered manually
			on (ETrigger eTrigger) : 
			{
				CEntity *penCaused = FixupCausedToPlayer(this, eTrigger.penCaused);
				// if can set the trigerer as soft target
				if (SetTargetSoft(penCaused)) 
				{
					// become active (patroling etc.)
					jump Active(); 
				}
			}
			// if damaged, send the message to yourself to apply the dammage (so the client side would get hurt too)
			on (EDamage eDamage) : 
			{
				if (_pNetwork->IsServer()) 
				{
					EEnemyBaseDamage eEBDamage;
					eEBDamage.eidInflictor = (CEntity*) eDamage.penInflictor;
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage,TRUE);
				}
				resume;
			}
			 // if received damage
			on (EEnemyBaseDamage eEBDamage) : 
			{
				// eventually set new hard target
				if (SetTargetHard((CEntity*)eEBDamage.eidInflictor)) 
				{
					// notify wounding to others
					WoundedNotify(eEBDamage);
					// make pain sound
					WoundSound();
					// play wounding animation
					call BeWounded(eEBDamage);
				}
				resume;
			}
		}
	};

	SingleInActive(EVoid)
	{
		// stop moving
		StopMoving();                 
		StandingAnim();

		// Ȱ��ȭ���� ���� ���¿����� Watcher�� �Ѱ� ĳ������ �������� ����.
		if (_pNetwork->IsServer()) 
		{
			// start watching
			GetWatcher()->SendEvent(EStart());			
		}
		// wait forever
		wait() 
		{
			on (EBegin) : { resume; }

			// if watcher detects that a player is near
			// watcher�� �÷��̾ �����̿��� ã�Ҵٸ�, �������ֱ�~
			on (EStart) : 
			{ 
				jump SingleActive();			
			}

			// if returned from wounding
			// ������ �ִٰ� ������ �ٷ� �����ؾ���~!!!
			on (EReturn) : 
			{ 				
				jump SingleActive();			
			}

			// if triggered manually
			on (ETrigger eTrigger) : 
			{
				CEntity *penCaused = FixupCausedToPlayer(this, eTrigger.penCaused);
				// if can set the trigerer as soft target
				if (SetTargetSoft(penCaused)) 
				{
					jump SingleActive();				
				}
			}

			// if damaged, send the message to yourself to apply the dammage (so the client side would get hurt too)
			on (EDamage eDamage) : 
			{
				if (_pNetwork->IsServer()) 
				{
					EEnemyBaseDamage eEBDamage;
					eEBDamage.eidInflictor = (CEntity*) eDamage.penInflictor;
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage,TRUE);
				}
				resume;
			}
			 // if received damage
			on (EEnemyBaseDamage eEBDamage) : 
			{
				// eventually set new hard target
				if (SetTargetHard((CEntity*)eEBDamage.eidInflictor)) 
				{
					// notify wounding to others
					WoundedNotify(eEBDamage);
					// make pain sound
					WoundSound();
					// play wounding animation
					call BeWounded(eEBDamage);
				}
				resume;
			}
		}
	};

	// overridable called before main enemy loop actually begins
	PreMainLoop(EVoid)
	{
		return EReturn();
	}

	MainLoop_internal()
	{
		if (m_bWasKilled) 
		{
			Destroy(FALSE);
			return;
		}
		// setup some model parameters that are global for all enemies
		
		// check that max health is properly set
//		ASSERT(m_fMaxHealth==GetHealth() || IsOfClass(this, "Devil") || IsOfClass(this, "ExotechLarva") || IsOfClass(this, "AirElemental") || IsOfClass(this, "Summoner"));

		// normalize parameters
		if (m_tmReflexMin<0) {
			m_tmReflexMin = 0.0f;
		}
		if (m_tmReflexMin>m_tmReflexMax) {
			m_tmReflexMax = m_tmReflexMin;
		}

		// adjust falldown and step up values
		if (m_fStepHeight==-1) 
		{
			m_fStepHeight = 2.0f;
		}

		// if this is a template(EnemySpawn�� ���� �κ�)
		if (m_bTemplate) 
		{
			// do nothing at all
			return;
		}

		// wait for just one tick
		// NOTES:
		// if spawned, we have to wait a bit after spawning for spawner to teleport us into proper place
		// if placed directly, we have to wait for game to start (not to start behaving while in editor)
		// IMPORTANT: 
		// this wait amount has to be lower than the one in music holder, so that the enemies are initialized before
		// they get counted
		SetFlagOn(ENF_CLIENTHANDLING);
		autowait(_pTimer->TickQuantum);		// 1/20�� ��ٸ�.
		SetFlagOff(ENF_CLIENTHANDLING);
		
		if(_pNetwork->m_bSingleMode)
		{
			if (_pNetwork->IsServer()) 
			{
				// spawn your watcher(AI ���õ� �κ�)
				m_penWatcher = CreateEntity(GetPlacement(), CLASS_WATCHER,WLD_AUTO_ENTITY_ID,FALSE);
				EWatcherInit eInitWatcher;   

				eInitWatcher.eidOwner = this;
				GetWatcher()->Initialize(eInitWatcher,FALSE);
			}
		}
		// store starting position
		m_vStartPosition = GetPlacement().pl_PositionVector;

		// set sound default parameters
		m_soSound.Set3DParameters( 100.0f, 10.0f, 1.0f, 1.0f);

		// adjust falldown and step up values
		en_fStepUpHeight = m_fStepHeight+0.01f;
		en_fStepDnHeight = m_fFallHeight+0.01f;

		// let derived class(es) adjust parameters if needed
//������ ���� ���� �α��� ó�� �۾�	07.20
		EnemyPostInit();
//������ ���� �� �α��� ó�� �۾�		07.20

		// adjust your difficulty(���̵� ����)
//������ ���� ���� �α��� ó�� �۾�	07.20
		//AdjustDifficulty();
//������ ���� �� �α��� ó�� �۾�		07.20
		// check enemy params
		ASSERT(m_fStopDistance>=0);
		ASSERT(m_fCloseDistance>=0);
//		ASSERT(m_fAttackDistance>m_fCloseDistance);
		ASSERT(m_fIgnoreRange>m_fAttackDistance);
		autocall PreMainLoop() EReturn;

		jump StandardBehavior();		// ���� ���ν����� ���߰� StandardBehavior�� ������.
	}

	// main entry point for enemy behavior
	MainLoop(EVoid) 
	{
		if (_pNetwork->IsServer()) 
		{
			jump MainLoop_internal();
		} 		
	}

	//--------------------------------------------------------------------------------
	// �⺻ �ൿ.
	StandardBehavior(EVoid)
	{
		// this is the main enemy loop
		wait()	// ���� ����
		{
			// initially
			on (EBegin) :	// ���ν��� ����.
			{
				// start in active or inactive state
				// ���� �����Ǿ��ٸ�...

				if (m_penEnemy!=NULL) 
				{
					if(!_pNetwork->m_bSingleMode)
					{
						call Active();
					}
					else
					{
						call SingleActive();
					}
				}
				// ���� ��ã������...
				else
				{
					//������ ���� ���� �̱� ���� �۾�	07.23
					if(!_pNetwork->m_bSingleMode)
					{
						call Inactive();
					}
					else
					{
						call SingleInActive();
					}	
				}
			};
			// if received damage
			// �������� �޾�����...
			on (EEnemyBaseDamage eEBDamage) : 
			{
				// if should already blow up
				if (ShouldBlowUp()) 
				{ //�����ؾ� �Ѵٸ�,
					// blow up now
					EBlowUp eBlow;
					eBlow.vDamage = m_vDamage;
					eBlow.vTranslation = en_vCurrentTranslationAbsolute;
					SendEvent(eBlow,TRUE);
					if (_pNetwork->IsServer()) 
					{
						BlowUpBase(); //����.
						// stop waiting
						stop;		// wait�� ����.
					}
					resume;		// ��� wait
				}
				// otherwise, ignore the damage
				resume;		// ��� wait
			}
			// if dead
			// ���ʹ̰� �׾��ٸ�...(EEnemyBaseDeath�� ������...)
			on (EDeath eDeath) : 
			{
				EEnemyBaseDeath eEBDeath;
				eEBDeath.eidInflictor = (CEntity*) eDeath.eLastDamage.penInflictor;
				SendEvent(eEBDeath,TRUE);
				resume;		// ��� wait
			}
			on (EEnemyBaseDeath eEBDeath) : 
			{
				m_bWasKilled = TRUE; 
				// die
				jump Die(eEBDeath);		// �׾����� ���ν����� ���߰� Die���ν����� �Ѿ�� ����.
			}

			// if an entity exits a teleport nearby
			on (ETeleport et) : 
			{
				// proceed message to watcher (so watcher can quickly recheck for players)
				if (_pNetwork->IsServer()) 
				{
//������ ���� ����	//(Bug Fix)(0.1)
					if( _pNetwork->m_bSingleMode )
					{
						if(GetWatcher() != NULL) 
						{
							GetWatcher()->SendEvent(et);
						}
					}
//������ ���� ��	//(Bug Fix)(0.1)
				}
				resume;
			}
			// if should stop being blind
			on (EStopBlindness) : 
			{
				// stop being blind
				m_bBlind = FALSE;
				resume;
			}
			// if should stop being deaf
			on (EStopDeafness) : 
			{
				// stop being deaf
				m_bDeaf = FALSE;
				resume;
			}
			on (EAddHostile) :
			{
				en_pwoWorld->m_vectorTargetNPC.push_back(this);
				_pNetwork->AddRescueNPC(this);
				resume;
			}
			// if needs to explode
			// ������ �ʿ䰡 �ִٸ�...
			on (EBlowUp eBlow) : 
			{
				if (!_pNetwork->IsServer()) 
				{
					m_vDamage = eBlow.vDamage;
					en_vCurrentTranslationAbsolute = eBlow.vTranslation;
					BlowUpBase();
					call DeathSequence();
				}
				stop;
			}
		}
	};

	// dummy main - never called
	Main(EVoid) 
	{
		return;
	};

	//--------------------------------------------------------------------------------
	//0628 kwon
	AttackTarget() // �� ���ν����� call�ϱ����� SetTargetEntity()�� ����Ǿ�� �Ѵ�.
	{
		return EReturn();
	};

	SkillingTarget() // override...
	{	
		return EReturn();
	};

	//--------------------------------------------------------------------------------
	StopMovement()
	{
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;

		SetNoTargetEntity();
		StopMoving(); 
		StandingAnim();

		return EReturn();
	};

	//--------------------------------------------------------------------------------
	MovetoPoint() //ȣ������ m_fMoveSpeed�� m_vDesiredPosition�� ��������� �Ѵ�.
	{
		SetNoTargetEntity();
		m_aRotateSpeed =  AngleDeg(1800.0f);		
		m_fMoveFrequency = 0.25f;		
		
		FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_vDesiredPosition;	

//		CPrintF("Start SetDesiredMovement () %f\n",_pTimer->CurrentTick());
	
		while (CalcDistanceInPlaneToDestination() > 0.5f)
		{	 
			wait (m_fMoveFrequency) {
				on (EBegin) : { 

					if(m_bAttack)
					{
						m_bMoving = FALSE;
						EReturn();
					}

					// adjust direction and speed
					ULONG ulFlags = SetDesiredMovement(); 
					MovementAnimation(ulFlags);	

					if(ulFlags==0){
						stop;
					}
					resume;
				}
				on (ETimer) : { stop; }
			}
		}

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;

		m_fMoveSpeed = 0.0f;
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 
		StandingAnim();
		
		//return EBegin();
		return EReturn();
	};

	IdleAnimation() 
	{
		return EReturn();
	};

	IdleLoop()
	{
		while(TRUE) 
		{
			autocall IdleAnimation() EReturn;
		}

	}
};
