/*
 * Player entity.
 */
4
%{
#include "StdH.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/CRC.h>
%}

class export CPlayerEntity : CMovableModelEntity {
name      "PlayerEntity";
thumbnail "";
features "AbstractBaseClass";
properties:
  1 FLOAT en_tmPing = 0.0f,    // ping value in seconds (determined by derived class and distributed by the engine)
//  2 INDEX m_iLastActionTime = 1, // when was the last action applied to this player entity (in milliseconds)
  //3 INDEX m_iMyIndex = -1, //0423
{
  CPlayerCharacter en_pcCharacter;   // character of the player
  CPlacement3D en_plViewpoint;       // placement of view point relative to the entity
  CPlacement3D en_plLastViewpoint;   // last view point (used for lerping)
}
components:
functions:
  /* Initialize this player */
  void InitializePlayer(ULONG ulWeaponsID,ULONG ulAnimatorID) 
  {
    EPlayerInit ePLInit;
    ePLInit.ulWeaponsID = ulWeaponsID;
    ePLInit.ulAnimatorID = ulAnimatorID;
    ePLInit.bCreate = TRUE;
    Initialize(ePLInit,FALSE);
  }

  // override in player.es
  // FIXME : SetWeaponAndAnimator()도 제거되야 할듯...
  virtual void SetWeaponAndAnimator(){};

  /* Is this a player entity? - Yes - override for IsPlayer from Entity.h*/
  virtual BOOL IsPlayer() {return TRUE;};

  // Dump player info to the console
  virtual void DumpToConsole(){};

  // virtual functions for reading/writing player data for transmission over network, redefined in Player.es
//  virtual void Read_net(CNetworkMessage *istr,TIME tmNewTime) {};
//  virtual void Write_net(CNetworkMessage *ostr) {};

//  virtual void Read_net_brief(CNetworkMessage *istr,TIME tmNewTime) {};
//  virtual void Write_net_brief(CNetworkMessage *ostr) {};


	// FIXME : 왜 이런식으로 해둔거지?ㅡ.ㅡ
  virtual void Read_net_Mob(int index, CNetworkMessage *istr) {};
  virtual void Read_net_Character(int index, CNetworkMessage *istr) {};
  virtual void Read_net_Pet(int index, CNetworkMessage *istr) {};
  virtual void Read_net_WildPet(int index, CNetworkMessage *istr) {};
  virtual void Read_net_Summon(int index, CNetworkMessage *istr) {};
  virtual void Read_net_Damage( SBYTE sbType, CNetworkMessage *istr) {};
  virtual void ProcessProduce(SBYTE sbChaType, SLONG slChaIndex, SBYTE sbTargetType, SLONG slTargetIndex, SLONG slTargetHP) {};
  
  virtual void StandDown( BOOL bStand ) {};  
  virtual void OpenGate( int iGateNumber, BOOL bOpenGate ) {};
  virtual void SetMobData(CEntity* penEntity, SLONG hp, SLONG maxHp, int level, BOOL bNpc, int mobIdx) {};
  virtual void SetChaData(int index, int type, ULONG hp, ULONG maxHp, SBYTE hairstyle, SBYTE facestyle, CTString& strName, SBYTE state, SBYTE pkTitle, SLONG pkstate) {};
  virtual void SetPetData(CEntity* penEntity, SLONG hp, SLONG maxHP) {};  
  virtual void SetWildPetData(CEntity* penEntity, SLONG hp, SLONG maxHP) {};
  virtual void SetPetColor(CEntity* penEntity, UBYTE sbPetColor) {};
  virtual void SetElementalData(CEntity* penEntity, SLONG hp, SLONG maxHP ) {};
  virtual void SetElementalStatus(CEntity* penEntity, SBYTE sbAttackSpeed, SBYTE sbMagicSpeed, LONG lSkillSpeed, FLOAT fWalkSpeed, FLOAT fRunSpeed, FLOAT fAttackRange ){};
  virtual void SetShopData( INDEX index, SBYTE sbShopType) {};

  virtual void StartCamera(CEntity *pTarget, BOOL bRestart) {};
  virtual void GetProjection(CAnyProjection3D& apr) {};

  virtual void LostTarget(){};
  virtual void UseSkill(int skillIndex){};
  virtual void UsePetPickItem(){};
  virtual void UsePetSkill(int skillIndex){};
  virtual void UseSlaveSkill(CEntity *pEntity, int skillIndex){};
  virtual void UseWildSkill(CEntity *pEntity, int skillIndex){};
//  virtual void UseAction(int ActionIndex){};//0807
  virtual void CancelSkill(BOOL bLostTarget, BOOL bAutoAttack, BOOL bSkillError){};
  virtual void LoadSkillAnimID(){};
  virtual void UseAction(int ActionIndex){};
  virtual void StopMove(){};
  

