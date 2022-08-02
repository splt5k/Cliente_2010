/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_ParticleCloudsMarker_INCLUDED
#define _EntitiesMP_ParticleCloudsMarker_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CParticleCloudsMarker_DLLClass;
class CParticleCloudsMarker : public CEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual const CTString &GetDescription(void) const { return m_strDescription; };
virtual BOOL IsTargetable(void) const { return TRUE; };
virtual BOOL IsImportant(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strDescription;
  CTString m_strName;
  INDEX m_ctCount;
  FLOAT m_fRadiusMin;
  FLOAT m_fRadiusMax;
  FLOAT m_fPitchMin;
  FLOAT m_fPitchMax;
  FLOAT m_fSize;
  FLOAT m_fRotPeriod;
  FLOAT m_fFramePeriod;
  FLOAT m_fExtraCurvature;
  FLOAT m_fRndSizingRatio;
  COLOR m_colMultiply1;
  COLOR m_colMultiply2;
  INDEX m_iFrameStart;
  INDEX m_iFrameEnd;
  BOOL m_bVisible;
  CEntityPointer m_penNextParticleCloudsMarker;
  INDEX m_iRndBase;
  CEntityPointer m_penSun;
  COLOR m_colStreaksMin;
  COLOR m_colStreaksMax;
  FLOAT m_fStreaksConePercentage;
  FLOAT m_fStreakSize;
  FLOAT m_fRndStreakSize;
  BOOL m_bRandomize;
  FLOAT m_fStreaksRndMovementRatio;
  INDEX m_ctStreaksPerCloud;
  FLOAT m_fStreakPulsatingRatio;
  FLOAT m_fStreakPulsatingPeroid;
  BOOL m_bRenderClouds;
#define  STATE_CParticleCloudsMarker_Main 1
  BOOL 
#line 54 "E:/USA/EntitiesMP/ParticleCloudsMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_ParticleCloudsMarker_INCLUDED