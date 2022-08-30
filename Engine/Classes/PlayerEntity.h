/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _Engine_Classes_PlayerEntity_INCLUDED
#define _Engine_Classes_PlayerEntity_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CPlayerEntity_DLLClass;
class DECL_DLL  CPlayerEntity : public CMovableModelEntity {
public:
   virtual void SetDefaultProperties(void);
  FLOAT en_tmPing;
CPlayerCharacter en_pcCharacter;
CPlacement3D en_plViewpoint;
CPlacement3D en_plLastViewpoint;
   
#line 28 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
void InitializePlayer(ULONG ulWeaponsID,ULONG ulAnimatorID);
  
#line 39 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetWeaponAndAnimator();
  
#line 42 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPlayer();
  
#line 45 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DumpToConsole();
  
#line 56 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Mob(int index,CNetworkMessage * istr);
  
#line 57 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Character(int index,CNetworkMessage * istr);
  
#line 58 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Pet(int index,CNetworkMessage * istr);
  
#line 59 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_WildPet(int index,CNetworkMessage * istr);
  
#line 60 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Summon(int index,CNetworkMessage * istr);
  
#line 61 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Damage(SBYTE sbType,CNetworkMessage * istr);
  
#line 62 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ProcessProduce(SBYTE sbChaType,SLONG slChaIndex,SBYTE sbTargetType,SLONG slTargetIndex,SLONG slTargetHP);
  
#line 64 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void StandDown(BOOL bStand);
  
#line 65 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void OpenGate(int iGateNumber,BOOL bOpenGate);
  
#line 66 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetMobData(CEntity * penEntity,SLONG hp,SLONG maxHp,int level,BOOL bNpc,int mobIdx);
  
#line 67 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetChaData(int index,int type,ULONG hp,ULONG maxHp,SBYTE hairstyle,SBYTE facestyle,CTString & strName,SBYTE state,SBYTE pkTitle,SLONG pkstate);
  
#line 68 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetPetData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 69 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetWildPetData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 70 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetPetColor(CEntity * penEntity,UBYTE sbPetColor);
  
#line 71 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetElementalData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 72 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetElementalStatus(CEntity * penEntity,SBYTE sbAttackSpeed,SBYTE sbMagicSpeed,LONG lSkillSpeed,FLOAT fWalkSpeed,FLOAT fRunSpeed,FLOAT fAttackRange);
  
#line 73 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetShopData(INDEX index,SBYTE sbShopType);
  
#line 75 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void StartCamera(CEntity * pTarget,BOOL bRestart);
  
#line 76 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void GetProjection(CAnyProjection3D & apr);
  
#line 78 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void LostTarget();
  
#line 79 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UseSkill(int skillIndex);
  
#line 80 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UsePetPickItem();
  
#line 81 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UsePetSkill(int skillIndex);
  
#line 82 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UseSlaveSkill(CEntity * pEntity,int skillIndex);
  
#line 83 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UseWildSkill(CEntity * pEntity,int skillIndex);
  
#line 85 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void CancelSkill(BOOL bLostTarget,BOOL bAutoAttack,BOOL bSkillError);
  
#line 86 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void LoadSkillAnimID();
  
#line 87 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void UseAction(int ActionIndex);
  
#line 88 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void StopMove();
  
#line 91 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetProduct(SLONG slChaIndex,SBYTE slProductType);
  
#line 92 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetConsensus(SLONG slChaIndex);
  
#line 93 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void WarpTeleport(FLOAT fX,FLOAT fZ,FLOAT fH,FLOAT fR,SBYTE sbLayer);
  
#line 95 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SingleModeOn();
  
#line 96 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void FieldModeOn();
  
#line 98 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetLegit(UBYTE sbType,SLONG chaindex);
  
#line 99 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetRepairLegit(SLONG chaindex,CCharacterTarget * Ct);
  
#line 100 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsLegit();
  
#line 101 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ClearMultiTargets();
  
#line 103 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void RidingPet(CEntity * pCharacter,INDEX iPetType);
  
#line 104 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void LeavingPet(CEntity * pCharacter);
  
#line 105 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsRidePet(CEntity * pCharacter);
  
#line 107 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsMoving();
  
#line 108 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsActionSitting();
  
#line 109 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsIdle();
  
#line 111 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ClearTargetInfo(CEntity * penEntity);
  
#line 113 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetCharacterHairChange(CEntity * penEntity,SBYTE sbType,INDEX Cnt,BOOL IsMe);
  
#line 114 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetCharacterAppearance(CModelInstance * pMI,int iJob,SBYTE sbHairStyle,SBYTE sbFaceStyle);
  
#line 115 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ChangeFaceMesh(CModelInstance * pMI,int iJob,SBYTE sbType);
  
#line 116 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ChangeHairMesh(CModelInstance * pMI,int iJob,SBYTE sbType);
  
#line 119 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void CancelProduct();
  
#line 120 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DeleteWearingWeapon(BOOL bException);
  
#line 121 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void AppearWearingWeapon();
  
#line 124 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DeleteDefaultArmor(int type);
  
#line 125 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void WearingDefaultArmor(int type);
  
#line 126 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DeleteCurrentArmor(int weartype);
  
#line 127 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetChaWearing(class CCharacterTarget & ct,CEntity * penEntity,CNetworkMessage * istr);
  
#line 128 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void Rebirth();
  
#line 129 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DeathYou();
  
#line 130 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DeathInit();
  
#line 131 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsAlreadyDie();
  
#line 132 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetDie();
  
#line 133 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetTarget(CEntity * penEntity);
  
#line 134 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetTarget(SLONG targetID,SBYTE targetType);
  
#line 135 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSameTarget(CEntity * penEntity);
  
#line 137 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetTargetMe();
  
#line 138 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void PlayItemSound(BOOL bPick,BOOL bMoney);
  
#line 139 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void PlayButtonSound();
  
#line 140 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void CommandAttack();
  
#line 141 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CommandAction();
  
#line 143 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void PlayBGM(CTFileName fnFileName);
  
#line 145 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SearchNearItem();
  
#line 147 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSkilling();
  
#line 148 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSitting();
  
#line 149 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsAttacking();
  
#line 150 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPetActing();
  
#line 151 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsProduct();
  
#line 152 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetSpeedUp(BOOL bMe,INDEX index,BOOL bSpeedup);
  
#line 154 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckEscKey(void);
  
#line 155 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void PlayItemEffect(SLONG itemindex,SBYTE effecttype);
  
#line 156 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSocialActing();
  
#line 157 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPolymophing();
  
#line 158 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL IsTransforming();
  
#line 159 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void AppearChaPolymoph(CEntity * pentity,INDEX mobindex);
  
#line 160 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void TransfromPet(CEntity * pentity,INDEX mobIndex,INDEX mobSize);
  
#line 161 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ReturnTransformPet(CEntity * pentity);
  
#line 163 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void TransformSorcerer(int iType);
  
#line 164 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ReturnSorcerer();
  
#line 165 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void AppearChaTransform(CEntity * pEntity,INDEX iTransformType);
  
#line 168 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void PlAddAccessoryEffect(INDEX iCt,CEntity * penEntity,BOOL bIsMe);
  
#line 171 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void EvocationStart(LONG lIndex,SBYTE sbType);
  
#line 172 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void EvocationStop(LONG lIndex);
  
#line 173 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckChangeCondition(INDEX level,INDEX zone);
  
#line 174 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void SetSummonCommand(CEntity * pEntity,INDEX iCommand);
  
#line 176 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckNormalAttack(CEntity * pEntity,FLOAT fDist);
  
#line 177 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual CModelInstance * GetPlayerModelInstance(CEntity * pEntity);
  
#line 178 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckAttackTarget(INDEX iSkillIndex,CEntity * pEntity,FLOAT fDist);
  
#line 179 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckEntityOfTarget(CEntity * pEntity,CEntity * pTarget);
  
#line 181 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void ReturnChange(BOOL bAppear);
  
#line 185 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void AddDeathItem(CEntity * penEnemy,FLOAT3D pos,class CItemTarget * pItemTarget,class CItemData * pItemData);
  
#line 189 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
virtual void DropDeathItem(CEntity * penEnemy);
   CTString GetPlayerName(void);
   const CTString & GetName(void)const;
   INDEX GetMyPlayerIndex(void);
   void DoMoving(void);
   void Copy(CEntity & enOther,ULONG ulFlags);
   void Read_t(CTStream * istr,BOOL bNetwork);
   void Write_t(CTStream * ostr,BOOL bNetwork);
  virtual void ApplyAction(const CPlayerAction & pa,FLOAT tmLatency);
  virtual void Disconnect(void);
  virtual void CharacterChanged(const CPlayerCharacter & pcNew);
#define  STATE_CPlayerEntity_Dummy 0x00040000
  BOOL 
#line 269 "D:\LC_2009_Project\lastchaos-source-client-2009/Engine/Classes/PlayerEntity.es"
Dummy(const CEntityEvent &__eeInput);
};
#endif // _Engine_Classes_PlayerEntity_INCLUDED
