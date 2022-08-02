#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIOXQuizEvent.h>

extern INDEX	g_iCountry;
extern INDEX	g_iEnterChat;
extern CDrawPort* _pdpMain;

#define QUIZBOX_TOP_HEIGHT		26
#define QUIZBOX_DESC_HEIGHT		164
#define QUIZBOX_GAP_HEIGHT		3
#define QUIZBOX_BOTTOM_HEIGHT	7
#define QUIZBOX_DESC_MAX_LINE	50
#define QUIZ_STRING_LINESPACING	3

// --------------------------------------------------------------------------
// Name : CUIOXQuizEvent()
// Desc : Constructor
// --------------------------------------------------------------------------
CUIOXQuizEvent::CUIOXQuizEvent()
{
	m_nCorrect	= 0;
	m_nQuizNum	= 0;
	m_nMaxCharCount		= 0;
	m_nChatLineHeight	= 0;
	m_nCurQuizInsert	= 0;
	m_nCurQuizCount		= 0;
	m_dOldTime	= 0.0f;
	m_dCurTime	= 0.0f;
	m_bQuizStart = FALSE;
}
// --------------------------------------------------------------------------
// Name : ~CUIOXQuizEvent()
// Desc : Destructor
// --------------------------------------------------------------------------
CUIOXQuizEvent::~CUIOXQuizEvent()
{
	Destroy();
}

// --------------------------------------------------------------------------
// Name : Create()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_nChatLineHeight = _pUIFontTexMgr->GetFontHeight() + QUIZ_STRING_LINESPACING;

	m_nMaxCharCount	= ( nWidth - QUIZ_STRING_OFFSETX - 9 ) /
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, nWidth, 22 );

	// Create texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();	

	// UV Coordinate of each part
	// Background
	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
/*	m_rtMiddleScrollL.SetUV( 0, 27, 40, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollM.SetUV( 40, 27, 176, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollR.SetUV( 176, 27, 216, 29, fTexWidth, fTexHeight );*/
	m_rtMiddleScrollL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleScrollM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleScrollR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtMiddleL.SetUV( 0, 31, 40, 33, fTexWidth, fTexHeight );
	m_rtMiddleM.SetUV( 40, 31, 176, 33, fTexWidth, fTexHeight );
	m_rtMiddleR.SetUV( 176, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );	

}

// --------------------------------------------------------------------------
// Name : ResetPosition()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI - GetWidth() ), ( pixMinJ + _pUIMgr->GetRadar()->GetHeight() ) );
}

// --------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// --------------------------------------------------------------------------
// Name : AddQuizDescString()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::AddQuizDescString( CTString &strQuizString, COLOR colQuizColor )
{
	INDEX	nLength = strQuizString.Length();

	if( nLength == 0 ) return;

		// wooss 051002
	if(g_iCountry == THAILAND){
	
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strQuizString);
		INDEX	nChatMax= (MAX_BUFFINFO_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nThaiLen == 0 )
			return;

		if( nThaiLen <= nChatMax )
		{
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if( strQuizString[iPos] == '\n' || strQuizString[iPos] == '\r' )
					break;
			}

			if( iPos == nLength )
			{
				m_strQuizString[m_nCurQuizInsert] = strQuizString;
				m_colQuizString[m_nCurQuizInsert] = colQuizColor;
				m_nCurQuizInsert++;
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strQuizString.Split( iPos, strTemp2, strTemp );

				m_strQuizString[m_nCurQuizInsert] = strTemp2;
				m_colQuizString[m_nCurQuizInsert] = colQuizColor;
				m_nCurQuizInsert++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddQuizDescString( strTemp, colQuizColor );
			}
		}
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strQuizString,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strQuizString[iPos] == '\n' || strQuizString[iPos] == '\r' )
					break;
			}

			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2 ;
				strQuizString.Split( iPos, strTemp2, strTemp );

				int nInsertIndex = m_nCurQuizInsert;

				if( nInsertIndex >= QUIZBOX_DESC_MAX_LINE )
					nInsertIndex = m_nCurQuizInsert = 0;
				
				m_strQuizString[nInsertIndex] = strTemp2;
				m_colQuizString[nInsertIndex] = colQuizColor;
				m_nCurQuizInsert++;

				AddQuizDescString( strTemp, colQuizColor );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2 ;
				strQuizString.Split( iPos, strTemp2, strTemp );

				int nInsertIndex = m_nCurQuizInsert;

				if( nInsertIndex >= QUIZBOX_DESC_MAX_LINE )
					nInsertIndex = m_nCurQuizInsert = 0;

				m_strQuizString[nInsertIndex] = strTemp2;
				m_colQuizString[nInsertIndex] = colQuizColor;
				m_nCurQuizInsert++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddQuizDescString( strTemp, colQuizColor );
			}	

		}
		
	}
	else
	{
		if( nLength <= m_nMaxCharCount )
		{
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if( strQuizString[iPos] == '\n' || strQuizString[iPos] == '\r' )
					break;
			}

			if( iPos == nLength )
			{
				m_strQuizString[m_nCurQuizInsert] = strQuizString;
				m_colQuizString[m_nCurQuizInsert] = colQuizColor;
				m_nCurQuizInsert++;
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2;
				strQuizString.Split( iPos, strTemp2, strTemp );

				m_strQuizString[m_nCurQuizInsert] = strTemp2;
				m_colQuizString[m_nCurQuizInsert] = colQuizColor;
				m_nCurQuizInsert++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddQuizDescString( strTemp, colQuizColor );
			}
		}
		else
		{
			int		nSplitPos = m_nMaxCharCount;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strQuizString[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar ) nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strQuizString[iPos] == '\n' || strQuizString[iPos] == '\r' )
					break;
			}

			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp, strTemp2 ;
