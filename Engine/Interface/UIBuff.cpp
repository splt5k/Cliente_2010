#include "stdh.h"
#include <Engine/Interface/UIBuff.h>
#include <Engine/Interface/UIInternalClasses.h>


extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : Set()
// Desc :
// ----------------------------------------------------------------------------
void CBuffIcon::Set( SLONG slItemIndex, SLONG slSkillIndex, SBYTE sbLevel, SLONG slRemain, __int64 llStartTime )
{
	m_llStartTime = llStartTime;
	m_slItemIndex = slItemIndex;
	m_slSkillIndex = slSkillIndex;
	m_sbLevel = sbLevel;
	m_llRemain = slRemain * 100;

	FLOAT	fTexWidth, fTexHeight;
	int		nTexRow, nTexCol;

	// Skill
	if( slItemIndex == -1 )
	{
		CSkill	&rSkillData = _pNetwork->GetSkillData( slSkillIndex );

		m_nTextureID = rSkillData.GetIconTexID();
		fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_SKILL, m_nTextureID );
		fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_SKILL, m_nTextureID );
		nTexRow = rSkillData.GetIconTexRow();
		nTexCol = rSkillData.GetIconTexCol();
	}
	// Item
	else
	{
		CItemData	&rItemData = _pNetwork->GetItemData( slItemIndex );

		m_nTextureID = rItemData.GetIconTexID();
		fTexWidth = _pUIBtnTexMgr->GetTexWidth( UBET_ITEM, m_nTextureID );
		fTexHeight = _pUIBtnTexMgr->GetTexHeight( UBET_ITEM, m_nTextureID );
		nTexRow = rItemData.GetIconTexRow();
		nTexCol = rItemData.GetIconTexCol();
	}

	int	nUVSX = BTN_SIZE * nTexCol;
	int	nUVSY = BTN_SIZE * nTexRow;
	m_rtBuff.SetUV( nUVSX + 1, nUVSY + 1, nUVSX + BTN_SIZE - 1, nUVSY + BTN_SIZE - 1, fTexWidth, fTexHeight );
}


bool CBuffIcon::IsUpSkill() const
{
	switch( GetSkillIndex() )
	{
	case 247: // 경험치 증폭
	case 248: // 숙련도 증폭
	case 249: // 드롭률 증폭
	case 250: // 나스 증폭 
		return true;
		break;
	}
	
	return false;
}


// ----------------------------------------------------------------------------
// Name : CUIBuff()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIBuff::CUIBuff()
{
	m_ubMyGoodCount = 0;
	m_ubMyBadCount = 0;
	m_slPartyIndex = -1;
	m_ubPartyGoodCount = 0;
	m_ubPartyBadCount = 0;
	m_ubTargetGoodCount = 0;
	m_ubTargetBadCount = 0;
	m_ptdBuffTexture = NULL;
	m_nSelBuffID = -1;
	m_nSelBuffItemIndex = -1;
	m_nSelBuffSkillIndex = -1;
	m_sbSelBuffSkillLevel = -1;
	m_bShowBuffInfo = FALSE;
	m_nCurInfoLines = 0;
	m_nRemainTimeLine = 0;
	m_rcMyGoodBuff.SetRect( -1, -1, -1, -1 );
	for(int i =0 ;i< MAX_BUFF_ROW;i++){
		m_rcMyGoodBuffRow[i].SetRect(-1,-1,-1,-1);
	}
	m_rcMyBadBuff.SetRect( -1, -1, -1, -1 );
}

