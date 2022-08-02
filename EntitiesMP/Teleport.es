219
%{
#include "StdH.h"
#include "EntitiesMP/Player.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/SlaveInfo.h>
%}

uses "EntitiesMP/BasicEffects";

enum eTeleportType
{
	0 TELEPORT_DEFAULT		"Teleport Default",
	1 TELEPORT_NETWORK		"Teleport Network",
	2 TELEPORT_GOZONE		"Teleport Go-Zone",
};

event ETeleportActivate {
};
event ETeleportDeactivate {
};

class CTeleport : CRationalEntity {
name      "Teleport";
thumbnail "Thumbnails\\Teleport.tbn";
features  "HasName", "HasTarget", "IsTargetable", "IsImportant";


properties:

  1 CTString m_strName          "Name" 'N' = "Teleport",
  3 CTString m_strDescription = "",
  2 CEntityPointer m_penTarget  "Target" 'T' COLOR(C_BROWN|0xFF),
  4 FLOAT m_fWidth              "Width"  'W' = 2.0f,
  5 FLOAT m_fHeight             "Height" 'H' = 3.0f,
  6 BOOL m_bActive              "Active" 'A' = TRUE features(EPROPF_NETSEND),
  7 BOOL m_bPlayersOnly         "Players only" 'P' = TRUE,
  8 BOOL m_bForceStop           "Force stop" 'F' = FALSE,
  9 enum eTeleportType	m_eTeleportType	"Teleport Type" = TELEPORT_DEFAULT,
  10 INDEX m_iTeleportIndex		"Teleport/Zone Index"	= -1,
  11 INDEX m_iTeleportExtIndex	"Teleport/Zone Extension Index"	= 0,


components:

  1 model   MODEL_TELEPORT     "Data\\Models\\Editor\\Teleport.mdl",
  2 editor texture TEXTURE_TELEPORT   "Data\\Models\\Editor\\Teleport.tex",
  3 class   CLASS_BASIC_EFFECT  "Classes\\BasicEffect.ecl",


functions:

  const CTString &GetDescription(void) const {
    ((CTString&)m_strDescription).PrintF("-><none>");
    if (m_penTarget!=NULL) {
      ((CTString&)m_strDescription).PrintF("->%s", m_penTarget->GetName());
    }
    return m_strDescription;
  }

  void TeleportEntity(CEntity *pen, const CPlacement3D &pl)
  {
    // teleport back
    pen->Teleport(pl);

    // spawn teleport effect
    ESpawnEffect ese;
    ese.colMuliplier = C_WHITE|CT_OPAQUE;
    ese.betType = BET_TELEPORT;
    ese.vNormal = FLOAT3D(0,1,0);
    FLOATaabbox3D box;
    pen->GetBoundingBox(box);
    FLOAT fEntitySize = box.Size().MaxNorm()*2;
    ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);
    CEntityPointer penEffect = CreateEntity(pl, CLASS_BASIC_EFFECT);
    penEffect->Initialize(ese);

	// ��ȯ���� �̵���ŵ�ϴ�.
	for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
	{
		CUISummon* pUISummon = (CUISummon*)_pUIMgr->GetUI(i);
		if( pUISummon->GetSummonEntity() )
		{
			pUISummon->SetCommand(CSlaveInfo::COMMAND_HOLD);
			pUISummon->GetSummonEntity()->SetPlacement(m_penTarget->GetPlacement());
			pUISummon->GetSummonEntity()->FallDownToFloor();
		}
	}

	// �ֿϵ����� �̵� ��ŵ�ϴ�.
	if( _pNetwork->_PetTargetInfo.bIsActive && _pNetwork->_PetTargetInfo.pen_pEntity )
	{
		_pNetwork->_PetTargetInfo.pen_pEntity->SetPlacement(m_penTarget->GetPlacement());
		_pNetwork->_PetTargetInfo.pen_pEntity->FallDownToFloor();
	}
}


  // returns bytes of memory used by this object
  SLONG GetUsedMemory(void)
  {
    // initial
    SLONG slUsedMemory = sizeof(CTeleport) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
    // add some more
    slUsedMemory += m_strName.Length();
    slUsedMemory += m_strDescription.Length();
    return slUsedMemory;
  }


