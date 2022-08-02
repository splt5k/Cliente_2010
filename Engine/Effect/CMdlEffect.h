//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CMDLEFFECT_H__
#define __CMDLEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Models/ModelObject.h>

#include "CEffect.h"

class CTag;
class CAnyProjection3D;
class CDrawPort;
class CEffectControl;

class ENGINE_API CMdlEffect : public CEffect
{
public:
	CMdlEffect();
	virtual ~CMdlEffect();

	//NEW_DELETE_DEFINITION(CMdlEffect);

//������ ���� ����	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//������ ���� ��	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	inline void SetColor(COLOR col)		{ m_colModel = col;		}
	inline COLOR GetColor()				{ return m_colModel;	}

	inline void SetStretch(const FLOAT3D &stretch)	{ m_vStretch = stretch;	}
	inline const FLOAT3D &GetStretch()				{ return m_vStretch;	}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}
	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	void SetPosTag(ptr_tag &ptrTag)	{ if(ptrTag.NotNull()) m_ptrPosTag = ptrTag;	}
	ptr_tag GetPosTag()				{ return m_ptrPosTag;							}

	void SetEffectControl(const CEffectControl &effectControl);
	CEffectControl *GetEffectControl()	{ return m_pEffectControl;	}
	void EraseEffectControl();

	BOOL SetModelObject(const CTFileName &strMdlFileName, const CTFileName &strTexFileName);
	const CTFileName &GetMdlFileName()		{ return m_strMdlFileName;		}
	const CTFileName &GetMdlTexFileName()	{ return m_strTextureFileName;	}

	inline void SetOverDraw(BOOL overDraw)	{ m_bOverDraw = overDraw;	}
	inline BOOL GetOverDraw()				{ return m_bOverDraw;		}

	//���߿� �ʿ� ������ �� ����.(GUI�ϼ���)
	inline FLOAT GetMdlAnimLength() { if(m_pModelObject) return m_pModelObject->GetAnimLength(0); else return 0.0f; }

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

protected:
	CMdlEffect( CMdlEffect &other ) {}
	CMdlEffect &operator =(CMdlEffect &other) {return *this;}
	
protected:
	//content variable
	CTFileName			m_strMdlFileName;		//MDL�����̸�
	CTFileName			m_strTextureFileName;	//MDL���� ���� Texture�� �����̸�
	COLOR				m_colModel;				//MDL�� ����.
	FLOAT3D				m_vStretch;				//MDL�� ũ��.
	EFFECT_OF_TAG_TYPE	m_eRotation;			//tag�� ȸ���� ��ŭ ������ ���� ���ΰ�?
	EFFECT_OF_TAG_TYPE	m_ePosition;			//tag�� �̵��� ��ŭ ������ ���� ���ΰ�?
	CEffectControl		*m_pEffectControl;		//effect control �������� NULL
	BOOL				m_bOverDraw;			//Depth test�� ���� �׸����ΰ�?
	//instance variable
	CModelObject		*m_pModelObject;		//���� �ε��� MDL
	CTextureData		*m_pModelTextureData;	//���� �ε�� Texture
	FLOAT3D				m_vStartPosition;		//���۽� ��ġ
	FLOATmatrix3D		m_matStartRotation;		//���۽� ����
	FLOAT3D				m_vPostion;				//dynamic pos�� ���
	ANGLE3D				m_vAngle;				//dynamic angle�� ���
	ptr_tag				m_ptrPosTag;			//dynamic pos�� �������� ���� Tag, ������ ���ڸ�
	BOOL				m_bOnlyStart;
};

#endif //__CMDLEFFECT_H__
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
