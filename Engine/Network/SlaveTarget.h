#ifndef __SLAVETARGET_H__
#define __SLAVETARGET_H__


//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>

class  CSlaveTargetInfom
{
public:
	LONG	lIndex;
	BOOL	bIsActive;	
	FLOAT	fMaxHealth;
	FLOAT	fHealth;	
	int		iType;		

	CEntity	*pen_pEntity;	

public:
	CSlaveTargetInfom(void);
	~CSlaveTargetInfom(void);

	ENGINE_API void	Init();
};

// FIXME : 많은 정리 필요...ㅡ.ㅡ
class ENGINE_API CSlaveTarget 
{
public:
	INDEX			slave_Index;
	INDEX			slave_iType;
	INDEX			slave_iClientIndex;
	CEntity			*slave_pEntity;
	CTString		slave_Name;
	SBYTE			slave_BuffCount;
	BuffInfo		slave_Buff[20];
	CTString		slave_OwnerName;
	SBYTE			slave_yLayer;	
	CStatusEffect	slave_statusEffect;
	CEffectGroup	*slave_pNormalEffect;
	UBYTE			slave_sbAttributePos;
	INDEX			slave_OwnerIndex;	
	
public:
	/* Default constructor. */
	CSlaveTarget(void);
	/* Destructor. */
	~CSlaveTarget(void);
	
	CSlaveTarget(const CSlaveTarget &other);
	CSlaveTarget &operator=(const CSlaveTarget &other);
	
	void	Init(void);
	void	SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iSlaveType, CEntity *pEntity, SBYTE sbyLayer );	
	void	SetClientSlaveId( INDEX index );
	void	SetyLayer( SBYTE sbyLayer ) { slave_yLayer = sbyLayer; }	
	void	AddBuff( BuffInfo &rBuffInfo )
	{
		slave_Buff[slave_BuffCount++] = rBuffInfo;
	}
	void	RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex );

	void	ChangeStatus(CTagManager* pTM, SLONG status)
	{
		if(slave_statusEffect.IsStatusChanged(status))
			slave_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		slave_statusEffect.Reset();
	}
};

#endif //__SLAVETARGET_H__