procedures:


  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_TOUCHMODEL);
    SetFlagOff(ENF_PROPSCHANGED);
    SetFlagOn(ENF_NONETCONNECT);
    SetFlagOn(ENF_MARKDESTROY);

    // correct height so teleport could collide as sphere
    if(m_fHeight<m_fWidth)
    {
      m_fHeight=m_fWidth;
    }

    // set appearance
    GetModelObject()->StretchModel(FLOAT3D(m_fWidth, m_fHeight, m_fWidth));
    SetModel(MODEL_TELEPORT);
    ModelChangeNotify();
    SetModelMainTexture(TEXTURE_TELEPORT);

    while (TRUE) {
      // wait to someone enter and teleport it
      wait() {
        on (EPass ePass) : 
		{
			if( m_eTeleportType == TELEPORT_DEFAULT )
			{
				if (m_penTarget!=NULL && m_bActive) 
				{
					if (m_bPlayersOnly && !IsOfClass( ePass.penOther, &CPlayer_DLLClass)) {
						resume;
					}
					TeleportEntity(ePass.penOther, m_penTarget->GetPlacement());
					if (m_bForceStop && (ePass.penOther->GetPhysicsFlags()&EPF_MOVABLE) ) 
					{
					((CMovableEntity*)&*ePass.penOther)->ForceFullStop();
					}
					stop;
				}
				resume;
			}
			else 
			if( m_eTeleportType == TELEPORT_NETWORK )
			{
				if( m_bActive )
				{
					// HARD CODING ---wooss 060515-------------------------------------->>
//					if(m_eTeleportType == TELEPORT_DEFAULT) { m_iTeleportIndex = 999; }
					// -----------------------------------------------------------------<<
					
					if( !IsOfClass( ePass.penOther, &CPlayer_DLLClass))
					{
						resume;
					}
					ASSERT( m_iTeleportIndex != -1 && "Invalid Teleport Index!" );
					// EDIT : BS : BEGIN : �ڷ���Ʈ ������ Lock
					CPlayer* pPlayer = (CPlayer*)CEntity::GetPlayerEntity(0); // ĳ���� �ڽ�
					if (pPlayer)
					{
						pPlayer->m_bRcvAtMsg			= FALSE;
						pPlayer->m_bLockMove			= TRUE;
						pPlayer->m_bReserveMove			= FALSE;
						pPlayer->StopMove();
					}
					// EDIT : BS : END : �ڷ���Ʈ ������ Lock
					_pNetwork->SendWarpTeleport( m_iTeleportIndex );
					stop;					
				}
				resume;
			}
			else
			{
				if( m_bActive )
				{
					if( !IsOfClass( ePass.penOther, &CPlayer_DLLClass))
					{
						resume;
					}
					ASSERT( m_iTeleportIndex != -1 && "Invalid Teleport Index!" );
					const int iWorldNum = m_iTeleportIndex;						// ���� ��ȣ.
					const int iExtraNum = m_iTeleportExtIndex;					// Extra ��ȣ.
					_pNetwork->GoZone(iWorldNum, iExtraNum);
					stop;
				}
				resume;
			}
        }
        on (EActivate) : {
          SetFlagOn(ENF_PROPSCHANGED);
          if (_pNetwork->IsServer()) {
            SendEvent(ETeleportActivate(),TRUE);
          }
          resume;
        }
        on (ETeleportActivate) : {
          m_bActive = TRUE;
          resume;
        }
        on (EDeactivate) : {
          SetFlagOn(ENF_PROPSCHANGED);
          if (_pNetwork->IsServer()) {
            SendEvent(ETeleportDeactivate(),TRUE);
          }
          resume;
        }
        on (ETeleportDeactivate) : {
          m_bActive = FALSE;
          resume;
        }
        otherwise() : {
          resume;
        };
      };
      
      // wait a bit to recover
      autowait(0.1f);
    }
  }
};