//				if( m_nCurQuizCount < QUIZBOX_DESC_MAX_LINE )
//					m_nCurQuizCount++;
				strQuizString.Split( iPos, strTemp2, strTemp );

				int nInsertIndex = m_nCurQuizInsert;

				if( nInsertIndex >= QUIZBOX_DESC_MAX_LINE )
					nInsertIndex = m_nCurQuizInsert = 0;
				
				m_strQuizString[nInsertIndex] = strTemp2;
				m_colQuizString[nInsertIndex] = colQuizColor;
				m_nCurQuizInsert++;

				AddQuizDescString( strTemp, colQuizColor );
			}
			else
			{
				// Split string
				CTString	strTemp, strTemp2 ;
				strQuizString.Split( iPos, strTemp2, strTemp );

				int nInsertIndex = m_nCurQuizInsert;

				if( nInsertIndex >= QUIZBOX_DESC_MAX_LINE )
					nInsertIndex = m_nCurQuizInsert = 0;

				m_strQuizString[nInsertIndex] = strTemp2;
				m_colQuizString[nInsertIndex] = colQuizColor;
				m_nCurQuizInsert++;

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddQuizDescString( strTemp, colQuizColor );
			}	

		}
	}
}

// --------------------------------------------------------------------------
// Name : ResetQuizData()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::ResetQuizData( void )
{
	m_nQuizNum	= 0;
	m_nCurQuizInsert	= 0;
	m_nCurQuizCount		= 0;
	m_dOldTime	= 0.0f;
	m_dCurTime	= 0.0f;	
	m_bQuizStart = FALSE;

	for( int i=0; i<MAX_QUIZ_DESC; i++ )
	{
		m_strQuizString[i].Clear();
	}

	
	if( g_iEnterChat && _pUIMgr->GetChatting()->GetInputBox().IsFocused()){
		_pUIMgr->RearrangeOrder(UI_QUIZEVENT,FALSE);
		_pUIMgr->GetChatting()->GetInputBox().SetFocus( TRUE );
	}
	else {
		_pUIMgr->RearrangeOrder(UI_QUIZEVENT,FALSE);
	}

	PIX pixMinI = _pdpMain->dp_MinI;
	PIX pixMinJ = _pdpMain->dp_MinJ;
	PIX pixMaxI = _pdpMain->dp_MaxI;
	PIX pixMaxJ = _pdpMain->dp_MaxJ;

	_pUIMgr->GetOXQuizEvent()->ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}


