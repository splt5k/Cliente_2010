#ifndef __CSPLINEPATHEFFECT_H__
#define __CSPLINEPATHEFFECT_H__

#include <string>
#include <vector>

#include "CEffect.h"
#include "CSampleSpline.h"

enum SplinePathDirType
{
	SPDT_NONE = 0,
	SPDT_MOVEDIR,		//z���� quaternion���� ȸ�� ��Ų��.
	SPDT_INVMOVEDIR,	//MOVEDIR�� �ݴ����
	SPDT_TANGENT,		//y���� quaternion���� ȸ�� ��Ų��.
	SPDT_INVTANGENT,	//TANGENT�� �ݴ����
	SPDT_BINORMAL,		//x���� quaternion���� ȸ�� ��Ų��.
	SPDT_INVBINORMAL,	//BINORMAL�� �ݴ����
};

class ENGINE_API CSplinePathEffect : public CEffect
{
public:
	struct sSplinePathInfo
	{
		//contents
		CSampleSpline< FLOAT3D, CCubicSplineInterpolate<FLOAT3D> >	ssPath;
		std::string			strEffectName;
		//instance
		//SplinePathDirType	eSplinePathDirType;
		//CSampleSpline< FLOATquat3D, CLerp<FLOATquat3D> >			ssDir;
	};
	typedef std::vector<CEffect*>					effect_vector;
	typedef std::vector<sSplinePathInfo>			pathinfo_vector;
	typedef CRefCountPtr<CTag>						tag_ptr;
	typedef std::vector<tag_ptr>					tag_vector;
public:
	CSplinePathEffect();
	virtual ~CSplinePathEffect();

	//NEW_DELETE_DEFINITION(CSplinePathEffect);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Stop(FLOAT leftTime);

	virtual void SetNotRenderAtThisFrame();

	INDEX AddSplinePath(const sSplinePathInfo &info);
	BOOL DeleteSplinePath(INDEX index);
	sSplinePathInfo *GetSplinePath(INDEX index);
	INDEX GetSplinePathCount()	{ return m_vectorSplinePath.size();	}
	void ClearSplinePath()		{ m_vectorSplinePath.clear();		}
	
	effect_vector& GetvectorEffect(void) { return m_vectorEffect; }

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}

	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	inline void SetOffsetPos(const FLOAT3D &offset)			{ m_vOffsetPos = offset;}
	inline const FLOAT3D &GetOffsetPos()					{ return m_vOffsetPos;	}
protected:
	CSplinePathEffect(const CSplinePathEffect& other) {}
	CSplinePathEffect &operator =(const CSplinePathEffect &other) { return *this; }

	void ClearInstance();
protected:
	//contents
	pathinfo_vector			m_vectorSplinePath;
	EFFECT_OF_TAG_TYPE		m_eRotation;			//tag�� ȸ���� ��ŭ ������ ���� ���ΰ�?
	EFFECT_OF_TAG_TYPE		m_ePosition;			//tag�� �̵��� ��ŭ ������ ���� ���ΰ�?
	FLOAT3D					m_vOffsetPos;
	//instance
	effect_vector	m_vectorEffect;
	tag_vector		m_vectorTag;
	ptr_tag			m_ptrGoalTag;
	FLOAT3D			m_vTagPos;
	FLOATquat3D		m_qTagRot;
};

#endif //__CSPLINEPATHEFFECT_H__
