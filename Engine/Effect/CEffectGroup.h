//������ ���� ����	//(Remake Effect)(0.1)
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CEFFECTGROUP_H__
#define __CEFFECTGROUP_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable : 4786)
#include <string>
#include <vector>
#include <Engine/Math/Projection.h>
#include "CEffect.h"

#include "CTagManager.h"
#include "CRefCountPtr.h"

class CTag;

class ENGINE_API CEffectGroup
{
protected:
	struct EffectKey
	{
		//�� ����Ʈ �׷쳻���� effect�� �����ϴ� ���� key value
		INDEX		m_iKeyValue;
		//���۽ð�, CEffectGroup�� ���۽ð����κ����� offset time.
		FLOAT		m_fStartTime;
		//����Ʈ �̸�. �� �̸��� ������ ����Ʈ�Ŵ��ͷκ��� ���� ����Ʈ�� �����´�.
		std::string	m_strEffectName;
		//����Ʈ�� �̸��� ������ ����Ʈ�Ŵ����� ��û�ؼ� ������� ����Ʈ.
		CEffect		*m_pCreatedEffect;
	};

	template <class Type>
	struct FindKeyValue
	{
		FindKeyValue(INDEX keyValue) : CompareKeyValue(keyValue) {}
		bool operator() (const Type& effectkey) { return effectkey.m_iKeyValue == CompareKeyValue; }
		INDEX CompareKeyValue;
	};
	
	struct TagKey
	{
		//�� ����Ʈ �׷쳻���� tag�� �����ϴ� ���� key value
		INDEX		m_iKeyValue;
		//������ EFFECT_KEY�� ���� effect key value
		INDEX		m_iEffectKeyValue;
		//���� �ð�, CEffectGroup�� ���۽ð����κ����� offset time
		FLOAT		m_fSettingTime;
		//������ tag�� ����Ű�� �̸�.
		std::string	m_strTagName;
		//������ ���� tag�� ����Ű�� ptr_tag
		ptr_tag		m_ptrReserveTag;
	};

public:
	typedef	CRefCountPtr< CTag >		ptr_tag;
	typedef std::vector< EffectKey >	vector_effect_key;
	typedef std::vector< TagKey >		vector_tag_key;

public:
	CEffectGroup();
	~CEffectGroup();

	//NEW_DELETE_DEFINITION(CEffectGroup);

//������ ���� ����	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//������ ���� ��	//(Remake Effect)(0.1)

	//content fill function
	INDEX AddEffect(FLOAT fStartTimeFromStart, std::string strEffectName);
	INDEX AddTagForEffect(FLOAT fSettingTimeFromStart, INDEX iEffectKeyValue, std::string tagName);
	//AddTagForAllEffect�Լ��� tag�� �߰��Ҷ��� effect�� �߰��� �����ٰ� ����. �ȱ׷���� ������ ������ �� ����.
	void  AddTagForAllEffect(FLOAT fSettingTimeFromStart, std::string tagName);

	void GetEffectKeyInfo(INDEX index, FLOAT &fStartTimeFromStart, std::string &effectName);
	void GetTagKeyInfo(INDEX index, FLOAT &fSettingTimeFromStart, INDEX &iAddedEffectKeyValue, std::string &TagName);

	EffectKey* FindEffectKey(INDEX keyValue);
	TagKey* FindTagKey(INDEX keyValue);

	//vector�� index�� �̿��Ͽ� key value�� �����Ѵ�.
	INDEX FindEffectKeyValue(INDEX index)	{ return m_vectorEffectKey[index].m_iKeyValue;	}
	INDEX FindTagKeyValue(INDEX index)		{ return m_vectorTagKey[index].m_iKeyValue;		}

	INDEX GetEffectCount() { return m_vectorEffectKey.size(); }
	INDEX GetTagCount() { return m_vectorTagKey.size(); }

	BOOL RemoveEffect(INDEX keyValue);
	BOOL RemoveTag(INDEX keyValue);

	void Start(FLOAT time, const ptr_tag &ptrTag);
	void Start(FLOAT time);
	BOOL Process(FLOAT time);
	CEffectGroup *Copy();
	void Stop(FLOAT leftTime = FLT_MAX);	//FLT_MAX�� ��ٸ���ŭ ��ٸ� �� stop.

	//access function
	inline void SetName(std::string &name) { m_strName = name; }
	inline void SetName(const char *name) { m_strName = name; }
	inline const std::string &GetName() const { return m_strName; }

	inline BOOL Active() { return m_bActive; }
	inline void SetTagManager(CTagManager *ptm) { m_pTagManager = ptm;	}
	inline CTagManager *GetTagManager()			{ return m_pTagManager;	}
	inline void SetWantTagManager(CTagManager *ptm) { m_pWantTagManager = ptm;	}
	inline CTagManager *GetWantTagManager()			{ return m_pWantTagManager;	}

	inline void SetProjection(CAnyProjection3D &apr)	{ m_pProjection = &apr;	}

	inline const vector_effect_key &GetEffectKeyVector()	{ return m_vectorEffectKey;	}
	inline const vector_tag_key &GetTagKeyVector()			{ return m_vectorTagKey;	}
	
	inline FLOAT GetStartTime()								{ return m_fStartTime;		}

	//life� �� ���� ��������.
	inline void SetSpeedMul(FLOAT fSpeedMul)	{ if(fSpeedMul > 0) m_fSpeedMul = fSpeedMul; else m_fSpeedMul = 1.0f;	}
	inline FLOAT GetSpeedMul()					{ return m_fSpeedMul; }

	inline void SetgERType(EFF_RENDER_TYPE gERType) { m_gERType = gERType; }
	inline EFF_RENDER_TYPE GetgERType(void)		{ return m_gERType; }

	inline void SetgERSubType(EFF_RENDER_SUB_TYPE gERSubType)		{ m_gERSubType = gERSubType; }
	inline EFF_RENDER_SUB_TYPE GetgERSubType(void)				{ return m_gERSubType; }
protected:
	CEffectGroup(const CEffectGroup &other ) {}
	CEffectGroup &operator =(CEffectGroup &other) {return *this;}

protected:
	//key����
	vector_effect_key	m_vectorEffectKey;
	vector_tag_key		m_vectorTagKey;

	//CEffectGroup��ü�� ���õ� �͵�.
	//content variable
	std::string		m_strName;
	//EFFECT_KEY�� m_fStartTime, m_pCreatedEffect->GetName()

	//instance variable
	CTagManager		*m_pTagManager;
	CTagManager		*m_pWantTagManager;
	BOOL			m_bActive;
	FLOAT			m_fStartTime;
	FLOAT			m_fLastProcessedTime;
	INDEX			m_iNextEffectKeyValue;
	INDEX			m_iNextTagKeyValue;
	BOOL			m_bDirectTag;
	ptr_tag			m_ptrDirectTag;
	CAnyProjection3D	*m_pProjection;
	FLOAT			m_fSpeedMul;
	EFF_RENDER_TYPE	m_gERType;
	EFF_RENDER_SUB_TYPE m_gERSubType;
	//EFFECT_KEY�� m_pCreatedEffect, TAG_KEY�� instance variable��.
};

#endif //__CEFFECTGROUP_H__
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ��	//(Remake Effect)(0.1)
