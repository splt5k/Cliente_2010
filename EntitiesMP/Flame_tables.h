/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EFlame_New(void) { return new EFlame; };
CDLLEntityEvent DLLEvent_EFlame = {
  0x01f80000, &EFlame_New
};
CEntityEvent *EStopFlaming_New(void) { return new EStopFlaming; };
CDLLEntityEvent DLLEvent_EStopFlaming = {
  0x01f80001, &EStopFlaming_New
};
CDLLEntityEvent *CFlame_events[] = {
  &DLLEvent_EStopFlaming,
  &DLLEvent_EFlame,
};
#define CFlame_eventsct ARRAYCOUNT(CFlame_events)
#define ENTITYCLASS CFlame

CEntityProperty CFlame_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000001f8<<8)+1, offsetof(CFlame, m_penOwner), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000001f8<<8)+2, offsetof(CFlame, m_penAttach), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000001f8<<8)+5, offsetof(CFlame, m_bLoop), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+8, offsetof(CFlame, m_vHitPoint), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_SOUNDOBJECT, NULL, (0x000001f8<<8)+10, offsetof(CFlame, m_soEffect), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+20, offsetof(CFlame, m_tmStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+21, offsetof(CFlame, m_fDamageToApply), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+22, offsetof(CFlame, m_fDamageStep), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+23, offsetof(CFlame, m_fAppliedDamage), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+24, offsetof(CFlame, m_tmFirstStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000001f8<<8)+29, offsetof(CFlame, m_ctFlames), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+30, offsetof(CFlame, m_vPos01), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+31, offsetof(CFlame, m_vPos02), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+32, offsetof(CFlame, m_vPos03), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+33, offsetof(CFlame, m_vPos04), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+34, offsetof(CFlame, m_vPos05), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+35, offsetof(CFlame, m_vPos06), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+36, offsetof(CFlame, m_vPos07), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+37, offsetof(CFlame, m_vPos08), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+38, offsetof(CFlame, m_vPos09), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+39, offsetof(CFlame, m_vPos10), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f8<<8)+40, offsetof(CFlame, m_vPlaneNormal), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000001f8<<8)+51, offsetof(CFlame, m_bBurningBrush), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f8<<8)+52, offsetof(CFlame, m_tmDeathParticlesStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000001f8<<8)+255, offsetof(CFlame, m_penPrediction), "", 0, 0, 0),
};
#define CFlame_propertiesct ARRAYCOUNT(CFlame_properties)

CEntityComponent CFlame_components[] = {
#define CLASS_LIGHT ((0x000001f8<<8)+1)
 CEntityComponent(ECT_CLASS, CLASS_LIGHT, 0, "EFNM" "Classes\\Light.ecl"),
#define MODEL_FLAME ((0x000001f8<<8)+10)
 CEntityComponent(ECT_MODEL, MODEL_FLAME, 0, "EFNM" "Data\\ModelsMP\\Effects\\Flame\\Flame.mdl"),
#define TEXTURE_FLAME ((0x000001f8<<8)+11)
 CEntityComponent(ECT_TEXTURE, TEXTURE_FLAME, 0, "EFNM" "Data\\ModelsMP\\Effects\\Flame\\Flame.tex"),
#define SOUND_FLAME ((0x000001f8<<8)+12)
 CEntityComponent(ECT_SOUND, SOUND_FLAME, 0, "EFNM" "data\\sounds\\Default.wav"),
};
#define CFlame_componentsct ARRAYCOUNT(CFlame_components)

CEventHandlerEntry CFlame_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CFlame::
#line 217 "E:/USA/EntitiesMP/Flame.es"
Main),DEBUGSTRING("CFlame::Main")},
 {0x01f80002, -1, CEntity::pEventHandler(&CFlame::H0x01f80002_Main_01), DEBUGSTRING("CFlame::H0x01f80002_Main_01")},
 {0x01f80003, -1, CEntity::pEventHandler(&CFlame::H0x01f80003_Main_02), DEBUGSTRING("CFlame::H0x01f80003_Main_02")},
 {0x01f80004, -1, CEntity::pEventHandler(&CFlame::H0x01f80004_Main_03), DEBUGSTRING("CFlame::H0x01f80004_Main_03")},
 {0x01f80005, -1, CEntity::pEventHandler(&CFlame::H0x01f80005_Main_04), DEBUGSTRING("CFlame::H0x01f80005_Main_04")},
};
#define CFlame_handlersct ARRAYCOUNT(CFlame_handlers)

CEntity *CFlame_New(void) { return new CFlame; };
void CFlame_OnInitClass(void) {};
void CFlame_OnEndClass(void) {};
void CFlame_OnPrecache(CDLLEntityClass *pdec, INDEX iUser);
void CFlame_OnWorldEnd(CWorld *pwo) {};
void CFlame_OnWorldInit(CWorld *pwo) {};
void CFlame_OnWorldTick(CWorld *pwo) {};
void CFlame_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CFlame, CMovableModelEntity, "Flame", "", 0x000001f8);
DECLARE_CTFILENAME(_fnmCFlame_tbn, "");
