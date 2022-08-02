/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

EP_ENUMBEG(EffectorEffectType)
  EP_ENUMVALUE(ET_NONE, "None"),
  EP_ENUMVALUE(ET_DESTROY_OBELISK, "Destroy obelisk"),
  EP_ENUMVALUE(ET_DESTROY_PYLON, "Destroy pylon"),
  EP_ENUMVALUE(ET_HIT_GROUND, "Hit ground"),
  EP_ENUMVALUE(ET_LIGHTNING, "Lightning"),
  EP_ENUMVALUE(ET_SIZING_BIG_BLUE_FLARE, "Sizing big blue flare"),
  EP_ENUMVALUE(ET_SIZING_RING_FLARE, "Sizing ring flare"),
  EP_ENUMVALUE(ET_MOVING_RING, "Moving ring"),
  EP_ENUMVALUE(ET_PORTAL_LIGHTNING, "Portal lightnings"),
  EP_ENUMVALUE(ET_MORPH_MODELS, "Morph two models"),
  EP_ENUMVALUE(ET_DISAPPEAR_MODEL, "Disappear model"),
  EP_ENUMVALUE(ET_APPEAR_MODEL, "Appear model"),
  EP_ENUMVALUE(ET_DISAPPEAR_MODEL_NOW, "Disappear model now"),
  EP_ENUMVALUE(ET_APPEAR_MODEL_NOW, "Appear model now"),
EP_ENUMEND(EffectorEffectType);

CEntityEvent *ESpawnEffector_New(void) { return new ESpawnEffector; };
CDLLEntityEvent DLLEvent_ESpawnEffector = {
  0x02600000, &ESpawnEffector_New
};
CEntityEvent *ETriggerEffector_New(void) { return new ETriggerEffector; };
CDLLEntityEvent DLLEvent_ETriggerEffector = {
  0x02600001, &ETriggerEffector_New
};
CDLLEntityEvent *CEffector_events[] = {
  &DLLEvent_ETriggerEffector,
  &DLLEvent_ESpawnEffector,
};
#define CEffector_eventsct ARRAYCOUNT(CEffector_events)
#define ENTITYCLASS CEffector

CEntityProperty CEffector_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENUM, &EffectorEffectType_enum, (0x00000260<<8)+1, offsetof(CEffector, m_eetType), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000260<<8)+2, offsetof(CEffector, m_tmStarted), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000260<<8)+3, offsetof(CEffector, m_vDamageDir), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000260<<8)+4, offsetof(CEffector, m_vFXDestination), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000260<<8)+5, offsetof(CEffector, m_tmLifeTime), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000260<<8)+6, offsetof(CEffector, m_fSize), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000260<<8)+8, offsetof(CEffector, m_ctCount), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000260<<8)+10, offsetof(CEffector, m_bLightSource), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ANIMOBJECT, NULL, (0x00000260<<8)+11, offsetof(CEffector, m_aoLightAnimation), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000260<<8)+12, offsetof(CEffector, m_iLightAnimation), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000260<<8)+13, offsetof(CEffector, m_bAlive), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000260<<8)+14, offsetof(CEffector, m_penModel), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000260<<8)+15, offsetof(CEffector, m_penModel2), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000260<<8)+16, offsetof(CEffector, m_bWaitTrigger), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000260<<8)+20, offsetof(CEffector, m_iModel1ID), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000260<<8)+21, offsetof(CEffector, m_iModel2ID), "", 0, 0, EPROPF_NETSEND ),
};
#define CEffector_propertiesct ARRAYCOUNT(CEffector_properties)

CEntityComponent CEffector_components[] = {
#define MODEL_MARKER ((0x00000260<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\Axis.mdl"),
#define TEXTURE_MARKER ((0x00000260<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\Vector.tex"),
#define MODEL_POWER_RING ((0x00000260<<8)+3)
 CEntityComponent(ECT_MODEL, MODEL_POWER_RING, 0, "EFNM" "Data\\Models\\CutSequences\\SpaceShip\\PowerRing.mdl"),
#define TEXTURE_POWER_RING ((0x00000260<<8)+4)
 CEntityComponent(ECT_TEXTURE, TEXTURE_POWER_RING, 0, "EFNM" "Data\\Models\\CutSequences\\SpaceShip\\PowerRing.tex"),
};
#define CEffector_componentsct ARRAYCOUNT(CEffector_components)

CEventHandlerEntry CEffector_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CEffector::
#line 406 "E:/USA/EntitiesMP/Effector.es"
Main),DEBUGSTRING("CEffector::Main")},
 {0x02600002, -1, CEntity::pEventHandler(&CEffector::H0x02600002_Main_01), DEBUGSTRING("CEffector::H0x02600002_Main_01")},
 {0x02600003, -1, CEntity::pEventHandler(&CEffector::H0x02600003_Main_02), DEBUGSTRING("CEffector::H0x02600003_Main_02")},
 {0x02600004, -1, CEntity::pEventHandler(&CEffector::H0x02600004_Main_03), DEBUGSTRING("CEffector::H0x02600004_Main_03")},
 {0x02600005, -1, CEntity::pEventHandler(&CEffector::H0x02600005_Main_04), DEBUGSTRING("CEffector::H0x02600005_Main_04")},
 {0x02600006, -1, CEntity::pEventHandler(&CEffector::H0x02600006_Main_05), DEBUGSTRING("CEffector::H0x02600006_Main_05")},
 {0x02600007, -1, CEntity::pEventHandler(&CEffector::H0x02600007_Main_06), DEBUGSTRING("CEffector::H0x02600007_Main_06")},
};
#define CEffector_handlersct ARRAYCOUNT(CEffector_handlers)

CEntity *CEffector_New(void) { return new CEffector; };
void CEffector_OnInitClass(void) {};
void CEffector_OnEndClass(void) {};
void CEffector_OnPrecache(CDLLEntityClass *pdec, INDEX iUser);
void CEffector_OnWorldEnd(CWorld *pwo) {};
void CEffector_OnWorldInit(CWorld *pwo) {};
void CEffector_OnWorldTick(CWorld *pwo) {};
void CEffector_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CEffector, CMovableModelEntity, "Effector", "", 0x00000260);
DECLARE_CTFILENAME(_fnmCEffector_tbn, "");
