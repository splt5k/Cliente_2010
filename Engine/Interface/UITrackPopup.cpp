#include "stdh.h"
#include <Engine/Interface/UITrackPopup.h>
#include <Engine/Interface/UITextureManager.h>

// 리스트 박스 크기에 맞게 박스를 그릴때 위,아래/ 좌,우 여백 
#define SPACE_UPDONW	14//27		
#define SPACE_LEFTRIGT	10//19


//------------------------------------------------------------------------------
// CUITrackPopup::CUITrackPopup
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITrackPopup::CUITrackPopup()
{
	m_nMaxCharCount = 0;	// 최대 글자 수 
	
}


//------------------------------------------------------------------------------
// CUITrackPopup::~CUITrackPopup
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITrackPopup::~CUITrackPopup()
{

}


void CUITrackPopup::Render()
{
	if( !IsVisible() ) return;
	if( m_vecString.size() <= 0 ) return;

	// 배경 렌더
	int	nX, nY;
	UIRect m_rcBack;

	GetAbsPos( nX, nY );
	m_rcBack.SetRect( nX, nY, nX + m_nWidth, nY + m_nHeight );

	// Item Back region
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left, m_rcBack.Top,
										m_rcBack.Left + 7, m_rcBack.Top + 7,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left + 7, m_rcBack.Top,
										m_rcBack.Right - 7, m_rcBack.Top + 7,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Right - 7, m_rcBack.Top,
										m_rcBack.Right, m_rcBack.Top + 7,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left, m_rcBack.Top + 7,
										m_rcBack.Left + 7, m_rcBack.Bottom - 7,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left + 7, m_rcBack.Top + 7,
										m_rcBack.Right - 7, m_rcBack.Bottom - 7,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Right - 7, m_rcBack.Top + 7,
										m_rcBack.Right, m_rcBack.Bottom - 7,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left, m_rcBack.Bottom - 7,
										m_rcBack.Left + 7, m_rcBack.Bottom,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Left + 7, m_rcBack.Bottom - 7,
										m_rcBack.Right - 7, m_rcBack.Bottom,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBack.Right - 7, m_rcBack.Bottom - 7,
										m_rcBack.Right, m_rcBack.Bottom,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// 원래 리스트 박스에 대한 내용 랜더
	CUIListBox::Render();
	
}

//------------------------------------------------------------------------------
// UITrackPopup::AddMenuList
// Explain:  메류 항목 추가 
//			새로운 항목이 추가 될 때만 ListBox의 크기가 조절된다.
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITrackPopup::AddMenuList( CTString strText, COLOR colText )
{
	AddString( 0, strText, colText );
		
	if( m_nMaxCharCount < strText.Length() ) // 최대 글자수를 저장해 놓는다~
	{
		m_nMaxCharCount = strText.Length();
	}
	
	Resize();

	// 일단은 숨겨 놓구 
	Hide();
}


//------------------------------------------------------------------------------
// CUITrackPopup::ResetAllStrings
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITrackPopup::ResetAllStrings()
{
	m_nMaxCharCount = 0;	// 최대 글자 수 
	SetCurSel( -1 );

	CUIListBox::ResetAllStrings();
}


//------------------------------------------------------------------------------
// CUITrackPopup::MouseMessage
// Explain:  변경 사항 
//			1. 셀렉션 처리 
//			2. LButton Up 처리
// Date : 2005-05-16,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUITrackPopup::MouseMessage( MSG *pMsg )
{
	// If list box is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelectList )
			{
				m_nOverList = -1;

				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					// Over 시 Selection 되게 수정
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount() )
					{
						m_nSelectList = nRealSel;
					}

					SetFocus(true);

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bSelectList )
			{
				// UP시에 선택 
				if( IsInside( nX, nY ) )
				{
					return WMSG_SUCCESS;
				}
			}
			
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bSelectList )
			{
				// UP시에 선택 
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 )//&& nRealSel < m_sbScrollBar.GetCurItemCount() )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}

					return WMSG_SUCCESS;
				}
			}
			
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUITrackPopup::Resize()
{
	// 현재 입력된 스트링에 따른 ListBox의 크기 설정 
	// Width = 가장 긴 스트링 길이 기준
	m_nWidth = SPACE_UPDONW - _pUIFontTexMgr->GetFontSpacing() + m_nMaxCharCount *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Height = 메뉴 수 
	//m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * _pUIFontTexMgr->GetLineHeight();
	m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * m_nLineHeight;

	// Selection Bar에 대한 크기 조절 
	int nWidth = m_nWidth - 5;
	int nHeight = _pUIFontTexMgr->GetLineHeight()+2;

	// Selection Width 
	if( nWidth > m_nWidth || nWidth == -1 )
		nWidth = m_nWidth;
	int	nHalfWidth = nWidth / 2;
	int	nLeft = m_nWidth / 2 - nHalfWidth;

	if( nHeight < m_nLineHeight || nHeight == -1 )
		nHeight = m_nLineHeight;

	nLeft+=1;  // 좌우 약간의 여백 조정
	nWidth-=4;

	m_rcSelectOver.SetRect( nLeft, -m_nSelectOffsetY, nLeft + nWidth, nHeight - m_nSelectOffsetY );
}	

void CUITrackPopup::DeleteMenuList( int nCol, int nIndex )
{
	CTString strMenu =GetString(nCol, nIndex);

	RemoveString(nIndex, nCol);	

	if( strMenu.Length() == m_nMaxCharCount )
	{
		m_nMaxCharCount =0;
		for(int i=0; i<m_vecString[nCol].vecString.size(); ++i)
		{
			if( m_vecString[nCol].vecString[i].Length() > m_nMaxCharCount )
				m_nMaxCharCount =m_vecString[nCol].vecString[i].Length();
		}
	}

	Resize();
}