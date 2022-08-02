/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_BlendController_INCLUDED
#define _EntitiesMP_BlendController_INCLUDED 1
#include <EntitiesMP/Marker.h>
extern DECL_DLL CEntityPropertyEnumType BlendControllerType_enum;
enum BlendControllerType {
  BCT_NONE = 0,
  BCT_PYRAMID_PLATES = 1,
  BCT_ACTIVATE_PLATE_1 = 2,
  BCT_ACTIVATE_PLATE_2 = 3,
  BCT_ACTIVATE_PLATE_3 = 4,
  BCT_ACTIVATE_PLATE_4 = 5,
  BCT_ACTIVATE_PYRAMID_MORPH_ROOM = 6,
};
DECL_DLL inline void ClearToDefault(BlendControllerType &e) { e = (BlendControllerType)0; } ;
#define EVENTCODE_EActivateBlend 0x02640000
class DECL_DLL EActivateBlend : public CEntityEvent {
public:
EActivateBlend();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EActivateBlend &e) { e = EActivateBlend(); } ;
#define EVENTCODE_EDeactivateBlend 0x02640001
class DECL_DLL EDeactivateBlend : public CEntityEvent {
public:
EDeactivateBlend();
CEntityEvent *MakeCopy(void);
BOOL CheckIDs(void);
SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EDeactivateBlend &e) { e = EDeactivateBlend(); } ;
extern "C" DECL_DLL CDLLEntityClass CBlendController_DLLClass;
class CBlendController : public CMarker {
public:
virtual BOOL IsImportant(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  enum BlendControllerType m_bctType;
   
#line 44 "E:/USA/EntitiesMP/BlendController.es"
BOOL HandleEvent(const CEntityEvent & ee);
#define  STATE_CBlendController_Main 1
  BOOL 
#line 115 "E:/USA/EntitiesMP/BlendController.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_BlendController_INCLUDED
