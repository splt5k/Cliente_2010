897
%{
#include "StdH.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIPortal.h>
%}

// Message View Type
enum MessageViewType
{
	0 MSGVIEW_PORTAL	"Open Portal",
	1 MSGVIEW_CUSTOM	"Custom MessageBox By StringID",
	2 MSGVIEW_STRING	"String Message",
};

class CMessageView : CRationalEntity 
{
name      "MessageView";
thumbnail "Thumbnails\\MessageHolder.tbn";
features  "HasName", "IsTargetable";

properties:
  1 CTString m_strName          "Name" 'N'	= "MessageView",
  3 CTString		m_strDescription	"Contents"		= "",				// ���� �Է��� ��츦 ���(����)
  2 CTFileName		m_fnmMessage		"Message" 'M'	= CTString(""),		// ȭ�Ͽ��� �о���� ��츦 ���.
  4 FLOAT m_fDistance  "Distance" 'D'		= 2.0f,				// Ȥ�ö� �÷��̾���� �Ÿ��� üũ�ؾ��Ѵٸ�...
  5 BOOL m_bActive "Active" 'A'				= TRUE,
  6 CEntityPointer m_penNext "Next" 'X',						// Ȯ�� ��ư�� ��������... ���� �޽��� â?
  10 enum MessageViewType	eViewType	"MessageView Type"	= MSGVIEW_CUSTOM,
  11 BOOL			m_bShowOnce			"Show Once"		= FALSE,

  31 INDEX			m_iStringIDContent1	"Content 1"		= -1,				// �޽��� �ڽ��� ����.
  32 INDEX			m_iStringIDContent2	"Content 2"		= -1,
  33 INDEX			m_iStringIDContent3	"Content 3"		= -1,
  34 INDEX			m_iStringIDContent4	"Content 4"		= -1,
  35 INDEX			m_iStringIDContent5	"Content 5"		= -1,

  41 INDEX			m_iStringIDChoice1	"Choice 1"		= -1,				// �������� ����
  42 INDEX			m_iStringIDChoice2	"Choice 2"		= -1,
  43 INDEX			m_iStringIDChoice3	"Choice 3"		= -1,
  44 INDEX			m_iStringIDChoice4	"Choice 4"		= -1,
  45 INDEX			m_iStringIDChoice5	"Choice 5"		= -1,

  // MessageViewType�� MSGVIEW_CUSTOM�� ���.
  15 CTString		m_strTitle			"Title"			= "Custom Message Box",	// ���� Ÿ��Ʋ �Է�
  51 INDEX			m_iStringTitleID	"Title ID"		= -1,					// ���� Ÿ��Ʋ �Է�

  60 CTString		m_strStringTitle		"String Title"		= "",
  61 CTString		m_strStringIDContent1	"String Content 1"	= "",				// �޽��� �ڽ��� ����.
  62 CTString		m_strStringIDContent2	"String Content 2"	= "",
  63 CTString		m_strStringIDContent3	"String Content 3"	= "",
  64 CTString		m_strStringIDContent4	"String Content 4"	= "",
  65 CTString		m_strStringIDContent5	"String Content 5"	= "",

  71 CTString		m_strStringIDChoice1	"String Choice 1"	= "",				// �������� ����
  72 CTString		m_strStringIDChoice2	"String Choice 2"	= "",
  73 CTString		m_strStringIDChoice3	"String Choice 3"	= "",
  74 CTString		m_strStringIDChoice4	"String Choice 4"	= "",
  75 CTString		m_strStringIDChoice5	"String Choice 5"	= "",

