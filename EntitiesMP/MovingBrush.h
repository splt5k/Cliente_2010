/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_MovingBrush_INCLUDED
#define _EntitiesMP_MovingBrush_INCLUDED 1
#include <EntitiesMP/MovingBrushMarker.h>
#include <EntitiesMP/SoundHolder.h>
#include <EntitiesMP\MirrorMarker.h>
#include <EntitiesMP/Debris.h>
#define EVENTCODE_EHit 0x00650000
class DECL_DLL EHit : public CEntityEvent {
public:
EHit();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EHit &e) { e = EHit(); } ;
#define EVENTCODE_EBrushDestroyed 0x00650001
class DECL_DLL EBrushDestroyed : public CEntityEvent {
public:
EBrushDestroyed();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EBrushDestroyed &e) { e = EBrushDestroyed(); } ;
#define EVENTCODE_EStartSounds 0x00650002
class DECL_DLL EStartSounds : public CEntityEvent {
public:
EStartSounds();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EStartSounds &e) { e = EStartSounds(); } ;
#define EVENTCODE_EStopSounds 0x00650003
class DECL_DLL EStopSounds : public CEntityEvent {
public:
EStopSounds();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EStopSounds &e) { e = EStopSounds(); } ;
#define EVENTCODE_EBrushDeath 0x00650004
class DECL_DLL EBrushDeath : public CEntityEvent {
public:
EBrushDeath();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
CEntityID eidInflictor;
};
DECL_DLL inline void ClearToDefault(EBrushDeath &e) { e = EBrushDeath(); } ;
extern DECL_DLL CEntityPropertyEnumType BlockAction_enum;
enum BlockAction {
  BA_NONE = 0,
  BA_BOUNCE = 1,
  BA_SKIPMARKER = 2,
};
DECL_DLL inline void ClearToDefault(BlockAction &e) { e = (BlockAction)0; } ;
extern DECL_DLL CEntityPropertyEnumType TouchOrDamageEvent_enum;
enum TouchOrDamageEvent {
  TDE_TOUCHONLY = 0,
  TDE_DAMAGEONLY = 1,
  TDE_BOTH = 2,
};
DECL_DLL inline void ClearToDefault(TouchOrDamageEvent &e) { e = (TouchOrDamageEvent)0; } ;
extern "C" DECL_DLL CDLLEntityClass CMovingBrush_DLLClass;
class CMovingBrush : public CMovableBrushEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  CTString m_strDescription;
  CEntityPointer m_penTarget;
  BOOL m_bAutoStart;
  FLOAT m_fSpeed;
  FLOAT m_fWaitTime;
  BOOL m_bMoveOnTouch;
  enum BlockAction m_ebaAction;
  FLOAT m_fBlockDamage;
  BOOL m_bPlayersOnly;
  BOOL m_bDynamicShadows;
  BOOL m_bVeryBigBrush;
  BOOL m_bStartAtFirstMarker;
  enum EventEType m_eetTouchEvent;
  CEntityPointer m_penTouchEvent;
  enum TouchOrDamageEvent m_tdeSendEventOnDamage;
  CEntityPointer m_penSwitch;
  enum EventEType m_eetMarkerEvent;
  CEntityPointer m_penMarkerEvent;
  FLOAT m_tmBankingRotation;
  BOOL m_bMoving;
  BOOL m_bRotating;
  BOOL m_bForceStop;
  BOOL m_bNoRotation;
  FLOAT3D m_vDesiredTranslation;
  ANGLE3D m_aDesiredRotation;
  BOOL m_bInverseRotate;
  BOOL m_bStopMoving;
  BOOL m_bMoveToMarker;
  BOOL m_bSkipMarker;
  BOOL m_bValidMarker;
  FLOAT m_fXLimitSign;
  FLOAT m_fYLimitSign;
  FLOAT m_fZLimitSign;
  ANGLE m_aHLimitSign;
  ANGLE m_aPLimitSign;
  ANGLE m_aBLimitSign;
  FLOAT3D m_vStartTranslation;
  ANGLE3D m_aStartRotation;
  FLOAT m_fCourseLength;
  ANGLE m_aHeadLenght;
  ANGLE m_aPitchLenght;
  ANGLE m_aBankLenght;
  CEntityPointer m_penSoundStart;
  CEntityPointer m_penSoundStop;
  CEntityPointer m_penSoundFollow;
  CSoundObject m_soStart;
  CSoundObject m_soStop;
  CSoundObject m_soFollow;
  CEntityPointer m_penMirror0;
  CEntityPointer m_penMirror1;
  CEntityPointer m_penMirror2;
  CEntityPointer m_penMirror3;
  CEntityPointer m_penMirror4;
  FLOAT m_fHealth;
  BOOL m_bBlowupByBull;
  enum EventEType m_eetBlowupEvent;
  CEntityPointer m_penBlowupEvent;
  BOOL m_bZoning;
  BOOL m_bMoveOnDamage;
  FLOAT m_fTouchDamage;
  COLOR m_colDebrises;
  INDEX m_ctDebrises;
  FLOAT m_fCandyEffect;
  FLOAT m_fCubeFactor;
  BOOL m_bBlowupByDamager;
  ULONG m_cbClassificationBits;
  ULONG m_vbVisibilityBits;
  BOOL m_bFade;
  BOOL m_bCollision;
   
