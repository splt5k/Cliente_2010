/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EWatcherInit_New(void) { return new EWatcherInit; };
CDLLEntityEvent DLLEvent_EWatcherInit = {
  0x02bc0000, &EWatcherInit_New
};
CEntityEvent *EWatch_New(void) { return new EWatch; };
CDLLEntityEvent DLLEvent_EWatch = {
  0x02bc0001, &EWatch_New
};
CDLLEntityEvent *CWatcher_events[] = {
  &DLLEvent_EWatch,
  &DLLEvent_EWatcherInit,
};
#define CWatcher_eventsct ARRAYCOUNT(CWatcher_events)
#define ENTITYCLASS CWatcher

CEntityProperty CWatcher_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000002bc<<8)+1, offsetof(CWatcher, m_penOwner), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000002bc<<8)+2, offsetof(CWatcher, m_tmDelay), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000002bc<<8)+20, offsetof(CWatcher, m_fClosestPlayer), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000002bc<<8)+21, offsetof(CWatcher, m_iPlayerToCheck), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000002bc<<8)+31, offsetof(CWatcher, m_iOwnerID), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000002bc<<8)+255, offsetof(CWatcher, m_penPrediction), "", 0, 0, 0),
};
#define CWatcher_propertiesct ARRAYCOUNT(CWatcher_properties)

CEntityComponent CWatcher_components[] = {
  CEntityComponent()
};
#define CWatcher_componentsct 0


CEventHandlerEntry CWatcher_handlers[] = {
 {0x02bc0002, -1, CEntity::pEventHandler(&CWatcher::
#line 443 "E:/USA/EntitiesMP/Watcher.es"
Active),DEBUGSTRING("CWatcher::Active")},
 {0x02bc0003, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0003_Active_01), DEBUGSTRING("CWatcher::H0x02bc0003_Active_01")},
 {0x02bc0004, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0004_Active_02), DEBUGSTRING("CWatcher::H0x02bc0004_Active_02")},
 {0x02bc0005, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0005_Active_03), DEBUGSTRING("CWatcher::H0x02bc0005_Active_03")},
 {0x02bc0006, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0006_Active_04), DEBUGSTRING("CWatcher::H0x02bc0006_Active_04")},
 {0x02bc0007, -1, CEntity::pEventHandler(&CWatcher::
#line 465 "E:/USA/EntitiesMP/Watcher.es"
Inactive),DEBUGSTRING("CWatcher::Inactive")},
 {0x02bc0008, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0008_Inactive_01), DEBUGSTRING("CWatcher::H0x02bc0008_Inactive_01")},
 {0x02bc0009, -1, CEntity::pEventHandler(&CWatcher::H0x02bc0009_Inactive_02), DEBUGSTRING("CWatcher::H0x02bc0009_Inactive_02")},
 {0x02bc000a, -1, CEntity::pEventHandler(&CWatcher::
#line 475 "E:/USA/EntitiesMP/Watcher.es"
Dummy),DEBUGSTRING("CWatcher::Dummy")},
 {0x02bc000b, -1, CEntity::pEventHandler(&CWatcher::H0x02bc000b_Dummy_01), DEBUGSTRING("CWatcher::H0x02bc000b_Dummy_01")},
 {0x02bc000c, -1, CEntity::pEventHandler(&CWatcher::H0x02bc000c_Dummy_02), DEBUGSTRING("CWatcher::H0x02bc000c_Dummy_02")},
 {1, -1, CEntity::pEventHandler(&CWatcher::
#line 484 "E:/USA/EntitiesMP/Watcher.es"
Main),DEBUGSTRING("CWatcher::Main")},
 {0x02bc000d, -1, CEntity::pEventHandler(&CWatcher::H0x02bc000d_Main_01), DEBUGSTRING("CWatcher::H0x02bc000d_Main_01")},
 {0x02bc000e, -1, CEntity::pEventHandler(&CWatcher::H0x02bc000e_Main_02), DEBUGSTRING("CWatcher::H0x02bc000e_Main_02")},
};
#define CWatcher_handlersct ARRAYCOUNT(CWatcher_handlers)

CEntity *CWatcher_New(void) { return new CWatcher; };
void CWatcher_OnInitClass(void) {};
void CWatcher_OnEndClass(void) {};
void CWatcher_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CWatcher_OnWorldEnd(CWorld *pwo) {};
void CWatcher_OnWorldInit(CWorld *pwo) {};
void CWatcher_OnWorldTick(CWorld *pwo) {};
void CWatcher_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CWatcher, CRationalEntity, "Watcher", "", 0x000002bc);
DECLARE_CTFILENAME(_fnmCWatcher_tbn, "");
