
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISignBoard.h>
#include <Engine/Templates/Stock_CFontData.h>		// Font Use ...
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/World/World.h>
#include <stdio.h>

CUISignBoard*	_UISignBoard = NULL;

extern BOOL g_bIsMalEng;
// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
ENGINE_API void ShowSignBoard ( int nIndex )
{
	_UISignBoard->ShowSingBoard ( nIndex );
}


// ----------------------------------------------------------------------------
// Name : CUIRefine()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISignBoard::CUISignBoard()
{
	m_ptdBaseTexture = NULL;
	Clear ();
	
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIRefine()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISignBoard::~CUISignBoard()
{
	if( m_ptdBaseTexture )
	{
		_pTextureStock->Release( m_ptdBaseTexture );
		m_ptdBaseTexture = NULL;
	}

	Clear ();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISignBoard::Clear ()
{

	m_nTextureIndex = -1;
	m_bStop			= FALSE;		// 정지 상태인감?
	m_fShowTime		= 0;
	m_iAlpha		= 0x01;
	m_FadeInOp		= 64;
	m_FadeOutOp		= -16;
	m_bText			= false;
	
	m_strLocalName.Clear ();

	m_bVisible		= false;
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISignBoard::Create( int iCountry )
{



}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISignBoard::ShowSingBoard ( int nIndex, SLONG slZoneIndex )
{
	SLONG	slIndex = g_slZone;

	if ( nIndex < 0 ) 
	{
		return;
	}
	else if ( nIndex != -1 && m_nTextureIndex == nIndex )	// 같은 지역 일 경우
	{
		// 지역 표시 시간만 다시 설정 해 준다.
		m_fShowTime	= SHOW_TIME;
		return;
	}

	if( slZoneIndex != -1 )
	{
		slIndex = slZoneIndex;
	}

	// 존이름과 지역 인덱스로 지역 이름 파일을 로딩한다.
	if ( LoadSignBoardTexture ( slIndex, nIndex ) == FALSE )
	{
		return;
	}
	
	// 데이터 초기화
	m_bVisible = TRUE;
	m_strLocalName	=_S( 2062,  "데이터" );		
	m_fShowTime		= SHOW_TIME;
	m_op			= m_FadeInOp;
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISignBoard::Render ()
{
	if ( !m_bVisible )	return;
	
	// 화면에 보여 주고 있다면 렌더링 시간을 체크 해서 렌더링을 중지 할 것 인지 확인 한다.
	if ( m_bStop )	
	{
		if ( _pTimer->GetHighPrecisionTimer().GetMilliseconds() - m_tmStartTime >= m_fShowTime )
		{
			m_op 	= m_FadeOutOp;
			m_bStop = FALSE;
		}
	}
	else
	{	
		m_iAlpha += m_op;

		if ( m_iAlpha >= 0xff )
		{
			m_iAlpha	= 0xff;
			m_bStop		= TRUE;
			m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
		}
		else if ( m_iAlpha <= 0 )
		{
			// END
			Clear ();
		}
	}

	/*
	if ( m_nTextureIndex == -1 )
	{
	// render 텍스트 출력
		_pUIMgr->GetDrawPort()->SetFont( _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Standard.fnt" ) ) );
		_pUIMgr->GetDrawPort()->SetTextScaling( 1.0 );
		_pUIMgr->GetDrawPort()->SetTextAspect( 1.0f );
		_pUIMgr->GetDrawPort()->SetTextShadow( +2 );
		
		int iFontSize =  _pUIMgr->GetDrawPort()->dp_fTextScaling + _pUIMgr->GetDrawPort()->dp_pixTextCharSpacing;

		int pX = _pUIMgr->GetDrawPort()->GetWidth() / 2 - m_strLocalName.Length() * iFontSize / 2;
		int pY = 150;
		
		_pUIMgr->GetDrawPort()->PutTextC ( "No SignBoard Data", pX, pY, 0xffffff00 | m_iAlpha );
		
		_pUIMgr->GetDrawPort()->EndTextEx();
	
	}
	*/
	if( m_ptdBaseTexture == NULL )
	{
		return;
	}

	int pX = _pUIMgr->GetDrawPort()->GetWidth() / 2 - m_fTexWidth / 2;
	int pY = SIGNBOARD_SY;

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	_pUIMgr->GetDrawPort()->AddTexture( pX, pY,	pX + m_fTexWidth , pY + m_fTexHeight, 
										m_rtCurrent.U0, m_rtCurrent.V0, 
										m_rtCurrent.U1, m_rtCurrent.V1,
										0xffffff00 | m_iAlpha );

	_pUIMgr->GetDrawPort()->FlushRenderingQueue();


}


//------------------------------------------------------------------------------
// CUISignBoard::LoadSignBoardTexture
// Explain: 존번호(nZoneIndex)와 지역번호(nLocalIndex), 국가 설정으로 
//          파일명을 생성하여 테스쳐 파일을 로딩 한다.
//
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUISignBoard::LoadSignBoardTexture( int nZoneIndex, int nLocalIndex )
{
	CTString strSignBoardName;
	CTString strDir = SIGNBOARD_DEFAULT_DIR;

	// 이미 테스쳐가 로딩 되어 있으면 해제
	if( m_ptdBaseTexture != NULL )
	{
		_pTextureStock->Release( m_ptdBaseTexture );
		m_ptdBaseTexture = NULL;
	}

	// += 존 이름 += 지역 번호  
	strSignBoardName.PrintF ( "sb_%d_%d", nZoneIndex, nLocalIndex );
	
	// 공성지역 (7번 존의 11번 지역) 인 경우에는 공성중이지 아닌지에 따라서 표시가 다름
	if( nZoneIndex == 7 && nLocalIndex == 11 )
	{
		if( _pUISWDoc->IsWar() )
			strSignBoardName += "_war";
	}

	extern INDEX	g_iCountry;
	
	switch( g_iCountry )
	{
	case KOREA:
		strDir += "\\Korea\\";
		strSignBoardName += ".tex";
		break;
	
	case TAIWAN:
	case TAIWAN2:
		strDir += "\\Taiwan\\";
		strSignBoardName += "_t.tex";
		break;
	
	case CHINA: 
		strDir += "\\China\\";
		strSignBoardName += "_c.tex";
		break;

	case THAILAND:
		strDir += "\\Thai\\";
		strSignBoardName += ".tex";
		break;
	case JAPAN:
		strDir += "\\Japan\\";
		strSignBoardName += ".tex";
		break;
	
	case MALAYSIA:
		if(g_bIsMalEng){
			strDir += "\\Malaysia_e\\";
			strSignBoardName += ".tex";
		}
		else {
			strDir += "\\Malaysia\\";
			strSignBoardName += ".tex";
		}
		break;
	case USA:
		strDir += "\\USA\\";
		strSignBoardName += ".tex";
		break;
	case BRAZIL:
		strDir += "\\BRZ\\";
		strSignBoardName += ".tex";
		break;
	case HONGKONG:
		if(g_bIsMalEng)
		{
			strDir += "\\HK_e\\";
			strSignBoardName += ".tex";
		}
		else
		{
			strDir += "\\HK\\";
			strSignBoardName += ".tex";
		}
		break;
	case GERMANY:
		strDir += "\\GER\\";
		strSignBoardName += ".tex";
		break;
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
		strDir += "\\SPA\\";
		strSignBoardName += ".tex";
		break;
	case FRANCE:
		strDir += "\\FRA\\";
		strSignBoardName += ".tex";
		break;
	case POLAND:
		strDir += "\\POL\\";
		strSignBoardName += ".tex";
		break;

	case TURKEY:
		strDir += "\\TURKEY\\";
		strSignBoardName += ".tex";
		break;
	}

	// 테스쳐 로딩
	m_ptdBaseTexture = _pTextureStock->Obtain_t ( strDir + strSignBoardName );

	if ( m_ptdBaseTexture == NULL )
	{
		return FALSE;
	}

	m_fTexWidth		= m_ptdBaseTexture->GetPixWidth();
	m_fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	m_rtCurrent.SetUV( 0, 0, m_fTexWidth, m_fTexHeight, m_fTexWidth, m_fTexHeight );

	return TRUE;
}