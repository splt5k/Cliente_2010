//������ ���� ���� Water ����		04.22
#ifndef _CNICEWATER_H_
#define _CNICEWATER_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Base/FileName.h>

// Water�� ������ �����ϴ� Ŭ����.
class ENGINE_API CNiceWater
{
public:
	CNiceWater();
	CNiceWater(CEntity* pEntity);
	virtual			~CNiceWater();

	void			Render();			// �ù�(SKA �������� ��κ��� ó����.)
	void			Clear(void);

public:
	FLOATplane3D	m_plPlane;			// plane in absolute space
	CEntity*		m_pWaterEntity;		// Water�� ��ƼƼ.

//protected:
	BOOL			m_bWave;
	BOOL			m_bReflection;		// �ݻ� ����.
	BOOL			m_bWireFrame;
	BOOL			m_bRenderSky;		// �ݻ�� �ϴ��� �������� ���ΰ�?
	BOOL			m_bRenderModels;	// �ݻ�� �ϴ��� �������� ���ΰ�?

	FLOAT			m_fBumpMat11;		// ���� ���� ��İ�.
	FLOAT			m_fBumpMat12;
	FLOAT			m_fBumpMat21;
	FLOAT			m_fBumpMat22;

	FLOAT			m_fTexFlowX;		// �ؽ��İ� �귯���� �ӵ� �� ����.
	FLOAT			m_fTexFlowY;

//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.17
	CTextureObject	m_toBump;			// ������ �ؽ���1
	CTextureObject	m_toWater;			// �� �ؽ���.
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.17

	CTFileName		m_fnBumpName;
	CTFileName		m_fnWaterName;
};

#endif  /* include-once check. */
//������ ���� �� Water ����			04.22