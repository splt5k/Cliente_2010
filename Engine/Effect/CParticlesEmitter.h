//������ ���� ����	//(Remake Effect)(0.1)
#ifndef __CPARTICLESEMITTER_H__
#define __CPARTICLESEMITTER_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <Engine/Math/Functions.h>
#include "CParticles.h"
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

enum PARTICLES_EMITTER_TYPE
{
	PET_NONE	= 0,
	PET_SPHERE	= 1,
	PET_CONE	= 2,
	PET_CYLINDER= 3,
};

enum EMIT_AXIS_TYPE
{
	EAT_PLUS_X	= 0,
	EAT_MINUS_X	= 1,
	EAT_PLUS_Y	= 2,	//�⺻��
	EAT_MINUS_Y	= 3,
	EAT_PLUS_Z	= 4,
	EAT_MINUS_Z	= 5,
};

class ENGINE_API CParticlesEmitter// : public CParticlesProcess
{
public:
	CParticlesEmitter();
	virtual ~CParticlesEmitter() = 0;

	virtual void Read(CTStream *istrFile) = 0;
	virtual void Write(CTStream *ostrFile) = 0;

	inline void SetTotalCount(ULONG count)	{ m_ulTotalCount = m_ulLeftCount = count;	}
	inline ULONG GetTotalCount()			{ return m_ulTotalCount;	}
	inline ULONG GetLeftCount()				{ return m_ulLeftCount;		}
	inline void SetEmitSpeed(FLOAT countPerSec)	{ m_fCountPerSec = countPerSec;	}
	inline FLOAT GetEmitSpeed()					{ return m_fCountPerSec;		}

	inline void SetPosition(const FLOAT3D &center)	{ m_vCenter = center;		}
	inline const FLOAT3D &GetPosition()				{ return m_vCenter;			}
	inline void SetDirection(const FLOATquat3D &dir){ m_quatDir = dir;			}
	inline const FLOATquat3D &GetDirection()		{ return m_quatDir;			}

	inline void SetParticleType(PARTICLE_RENDER_TYPE ePtcType)	{ m_ePtcType = ePtcType;	}
	inline PARTICLE_RENDER_TYPE GetParticleType()				{ return m_ePtcType;		}

	inline void SetParticlePrototype(CParticlePrototype &particlePrototype)
	{
		m_particlePrototype = particlePrototype;
	}
	inline CParticlePrototype &GetParticlePrototype()	{ return m_particlePrototype; }

	inline BOOL IsLocalType()				{ return m_bLocalType;		}
	inline void SetLocalType(BOOL bLocal)	{ m_bLocalType = bLocal;	}

	void Restart();

	inline PARTICLES_EMITTER_TYPE GetType()	{ return m_petType; }
	
	virtual CParticlesEmitter *Copy() = 0;
	virtual void Process(CParticles &particle, const FLOAT fDeltaTime) = 0;
	virtual void SetShapeSize(FLOAT size) = 0;
protected:
	inline void SetContent(CParticlesEmitter &emitter)
	{
		m_bLocalType = emitter.IsLocalType();
		SetTotalCount(emitter.m_ulTotalCount);
		m_fCountPerSec = emitter.m_fCountPerSec;
		m_particlePrototype = emitter.m_particlePrototype;
	}
	INDEX TimeProcessAndAddParticle(FLOAT deltaTime, CParticles &particles);

	//content
	PARTICLES_EMITTER_TYPE	m_petType;	//������ Ÿ��.
	ULONG	m_ulTotalCount;		//�� ���� ����
	FLOAT	m_fCountPerSec;		//���� �ӵ�.
	CParticlePrototype	m_particlePrototype;	//��ƼŬ�� ������Ÿ��, ��ġ, ����, �ӵ�, ���ӵ��� ������ ����.
	BOOL	m_bLocalType;
	//instance
	PARTICLE_RENDER_TYPE	m_ePtcType;
	FLOAT3D		m_vCenter;		//Emitter�� �߽�
	FLOATquat3D	m_quatDir;		//Emitter�� ����, �ִٸ�...
	ULONG	m_ulLeftCount;		//���� ���� ���� ���ɼ�
	FLOAT	m_fProcessedTime;	//����� �ð�.
};

typedef CRefCountPtr<CParticlesEmitter> ptr_emitter;

#endif //__CPARTICLESEMITTER_H__
//������ ���� ��	//(Remake Effect)(0.1)
