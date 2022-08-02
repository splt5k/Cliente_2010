1049
%{
#include "StdH.h"
#include "Common/RestrictedFieldContainer.h"
%}

class CRestrictedField : CRationalEntity 
{
	name      "Restricted Field";
	thumbnail "Thumbnails\\DoorController.tbn";
	features  "HasName", "IsTargetable";
	
properties :
	1 CTString m_strName          "Name" 'N' = "Restricted Field",
    2 CTString m_strDescription = "",
    3 FLOAT du "du" 'u' = 1.0f,
    4 FLOAT dv "dv" 'v' = 1.0f,
    5 FLOAT dw "dw" 'w' = 1.0f,
	6 INDEX iMobCount "rest of enemies" = 0,
	7 CEntityPointer m_penTarget   "Target Entity",
	8 BOOL m_bActive "Active" 'A' = TRUE,              // is field active
        
components:
	
	1 model				MODEL_DOORCONTROLLER     "Data\\Models\\Editor\\DoorController.mdl",
	2 editor texture	TEXTURE_DOORCONTROLLER   "Data\\Models\\Editor\\DoorController.tex",
        
functions:
	
	// constructor
	// inserts itself into the container upon creation
	void CRestrictedField(void)
	{
		InsertRestrictedField(this);

	}
	
	// destructor
	// removes itself into the container upon destruction
	void ~CRestrictedField(void)
	{
		RemoveRestrictedField(this);
	}
	
	const CTString& GetDescription(void)const
	{
		((CTString&)m_strDescription).PrintF("-><none>");
		return m_strDescription;
	}
	SLONG GetUsedMemory(void)
	{
		SLONG slUsedMemory = sizeof(CRestrictedField) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
		slUsedMemory += m_strName.Length();
		slUsedMemory += m_strDescription.Length();
		return slUsedMemory;
	}
	
procedures:
	
	Main(EVoid)
	{
		// set properties
		InitAsEditorModel();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);
		
		// set appearance
		GetModelObject()->StretchModel(FLOAT3D(du, dv, dw));
		SetModel(MODEL_DOORCONTROLLER);
		SetModelMainTexture(TEXTURE_DOORCONTROLLER);
		ModelChangeNotify();
		return;
	}
};