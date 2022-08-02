#include "stdh.h"
#include <Engine/Interface/UINotice.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <algorithm>
#include <Engine/LocalDefine.h>

const int iStopChangeItem		= 521;		// 변신 취소.
const int iStopTransformItem	= 522;		// 강신 취소.

#define CHARGE_BTN_X			0
#define CHARGE_BTN_Y			104
#define CHARGE_BTN_INDEX		1000
#define EVENT_START_INDEX		2000		// 이벤트 인덱스의 시작 위치.
#define PET_QUEST_HORSE_INDEX	4000		// 펫 관련 퀘스트...
#define PET_QUEST_DRAGON_INDEX	4001
#define EVENT_NETCAFE_INDEX		4002		// (일본, 태국) netcafe캠페인 이벤트
// wooss 060809 add rare item
#define PET_QUEST_BLUE_HORSE_INDEX		4003
#define PET_QUEST_PINK_DRAGON_INDEX		4004
#define PET_QUEST_MYSTERY_HORSE_INDEX	4005
#define PET_QUEST_MYSTERY_DRAGON_INDEX	4006

enum eEvent_Index
{
	PANDORA_EVENT_INDEX =26,
	XMAS_EVENT_INDEX =27,
	CHILDREN_DAY_EVENT_INDEX =28,
	SONGKRAN_EVENT_INDEX =32,
};

//#define PANDORA_EVENT_INDEX		26
//#define	CHILDREN_DAY_EVENT_INDEX	28

extern INDEX	g_iCountry;
extern UINT g_uiEngineVersion;

int nMaxNotice = 4;
int nChargeBtnPos = 0;

// ----------------------------------------------------------------------------
// Name : CUINotice()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUINotice::CUINotice()
{	
	m_nSelNoticeID	= -1;
	m_bShowSlotInfo = FALSE;
	m_nLeftTime		= 0;
	m_tmLeftTime	= 0;
}

// ----------------------------------------------------------------------------
// Name : ~CUINotice()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUINotice::~CUINotice()
{
	if( !m_vectorNoticeList.empty() )
		m_vectorNoticeList.clear();

	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	if( g_iCountry == TAIWAN || g_iCountry == TAIWAN2 || g_iCountry == HONGKONG ) //wooss 050929
	{
		nMaxNotice = 3;
		nChargeBtnPos = BTN_SIZE+10; // wooss 050926 
		nHeight = BTN_SIZE * 3;
	}
	
	m_pParentWnd = pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	// Create quick slot texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Buff.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Buff information region
	m_rtInfoUL.SetUV( 85, 0, 92, 7, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 95, 0, 97, 7, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 100, 0, 107, 7, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 85, 10, 92, 12, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 95, 10, 97, 12, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 100, 10, 107, 12, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 85, 15, 92, 22, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 95, 15, 97, 22, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 100, 15, 107, 22, fTexWidth, fTexHeight );

	m_rcIcons.SetRect( 0, 0, 34, nMaxNotice * 35 );

	int	nTempX = 0, nTempY = 0;	
	for(int i = 0; i < nMaxNotice; ++i, nTempY += 35)
	{
		m_abtnItems[i].Create( this, nTempX, nTempY, BTN_SIZE, BTN_SIZE, UI_NOTICE, UBET_QUEST );
	}	

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth, pixMaxJ - m_nHeight - QUICKSLOT_HEIGHT - nChargeBtnPos ); // 지불 버튼 추가 
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ShowSlotInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::ShowSlotInfo( BOOL bShowInfo, int nSlotIndex )
{	
	// Hide slot information
	if( !bShowInfo || nSlotIndex < 0 )
	{
		m_bShowSlotInfo = FALSE;
		return;
	}
	
	BOOL	bUpdateInfo = FALSE;
	int		nInfoPosX, nInfoPosY;

		CUIButtonEx	& rbtnSelect = m_abtnItems[nSlotIndex];
		
		if( rbtnSelect.IsEmpty() )
		{
			m_bShowSlotInfo = FALSE;
		}
		else
		{
			m_bShowSlotInfo = TRUE;
			bUpdateInfo = TRUE;
			
			rbtnSelect.GetAbsPos( nInfoPosX, nInfoPosY );

			const UIBtnExType btnType = rbtnSelect.GetBtnType();
			switch( btnType )
			{
			case UBET_QUEST:
				{
					// NOTE : 애완동물 획득 관련되서 하드코딩된 부분.
					if( rbtnSelect.GetQuestIndex() == PET_QUEST_HORSE_INDEX
						||rbtnSelect.GetQuestIndex() == PET_QUEST_BLUE_HORSE_INDEX
						||rbtnSelect.GetQuestIndex() == PET_QUEST_MYSTERY_HORSE_INDEX)
					{
						m_strSlotInfo = _S(2160,"애완동물 획득 관련 퀘스트 (무한반복 퀘스트)");
					}
					else if( rbtnSelect.GetQuestIndex() == PET_QUEST_DRAGON_INDEX
						|| rbtnSelect.GetQuestIndex() == PET_QUEST_PINK_DRAGON_INDEX
						|| rbtnSelect.GetQuestIndex() == PET_QUEST_MYSTERY_DRAGON_INDEX)
					{
						m_strSlotInfo = _S(2161,"애완동물 획득 관련 퀘스트 (무한반복 퀘스트)");
					}
					else if( rbtnSelect.GetQuestIndex() >= EVENT_START_INDEX )
					{
						m_strSlotInfo = _S( 100, "이벤트" );
					}
					else
					{
						CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(rbtnSelect.GetQuestIndex()));
						m_strSlotInfo.PrintF( _S( 559, "%s 퀘스트(1회만 수행가능)" ), QuestDD.GetName());		
					}
				}
				break;
			case UBET_ITEM:
				{
					const int iStopChangeItem		= 521;
					const int iStopTransformItem	= 522;
					const int iIndex				= rbtnSelect.GetIndex();
					
					// 강신 중지 버튼.
					if( iIndex == iStopTransformItem )
					{
						UpdateTimeInfo();						
					}
					// 변신 중지 버튼.
					else if( iIndex == iStopChangeItem )
					{
						m_bShowSlotInfo = FALSE;
						const char* szDesc = _pNetwork->GetItemDesc( iStopChangeItem );
						m_strSlotInfo.PrintF( szDesc );
					}
				}
				break;		
			case UBET_AUCTION:
				m_strSlotInfo = _S(4355, "정산 알림");
				break;
			}
		}