#line 160 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL HandleEvent(const CEntityEvent & ee);
   
#line 190 "E:/USA/EntitiesMP/MovingBrush.es"
ULONG GetVisTweaks(void);
   
#line 195 "E:/USA/EntitiesMP/MovingBrush.es"
void Precache(void);
   
#line 202 "E:/USA/EntitiesMP/MovingBrush.es"
void GetForce(INDEX iForce,const FLOAT3D & vPoint,
#line 203 "E:/USA/EntitiesMP/MovingBrush.es"
CForceStrength & fsGravity,CForceStrength & fsField);
   
#line 208 "E:/USA/EntitiesMP/MovingBrush.es"
void ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 209 "E:/USA/EntitiesMP/MovingBrush.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection);
   
#line 265 "E:/USA/EntitiesMP/MovingBrush.es"
void AdjustAngle(ANGLE & a);
   
#line 274 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 279 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 284 "E:/USA/EntitiesMP/MovingBrush.es"
const CTString & GetDescription(void)const;
   
#line 292 "E:/USA/EntitiesMP/MovingBrush.es"
const CTString & GetMirrorName(INDEX iMirror);
   
#line 326 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL GetMirror(INDEX iMirror,class CMirrorParameters & mpMirror);
   
#line 348 "E:/USA/EntitiesMP/MovingBrush.es"
void PreMoving();
   
#line 440 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL LoadMarkerParameters();
   
#line 513 "E:/USA/EntitiesMP/MovingBrush.es"
BOOL CanReactOnEntity(CEntity * pen);
   
#line 531 "E:/USA/EntitiesMP/MovingBrush.es"
void PlayStartSound(void);
   
#line 541 "E:/USA/EntitiesMP/MovingBrush.es"
void PlayStopSound(void);
   
#line 551 "E:/USA/EntitiesMP/MovingBrush.es"
void PlayFollowSound(void);
   
#line 561 "E:/USA/EntitiesMP/MovingBrush.es"
void StopFollowSound(void);
   
#line 566 "E:/USA/EntitiesMP/MovingBrush.es"
void MovingOn(void);
   
#line 576 "E:/USA/EntitiesMP/MovingBrush.es"
void MovingOff(void);
   
#line 587 "E:/USA/EntitiesMP/MovingBrush.es"
void MaybeActivateRotation(void);
   
#line 599 "E:/USA/EntitiesMP/MovingBrush.es"
void DeactivateRotation(void);
   
#line 605 "E:/USA/EntitiesMP/MovingBrush.es"
void SetCombinedRotation(ANGLE3D aRotAngle,ANGLE3D aAddAngle);
   
#line 614 "E:/USA/EntitiesMP/MovingBrush.es"
SLONG GetUsedMemory(void);
#define  STATE_CMovingBrush_MoveToMarker 0x00650005
  BOOL 
#line 630 "E:/USA/EntitiesMP/MovingBrush.es"
MoveToMarker(const CEntityEvent &__eeInput);
  BOOL H0x00650006_MoveToMarker_01(const CEntityEvent &__eeInput);
  BOOL H0x00650007_MoveToMarker_02(const CEntityEvent &__eeInput);
#define  STATE_CMovingBrush_BounceObstructed 0x00650008
  BOOL 
#line 715 "E:/USA/EntitiesMP/MovingBrush.es"
BounceObstructed(const CEntityEvent &__eeInput);
  BOOL H0x00650009_BounceObstructed_01(const CEntityEvent &__eeInput);
  BOOL H0x0065000a_BounceObstructed_02(const CEntityEvent &__eeInput);
#define  STATE_CMovingBrush_MoveBrush 0x0065000b
  BOOL 
#line 764 "E:/USA/EntitiesMP/MovingBrush.es"
MoveBrush(const CEntityEvent &__eeInput);
  BOOL H0x0065000c_MoveBrush_01(const CEntityEvent &__eeInput);
  BOOL H0x0065000d_MoveBrush_02(const CEntityEvent &__eeInput);
  BOOL H0x0065000e_MoveBrush_03(const CEntityEvent &__eeInput);
  BOOL H0x0065000f_MoveBrush_04(const CEntityEvent &__eeInput);
  BOOL H0x00650010_MoveBrush_05(const CEntityEvent &__eeInput);
  BOOL H0x00650011_MoveBrush_06(const CEntityEvent &__eeInput);
  BOOL H0x00650012_MoveBrush_07(const CEntityEvent &__eeInput);
  BOOL H0x00650013_MoveBrush_08(const CEntityEvent &__eeInput);
  BOOL H0x00650014_MoveBrush_09(const CEntityEvent &__eeInput);
  BOOL H0x00650015_MoveBrush_10(const CEntityEvent &__eeInput);
#define  STATE_CMovingBrush_TeleportToStopMarker 0x00650016
  BOOL 
#line 851 "E:/USA/EntitiesMP/MovingBrush.es"
TeleportToStopMarker(const CEntityEvent &__eeInput);
#define  STATE_CMovingBrush_Main 1
  BOOL 
#line 879 "E:/USA/EntitiesMP/MovingBrush.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00650017_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00650018_Main_02(const CEntityEvent &__eeInput);
  BOOL H0x00650019_Main_03(const CEntityEvent &__eeInput);
  BOOL H0x0065001a_Main_04(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_MovingBrush_INCLUDED
