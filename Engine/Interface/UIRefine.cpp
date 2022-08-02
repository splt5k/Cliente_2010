#include "stdh.h"
#include <Engine/Interface/UIRefine.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>

enum eSelection
{
	REFINE_OK,
	REFINE_TALK,
	REFINE_EVENT,
};

static int	_iMaxMsgStringChar = 0;

extern INDEX g_iCountry;

#ifdef HELP_SYSTEM_1
// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;
#endif

// ----------------------------------------------------------------------------
// Name : CUIRefine()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIRefine::CUIRefine()
{
	m_nStringCount = 0;
	m_bWaitRefineResult = TRUE;
	m_strRefineMoney = CTString( "" );
	m_llRefineMoney = 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUIRefine()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIRefine::~CUIRefine()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	_iMaxMsgStringChar = 190 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcItemSlot.SetRect( 91, 0, 125, 0 );
	m_rcInsertItem.SetRect( 3, 0, 213, 0 );

	// Create refine texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 0, 68, 34, 102, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), 36, 154, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 117, 154, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Add string
	AddString( _S( 225, "제련석으로 변환시킬 아이템을 인벤토리에서 선택하여 넣어주십시오." ) );

	// Set region of slot item & money...
	int	nNewHeight = REFINE_DESC_TEXT_SY + ( m_nStringCount + 1 ) * _pUIFontTexMgr->GetLineHeight();
	m_rcItemSlot.Top = nNewHeight;
	m_rcItemSlot.Bottom = m_rcItemSlot.Top + 34;

	nNewHeight += 34 + _pUIFontTexMgr->GetLineHeight();
	m_nMoneyPosY = nNewHeight;
	nNewHeight += _pUIFontTexMgr->GetLineHeight() + 8 - _pUIFontTexMgr->GetLineSpacing();
	m_nTextRegionHeight = nNewHeight - REFINE_DESC_TEXT_SY + 8;

	m_rcInsertItem.Top = REFINE_DESC_TEXT_SY;
	m_rcInsertItem.Bottom = nNewHeight;

	nNewHeight += 6;
	m_btnOK.SetPosY( nNewHeight );
	m_btnCancel.SetPosY( nNewHeight );

	nNewHeight += m_btnOK.GetHeight() + 7;
	SetHeight( nNewHeight );

	// Slot item button
	m_btnSlotItem.Create( this, m_rcItemSlot.Left + 1, m_rcItemSlot.Top + 1, BTN_SIZE, BTN_SIZE, UI_REFINE, UBET_ITEM );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenRefine()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::OpenRefine(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	// If this is already exist
	if( _pUIMgr->DoesMessageBoxLExist( MSGLCMD_REFINE_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( _pUIMgr->GetInventory()->IsLocked() )
	{
		_pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData& MD = _pNetwork->GetMobData(iMobIndex);

	// Create refine message box
	_pUIMgr->CreateMessageBoxL( _S( 226, "제련" ), UI_REFINE, MSGLCMD_REFINE_REQ );

	CTString	strNpcName = _pNetwork->GetMobName(iMobIndex);
	_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, TRUE, _S( 227, "어서 오시오, 젊은 전사여.\n나는 긍지있는 연금술사 제랄 드 모랄이오.\n\n음? 아니 아니, 아무 말 하지 않아도 좋소. 여행자들이 나를 찾아온 용무는 언제나 같으니.\n" ), -1, 0xA3A1A3FF );
	_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, TRUE, _S( 228, "내게 있어선 이 또한 하나의 수행이자 경험.\n제련석으로 만들고자 하는 물건을 넘겨주게나." ), -1, 0xA3A1A3FF );

	_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, FALSE, _S( 229, "제련석을 만든다." ), REFINE_OK  );	
	_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, FALSE, _S( 1220, "취소한다." ) );	

	if( bHasQuest )
	{
#ifdef	NEW_QUESTBOOK
		// 2009. 05. 27 김정래
		// 이야기한다 변경 처리
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_REFINE_REQ);				
#else
		_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, FALSE, _S( 1053, "이야기한다." ), REFINE_TALK  );	
#endif
	}

	// FIXME : 퀘스트가 없을 경우에 문제가 됨.
	// FIXME : 고로, 이벤트 NPC는 되도록 퀘스트를 갖고 있는 형태로???
	if( MD.IsEvent() )
	{
		_pUIMgr->AddMessageBoxLString( MSGLCMD_REFINE_REQ, FALSE, _S( 100, "이벤트" ), REFINE_EVENT );		
	}

	m_bWaitRefineResult = FALSE;
}

