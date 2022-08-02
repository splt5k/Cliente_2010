//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CEFFECT_H__
#define __CEFFECT_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>
#pragma warning(disable :4786)
#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include <Engine/Entities/Entity.h>
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

class CTag;

enum EFF_RENDER_TYPE
{
	ER_NORMAL = 0,	// CRenderer::RenderEffects() ���� �׷����� Effect
	ER_IN_UI,		// UI ���ο��� �׷��� Effect -- HUD_DrawSkaModel()
	ER_COUNT,
};

enum EFF_RENDER_SUB_TYPE
{
	ERS_NORMAL		= 0,
	ERS_CASHSHOP,
	ERS_MATRYOSHKA,
	ERS_CHAR_CREATE,
};
enum EFFECT_TYPE
{
	ET_NOTHING			= 0,
	ET_TERRAIN,
	ET_LIGHT,
	ET_PARTICLE,
	ET_SKA,
	ET_MDL,
	ET_TRACE,
	ET_SOUND,
	ET_SPLINEBILLBOARD,
	ET_ORBIT,
	ET_SHOCKWAVE,
	ET_SPLINEPATH,
	ET_CAMERA,
	ET_ENTITY,
	ET_COUNT,
};

//effect�� play ���� ����
enum EFFECT_STATE
{
	ES_NOT_STARTED	= 0,	//���� �������� ���� ����, �ʱ� ������.
	ES_PLAYING		= 1,	//�÷��� ��, �ѹ��� play��.
	ES_PLAY_END		= 3,	//�÷��̰� ���� ����.
};

enum EFFECT_OF_TAG_TYPE
{
	EOTT_NONE	= 0,
	EOTT_START	= 1,
	EOTT_ALWAYS	= 2,
};

class ENGINE_API CEffect
{
public:
	typedef std::vector<CTag *> my_vector;
public:
	CEffect();
	virtual ~CEffect() = 0;

//������ ���� ����	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile) = 0;
	virtual void Write(CTStream *ostrFile) = 0;
//������ ���� ��	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE) = 0;
	virtual BOOL Process(FLOAT time) = 0;
	virtual CEffect *Copy() = 0;
	virtual void Render() {};
	virtual void Stop(FLOAT leftTime);

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	virtual INDEX GetWantTagCount()						{ return 0;		}
	virtual const char *GetWantTagName(INDEX index)		{ return "";	}
	virtual void SetWantTag(INDEX index, ptr_tag tag)	{ NULL;			}

	void AttachToTag(const ptr_tag &ptrTag)	{ if(ptrTag.NotNull()) m_ptrAttachTag = ptrTag; }
	void DetachTag()					{ m_ptrAttachTag.SetNull();		}
	ptr_tag GetTag()					{ return m_ptrAttachTag;		}

	//access time
	inline BOOL Playing()				{ return (ES_PLAYING == m_eState);	}
	inline EFFECT_STATE GetState()		{ return m_eState;					}
	inline EFFECT_TYPE GetType()		{ return m_eType;					}
	inline FLOAT GetLastProcessedTime()	{ return m_fLastProcessTime;		}

	inline FLOAT GetLeftTime()			{ return m_fLeftTime; }

	//Fade�� ���� �ʴ� ���. fade Ÿ�ӵ� Setting
	inline void SetLifeTime(FLOAT time) { m_fLifeTime = time; m_fFadeInTime = 0; m_fFadeOutTime = 0; }
	inline FLOAT GetLifeTime()			{ return m_fLifeTime; }

	//Fade�� ���� ���. Life Time�� Setting.
	inline void SetFadeTime(FLOAT fadeInTime, FLOAT noFadeTime, FLOAT fadeOutTime)
	{
		m_fLifeTime = fadeInTime + noFadeTime + fadeOutTime;
		m_fFadeInTime = fadeInTime;
		m_fFadeOutTime = fadeOutTime;
	}
	inline FLOAT GetFadeInTime()					{ return m_fFadeInTime; }
	inline FLOAT GetFadeOutTime()					{ return m_fFadeOutTime; }

	//�ٿ���� ����
	inline void SetBoundingSphereRadius(FLOAT radius)	{ m_fBoundingSphereRadius = radius;	}
	inline FLOAT GetBoundingSphereRadius()				{ return m_fBoundingSphereRadius;	}

	//�ݺ�ȸ�� ����, Fade�� ���� �� ��� glittering(��¦��)�� ����.
	inline SLONG GetRepeatCount()					{ return m_slRepeat;	}
	inline void SetRepeatCount(SLONG cnt)			{ m_slRepeat = cnt;		}
	