// ----------------------------------------------------------------------------
// Name : ~CUIBuff()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIBuff::~CUIBuff()
{
	if( m_ptdBuffTexture )
	{
		_pTextureStock->Release( m_ptdBuffTexture );
		m_ptdBuffTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::Create()
{
	// Create texture
	m_ptdBuffTexture = _pTextureStock->Obtain_t( CTString( "Data\\Interface\\Buff.tex" ) );
	FLOAT	fTexWidth = m_ptdBuffTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBuffTexture->GetPixHeight();

	// Outline
	m_rtOutlineBigGood.SetUV( 0, 0, 18, 18, fTexWidth, fTexHeight );
	m_rtOutlineBigBad.SetUV( 19, 0, 37, 18, fTexWidth, fTexHeight );
	m_rtOutlineSmallGood.SetUV( 38, 0, 60, 22, fTexWidth, fTexHeight );
	m_rtOutlineSmallBad.SetUV( 61, 0, 83, 22, fTexWidth, fTexHeight );

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
}

// ----------------------------------------------------------------------------
// Name : RenderToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RenderToolTip( int nWhichBuff, __int64 llCurTime )
{

	// Set buff texture
	_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left, m_rcBuffInfo.Top,
										m_rcBuffInfo.Left + 7, m_rcBuffInfo.Top + 7,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left + 7, m_rcBuffInfo.Top,
										m_rcBuffInfo.Right - 7, m_rcBuffInfo.Top + 7,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Right - 7, m_rcBuffInfo.Top,
										m_rcBuffInfo.Right, m_rcBuffInfo.Top + 7,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left, m_rcBuffInfo.Top + 7,
										m_rcBuffInfo.Left + 7, m_rcBuffInfo.Bottom - 7,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left + 7, m_rcBuffInfo.Top + 7,
										m_rcBuffInfo.Right - 7, m_rcBuffInfo.Bottom - 7,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Right - 7, m_rcBuffInfo.Top + 7,
										m_rcBuffInfo.Right, m_rcBuffInfo.Bottom - 7,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left, m_rcBuffInfo.Bottom - 7,
										m_rcBuffInfo.Left + 7, m_rcBuffInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Left + 7, m_rcBuffInfo.Bottom - 7,
										m_rcBuffInfo.Right - 7, m_rcBuffInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFFF );
	_pUIMgr->GetDrawPort()->AddTexture( m_rcBuffInfo.Right - 7, m_rcBuffInfo.Bottom - 7,
										m_rcBuffInfo.Right, m_rcBuffInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFFF );

	// Render all elements
	_pUIMgr->GetDrawPort()->FlushRenderingQueue();

	if( m_nCurInfoLines > 1 )
	{
		int		nSelBuff = m_nSelBuffID % BUFF_MAX_COUNT;
		__int64	llRemainTime;
		INDEX	iItemIndex;

		switch( nWhichBuff )
		{
		case MY_BUFF:
			llRemainTime = m_aMyBuff[nSelBuff].GetRemainTime( llCurTime ) *WORLDTIME_MUL / 1000;
			iItemIndex = m_aMyBuff[nSelBuff].GetItemIndex();
			break;

		case PARTY_BUFF:
			llRemainTime = m_aPartyBuff[nSelBuff].GetRemainTime( llCurTime ) *WORLDTIME_MUL / 1000;
			iItemIndex = m_aPartyBuff[nSelBuff].GetItemIndex();
			break;

		case TARGET_BUFF:
			llRemainTime = m_aTargetBuff[nSelBuff].GetRemainTime( llCurTime ) *WORLDTIME_MUL / 1000;
			iItemIndex = m_aTargetBuff[nSelBuff].GetItemIndex();
			break;
		}

		// Date : 2005-09-09(오후 4:12:22), By Lee Ki-hwan
		if( m_aMyBuff[nSelBuff].IsUpSkill() ) 
		{
			m_strBuffInfo[m_nRemainTimeLine].PrintF( _S( 1885, "남은 시간 : 무제한" ), llRemainTime ); 
		}
		else
		{
			if( llRemainTime < 0 ||
				//써치라이프는 남은 시간 표시하지 않음...
				(iItemIndex==2461 || iItemIndex==2462 || iItemIndex==2463 || iItemIndex==2606) )
				llRemainTime = 0;

			//wooss 050903
			else {
				CTString tv_str;
				int tv_tmp	=	llRemainTime%86400;		// 24*3600 
								
				int tv_day	=	llRemainTime/86400;					
				int tv_hour	=	tv_tmp/3600;			// 60*60
				tv_tmp		=	tv_tmp%3600;
				int tv_min	=	tv_tmp/60;
				int tv_sec	=	tv_tmp%60;
				
				m_strBuffInfo[m_nRemainTimeLine].PrintF( _S(2510, "남은 기간 : " ));
				
				if( tv_day >0 ) {
					tv_str.PrintF(_S(2511, "%d일 " ), tv_day); 
					m_strBuffInfo[m_nRemainTimeLine]+=tv_str; 
				}
				if( tv_hour >0 ) {
					tv_str.PrintF(_S(2512, "%d시간 " ), tv_hour); 
					m_strBuffInfo[m_nRemainTimeLine]+=tv_str; 
				}
				if( tv_min >0 ) {
					tv_str.PrintF(_S(2513, "%d분 " ), tv_min); 
					m_strBuffInfo[m_nRemainTimeLine]+=tv_str; 
				}
				if( tv_sec >0 && tv_day==0 && tv_hour==0 && tv_min==0) {
					tv_str.PrintF(_S(2514, "%d초 " ), tv_sec); 
					m_strBuffInfo[m_nRemainTimeLine]+=tv_str; 
				}

//				if( llRemainTime > 0 ) m_strBuffInfo[m_nRemainTimeLine].PrintF( _S( 1034, "남은 시간 : %I64d초" ), llRemainTime );
			}
			
		}
	}
	int	nInfoX = m_rcBuffInfo.Left + 12;
	int	nInfoY = m_rcBuffInfo.Top + 8;
	for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
	{
		_pUIMgr->GetDrawPort()->PutTextEx( m_strBuffInfo[iInfo], nInfoX, nInfoY, m_colBuffInfo[iInfo] );
		nInfoY += _pUIFontTexMgr->GetLineHeight();
	}

	// Flush all render text queue
	_pUIMgr->GetDrawPort()->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : RenderMyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RenderMyBuff()
{
	COLOR	colBlend;
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	// Good buff
	if( m_ubMyGoodCount > 0 )
	{
		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nOutX = m_nMyGoodBuffSX;
		int	nOutY = m_nMyGoodBuffSY;
		int	nBuffX = nOutX + 1;
		int	nBuffY = nOutY + 1;
		for( int i = 0; i < m_ubMyGoodCount; i++ )
		{
			if( (i%BUFF_BAD_COUNT) == 0 && i > 0)
			{
				nOutX = m_nMyGoodBuffSX;
				nOutY += BUFFOUT_SIZE_BIG;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_BIG;
				
			}

			if( m_aMyBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aMyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aMyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_BIG, nOutY + BUFFOUT_SIZE_BIG,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aMyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_BIG, nBuffY + BUFF_SIZE_BIG,
														m_aMyBuff[i].GetUV().U0,
														m_aMyBuff[i].GetUV().V0,
														m_aMyBuff[i].GetUV().U1,
														m_aMyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_BIG;
			nBuffX += BUFFOUT_SIZE_BIG;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		nOutX = m_nMyGoodBuffSX;
		nOutY = m_nMyGoodBuffSY;
		nBuffX = nOutX + 1;
		nBuffY = nOutY + 1;
		for(int i = 0; i < m_ubMyGoodCount; i++ )
		{
			if( (i%MAX_BUFF_COL) == 0 && i > 0)
			{
				nOutX = m_nMyGoodBuffSX;
				nOutY += BUFFOUT_SIZE_BIG;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_BIG;
			}

			if( m_aMyBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aMyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aMyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_BIG, nOutY + BUFFOUT_SIZE_BIG,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aMyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_BIG, nBuffY + BUFF_SIZE_BIG,
														m_aMyBuff[i].GetUV().U0,
											 			m_aMyBuff[i].GetUV().V0,
														m_aMyBuff[i].GetUV().U1,
														m_aMyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_BIG;
			nBuffX += BUFFOUT_SIZE_BIG;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Bad buff
	if( m_ubMyBadCount > 0 )
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubMyBadCount;

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nOutX = m_nMyBadBuffSX - BUFFOUT_SIZE_BIG;
		int	nOutY = m_nMyBadBuffSY;
		int	nBuffX = nOutX + 1;
		int	nBuffY = nOutY + 1;
		for( int i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aMyBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aMyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aMyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_BIG, nOutY + BUFFOUT_SIZE_BIG,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aMyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_BIG, nBuffY + BUFF_SIZE_BIG,
														m_aMyBuff[i].GetUV().U0,
														m_aMyBuff[i].GetUV().V0,
														m_aMyBuff[i].GetUV().U1,
														m_aMyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX -= BUFFOUT_SIZE_BIG;
			nBuffX -= BUFFOUT_SIZE_BIG;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		nOutX = m_nMyBadBuffSX - BUFFOUT_SIZE_BIG;
		nOutY = m_nMyBadBuffSY;
		nBuffX = nOutX + 1;
		nBuffY = nOutY + 1;
		for(int i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aMyBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aMyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aMyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_BIG, nOutY + BUFFOUT_SIZE_BIG,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aMyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_BIG, nBuffY + BUFF_SIZE_BIG,
														m_aMyBuff[i].GetUV().U0,
														m_aMyBuff[i].GetUV().V0,
														m_aMyBuff[i].GetUV().U1,
														m_aMyBuff[i].GetUV().V1,
														colBlend );
			
			nOutX -= BUFFOUT_SIZE_BIG;}
			nBuffX -= BUFFOUT_SIZE_BIG;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Tool tip
	if( m_bShowBuffInfo && m_nSelBuffID != -1 && m_nSelBuffID < BUFF_MAX_COUNT )
		RenderToolTip( MY_BUFF, llCurTime );
}

// ----------------------------------------------------------------------------
// Name : RenderPartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RenderPartyBuff()
{
	COLOR	colBlend;
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	// Background
	if( m_ubPartyGoodCount > 0 || m_ubPartyBadCount > 0 )
	{
		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		UIRect	rcTemp = m_rcPartyBuff;
		rcTemp.Left -= 1;
		rcTemp.Top -= 1;
		rcTemp.Right += 4;
		rcTemp.Bottom += 4;
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left, rcTemp.Top,
											rcTemp.Left + 7, rcTemp.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left + 7, rcTemp.Top,
											rcTemp.Right - 7, rcTemp.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Right - 7, rcTemp.Top,
											rcTemp.Right, rcTemp.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left, rcTemp.Top + 7,
											rcTemp.Left + 7, rcTemp.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left + 7, rcTemp.Top + 7,
											rcTemp.Right - 7, rcTemp.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Right - 7, rcTemp.Top + 7,
											rcTemp.Right, rcTemp.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left, rcTemp.Bottom - 7,
											rcTemp.Left + 7, rcTemp.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Left + 7, rcTemp.Bottom - 7,
											rcTemp.Right - 7, rcTemp.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		_pUIMgr->GetDrawPort()->AddTexture( rcTemp.Right - 7, rcTemp.Bottom - 7,
											rcTemp.Right, rcTemp.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();
	}

	// Good buff
	if( m_ubPartyGoodCount > 0 )
	{
		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nOutX = m_rcPartyBuff.Left;
		int	nOutY = m_rcPartyBuff.Top;
		int	nBuffX = nOutX + 1;
		int	nBuffY = nOutY + 1;
		for( int i = 0; i < m_ubPartyGoodCount; i++ )
		{
			if( (i%MAX_BUFF_COL) == 0 && i > 0)
			{
				nOutX = m_rcPartyBuff.Left;
				nOutY += BUFFOUT_SIZE_SMALL;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_SMALL;
				
			}

			if( m_aPartyBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aPartyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aPartyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aPartyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aPartyBuff[i].GetUV().U0,
														m_aPartyBuff[i].GetUV().V0,
														m_aPartyBuff[i].GetUV().U1,
														m_aPartyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		nOutX = m_rcPartyBuff.Left;
		nOutY = m_rcPartyBuff.Top;
		nBuffX = nOutX + 1;
		nBuffY = nOutY + 1;
		for(int i = 0; i < m_ubPartyGoodCount; i++ )
		{
			if( (i%MAX_BUFF_COL) == 0 && i > 0)
			{
				nOutX = m_rcPartyBuff.Left;
				nOutY += BUFFOUT_SIZE_SMALL;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_SMALL;
			}

			if( m_aPartyBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aPartyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aPartyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aPartyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aPartyBuff[i].GetUV().U0,
											 			m_aPartyBuff[i].GetUV().V0,
														m_aPartyBuff[i].GetUV().U1,
														m_aPartyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Bad buff
	if( m_ubPartyBadCount > 0 )
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubPartyBadCount;

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nBuffX, nBuffY, nOutX, nOutY;
		// Three line
		if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL * 2;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// Two line
		else if( m_ubPartyGoodCount > 0 )
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// One line
		else
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		for( int i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aPartyBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aPartyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aPartyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}
				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aPartyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aPartyBuff[i].GetUV().U0,
														m_aPartyBuff[i].GetUV().V0,
														m_aPartyBuff[i].GetUV().U1,
														m_aPartyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		// Three line
		if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL * 2;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// Two line
		else if( m_ubPartyGoodCount > 0 )
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// One line
		else
		{
			nOutX = m_rcPartyBuff.Left;
			nOutY = m_rcPartyBuff.Top;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		for(int i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aPartyBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aPartyBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aPartyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aPartyBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aPartyBuff[i].GetUV().U0,
														m_aPartyBuff[i].GetUV().V0,
														m_aPartyBuff[i].GetUV().U1,
														m_aPartyBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Tool tip
	if( m_bShowBuffInfo && m_nSelBuffID != -1 &&
		m_nSelBuffID >= BUFF_MAX_COUNT && m_nSelBuffID < BUFF_MAX_COUNT * 2 )
		RenderToolTip( PARTY_BUFF, llCurTime );
}

// ----------------------------------------------------------------------------
// Name : RenderTargetBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RenderTargetBuff()
{
	COLOR	colBlend;
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	int	nX, nY;
	_pUIMgr->GetTargetInfo()->GetAbsPos( nX, nY );
	nX += ( _pUIMgr->GetTargetInfo()->GetWidth() - m_rcTempTargetBuff.Right ) / 2;
	nY += _pUIMgr->GetTargetInfo()->GetHeight() + 1;
	m_rcTargetBuff = m_rcTempTargetBuff;
	m_rcTargetBuff.Offset( nX, nY );

	// Good buff
	if( m_ubTargetGoodCount > 0 )
	{
		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nOutX = m_rcTargetBuff.Left;
		int	nOutY = m_rcTargetBuff.Top;
		int	nBuffX = nOutX + 1;
		int	nBuffY = nOutY + 1;
		for( int i = 0; i < m_ubTargetGoodCount; i++ )
		{
			if( (i%MAX_BUFF_COL) == 0 && i > 0)
			{
				nOutX = m_rcTargetBuff.Left;
				nOutY += BUFFOUT_SIZE_SMALL;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_SMALL;
			}

			if( m_aTargetBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aTargetBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aMyBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aTargetBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aTargetBuff[i].GetUV().U0,
														m_aTargetBuff[i].GetUV().V0,
														m_aTargetBuff[i].GetUV().U1,
														m_aTargetBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		nOutX = m_rcTargetBuff.Left;
		nOutY = m_rcTargetBuff.Top;
		nBuffX = nOutX + 1;
		nBuffY = nOutY + 1;
		for(int i = 0; i < m_ubTargetGoodCount; i++ )
		{
			if( (i%MAX_BUFF_COL) == 0 && i > 0)
			{
				nOutX = m_rcTargetBuff.Left;
				nOutY += BUFFOUT_SIZE_SMALL;
				nBuffX = nOutX + 1;
				nBuffY += BUFFOUT_SIZE_SMALL;
				
			}

			if( m_aTargetBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aTargetBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aTargetBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigGood.U0, m_rtOutlineBigGood.V0,
													m_rtOutlineBigGood.U1, m_rtOutlineBigGood.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aTargetBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aTargetBuff[i].GetUV().U0,
											 			m_aTargetBuff[i].GetUV().V0,
														m_aTargetBuff[i].GetUV().U1,
														m_aTargetBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Bad buff
	if( m_ubTargetBadCount > 0 )
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubTargetBadCount;

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Skill
		int	nBuffX, nBuffY, nOutX, nOutY;
		// Three line
		if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL * 2;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// Tow line
		else if( m_ubTargetGoodCount > 0 )
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// One line
		else
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		int i;
		for(  i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aTargetBuff[i].GetItemIndex() == -1 )
			{
				__int64	llRemainTime = m_aTargetBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aTargetBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}

				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aTargetBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aTargetBuff[i].GetUV().U0,
														m_aTargetBuff[i].GetUV().V0,
														m_aTargetBuff[i].GetUV().U1,
														m_aTargetBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_SKILL );

		// Set buff texture
		_pUIMgr->GetDrawPort()->InitTextureData( m_ptdBuffTexture );

		// Item
		// Three line
		if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL * 2;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// Two line
		else if( m_ubTargetGoodCount > 0 )
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		// One line
		else
		{
			nOutX = m_rcTargetBuff.Left;
			nOutY = m_rcTargetBuff.Top;
			nBuffX = nOutX + 1;
			nBuffY = nOutY + 1;
		}
		for( i = BUFF_GOOD_COUNT; i < nBadBuffEnd; i++ )
		{
			if( m_aTargetBuff[i].GetItemIndex() != -1 )
			{
				__int64	llRemainTime = m_aTargetBuff[i].GetRemainTime( llCurTime ) / 1000;
				if( llRemainTime < 0 ) llRemainTime = 0;
				if( llRemainTime < 10 && llRemainTime % 2 )
				{
					if( m_aTargetBuff[i].IsUpSkill() )
						colBlend = 0xFFFFFFFF;
					else
						colBlend = 0x808080FF;
				}
				else
				{
					colBlend = 0xFFFFFFFF;
				}
				_pUIMgr->GetDrawPort()->AddTexture( nOutX, nOutY,
													nOutX + BUFFOUT_SIZE_SMALL, nOutY + BUFFOUT_SIZE_SMALL,
													m_rtOutlineBigBad.U0, m_rtOutlineBigBad.V0,
													m_rtOutlineBigBad.U1, m_rtOutlineBigBad.V1,
													0xFFFFFFFF );

				_pUIMgr->GetDrawPort()->AddBtnTexture( m_aTargetBuff[i].GetTextureID(),
														nBuffX, nBuffY, nBuffX + BUFF_SIZE_SMALL, nBuffY + BUFF_SIZE_SMALL,
														m_aTargetBuff[i].GetUV().U0,
														m_aTargetBuff[i].GetUV().V0,
														m_aTargetBuff[i].GetUV().U1,
														m_aTargetBuff[i].GetUV().V1,
														colBlend );
			}
			nOutX += BUFFOUT_SIZE_SMALL;
			nBuffX += BUFFOUT_SIZE_SMALL;
		}

		// Render all elements
		_pUIMgr->GetDrawPort()->FlushRenderingQueue();

		// Render all button elements
		_pUIMgr->GetDrawPort()->FlushBtnRenderingQueue( UBET_ITEM );
	}

	// Tool tip
	if( m_bShowBuffInfo && m_nSelBuffID != -1 &&
		m_nSelBuffID >= BUFF_MAX_COUNT * 2 && m_nSelBuffID < BUFF_MAX_COUNT * 3 )
		RenderToolTip( TARGET_BUFF, llCurTime );
}

// ----------------------------------------------------------------------------
// Name : AdjustMyBuffRegion()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AdjustMyBuffRegion( BOOL bGoodBuff )
{
	if( bGoodBuff )
	{
		// Multi line
		for( int i=0 ; i < MAX_BUFF_ROW ;i++)
		{
			if( m_ubMyGoodCount < MAX_BUFF_COL*(i+1))
			{
				m_rcMyGoodBuffRow[i].SetRect(	m_nMyGoodBuffSX,
												m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG*i,
												m_nMyGoodBuffSX + (m_ubMyGoodCount%MAX_BUFF_COL) * BUFFOUT_SIZE_BIG,
												m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG*(i+1) );
				m_rcMyGoodBuff.SetRect( m_nMyGoodBuffSX,
										m_nMyGoodBuffSY,
										m_nMyGoodBuffSX + MAX_BUFF_COL * BUFFOUT_SIZE_BIG,
										m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG *(i+1) );
				break;
			}
			else {
				m_rcMyGoodBuffRow[i].SetRect( m_nMyGoodBuffSX,
										m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG*i,
										m_nMyGoodBuffSX + MAX_BUFF_COL * BUFFOUT_SIZE_BIG,
										m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG*(i+1) );
								
				
			}
			
		}
	}
	else
	{
		m_rcMyBadBuff.SetRect( m_nMyBadBuffSX - m_ubMyBadCount * BUFFOUT_SIZE_BIG,
								m_nMyBadBuffSY,
								m_nMyBadBuffSX,
								m_nMyBadBuffSY + BUFFOUT_SIZE_BIG );
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustPartyBuffRegion()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AdjustPartyBuffRegion()
{
	int	nMaxRow, nMaxCol;

	if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
	{
		nMaxCol = MAX_BUFF_COL;
		nMaxRow = 2;
	}
	else
	{
		nMaxCol = m_ubPartyGoodCount;
		if( m_ubPartyGoodCount > 0 )
			nMaxRow = 1;
		else
			nMaxRow = 0;
	}

	if( nMaxCol < m_ubPartyBadCount )
		nMaxCol = m_ubPartyBadCount;

	if( m_ubPartyBadCount > 0 )
		nMaxRow++;

	m_rcTempPartyBuff.SetRect( 0, -( nMaxRow * BUFFOUT_SIZE_SMALL ), nMaxCol * BUFFOUT_SIZE_SMALL, 0 );

	if( m_nPartyBuffSX + m_rcTempPartyBuff.Right > _pUIMgr->GetMaxI() )
	{
		m_nPartyBuffSX += 10;
		int	nTemp = m_rcTempPartyBuff.Left;
		m_rcTempPartyBuff.Left = m_rcTempPartyBuff.Right;
		m_rcTempPartyBuff.Right = nTemp;
	}
	if( m_nPartyBuffSY + m_rcTempPartyBuff.Top < _pUIMgr->GetMinI() )
	{
		m_nPartyBuffSY -= 10;
		int	nTemp = m_rcTempPartyBuff.Top;
		m_rcTempPartyBuff.Top = m_rcTempPartyBuff.Bottom;
		m_rcTempPartyBuff.Bottom = m_rcTempPartyBuff.Top;
		m_rcTempPartyBuff.Top = nTemp;
	}
	m_rcPartyBuff = m_rcTempPartyBuff;
	m_rcPartyBuff.Offset( m_nPartyBuffSX, m_nPartyBuffSY );
}

// ----------------------------------------------------------------------------
// Name : AdjustTargetBuffRegion()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AdjustTargetBuffRegion()
{
	int	nMaxRow, nMaxCol;

	if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
	{
		nMaxCol = MAX_BUFF_COL;
		nMaxRow = m_ubTargetGoodCount / MAX_BUFF_COL + 1;
	}
	else
	{
		nMaxCol = m_ubTargetGoodCount;
		if( m_ubTargetGoodCount > 0 )
			nMaxRow = 1;
		else
			nMaxRow = 0;
	}

	if( nMaxCol < m_ubTargetBadCount )
		nMaxCol = m_ubTargetBadCount;

	if( m_ubTargetBadCount > 0 )
		nMaxRow++;

	m_rcTempTargetBuff.SetRect( 0, 0, nMaxCol * BUFFOUT_SIZE_SMALL, nMaxRow * BUFFOUT_SIZE_SMALL );

	int	nX, nY;
	_pUIMgr->GetTargetInfo()->GetAbsPos( nX, nY );
	nX += ( _pUIMgr->GetTargetInfo()->GetWidth() - m_rcTempTargetBuff.Right ) / 2;
	nY += _pUIMgr->GetTargetInfo()->GetHeight() + 1;
	m_rcTargetBuff = m_rcTempTargetBuff;
	m_rcTargetBuff.Offset( nX, nY );
}

// ----------------------------------------------------------------------------
// Name : CopyMyBuffToTargetBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::CopyMyBuffToTargetBuff()
{
	int i;
	for(  i = 0; i < m_ubMyGoodCount; i++ )
		AddTargetBuff( m_aMyBuffInfo[i] );

	for( i = BUFF_GOOD_COUNT; i < BUFF_GOOD_COUNT + m_ubMyBadCount; i++ )
		AddTargetBuff( m_aMyBuffInfo[i] );
}

// ----------------------------------------------------------------------------
// Name : AddMyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AddMyBuff( BuffInfo &rBuffInfo )
{
	CSkill	&rSkillData = _pNetwork->GetSkillData( rBuffInfo.m_slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		m_aMyBuffInfo[m_ubMyGoodCount] = rBuffInfo;
		m_aMyBuff[m_ubMyGoodCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
											rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubMyGoodCount++;
		AdjustMyBuffRegion( TRUE );
	}
	// Bad
	else
	{
		m_aMyBuffInfo[BUFF_GOOD_COUNT + m_ubMyBadCount] = rBuffInfo;
		m_aMyBuff[BUFF_GOOD_COUNT + m_ubMyBadCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
															rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubMyBadCount++;
		AdjustMyBuffRegion( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : AddPartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AddPartyBuff( SLONG slPartyIndex, BuffInfo &rBuffInfo )
{
	if( m_slPartyIndex != slPartyIndex )
		return;

	CSkill	&rSkillData = _pNetwork->GetSkillData( rBuffInfo.m_slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		m_aPartyBuff[m_ubPartyGoodCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
												rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubPartyGoodCount++;
	}
	// Bad
	else
	{
		m_aPartyBuff[BUFF_GOOD_COUNT + m_ubPartyBadCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
																rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubPartyBadCount++;
	}

	AdjustPartyBuffRegion();
}

// ----------------------------------------------------------------------------
// Name : AddTargetBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AddTargetBuff( BuffInfo &rBuffInfo )
{
	CSkill	&rSkillData = _pNetwork->GetSkillData( rBuffInfo.m_slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		m_aTargetBuff[m_ubTargetGoodCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
													rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubTargetGoodCount++;
	}
	// Bad
	else
	{
		m_aTargetBuff[BUFF_GOOD_COUNT + m_ubTargetBadCount].Set( rBuffInfo.m_slItemIndex, rBuffInfo.m_slSkillIndex, rBuffInfo.m_sbLevel,
																	rBuffInfo.m_slRemain, rBuffInfo.m_llStartTime );
		m_ubTargetBadCount++;
	}

	AdjustTargetBuffRegion();
}

// ----------------------------------------------------------------------------
// Name : RemoveMyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RemoveMyBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	CSkill	&rSkillData = _pNetwork->GetSkillData( slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		int iBuff;
		for(  iBuff = 0; iBuff < m_ubMyGoodCount; iBuff++ )
		{
			if( m_aMyBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aMyBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == m_ubMyGoodCount )
			return;

		for( iBuff++; iBuff < m_ubMyGoodCount; iBuff++ )
		{
			m_aMyBuffInfo[iBuff - 1] = m_aMyBuffInfo[iBuff];
			m_aMyBuff[iBuff - 1] = m_aMyBuff[iBuff];
		}

		m_ubMyGoodCount--;
		m_aMyBuffInfo[m_ubMyGoodCount].Init();
		m_aMyBuff[m_ubMyGoodCount].Init();

		AdjustMyBuffRegion( TRUE );
	}
	// Bad
	else
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubMyBadCount;
		int iBuff;
		for(  iBuff = BUFF_GOOD_COUNT; iBuff < nBadBuffEnd; iBuff++ )
		{
			if( m_aMyBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aMyBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == nBadBuffEnd )
			return;

		for( iBuff++; iBuff < nBadBuffEnd; iBuff++ )
		{
			m_aMyBuffInfo[iBuff - 1] = m_aMyBuffInfo[iBuff];
			m_aMyBuff[iBuff - 1] = m_aMyBuff[iBuff];
		}

		m_ubMyBadCount--;
		m_aMyBuffInfo[BUFF_GOOD_COUNT + m_ubMyBadCount].Init();
		m_aMyBuff[BUFF_GOOD_COUNT + m_ubMyBadCount].Init();

		AdjustMyBuffRegion( FALSE );
	}

	if( m_bShowBuffInfo )
	{
		int	nWhichBuff = m_nSelBuffID / BUFF_MAX_COUNT;
		if( nWhichBuff != MY_BUFF )
			return;

		int		nSelBuff = m_nSelBuffID % BUFF_MAX_COUNT;
		int		nItemIndex = m_aMyBuff[nSelBuff].GetItemIndex();
		int		nSkillIndex = m_aMyBuff[nSelBuff].GetSkillIndex();
		SBYTE	sbSkillLevel = m_aMyBuff[nSelBuff].GetLevel();
		if( nItemIndex == m_nSelBuffItemIndex &&
			nSkillIndex == m_nSelBuffSkillIndex &&
			sbSkillLevel == m_sbSelBuffSkillLevel )
			return;

		if( nSelBuff >= BUFF_GOOD_COUNT + m_ubMyBadCount ||
			( nSelBuff >= m_ubMyGoodCount && nSelBuff < BUFF_GOOD_COUNT ) )
			ShowBuffInfo( FALSE );
		else
			ShowBuffInfo( TRUE, MY_BUFF, nSelBuff );
	}
}

// ----------------------------------------------------------------------------
// Name : RemovePartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RemovePartyBuff( SLONG slPartyIndex, SLONG slItemIndex, SLONG slSkillIndex )
{
	if( m_slPartyIndex != slPartyIndex )
		return;

	CSkill	&rSkillData = _pNetwork->GetSkillData( slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		int iBuff;
		for(  iBuff = 0; iBuff < m_ubPartyGoodCount; iBuff++ )
		{
			if( m_aPartyBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aPartyBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == m_ubPartyGoodCount )
			return;

		for( iBuff++; iBuff < m_ubPartyGoodCount; iBuff++ )
		{
			m_aPartyBuff[iBuff - 1] = m_aPartyBuff[iBuff];
		}

		m_ubPartyGoodCount--;
		m_aPartyBuff[m_ubPartyGoodCount].Init();
	}
	// Bad
	else
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubPartyBadCount;
		int iBuff;
		for(  iBuff = BUFF_GOOD_COUNT; iBuff < nBadBuffEnd; iBuff++ )
		{
			if( m_aPartyBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aPartyBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == nBadBuffEnd )
			return;

		for( iBuff++; iBuff < nBadBuffEnd; iBuff++ )
		{
			m_aPartyBuff[iBuff - 1] = m_aPartyBuff[iBuff];
		}

		m_ubPartyBadCount--;
		m_aPartyBuff[BUFF_GOOD_COUNT + m_ubPartyBadCount].Init();
	}

	AdjustPartyBuffRegion();

	if( m_bShowBuffInfo )
	{
		int	nWhichBuff = m_nSelBuffID / BUFF_MAX_COUNT;
		if( nWhichBuff != PARTY_BUFF )
			return;

		int		nSelBuff = m_nSelBuffID % BUFF_MAX_COUNT;
		int		nItemIndex = m_aMyBuff[nSelBuff].GetItemIndex();
		int		nSkillIndex = m_aMyBuff[nSelBuff].GetSkillIndex();
		SBYTE	sbSkillLevel = m_aMyBuff[nSelBuff].GetLevel();
		if( nItemIndex = m_nSelBuffItemIndex &&
			nSkillIndex == m_nSelBuffSkillIndex &&
			sbSkillLevel == m_sbSelBuffSkillLevel )
			return;

		if( nSelBuff >= BUFF_GOOD_COUNT + m_ubPartyBadCount ||
			( nSelBuff >= m_ubPartyGoodCount && nSelBuff < BUFF_GOOD_COUNT ) )
			ShowBuffInfo( FALSE );
		else
			ShowBuffInfo( TRUE, PARTY_BUFF, nSelBuff );
	}
}

// ----------------------------------------------------------------------------
// Name : RemoveTargetBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::RemoveTargetBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	CSkill	&rSkillData = _pNetwork->GetSkillData( slSkillIndex );

	// Good
	if( rSkillData.GetFlag() & SF_FORHELP )
	{
		int iBuff;
		for(  iBuff = 0; iBuff < m_ubTargetGoodCount; iBuff++ )
		{
			if( m_aTargetBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aTargetBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == m_ubTargetGoodCount )
			return;

		for( iBuff++; iBuff < m_ubTargetGoodCount; iBuff++ )
		{
			m_aTargetBuff[iBuff - 1] = m_aTargetBuff[iBuff];
		}

		m_ubTargetGoodCount--;
		m_aTargetBuff[m_ubTargetGoodCount].Init();
	}
	// Bad
	else
	{
		int	nBadBuffEnd = BUFF_GOOD_COUNT + m_ubTargetBadCount;
		int iBuff;
		for(  iBuff = BUFF_GOOD_COUNT; iBuff < nBadBuffEnd; iBuff++ )
		{
			if( m_aTargetBuff[iBuff].GetItemIndex() == slItemIndex &&
				m_aTargetBuff[iBuff].GetSkillIndex() == slSkillIndex )
				break;
		}

		if( iBuff == nBadBuffEnd )
			return;

		for( iBuff++; iBuff < nBadBuffEnd; iBuff++ )
		{
			m_aTargetBuff[iBuff - 1] = m_aTargetBuff[iBuff];
		}

		m_ubTargetBadCount--;
		m_aTargetBuff[BUFF_GOOD_COUNT + m_ubTargetBadCount].Init();
	}

	AdjustTargetBuffRegion();

	if( m_bShowBuffInfo )
	{
		int	nWhichBuff = m_nSelBuffID / BUFF_MAX_COUNT;
		if( nWhichBuff != TARGET_BUFF )
			return;

		int		nSelBuff = m_nSelBuffID % BUFF_MAX_COUNT;
		int		nItemIndex = m_aMyBuff[nSelBuff].GetItemIndex();
		int		nSkillIndex = m_aMyBuff[nSelBuff].GetSkillIndex();
		SBYTE	sbSkillLevel = m_aMyBuff[nSelBuff].GetLevel();
		if( nItemIndex == m_nSelBuffItemIndex &&
			nSkillIndex == m_nSelBuffSkillIndex &&
			sbSkillLevel == m_sbSelBuffSkillLevel )
			return;

		if( nSelBuff >= BUFF_GOOD_COUNT + m_ubTargetBadCount ||
			( nSelBuff >= m_ubTargetGoodCount && nSelBuff < BUFF_GOOD_COUNT ) )
			ShowBuffInfo( FALSE );
		else
			ShowBuffInfo( TRUE, TARGET_BUFF, nSelBuff );
	}
}

// ----------------------------------------------------------------------------
// Name : AddBuffInfoString()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::AddBuffInfoString( CTString &strBuffInfo, COLOR colBuffInfo )
{
	if( m_nCurInfoLines >= MAX_BUFFINFO_LINE )
		return;

	// Get length of string
	INDEX	nLength = strBuffInfo.Length();
	if( nLength <= 0 )
		return;
	
	
	// wooss 051002
	if(g_iCountry == THAILAND){
		// Get length of string
		INDEX	nThaiLen = FindThaiLen(strBuffInfo);
		INDEX	nChatMax= (MAX_BUFFINFO_CHAR-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
		if( nLength == 0 )
			return;
		// If length of string is less than max char
		if( nThaiLen <= nChatMax )
		{
			m_strBuffInfo[m_nCurInfoLines] = strBuffInfo;
			m_colBuffInfo[m_nCurInfoLines++] = colBuffInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_BUFFINFO_CHAR;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nLength; iPos++ )
			{
				if(nChatMax < FindThaiLen(strBuffInfo,0,iPos))
					break;
			}
			nSplitPos = iPos;

			// Split string
			CTString	strTemp;
			strBuffInfo.Split( nSplitPos, m_strBuffInfo[m_nCurInfoLines], strTemp );
			m_colBuffInfo[m_nCurInfoLines++] = colBuffInfo;

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

			AddBuffInfoString( strTemp, colBuffInfo );

		}
		
	} else {
	
		// If length of string is less than max char
		if( nLength <= MAX_BUFFINFO_CHAR )
		{
			m_strBuffInfo[m_nCurInfoLines] = strBuffInfo;
			m_colBuffInfo[m_nCurInfoLines++] = colBuffInfo;
		}
		// Need multi-line
		else
		{
			// Check splitting position for 2 byte characters
			int		nSplitPos = MAX_BUFFINFO_CHAR;
			BOOL	b2ByteChar = FALSE;
			int iPos;
			for(  iPos = 0; iPos < nSplitPos; iPos++ )
			{
				if( strBuffInfo[iPos] & 0x80 )
					b2ByteChar = !b2ByteChar;
				else
					b2ByteChar = FALSE;
			}

			if( b2ByteChar )
				nSplitPos--;

			// Split string
			CTString	strTemp;
			strBuffInfo.Split( nSplitPos, m_strBuffInfo[m_nCurInfoLines], strTemp );
			m_colBuffInfo[m_nCurInfoLines++] = colBuffInfo;

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

			AddBuffInfoString( strTemp, colBuffInfo );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetBuffInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::GetBuffInfo( int nItemIndex, int nSkillIndex, int nSkillLevel, int &nInfoWidth, int &nInfoHeight )
{
	CTString	strTemp;
	m_nCurInfoLines = 0;

	CSkill	&rSelSkill = _pNetwork->GetSkillData( nSkillIndex );

	// Get buff name
	if( nItemIndex == -1 )
	{
		if( nSkillLevel > 0 )
			strTemp.PrintF( "%s Lv %d", rSelSkill.GetName(), nSkillLevel );
		else
			strTemp.PrintF( "%s", rSelSkill.GetName() );
	}
	else
	{
		const char* szItemName = _pNetwork->GetItemName( nItemIndex );
		//CItemData	&rItemData = _pNetwork->GetItemData( nItemIndex );
		if( nSkillLevel > 0 )
			strTemp.PrintF( "%s Lv %d", szItemName, nSkillLevel );
		else
			strTemp.PrintF( "%s", szItemName );
	}

	AddBuffInfoString( strTemp );
	m_nRemainTimeLine = m_nCurInfoLines;
	AddBuffInfoString( CTString( " " ), 0xCACACAFF );		// Temporary space for remaining time

	const char	*pDesc = rSelSkill.GetToolTip();
	if( pDesc[0] != NULL )
	{
		AddBuffInfoString( CTString( " " ) );
		strTemp.PrintF( "%s", pDesc );
		AddBuffInfoString( strTemp, 0x94B7C6FF );
	}

//	if( g_iCountry == THAILAND ) {
//		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + ( MAX_BUFFINFO_CHAR + 5 ) *
//					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
//	}
//	else {
		nInfoWidth = 27 - _pUIFontTexMgr->GetFontSpacing() + MAX_BUFFINFO_CHAR *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
//	}
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
}

// ----------------------------------------------------------------------------
// Name : ShowBuffInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIBuff::ShowBuffInfo( BOOL bShowInfo, int nWhichBuff, int nSelBuff )
{
	// Hide buff information
	if( !bShowInfo )
	{
		m_bShowBuffInfo = FALSE;
		m_nSelBuffID = -1;
		m_nSelBuffItemIndex = -1;
		m_nSelBuffSkillIndex = -1;
		m_sbSelBuffSkillLevel = -1;
		return;
	}

	BOOL	bUpdateInfo = FALSE;
	int		nItemIndex, nSkillIndex;
	SBYTE	sbLevel;
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	switch( nWhichBuff )
	{
	case MY_BUFF:
		nItemIndex = m_aMyBuff[nSelBuff].GetItemIndex();
		nSkillIndex = m_aMyBuff[nSelBuff].GetSkillIndex();
		sbLevel = m_aMyBuff[nSelBuff].GetLevel();
		break;

	case PARTY_BUFF:
		nItemIndex = m_aPartyBuff[nSelBuff].GetItemIndex();
		nSkillIndex = m_aPartyBuff[nSelBuff].GetSkillIndex();
		sbLevel = m_aPartyBuff[nSelBuff].GetLevel();
		break;

	case TARGET_BUFF:
		nItemIndex = m_aTargetBuff[nSelBuff].GetItemIndex();
		nSkillIndex = m_aTargetBuff[nSelBuff].GetSkillIndex();
		sbLevel = m_aTargetBuff[nSelBuff].GetLevel();
		break;
	};

	m_bShowBuffInfo = TRUE;
	m_nSelBuffID = BUFF_MAX_COUNT * nWhichBuff + nSelBuff;

	// Update skill information
	if( m_nSelBuffItemIndex != nItemIndex ||
		m_nSelBuffSkillIndex != nSkillIndex ||
		m_sbSelBuffSkillLevel != sbLevel )
	{
		bUpdateInfo = TRUE;
		m_nSelBuffItemIndex = nItemIndex;
		m_nSelBuffSkillIndex = nSkillIndex;
		m_sbSelBuffSkillLevel = sbLevel;

		switch( nWhichBuff )
		{
		case MY_BUFF:
			{
				// Bad
				if( nSelBuff >= BUFF_GOOD_COUNT )
				{
					nInfoPosX = m_nMyBadBuffSX - ( nSelBuff - BUFF_GOOD_COUNT ) * BUFFOUT_SIZE_BIG -
								BUFFOUT_SIZE_BIG / 2;
					nInfoPosY = m_nMyBadBuffSY + BUFFOUT_SIZE_BIG;
				}
				// Good
				else
				{
					nInfoPosX = m_nMyGoodBuffSX + (nSelBuff%BUFF_BAD_COUNT) * BUFFOUT_SIZE_BIG +
								BUFFOUT_SIZE_BIG / 2;
					nInfoPosY = m_nMyGoodBuffSY + BUFFOUT_SIZE_BIG * (nSelBuff/BUFF_BAD_COUNT+1);
				}
			}
			break;

		case PARTY_BUFF:
			{
				// Bad
				if( nSelBuff >= BUFF_GOOD_COUNT )
				{
					nInfoPosX = m_rcPartyBuff.Left + ( nSelBuff - BUFF_GOOD_COUNT ) * BUFFOUT_SIZE_SMALL +
								BUFFOUT_SIZE_SMALL / 2;
					if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
						nInfoPosY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL * 3;
					else if( m_ubPartyGoodCount > 0 )
						nInfoPosY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL * 2;
					else
						nInfoPosY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL;
				}
				// Good
				else
				{
					nInfoPosX = m_rcPartyBuff.Left + (nSelBuff%BUFF_BAD_COUNT) * BUFFOUT_SIZE_SMALL +
								BUFFOUT_SIZE_SMALL / 2;
					nInfoPosY = m_rcPartyBuff.Top + BUFFOUT_SIZE_SMALL * (nSelBuff/BUFF_BAD_COUNT+1);
				}
			}
			break;

		case TARGET_BUFF:
			{
				// Bad
				if( nSelBuff >= BUFF_GOOD_COUNT )
				{
					nInfoPosX = m_rcTargetBuff.Left + ( nSelBuff - BUFF_GOOD_COUNT ) * BUFFOUT_SIZE_SMALL +
								BUFFOUT_SIZE_SMALL / 2;
					if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
						nInfoPosY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL * 3;
					else if( m_ubTargetGoodCount > 0 )
						nInfoPosY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL * 2;
					else
						nInfoPosY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL;
				}
				// Good
				else
				{
					nInfoPosX = m_rcTargetBuff.Left + (nSelBuff%BUFF_BAD_COUNT) * BUFFOUT_SIZE_SMALL +
								BUFFOUT_SIZE_SMALL / 2;
					nInfoPosY = m_rcTargetBuff.Top + BUFFOUT_SIZE_SMALL * (nSelBuff/BUFF_BAD_COUNT+1);
				}
			}
			break;
		};
		
		// Get buff information
		GetBuffInfo( nItemIndex, nSkillIndex, sbLevel, nInfoWidth, nInfoHeight );
	}

	// Update buff information box
	if( bUpdateInfo )
	{
		if( nWhichBuff == MY_BUFF )
			nInfoPosX -= nInfoWidth / 2;
		else
			nInfoPosX -= nInfoWidth / 2;

		if( nInfoPosX < _pUIMgr->GetMinI() )
			nInfoPosX = _pUIMgr->GetMinI();
		else if( nInfoPosX + nInfoWidth > _pUIMgr->GetMaxI() )
			nInfoPosX = _pUIMgr->GetMaxI() - nInfoWidth;

		if( nInfoPosY + nInfoHeight > _pUIMgr->GetMaxJ() )
		{
			if( nWhichBuff == MY_BUFF )
				nInfoPosY -= BUFFOUT_SIZE_BIG;
			else
				nInfoPosY -= BUFFOUT_SIZE_SMALL;
			m_rcBuffInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
		}
		else
		{
			m_rcBuffInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessageMyBuff()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIBuff::MouseMessageMyBuff( MSG *pMsg )
{
	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if( pMsg->message == WM_MOUSEMOVE )
	{
		if( nX > m_rcMyGoodBuff.Left && nX < m_rcMyGoodBuff.Right &&
			nY > m_rcMyGoodBuff.Top && nY < m_rcMyGoodBuff.Bottom )
		{
			
			for(int i=0 ; i< MAX_BUFF_ROW ; i++){
				if( nX > m_rcMyGoodBuffRow[i].Left && nX < m_rcMyGoodBuffRow[i].Right &&
					nY > m_rcMyGoodBuffRow[i].Top && nY < m_rcMyGoodBuffRow[i].Bottom )
				{
					int	nSelBuff = ( nX - m_rcMyGoodBuffRow[i].Left ) / BUFFOUT_SIZE_BIG;
					ShowBuffInfo( TRUE, MY_BUFF, nSelBuff + MAX_BUFF_COL*i );
					_pUIMgr->RearrangeOrder(UI_PLAYERINFO,TRUE);
					return WMSG_SUCCESS;
				}
			}
		}

		if( nX > m_rcMyBadBuff.Left && nX < m_rcMyBadBuff.Right &&
			nY > m_rcMyBadBuff.Top && nY < m_rcMyBadBuff.Bottom )
		{
			int	nSelBuff = BUFF_GOOD_COUNT + ( nX - m_rcMyBadBuff.Left ) / BUFFOUT_SIZE_BIG;
			ShowBuffInfo( TRUE, MY_BUFF, nSelBuff );
			_pUIMgr->RearrangeOrder(UI_PLAYERINFO,TRUE);
			return WMSG_SUCCESS;
		}

		ShowBuffInfo( FALSE );
	}
	else if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( nX > m_rcMyGoodBuff.Left && nX < m_rcMyGoodBuff.Right &&
			nY > m_rcMyGoodBuff.Top && nY < m_rcMyGoodBuff.Bottom )
		{
			for(int i=0 ; i< MAX_BUFF_ROW ; i++){
				if( nX > m_rcMyGoodBuffRow[i].Left && nX < m_rcMyGoodBuffRow[i].Right &&
					nY > m_rcMyGoodBuffRow[i].Top && nY < m_rcMyGoodBuffRow[i].Bottom )
				{
				
					return WMSG_SUCCESS;
				}
			}
				
		}

		if( nX > m_rcMyBadBuff.Left && nX < m_rcMyBadBuff.Right &&
			nY > m_rcMyBadBuff.Top && nY < m_rcMyBadBuff.Bottom )
			return WMSG_COMMAND;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessagePartyBuff()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIBuff::MouseMessagePartyBuff( MSG *pMsg )
{
	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if( pMsg->message == WM_MOUSEMOVE )
	{
		if( nX > m_rcPartyBuff.Left && nX < m_rcPartyBuff.Right &&
			nY > m_rcPartyBuff.Top && nY < m_rcPartyBuff.Bottom )
		{
			int	nSelBuff = -1;
			int	nSelCol = ( nX - m_rcPartyBuff.Left ) / BUFFOUT_SIZE_SMALL;
			int	nSelRow = ( nY - m_rcPartyBuff.Top ) / BUFFOUT_SIZE_SMALL;

			if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_BAD_COUNT + nSelCol;
				else if( nSelRow == 2 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else if( m_ubPartyGoodCount > 0 )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else
			{
				if( nSelRow == 0 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}

			if( nSelBuff != -1 && ( nSelBuff < m_ubPartyGoodCount ||
				( nSelBuff >= BUFF_GOOD_COUNT && nSelBuff < BUFF_GOOD_COUNT + m_ubPartyBadCount ) ) )
				ShowBuffInfo( TRUE, PARTY_BUFF, nSelBuff );
			else
				ShowBuffInfo( FALSE );

			return WMSG_SUCCESS;
		}
		else
		{
			_pUIMgr->GetParty()->HidePartyBuff();
			ResetPartyBuff();
			ShowBuffInfo( FALSE );
		}
	}
	else if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( nX > m_rcPartyBuff.Left && nX < m_rcPartyBuff.Right &&
			nY > m_rcPartyBuff.Top && nY < m_rcPartyBuff.Bottom )
		{
			int	nSelBuff = -1;
			int	nSelCol = ( nX - m_rcPartyBuff.Left ) / BUFFOUT_SIZE_SMALL;
			int	nSelRow = ( nY - m_rcPartyBuff.Top ) / BUFFOUT_SIZE_SMALL;

			if( m_ubPartyGoodCount > BUFF_BAD_COUNT )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_BAD_COUNT + nSelCol;
				else if( nSelRow == 2 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else if( m_ubPartyGoodCount > 0 )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else
			{
				if( nSelRow == 0 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}

			if( nSelBuff != -1 && ( nSelBuff < m_ubPartyGoodCount ||
				( nSelBuff >= BUFF_GOOD_COUNT && nSelBuff < BUFF_GOOD_COUNT + m_ubPartyBadCount ) ) )
				return WMSG_COMMAND;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MouseMessageTargetBuff()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIBuff::MouseMessageTargetBuff( MSG *pMsg )
{
	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if( pMsg->message == WM_MOUSEMOVE )
	{
		if( nX > m_rcTargetBuff.Left && nX < m_rcTargetBuff.Right &&
			nY > m_rcTargetBuff.Top && nY < m_rcTargetBuff.Bottom )
		{
			int	nSelBuff = -1;
			int	nSelCol = ( nX - m_rcTargetBuff.Left ) / BUFFOUT_SIZE_SMALL;
			int	nSelRow = ( nY - m_rcTargetBuff.Top ) / BUFFOUT_SIZE_SMALL;

			if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_BAD_COUNT + nSelCol;
				else if( nSelRow > 1 )
					nSelBuff = (MAX_BUFF_COL * nSelRow) + nSelCol;
			}
			else if( m_ubTargetGoodCount > 0 )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else
			{
				if( nSelRow == 0 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}

			if( nSelBuff != -1 && ( nSelBuff < m_ubTargetGoodCount ||
				( nSelBuff >= BUFF_GOOD_COUNT && nSelBuff < BUFF_GOOD_COUNT + m_ubTargetBadCount ) ) )
			{
				ShowBuffInfo( TRUE, TARGET_BUFF, nSelBuff );
				return WMSG_SUCCESS;
			}
		}

		ShowBuffInfo( FALSE );
	}
	else if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( nX > m_rcTargetBuff.Left && nX < m_rcTargetBuff.Right &&
			nY > m_rcTargetBuff.Top && nY < m_rcTargetBuff.Bottom )
		{
			int	nSelBuff = -1;
			int	nSelCol = ( nX - m_rcTargetBuff.Left ) / BUFFOUT_SIZE_SMALL;
			int	nSelRow = ( nY - m_rcTargetBuff.Top ) / BUFFOUT_SIZE_SMALL;

			if( m_ubTargetGoodCount > BUFF_BAD_COUNT )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_BAD_COUNT + nSelCol;
				else if( nSelRow == 2 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else if( m_ubTargetGoodCount > 0 )
			{
				if( nSelRow == 0 )
					nSelBuff = nSelCol;
				else if( nSelRow == 1 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}
			else
			{
				if( nSelRow == 0 )
					nSelBuff = BUFF_GOOD_COUNT + nSelCol;
			}

			if( nSelBuff != -1 && ( nSelBuff < m_ubTargetGoodCount ||
				( nSelBuff >= BUFF_GOOD_COUNT && nSelBuff < BUFF_GOOD_COUNT + m_ubTargetBadCount ) ) )
				return WMSG_COMMAND;
		}
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IsBuff() wooss 050803
// Desc : find some buff in buff array
// ----------------------------------------------------------------------------
BOOL CUIBuff::IsBuff( SLONG itemIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
		if ((m_aMyBuffInfo[i].m_slItemIndex)==itemIndex) return TRUE;
	return FALSE;	
}

// wooss 070310 --------------------------------------------------------------->>
// kw : WSS_WHITEDAY_2007
CEffectGroup* CUIBuff::IsBuffBySkill( SLONG skillIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
	{
		if ((m_aMyBuffInfo[i].m_slSkillIndex)==skillIndex)
		{
			return m_aMyBuffInfo[i].m_pEG;
		}
	}
	return NULL;
}


CEffectGroup* CUIBuff::GetEGFromSkillIndex(SLONG skillIndex)
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
		if ((m_aMyBuffInfo[i].m_slSkillIndex)==skillIndex) return m_aMyBuffInfo[i].m_pEG;
	return NULL;			
}

// ----------------------------------------------------------------------------<<
