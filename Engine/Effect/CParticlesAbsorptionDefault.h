//������ ���� ����	//(Remake Effect)(0.1)
#ifndef __CPARTICLESABSORPTIONDEFAULT_H__
#define __CPARTICLESABSORPTIONDEFAULT_H__

#include <Engine/Base/Types.h>
#include <Engine/Math/Functions.h>
#include "CParticlesAbsorption.h"

class ENGINE_API CParticlesAbsorptionDefault : public CParticlesAbsorption
{
public:
	CParticlesAbsorptionDefault();
	virtual ~CParticlesAbsorptionDefault();

	//NEW_DELETE_DEFINITION(CParticlesAbsorptionDefault);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
	
	virtual CParticlesAbsorption *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	//content
	//instance
};

#endif //__CPARTICLESABSORPTIONDEFAULT_H__
//������ ���� ��	//(Remake Effect)(0.1)
