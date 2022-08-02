403
%{
#include "StdH.h"
#include "EntitiesMP/Player.h"
#include "EntitiesMP/PlayerWeapons.h"
%}


enum ViewType {
	0 VT_PLAYERDEATH    "",   // player death
	1 VT_PLAYERREBIRTH  "",   // player rebirth (player is spawned)
	2 VT_CAMERA         "",   // camera view
	3 VT_3RDPERSONVIEW  "",   // 3rd person view
};

// input parameter for viewer
event EViewInit {
	CEntityID eidOwner,        // who owns it
	CEntityID eidCamera,       // first camera for camera view
	enum ViewType vtView,           // view type
	BOOL bDeathFixed,
};

%{
#include <Engine/Effect/CCameraEffect.h>
extern ENGINE_API CCameraEffect::CCameraValue g_cvCameraShake;
FLOAT3D g_vOldCameraShake;
FLOAT3D g_vOldCameraPosByCharCoord;

void CPlayerView_Precache(void) 
{
	CDLLEntityClass *pdec = &CPlayerView_DLLClass;
	pdec->PrecacheModel(MODEL_MARKER);
	pdec->PrecacheTexture(TEXTURE_MARKER);
}

static FLOAT m_fLastTime;
static BOOL m_bLockRotate = FALSE;

// FIXME : �ӽ������� ���� ����.
// ���� ��带 �� ������ ���� ����� ��������...
static BOOL m_bUseTraceMode	= FALSE;

void CameraEffect(const CPlacement3D &plPlr, CPlacement3D &plCam)
{
	CPlacement3D plNew = plCam;
	//twist ó��
	if(g_cvCameraShake.m_fTwist != 0)
	{
		plNew.pl_OrientationAngle(3) += g_cvCameraShake.m_fTwist;
	}

	//ī�޶� ��ǥ���� �̵�ó��
	FLOATmatrix3D m;
	if(g_cvCameraShake.m_fHorizonal != 0
	|| g_cvCameraShake.m_fVertical != 0
	|| g_cvCameraShake.m_fZoom != 0)
	{
		MakeRotationMatrixFast(m, plCam.pl_OrientationAngle);
		FLOAT3D vMoveByCamCoord(g_cvCameraShake.m_fHorizonal
								, g_cvCameraShake.m_fVertical
								, g_cvCameraShake.m_fZoom);
		RotateVector(vMoveByCamCoord, m);
		plNew.pl_PositionVector += vMoveByCamCoord;
	}

	//ĳ���� ��ǥ���� �̵�ó��
	if(g_cvCameraShake.m_vMoveByCharCoord(1) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(2) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(3) != 0)
	{
		MakeRotationMatrixFast(m, plPlr.pl_OrientationAngle);
		FLOAT3D vMoveByChaCoord = g_cvCameraShake.m_vMoveByCharCoord;
		RotateVector(vMoveByChaCoord, m);
		plNew.pl_PositionVector += vMoveByChaCoord;
	}

	plCam = plNew;
}
%}

class export CPlayerView : CMovableEntity {
name      "Player View";
thumbnail "";
features "CanBePredictable","NotSentOverNet";

properties:
	1 CEntityPointer m_penOwner,            // class which owns it
	2 INDEX m_iViewType=0,                  // view type  
	3 FLOAT m_fDistance = 1.0f,             // current distance
	4 FLOAT3D m_vZLast = FLOAT3D(0,0,0), 
	5 FLOAT3D m_vTargetLast = FLOAT3D(0,0,0), 
	6 BOOL m_bFixed = FALSE,  // fixed view (player falling in abyss)

//������ ���� ���� �̱� ���� �۾�	08.05
	10 BOOL m_bInit			= FALSE,
	15 ANGLE m_aCurAngle	= 0.0f,			// ���� ����
	16 ANGLE m_aAccel		= 0.0f,			// ���ӵ�
	17 ANGLE m_aVelocity	= 0.0f,			// �ӵ�
	18 COLOR m_PrevModelColor = 0.0f,		// ���� �÷� ����
	19 BOOL m_bPrevModelColor = FALSE,		// ���� �÷� ���� ����
//������ ���� �� �̱� ���� �۾�		08.05	

components:
	1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\Axis.mdl",
	2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\Vector.tex"

functions:
	void PreMoving() {};
	void DoMoving() 
	{
		en_plLastPlacement = GetPlacement();  // remember old placement for lerping
	};
	void PostMoving() 
	{
		// �ڿ��� �����ϴ� ���.
		if(m_bUseTraceMode)
		{
			// �� ������ ���� ȣ��Ǵ� �κ�.
			SetCameraPosition();
		}
		// ���� ���.
		else
		{
			SetCameraPositionEx();
		}
	}
	CPlacement3D GetLerpedPlacement(void) const
	{
		FLOAT fLerpFactor;
		fLerpFactor = _pTimer->GetLerpFactor2();
		return LerpPlacementsPrecise(en_plLastPlacement, en_plPlacement, fLerpFactor);
//		return en_plPlacement;//�̷��� �س����� ĳ���Ͱ� ����.
		//return CMovableEntity::GetLerpedPlacement();
	}

