/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CWaterHolderMarker_events[] = {NULL};
#define CWaterHolderMarker_eventsct 0
#define ENTITYCLASS CWaterHolderMarker

CEntityProperty CWaterHolderMarker_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x00000781<<8)+1, offsetof(CWaterHolderMarker, m_strName), " Name", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000781<<8)+4, offsetof(CWaterHolderMarker, m_iGridX), "Row", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000781<<8)+5, offsetof(CWaterHolderMarker, m_iGridY), "Col", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000781<<8)+6, offsetof(CWaterHolderMarker, m_fWidthSize), "Tile Width", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000781<<8)+7, offsetof(CWaterHolderMarker, m_fHeightSize), "Tile Height", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000781<<8)+8, offsetof(CWaterHolderMarker, m_bGenerate), "Generate", 0, 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000781<<8)+20, offsetof(CWaterHolderMarker, m_iStartID), "Start ID", 0, 0x7F0000FFUL, 0),
};
#define CWaterHolderMarker_propertiesct ARRAYCOUNT(CWaterHolderMarker_properties)

CEntityComponent CWaterHolderMarker_components[] = {
#define MODEL_AREAMARKER ((0x00000781<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_AREAMARKER, 2, "EFNM" "Data\\Models\\Editor\\Axis.mdl"),
#define TEXTURE_AREAMARKER ((0x00000781<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_AREAMARKER, 2, "EFNM" "Data\\Models\\Editor\\Vector.tex"),
};
#define CWaterHolderMarker_componentsct ARRAYCOUNT(CWaterHolderMarker_components)

CEventHandlerEntry CWaterHolderMarker_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CWaterHolderMarker::
#line 70 "E:/USA/EntitiesMP/WaterHolderMarker.es"
Main),DEBUGSTRING("CWaterHolderMarker::Main")},
};
#define CWaterHolderMarker_handlersct ARRAYCOUNT(CWaterHolderMarker_handlers)

CEntity *CWaterHolderMarker_New(void) { return new CWaterHolderMarker; };
void CWaterHolderMarker_OnInitClass(void) {};
void CWaterHolderMarker_OnEndClass(void) {};
void CWaterHolderMarker_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CWaterHolderMarker_OnWorldEnd(CWorld *pwo) {};
void CWaterHolderMarker_OnWorldInit(CWorld *pwo) {};
void CWaterHolderMarker_OnWorldTick(CWorld *pwo) {};
void CWaterHolderMarker_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CWaterHolderMarker, CMarker, "WaterHolderMarker", "Thumbnails\\Terrain.tbn", 0x00000781);
DECLARE_CTFILENAME(_fnmCWaterHolderMarker_tbn, "Thumbnails\\Terrain.tbn");
