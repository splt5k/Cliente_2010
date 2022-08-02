//������ ���� ����	//(Remake Effect)(0.1)
#ifndef __CPARTICLESEMITTERSPHERE_H__
#define __CPARTICLESEMITTERSPHERE_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include "CParticles.h"
#include "CParticlesEmitter.h"
#include "CForce.h"
#include "CSphereSpace.h"
#include "FreeMemStack.h"

class ENGINE_API CParticlesEmitterSphere : public CParticlesEmitter
{
public:
	CParticlesEmitterSphere();
	virtual ~CParticlesEmitterSphere();

	//NEW_DELETE_DEFINITION(CParticlesEmitterSphere);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	inline PARTICLES_EMITTER_TYPE GetType()		{ return m_petType; }
	inline void SetForce(const CForce &force, FLOAT delayTime)
	{
		m_force = force;
		m_fDelayTime = delayTime;
	}
	inline const CForce &GetForce()	{ return m_force; }
	inline FLOAT GetDelayTime()		{ return m_fDelayTime; }
	inline void SetShape(CSphereDoubleSpace &sphereDoubleSpace)
	{
		m_cdsShape = sphereDoubleSpace;
	}
	inline const CSphereDoubleSpace &GetShape() { return m_cdsShape; }
	
	virtual CParticlesEmitter *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
	virtual void SetShapeSize(FLOAT size);
protected:
	//content
	CForce	m_force;			//�ʱ� �ӵ�, ���ӵ� ����.
	FLOAT	m_fDelayTime;		//�ʱ� �ӵ� ����. ���ʵ��� ���� �޳��� ����.
	CSphereDoubleSpace	m_cdsShape;	//��ƼŬ�� �ʱ���ġ ����.
	//instance
};

#endif //__CPARTICLESEMITTERSPHERE_H__
//������ ���� ��	//(Remake Effect)(0.1)