	inline void SetName(const std::string &name)	{ m_strName = name;		}
	inline void SetName(const char *name)			{ m_strName = name;		}
	inline const std::string &GetName() const		{ return m_strName;		}

	inline BOOL IsNotRenderAtThisFrame()			{ return m_bNotRenderAtThisFrame; }
	virtual void SetNotRenderAtThisFrame();

	//life� �� ���� ��������.
	inline void SetSpeedMul(FLOAT fSpeedMul)	{ m_fSpeedMul = fSpeedMul;	}
	inline FLOAT GetSpeedMul()					{ return m_fSpeedMul;		}

	//Entity
	inline void SetOwner(CEntity *pen)			{ m_penOwner = pen;			}
	inline CEntity *GetOwner()					{ return m_penOwner.ep_pen;	}

	inline void SetERType(EFF_RENDER_TYPE erType) { m_ERType = erType; }
	inline EFF_RENDER_TYPE GetERType(void)		{ return m_ERType; }

	inline void SetERSubType(EFF_RENDER_SUB_TYPE ersType )		{ m_ERSubType = ersType; }
	inline EFF_RENDER_SUB_TYPE GetERSubType(void)				{ return m_ERSubType; }

protected:
	CEffect( CEffect &other ) {}
	CEffect &operator =(CEffect &other) {return *this;}

	//�ڽĵ鿡�� ���������� ���̴� �κе�.
	BOOL PreStart(FLOAT time, BOOL bRestart);		//Start�� �������� �պκп� ����.
	void PostStart() { m_eState = ES_PLAYING; } //Start�� �������� �޺κп� ����.
	
	BOOL PreProcess(FLOAT time, BOOL &bRetValue, BOOL &bRenderInThisFrame, FLOAT &retDeltaTime, FLOAT &retProcessedTime);	//Process�� �������� �պκп� ����
	void PostProcess() {}	//Process�� �������� �޺κп� ����
	
	BOOL PreRender();
	void PostRender() {}

	void SetContent(const CEffect *pEffect);	//CEffect�� Content�� Setting��.

	inline FLOAT GetFadeValue(FLOAT fProcessedTime)
	{
		//fade in, [0, fade-in time)
		if(fProcessedTime <= m_fFadeInTime) {
			if(m_fFadeInTime > 0.0f || m_fFadeInTime < 0.0f)
			{
				return (fProcessedTime / m_fFadeInTime);
			}

			return 0.0f;
		}
		//fade out, [fade-in time, life time - fade-out time)
		else if(m_fLeftTime <= m_fFadeOutTime) {
			if(m_fFadeOutTime > 0.0f || m_fFadeOutTime < 0.0f)			
			{
				return (m_fLeftTime / m_fFadeOutTime);
			}

			return 0.0f;
		}
		//no fade, [life time - fade-out time, life time)
		else if(fProcessedTime <= m_fLifeTime) return 1.0f;

		return 0.0f;
	}
protected:
	//content variable
	EFFECT_TYPE		m_eType;				//Ÿ��, ��ӹ��� ��ü�� ��Ÿ��.
	std::string		m_strName;				//�̸�. ��ϵǴ� �̸��� ��Ÿ��
	FLOAT			m_fLifeTime;			//������ ����, �׻� ���̾�� ��.
	SLONG			m_slRepeat;				//�� �� �ݺ��Ұ��ΰ��� ��Ÿ��. life time * Ƚ�� ��ŭ�� �ð����� effect ����, -1�̸� ���ѹݺ�
	FLOAT			m_fFadeInTime;			//Fade-in�Ǵ� �ð�
	FLOAT			m_fFadeOutTime;			//Fade-Out�Ǵ� �ð�
	FLOAT			m_fBoundingSphereRadius;//Bounding-Sphere�� ������.
	//instance variable
	EFFECT_STATE	m_eState;				//������ �÷��� ����
	FLOAT			m_fLeftTime;			//���� �÷��� �ð�
	FLOAT			m_fStartTime;			//���۵Ǿ��ٸ� �� ���۽ð�
	FLOAT			m_fLastProcessTime;		//���������� Process�� ȣ��� �ð�
	ptr_tag			m_ptrAttachTag;			//�پ��ִ� Tag
	BOOL			m_bNotRenderAtThisFrame;//���̸� �� �����ӵ��� Rendering���� ����.
	FLOAT			m_fSpeedMul;			//���� ������.
	CEntityPointer	m_penOwner;			//�� Effect�� �ٿ����� Entity, Null�� �������.

	EFF_RENDER_TYPE m_ERType;				// ����Ʈ�� �׷����� ���� ���� Ÿ�� ����
	EFF_RENDER_SUB_TYPE m_ERSubType;
};

#endif //__CEFFECT_H__
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)