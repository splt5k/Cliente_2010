//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CLIGHTEFFECT_H__
#define __CLIGHTEFFECT_H__

#pragma warning(disable :4786)

#include <Engine/Base/Memory.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Graphics/Texture.h>
#include "CEffect.h"

class CTag;
class CAnyProjection3D;
class CDrawPort;
class CLightSource;
class CWorld;


class ENGINE_API CLightEffect : public CEffect
{
public:
	CLightEffect();
	virtual ~CLightEffect();

	//NEW_DELETE_DEFINITION(CLightEffect);
	
//������ ���� ����	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//������ ���� ��	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	//acess function
	inline void SetAmbientMode(BOOL am)			{ m_bAmbientMode = am;		}
	inline BOOL GetAmbientMode()				{ return m_bAmbientMode;	}
	inline void SetMaxFallOff(FLOAT falloff)	{ m_fMaxFallOff = falloff;	}
	inline FLOAT GetMaxFallOff()				{ return m_fMaxFallOff;		}

	inline void SetLightColor(COLOR color)		{ m_colLight = color;		}
	inline COLOR GetLightColor()				{ return m_colLight;		}
	
	static inline void SetWorld(CWorld *pWorld)						{ m_pWorld = pWorld;			}
	static inline void SetTerrain(CTerrain *pTerrain)				{ m_pTerrain = pTerrain;		}
	static inline void SetLightTexture(CTextureData *ptdLightMap)	{ m_ptdLightMap = ptdLightMap;	}

protected:
	void SetupLightSource( CLightSource &lsNew);

protected:
	//content variable
	FLOAT				m_fMaxFallOff;	//���� ����
	COLOR				m_colLight;		//���� ����, ����� Terrain������ ���õ�. BSP������ �Դµ�.
	BOOL				m_bAmbientMode;
	//instance variable
	CEntity				*m_penDummy;	//CLightSource�� ��ġ���� �Ȱ��� �־ CEntity�� �ʿ��ϴ�.
	CLightSource		*m_plsLight;	//������ CLightSource�� Point����Ʈ��.
	static CWorld		*m_pWorld;		//CEntity�� �����ϴµ� �ʿ��ϴ�. Start������ ����.
	static CTerrain		*m_pTerrain;	//Rendering �ϴµ� �ʿ��ϴ�. EndRender������ ����.
	static CTextureData	*m_ptdLightMap;	//CEntity�� �����ϴµ� �ʿ��ϴ�. EndRender������ ����.
};

#endif //__CLIGHTEFFECT_H__
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)