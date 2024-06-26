/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_Unit_INCLUDED
#define _EntitiesMP_Unit_INCLUDED 1
extern DECL_DLL CEntityPropertyEnumType wPetAIState_enum;
enum wPetAIState {
  WPET_AI_NONE = 0,
  WPET_AI_STOP = 1,
  WPET_AI_ACTION = 2,
  WPET_AI_NORMALATTACK = 3,
  WPET_AI_USESKILL = 4,
  WPET_AI_FOLLOW = 5,
};
DECL_DLL inline void ClearToDefault(wPetAIState &e) { e = (wPetAIState)0; } ;
extern DECL_DLL CEntityPropertyEnumType EventType_enum;
enum EventType {
  EVENT_NONE = 0,
  EVENT_MOBCREATE = 1,
  EVENT_MOBDESTROY = 2,
  EVENT_MOBCLICK = 3,
};
DECL_DLL inline void ClearToDefault(EventType &e) { e = (EventType)0; } ;
extern DECL_DLL CEntityPropertyEnumType DestinationType_enum;
enum DestinationType {
  DT_PLAYERCURRENT = 0,
  DT_PLAYERSPOTTED = 1,
  DT_PATHTEMPORARY = 2,
  DT_PATHPERSISTENT = 3,
};
DECL_DLL inline void ClearToDefault(DestinationType &e) { e = (DestinationType)0; } ;
extern DECL_DLL CEntityPropertyEnumType TargetType_enum;
enum TargetType {
  TT_NONE = 0,
  TT_SOFT = 1,
  TT_HARD = 2,
};
DECL_DLL inline void ClearToDefault(TargetType &e) { e = (TargetType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CUnit_DLLClass;
class DECL_DLL  CUnit : public CMovableModelEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
   virtual void SetDefaultProperties(void);
  enum TargetType m_ttTarget;
  CTString m_strName;
  FLOAT m_fMaxHealth;
  INDEX m_nMaxiHealth;
  INDEX m_nCurrentHealth;
  INDEX m_nMobLevel;
  INDEX m_nPreHealth;
  INDEX m_nMobDBIndex;
  FLOAT m_fActivityRange;
  RANGE m_fAttackRadius;
  enum DestinationType m_dtDestination;
  FLOAT m_fWalkSpeed;
  ANGLE m_aWalkRotateSpeed;
  FLOAT m_fAttackRunSpeed;
  ANGLE m_aAttackRotateSpeed;
  FLOAT m_fCloseRunSpeed;
  ANGLE m_aCloseRotateSpeed;
  FLOAT m_fAttackDistance;
  FLOAT m_fCloseDistance;
  FLOAT m_fAttackFireTime;
  FLOAT m_fCloseFireTime;
  FLOAT m_fStopDistance;
  FLOAT m_fIgnoreRange;
  FLOAT m_fMoveTime;
  FLOAT m_fMoveSpeed;
  ANGLE m_aRotateSpeed;
  FLOAT m_fMoveFrequency;
  RANGE m_fSenseRange;
  FLOAT m_fViewAngle;
  FLOAT m_fFallHeight;
  FLOAT m_fStepHeight;
  FLOAT m_fShootTime;
  FLOAT m_fDamageConfused;
  FLOAT m_fBlowUpAmount;
  INDEX m_fBodyParts;
  FLOAT m_fDamageWounded;
  INDEX m_nPlayActionNum;
  FLOAT3D m_vDesiredPosition;
  BOOL m_bSkilling;
  INDEX m_nCurrentSkillNum;
  FLOAT m_fSkillSpeed;
  FLOAT m_fAttackLengthMul;
  FLOAT m_fAttackSpeedMul;
  INDEX m_idCurrentSkillAnim;
  BOOL m_bAttack;
  BOOL m_bWasKilled;
  BOOL m_bUseAI;
  BOOL m_bAIStart;
  FLOAT m_tmSkillStartTime;
  FLOAT m_tmSkillAnimTime;
  FLOAT m_fSkillAnimTime;
  INDEX m_bKillEnemy;
  FLOAT3D m_vMyPositionTmp;
  BOOL m_bStuned;
  BOOL m_bHold;
  BOOL m_bCannotUseSkill;
CSelectedEntities m_dcEnemies;
  
#line 127 "E:/USA/EntitiesMP/Unit.es"
virtual void OnInitialize(const CEntityEvent & eeInput);
  
#line 132 "E:/USA/EntitiesMP/Unit.es"
virtual void OnEnd(void);
  
#line 138 "E:/USA/EntitiesMP/Unit.es"
virtual void DeathNow();
  
#line 139 "E:/USA/EntitiesMP/Unit.es"
virtual void SkillNow();
  
#line 140 "E:/USA/EntitiesMP/Unit.es"
virtual void PreSkillNow();
  
#line 141 "E:/USA/EntitiesMP/Unit.es"
virtual void AttackNow();
  
#line 142 "E:/USA/EntitiesMP/Unit.es"
virtual void ActionNow();
  
#line 143 "E:/USA/EntitiesMP/Unit.es"
virtual void SetAttackSpeed(SBYTE attackspeed);
  
#line 144 "E:/USA/EntitiesMP/Unit.es"
virtual void StopNow();
  
#line 145 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL SetTargetEntity(CEntity * penPlayer);
  
#line 146 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL SetKillTargetEntity(CEntity * penKill);
  
#line 147 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL SetEnemyEntity(CEntity * penEnemy,int type);
  
#line 148 "E:/USA/EntitiesMP/Unit.es"
virtual void SetNoTargetEntity(void);
  
#line 149 "E:/USA/EntitiesMP/Unit.es"
virtual void StopandTeleport();
  
#line 150 "E:/USA/EntitiesMP/Unit.es"
virtual void MoveNow();
  
#line 152 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL CheckTarget(CEntity * penTarget);
  
#line 153 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL UseSkill(int iSkillIndex);
  
#line 156 "E:/USA/EntitiesMP/Unit.es"
virtual BOOL IsMovable();
  
#line 157 "E:/USA/EntitiesMP/Unit.es"
virtual void StopMove();
  
#line 175 "E:/USA/EntitiesMP/Unit.es"
virtual FLOAT & GetProp(FLOAT & m_fBase);
  
#line 181 "E:/USA/EntitiesMP/Unit.es"
virtual FLOAT GetThreatDistance(void);
   
#line 188 "E:/USA/EntitiesMP/Unit.es"
FLOAT3D CalcDelta(CEntity * penEntity);
   
#line 196 "E:/USA/EntitiesMP/Unit.es"
FLOAT CalcDist(CEntity * penEntity);
   
#line 202 "E:/USA/EntitiesMP/Unit.es"
FLOAT3D CalcPlaneDelta(CEntity * penEntity);
   
#line 214 "E:/USA/EntitiesMP/Unit.es"
FLOAT CalcPlaneDist(CEntity * penEntity);
   
#line 220 "E:/USA/EntitiesMP/Unit.es"
FLOAT GetFrustumAngle(const FLOAT3D & vDir);
   
#line 229 "E:/USA/EntitiesMP/Unit.es"
FLOAT GetPlaneFrustumAngle(const FLOAT3D & vDir);
   
#line 245 "E:/USA/EntitiesMP/Unit.es"
BOOL IsInFrustum(CEntity * penEntity,FLOAT fCosHalfFrustum);
   
#line 257 "E:/USA/EntitiesMP/Unit.es"
BOOL IsInPlaneFrustum(CEntity * penEntity,FLOAT fCosHalfFrustum);
   
#line 273 "E:/USA/EntitiesMP/Unit.es"
BOOL IsVisible(CEntity * penEntity);
   
#line 291 "E:/USA/EntitiesMP/Unit.es"
BOOL IsVisibleCheckAll(CEntity * penEntity);
   
#line 309 "E:/USA/EntitiesMP/Unit.es"
FLOAT CalcDistanceInPlaneToDestination(void);
  
#line 325 "E:/USA/EntitiesMP/Unit.es"
virtual ULONG SetDesiredMovementEx(void);
  
#line 480 "E:/USA/EntitiesMP/Unit.es"
virtual ULONG SetDesiredMovement(void);
  
#line 577 "E:/USA/EntitiesMP/Unit.es"
virtual ULONG SetDesiredRotate(void);
   
#line 638 "E:/USA/EntitiesMP/Unit.es"
void StopMoving();
   
#line 645 "E:/USA/EntitiesMP/Unit.es"
void StopRotating();
   
#line 651 "E:/USA/EntitiesMP/Unit.es"
void StopTranslating();
  
#line 661 "E:/USA/EntitiesMP/Unit.es"
virtual void StandingAnim(void);
  
#line 662 "E:/USA/EntitiesMP/Unit.es"
virtual void WalkingAnim(void);
  
#line 663 "E:/USA/EntitiesMP/Unit.es"
virtual void RunningAnim(void);
  
#line 664 "E:/USA/EntitiesMP/Unit.es"
virtual void RotatingAnim(void);
  
#line 665 "E:/USA/EntitiesMP/Unit.es"
virtual void AttackAnim(void);
  
#line 666 "E:/USA/EntitiesMP/Unit.es"
virtual void HungryAnim(void);
   
#line 673 "E:/USA/EntitiesMP/Unit.es"
BOOL IsValidForEnemy(CEntity * penPlayer);
#define  STATE_CUnit_Main 1
  BOOL 
#line 683 "E:/USA/EntitiesMP/Unit.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_Unit_INCLUDED
