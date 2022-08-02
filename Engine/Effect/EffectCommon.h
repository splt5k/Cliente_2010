//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
#ifdef PRAGMA_ONCE
  #pragma once
#endif

class CAnyProjection;
class CDrawPort;
class CTextureData;
class CTFileName;

extern CTextureObject g_toLightMap;

#include "CEffect.h"

ENGINE_API extern FLOAT g_fEffectDistance;
ENGINE_API extern void FinalizeShaders(void);
ENGINE_API extern void InitializeShaders(void);
//����Ʈ ���� �ʱ�ȭ �۾��� �����Ѵ�.
ENGINE_API extern BOOL Initialize_EffectSystem(const CTFileName *fnm = NULL);
//����Ʈ ���� ������ �۾��� �����Ѵ�.
ENGINE_API extern BOOL Finalize_EffectSystem();
//����Ʈ ������ ���� ���� ����
ENGINE_API extern BOOL PrepareRender_EffectSystem(CTerrain *pTerrain, CWorld *pWorld);
//����Ʈ ������ ����
ENGINE_API extern BOOL BeginRender_EffectSystem(EFFECT_TYPE effectType, CAnyProjection3D &apr, CDrawPort *pdp);
//����Ʈ ������ ��
ENGINE_API extern BOOL EndRender_EffectSystem(EFFECT_TYPE effectType, BOOL bRestoreOrtho);
//����Ʈ ����
ENGINE_API extern void Reset_EffectSystem();
//����Ʈ ���� �ε�
ENGINE_API extern BOOL Open_EffectSystem(const CTFileName &fnm);
//����Ʈ ���� ����
ENGINE_API extern BOOL Save_EffectSystem(const CTFileName &fnm);

CEffect *CreateFromType(EFFECT_TYPE et);

//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)