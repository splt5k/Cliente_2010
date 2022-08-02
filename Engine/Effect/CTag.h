//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
// CTag.h
//

#if !defined(__CTAG_H__)
#define __CTAG_H__
#ifdef PRAGMA_ONCE
#	pragma once
#endif
#include <Engine/Base/Memory.h>

#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/base/Types.h>
#include <Engine/base/CTString.h>
#include <deque>
#include <string>

//������ ���� ����	//(Remake Effect)(0.1)
#define TAG_INDICATOR_STRING	"_TAG_"
#define TAG_V_INDICATOR_STRING	"_TAG_V_"
#define TAG_T_INDICATOR_STRING	"_TAG_T_"
#define TAG_G_INDICATOR_STRING	"_TAG_G"

enum VERTEX_TAG_CONVERT_TYPE
{
	VTCT_VERTEX		= 1,
	VTCT_TRIANGLE	= 2,
	VTCT_GROUP		= 3,
};
//������ ���� ��	//(Remake Effect)(0.1)

extern CModelInstance *g_pmiAxis;

enum TAG_TYPE
{
	TT_NOTHING		= 0,
	TT_WORLD		= 1,
	TT_ENTITY		= 2,
	TT_SKA			= 3,
	TT_GROUP		= 4,
	TT_COUNT		= 5,
	TT_FORCE_DWORD	= 0xFFFFFFFF,
};

class TagInfo
{
public:
	TagInfo() : m_fTime( 0.0f ), m_vPos(0,0,0), m_qRot(1,0,0,0) {}
	TagInfo(const FLOAT time, const FLOAT3D &pos, const FLOATquat3D &rot)
		: m_fTime(time), m_vPos(pos), m_qRot(rot) {}

	FLOAT			m_fTime;	//�� ������ ��Ÿ���� �ð�
	FLOAT3D			m_vPos;		//Ư���ð��϶��� ��ġ
	FLOATquat3D		m_qRot;		//Ư���ð��϶��� ����
};

class ENGINE_API CTag
{
public:
	CTag();
	virtual ~CTag() = 0;
	virtual CTag *Copy() = 0;

	void Render(COLOR col = 0xCCCCCCCC, FLOAT3D stretch = FLOAT3D(0.2f, 0.2f, 0.2f));

	inline TagInfo &CurrentTagInfo()	{ return m_tiTagInfo;	}

	//access functions
	inline TAG_TYPE GetType()					{ return m_eType; }

	inline std::string &GetName()				{ return m_strName; }
	inline void SetName(const std::string &name){ m_strName = strupr(const_cast<char*>(name.c_str()));	}
	inline void SetName(const char *name)		{ m_strName = strupr(const_cast<char*>(name));			}

	inline FLOAT3D &GetOffsetPos()						{ return m_vOffsetPos; }
	inline void SetOffsetPos(const FLOAT3D &pos)		{ m_vOffsetPos = pos; }
	inline void SetOffsetPos(FLOAT x, FLOAT y, FLOAT z)	{ m_vOffsetPos(1) = x; m_vOffsetPos(2) = y; m_vOffsetPos(3) = z; }

	inline FLOATquat3D &GetOffsetRot()					{ return m_qOffsetRot; }
	inline void SetOffsetRot(const ANGLE3D &rot)		{ m_qOffsetRot.FromEuler(rot); }
	inline void SetOffsetRot(const FLOATquat3D &rot)	{ m_qOffsetRot = rot; }

	//Tag info�� ������ �ð�.
	inline FLOAT GetMaxTimeLength()				{ return m_fMaxTimeLength; }
	inline void SetMaxTimeLength(FLOAT len)		{ m_fMaxTimeLength = len; }

	//active ���� �Լ���, �߿���.
	inline void Activate()						{ ++m_iActive; }
	inline void Deactivate()					{ if((--m_iActive) < 0) m_iActive=0; }
	inline BOOL Active()						{ return m_iActive; }

	//CRefCountPtr�� ���� ���� ���ؼ� �ݵ�� �ʿ��� �Լ���.
	inline ULONG AddRef()						{ return ++m_ulRefCount; }
	inline ULONG Release()						{ return --m_ulRefCount; }
protected:
	//tag info control functions
	void AddNewTagInfo(const TagInfo &info);
	void AddNewTagInfo(FLOAT time, const Matrix12 &mInfo, const Matrix12 &mStrInfo);
	void AddNewTagInfo(FLOAT time, const FLOATquat3D &rot, const FLOAT3D &pos, const FLOAT3D &stretch);

	//for copy
	void SetContent(CTag *pTag);
	
protected:
	static INDEX	m_iAxisRefCount;	//axis ska model�� ���������Ҷ� ���Ǵ� ���� ǥ���Ѵ�.
	std::string		m_strName;			//�̸�
	TAG_TYPE		m_eType;			//�±��� Ÿ��
	INDEX			m_iActive;			//�� tag�� Ȱ�������� ��Ÿ���� count
	TagInfo			m_tiTagInfo;		//���� tag info
	FLOAT3D			m_vOffsetPos;		//�߽ɿ��������� ������
	FLOATquat3D		m_qOffsetRot;		//�߽��� ȸ������������ ������
	FLOAT			m_fOldestTime;		//���� ������ TagInfo�� �ð�
	FLOAT			m_fMaxTimeLength;	//TagInfo�� �״� �ð�
	ULONG			m_ulRefCount;		//CRefCountPtr���� ���Ǵ� Reference count
};

#endif /* __CTAG_H__ */
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
