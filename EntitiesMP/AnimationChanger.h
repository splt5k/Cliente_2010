/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_AnimationChanger_INCLUDED
#define _EntitiesMP_AnimationChanger_INCLUDED 1
#include <EntitiesMP/ModelHolder2.h>
#include <EntitiesMP/Light.h>
extern DECL_DLL CEntityPropertyEnumType ChangeAnimType_enum;
enum ChangeAnimType {
  ACTION_NONE = 0,
  ACTION_SKILL = 1,
  ACTION_DISAPPEAR = 2,
  ACTION_APPEAR = 3,
};
DECL_DLL inline void ClearToDefault(ChangeAnimType &e) { e = (ChangeAnimType)0; } ;
#define EVENTCODE_EChangeAnim 0x00da0000
class DECL_DLL EChangeAnim : public CEntityEvent {
public:
EChangeAnim();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
INDEX iModelAnim;
BOOL bModelLoop;
INDEX iTextureAnim;
BOOL bTextureLoop;
INDEX iLightAnim;
INDEX iAmbientLightAnim;
BOOL bLightLoop;
BOOL bAmbientLightLoop;
COLOR colAmbient;
COLOR colDiffuse;
CTString strAnimation;
FLOAT fBlendTime;
ChangeAnimType AnimType;
};
DECL_DLL inline void ClearToDefault(EChangeAnim &e) { e = EChangeAnim(); } ;
extern "C" DECL_DLL CDLLEntityClass CAnimationChanger_DLLClass;
class CAnimationChanger : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual CEntity *GetTarget(void) const { return m_penTarget; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  CTString m_strDescription;
  CEntityPointer m_penTarget;
  ANIMATION m_iModelAnim;
  BOOL m_bModelLoop;
  ANIMATION m_iTextureAnim;
  BOOL m_bTextureLoop;
  ANIMATION m_iLightAnim;
  BOOL m_bLightLoop;
  ANIMATION m_iAmbientLightAnim;
  BOOL m_bAmbientLightLoop;
  COLOR m_colAmbient;
  COLOR m_colDiffuse;
  CTString m_strModelAnim;
  FLOAT m_fBlendTime;
   
#line 64 "D:/LC_2009_Project/lastchaos-source-client-2009/EntitiesMP/AnimationChanger.es"
const CTString & GetDescription(void)const;
   
#line 73 "D:/LC_2009_Project/lastchaos-source-client-2009/EntitiesMP/AnimationChanger.es"
CAnimData * GetAnimData(SLONG slPropertyOffset);
#define  STATE_CAnimationChanger_Main 1
  BOOL 
#line 118 "D:/LC_2009_Project/lastchaos-source-client-2009/EntitiesMP/AnimationChanger.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x00da0001_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x00da0002_Main_02(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_AnimationChanger_INCLUDED
