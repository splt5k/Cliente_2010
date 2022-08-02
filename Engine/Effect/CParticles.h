//������ ���� ����	//(Remake Effect)(0.1)
#ifndef __CPARTICLES_H__
#define __CPARTICLES_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <vector>
#include <algorithm>
#include "CParticle.h"

class ENGINE_API CParticles
{
public:
	typedef std::vector<CParticle>::iterator iterator;
	typedef std::vector<CParticle>::const_iterator const_iterator;
protected:
	class IsParticleDead
	{
	public:
		bool operator()(const CParticle &particle)
		{
			if(!particle.Live()) return true;
			else return false;
		}
	};
public:
	//���������� �߰��Ѵ�. �����ϴ� ���� ���� ������ ����, �Ѱ����� �ƴ�. �޸� ����.
	UINT AddNew(UINT wantAddCount);
	//���������� �����. ���ϰ��� ���� ������ ��.
	void EraseLast(UINT wantEraseCount)
	{
		if(wantEraseCount <= 0) return;
		if(wantEraseCount > m_vectorParticle.size()) wantEraseCount = m_vectorParticle.size();
		m_vectorParticle.erase(m_vectorParticle.end() - wantEraseCount, m_vectorParticle.end());
		if(m_uiAllCount >= wantEraseCount) m_uiAllCount -= wantEraseCount;
		else m_uiAllCount = 0;
	}
public:
	inline CParticle &operator[](UINT i)		{ return m_vectorParticle[i];		}
	inline bool Empty()							{ return m_vectorParticle.empty();	}
	inline UINT Size()							{ return m_vectorParticle.size();	}

	inline void RenderQuad(MEX mexWidth, MEX mexHeight);
	void RenderLine(MEX mexWidth, MEX mexHeight);
	void RenderTrail(MEX mexWidth, MEX mexHeight);
	void RenderTrailVelocity(MEX mexWidth, MEX mexHeight);
protected:
	static UINT	m_uiAllCount;	//��ü ��ƼŬ�� ����.
	std::vector<CParticle> m_vectorParticle;
};

#endif //__CPARTICLES_H__
//������ ���� ��	//(Remake Effect)(0.1)