  virtual void SetProduct(SLONG slChaIndex, SBYTE slProductType) {};
  virtual void SetConsensus(SLONG slChaIndex) {};
  virtual void WarpTeleport( FLOAT fX, FLOAT fZ, FLOAT fH, FLOAT fR, SBYTE sbLayer ) {};
  
  virtual   void SingleModeOn() {};
  virtual   void FieldModeOn() {};

  virtual void SetLegit(UBYTE sbType, SLONG chaindex) {};
  virtual void SetRepairLegit(SLONG chaindex, CCharacterTarget *Ct) {};
  virtual BOOL IsLegit(){return FALSE; };
  virtual void ClearMultiTargets() {};

  virtual void RidingPet(CEntity *pCharacter, INDEX iPetType ) {};
  virtual void LeavingPet(CEntity *pCharacter ) {};
  virtual BOOL IsRidePet( CEntity *pCharacter ) { return FALSE; };

  virtual	BOOL IsMoving() { return FALSE; };
  virtual	BOOL IsActionSitting() { return FALSE; }
  virtual	BOOL IsIdle()	{ return FALSE; }

  virtual   void ClearTargetInfo(CEntity* penEntity){};

  virtual	void SetCharacterHairChange(CEntity* penEntity, SBYTE sbType, INDEX Cnt, BOOL IsMe) {};
  virtual	void SetCharacterAppearance(CModelInstance *pMI, int iJob, SBYTE sbHairStyle, SBYTE sbFaceStyle)	{};
  virtual	void ChangeFaceMesh(CModelInstance *pMI, int iJob, SBYTE sbType)	{};		// 머리 바꾸기  
  virtual	void ChangeHairMesh(CModelInstance *pMI, int iJob, SBYTE sbType)	{};		// 헤어 스타일 바꾸기

  // add for collect quest 060721
  virtual	void CancelProduct() {};
  virtual	void DeleteWearingWeapon(BOOL bException) {};
  virtual	void AppearWearingWeapon() {};

//  virtual	void WearingArmor(SLONG tabId, SLONG rowId, SLONG colId) {};
  virtual	void DeleteDefaultArmor(int type) {};
  virtual	void WearingDefaultArmor(int type) {};
  virtual	void DeleteCurrentArmor(int weartype) {};
  virtual	void SetChaWearing(class CCharacterTarget &ct, CEntity* penEntity, CNetworkMessage *istr) {};
  virtual   void Rebirth(){};
  virtual   void DeathYou(){};
  virtual   void DeathInit(){};
  virtual   BOOL IsAlreadyDie(){return FALSE;};
  virtual   void SetDie(){};
  virtual	void SetTarget(CEntity* penEntity){};
  virtual	void SetTarget( SLONG targetID, SBYTE targetType ){};
  virtual	BOOL IsSameTarget(CEntity* penEntity){return FALSE;}

  virtual void SetTargetMe(){};
  virtual void PlayItemSound(BOOL bPick, BOOL bMoney){};
  virtual void PlayButtonSound(){};
  virtual void CommandAttack(){};
  virtual BOOL CommandAction(){ return FALSE; };		//물뿌리기 액션을 사용하기 위한 타겟을 설정

  virtual void PlayBGM(CTFileName fnFileName){};
  
  virtual void SearchNearItem(){};
  // FIXME : 상태를 확인하는데 매번 이렇게 찾아야 하는가?
  virtual BOOL IsSkilling(){return FALSE;};
  virtual BOOL IsSitting(){return FALSE;};
  virtual BOOL IsAttacking() { return FALSE; };
  virtual BOOL IsPetActing() { return FALSE; };
  virtual BOOL IsProduct() { return FALSE; };
  virtual void SetSpeedUp(BOOL bMe, INDEX index, BOOL bSpeedup){};
  // 이기환 추가 (04.12.29) : ESC키로 취소 되는 동작들이 활성화 되어 있는지 체크 
  virtual BOOL CheckEscKey (void) { return FALSE; } 
  virtual void PlayItemEffect(SLONG itemindex, SBYTE effecttype){};
  virtual BOOL IsSocialActing(){ return FALSE; }
  virtual BOOL IsPolymophing(){ return FALSE; }
  virtual BOOL IsTransforming(){ return FALSE; }
  virtual void AppearChaPolymoph(CEntity* pentity, INDEX mobindex){};
  virtual void TransfromPet(CEntity* pentity, INDEX mobIndex, INDEX mobSize){};
  virtual void ReturnTransformPet(CEntity* pentity){};

