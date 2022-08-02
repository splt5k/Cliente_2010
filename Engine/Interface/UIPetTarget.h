// ----------------------------------------------------------------------------
//  File : UIPetTargetInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIPETTARGETINFO_H_
#define	UIPETTARGETINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIDrawFigure.h>


// Define position
#define	PETTARGETINFO_NAME_SY				5


// Define size of player information
#define	PETTARGETINFO_WIDTH				140
#define	PETTARGETINFO_HEIGHT			56
#define	PETTARGETINFO_BAR_WIDTH			120

// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIPetTargetInfo : public CUIWindow
{
protected:
	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcHP;							// HP
	UIRect				m_rcHPBack;						// Background of HP
	UIRect				m_rcHungry;						// Hungry
	UIRect				m_rcHungryBack;					// Background of Hungry
	UIRect				m_rcInfoMark;	
	

	// UV of each part
	UIRectUV			m_rtBackground1;					// Background	
	UIRectUV			m_rtBackground2;					// Background	
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtHPBack;						// Background of HP	
	UIRectUV			m_rtHungry;						// Hungry
	UIRectUV			m_rtHungryBack;					// Background of Hungry
	CUIButton			m_btnPetInfo;
	
	// TOOLTIP UI
	BOOL				m_bShowTooltip;
	UIRectUV			m_rtToolTipL;
	UIRectUV			m_rtToolTipM;
	UIRectUV			m_rtToolTipR;
	CTString			m_strToolTip;
	
	
protected:
	// Update info of target
	void	UpdateHPInfo();
	void	UpdateHungryInfo();

public:
	CUIPetTargetInfo();
	~CUIPetTargetInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();	
	void    RenderTooltip();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

class CUIWildPetTargetInfo : public CUIWindow
{
protected:
	CTextureData		*m_ptdButtonTexture; 
	UIRect				m_rcTitle;						// Title
	CUIDrawBox			m_bxTitle;
	CUIDrawBox			m_bxTooltip;
	UIRectUV			m_rtHP;							// HP
	UIRectUV			m_rtMP;							// MP
	UIRectUV			m_rtEXP;						// EXP

	UIRect				m_rcHP;							// HP
	UIRect				m_rcMP;							// MP
	UIRect				m_rcEXP;						// EXP
	CUIButton			m_btnPetInfo;

	BOOL				m_bShowTooltip;
	CTString			m_strToolTip;
protected:
	void UpdateState();

public:
	CUIWildPetTargetInfo();
	~CUIWildPetTargetInfo();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	// Render
	void	Render();	
	void    RenderTooltip();

	void	UpdateHPInfo();
	void	UpdateMPInfo();
	void	UpdateEXPInfo();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
};


#endif	// UIPETTARGETINFO_H_

