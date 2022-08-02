/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_MirrorMarker_INCLUDED
#define _EntitiesMP_MirrorMarker_INCLUDED 1
#include <EntitiesMP/Marker.h>
extern DECL_DLL CEntityPropertyEnumType WarpRotation_enum;
enum WarpRotation {
  WR_NONE = 0,
  WR_BANKING = 1,
  WR_TWIRLING = 2,
};
DECL_DLL inline void ClearToDefault(WarpRotation &e) { e = (WarpRotation)0; } ;
extern "C" DECL_DLL CDLLEntityClass CMirrorMarker_DLLClass;
class CMirrorMarker : public CMarker {
public:
virtual BOOL IsImportant(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  enum WarpRotation m_wrRotation;
  FLOAT m_fRotationSpeed;
   
#line 32 "E:/USA/EntitiesMP/MirrorMarker.es"
const CTString & GetMirrorName(void);
   
#line 37 "E:/USA/EntitiesMP/MirrorMarker.es"
void GetMirror(class CMirrorParameters & mpMirror);
#define  STATE_CMirrorMarker_Main 1
  BOOL 
#line 62 "E:/USA/EntitiesMP/MirrorMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_MirrorMarker_INCLUDED
