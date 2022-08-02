//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CTRACEEFFECT_H__
#define __CTRACEEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Graphics/Vertex.h>
#include <vector>
#include <Engine/Graphics/Texture.h>

#include "CEffect.h"
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

class CTag;

class ENGINE_API CTraceEffect : public CEffect
{
public:
	typedef std::vector< UWORD >		vector_index;
	typedef std::vector< GFXVertex >	vector_pos;
	typedef std::vector< GFXColor >		vector_color;
	typedef std::vector< GFXTexCoord >	vector_texcoord;
	typedef std::vector< FLOAT >		vector_time;
public:
	CTraceEffect();
	virtual ~CTraceEffect();

	//NEW_DELETE_DEFINITION(CTraceEffect);

//������ ���� ����	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//������ ���� ��	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	BOOL SetTexture(const CTFileName &filename);
	const CTFileName &GetTexFileName()	{ return m_strTextureFileName;	}

	//acess function
	inline void SetTraceTime(FLOAT time)	{ m_fTraceTime = time; }
	inline FLOAT GetTraceTime()				{ return m_fTraceTime; }

	inline void SetTimeInterval(FLOAT time)	{ m_fTimeInterval = time; }
	inline FLOAT GetTimeInterval()			{ return m_fTimeInterval; }

	inline void SetTwinklePeriod(FLOAT twinklePeriod)	{ m_fTwinklePeriod = twinklePeriod;	}
	inline FLOAT GetTwinklePeriod()						{ return m_fTwinklePeriod;			}

	inline void SetColor(COLOR colBegin, COLOR colEnd)	{ m_colTraceBegin = colBegin; m_colTraceEnd = colEnd; }
	inline COLOR GetColorBegin()						{ return m_colTraceBegin;			}
	inline COLOR GetColorEnd()							{ return m_colTraceEnd;				}

	inline void SetCapEnd(BOOL bCapEnd)					{ m_bCapEnd = bCapEnd;				}
	inline BOOL GetCapEnd()								{ return m_bCapEnd;					}

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	static void InitializeShaders();
	static void FinalizeShaders();
protected:
	CTraceEffect( CTraceEffect &other ) {}
	CTraceEffect &operator =(CTraceEffect &other) {return *this;}

	inline void RemakeColorsAndTexCoords(INDEX cntVtx);
	
protected:
	//content variable
	CTFileName			m_strTextureFileName;	//texture�� ���� �̸�
	COLOR				m_colTraceBegin;		//trace�� ���۵Ǵ� �κ��� ����
	COLOR				m_colTraceEnd;			//trace�� ������ �κ��� ����
	FLOAT				m_fTraceTime;			//trace�Ǵ� �ð�
	FLOAT				m_fTimeInterval;		//trace�Ǵ� ����, ���ۺκ��� �׻� ��.
	FLOAT				m_fTwinklePeriod;		//��ü�� ���ĸ� ������������ ����.
	BOOL				m_bCapEnd;				//���κ��� �����Ѵ�.
	PredefinedBlendType	m_eBlendType;
	//instance variable
	static ULONG		m_ulVertexProgramNoTex;
	static ULONG		m_ulPixelProgramNoTex;
	static ULONG		m_ulVertexProgramTex;
	static ULONG		m_ulPixelProgramTex;
	INDEX				m_iTraceCount;			//trace�Ǵ� �ð��� ������ ����� ��
	CTextureData		*m_ptdTexture;			//���� �ε��� texture
	vector_index		m_vectorIndex;			//�׷��� tri�� index��
	vector_pos			m_vectorPos;			//�׷��� vertex�� pos
	vector_color		m_vectorColor;			//�׷��� vertex�� color
	vector_texcoord		m_vectorTexCoord;		//�׷��� vertex�� texure ��ǥ, �� texture�� ���õ� ��츸 ����.
	vector_time			m_vectorAddTime;		//vertex�� �߰��� �ð��� ǥ���Ѵ�.
	FLOAT				m_fTwinkleValue;		//��¦��¦��, �������Ӹ��� ����.
	INDEX				m_iTagCount;
};

#endif //__CTRACEEFFECT_H__
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