// ----------------------------------------------------------------------------
// Name : CloseRefine()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::CloseRefine()
{
	// Reset slot item
	m_btnSlotItem.InitBtn();

	// Close refine
	_pUIMgr->RearrangeOrder( UI_REFINE, FALSE );

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_REFINE );

	m_strRefineMoney.Clear();
	m_bWaitRefineResult = FALSE;
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::AddString( CTString &strDesc )
{
	if( m_nStringCount >= MAX_REFINE_STRING )
		return;

	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strDesc);
		INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			// Check line character
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strRefineDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strDesc,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}

		}
		
	} else {
		// If length of string is less than max char
		if( nLength <= _iMaxMsgStringChar )
		{
			// Check line character
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nLength )
				m_strRefineDesc[m_nStringCount++] = strDesc;
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = _iMaxMsgStringChar;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Check line character
			for( iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
					break;
			}

			// Not exist
			if( iPos == nSplitPos )
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( nSplitPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim space
				if( strTemp[0] == ' ' )
				{
					int	nTempLength = strTemp.Length();
					for( iPos = 1; iPos < nTempLength; iPos++ )
					{
						if( strTemp[iPos] != ' ' )
							break;
					}

					strTemp.TrimLeft( strTemp.Length() - iPos );
				}

				AddString( strTemp );
			}
			else
			{
				// Split string
				CTString	strTemp;
				strDesc.Split( iPos, m_strRefineDesc[m_nStringCount++], strTemp );

				// Trim line character
				if( strTemp[0] == '\r' && strTemp[1] == '\n' )
					strTemp.TrimLeft( strTemp.Length() - 2 );
				else
					strTemp.TrimLeft( strTemp.Length() - 1 );

				AddString( strTemp );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		CloseRefine();

	// Set refine texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	// Middle 1
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, nY + m_nTextRegionHeight,
										m_rtMiddle1.U0, m_rtMiddle1.V0, m_rtMiddle1.U1, m_rtMiddle1.V1,
										0xFFFFFFFF );

	// Middle 2
	nY += m_nTextRegionHeight;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtMiddle2.U0, m_rtMiddle2.V0, m_rtMiddle2.U1, m_rtMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

	// Slot item region
	_pUIMgr->GetDrawPort()->AddTexture( m_nPosX + m_rcItemSlot.Left, m_nPosY + m_rcItemSlot.Top,
										m_nPosX + m_rcItemSlot.Right, m_nPosY + m_rcItemSlot.Bottom,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1,
										0xFFFFFFFF );

	// Close button
	m_btnClose.Render();

	// OK button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	// Item
	if( !m_btnSlotItem.IsEmpty() )
	{
		m_btnSlotItem.Render();
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Text in refine
	_pUIMgr->GetDrawPort()->PutTextEx( _S( 231, "제련" ), m_nPosX + REFINE_TITLE_TEXT_OFFSETX,
										m_nPosY + REFINE_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nX = m_nPosX + REFINE_DESC_TEXT_SX;
	nY = m_nPosY + REFINE_DESC_TEXT_SY;
	for( int iDesc = 0; iDesc < m_nStringCount; iDesc++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strRefineDesc[iDesc], nX , nY, 0xC5C5C5FF );
		nY += _pUIFontTexMgr->GetLineHeight();
	}

	// Refineing money
	if( m_llRefineMoney > 0 )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strRefineMoney,
											m_nPosX + REFINE_DESC_TEXT_SX, m_nPosY + m_nMoneyPosY, 0xE1B300FF );
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRefine::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				_pUIMgr->SetMouseCursorInsideUIs();

			// Move refine
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				_pUIMgr->RearrangeOrder( UI_REFINE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If refine isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseRefine();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendRefineReq();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseRefine();

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and comes from inventory
					if( _pUIMgr->GetHoldBtn().GetBtnType() == UBET_ITEM &&
						_pUIMgr->GetHoldBtn().GetWhichUI() == UI_INVENTORY )
					{
						if( IsInsideRect( nX, nY, m_rcInsertItem ) )
						{
							// Set refine item
							SetRefineItem();
						}
					}

					// Reset holding button
					_pUIMgr->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SetRefineItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::SetRefineItem()
{
	// If this is wearing item
	if( _pUIMgr->GetHoldBtn().GetItemWearType() >= 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 232, "착용된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
		return;
	}

	// If this is not weapon or armor
	int	nTab = _pUIMgr->GetHoldBtn().GetItemTab();
	int	nRow = _pUIMgr->GetHoldBtn().GetItemRow();
	int	nCol = _pUIMgr->GetHoldBtn().GetItemCol();
	CItems		&rItems = _pNetwork->MySlotItem[nTab][nRow][nCol];
	CItemData	&rItemData = rItems.ItemData;
	if( rItemData.GetType() != CItemData::ITEM_WEAPON &&
		rItemData.GetType() != CItemData::ITEM_SHIELD )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 233, "무기와 방어구만 제련 할 수 있습니다." ), SYSMSG_ERROR );
		return;
	}

	// If this item is upgraded
	if( rItems.Item_Plus > 0 )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 234, "업그레이드 된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
		return;
	}

	// If refining money is short
	SLONG	slWearLevel = rItemData.GetLevel();
	m_llRefineMoney = ( ( slWearLevel + 1 ) * ( slWearLevel + 3 ) * ( slWearLevel -1 ) + 100 ) / 4;
	m_strRefineMoney.PrintF( _S( 419, "제련 비용 : %I64d" ), m_llRefineMoney );

	// Insert upgrade slot
	m_btnSlotItem.Copy( _pUIMgr->GetHoldBtn() );

	// Lock inventory
	_pUIMgr->GetInventory()->Lock( TRUE, FALSE, LOCK_REFINE );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_REFINE_REQ:
		if( nResult == REFINE_OK )
		{
			if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 946, "스킬 사용중에는 제련을 할 수 없습니다." ), SYSMSG_ERROR );		
				CloseRefine();
				return;
			}

			if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
			{
				_pUIMgr->GetChatting()->AddSysMessage( _S( 947, "순간 이동중에는 제련을 할 수 없습니다." ), SYSMSG_ERROR );	
				CloseRefine();
				return;
			}

			// If inventory is already locked
			if( _pUIMgr->GetInventory()->IsLocked() )
			{
				_pUIMgr->GetInventory()->ShowLockErrorMessage();
				CloseRefine();
				return;
			}

			if( !_pUIMgr->GetInventory()->IsVisible() )
				_pUIMgr->GetInventory()->ToggleVisible();

			_pUIMgr->RearrangeOrder( UI_REFINE, TRUE );
			