// --------------------------------------------------------------------------
// Name : RenderQuizStringList()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::RenderQuizStringList( void )
{
	int nX = m_nPosX + 20;
	int nY = m_nPosY + 7;

	// title str
	_pUIMgr->GetDrawPort()->PutTextEx( _S(2614, "O.X 퀴즈" ), nX, nY, 0xCCCCCCFF );
	nY += ( 26 + 3 );

	for( int i = 0; i<m_nCurQuizInsert; i++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strQuizString[i], nX, nY, m_colQuizString[i] );
		nY += m_nChatLineHeight;
	}

	nY += m_nChatLineHeight;

	double dNewTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	double dQuizTemp = m_dCurTime -( dNewTime - m_dOldTime);

	int nTime = (int)( dQuizTemp / 1000 );
	CTString strTime;
	
	strTime.PrintF( _S(2615, "남은 시간: %d초" ), nTime );

	if( nTime > 5 ) // 여유로운 시간
		_pUIMgr->GetDrawPort()->PutTextExRX( strTime, nX+120, m_nPosY+167, 0x00EDBDFF );
	else if( nTime >= 0 ) // 5초 이후 붉은 색으로
		_pUIMgr->GetDrawPort()->PutTextExRX( strTime, nX+120, m_nPosY+167, 0xFF1924FF );

	if( dQuizTemp <= 0 )
	{
		m_bQuizStart = FALSE;
		ResetQuizData();
	}	
}

// --------------------------------------------------------------------------
// Name : Render()
// Desc : 
// --------------------------------------------------------------------------
void CUIOXQuizEvent::Render( void )
{
	//Set texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int nY, nX2;
	nX2 = m_nPosX + m_nWidth;

	// Background
	// Top
	nY = m_nPosY + QUIZBOX_TOP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	// Desc middle
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + QUIZBOX_DESC_HEIGHT,
										m_rtMiddleScrollL.U0, m_rtMiddleScrollL.V0,
										m_rtMiddleScrollL.U1, m_rtMiddleScrollL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + QUIZBOX_DESC_HEIGHT,
										m_rtMiddleScrollM.U0, m_rtMiddleScrollM.V0,
										m_rtMiddleScrollM.U1, m_rtMiddleScrollM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + QUIZBOX_DESC_HEIGHT,
										m_rtMiddleScrollR.U0, m_rtMiddleScrollR.V0,
										m_rtMiddleScrollR.U1, m_rtMiddleScrollR.V1,
										0xFFFFFFFF );

	// Gap middle
	nY += MSGBOXL_DESC_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + QUIZBOX_GAP_HEIGHT,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + QUIZBOX_GAP_HEIGHT,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + QUIZBOX_GAP_HEIGHT,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );

	// Bottom
	nY += QUIZBOX_GAP_HEIGHT;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + QUIZBOX_BOTTOM_HEIGHT,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + QUIZBOX_BOTTOM_HEIGHT,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( nX2 - 40, nY, nX2, nY + QUIZBOX_BOTTOM_HEIGHT,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );

	RenderQuizStringList();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// --------------------------------------------------------------------------
// Name : Render()
// Desc : 
// --------------------------------------------------------------------------
WMSG_RESULT CUIOXQuizEvent::MouseMessage( MSG *pMsg )
{
//	WMSG_RESULT wmsgResult;

	// Mouse point
	static int nOldX, nOldY;
	int nX = LOWORD( pMsg->lParam );
	int nY = HIWORD( pMsg->lParam );

	//Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
/*		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			int ndX = nX - nOldX;
			int ndY = nY - nOldY;

			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX; nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
		}*/
		break;
	case WM_LBUTTONDOWN:
		/*{	
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bTitleBarClick = TRUE;
				}

				return WMSG_SUCCESS;
			}
		}	*/
		break;
	case WM_LBUTTONUP:
/*		{
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				m_bTitleBarClick = FALSE;

				if( !IsFocused() ) return WMSG_FAIL;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}*/
		break;
	}


	return WMSG_FAIL;
}

// ---------------------------------------------------------------------
// Name : ReceiveQuiz()
// Desc : 
// ---------------------------------------------------------------------
void CUIOXQuizEvent::ReceiveQuiz( int nQuizNo, int limitsec, CTString QuizStr )
{
	ResetQuizData();

	m_nQuizNum = nQuizNo;
	m_dCurTime = limitsec*1000;
	m_dOldTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	CTString QuizNoStr;
	QuizNoStr.PrintF( CTString( "Q%d. " ), m_nQuizNum );

	QuizStr = ( QuizNoStr + QuizStr );

	AddQuizDescString( QuizStr, 0xFFFFFFFF );

	_pUIMgr->GetOXQuizEvent()->SetVisible( TRUE );
	_pUIMgr->GetOXQuizEvent()->SetEnable( TRUE );
}