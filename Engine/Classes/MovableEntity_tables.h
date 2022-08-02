/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CMovableEntity_events[] = {NULL};
#define CMovableEntity_eventsct 0
#define ENTITYCLASS CMovableEntity

CEntityProperty CMovableEntity_properties[] = {
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+1, offsetof(CMovableEntity, en_vDesiredTranslationRelative), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ANGLE3D, NULL, (0x00000001<<8)+2, offsetof(CMovableEntity, en_aDesiredRotationRelative), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+3, offsetof(CMovableEntity, en_vCurrentTranslationAbsolute), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ANGLE3D, NULL, (0x00000001<<8)+4, offsetof(CMovableEntity, en_aCurrentRotationAbsolute), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000001<<8)+6, offsetof(CMovableEntity, en_penReference), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+7, offsetof(CMovableEntity, en_vReferencePlane), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000001<<8)+8, offsetof(CMovableEntity, en_iReferenceSurface), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000001<<8)+9, offsetof(CMovableEntity, en_penLastValidReference), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000001<<8)+15, offsetof(CMovableEntity, en_penLastValidReferenceTmp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+14, offsetof(CMovableEntity, en_tmLastSignificantVerticalMovement), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+10, offsetof(CMovableEntity, en_tmLastBreathed), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+11, offsetof(CMovableEntity, en_tmMaxHoldBreath), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+12, offsetof(CMovableEntity, en_fDensity), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+13, offsetof(CMovableEntity, en_tmLastSwimDamage), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000001<<8)+20, offsetof(CMovableEntity, en_iUpContent), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000001<<8)+21, offsetof(CMovableEntity, en_iDnContent), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+22, offsetof(CMovableEntity, en_fImmersionFactor), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+25, offsetof(CMovableEntity, en_vGravityDir), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+26, offsetof(CMovableEntity, en_fGravityA), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+27, offsetof(CMovableEntity, en_fGravityV), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+66, offsetof(CMovableEntity, en_vForceDir), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+67, offsetof(CMovableEntity, en_fForceA), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+68, offsetof(CMovableEntity, en_fForceV), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+30, offsetof(CMovableEntity, en_tmJumped), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+31, offsetof(CMovableEntity, en_tmMaxJumpControl), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+32, offsetof(CMovableEntity, en_fJumpControlMultiplier), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+35, offsetof(CMovableEntity, en_fAcceleration), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+36, offsetof(CMovableEntity, en_fDeceleration), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+37, offsetof(CMovableEntity, en_fStepUpHeight), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+42, offsetof(CMovableEntity, en_fStepDnHeight), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+38, offsetof(CMovableEntity, en_fBounceDampParallel), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+39, offsetof(CMovableEntity, en_fBounceDampNormal), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+40, offsetof(CMovableEntity, en_fCollisionSpeedLimit), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+41, offsetof(CMovableEntity, en_fCollisionDamageFactor), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOATAABBOX3D, NULL, (0x00000001<<8)+51, offsetof(CMovableEntity, en_boxMovingEstimate), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOATAABBOX3D, NULL, (0x00000001<<8)+52, offsetof(CMovableEntity, en_boxNearCached), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+64, offsetof(CMovableEntity, en_vIntendedTranslation), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOATMATRIX3D, NULL, (0x00000001<<8)+65, offsetof(CMovableEntity, en_mIntendedRotation), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+70, offsetof(CMovableEntity, en_tmEntityTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+75, offsetof(CMovableEntity, m_bReferenceMovingInY), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+76, offsetof(CMovableEntity, m_bReferenceRotatingNonY), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000001<<8)+77, offsetof(CMovableEntity, m_iPlayerRefCounter), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000001<<8)+78, offsetof(CMovableEntity, en_vDesiredTranslationFinal), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000001<<8)+80, offsetof(CMovableEntity, m_tmMoveStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000001<<8)+81, offsetof(CMovableEntity, m_tickMoveStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+82, offsetof(CMovableEntity, m_bForceStop), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+83, offsetof(CMovableEntity, m_bBlockDown), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+84, offsetof(CMovableEntity, m_bBlockUp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+85, offsetof(CMovableEntity, m_bStair), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000001<<8)+86, offsetof(CMovableEntity, m_bFallDownToFloor), "", 0, 0, 0),
};
#define CMovableEntity_propertiesct ARRAYCOUNT(CMovableEntity_properties)

CEntityComponent CMovableEntity_components[] = {
  CEntityComponent()
};
#define CMovableEntity_componentsct 0


CEventHandlerEntry CMovableEntity_handlers[] = {
 {0x00010000, -1, CEntity::pEventHandler(&CMovableEntity::
#line 2793 "E:/Бъ»к/Korea/Client_USA/Engine/Classes/MovableEntity.es"
Dummy),DEBUGSTRING("CMovableEntity::Dummy")},
};
#define CMovableEntity_handlersct ARRAYCOUNT(CMovableEntity_handlers)

CEntity *CMovableEntity_New(void) { return new CMovableEntity; };
void CMovableEntity_OnInitClass(void) {};
void CMovableEntity_OnEndClass(void) {};
void CMovableEntity_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CMovableEntity_OnWorldEnd(CWorld *pwo) {};
void CMovableEntity_OnWorldInit(CWorld *pwo) {};
void CMovableEntity_OnWorldTick(CWorld *pwo) {};
void CMovableEntity_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CMovableEntity, CRationalEntity, "MovableEntity", "", 0x00000001);
DECLARE_CTFILENAME(_fnmCMovableEntity_tbn, "");