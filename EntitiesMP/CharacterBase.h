/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_CharacterBase_INCLUDED
#define _EntitiesMP_CharacterBase_INCLUDED 1
#include <EntitiesMP/Player.h>
#include <EntitiesMP/BasicEffects.h>
#include <EntitiesMP/Projectile.h>
#include <EntitiesMP/Debris.h>
#include <EntitiesMP/EnemyMarker.h>
#include <EntitiesMP/MusicHolder.h>
#include <EntitiesMP/BloodSpray.h>
#include <EntitiesMP/Unit.h>
#define EVENTCODE_ECharReconsiderBehavior 0x04570000
class DECL_DLL ECharReconsiderBehavior : public CEntityEvent {
public:
ECharReconsiderBehavior();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ECharReconsiderBehavior &e) { e = ECharReconsiderBehavior(); } ;
#define EVENTCODE_ECharForceWound 0x04570001
class DECL_DLL ECharForceWound : public CEntityEvent {
public:
ECharForceWound();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(ECharForceWound &e) { e = ECharForceWound(); } ;
#define EVENTCODE_ECharBaseDeath 0x04570002
class DECL_DLL ECharBaseDeath : public CEntityEvent {
public:
ECharBaseDeath();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
CEntityID eidInflictor;
};
DECL_DLL inline void ClearToDefault(ECharBaseDeath &e) { e = ECharBaseDeath(); } ;
#define EVENTCODE_ECharBaseDamage 0x04570003
class DECL_DLL ECharBaseDamage : public CEntityEvent {
public:
ECharBaseDamage();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
CEntityID eidInflictor;
FLOAT3D vDirection;
FLOAT3D vHitPoint;
FLOAT fAmount;
enum DamageType dmtType;
FLOAT fHealth;
};
DECL_DLL inline void ClearToDefault(ECharBaseDamage &e) { e = ECharBaseDamage(); } ;
#define EVENTCODE_ECharBlowUp 0x04570004
class DECL_DLL ECharBlowUp : public CEntityEvent {
public:
ECharBlowUp();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
FLOAT3D vDamage;
FLOAT3D vTranslation;
};
DECL_DLL inline void ClearToDefault(ECharBlowUp &e) { e = ECharBlowUp(); } ;
extern "C" DECL_DLL CDLLEntityClass CCharacterBase_DLLClass;
class DECL_DLL  CCharacterBase : public CUnit {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
   virtual void SetDefaultProperties(void);
  CEntityPointer m_penWatcher;
  FLOAT3D m_vStartPosition;
  CEntityPointer m_penEnemy;
  CTString m_strDescription;
  CTString m_strName;
  CSoundObject m_soSound;
  CEntityPointer m_penKillEnemy;
  FLOAT3D m_vDamage;
  FLOAT m_tmLastDamage;
  BOOL m_bRobotBlowup;
  FLOAT m_fBlowUpSize;
  CEntityPointer m_penPathMarker;
  FLOAT3D m_vPlayerSpotted;
  FLOAT m_fLockStartTime;
  FLOAT m_fRangeLast;
  BOOL m_bFadeOut;
  FLOAT m_fFadeStartTime;
  FLOAT m_fFadeTime;
  CEntityPointer m_penDeathTarget;
  enum EventEType m_eetDeathType;
  BOOL m_bTemplate;
  COLOR m_colColor;
  BOOL m_bDeaf;
  BOOL m_bBlind;
  FLOAT m_tmGiveUp;
  FLOAT m_tmReflexMin;
  FLOAT m_tmReflexMax;
  FLOAT m_fActivityRange;
  CEntityPointer m_penMarker;
  CEntityPointer m_penMainMusicHolder;
  FLOAT m_tmLastFussTime;
  FLOAT m_fSpiritStartTime;
  FLOAT m_tmSpraySpawned;
  FLOAT m_fSprayDamage;
  CEntityPointer m_penSpray;
  FLOAT m_fMaxDamageAmmount;
  FLOAT3D m_vLastStain;
  enum SprayParticlesType m_sptType;
  FLOAT3D m_vTacticsStartPosition;
  FLOAT m_fTacticVar3;
  FLOAT m_fTacticVar4;
  FLOAT m_fTacticVar5;
  COLOR m_colBurning;
  BOOL m_bResizeAttachments;
  FLOAT m_tmStopJumpAnim;
  BOOL m_bMoving;
  BOOL m_bRotate;
  INDEX m_iHitCnt;
  FLOAT m_fAttackStartTime;
  FLOAT m_fAttackFrequency;
  BOOL m_bStop;
  FLOAT m_fImpactTime;
  BOOL m_bPreSkilling;
  FLOAT3D vLastPosition;
  BOOL m_bPlayAction;
  BOOL m_bPlayProduce;
  BOOL m_bConsensus;
  BOOL m_bSpeedUp;
  FLOAT m_tmSeriousSpeed;
  BOOL m_bDeath;
  BOOL m_bIdleAnim;
  BOOL m_bPolymoph;
  BOOL m_bMobChange;
  BOOL m_bEvocating;
  BOOL m_bIsTransform;
    CCharacterBase(void);
  
#line 204 "E:/USA/EntitiesMP/CharacterBase.es"
virtual CTString GetPlayerKillDescription(const CTString & strPlayerName,const EDeath & eDeath);
  
#line 211 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT GetCrushHealth(void);
  
#line 217 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT & GetProp(FLOAT & m_fBase);
   
#line 224 "E:/USA/EntitiesMP/CharacterBase.es"
void MaybeSwitchToAnotherPlayer(void);
  
#line 253 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void MovementAnimation(ULONG ulFlags);
   
#line 300 "E:/USA/EntitiesMP/CharacterBase.es"
void SetTargetNone(void);
   
#line 308 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SetTargetSoft(CEntity * penPlayer);
   
#line 329 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SetTargetHard(CEntity * penPlayer);
   
#line 350 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SetTargetHardForce(CEntity * penPlayer);
   
#line 370 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SetTargetEntity(CEntity * penPlayer);
   
#line 376 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SetKillTargetEntity(CEntity * penKill);
   
#line 382 "E:/USA/EntitiesMP/CharacterBase.es"
void SetNoTargetEntity(void);
   
#line 386 "E:/USA/EntitiesMP/CharacterBase.es"
void AssignWatcher(CEntity * penWatcher);
   
#line 392 "E:/USA/EntitiesMP/CharacterBase.es"
class CWatcher * GetWatcher(void);
   void Copy(CEntity & enOther,ULONG ulFlags);
   
#line 404 "E:/USA/EntitiesMP/CharacterBase.es"
void Precache(void);
  
#line 429 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT3D PlayerDestinationPos(void);
   
#line 434 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL IfTargetCrushed(CEntity * penOther,const FLOAT3D & vDirection);
   
#line 451 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 456 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 461 "E:/USA/EntitiesMP/CharacterBase.es"
const CTString & GetDescription(void)const;
  
#line 469 "E:/USA/EntitiesMP/CharacterBase.es"
virtual const CTFileName & GetComputerMessageName(void)const;
   
#line 475 "E:/USA/EntitiesMP/CharacterBase.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 485 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL FillEntityStatistics(EntityStats * pes);
   
#line 496 "E:/USA/EntitiesMP/CharacterBase.es"
void ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 497 "E:/USA/EntitiesMP/CharacterBase.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection);
   
#line 642 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL AdjustShadingParameters(FLOAT3D & vLightDirection,COLOR & colLight,COLOR & colAmbient);
   
#line 704 "E:/USA/EntitiesMP/CharacterBase.es"
void AddToFuss(void);
   
#line 734 "E:/USA/EntitiesMP/CharacterBase.es"
void RemoveFromFuss(void);
   
#line 749 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL ShouldCeaseAttack(void);
  
#line 768 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT GetAttackMoveFrequency(FLOAT fEnemyDistance);
  
#line 778 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void SetSpeedsToDesiredPosition(const FLOAT3D & vPosDelta,FLOAT fPosDist,BOOL bGoingToPlayer);
   
#line 843 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL CanAttackEnemy(CEntity * penTarget,FLOAT fCosAngle);
  
#line 853 "E:/USA/EntitiesMP/CharacterBase.es"
virtual BOOL CanHitEnemy(CEntity * penTarget,FLOAT fCosAngle);
   
#line 861 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SeeEntity(CEntity * pen,FLOAT fCosAngle);
   
#line 869 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL SeeEntityInPlane(CEntity * pen,FLOAT fCosAngle);
   
#line 877 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL WouldNotLeaveAttackRadius(void);
  
#line 892 "E:/USA/EntitiesMP/CharacterBase.es"
virtual BOOL MayMoveToAttack(void);
  
#line 907 "E:/USA/EntitiesMP/CharacterBase.es"
virtual BOOL ShouldBlowUp(void);
   
#line 920 "E:/USA/EntitiesMP/CharacterBase.es"
void BlowUpBase(void);
  
#line 928 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void BlowUp(void);
  
#line 1005 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void LeaveStain(BOOL bGrow);
  
#line 1041 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void AdjustDifficulty(void);
   
#line 1061 "E:/USA/EntitiesMP/CharacterBase.es"
void WoundedNotify(const ECharBaseDamage & eEBDamage);
   
#line 1080 "E:/USA/EntitiesMP/CharacterBase.es"
void SeeNotify();
  
#line 1100 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void ChargeAnim(void);
  
#line 1101 "E:/USA/EntitiesMP/CharacterBase.es"
virtual INDEX AnimForDamage(FLOAT fDamage);
  
#line 1102 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void BlowUpNotify(void);
  
#line 1103 "E:/USA/EntitiesMP/CharacterBase.es"
virtual INDEX AnimForDeath(void);
  
#line 1104 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT WaitForDust(FLOAT3D & vStretch);
  
#line 1105 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void DeathNotify(void);
  
#line 1106 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void IdleSound(void);
  
#line 1107 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void SightSound(void);
  
#line 1108 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void WoundSound(void);
  
#line 1109 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void DeathSound(void);
  
#line 1110 "E:/USA/EntitiesMP/CharacterBase.es"
virtual FLOAT GetLockRotationSpeed(void);
   
#line 1113 "E:/USA/EntitiesMP/CharacterBase.es"
void RenderParticles(void);
  
#line 1144 "E:/USA/EntitiesMP/CharacterBase.es"
virtual void EnemyPostInit(void);
   
#line 1147 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 1184 "E:/USA/EntitiesMP/CharacterBase.es"
FLOAT GetAnimLength(int iAnim);
   
#line 1194 "E:/USA/EntitiesMP/CharacterBase.es"
FLOAT GetCurrentAnimLength();
   
#line 1204 "E:/USA/EntitiesMP/CharacterBase.es"
BOOL IsAnimFinished();
   
#line 1213 "E:/USA/EntitiesMP/CharacterBase.es"
FLOAT3D & GetModelStretch();
   
#line 1223 "E:/USA/EntitiesMP/CharacterBase.es"
void StretchModel(FLOAT3D vStretch);
   
#line 1233 "E:/USA/EntitiesMP/CharacterBase.es"
void StretchSingleModel(FLOAT3D vStretch);
   
#line 1244 "E:/USA/EntitiesMP/CharacterBase.es"
SLONG GetUsedMemory(void);
   
#line 1256 "E:/USA/EntitiesMP/CharacterBase.es"
void SpawnBlood_client(ECharBaseDamage eEBDamage);
   
#line 1322 "E:/USA/EntitiesMP/CharacterBase.es"
void DeathNow();
   
#line 1343 "E:/USA/EntitiesMP/CharacterBase.es"
void MoveNow();
   
#line 1358 "E:/USA/EntitiesMP/CharacterBase.es"
void AttackNow();
   
#line 1373 "E:/USA/EntitiesMP/CharacterBase.es"
void StopNow();
   
#line 1388 "E:/USA/EntitiesMP/CharacterBase.es"
void StopandTeleport();
   
#line 1415 "E:/USA/EntitiesMP/CharacterBase.es"
void SkillNow();
   
#line 1430 "E:/USA/EntitiesMP/CharacterBase.es"
void PreSkillNow();
   
#line 1445 "E:/USA/EntitiesMP/CharacterBase.es"
void ActionNow();
   
#line 1462 "E:/USA/EntitiesMP/CharacterBase.es"
void ProduceNow();
   
#line 1479 "E:/USA/EntitiesMP/CharacterBase.es"
void ConsensusNow();
   
#line 1497 "E:/USA/EntitiesMP/CharacterBase.es"
void StopProduce();
   
#line 1515 "E:/USA/EntitiesMP/CharacterBase.es"
void PolymophNow();
   
#line 1534 "E:/USA/EntitiesMP/CharacterBase.es"
void EvocateNow();
   
#line 1553 "E:/USA/EntitiesMP/CharacterBase.es"
void Rebirth();
#define  STATE_CCharacterBase_NewEnemySpotted 0x04570005
  BOOL 
#line 1574 "E:/USA/EntitiesMP/CharacterBase.es"
NewEnemySpotted(const CEntityEvent &__eeInput);
  BOOL H0x04570006_NewEnemySpotted_01(const CEntityEvent &__eeInput);
  BOOL H0x04570007_NewEnemySpotted_02(const CEntityEvent &__eeInput);
  BOOL H0x04570008_NewEnemySpotted_03(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_StopAttack 0x04570009
  BOOL 
#line 1610 "E:/USA/EntitiesMP/CharacterBase.es"
StopAttack(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_InitializeAttack 0x0457000a
  BOOL 
#line 1625 "E:/USA/EntitiesMP/CharacterBase.es"
InitializeAttack(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_AttackEnemy 0x0457000b
  BOOL 
#line 1651 "E:/USA/EntitiesMP/CharacterBase.es"
AttackEnemy(const CEntityEvent &__eeInput);
  BOOL H0x0457000c_AttackEnemy_01(const CEntityEvent &__eeInput);
  BOOL H0x0457000d_AttackEnemy_02(const CEntityEvent &__eeInput);
  BOOL H0x0457000e_AttackEnemy_03(const CEntityEvent &__eeInput);
  BOOL H0x0457000f_AttackEnemy_04(const CEntityEvent &__eeInput);
  BOOL H0x04570010_AttackEnemy_05(const CEntityEvent &__eeInput);
  BOOL H0x04570011_AttackEnemy_06(const CEntityEvent &__eeInput);
  BOOL H0x04570012_AttackEnemy_07(const CEntityEvent &__eeInput);
  BOOL H0x04570013_AttackEnemy_08(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_PerformAttack 0x04570014
  BOOL 
#line 1669 "E:/USA/EntitiesMP/CharacterBase.es"
PerformAttack(const CEntityEvent &__eeInput);
  BOOL H0x04570015_PerformAttack_01(const CEntityEvent &__eeInput);
  BOOL H0x04570016_PerformAttack_02(const CEntityEvent &__eeInput);
  BOOL H0x04570017_PerformAttack_03(const CEntityEvent &__eeInput);
  BOOL H0x04570018_PerformAttack_04(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_FireOrHit 0x04570019
  BOOL 
#line 1826 "E:/USA/EntitiesMP/CharacterBase.es"
FireOrHit(const CEntityEvent &__eeInput);
  BOOL H0x0457001a_FireOrHit_01(const CEntityEvent &__eeInput);
  BOOL H0x0457001b_FireOrHit_02(const CEntityEvent &__eeInput);
  BOOL H0x0457001c_FireOrHit_03(const CEntityEvent &__eeInput);
  BOOL H0x0457001d_FireOrHit_04(const CEntityEvent &__eeInput);
  BOOL H0x0457001e_FireOrHit_05(const CEntityEvent &__eeInput);
  BOOL H0x0457001f_FireOrHit_06(const CEntityEvent &__eeInput);
  BOOL H0x04570020_FireOrHit_07(const CEntityEvent &__eeInput);
  BOOL H0x04570021_FireOrHit_08(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Hit 0x04570022
  BOOL 
#line 1871 "E:/USA/EntitiesMP/CharacterBase.es"
Hit(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Fire 0x04570023
  BOOL 
#line 1877 "E:/USA/EntitiesMP/CharacterBase.es"
Fire(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_BeWounded 0x04570024
  BOOL 
#line 1891 "E:/USA/EntitiesMP/CharacterBase.es"
BeWounded(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Die 0x04570025
  BOOL 
#line 1905 "E:/USA/EntitiesMP/CharacterBase.es"
Die(const CEntityEvent &__eeInput);
  BOOL H0x04570026_Die_01(const CEntityEvent &__eeInput);
  BOOL H0x04570027_Die_02(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Death 0x04570028
  BOOL 
#line 2032 "E:/USA/EntitiesMP/CharacterBase.es"
Death(const CEntityEvent &__eeInput);
  BOOL H0x04570029_Death_01(const CEntityEvent &__eeInput);
  BOOL H0x0457002a_Death_02(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_DeathSequence 0x0457002b
  BOOL 
#line 2069 "E:/USA/EntitiesMP/CharacterBase.es"
DeathSequence(const CEntityEvent &__eeInput);
  BOOL H0x0457002c_DeathSequence_01(const CEntityEvent &__eeInput);
  BOOL H0x0457002d_DeathSequence_02(const CEntityEvent &__eeInput);
  BOOL H0x0457002e_DeathSequence_03(const CEntityEvent &__eeInput);
  BOOL H0x0457002f_DeathSequence_04(const CEntityEvent &__eeInput);
  BOOL H0x04570030_DeathSequence_05(const CEntityEvent &__eeInput);
  BOOL H0x04570031_DeathSequence_06(const CEntityEvent &__eeInput);
  BOOL H0x04570032_DeathSequence_07(const CEntityEvent &__eeInput);
  BOOL H0x04570033_DeathSequence_08(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Active 0x04570034
  BOOL 
#line 2117 "E:/USA/EntitiesMP/CharacterBase.es"
Active(const CEntityEvent &__eeInput);
  BOOL H0x04570035_Active_01(const CEntityEvent &__eeInput);
  BOOL H0x04570036_Active_02(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Inactive 0x04570037
  BOOL 
#line 2272 "E:/USA/EntitiesMP/CharacterBase.es"
Inactive(const CEntityEvent &__eeInput);
  BOOL H0x04570038_Inactive_01(const CEntityEvent &__eeInput);
  BOOL H0x04570039_Inactive_02(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_PreMainLoop 0x0457003a
  BOOL 
#line 2347 "E:/USA/EntitiesMP/CharacterBase.es"
PreMainLoop(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_MainLoop_internal 0x0457003b
  BOOL 
#line 2352 "E:/USA/EntitiesMP/CharacterBase.es"
MainLoop_internal(const CEntityEvent &__eeInput);
  BOOL H0x0457003c_MainLoop_internal_01(const CEntityEvent &__eeInput);
  BOOL H0x0457003d_MainLoop_internal_02(const CEntityEvent &__eeInput);
  BOOL H0x0457003e_MainLoop_internal_03(const CEntityEvent &__eeInput);
  BOOL H0x0457003f_MainLoop_internal_04(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_MainLoop 0x04570040
  BOOL 
#line 2418 "E:/USA/EntitiesMP/CharacterBase.es"
MainLoop(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_StandardBehavior 0x04570041
  BOOL 
#line 2428 "E:/USA/EntitiesMP/CharacterBase.es"
StandardBehavior(const CEntityEvent &__eeInput);
  BOOL H0x04570042_StandardBehavior_01(const CEntityEvent &__eeInput);
  BOOL H0x04570043_StandardBehavior_02(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Main 1
  BOOL 
#line 2498 "E:/USA/EntitiesMP/CharacterBase.es"
Main(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_AttackTarget 0x04570044
  BOOL 
#line 2504 "E:/USA/EntitiesMP/CharacterBase.es"
AttackTarget(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_SkillingTarget 0x04570045
  BOOL 
#line 2509 "E:/USA/EntitiesMP/CharacterBase.es"
SkillingTarget(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_SpellSkill 0x04570046
  BOOL 
#line 2514 "E:/USA/EntitiesMP/CharacterBase.es"
SpellSkill(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_ActionAnimation 0x04570047
  BOOL 
#line 2519 "E:/USA/EntitiesMP/CharacterBase.es"
ActionAnimation(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_ProduceAnimation 0x04570048
  BOOL 
#line 2526 "E:/USA/EntitiesMP/CharacterBase.es"
ProduceAnimation(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_ConsensusAnimation 0x04570049
  BOOL 
#line 2534 "E:/USA/EntitiesMP/CharacterBase.es"
ConsensusAnimation(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Polymoph 0x0457004a
  BOOL 
#line 2541 "E:/USA/EntitiesMP/CharacterBase.es"
Polymoph(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_Evocate 0x0457004b
  BOOL 
#line 2547 "E:/USA/EntitiesMP/CharacterBase.es"
Evocate(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_StopMovement 0x0457004c
  BOOL 
#line 2553 "E:/USA/EntitiesMP/CharacterBase.es"
StopMovement(const CEntityEvent &__eeInput);
#define  STATE_CCharacterBase_MovetoPoint 0x0457004d
  BOOL 
#line 2566 "E:/USA/EntitiesMP/CharacterBase.es"
MovetoPoint(const CEntityEvent &__eeInput);
  BOOL H0x0457004e_MovetoPoint_01(const CEntityEvent &__eeInput);
  BOOL H0x0457004f_MovetoPoint_02(const CEntityEvent &__eeInput);
  BOOL H0x04570050_MovetoPoint_03(const CEntityEvent &__eeInput);
  BOOL H0x04570051_MovetoPoint_04(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_CharacterBase_INCLUDED
