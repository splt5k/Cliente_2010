/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

EP_ENUMBEG(TouchFieldType)
  EP_ENUMVALUE(TOUCHFIELD_DEFAULT, "Default TouchField"),
  EP_ENUMVALUE(TOUCHFIELD_ITEMCHECK, "Item Check TouchField"),
  EP_ENUMVALUE(TOUCHFIELD_QUESTCOMPLETE, "Quest Complete TouchField"),
  EP_ENUMVALUE(TOUCHFIELD_CHECKPOINT, "Quest Check Point"),
EP_ENUMEND(TouchFieldType);

CDLLEntityEvent *CTouchField_events[] = {NULL};
#define CTouchField_eventsct 0
#define ENTITYCLASS CTouchField

CEntityProperty CTouchField_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ce<<8)+1, offsetof(CTouchField, m_strName), "Name", 'N', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ce<<8)+2, offsetof(CTouchField, m_penEnter), "Enter Target", 'T', C_BROWN  | 0xFF, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &EventEType_enum, (0x000000ce<<8)+3, offsetof(CTouchField, m_eetEnter), "Enter Event", 'E', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ce<<8)+7, offsetof(CTouchField, m_penExit), "Exit Target", 0, C_dRED  | 0xFF, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &EventEType_enum, (0x000000ce<<8)+8, offsetof(CTouchField, m_eetExit), "Exit Event", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000ce<<8)+4, offsetof(CTouchField, m_bActive), "Active", 'A', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000ce<<8)+5, offsetof(CTouchField, m_bPlayersOnly), "Players only", 'P', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ce<<8)+6, offsetof(CTouchField, m_tmExitCheck), "Exit check time", 'X', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000ce<<8)+9, offsetof(CTouchField, m_bBlockNonPlayers), "Block non-players", 'B', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ce<<8)+100, offsetof(CTouchField, m_penLastIn), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ce<<8)+110, offsetof(CTouchField, m_penTarget), "Target Only", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &TouchFieldType_enum, (0x000000ce<<8)+120, offsetof(CTouchField, eType), "TouchField Type", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000000ce<<8)+125, offsetof(CTouchField, m_iCheckItemIndex), "CheckItem Index", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000000ce<<8)+126, offsetof(CTouchField, m_iQuestIndex), "Quest Index(hardcoding)", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000000ce<<8)+130, offsetof(CTouchField, m_iCheckPointFlag), "CheckPoint Flag", 0, 0x7F0000FFUL, 0),
};
#define CTouchField_propertiesct ARRAYCOUNT(CTouchField_properties)

CEntityComponent CTouchField_components[] = {
#define TEXTURE_FIELD ((0x000000ce<<8)+1)
 CEntityComponent(ECT_TEXTURE, TEXTURE_FIELD, 2, "EFNM" "Data\\Models\\Editor\\CollisionBox.tex"),
};
#define CTouchField_componentsct ARRAYCOUNT(CTouchField_components)

CEventHandlerEntry CTouchField_handlers[] = {
 {0x00ce0000, -1, CEntity::pEventHandler(&CTouchField::
#line 165 "E:/USA/EntitiesMP/TouchField.es"
WaitingEntry),DEBUGSTRING("CTouchField::WaitingEntry")},
 {0x00ce0001, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0001_WaitingEntry_01), DEBUGSTRING("CTouchField::H0x00ce0001_WaitingEntry_01")},
 {0x00ce0002, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0002_WaitingEntry_02), DEBUGSTRING("CTouchField::H0x00ce0002_WaitingEntry_02")},
 {0x00ce0003, -1, CEntity::pEventHandler(&CTouchField::
#line 298 "E:/USA/EntitiesMP/TouchField.es"
WaitingExit),DEBUGSTRING("CTouchField::WaitingExit")},
 {0x00ce0004, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0004_WaitingExit_01), DEBUGSTRING("CTouchField::H0x00ce0004_WaitingExit_01")},
 {0x00ce0005, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0005_WaitingExit_02), DEBUGSTRING("CTouchField::H0x00ce0005_WaitingExit_02")},
 {0x00ce0006, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0006_WaitingExit_03), DEBUGSTRING("CTouchField::H0x00ce0006_WaitingExit_03")},
 {0x00ce0007, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0007_WaitingExit_04), DEBUGSTRING("CTouchField::H0x00ce0007_WaitingExit_04")},
 {0x00ce0008, -1, CEntity::pEventHandler(&CTouchField::
#line 336 "E:/USA/EntitiesMP/TouchField.es"
Frozen),DEBUGSTRING("CTouchField::Frozen")},
 {0x00ce0009, -1, CEntity::pEventHandler(&CTouchField::H0x00ce0009_Frozen_01), DEBUGSTRING("CTouchField::H0x00ce0009_Frozen_01")},
 {0x00ce000a, -1, CEntity::pEventHandler(&CTouchField::H0x00ce000a_Frozen_02), DEBUGSTRING("CTouchField::H0x00ce000a_Frozen_02")},
 {1, -1, CEntity::pEventHandler(&CTouchField::
#line 351 "E:/USA/EntitiesMP/TouchField.es"
Main),DEBUGSTRING("CTouchField::Main")},
};
#define CTouchField_handlersct ARRAYCOUNT(CTouchField_handlers)

CEntity *CTouchField_New(void) { return new CTouchField; };
void CTouchField_OnInitClass(void) {};
void CTouchField_OnEndClass(void) {};
void CTouchField_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CTouchField_OnWorldEnd(CWorld *pwo) {};
void CTouchField_OnWorldInit(CWorld *pwo) {};
void CTouchField_OnWorldTick(CWorld *pwo) {};
void CTouchField_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CTouchField, CRationalEntity, "Touch Field", "Thumbnails\\TouchField.tbn", 0x000000ce);
DECLARE_CTFILENAME(_fnmCTouchField_tbn, "Thumbnails\\TouchField.tbn");