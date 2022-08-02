//������ ���� ����	//(Remake Effect)(0.1)
#ifndef __CPARTICLESPROCESSCONTROL_H__
#define __CPARTICLESPROCESSCONTROL_H__

#include <Engine/Base/Memory.h>
#include "CParticles.h"
#include "CParticlesCommonProcess.h"

enum CONTROL_TYPE
{
	CT_NONE		= -1,
	CT_CIRCLE	= 0,
	CT_SPIRAL	= 1,
};

class ENGINE_API CParticlesProcessControl : public CParticlesCommonProcess
{
public:
	CParticlesProcessControl();

	//NEW_DELETE_DEFINITION(CParticlesProcessControl);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	//usePtcPos�� ���̸� ��ƼŬ�� �����ǿ� ���� ����, �ƴϸ� velocity�̿�, velocity�̿��� trail����.
	void SetCircleInfo(BOOL usePtcPos, FLOAT angleSpeed);
	void SetSpiralInfo(BOOL usePtcPos, FLOAT angleSpeed, FLOAT heightSpeed);

	inline void SetDependRadius(BOOL bDependRadius)	{ m_bDependRadius = bDependRadius;	}
	inline BOOL GetDependRadius()					{ return m_bDependRadius;			}

	CONTROL_TYPE GetControlType()	{ return m_eControlType;	}
	FLOAT	GetAngleSpeed()			{ return m_fAngleSpeed;		}
	FLOAT	GetHeightSpeed()		{ return m_fHeightSpeed;	}
	BOOL	IsUseParticlePos()		{ return m_bUseParticlePos;	}

	virtual CParticlesCommonProcess *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	//content
	CONTROL_TYPE	m_eControlType;
	BOOL			m_bDependRadius;
	//circle or spiral
	BOOL	m_bUseParticlePos;
	FLOAT	m_fAngleSpeed;
	FLOAT	m_fHeightSpeed;		//spiral only
	//instance
};

#endif //__CPARTICLESPROCESSCONTROL_H__
//������ ���� ��	//(Remake Effect)(0.1)
