
#ifndef _UISIEGE_WARFARE_H
#define _UISIEGE_WARFARE_H

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UISpinButton.h>

class CUISiegeWarfare : public CUIWindow
{
// Control
	// 타이틀 
	CTString				m_strTitle;
	UIRect					m_rcTitle;

	// 공성 시간 설정시 사용
	CUIButton				m_btnOK;				
	CUIButton				m_btnClose;
	CUIButton				m_btnCancel;
	CUICheckButton			m_cbtnSaturday;				// 공성 일 설정
	CUICheckButton			m_cbtnSunday;
	CUISpinButton			m_sbtnSWTime;				// 공성 시간 설정
	CUIListBox				m_lbDesc;					// 공성 가능 시간
		
	// Back UV
	UIRectUV3				m_rt3BackT;								
	UIRectUV3				m_rt3BackSM;							
	UIRectUV3				m_rt3BackNM;
	UIRectUV3				m_rt3BackM;	
	UIRectUV3				m_rt3BackL;	

	// BattleBack UV
	UIRectUV				m_rtBattleBack;
	UIRectUV				m_rtSelectBattleBack;

	// Number UV
	UIRectUV				m_rtNumber[11];
	UIRectUV				m_rtColon;
	
	// Notice 
	UIRect					m_rcNotice1;				// Region of notice1 (위 쪽)
	UIRect					m_rcNotice2;				// Region of notice2 (아랫 쪽)
	
	UIRectUV				m_rtNoticeL;				// UV of notice left region
	UIRectUV				m_rtNoticeC;				// UV of notice center region
	UIRectUV				m_rtNoticeR;				// UV of notice right region
	
	//Notice Info
	TIME					m_tmNoticeTime;				// 공지 사항 표시 시간
	BOOL					m_bShowNotice;				// 공지 사항을 보여주고 있는 가
	CTString				m_strNoticeMessage;			// 공자 사항 1 스트링 (위)
	CTString				m_strNoticeMessage2;		// 공지 사항 2 스트링 (아래)

public:	
	CUISiegeWarfare();
	virtual ~CUISiegeWarfare();
	
	void	Clear();
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );
	
	// Adjust position
	void	ResetPos( BOOL bCenter = TRUE );
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Message
	WMSG_RESULT MouseMessage( MSG *pMsg );

	// Open SiegeWarfare
	void	OpenSiegeWarfare();
	void	OpenSWReq();
	void	OpenMercearReq();
	void	OpenSetSWTime();
	void	OpenSWInfo();
	void	OpenSelectBattle();
	void	OpenBattle();
	
	// Close
	void	CloseSiegeWarfare();	
	
	// Render
	void	RenderNotice();
	void	RenderSetSWTime();
	void	RenderBattle();
	void	RenderPoint( int nX, int nY, int nPoint );
	void	DrawNumber( int nX, int nY, int nNumber, int nWidth = 16 , int nHeight = 19 );
	void	RenderLeftTime();
	
	// MessageBox
	void	CloseAllMsgBox();
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	ErrorMessage( CTString strErrorMessage );					// 에러 에디트 박스
	void	Message( int nCommandCode, CTString strMessage, DWORD dwStyle );	// 메세지 박스
	
	// Etc
	void	SetNotice( CTString strNoticeMessage, CTString strNoticeMessage2 = " " );
	BOOL	m_bBottomNotice;

	void	SetDayOfWeek( int eDayOfWeek );

};




#endif // _UISIEGE_WARFARE_H