#ifdef HELP_SYSTEM_1
// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				_pUIMgr->GetHelp3()->ClearHelpString();
				_pUIMgr->GetHelp3()->AddHelpString(_S(3298, "제련석으로 만들고자 하는 장비를 인벤토리로부터 드래그하여 옆 창의 빈 공간에 올려 놓은 뒤 확인 버튼을 클릭하면 해당 장비가 동일한 레벨의 일반 제련석으로 변환됩니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3299, "※ 무기는 2개의 제련석으로 변환되며 방어구는 1개의 제련석으로 변환됩니다."));
				_pUIMgr->GetHelp3()->AddHelpString(_S(3300, "※ 착용중인 장비 또는 장비가 아닌 아이템은 제련석으로 변환 할 수 없습니다."));
				_pUIMgr->GetHelp3()->OpenHelp(this);
				
			}
#endif
		}
		else if( nResult == REFINE_TALK )
		{
			//TODO : NewQuestSystem
			// 퀘스트 창 띄우기
			CUIQuestBook::TalkWithNPC();
			// Unlock inventory
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_REFINE );
		}
		else if( nResult == REFINE_EVENT )		// Event
		{
		}

		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
			// Unlock inventory
			_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_REFINE );
		}
		else
		{
		}
		break;
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendRefineReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::SendRefineReq()
{
	if( m_bWaitRefineResult )
		return;

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 946, "스킬 사용중에는 제련을 할 수 없습니다." ), SYSMSG_ERROR );		
		return;
	}

	if( _pUIMgr->IsCSFlagOn( CSF_TELEPORT ) )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 947, "순간 이동중에는 제련을 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if( m_btnSlotItem.IsEmpty() )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 235, "제련할 아이템이 없습니다." ), SYSMSG_ERROR );
		return;
	}

	if( m_llRefineMoney > _pNetwork->MyCharacterInfo.money )
	{
		_pUIMgr->GetChatting()->AddSysMessage( _S( 369, "제련 비용이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	SBYTE	sbRow = m_btnSlotItem.GetItemRow();
	SBYTE	sbCol = m_btnSlotItem.GetItemCol();

	_pNetwork->RefineReq( sbRow, sbCol );

	m_bWaitRefineResult = TRUE;
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : RefineRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIRefine::RefineRep( SBYTE sbResult )
{
	// Close message box
	_pUIMgr->CloseMessageBox( MSGCMD_REFINE_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case 0:
		strMessage = _S( 236, "아이템이 순조롭게 연성되어 제련석이 2개 완성되었습니다." );
		break;

	case 1:
		strMessage = _S( 237, "아이템이 연성되어 제련석이 1개 완성되었습니다." );
		break;

	case 2:
		strMessage = _S( 238, "아이템이 성공적으로 연성되어 고급 제련석이 1개 완성되었습니다." );
		break;
	}

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 231, "제련" ), UMBS_OK, UI_REFINE, MSGCMD_REFINE_REP );
	MsgBoxInfo.AddString( strMessage );
	_pUIMgr->CreateMessageBox( MsgBoxInfo );

	// Reset slot item
	m_btnSlotItem.InitBtn();

	// Unlock inventory
	_pUIMgr->GetInventory()->Lock( FALSE, FALSE, LOCK_REFINE );

	m_strRefineMoney.Clear();
	m_bWaitRefineResult = FALSE;
}