//	}
	// Update slot information box
	if( bUpdateInfo )
	{
		int nInfoWidth;
		if(g_iCountry == THAILAND) {
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strSlotInfo);				
		} else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strSlotInfo.Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nInfoHeight = 22;
		
		nInfoPosX += ( BTN_SIZE - nInfoWidth ) / 2;
		
		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX += _pUIMgr->GetMaxI() - ( nInfoPosX + nInfoWidth );
		
		if( nInfoPosY - nInfoHeight < _pUIMgr->GetMinJ() )
		{
			nInfoPosY += BTN_SIZE + 2;
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
		else
		{
			m_rcSlotInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
	}
	
	if( !m_bShowSlotInfo )
	{
		m_bShowSlotInfo = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::RenderBtns()
{
	// Timer for highlight buttons
	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;
	if( dElapsedTime > 0.5 )
	{
		bHighlight = !bHighlight;
		do
		{
			dElapsedTime -= 0.5;
		}
		while( dElapsedTime > 0.5 );
	}	

	// 퀘스트 버튼 렌더링.
	// Quest Buttons
	int	nX = 0, nY = 0;
	int i;
	for( i = 0; i < nMaxNotice; ++i, nY += 35)
	{
		//m_abtnItems[i].SetPos( nX, nY );
		if( m_abtnItems[i].IsEmpty() || m_abtnItems[i].GetBtnType() != UBET_QUEST )
			continue;

		m_abtnItems[i].Render();
		if( bHighlight )
			m_abtnItems[i].RenderHighlight( 0xFFFFFFFF );
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_QUEST );

	// 아이템 버튼 렌더링.		
	for( i = 0; i < nMaxNotice; ++i, nY += 35)
	{
		//m_abtnItems[i].SetPos( nX, nY );
		if( m_abtnItems[i].IsEmpty() || m_abtnItems[i].GetBtnType() != UBET_ITEM )
			continue;

		m_abtnItems[i].Render();

		if( bHighlight )
			m_abtnItems[i].RenderHighlight( 0xFFFFFFFF );
	}

	// Render all button elements
	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );


	for( i = 0; i < nMaxNotice; ++i, nY += 35)
	{
		//m_abtnItems[i].SetPos( nX, nY );
		if( m_abtnItems[i].IsEmpty() || m_abtnItems[i].GetBtnType() != UBET_AUCTION )
			continue;

		m_abtnItems[i].Render();

		if( bHighlight )
			m_abtnItems[i].RenderHighlight( 0xFFFFFFFF );
	}

	_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_AUCTION );

	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

	// Slot information
	if( m_bShowSlotInfo )
	{
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left, m_rcSlotInfo.Top,
			m_rcSlotInfo.Left + 7, m_rcSlotInfo.Top + 7,
			m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left + 7, m_rcSlotInfo.Top,
			m_rcSlotInfo.Right - 7, m_rcSlotInfo.Top + 7,
			m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Right - 7, m_rcSlotInfo.Top,
			m_rcSlotInfo.Right, m_rcSlotInfo.Top + 7,
			m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left, m_rcSlotInfo.Top + 7,
			m_rcSlotInfo.Left + 7, m_rcSlotInfo.Bottom - 7,
			m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left + 7, m_rcSlotInfo.Top + 7,
			m_rcSlotInfo.Right - 7, m_rcSlotInfo.Bottom - 7,
			m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Right - 7, m_rcSlotInfo.Top + 7,
			m_rcSlotInfo.Right, m_rcSlotInfo.Bottom - 7,
			m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left, m_rcSlotInfo.Bottom - 7,
			m_rcSlotInfo.Left + 7, m_rcSlotInfo.Bottom,
			m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Left + 7, m_rcSlotInfo.Bottom - 7,
			m_rcSlotInfo.Right - 7, m_rcSlotInfo.Bottom,
			m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
			0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( m_rcSlotInfo.Right - 7, m_rcSlotInfo.Bottom - 7,
			m_rcSlotInfo.Right, m_rcSlotInfo.Bottom,
			m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
			0xFFFFFFFF );
		
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render item information
		_pUIMgr->GetDrawPort()->PutTextEx( m_strSlotInfo, m_rcSlotInfo.Left + 8, m_rcSlotInfo.Top + 4 );

		// Flush all render text queue
		_pUIMgr->GetDrawPort()->EndTextEx();
	}
	else
	{
		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::Render()
{
	// Buttons
	RenderBtns();
}

// ----------------------------------------------------------------------------
// Name : PressOK()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::PressOK( int iSlotIndex )
{	
	ASSERT( iSlotIndex <= nMaxNotice && "Invalid Slot Index" );
	if( iSlotIndex < 0 )
	{		
		return;
	}
	
	CUIButtonEx	& rbtnSelect = m_abtnItems[iSlotIndex];
	
	if( rbtnSelect.IsEmpty() )
	{
		return;
	}
	
	UIBtnExType btnType = rbtnSelect.GetBtnType();	

	// Temp String
	CTString strMsg;

	switch( btnType )
	{
	case UBET_QUEST:		// 퀘스트 버튼
		{
			const int iNoticeIndex	= rbtnSelect.GetQuestIndex();

			// NOTE : 애완동물 획득 퀘스트 관련 하드코딩된 부분.
			if( iNoticeIndex == PET_QUEST_HORSE_INDEX
				|| iNoticeIndex == PET_QUEST_BLUE_HORSE_INDEX 
				|| iNoticeIndex == PET_QUEST_MYSTERY_HORSE_INDEX	)
			{
				CTString strMessage;				
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
				strMessage = _S(2162, "애완동물 획득 퀘스트" );
				_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
				
				if(iNoticeIndex == PET_QUEST_BLUE_HORSE_INDEX )
					strMessage =_S(3350, "파란 판의 피리를 가지고 로레인을 찾아가면 파란 애완동물 포니를 얻을 수 있는 퀘스트를 진행할 수 있습니다.");
				else if(iNoticeIndex == PET_QUEST_MYSTERY_HORSE_INDEX )
					
					strMessage =_S(3351, "불가사의한 판의 피리를 가지고 로레인을 찾아가면 불가사의한 애완동물 포니를 얻을 수 있는 퀘스트를 진행할 수 있습니다.");
				else strMessage =_S(2163,"판의 피리를 가지고 로레인을 찾아가면 깜찍한 애완동물 포니를 얻을 수 있는 퀘스트를 진행할 수 있습니다.");
				
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
				
				strMessage = _S( 191, "확인" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				// NOTE : 쓰면 바로 지워져야 합니다.
				DelNoticeBySlotIndex( iSlotIndex );
				RefreshNoticeList();
				//break;
			}
			else if( iNoticeIndex == PET_QUEST_DRAGON_INDEX
				|| iNoticeIndex == PET_QUEST_PINK_DRAGON_INDEX
				|| iNoticeIndex == PET_QUEST_MYSTERY_DRAGON_INDEX )
			{
				CTString strMessage;				
				_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
				strMessage = _S(2164, "애완동물 획득 퀘스트" );
				_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );

				// wooss 060810
				// 레어 아이템 추가 
				if( iNoticeIndex == PET_QUEST_PINK_DRAGON_INDEX )
					strMessage = _S(3352, "핑크 드레이크의 알을 가지고 로레인을 찾아가면 핑크색의 애완동물 헤츨링을 얻을 수 있는 퀘스트를 진행할 수 있습니다.");
				if( iNoticeIndex == PET_QUEST_MYSTERY_DRAGON_INDEX )
					strMessage = _S(3353, "불가사의한 드레이크의 알을 가지고 로레인을 찾아가면 불가사의한 애완동물 헤츨링을 얻을 수 있는 퀘스트를 진행할 수 있습니다.");
				else strMessage = _S(2165,"드레이크의 알을 가지고 로레인을 찾아가면 깜찍한 애완동물 헤츨링을 얻을 수 있는 퀘스트를 진행할 수 있습니다.");

				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );

				strMessage = _S( 191, "확인" );
				_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				// NOTE : 쓰면 바로 지워져야 합니다.
				DelNoticeBySlotIndex( iSlotIndex );
				RefreshNoticeList();
			}
			// 이벤트인 경우.
			else if( iNoticeIndex >= EVENT_START_INDEX )
			{
				//const int iEventIndex	= rbtnSelect.GetEventIndex();
				int iEventIndex = iNoticeIndex - EVENT_START_INDEX;

				// 보물 상자.	TEVENT_TREASUREBOX
				if( iEventIndex == 1 )
				{					
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					// Date : 2005-07-05(오후 4:26:58), By Lee Ki-hwan
					// 해외 작업시 고려사항 (이벤트추가)
					// Create event message box
					//_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					_pUIMgr->CreateMessageBoxL( _S( 1940,"보물상자 퀘스트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1940, "보물상자 퀘스트" ), -1, 0xE18600FF );	
					
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1676, "캐릭터를 생성하면 생성된 캐릭터의 인벤토리에 보물상자를 1개 지급됩니다." ));		
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(1941,"캐릭터를 생성하면 생성된 캐릭터 인벤토리의 이벤트 탭에 보물상자가 1개 지급됩니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1677, "생성시에 지급되는 보물상자는 캐릭터의 레벨이 10Lv이 되었을 때 그 보물상자를 쥬노 지역 란돌마을의 로레인을 통해서 열어볼 수가 있습니다." ));	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1678, "보물 상자를 열 경우 해당 보물상자가 지급할 수 있는 품목중에서 랜덤으로 아이템을 지급됩니다." ));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1942, "보물 상자를 열 경우 해당 보물상자가 지급할 수 있는 품목중에서 랜덤으로 아이템이 지급됩니다." ));		
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1679, "10Lv 보물상자를 열면 다음 단계의 보물상자인 14Lv 보물상자를 로레인이 지급하게되고 10Lv, 14Lv, 18Lv, 22Lv, 26Lv, 30Lv 총 6개의 보물상자를 이벤트 기간동안 지급됩니다." ));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(1943,"10Lv 보물상자를 열면 다음 단계의 보물상자인 14Lv 보물상자를 로레인이 지급하게 되고 10Lv, 14Lv, 18Lv, 22Lv, 26Lv, 30Lv 총 6개의 보물상자를 단계적으로 지급합니다." ));	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1680, "이벤트 기간" ), -1, 0xFFCB00FF );	
					//_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1681, "7월 19일까지" ) );	
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// 후견인 보너스 이벤트
				else if( iEventIndex == 2 )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S(1944,"후견인 보너스 이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1944, "후견인 보너스 이벤트" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1945, "이벤트기간 동안 후견인이 후견인 시스템을 통해 신규유저를 양성하는데 성공하면 명성 포인트를 평소의 2배인 20포인트를 지급합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1946, "이벤트기간 동안 견습생이 후견인 시스템을 통해 20레벨에 도달할 경우 각자 자신의 클래스에 맞는 '25Lv + 3' 무기를 지급합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1947, "이벤트기간 동안 견습생이 후견인 시스템을 이용해 성장할 경우 10레벨부터 19레벨까지 레벨 업을 할 때마다 10,000나스씩 격려금이 지급됩니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1948, "기간 : 7월 29일 10:00 ~ 8월 12일 9:00까지" ), -1, 0xE18600FF );			
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 3 )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S(1949,"여름 과일 이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1949, "여름 과일 이벤트" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1950 , "이벤트기간 동안 몬스터를 사냥하면 일정 확률로 몬스터가 날짜에 따라 '수박', '참외', '자두' 아이템을 드롭합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1951, "수박" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1952, "수박 아이템을 사용할 경우 10분간 경험치 습득률이 1.5배 상승합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1953, "기간 : 8월 2일 ~ 5일 오전 정기점검까지" ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1954, "참외" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1955, "참외 아이템을 사용할 경우 10분간 숙련도 습득률이 2배 상승합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1956, "기간 : 8월 5일 ~ 9일 오전 정기점검까지" ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1957, "자두" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1958, "자두 아이템을 사용할 경우 10분간 아이템 드롭률이 2배 상승합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1959, "기간 : 8월 9일 ~ 12일 오전 정기점검까지" ));			
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// 추석맞이 이벤트 // Date : 2005-09-08(오후 2:28:22), By Lee Ki-hwan
				else if( iEventIndex == 4 )
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S( 1960, "추석맞이 이벤트" );
					_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );			
					
					strMessage = _S( 1961, "민족의 명절 추석을 맞이하여 추석맞이 이벤트를 진행합니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					
					strMessage =_S( 1962,  "이벤트 기간 내에 몬스터를 사냥하면 씨앗 아이템을 얻을 수 있으며, 씨앗은 몬스터 사냥을 통하여 얻을 수 있는 '배양토'로 업그레이드가 가능하며, 업그레이드는 로레인을 통하여 하실 수 있습니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage = _S( 1963, "씨앗은 씨앗, 묘목, 분재, 나무, 열매 단계로 업그레이드 할 수 있으며 이벤트 기간 내에나 이벤트 기간 종료 후 업그레이드한 씨앗을 로레인에게 가져가면 보상물품을 받을 수 있습니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage =_S(1964 ,  "보상 물품은 홈페이지 공지사항을 참조하시기 바랍니다." );	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				// Date : 2005-09-08(오후 3:38:14), By Lee Ki-hwan 가을 맞이 이벤트 
				else if( iEventIndex == 5 )
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage =_S( 1965,  "가을맞이 이벤트" );
					_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );			
					
					strMessage = _S( 1966, "가을맞이 이벤트 기간 동안 몬스터를 사냥하시면 일정확률로 특별한 아이템 경험치 증폭제, 숙련도 증폭제, 드롭률 증폭제, 나스 증폭제를 얻을 수 있습니다." ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1967, "4가지 증폭제 중 자신이 원하는 증폭제를 복용하고 몬스터를 사냥할 경우 증폭제 효과에 따라 일정 확률로 몬스터의 보상이 높아집니다." ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1968, "자세한 사항은 홈페이지 공지사항을 참조하시기 바랍니다." ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1969, "기간 : 9월 17일 ~ 30일 까지" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					
					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				else if( iEventIndex == 6 ) // 리뉴얼 이벤트 
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2166, "게임 포탈 2pan4pan 오픈 이벤트" );
					_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "하나!  낱말 맞추기 이벤트";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "몬스터가 드롭하는 낱말 아이템을 가지고 특정 문자를 조합하여 로레인에게서 보상을 받을 수 있는 이벤트 입니다.";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "둘!  무기교체 이벤트";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "이벤트 기간동안 유저들은 무료로 자신이 사용하던 무기를 같은 레벨 대 다른 클래스의 무기로 교체 할 수 있는 이벤트 입니다. 란돌마을 수집가 릴에게서 교체할 수 있습니다.";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "셋!  문스톤 보상 이벤트";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "유저 여러분의 의견을 수렴하여 문스톤 보상 물품을 일부 수정하여 보상해 드립니다.";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "넷!  후견인 명성보상 이벤트";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "견습생의 레벨이 20이 되었을경우 견습생에게 이벤트 검을 지급하고 후견인은 2배의 명성치를 얻습니다";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				else if( iEventIndex == 8 ) // 빼빼로 이벤트.
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2326, "빼빼로 데이 이벤트" );
					_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2327,"모두가 함께하는 빼빼로 데이를 맞이하여 '빼빼로 데이 이벤트'를 진행 합니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2328,"이벤트 기간 내에 몬스터를 사냥하면 일정확률로 이벤트 아이템인 '빼빼로'와 '아몬드 빼빼로'를 획득할 수 있습니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2329,"'빼빼로'와 '아몬드 빼빼로'는 각각 HP와 MP를 동시에 회복 시켜 주며 '아몬드 빼빼로'의 경우 일시적으로 회복시켜주는 물약입니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2330,"기간 : 2005년 11월 11일(금)" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S(2331, "     ~ 2005년 11월 15일(화) 서버 정기점검 전" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 9 )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2286, "2005 크리스마스 이벤트" ), -1, 0xE18600FF );	
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2297, "몬스터를 사냥하면 일정한 확률로 [꿈],[희망] 아이템이 드롭이 됩니다."));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2298, "이 아이템을 가지고 쥬노 마을의 랜디를 찾아가 조합을 시키면 보상을 얻을 수 있는 이벤트 입니다." ));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2299, "자세한 보상 물품은 홈페이지 공지 사항을 참조하시기 바랍니다."));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2300, "이벤트 기간 : 2005.12.23 ~ 2005.12.30" ));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
									
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 10 )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2287, "2006 신년 인내의 열매 이벤트" ), -1, 0xE18600FF );	
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2301, "이벤트 기간동안 유저는 자신의 캐릭의 사냥 시간에 따라 그에 따른 보상을 얻으실 수 있는 이벤트입니다."));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2302, "보상품은 이벤트가 끝난후 쥬노 마을의 랜디를 찾아가면 이벤트 기간동안 사냥시간에 따른 보상품이 주어집니다." ));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2299, "자세한 보상 물품은 홈페이지 공지 사항을 참조하시기 바랍니다."));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2303, "이벤트 기간 : 2005.12.30 ~ 2006.1.3" ));	
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
									
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if(iEventIndex == 11 ) // 무기교체 이벤트
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2332, "무기&방어구 교체 이벤트" );
					_pUIMgr->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2333,"29Lv이상의 무기, 방어구를 타 클래스의 장비로 무료로 교체해 주는 이벤트 입니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2334,"이벤트 기간 동안에는 쥬노마을의 수집가 릴에게 찾아가면 무료로 장비를 교체해 주지만 이벤트가 P나면 일정량의 Nas를 받고 장비를 교체해 줍니다. ");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2335,"주의 : 장비가 교체될 때 제련값은 교체가 되지만 블러드 옵션값은 삭제가 됩니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2336, "기간 : 2005년 12월 20일(화)" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S(2337, "     ~ 2005년 12월 23일(금) 서버 정기점검 전" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				}
				else if(iEventIndex == 12 ) // 친구 찾기 이벤트
				{
					CTString strMessage;				
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );		
					strMessage = _S( 2433, "친구찾기 이벤트 " );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2434, "현재 사용자가 특정 NPC를 통해 최소 1개월 이상 휴면 상태인 캐릭터를 등록 합니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2435, "그런 후에 그 휴면 상태 캐릭터가 게임에 참여하여 일정 시간 플레이를 하게 되면 휴면 상태 캐릭터는 보통 캐릭터보다 경험치를 더 많이 얻게 됩니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2436, "또한, 일정 시간 전투에 참여하면 자신을 등록한 친구와 같이 보상을 받을 수 있는 이벤트 입니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2437, "단, 동일 계정 캐릭터나 동일 주민번호로 등록된 계정의 캐릭터는 친구로 등록할 수 없습니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2438, "기간 : 2006년 2월 14일" ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 2439, "     ~ 2006년 3월 14일 " ); 
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				}
				else if( iEventIndex == 13 ) // 발렌 타인 데이 이벤트
				{
					CTString strMessage;
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2418, "발렌타인 데이 이벤트" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2419, "이벤트 기간동안에 몬스터들은 빨간 초코렛과 하얀 초코렛을 드롭합니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2420, "빨간 초코렛 효과: 5분간 물리,마법 공격력 65증가, 이동속도 향상의 효과를 냅니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2421, "하얀 초코렛 효과: 5분간 물리,마법 방어력 125증가, 이동속도 향상의 효과를 냅니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2422, "이벤트 기간: 2006. 2. 10 ~ 2006. 2. 17" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 1220, "취소한다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 14 )
				{
					CTString strMessage;
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S( 2520, "화이트 데이 이벤트" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";//A3A1A3
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2521, "이벤트 기간 동안 몬스터들이 사탕과 알사탕을 드롭합니다. 달콤한 사탕으로 부족한 체력을 보강하시기 바랍니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2522, "사탕효과 : 복용할 경우 HP와 MP를 각 600씩 15초 동안 회복시켜 줍니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S( 2523, "알사탕효과 : 복용할 경우 15분간 최대HP 300과 최대MP 200이 증가됩니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2524, "이벤트 기간: 2006. 3. 10 ~ 2006. 3. 17" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 1220, "취소한다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 15 )
				{
					CTString strMessage;
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2598, "신서버 오픈 이벤트" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2599, "이벤트 기간동안 서버내 모든 몬스터가 부스터를 드롭할 확률이 2배 상승됩니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2600,"또한 자신의 레벨 이상의 몬스터를 사냥하면 일정확률로 경험치 포션과 공격력, 방어력 향상포션을 획득하실 수 있습니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );					
					strMessage = _S(2560, "공격력 향상제(소) : 5분간 물리공격력 35,마법공격력 30 증가" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2561, "방어력 향상제(소) : 5분간 물리방어력 55 상승" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2562, "공격력 향상제(대) : 5분간 물리공격력 100,마법공격력 80 증가" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2563, "방어력 향상제(대) : 5분간 물리방어력 95 상승" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S( 2601,"경험치 향상제: 20분 동안 습득경험치 1.5배 향상" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2602, "이벤트 기간: 2006. 4. 20 ~ 2006. 4. 28" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}else if( iEventIndex == 16 )
				{
						CTString strMessage;
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100," 이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2557, "혼돈의 권좌 이벤트 ");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";//A3A1A3
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2566, "신규 서버 오픈및 혼돈의 권좌 업데이트 관련 축하 이벤트로 부스터 이벤트, 경험치 포션 이벤트와 포션 이벤트가 진행됩니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2567, "이벤트 기간동안 모든 몬스터의 부스터 드랍확률이 2배가 상승되어 손쉽게 부스터를 습득할 수 있습니다.");
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2568, "또한 자신의 레벨 이상의 몬스터에게서 일정확률로 경험치 증가 포션과 공,방 향상 포션이 드롭됩니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage);
					strMessage = _S(2569, "경험치 포션 : 20분간 경험치 1.5배 획득" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2560, "공격력 향상제(소) : 5분간 물리공격력 35,마법공격력 30 증가" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2561, "방어력 향상제(소) : 5분간 물리방어력 55 상승" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2562, "공격력 향상제(대) : 5분간 물리공격력 100,마법공격력 80 증가" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2563, "방어력 향상제(대) : 5분간 물리방어력 95 상승" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2564, "이벤트 기간: 2006. 3. 30 ~ 2006. 4. 7" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 191, "확인" );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 17 ) // 일본 사쿠라 이벤트
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2530, "사쿠라 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2531, "봄맞이 사쿠라 이벤트는 사쿠라 꽃 이벤트와 일본 전통의상 지급 이벤트가 동시에 진행됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2532, "사쿠라 꽃 이벤트: 몬스터 사냥시 일정확률로 공격력과 방어력을 향상시켜주는 작은 분홍 벚꽃, 작은 흰 벚꽃, 분홍 벚꽃, 흰 벚꽃이 드롭됩니다." ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2533, "일본 전통의상 지급 이벤트: 사냥을 통하여 벚꽃 봉우리를 모아서 쥬노마을의 로레인에게 가져가면 일본 전통의상으로 교환 하실수 있습니다." ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2534, "단, 캐릭터의 레벨이상의 몬스터에게서만 아이템이 드롭됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2535, "이벤트 기간: 2006년 3월 23일 ~ 2006년 4월 6일" ) );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == 18 ) // O.X 이벤트
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2603,"O.X 퀴즈 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2604, "이벤트 기간 동안 라스트 카오스관련 게임 상식과 일반 상식을 모두 체크할 수 있는 기회로 O.X 이벤트를 진행합니다."));
					 _pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2605, "또한, 문제를 많이 맞출 수록 다양한 보상품이 지급됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2606, "진행 방식 : 이벤트 존에서 일정개수의 문제를 맞추면 존안에 생존해 보상품을 받을 수 있지만, 틀릴 경우 쥬노지역 란돌 마을로 추방을 당합니다." ) );

					if( g_uiEngineVersion < 10000 )	// 본섭일 경우
					{
						_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
							_S(2607, "이벤트는 각 서버군의 2채널에서만 진행 됩니다." ) );
						
						_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
						
						if( _pNetwork->m_iServerGroup < 4 )
						{
							_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
								_S(2608, "기간: 2006년 4월 18일 ~ 2006년 4월 24일(오후 8시)" ), -1, 0x6BD2FFFF );
						}
						else
						{
							_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
								_S(2609, "기간: 2006년 4월 20일 ~ 2006년 4월 24일(오후 8시)" ), -1, 0x6BD2FFFF );
						}
					}

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2610, "보상: 10문제: 부스터 10개" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2611, "      15문제: 문스톤 10개" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2612, "      18문제: 고급제련석 5개" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2613, "      20문제: 고급제련석 10개" ) );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				
				// ---------- WORLDCUP EVENT wooss 060530 --------------------------------------------------->>
				else if( iEventIndex == 19 ) // 월드컵 이벤트
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2771, "축구 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2772, "축구 대회 기간 동안 두 가지 이벤트가 진행됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2773, "1. 우승컵을 모아라" ), -1, 0xFF44AAFF );
					strMsg = _S( 2774, "축구 대회 기간동안 몬스터에게 드롭되는 FIFA컵을 모아 월드컵 우승이 예상되는 참가국의 국기와 교환하고" );
					strMsg+= _S( 2775, "교환된 국가의 대회 순위에 따라 보상을 받는 이벤트 입니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg , -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2776, "이벤트 기간" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2777, "① 우승컵 드롭&국기 교환 : 2006.06.09(화) ~ 06.30(금) 점검 전까지" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2778, "② 결과 보상 : 2006.07.10 이후" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2779, "2. 골든볼 이벤트" ) ,-1,0xFF44AAFF);
					strMsg = _S( 2780, "특정 경기의 결과를 예상해서 응모하는 이벤트로 이벤트가 시작되면 몬스터에게서 드롭되는 골든볼을 10개 모아" ) ;
					strMsg += _S( 2781, "승리국과 점수를 응모하고 골든볼 카드를 받습니다. 경기가 종료된 후 결과가 맞으면 보상을 받을 수 있습니다." ) ;
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg , -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2782, "이벤트 기간 : 2006.06.09 ~ 07.10" ), -1, 0x6BD2FFFF );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "확인" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// ---------- WORLDCUP EVENT wooss 060530 ---------------------------------------------------<<
				else if( iEventIndex == 100 ) // 골든 볼 이벤트 ( 네트워크에서 인덱스를 받지는 않는다. )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2783, "골든 볼 이벤트" ), -1, 0xE18600FF );
					strMsg = _S( 2784, "골든 볼 이벤트 응모가 시작 되었습니다. 팀가이스트 축구공 10개를 수집하여 쥬노의 란돌마을 랜디에게 찾아가" );
					
					CTString strMessage;
					strMessage.PrintF( _S( 2785, " %s VS %s 축구경기의 결과를 맞춰 보시기 바랍니다." ),
						_pUIMgr->GetQuest()->GetStrTeamA(), _pUIMgr->GetQuest()->GetStrTeamB() );

					strMsg += strMessage;
					strMsg += _S( 2786, "이벤트 종료 후 경기 결과에 따라 아이템과 나스를 지급합니다." );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2787, "골든볼 응모 가능 시간" ), -1, 0x6BD2FFFF );
					
					SYSTEMTIME GoldenTime = _pUIMgr->GetQuest()->GetGoldenTime();
					strMessage.PrintF( _S( 2788, "%d년 %d월 %d일 %d시 %d분 까지" ), 
						GoldenTime.wYear, GoldenTime.wMonth, GoldenTime.wDay, GoldenTime.wHour, GoldenTime.wMinute );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == 101 )// 골든 볼 이벤트 보상( 네트워크에서 인덱스를 받지는 않는다. )
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2789, "골든 볼 이벤트 보상" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2790, "골든 볼 이벤트 보상이 시작 되었습니다." ) );

					CTString strMessage;
					strMessage.PrintF( _S( 2791, "%s VS %s 축구경기의 결과는 %d : %d 입니다." ),
						_pUIMgr->GetQuest()->GetStrTeamA(), _pUIMgr->GetQuest()->GetStrTeamB(),
						_pUIMgr->GetQuest()->GetScoreTeamA(), _pUIMgr->GetQuest()->GetScoreTeamB() );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2792, "골든 볼 이벤트에 응모 하신 분은 쥬노 란돌 마을의 랜디를 찾아가서 이벤트에 대한 보상을 받으시기 바랍니다." ) );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2793, "골든볼 보상 시간" ), -1, 0x6BD2FFFF );
					
					SYSTEMTIME GoldenTime = _pUIMgr->GetQuest()->GetGoldenTime();
					strMessage.PrintF( _S( 2788, "%d년 %d월 %d일 %d시 %d분 까지" ), 
						GoldenTime.wYear, GoldenTime.wMonth, GoldenTime.wDay, GoldenTime.wHour, GoldenTime.wMinute );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == TEVENT_RAIN ) // 빗방울 이벤트  -> 어머니날 이벤트
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2753, "어머니날 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2892, "이벤트 기간 동안 몬스터를 사냥하면 일정확률로 빗방울, 빗물병 2종류의 아이템을 드롭 합니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2893, "이것을 쥬노지역의 랜디에게 가지고 가면, 빗방울 10개 또는 빗물병 2개를 1회에 한해 보상을 받을 수 있습니다. (반복수행가능)" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
									_S( 2894, "이벤트 기간 : 2006년 8월 9일 ~ 2006년 8월 30일" ), -1, 0x6BD2FFFF );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == TEVENT_BUDDHIST ) // 불교 촛불 이벤트
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2913, "불교 촛불 축제 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2914, "이벤트 기간 동안 모든 몬스터들로 부터 각종 색깔(적, 녹, 황, 남색)의 모래가 담긴 봉투 아이템이 드롭됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2915, "모래봉투를 모아 랜디를 찾아가면 보상아이템으로 교환을 받을 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2916, "이벤트 기간: 2006년 7월 11일 ~ 7월 26일" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2917, "자세한 내용은 웹사이트를 참고하세요." ) );

					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}				
				else if( iEventIndex == TEVENT_COLLECT_BUG )
				{	// 여름 이벤트 - 곤충 채집
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2940, "곤충 채집 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2941, "여름을 맞아 어린 시절을 회상해 볼 수 있는 곤충채집 이벤트를 진행합니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2942, "이벤트기간 동안 로레인에게 곤충채집상자를 구입하면 몬스터가 드롭하는 여름 곤충을 모아서 채집상자에 수집할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2943, "채집상자가 가득 차면 로레인에게 가져가서 채집한 곤충에 따라서 다양한 보상을 받을 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2944, "채집상자는 보상 후 사라지나 재 구입이 가능하여 계속해서 이벤트에 참가할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2945, "이벤트 기간:2006년 7월 20일 ~ 8월 31일" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == 23 )
				{	// 신섭 바스타드 오픈 이벤트 : 신섭
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "업데이트및 서버 오픈 기념 이벤트") , -1, 0xE18600FF );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트1. 축복의 물병을 모아라"),-1,0xF9E061FF  );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트 기간 동안 자신의 레벨에 맞는 몬스터를 사냥하면 축복의 물병이 드롭됩니다." ) );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "축복의 물병 효과 : 복용하면 20분간 몬스터의 경험치 20%를 추가 획득함." ) );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트2. 100만 나스르 잡아라!"),-1, 0xF9E061FF  );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트 기간동안 35레벨에 도달한 모든 캐릭터에게 100만 나스를 지급합니다." ) );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트 기간:2006년 7월 21일 ~ 8월 4일" ), -1, 0x6BD2FFFF );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == 24 )
				{	// 신섭 바스타드 오픈 이벤트 : 구섭
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "서드 임펙트 '혼돈의 시대' 업데이트 기념 이벤트" ), -1, 0xE18600FF );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트 기간 동안 자신의 레벨에 맞는 몬스터를 사냥하면 축복의 물병이 드롭됩니다." ) );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "축복의 물병 효과 : 복용하면 20분간 몬스터의 경험치 20%를 추가 획득함." ) );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "이벤트 기간:2006년 7월 21일 ~ 8월 4일" ), -1, 0x6BD2FFFF );//TRNS
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == TEVENT_CHUSEOK_2006 )
				{	// 2006 추석 이벤트	:Su-won
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3114, "추석맞이 송편 만들기 이벤트"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3115, "민족의 명절 추석을 맞이하여 송편만들기 이벤트를 진행합니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3116, "이벤트 기간 내에 몬스터를 사냥하면 송편의 재료인 쌀가루, 꿀, 솔잎을 모을 수 있는데, ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3117, "각 1개씩 모아 로레인에게 가져가면 일정확률로 송편을 만들어 줍니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3118, "송편을 먹으면 5분간 이동속도, 공격속도가 향상되며, 송편 10개를 모아서 오색송편을 만들어" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3119, "랜디에게 가져가면 나스와 고급제련석등으로 보상을 받을 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3120, "송편만들기 이벤트는 이벤트 기간동안 반복해서 수행할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3121, "이벤트 기간:2006년 9월 26일 ~ 10월 10일" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == TEVENT_HALLOWEEN )
				{
					// 빼빼로 데이 이벤트( == 판도라 상자 )
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3154, "판도라의 상자 이벤트" ), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3155, "판도라의 상자 이벤트 기간동안 쥬노, 드라탄, 메라크 지역에 각각 15개씩 판도라의 상자가 나타납니다. 판도라의 상자를 오픈 시 일정 확률로 빼빼로 이벤트 아이템 혹은 고제가 드롭되거나 판도라의 저주를 받은 몬스터가 출현하게 됩니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3156, "판도라의 상자는 일정시간 공격을 받지 않을경우 랜덤으로 다른 위치로 텔레포트 됩니다.") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3157, "이벤트 기간:2006년 11월 7일 ~ 11월 14일" ), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if( iEventIndex == TEVENT_XMAS_2006 )
				{
					// 2006 X-MAS Event [12/11/2006 Theodoric]
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3169, "크리스마스 이벤트"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  CTString(" "));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3170, "크리스마스 이벤트 기간 동안 이벤트 몬스터를 사냥하면 여러 가지 유용한 아이템을 얻을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3171, "얻은 아이템 중 케이크 아이템을 3개 모아서 마을에 위치한 눈사람에게 가져가시면 보상으로 눈사람 인형 아이템을 받으실 수 있습니다."));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3172, "기쁨이 가득한 크리스마스를 라스트 카오스 와 함께 더욱 즐겁게 보내세요."));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  CTString(" "));
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(1680, "이벤트 기간"), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3173, "2006년 11월 21일 ~ 2007년 1월 10일"), -1, 0x6BD2FFFF );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));									
				}
				else if( iEventIndex == CHILDREN_DAY_EVENT_INDEX )
				{
					//태국: 어린이날 이벤트
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 3184, "어린이날 이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3184, "어린이날 이벤트" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1945, "이벤트기간 동안 후견인이 후견인 시스템을 통해 신규유저를 양성하는데 성공하면 명성 포인트를 평소의 2배인 20포인트를 지급합니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3185, "이벤트기간 동안 견습생이 후견인 시스템을 통해 20레벨에 도달할 경우 기존의 이벤트 무기 이외에 500,000나스가 격려금으로 지급됩니다." ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3186, "기간: 2007년 1월 10일 ~ 2007년 1월 24일" ), -1, 0xE18600FF );			
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ));
				}
				else if( iEventIndex == 29 )
				{
					// 러브 러브 이벤트
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3203, "러브러브 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3204, "발렌타인데이와 민적의 명절인 설날을 맞이 하여 러브러브 이벤트를 실시합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3205, "이벤트 기간 동안 몬스터들이 카카오 함유량이 다른 3종류의 초코렛을 드롭합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3206, "초코렛은 그냥 사용할 수도 있고, 마법상점에서 판매하는 초코상자를 구입하여 빙고게임에 참여 할 수도 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3207, "초코상자에 초코렛을 넣기 위해서는 마법상점에서 초코포장지를 구입하여 초코렛을 담아야만 초코상자에 넣을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3208, "빙고가 완성된 초코상자는 이벤트 NPC 랜디에게 가져가면 빙고에 따라 상품을 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3209, "카카오 함유량이 높은 초코렛으로 빙고를 만들 경우 더 좋은 상품을 받을 확률이 높아집니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3210, "초코상자는 이벤트 종료 후 1주일간 존재하며 이후에는 사라집니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3211, "카카오 초코렛 효과"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3212, "카카오 함유량이 30,60,70인 3종류의 초코렛을 사용할 경우 마법, 물리 공격력을 5%,7%,10% 5분간 상승시켜 줍니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3213, "이벤트 기간: 2007.02.13 ~ 2007.02.20"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if( iEventIndex == 31 )
				{
					// 화이트데이 이벤트
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3227, "2007 화이트데이 러브매직 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3228, "화이트데이를 맞아 이벤트 기간동안 몬스터가 하늘색과 분홍색 사탕을 드롭합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3229, "몬스터가 드롭한 하늘색 사탕은 남성 캐릭터, 분홍색 사탕은 여성 캐릭터만 획득할 수 잇지만,"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3230, "획득한 이후에는 캐릭터간 교환이나 개인상점을 통해서 수집하실 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3231, "하늘색 사탕과 분홍색 사탕을 모아 쥬노란돌 마을에 있는 로레인에게 가져다 주면"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3232, "캐릭터의 능력을 높여 주는 러브매직 주문과 게임 전체에 공개적으로 사랑의 메시지를 전할 수 있는 편지지를 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3233, "◈하늘색사탕 5개 + 분홍색사탕 5개 : 6가지 러브매직 중 랜덤으로 하나의 호과를 즉시 받게 됩니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3234, "◈하늘색사탕 20개 : 하늘색 편지지 1장"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3235, "◈분홍색사탕 20개 : 분홍색 편지지 1장"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3236, "!이벤트 기간동안 사탕을 모아 반복해서 보상 받을 수 있지만 같은 종류의 편지지는 최대 5장까지만 소유 가능합니다."), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3237, "이벤트 기간: 2007.03.13 ~ 2007.03.27"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				// [KH_070326] 부활절 이벤트
				else if( iEventIndex == 33 )
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3268, "Multiple Easter Eggs Hunt"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3269, "4월8일 부활절을 맞아 부활절 달걀 사냥 이벤트를 진행합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3270, "이벤트기간 동안 게임 내 곳곳에 GM이 부활절 달걀을 떨어트립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3271, "바닥에 있는 달걀을 주워 개봉하면 랜덤하게 다양한 보상품을 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3272, "◈부활절 달걀은 동시에 1개 이상 획득할 수 없습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3273, "◈희귀한 확률로 몬스터도 부활절 달걀을 떨어트릴 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3274, "이벤트 기간: 2007.04.04 ~ "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if( iEventIndex == TEVENT_GOMDOLI )	
				{
					// 곰돌이 이벤트  35
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3457, "곰돌이 웬디를 찾아라!"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3458, "교복 셋트와 엽기무기를 체험하고 어린이들에게 라스트카오스 고객분들의 뜻을 모아 선물을 전할 수 있는 뜻 깊은 기회!!!"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3459, "빛의 대현자 '스타니슬라나'는 아이리스 대륙의 각 마을에 살고 있는 어린아이들에게 선물해줄 곰돌이 '웬디'를 만들고 있었습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3460, "그 과정 중 '스타니슬라나'의 숙적인 어둠의 마도사' 프라우론'의 흑마법에 의해 웬디가 그만 조각나 사악한 몬스터에게 넘어가 버리고 말았습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3461, "[스타니슬라나]의 충실한 종 '사로얀'은 아이리스 대륙의 몬스터에게서 곰돌이 조각을 찾아줄 용사를 찾고 있다고 합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3462, "캐릭터의 능력을 높여 주는 러브매직 주문과 게임 전체에 공개적으로 사랑의 메시지를 전할 수 있는 편지지를 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3463, "◈웬디 조각 9개를 모두 모아 [사로얀]에게 가져가면 가위바위보 게임을 통해 보상을 받을 수 있으며 참여한 1인 캐릭터에 한해 단 한번만 [체험용 교복 세트]와 [엽기무기]를 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3464, "◈완성된 곰돌이 웬디의 전체 누적 수에 따라 실제 곰 인형과 학용품 셋트를 어린이날 천사의 집 어린이들에게 선물하게 됩니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3465, "이벤트 기간: 2007년 4월 24일 ~ 2007년 4월 30일"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
// [KH_070425] 이벤트 관련 공지 추가(TEVENT_PARENTS_2007, TEVENT_CHILDREN_2007, TEVENT_TEACHER_2007, TEVENT_UCC_2007)
				else if(iEventIndex == TEVENT_PARENTS_2007)
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3362, "카네이션을 모아라~"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3363, "어버이날을 맞아 라스트카오스의 어버이 격인 길드마스터를 위한 이벤트를 준비했습니다." ) );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3364, "이벤트 기간 동안 몬스터가 드랍 한 빨간 카네이션을 유저간의 교환이나 개인상점을 통해서 수집할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3365, "빨간 카네이션은 오직 길드 마스터와 부 길드 마스터만이 NPC를 통해서 길드 포인트와 보상 품을 획득할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3366, "길드의 이름을 널리 알리고 진귀한 보상아이템도 받을 수 있는 이벤트에 많은 참여 바랍니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3367, "※ 길드 마스터와 부 길드 마스터만이 빨간 카네이션을 사용할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3368, "※ 길드 원들과 파티사냥을 하면 빨간 카네이션을 더욱 많이 구할 수 있습니다." ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3369, "이벤트 기간: 2007년 5월 8일 ~ 2007년 5월 15일" ) );
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if(iEventIndex == TEVENT_CHILDREN_2007)
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3370, "저 레벨 탈출작전"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3371, "어린이날을 맞아 라스트카오스의 어린이 격인 저 레벨을 위한 이벤트를 준비했습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3372, "이벤트 기간 동안 몬스터가 드랍 한 노란 개나리꽃을 유저간의 교환이나 개인상점을 통해서 수집할 수 있습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3373, "노란 개나리꽃을 모아 이벤트 NPC에게 가져다 주면 모험에 필요한 +3장비를 드립니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3374, "※ 이벤트 조건: 1레벨부터 24레벨 캐릭터") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3375, "※ 한번 받은 장비는 두 번 받으실 수 없습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3376, "※ 다른 직업의 장비는 받을 수 없고 자신의 캐릭터에 맞는 장비만 획득할 수 있습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3377, "※ 이벤트 기간 종료 시까지 사용하지 않는 노란 개나리꽃은 일괄 삭제됩니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3378, "이벤트 기간: 2007년 4월 30일 ~ 2007년 5월 15일") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if(iEventIndex == TEVENT_TEACHER_2007)
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3379, "스승의 은혜"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3380, "스승의 날을 맞아 라스트카오스의 스승 격인 후견인을 위한 이벤트를 준비했습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3381, "이벤트 기간 동안 견습생 양성을 성공하였을 경우 추가 보상이 주어집니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3382, "후견인과 견습생 둘 다 추가 보상이 주어지니 이번 이벤트에 많은 참여 부탁 드리겠습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3383, "※ 이벤트 기간 동안 후견인과 견습생간의 파티의 페널티가 없어집니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3384, "※ 이벤트 아이템은 인벤에서 이벤트 아이템 칸에 들어가게 됩니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3385, "이벤트 기간: 2007년 4월 30일 ~ 2007년 5월 15일") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if(iEventIndex == TEVENT_UCC_2007)
				{
					_pUIMgr->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					_pUIMgr->CreateMessageBoxL( _S( 100, "이벤트" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3386, "내가 만든 라카"), -1, 0xE18600FF );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3387, "라스트카오스를 즐기면서 재미있었던 에피소드 및 여러 가지 주제상관 없이") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3388, "라스트카오스에 관한 모든 소재로 UCC를 제작하여") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3389, "홈페이지를 통해서 올려주시면 유저심사 50% 자체심사 50%의 심사를 통해서 시상을 할 예정입니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3390, "푸짐한 상품들이 기다리고 있으니 많은 참여 부탁 드리겠습니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3391, "※ UCC (동영상, 일러스트, 카툰, 에피소드 글)") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3392, "※ 주제는 라스트카오스에 관한 모든 소재로 제작하여 주시면 됩니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3393, "※ 이벤트에 참여한 작품의 저작권은 저희 ㈜티 엔터테인먼트에 귀속됨을 알려드립니다.") );
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3394, "이벤트 기간: 2007년 4월 30일 ~ 2007년 5월 31일") );
					
					_pUIMgr->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "확인" ) );
				}
				else if(iEventIndex == TEVENT_FLOWERTREE)
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3504, "꽃놀이 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3505, "이벤트 기간 동안 몬스터를 사냥하면 꽃나무를 키울 수 있는 [퇴비]와 [정화수]가 드롭됩니다"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3506, "[퇴비]와 [정화수]를 모아 쥬노 북문에 있는 정원사에게 가져가면 정원사 옆에 있는 앙상한 나무에 꽃을 피우는데 도움을 줄 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3509, "이벤트 기간: 2007년 5월 15일 ~ 2007년 5월 29일"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3507, "※ 이벤트 참여방법: 사냥을 통해 [퇴비]와 [정화수]를 획득하여 정원사에게 가져가 개화를 도우면, 가져간 [퇴비]와 [정화수]의 숫자만큼 기여도가 증가되며, 전체 서버의 총 기여도가 증가됨에 따라 총 10단계로 꽃나무가 개화하게 됩니다."), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3508, "※ 이벤트 보상1: 자신의 기여도 100당 꽃놀이 교환권 1장을 받을 수 있습니다. 받은 교환권을 확인하면 고급제련석을 포함한 각종 보상품을 받을 수 있습니다."), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3524, "단 교환권을 받아도 기여도는 계속 누적되어 표시됩니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3525, "※ 이벤트 상품2: 이벤트가 종료 된 후 가장 높은 기여도를 달성한 서버 군에는 한달 동안 추천 서버 이벤트가 진행됩니다."), -1, 0xE18600FF);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				// [070705: Su-won] EVENT_SUMMER_2007
				else if( iEventIndex == TEVENT_SUMMER_2007)
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3561, "종이접기 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3562, "여름을 맞아 어린 시절을 회상해 볼 수 있는 종이접기 이벤트를 진행합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3563, "이벤트 기간동안 몬스터 사냥 시에 드롭되는 여러가지 색종이를 모아서 로레인에게 찾아가 부탁하여 색종이를 이용한 종이 접기를 하여 나오는 결과에 따라 보상을 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3564, "이벤트 종료 후 종이는 드롭되지 않으나 보상은 한달 후까지 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3565, "이벤트 기간: 2007년 7월 10일(화) ~ 2007년 8월 24일(화)"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if (iEventIndex == TEVENT_INDEPENDENCEDAY)
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3547, "독립기념일 이벤트"), -1, 0xE18600FF);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3548, "미국의 독립기념일을 맞아 이벤트를 진행합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3549, "이벤트 기간 동안 몬스터를 사냥하면 사냥에 도움을 주는 줄무늬, 별무늬 성조기와 각 클래스의 엉클 샘 모자를 획득할 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3550, "아이템 효과"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3551, "* 줄무늬 성조기: 사용 시 5분간 획득 경험치 및 SP를 50% 상승 시켜준다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3552, "* 별무늬 성조기: 사용 시 5분간 물리&마법 공격력, 물리방어력이 50이 증가된다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3553, "이벤트 기간: 2007년 7월 3일 ~2007년 7월 4일"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));

				}
				else if( iEventIndex == TEVENT_TG2007_1000DAYS || iEventIndex == TEVENT_TG2007_FRUITFULL ) // WSS_TG2007 2007/09/17 풍년 이벤트
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3761, "풍년 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3762, "수확의 기쁨을 누리기 위해 라스트카오스씨앗을 키워 [관심, 관용, 사랑, 질타]를 이용해서 다음단계로 업그레이드하면 보다 좋은 보상을 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3763, "최종단계까지 키우면 라스트카오스로고를 획득하여 이벤트 진행요원에게 고급의 아이템을 보상받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3764, "(이벤트 아이템은 이벤트 기간이 지나면 일과적으로 삭제됩니다)"));



					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(1680, "이벤트 기간"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3765, "2007년 9월 20일 ~ 2007년 10월 4일"));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));

				}
				else if (iEventIndex == TEVENT_HOLLOWEEN2007) // 할로윈 이벤트2007
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3797, "할로윈 이벤트"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3798, "1. 할로윈 이벤트 코스츔 의상 받기"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3799, "이벤트 기간 동안 호박광대 허수아비를 찾아가 말을 걸면 ""악마 날개 머리띠""를 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3800, "2. 호박 머리 탈 받기"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3801, "사냥 중 드롭되는 ""빈 사탕 바구니""를 가지고 호박광대 허수아비를 찾아가 말을 걸면 랜덤으로 6종의 ""사탕"" 중 1가지를 얻을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3802, "6종 모두를 얻은 후 호박광대 허수아비를 통해 ""호박 머리 탈""을 교환 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3803, "3. 마녀모자 및 마법사 모자 받기"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3804, "교환한 ""사탕""을 사용하면 랜덤으로 ""최상의 맛 증표""와 ""최악의 맛 증표""를 얻을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3805, "이 2종의 증표를 이벤트NPC를 통해 ""마녀모자 및 마법사모자""로 교환 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3806, "4. 호박머리 귀신 잡기"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3807, "필드에 숨어 있는 ""호박 머리 귀신""을 교환 받은 ""호박 머리 탈""을 쓰고 찾아 사냥 할 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3808, "드롭되는 ""호박"" 아이템은 호박광대 허수아비를 통하여 다양한 아이템으로 교환 가능합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(1680, "이벤트 기간"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3809, "2007년 10월 23일 ~ 2007년 11월 6일"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if (iEventIndex == TEVENT_XMAS_2007) // 크리스마스 이벤트 2007
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3169, "크리스마스 이벤트"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3957, "다가오는 2007 크리스마스와 2008 신년을 맞이 하여 두 가지 이벤트가 시작됩니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3958, "1. 크리스마스 트리장식"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3959, "이벤트 기간 동안 사냥을 통해서 크리스마스 장식 6종을 획득할 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3960, "크리스마스 장식을 쥬노마을 광장에 있는 크리스마스 트리에 가져가면 장식의 개수에 따라서 트리의 모습이 단계적으로 변하며 크리스마스 선물 아이템 및 각종 크리스마스 축복버프를 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3961, "2. 크리스마스 선물 상자"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3962, "이벤트 기간 동안 게임에 접속하여 1시간이 지나면 크리스마스 선물 상자가 소환되고, 소환된 크리스마스 선물상자를 사냥하면 루돌프 펫 변신 아이템 등 이벤트 아이템이 드롭됩니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(1680, "이벤트 기간"), -1, 0xE18600FF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3963, "2007년 12월 11일 ~ 2008년 1월 8일"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if( iEventIndex == TEVENT_NEWYEAR_2008) //2008년 신년 이벤트 
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3969, "신년 이벤트"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3970, "신년을 맞이하여 새해인사를 하면서 해맞이버프를 할 수있는 이벤트를 마련하였습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3971, "이벤트 기간: 2007년 12월 27일(목) ~ 2008년 1월 15일(화)"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3972, "이벤트 방법: 해맞이 인사할 유저를 선택하고 소셜창에서 인사를 하면 해맞이 버프와 새해인사를 하게 됩니다."), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3973, "새해인사는 제한없이 계속할 수 있으나 버프는 1시간마다 한번씩 받을 수 있습니다. 단, 해맞이버프 받기는 수락여부를 결정할 수 있습니다."), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				else if (iEventIndex == TEVENT_LUNARNEWYEAR_2008) // 2008 구정 이벤트
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3996, "2008 설날 복주머니 이벤트"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3997, "민족의 명절인 설날을 맞이 하여 2007년 러브러브 이벤트 2탄 설날 맞이 복주머니 이벤트를 실시합니다"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3998, "3종류의 복주머니는 개봉 시 각기 다른 효과를 주는데, 이 복주머니를 모아서 빙고게임을 위한 수집을 할 수도 있다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3999, "빙고게임에 참여하기 위해서는 상점에서 판매하는 소망상자를 구입하여야 하며, 복주머니는 상점에서 판매하는 노끈으로 묶어서 소망상자에 넣을 수 있다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4000, "새해 소망상자는 1캐릭터당 1개의 상자를 소지할 수 있으며, 상자는 창고보관이 되질 않는다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4001, "상자에는 3 * 3 총 9개의 복주머니를 담을 수 있으며, 같은 색깔의 복주머니를 같은 줄에 배치하여 빙고를 만들 수 있다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4002, "소망상자를 모두 채우면 이벤트 NPC를 찾아가 상품으로 교환할 수 있다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4003, "이벤트 기간: 2008년 2월 5일(화) ~ 2008년 2월 19일(화)"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}				
				else if( iEventIndex == TEVENT_SAKURA_2008 ) //2008 벚꽃 이벤트
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4104, "벚꽃 이벤트"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4105, "화창한 봄날 벚꽃 이벤트를 실시합니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4106, "이벤트 기간 동안 몬서터들에게서 퇴비를 획득하여 이벤트 NPC인 정원사를 찾아가면 벚꽃 선물상자로 교환받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4107, "벚꽃 선물상자는 더블클릭하여 열면 보상품을 받을 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4108, "고급제련석, 촤상급 문스톤, 상급 문스톤, 드롭율 증폭제, 부스터, 분홍 벚꽃, 흰 벚꽃, 작은 분홍 벚꽃, 작은 흰 벚꽃, 속도향상 물약을 보상품으로 획득할 수 있습니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4109, "이벤트 기간: 2008년 3월 27일 ~ 2008년 4월 10일"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}
				// [080422: Su-won] EVENT_CHAOSBALL
				else if( iEventIndex == TEVENT_CHAOSBALL) //카오스볼 이벤트
				{
					_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					_pUIMgr->CreateMessageBoxL(_S(100, "이벤트"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					CTString strReward;
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4118, "아이리스 상점 이벤트 [카오스 볼을 열어라!]"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4119, "아이리스 상점에서 추천하는 상품을 구입하시면 또 하나의 찬스! 카오스 볼을 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4120, "가지고 계신 카오스 볼은 쥬노 란돌마을에 파견되어 있는 이벤트 진행요원에게 가져가세요."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4121, "카오스 볼에 숨겨져 있는 각종 보물을 드립니다."));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4122, "숨겨진 보물 목록"), -1, 0xA3A1A3FF);
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(2844), 1);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					if (g_iCountry == GERMANY)
					{
						strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(974), 1);
						_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					}
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(1416), 1);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(1288), 1);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(85), 1);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4123, "스크롤 3종(경험/노력/행운) 1세트"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4124, "증폭제 3종(경험치/노력도/드롭율) 1세트"));
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(45), 10);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(724), 10);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4125, "12레벨 붉은 케르/푸른 케르 각 5개"));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4126, "12레벨 붉은 넨/푸른 넨 각 5개"));
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(786), 3);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					strReward.PrintF(_S(61, "%s %d개"), _pNetwork->GetItemName(556), 10);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					strReward.PrintF(_S(836, "%d 나스"), 10000);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4127, "이벤트 기간: 2008년 4월 29일 ~"), -1, 0x6BD2FFFF);
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));
				}				

				// NOTE : 쓰면 바로 지워져야 합니다.
				DelNoticeBySlotIndex( iSlotIndex );
				RefreshNoticeList();
			
			}
			else
			{
				//TODO : NewQuestSystem
				/*
				CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create( iNoticeIndex );
				_pUIMgr->GetCharacterInfo()->OpenQuestIntro( pQuestDD );
				*/
				_pUIMgr->GetQuestBookNew()->OpenQuestBook( iNoticeIndex );
				
			 	// NOTE : 쓰면 바로 지워져야 합니다.
				DelNoticeBySlotIndex( iSlotIndex );
				RefreshNoticeList();
			}
		}
		break;

	case UBET_ITEM:		// 변신 중지 버튼.
		{
			const int iStopChangeItem = 521;
			const int iStopTransformItem = 522;
			const int iIndex = rbtnSelect.GetIndex();

			// 강신 중지 버튼.
			if( iIndex == iStopTransformItem )
			{
				_pNetwork->SendStopEvocation();
			}
			// 변신 중지 버튼.
			else if( iIndex == iStopChangeItem )
			{
				_pNetwork->SendStopChange();
			}
		}
		break;
	case UBET_AUCTION:
		{
			_pUIMgr->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
			_pUIMgr->CreateMessageBoxL(_S(4356, "거래 대행 서비스 알림"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4356, "거래 대행 서비스 알림"), -1, 0xE18600FF);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4357, "귀하께서 맡기신 거래 물품을 정산할 수 있게되어 알려 드립니다."), -1, 0xE18600FF);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4358, "마을에 위치한 저희 상인의 후예 길드원을 통해 물품을 찾으실 수 있으니 꼭 길드원을 방문하셔서 물품을 수령하시길 바랍니다."), -1, 0xE18600FF);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4359, "15일 이내에 물품을 찾아가시지 않는 경우 물품들은 자동으로 폐기되니 이 점 필히 유념하시길 바랍니다."), -1, 0xE18600FF);
			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "), -1, 0xE18600FF);
			
			_pUIMgr->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "확인"));

			DelNoticeBySlotIndex( iSlotIndex );
			RefreshNoticeList();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_EVENT_NOTICE:
		{
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUINotice::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL bTitleBarClick = FALSE;
	
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	
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
			
		
			// Move remission
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			// Quest remission tab
			else
			{

				if( bLButtonDownInBtn && m_nSelNoticeID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{					
					m_abtnItems[m_nSelNoticeID].SetBtnState( UBES_IDLE );				
					bLButtonDownInBtn = FALSE;
				}
				
				// Active skill slot
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	nSlotIndex = -1;	
					for(int i = 0; i < nMaxNotice; ++i)
					{
						if( m_abtnItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
							nSlotIndex = i;
					}
					
					// Show tool tip
					ShowSlotInfo( TRUE, nSlotIndex );

					return WMSG_SUCCESS;
				}
			}
			ShowSlotInfo( FALSE );
		}
		break;
		
	case WM_LBUTTONDOWN:
		{

			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					m_nSelNoticeID = -1;
					for( int i = 0; i < nMaxNotice; ++i )
					{
						if( m_abtnItems[i].MouseMessage( pMsg ) != WMSG_FAIL )
						{							
							m_nSelNoticeID = i;
							bLButtonDownInBtn = TRUE;
							_pUIMgr->RearrangeOrder( UI_NOTICE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				_pUIMgr->RearrangeOrder( UI_NOTICE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		
	case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;
					
			
			// If holding button doesn't exist
			if( _pUIMgr->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If remission isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInsideRect( nX, nY, m_rcIcons ) )
				{	
					m_nSelNoticeID = -1;
					for(int i = 0; i < nMaxNotice; ++i)
					{
						if( ( wmsgResult = m_abtnItems[i].MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							if( wmsgResult == WMSG_COMMAND )
								PressOK( i );
							
							return WMSG_SUCCESS;
						}
					}
				}				
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					_pUIMgr->ResetHoldBtn();
					
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : AddBtnNotice()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::AddBtnNotice(int iNoticeIndex, eNoticeType iNoticeType)
{
	int k = nMaxNotice;
	
	// Find empty slot
	int iItem;
	for(  iItem = 0; iItem < nMaxNotice; ++iItem )
	{
		if( m_abtnItems[iItem].IsEmpty() )
			break;
	}
	
	if( iItem == nMaxNotice )
		return;
	
	switch(iNoticeType)
	{
	case NOTICE_QUEST:			// 퀘스트 타입일때...	
		{	
			CQuestDynamicData *pQuestDD = NULL;
			CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData( iNoticeIndex ));
			pQuestDD = &QuestDD;
			m_abtnItems[iItem].SetQuestInfo( iNoticeIndex, pQuestDD->GetQuestType1(), 0 );
		}
		break;

	case NOTICE_POLYMOPH:		// 변신에서 되돌아옴.
		{			
			m_abtnItems[iItem].SetItemInfo( -1, -1, -1, iStopChangeItem, -1, -1 );
		}
		break;

	case NOTICE_TRANSFORM:		// 변신체에서 되돌아옴.
		{
			m_abtnItems[iItem].SetItemInfo( -1, -1, -1, iStopTransformItem, -1, -1 );
		}
		break;

	case NOTICE_EVENT:			// 이벤트 알림.
		{			
			m_abtnItems[iItem].SetQuestInfo( iNoticeIndex, -1, 0 );
		}
		break;
	case NOTICE_AUCTION:
		{
			m_abtnItems[iItem].SetAuctionNotice(iNoticeIndex);
		}
		break;
	}
	m_nSelNoticeID	= -1;
}

// ----------------------------------------------------------------------------
// Name : AddToNoticeList()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::AddToNoticeList( int iNoticeIndex, eNoticeType iNoticeType )
{
	if( m_vectorNoticeList.size() > nMaxNotice )
		return;

//	ASSERT( iNoticeIndex >= EVENT_START_INDEX && iNoticeType == NOTICE_EVENT && "Invalid Event Index" );

	sNoticeInfo TempInfo;
	TempInfo.iIndex = iNoticeIndex;
	TempInfo.iType	= iNoticeType;

	std::vector<sNoticeInfo>::iterator iter = 
		std::find_if(m_vectorNoticeList.begin(), m_vectorNoticeList.end(), FindNotice(TempInfo) );

	if( iter == m_vectorNoticeList.end() )
	{
		m_vectorNoticeList.push_back(TempInfo);
	}
}

// ----------------------------------------------------------------------------
// Name : DelNoticeBySlotIndex()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::DelNoticeBySlotIndex( int iSlotIndex )
{
	if( iSlotIndex < 0 )
		return;

	ASSERT( iSlotIndex <= nMaxNotice && "Invalid Slot Index" );
	if( !m_abtnItems[iSlotIndex].IsEmpty() )
	{
		sNoticeInfo TempInfo;
		if( m_abtnItems[iSlotIndex].GetBtnType() == UBET_QUEST )
		{		
			TempInfo.iIndex = m_abtnItems[iSlotIndex].GetQuestIndex();
			TempInfo.iType	= NOTICE_QUEST;

			if( TempInfo.iIndex == PET_QUEST_HORSE_INDEX )
			{
				TempInfo.iType	= NOTICE_EVENT;
			}
			else if( TempInfo.iIndex == PET_QUEST_DRAGON_INDEX )
			{
				TempInfo.iType	= NOTICE_EVENT;
			}
			else if( TempInfo.iIndex >= EVENT_START_INDEX )
			{
				TempInfo.iType	= NOTICE_EVENT;
			}
		}
		// 변신체 및 몬스터 변신 처리...
		else if( m_abtnItems[iSlotIndex].GetBtnType() == UBET_ITEM )
		{
			const int iIndex = m_abtnItems[iSlotIndex].GetItemIndex(); 
			if( iIndex == iStopChangeItem )
			{
				TempInfo.iType	= NOTICE_POLYMOPH;
			}
			else if( iIndex == iStopTransformItem )
			{
				TempInfo.iType	= NOTICE_TRANSFORM;
			}
			TempInfo.iIndex = iIndex;
		}
		else if( m_abtnItems[iSlotIndex].GetBtnType() == UBET_AUCTION )
		{
			TempInfo.iIndex = m_abtnItems[iSlotIndex].GetAuctionIndex();
			TempInfo.iType	= NOTICE_AUCTION;
		}

		std::vector<sNoticeInfo>::iterator iter = 
			std::find_if(m_vectorNoticeList.begin(), m_vectorNoticeList.end(), FindNotice(TempInfo) );

		if( iter != m_vectorNoticeList.end() )
		{
			m_vectorNoticeList.erase( iter );
			//m_abtnItems[iSlotIndex].SetEmpty( TRUE );
			//m_abtnItems[iSlotIndex].InitBtn();
		}
	}

	m_nSelNoticeID	= -1;
	m_bShowSlotInfo = FALSE;
}

// ----------------------------------------------------------------------------
// Name : DelFromNoticeList()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::DelFromNoticeList( int iNoticeIndex, eNoticeType iNoticeType )
{
	if( iNoticeIndex < 0 )
		return;		
	
	sNoticeInfo TempInfo;		
	TempInfo.iIndex = iNoticeIndex;
	TempInfo.iType	= iNoticeType;		
	
	std::vector<sNoticeInfo>::iterator iter = 
			std::find_if(m_vectorNoticeList.begin(), m_vectorNoticeList.end(), FindNotice(TempInfo) );
		
	if( iter != m_vectorNoticeList.end() )
	{
		m_vectorNoticeList.erase( iter );	
	}
	
	m_nSelNoticeID	= -1;
	m_bShowSlotInfo = FALSE;
}

// ----------------------------------------------------------------------------
// Name : RefreshNoticeList()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::RefreshNoticeList()
{
	m_nSelNoticeID	= -1;
	m_bShowSlotInfo = FALSE;

	std::sort( m_vectorNoticeList.begin(), m_vectorNoticeList.end() );

	for( int i = 0; i < nMaxNotice; ++i)
	{
		m_abtnItems[i].InitBtn();
	}

	std::vector<sNoticeInfo>::const_iterator end = m_vectorNoticeList.end();
	for(std::vector<sNoticeInfo>::iterator it = m_vectorNoticeList.begin(); 
		it != end; ++it)
	{
		AddBtnNotice( (*it).iIndex, (*it).iType );
	}
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::Clear()
{
	m_nSelNoticeID	= -1;
	m_bShowSlotInfo = FALSE;

	if( !m_vectorNoticeList.empty() )
		m_vectorNoticeList.clear();

	for( int i = 0; i < nMaxNotice; ++i)
	{
		m_abtnItems[i].InitBtn();
	}

	m_nLeftTime		= 0;
	m_tmLeftTime	= 0;
}

// ----------------------------------------------------------------------------
// Name : UpdateTimeInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::UpdateTimeInfo()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds(); // 현재 시간 얻기
	__int64	llCurDelay = llCurTime - m_tmLeftTime;

	int lLeftTime = m_nLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	int iSec = lLeftTime % 60;
	lLeftTime /= 60;

	int iMin = lLeftTime % 60;	

	m_strSlotInfo.PrintF(_S(2338,"강신 취소[%d분 %d초 남음]"), iMin, iSec);
}

// ----------------------------------------------------------------------------
// Name : SetLeftTime()
// Desc :
// ----------------------------------------------------------------------------
void CUINotice::SetLeftTime( int nLeftTime )
{
	m_nLeftTime		= nLeftTime;
	m_tmLeftTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
}