  virtual void TransformSorcerer( int iType ){};
  virtual void ReturnSorcerer(){};
  virtual void AppearChaTransform( CEntity* pEntity, INDEX iTransformType ){};

  // eons 추가 (061207) : 악세사리 착용시 Effect적용
  virtual void PlAddAccessoryEffect(INDEX iCt,CEntity *penEntity, BOOL bIsMe){};

  // 강신 시작 & 강신 종료.
  virtual void EvocationStart( LONG lIndex, SBYTE sbType){};
  virtual void EvocationStop( LONG lIndex){};
  virtual BOOL CheckChangeCondition(INDEX level, INDEX zone){ return FALSE;};  
  virtual void SetSummonCommand( CEntity* pEntity, INDEX iCommand ) {};

  virtual BOOL CheckNormalAttack( CEntity *pEntity, FLOAT fDist ) { return FALSE; }
  virtual CModelInstance* GetPlayerModelInstance( CEntity* pEntity ) { return NULL; }
  virtual BOOL CheckAttackTarget( INDEX iSkillIndex, CEntity *pEntity, FLOAT fDist ) { return FALSE; };  
  virtual BOOL CheckEntityOfTarget(CEntity *pEntity, CEntity *pTarget) { return FALSE; }

  virtual void ReturnChange( BOOL bAppear ){};
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	//현재 몹이 죽을때 말고는 몹이 item drop하는 경우는 없음.
	//추후 그런 경우가 생긴다면 수정해야함.
	virtual void AddDeathItem(CEntity *penEnemy, FLOAT3D pos, class CItemTarget *pItemTarget, class CItemData *pItemData) {};
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// 전사의 축복 시스템 작업시 추가 wooss
	virtual void DropDeathItem(CEntity *penEnemy) {};

  /* Get name of this player. */
  export CTString GetPlayerName(void)
  {
    return en_pcCharacter.GetNameForPrinting();
  }
  export const CTString &GetName(void) const
  {
    return en_pcCharacter.GetName();
  }
  /* Get index of this player in the game. */
  // FIXME : GetMyPlayerIndex가 반환하는 값은 무조건 0.
  // FIXME : 고로, 이 부분이 들어간 모든 부분을 제거할것.
  export INDEX GetMyPlayerIndex(void)
  {
    CEntity *penMe = this;
    for (INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) {
      // if this is ME (this)
      if (GetPlayerEntity(iPlayer)==penMe) {
        return iPlayer;
      }
    }
    // must find my self
    return 15;  // if not found, still return a relatively logical value
  }

  /* Calculate physics for moving. */
  export void DoMoving(void)  // override from CMovableEntity
  {
    CMovableModelEntity::DoMoving();
  }

  /* Copy entity from another entity of same class. */
  export void Copy(CEntity &enOther, ULONG ulFlags)
  {
    CMovableModelEntity::Copy(enOther, ulFlags);

    CPlayerEntity *ppenOther = (CPlayerEntity *)(&enOther);
    en_pcCharacter = ppenOther->en_pcCharacter;
    en_plViewpoint = ppenOther->en_plViewpoint;
    en_plLastViewpoint = ppenOther->en_plLastViewpoint;
  }

  /* Copy entity from another entity of same class. */
  /*CPlayerEntity &operator=(CPlayerEntity &enOther)
  {
    CMovableModelEntity::operator=(enOther);
    en_pcCharacter = enOther.en_pcCharacter;
    en_plViewpoint = enOther.en_plViewpoint;
    return *this;
  }*/
  /* Read from stream. */
  export void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
  {
    CMovableModelEntity::Read_t(istr,bNetwork);
    (*istr)>>en_pcCharacter>>en_plViewpoint;
    en_plLastViewpoint = en_plViewpoint;
  }
  /* Write to stream. */
  export void Write_t( CTStream *ostr,BOOL bNetwork) // throw char *
  {
    CMovableModelEntity::Write_t(ostr,bNetwork);
    (*ostr)<<en_pcCharacter<<en_plViewpoint;
  }

  // Apply the action packet to the entity movement.
  export virtual void ApplyAction(const CPlayerAction &pa, FLOAT tmLatency) {};
  // Called when player is disconnected
  export virtual void Disconnect(void) {};

  // Called when player character is changed
  // FIXME : CharacterChanged가 들어간 모든 부분 제거할것.
  export virtual void CharacterChanged(const CPlayerCharacter &pcNew) { en_pcCharacter = pcNew; };

  // provide info for GameSpy enumeration
  //export virtual void GetGameSpyPlayerInfo(INDEX iPlayer, CTString &strKeys) { };

procedures:
  // must have at least one procedure per class
  Dummy() {};
};
