//������ ���� ����	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesAbsorptionDefault.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesAbsorptionDefault);

CParticlesAbsorptionDefault::CParticlesAbsorptionDefault()
{
	m_patType = PAT_DEFAULT;
}

CParticlesAbsorptionDefault::~CParticlesAbsorptionDefault()
{
}

CParticlesAbsorption *CParticlesAbsorptionDefault::Copy()
{
	CParticlesAbsorptionDefault *pRet = new CParticlesAbsorptionDefault;
	pRet->SetContent(*this);
	return pRet;
}

void CParticlesAbsorptionDefault::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	const UINT count = particles.Size();
	UINT eraseCount = 0;
	for(UINT i=0; i<count-eraseCount; ++i)
	{
		MoveProcess( particles[i], fDeltaTime );
		if( !LifeProcess( particles[i], fDeltaTime ) )//�׾���.
		{
			//end�� ������ ����鼭 end���� ������ ���簪�� ���.
			//�� �ڸ��� ���� �ǵ��� ������ �ٲ����Ƿ� ���⿡�� ���� �ٽ� ó��.
			++eraseCount;
			particles[i] = particles[count-eraseCount];
			--i;
		}
	}
	particles.EraseLast(eraseCount);
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesAbsorptionDefault::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesAbsorption::Read(pIS);

	is.ExpectID_t("PADF");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error");
	}
}

void CParticlesAbsorptionDefault::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesAbsorption::Write(pOS);

	os.WriteID_t("PADF");
	os << (UBYTE)CURRENT_VERSION;
}
//������ ���� ��	//(Remake Effect)(0.1)