	// render particles
	void RenderParticles(void)
	{
		if (Particle_GetViewer()==this) {
			Particles_ViewerLocal(this);
		}
	}

//������ ���� ���� �̱� ���� �۾�	08.05
	void UnlockRotate()
	{
		m_bLockRotate = FALSE;
	};
//������ ���� �� �̱� ���� �۾�		08.05

	// FIXME : ���� ����� ī�޶�.  
	// FIXME : ������ �κ�.
	void SetCameraPositionEx()
	{
		// 3rd person view
		FLOAT fDistance = 1.0f;
		//FLOAT fDistanceLimit = 4.5f;
		FLOAT fDistanceLimit = 9.5f;
		static FLOAT fDistanceTmp	= 0.0f;
		BOOL bHit = FALSE;

		// dungeon�� field ������ zoom out�� ���� �ٸ��� �����Ѵ�. *** �̷��� �� �ʿ� ������ ���Ƽ� �Ʒ� IF�� ���� ***
//		if (g_slZone == 0 || g_slZone == 4 || g_slZone == 7) 
//		{
			//fDistanceLimit = 6.5f;
			//fDistanceLimit = 9.5f;

	/*		
			// �ֿϵ����� Ÿ�� ������ ī�޶��� ó��.
			if( ((CPlayer&) *m_penOwner).m_bRide)
			{
				if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
				{
					fDistanceLimit = 2.0f;
				}
				else 
				{
					fDistanceLimit = 2.5f;
				}
			}
	*/
//		} 
		
		// -------wooss 060521 Camera Zoom fix -------------->>
		static BOOL bLimitIn	= FALSE;
		static FLOAT fSmooth	= 0.0f;
		FLOAT zoomMin = -2.0f;
		
		// �ֿϵ����� Ÿ�� ������ ī�޶��� ó��.
		if( ((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
			{
				zoomMin = 3.0f;
			}
			else 
			{
				zoomMin = 2.5f;
			}
		}
		// --------------------------------------------------<<

		CPlacement3D pl = ((CPlayerEntity&) *m_penOwner).en_plViewpoint;		// ī�޶� ��ġ?
		BOOL bFollowCrossHair;
		
		if (m_iViewType == VT_3RDPERSONVIEW || m_iViewType == VT_PLAYERDEATH) 
		{
		
			fDistance = 5.0f;//5.75f;			

			int WheelDelta = abs(_pInput->m_WheelPos)/120;
			if(_pInput->m_WheelPos > 0 || _pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
			{
				//���� ���� �о�ø���.Zoom In
				if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
				{
					WheelDelta = 1.0f;
				}
				if(fDistanceTmp> zoomMin)
				{
					if(fDistanceTmp <= -1.6)
					{
						fDistanceTmp -= 0.2f*WheelDelta;
					}
					else
					{
						fDistanceTmp -= 2.0f*WheelDelta;
					}

					if(fDistanceTmp< zoomMin)
					{
						fDistanceTmp = zoomMin;
					
					}
								
					_pInput->m_WheelPos = 0;		
				}
				else 
				{
					if( fSmooth	> 0.0f && !bLimitIn) { fSmooth -=0.03f; }
					if( fSmooth	<= 0.0f) {
						fSmooth = 0.0f;
						_pInput->m_WheelPos = 0;
						if(WheelDelta > 1 ) {
							fDistanceTmp = zoomMin-1.0f;	
							fSmooth = 1.0f;
							bLimitIn =TRUE;

						}
					}

				}

				// �ֿϵ����� Ÿ�� ������ ī�޶��� ó��.
				if( ((CPlayer&) *m_penOwner).m_bRide && fDistanceTmp <= -1.1 )
				{
					if( ((CPlayer&) *m_penOwner).m_iRideType%2 != 0 )
					{
						fDistanceTmp = -1.1f;
					}
				}
			}
			else if(_pInput->m_WheelPos < 0 || _pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
			{//�� �Ʒ���.Zoom Out
				
				if( bLimitIn ) { fDistanceTmp = zoomMin; }
				bLimitIn = FALSE;
				if(fSmooth < 0.9f) { fSmooth+=0.1f;}
				else if(fSmooth > 0.9f ) { 
					fSmooth =0.9f; 
				}
				if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
				{
					WheelDelta = 1.0f;
				}

				if(fDistanceTmp < fDistanceLimit)
				{
					if(fDistanceTmp <= -1.6)
					{
						fDistanceTmp += 0.2f*WheelDelta;
					}
					else
					{
						fDistanceTmp += 0.4f*WheelDelta;
					}

					//	fDistanceTmp += 0.5f;
					_pInput->m_WheelPos = 0;
				} 
				
			}			

			if(fDistanceTmp > fDistanceLimit )
			{
				fDistanceTmp = fDistanceLimit;
			}
			if(fDistanceTmp < zoomMin-1.0f)
			{
				fDistanceTmp = zoomMin-1.0f;
			}
		
			
			fDistance = fDistance + fDistanceTmp;
			
			bFollowCrossHair = TRUE;
			// death
		} 
		
		// transform rotation angle
		// ȸ�� �ޱ��� ��ȯ��.
		// ����� ��ǥ�� pl�� ĳ������ ��ġ�� �������� ��ȯ��Ŵ.
		CPlacement3D plTmp;
		plTmp = m_penOwner->GetPlacement();
		plTmp.pl_OrientationAngle(1) = pl.pl_OrientationAngle(1);

		pl.RelativeToAbsolute(plTmp);
		//pl.RelativeToAbsolute(m_penOwner->GetPlacement());
		// make base placement to back out from
		FLOAT3D vBase;
		EntityInfo *pei= (EntityInfo*) (m_penOwner->GetEntityInfo());
		
//������ ���� �� �̱� ���� �۾�		07.29

//������ ���� ����	//(Modify Camera Behavior)(0.1)
		//����� �ö󰥶� ĳ���Ͱ� ����Ƣ�� ȿ�� ����.
		//GetLerpedPlacement�� ������������ placement�� �����ϴ� ����.
		//GetEntityInfoPosition(m_penOwner, pei->vTargetCenter, vBase);	//����
		vBase = m_penOwner->GetPlacement().pl_PositionVector;

		FLOATmatrix3D mRotation;
		// ĳ������ Angle�� ȸ�� ��ķ� ��ȯ.
		MakeRotationMatrixFast(mRotation, m_penOwner->GetPlacement().pl_OrientationAngle);
		vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2])*mRotation;
		//vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2]);
//������ ���� ��	//(Modify Camera Behavior)(0.1)
		
		// create a set of rays to test
		FLOATmatrix3D m;
		// ȸ�� ��� �����...
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);

		// �� ���� ���.
		FLOAT3D vRight = m.GetColumn(1);
		FLOAT3D vUp    = m.GetColumn(2);
		FLOAT3D vFront = m.GetColumn(3);
		
		// Ray�� ������� 5�� ����.
		FLOAT3D vDest[5];
		vDest[0] = vBase+vFront*fDistance+vUp*1.5f;
		vDest[1] = vBase+vFront*fDistance+vRight*1.5f;
		vDest[2] = vBase+vFront*fDistance-vRight*1.5f;
		vDest[3] = vBase+vFront*fDistance-vUp*1.5f;
		vDest[4] = vBase+vFront*fDistance;
			
		FLOAT fBack = 0;		
	//	if(_pNetwork->m_ubGMLevel < 2)	//0820	
		{		
		// for each ray
		for (INDEX i=0; i<5; i++) //0915 ���� �ϳ��� �ᵵ �ɵ��ϴ�...
		{
			// cast a ray to find if any brush is hit
			CCastRay crRay( m_penOwner, vBase, vDest[i]);
			crRay.cr_bHitTranslucentPortals = FALSE;
//			if(i==4)
//			{
			//crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
			crRay.cr_ttHitModels = CCastRay::TT_NONE;
//			}
//			else
//			{
//				crRay.cr_ttHitModels = CCastRay::TT_NONE;
//			}
			GetWorld()->CastRay(crRay);
								
			// if hit something //0809 �������̳� ����,ĳ���Ͱ� �ƴҶ��� �Ÿ� ������ �ȴ�.
			if (crRay.cr_penHit!=NULL && (!(crRay.cr_penHit->GetFlags()&ENF_ALIVE) && !(crRay.cr_penHit->GetFlags()&ENF_ITEM))) 
			{			
				// clamp distance
				fDistance = Min(fDistance, crRay.cr_fHitDistance-0.5f);			

				// if hit polygon
				if (crRay.cr_pbpoBrushPolygon!=NULL) 
				{
					// back off
					FLOAT3D vDir = (vDest[i]-vBase).Normalize();

					// �浹�� ������� ������ ���� �� 1/4 ������ ī�޶��� Distance ����.
					FLOAT fD = Abs(FLOAT3D(crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute)%vDir)*0.25f;
					fBack = Max(fBack, fD);
					bHit = TRUE;
				}	
			}
		}
		}	
		
		// �ֿϵ����� Ÿ�� ������ ī�޶��� ó��.
		if( ((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
			{
				fDistance -= 5.0f;
			}
			else 
			{
				fDistance -= 4.5f;
			}
		}

		fDistance = ClampDn(fDistance-fBack, 0.0f); // �Ÿ��� 0�̻�

		if (m_fDistance < fDistance)
		{
			m_fDistance += 0.5f; // ���� �־�����
		}
		
		if (m_fDistance >= fDistance)
		{
			m_fDistance = fDistance;
		}

		vBase += vFront*m_fDistance;
		pl.pl_PositionVector = vBase;//0610 kwon �߰�

		// wooss 060803 ����
		// ĳ���� ���ܸ��� ������ �ʰ� ī�޶�� ������� ���ε����� 
		// ĳ���� ���ܰŸ����� ĳ���ʹ� �׸��� �ʰ� �Ѵ�.
		if(m_fDistance<1.0f) 
		{ 
			if (!m_bPrevModelColor)
			{
				m_PrevModelColor = ((CPlayer&)*m_penOwner).GetModelInstance()->GetModelColor();
				COLOR tColor = m_PrevModelColor;
				tColor &= 0xFFFFFF00;
				tColor |= 0x0F;
				((CPlayer&)*m_penOwner).GetModelInstance()->mi_colModelColor = tColor;
				m_bPrevModelColor = TRUE;
			}
		}
		else
		{
			if (m_bPrevModelColor)
			{
				((CPlayer&)*m_penOwner).GetModelInstance()->mi_colModelColor = m_PrevModelColor;
				m_bPrevModelColor = FALSE;
			}
		}

		

		// �ֿϵ����� ī�޶� ������ ���� �κ�.
		if(((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType %2 == 0 )
			{
				vBase += vFront*m_fDistance;
		    	pl.pl_PositionVector = vBase;
				pl.pl_PositionVector(2) += 1.0f;
			}
			else
			{
				vBase += vFront*m_fDistance;
		    	pl.pl_PositionVector = vBase;
				pl.pl_PositionVector(2) += 3.0f;
			}
		}
		if(!_pNetwork->m_bSingleMode)		
		{
			// wooss 060323
			// ī�޶� �⺻ ��ġ ���� ����
			pl.pl_PositionVector(2) += fSmooth;
		}

		// set camera placement
		// ī�޶��� ��ġ ����.
		// en_plPlacement���� ������.
		SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
	};

//0408 kwon
	void SetCameraPosition() 
	{
		// 3rd person view
		FLOAT fDistance = 1.0f;
		static FLOAT fDistanceTmp	= 0.0f;
		static FLOAT fUpTmp			= 0.0f;		

		CPlacement3D pl = ((CPlayerEntity&) *m_penOwner).en_plViewpoint;		// ī�޶� ��ġ?
		BOOL bFollowCrossHair;
		
//������ ���� ���� �̱� ���� �۾�	08.05
		extern BOOL	_bWorldEditorApp;
		//BOOL bBackView		= !_bWorldEditorApp;

		// FIXME : �ӽ������� �׽�Ʈ.
		BOOL bBackView		= TRUE;
//������ ���� �� �̱� ���� �۾�		08.05
		
		if (m_iViewType == VT_3RDPERSONVIEW || m_iViewType == VT_PLAYERDEATH) 
		{
		
			if(_pNetwork->m_bSingleMode)
			{
				fDistance	= 6.0f;//5.75f;				
				}
			else
			{
				fDistance = 5.0f;//5.75f;
				}

			if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
			{
				fUpTmp -= 10.0f;
				m_bLockRotate			= TRUE;
			}
			if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
			{
				fUpTmp += 10.0f;
				m_bLockRotate			= TRUE;
			}

			//pl.pl_OrientationAngle(1) += fUpTmp;

			if(_pInput->m_WheelPos > 0){//���� ���� �о�ø���.Zoom In
				if(fDistanceTmp> -3.6f){
					if(fDistanceTmp <= -1.6){
						fDistanceTmp -= 0.2f;
					}
					else
					{
						fDistanceTmp -= 1.0f;
					}
					//fDistanceTmp -= 0.5f;
					_pInput->m_WheelPos = 0;		
				}
			}
			else if(_pInput->m_WheelPos < 0){//�� �Ʒ���.Zoom Out
				if(fDistanceTmp < 25.0f){//10.0f){ 
					if(fDistanceTmp <= -1.6){
						fDistanceTmp += 0.2f;
					}
					else
					{
						fDistanceTmp += 1.0f;
					}
					//	fDistanceTmp += 0.5f;
					_pInput->m_WheelPos = 0;
				}
			}
			
			if(_pNetwork->m_bSingleMode)
			{
				fDistanceTmp = 0.0f;
			}
			
			fDistance = fDistance + fDistanceTmp;
			
			bFollowCrossHair = TRUE;
			// death
		} 
		
		pl.pl_OrientationAngle(3) = 0.0f;
		
		// transform rotation angle
		// ȸ�� �ޱ��� ��ȯ��.
		// ����� ��ǥ�� pl�� ĳ������ ��ġ�� �������� ��ȯ��Ŵ.
		CPlacement3D plTmp;
		plTmp = m_penOwner->GetPlacement();
		//plTmp.pl_OrientationAngle(1) = pl.pl_OrientationAngle(1);

		pl.RelativeToAbsolute(plTmp);
		//pl.RelativeToAbsolute(m_penOwner->GetPlacement());
		// make base placement to back out from
		FLOAT3D vBase;
		EntityInfo *pei= (EntityInfo*) (m_penOwner->GetEntityInfo());

//������ ���� ���� �̱� ���� �۾�	07.29
		if(_pNetwork->m_bSingleMode && bBackView)
		{
			CPlayerWeapons *ppw = ((CPlayer&) *m_penOwner).GetPlayerWeapons();			
			FLOAT m_fCurTime	= _pTimer->CurrentTick();

			if(!m_bLockRotate)
			{				
				// �÷��̾�� �� ��ƼƼ ������ ������ ����Ͽ� ó����.
				if(ppw->m_penRayHitTmp)
				{				
					FLOAT3D vDelta	= ppw->m_vRayHitTmp - m_penOwner->GetPlacement().pl_PositionVector;
					vDelta.SafeNormalize();
					ANGLE aDelta	= GetRelativeHeading(vDelta);
					aDelta			= NormalizeAngle(aDelta);
	
					if(!m_bInit)
					{
						m_bInit		= TRUE;
						m_fLastTime	= m_fCurTime;
					}
					m_aVelocity		= aDelta;
				}
				FLOAT m_fDeltaTime	= m_fCurTime - m_fLastTime;
				m_aVelocity			= m_aVelocity + (m_aAccel * m_fDeltaTime);
				m_aCurAngle			= m_aCurAngle + (m_aVelocity * m_fDeltaTime);				
			}

			m_fLastTime				= m_fCurTime;
			//pl.pl_OrientationAngle(1) = m_aCurAngle;
			pl.pl_OrientationAngle(1) = m_aCurAngle - fUpTmp;
		}
		
//������ ���� �� �̱� ���� �۾�		07.29

//������ ���� ����	//(Modify Camera Behavior)(0.1)
		//����� �ö󰥶� ĳ���Ͱ� ����Ƣ�� ȿ�� ����.
		//GetLerpedPlacement�� ������������ placement�� �����ϴ� ����.
		//GetEntityInfoPosition(m_penOwner, pei->vTargetCenter, vBase);	//����
		vBase = m_penOwner->GetPlacement().pl_PositionVector;

		FLOATmatrix3D mRotation;
		// ĳ������ Angle�� ȸ�� ��ķ� ��ȯ.
		MakeRotationMatrixFast(mRotation, m_penOwner->GetPlacement().pl_OrientationAngle);
		vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2])*mRotation;
		//vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2]);
//������ ���� ��	//(Modify Camera Behavior)(0.1)
		
		// create a set of rays to test
		FLOATmatrix3D m;

		// ȸ�� ��� �����...
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);

		// �� ���� ���.
		FLOAT3D vRight = m.GetColumn(1);
		FLOAT3D vUp    = m.GetColumn(2);
		FLOAT3D vFront = m.GetColumn(3);
		
		// Ray�� ������� 5�� ����.
		FLOAT3D vDest[5];
		vDest[0] = vBase+vFront*fDistance+vUp*1.0f;
		vDest[1] = vBase+vFront*fDistance-vUp*1.0f;
		vDest[2] = vBase+vFront*fDistance+vRight*1.0f;
		vDest[3] = vBase+vFront*fDistance-vRight*1.0f;
		vDest[4] = vBase+vFront*fDistance;
		
		FLOAT fBack = 0;
		// for each ray
		for (INDEX i=0; i<5; i++) 
		{
			// cast a ray to find if any brush is hit
			CCastRay crRay( m_penOwner, vBase, vDest[i]);
			crRay.cr_bHitTranslucentPortals = FALSE;
			crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
			GetWorld()->CastRay(crRay);
			
			// if hit something //0809 �������� hit���� �ʴ´�..
			if (crRay.cr_penHit!=NULL && (!(crRay.cr_penHit->GetFlags()&ENF_ALIVE) && !(crRay.cr_penHit->GetFlags()&ENF_ITEM))) 
			{
				// clamp distance
				fDistance = Min(fDistance, crRay.cr_fHitDistance-0.5f);
				// if hit polygon
				if (crRay.cr_pbpoBrushPolygon!=NULL) 
				{
					// back off
					FLOAT3D vDir = (vDest[i]-vBase).Normalize();

					/*
					// FIXME : ���� ���� �ʴ� �κ�.
					// FIXME : ���� �浹������, ������ �ٶ󺸵��� ó���ϴ� �κ���.
					// FIXME : ��ȯ�� ���� �ٽ� �����ؼ� �ٽ� ��ȯ�ϰ�... ���� �ȵ�.
					if(_pNetwork->m_bSingleMode)
					{
						// ī�޶� ���� �ø��� �κ�.
						pl.AbsoluteToRelative(m_penOwner->GetPlacement());
						pl.pl_OrientationAngle(2) = -85.0f;
						pl.RelativeToAbsolute(m_penOwner->GetPlacement());
						MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
						vFront = m.GetColumn(3);
						*/

						// �浹������ ���İ��� �־��ִ� �κ�.
						/*
						COLOR crModel	= m_penOwner->GetModelColor();
						COLOR crColor	= 0xFFFFFF00;
						m_penOwner->SetModelColor(0xFFFFFFA0);
						*/
						/*
					}
					else
					{*/
						// �浹�� ������� ������ ���� �� 1/4 ������ ī�޶��� Distance ����.
					FLOAT fD = Abs(FLOAT3D(crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute)%vDir)*0.25f;
					fBack = Max(fBack, fD);
					//}					
					//m_penOwner->SetModelColor(0xFFFFFF10);
				}	
			}
		}
		
		fDistance = ClampDn(fDistance-fBack, 0.0f);
		m_fDistance = fDistance;
		vBase += vFront*fDistance;		
		
//������ ���� ����	//(Modify Camera Behavior)(0.1)
		//����� �ö󰥶� ȭ���� �ﷷ�Ÿ��� ȿ�� ����.
/*
		if (bFollowCrossHair) {
			FLOAT3D vTarget = vBase-ppw->m_vRayHit;
			FLOAT fLen = vTarget.Length();
			if (fLen>0.01) {
				vTarget/=fLen;
			} else {
				vTarget = FLOAT3D(0,1,0);
			}
			
			FLOAT3D vX;
			FLOAT3D vY = m.GetColumn(2);
			FLOAT3D vZ = vTarget;
			vZ.Normalize();
			
			if (Abs(vY%vZ)>0.9f) {
				vY = -m.GetColumn(3);
			}
			
			vX = vY*vZ;
			vX.Normalize();
			vY = vZ*vX;
			vY.Normalize();
			m_vZLast = vZ;
			
			m(1,1) = vX(1); m(1,2) = vY(1); m(1,3) = vZ(1);
			m(2,1) = vX(2); m(2,2) = vY(2); m(2,3) = vZ(2);
			m(3,1) = vX(3); m(3,2) = vY(3); m(3,3) = vZ(3);
			DecomposeRotationMatrixNoSnap(pl.pl_OrientationAngle, m);
		}
*/		//����
//������ ���� ��	//(Modify Camera Behavior)(0.1)

    	pl.pl_PositionVector = vBase;//0610 kwon �߰�
	/* //0610 kwon ����.
		if (m_bFixed) {
			pl.pl_PositionVector = GetPlacement().pl_PositionVector;
			pl.pl_OrientationAngle = ANGLE3D(0,-90, 0);
			m_fDistance = (pl.pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
			MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
		} else {
			pl.pl_PositionVector = vBase;
		}
	*/		
		
		// set camera placement
		// ī�޶��� ��ġ ����.
		// en_plPlacement���� ������.
		SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
	};
	
procedures:
	Main(EViewInit eInit) 
	{
		// remember the initial parameters
		ASSERT(((CEntity*)eInit.eidOwner)!=NULL);
		m_penOwner = eInit.eidOwner;
//0213 ���ۺ��� 3��Ī
		m_iViewType = VT_3RDPERSONVIEW; //eInit.vtView;
		m_bFixed = eInit.bDeathFixed;
		ASSERT(IsOfClass(m_penOwner, &CPlayer_DLLClass));

		// init as model
		InitAsEditorModel();
		SetFlags(GetFlags()|ENF_CROSSESLEVELS);
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL|EPF_MOVABLE);
		SetCollisionFlags(ECF_IMMATERIAL);

		SetFlagOff(ENF_PLACCHANGED|ENF_PROPSCHANGED);
		SetFlagOff(ENF_PROPSCHANGED);

		// set appearance
		SetModel(MODEL_MARKER);
		SetModelMainTexture(TEXTURE_MARKER);
/*//0815 ����.
		// add to movers list if needed
		if (m_iViewType == VT_PLAYERDEATH) 
		{
			AddToMovers();
		}
*/
		m_bLockRotate = FALSE;

		SendEvent(EStart());
		wait() 
		{
			on (EBegin) : 
			{ 				
				resume; 
			}
			on (EStart) : 
			{
				// PlayerView�� Ȱ���� �����ϸ� ���� ī�޶� ��ġ�� ������(�� �ѹ���.)
				//SetCameraPosition();
				// �ڿ��� �����ϴ� ���.
				if(m_bUseTraceMode)
				{
					// �� ������ ���� ȣ��Ǵ� �κ�.
					SetCameraPosition();
				}
				// ���� ���.
				else
				{
					SetCameraPositionEx();
				}
				en_plLastPlacement = GetPlacement();  // remember old placement for lerping
				m_vTargetLast = ((CPlayer&) *m_penOwner).GetPlayerWeapons()->m_vRayHit;
				resume;
			};
			on (EEnd) : { stop; }
			otherwise() : { resume; }
		}
		// cease to exist
		Destroy(FALSE);

		return;
	};
};