  // MessageViewType�� MSGVIEW_PORTAL�� ���.
  20 INDEX		m_iZone					"Zone Number"		= -1,
  21 INDEX		m_iExtra				"Extra Number"		= -1,

components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\MessageHolder.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\MessageHolder.tex"

functions:
  void OpenMessageView()
  {
		if(eViewType == MSGVIEW_CUSTOM)
		{
			if(!_pUIMgr->DoesMessageBoxLExist(MSGLCMD_STORY_INTRO))
			{
				// Ÿ��Ʋ ǥ���ϱ�.
				if(m_iStringTitleID != -1)
				{
					// Create skill learn message box
					_pUIMgr->CreateMessageBoxL( _S( m_iStringTitleID, "" ), UI_NONE, MSGLCMD_STORY_INTRO );
	
					// ���� ǥ���ϱ�.		
					if(m_iStringIDContent1 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE, _S( m_iStringIDContent1, "") );
					}
	
					if(m_iStringIDContent2 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE, _S( m_iStringIDContent2, "" ) );
					}
	
					if(m_iStringIDContent3 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE, _S( m_iStringIDContent3, "" ) );
					}
	
					if(m_iStringIDContent4!= -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE, _S( m_iStringIDContent4, "" ) );
					}
	
					if(m_iStringIDContent5!= -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE, _S( m_iStringIDContent5, "" ) );
					}
					//--------------------------------------------------------------------------------------------------------
					// ������ ǥ���ϱ�.
					if(m_iStringIDChoice1 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, _S( m_iStringIDChoice1, "" ) );
					}
	
					if(m_iStringIDChoice2 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, _S( m_iStringIDChoice2, "" ) );
					}
	
					if(m_iStringIDChoice3 != -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, _S( m_iStringIDChoice3, "" ) );
					}
	
					if(m_iStringIDChoice4!= -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, _S( m_iStringIDChoice4, "" ) );
					}
	
					if(m_iStringIDChoice5!= -1)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, _S( m_iStringIDChoice5, "" ) );
					}

					if(m_bShowOnce)
					{
					// Active�Ǿ�������...
					if(m_bActive)
					{
						m_bActive	= FALSE;
						}
					}
				}				
			}
	  }
	  else if (eViewType == MSGVIEW_STRING)
	  {
	  		if(!_pUIMgr->DoesMessageBoxLExist(MSGLCMD_STORY_INTRO))
			{
				// Ÿ��Ʋ ǥ���ϱ�.
				if(m_strStringTitle.Length() > 0)
				{
					// Create skill learn message box
					_pUIMgr->CreateMessageBoxL( m_strStringTitle, UI_NONE, MSGLCMD_STORY_INTRO );
	
					// ���� ǥ���ϱ�.		
					if(m_strStringIDContent1.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE,  m_strStringIDContent1 );
					}
	
					if(m_strStringIDContent2.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE,  m_strStringIDContent2 );
					}
	
					if(m_strStringIDContent3.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE,  m_strStringIDContent3 );
					}
	
					if(m_strStringIDContent4.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE,  m_strStringIDContent4 );
					}
	
					if(m_strStringIDContent5.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, TRUE,  m_strStringIDContent5 );
					}
					//--------------------------------------------------------------------------------------------------------
					// ������ ǥ���ϱ�.
					if(m_strStringIDChoice1.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, m_strStringIDChoice1 );
					}
	
					if(m_strStringIDChoice2.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, m_strStringIDChoice2 );
					}
	
					if(m_strStringIDChoice3.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, m_strStringIDChoice3 );
					}
	
					if(m_strStringIDChoice4.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, m_strStringIDChoice4 );
					}
	
					if(m_strStringIDChoice5.Length() > 0)
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_STORY_INTRO, FALSE, m_strStringIDChoice5 );
					}

					if(m_bShowOnce)
					{
					// Active�Ǿ�������...
					if(m_bActive)
					{
						m_bActive	= FALSE;
						}
					}
				}				
			}
	  }
  };


procedures:
	Main()
	{
		InitAsEditorModel();
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
		SetCollisionFlags(ECF_IMMATERIAL);

		// set appearance
		SetModel(MODEL_MARKER);
		SetModelMainTexture(TEXTURE_MARKER);
		wait() 
		{
			on (ETrigger eTrigger): 
			{
				if (!m_bActive) 
				{
					resume;
				}
			
				OpenMessageView();
				// UIManager�� �̿��ؼ� ���� ������ֱ�.
				resume;
			}
			on (EActivate): 
			{
				m_bActive = TRUE;
				resume;
			}
			on (EDeactivate): 
			{
				m_bActive = FALSE;
				resume;
			}
		}
		return;
	}
};

