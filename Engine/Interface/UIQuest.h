// ----------------------------------------------------------------------------
//  File : UIQuest.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUEST_H_
#define	UIQUEST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <vector>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIButtonEx.h>

#define	QUEST_TAB_WIDTH					96

// Define remission slot
#define	QUEST_SLOT_SX					18
#define	QUEST_SLOT_SY					56
#define	QUEST_SLOT_OFFSETY				37

#define	QUESTLIST_SLOT_ROW				4
#define	QUESTLIST_SLOT_ROW_TOTAL		30

// Define text
#define	QUEST_NAME_CX					122
#define	QUEST_NAME_SY					58
#define	QUEST_NEED_RX					184
#define	QUEST_CURSP_SX					97
#define	QUEST_CURSP_RX					199
#define	QUEST_CURSP_SY					343
#define	QUEST_TAB_CX					108
#define	QUEST_TAB_SY					34
#define	MAX_QUEST_DESC_CHAR				28

// Define text position
#define	QUEST_TITLE_TEXT_OFFSETX		25
#define	QUEST_TITLE_TEXT_OFFSETY		5

#define	QUEST_TOP_HEIGHT				339
#define	QUEST_BOTTOM_HEIGHT				41

// Define size of remission learn
#define	QUEST_WIDTH						216
#define	QUEST_HEIGHT					380

// Define max char and line of action informaion 
#define	QUESTINFO_CHAR_WIDTH			170
#define	MAX_QUESTINFO_LINE				10

// 월드컵이벤트
#define WORLDCUP_MAX_COUNTRY 32

// 2009. 05. 27 김정래
// 퀘스트의 번호 구분자
const int ciQuestClassifier			=	1000;

enum eSelectionQuest
{
	QUEST_TALK,
	QUEST_EVENT,
	QUEST_TREASURE_BOX,				// 보물 상자.
	QUEST_CHANGEWEAPON,
	QUEST_KILL_BOSS,				// 발록 격파
	QUEST_SAVE_PRINCESS,			// 공주 구출
	QUEST_AZAKA_RAVINE,				// 아자카 협곡
	QUEST_GATE_OF_DIMENSION,		// 차원의 문
	QUEST_HARVEST_MOON_DAY_EVENT,	// 추석이벤트
	QUEST_HARVEST_MOON_DAY_EVENT1,	// 2006 추석이벤트(송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT2,	// 2006 추석이벤트(오색송편 보상품과 교환하기)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE1,	// 2006 추석이벤트(송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE2,	// 2006 추석이벤트(오색송편 만들기)
	QUEST_HARVEST_MOON_DAY_EVENT_GIVE_ITEM,	// 보상품을 지급받는다.
	QUEST_RENUAL_EVENT,
	QUEST_RENUAL_EVENT_OK,
	QUEST_EXCHANGE_MONEY,			// 랜디 이판사판 머니 교환 wooss 051031
	QUEST_OPEN_TREASURE,			// 랜디 이판사판 보물상자 열기
	QUEST_CHARACTER_CARD,			// 
	EVENT_NEWYEAR1,					// 2006 꿈과 희망 이벤트
	EVENT_NEWYEAR2,					// 인내의 열매 이벤트 
	EVENT_FIND_FRIEND,
	EVENT_COUPON ,					// 일본 쿠폰 이벤트 
	EVENT_CONNECT,					// 일본 접속자 이벤트	

	EVENT_CLOTHES_EXCHANGE,			// 일본 전통 의상 교환
	EVENT_OX_GOZONE,				// O.X이벤트 존 입장

	PLATINUM_EXCHANGE_STONE,        // 플래티늄 제련석 교환
	WIN_SELECT,						// 우승국 알아 맞추기
	EVENT_GOLDENBALL,				// 골든 볼 이벤트	
	EVENT_OPENBETAITEMGIFT,			// 미국 이벤트 무기 지급
	EVENT_NETCAFE_BOX,				// net cafe 캠페인
	EVENT_RAINYDAY,
	EVENT_BUDDHISM,					// 불교 촛불 축제 이벤트( 태국 )
	EVENT_COLLECTBUGS,				// 여름 곤충 채집 이벤트
	EVENT_PROMOPACK,				// 프로모 패키지 이벤트 ( 말레샤 )
	EVENT_PROMO_KEY_ENTER,			// 프로모 패키지 인증키 입력
	
	EVENT_XMAS_2006,				// 2006 X-MAS Event [12/11/2006 Theodoric]
	EVENT_LOVE_LOVE,				// 2007발렌타인's 데이(러브러브 이벤트)
	EVENT_WHITEDAY_2007,			// 2007 WhiteDay Event
	NPC_HELP,						// NPC 안내 시스템
	
	EVENT_MAY,						// WSS_GUILD_MASTER 070411 -->><<
	EVENT_MINIGAME,					// WSS_MINIGAE 070420 곰돌이 이벤트
	EVENT_FLOWERS_SUPPORT,			// 꽃놀이 이벤트 개화 돕기
	EVENT_SUPPORT_EXCHANGE,			// 꽃놀이 이벤트 기여도와 교환권
	EVENT_SUMMER_2007,				// [070705: Su-won] EVENT_SUMMER_2007
	SIEGEWARFARE_MASTERTOWER1,		// WSS_DRATAN_SEIGEWARFARE 2007/07/30 // 가동하기
	SIEGEWARFARE_MASTERTOWER2,		// 타워 강화하기 
	SIEGEWARFARE_MASTERTOWER3,		// 성문 강화하기
	SIEGEWARFARE_MASTERTOWER4,		// 마스터 타워 수리하기
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// [070708: Su-won] EVENT_ADULT_OPEN
	EVENT_ADULT_MAGICCARD,					// 성인서버 오픈 이벤트(매직카드를 찾아라!)
	EVENT_ADULT_CHANGEJOB,					// 성인서버 오픈 이벤트(전직달성!절망의부적을 받아라!)
	EVENT_ADULT_ALCHEMIST,					// 성인서버 오픈 이벤트(장비 조합 연금술 이벤트)
	EVENT_SHOOT,							// 성인서버 오픈 이벤트(성인들만의 특권)
	// [070708: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////

	EVENT_BJMONO_2007_REQ,					// SK BJ MONO 응모권 받기
	EVENT_BJMONO_2007_CHANGE_REQ,			// SK BJ MONO 응모권 교환
	
	EVENT_TG2007_1000DAYS_CAP,				// 라스트카오스 1000일 기념모자 받기
	EVENT_TG2007_1000DAYS_CAP_UPGRADE,		// 기념모자 업그레이드
	EVENT_TG2007_1000DAYS_FIRECRACKER,		// 폭죽 받기 이벤트
	EVENT_TG2007_FRUITFULL,					// 풍년이벤트  	
	EVENT_TG2007_SCREENSHOT,				// 스크린샷 이벤트
	
	EVENT_HALLOWEEN2007_DEVILHAIR,			// 악마날개 머리띠 지급 받기
	EVENT_HALLOWEEN2007_CANDYBASKET,		// 사탕바구니에 사탕 받기
	EVENT_HALLOWEEN2007_PUMKINHEAD,			// 호박머리 탈 교환 받기
	EVENT_HALLOWEEN2007_WITCHHAT,			// 마녀모자 교환 받기
	EVENT_HALLOWEEN2007_PUMKIN,				// 호박교환 받기
	
	// 071129_ttos: 판매 대행협회 회장
	CASH_PERSONSHOP_CLOSE,					// 아이템 판매 대행 종료
	CASH_PERSONSHOP_ITEM,					// 보관된 물품 회수
	CASH_PERSONSHOP_NAS,					// 보관된 나스 회수

	EVENT_XMAS2007_DECO,					// 크리스마스트리 장식하기
	EVENT_XMAS2007_BLESSEDNESS,				// 크리스마스 축복 받기

	EVENT_RED_TREASUREBOX,					//붉은색 보물상자 이벤트

	EVENT_SAKURA_2008,						//2008년 벚꽃 이벤트

	EVENT_CHAOSBALL,						//카오스볼 이벤트

	EVENT_PHOENIX_REQ,							//피닉스 이벤트

	USE_AUCTION,
	
	EVENT_HANAPOS_SK_EVENT,					//[ttos_2009_3_18]: 하나포스 및 SK브로드밴드 가입자 이벤트
};

enum eEvent
{
	EVENT_NEWYEAR1_REWARD,
	EVENT_NEWYEAR2_TIMECHECK,
	EVENT_NEWYEAR2_REWARD,

	EVENT_FF_REG,
	EVENT_FF_ISREWARD,
	EVENT_FF_TIMECHECK,
	EVENT_FF_REWARD,

	EVENT_REWARD_1,
	EVENT_REWARD_2,
	EVENT_REWARD_3,

	EVENT_MAY_CHILDREN,
	EVENT_MAY_PARENTS,
	EVENT_MAY_MASTER,

};

// ------------------------------------------------------------<<

// ----------------------------------------------------------------------------
// Name : CUIQuest
// Desc :
// ----------------------------------------------------------------------------
class CUIQuest : public CUIWindow
{
protected:
	// 서버.
	struct sQuestList
	{		
		INDEX	iQuestIndex;
		SBYTE	sbQuestFlag;
		
		bool operator<(const sQuestList &other) const
		{
			if( sbQuestFlag > other.sbQuestFlag )
				return true;
			return false;
		}
		bool operator>(const sQuestList &other) const
		{			
			return other.operator < (*this);
		}
	};
	
	// Controls
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// Learn button
	CUIButton				m_btnCancel;							// Cancel button
	CUIScrollBar			m_sbQuestIcon;							// Scrollbar of special remission icon
	CUIListBox				m_lbQuestDesc;							// List box of remission description

	BOOL					m_bShowQuestInfo;						// If skill tool tip is shown or not	
	UIRect					m_rcQuestInfo;							// Action information region
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	int						m_nCurQuestInfoLines;					// Count of current Quest information lines
	CTString				m_strQuestInfo[MAX_QUESTINFO_LINE];		// Quest information string
	COLOR					m_colQuestInfo[MAX_QUESTINFO_LINE];		// Color of Quest information string

	// Quest buttons
	std::vector<CUIButtonEx>	m_vectorbtnQuests;					// Buttons of special remission

	// Quest information
	int						m_nSelQuestID;							// Selected special remission ID
	CTString				m_strShortDesc;							// Short remission information string

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons

	// UV of each part
	UIRectUV				m_rtBackgroundTop;						// UV of background top
	UIRectUV				m_rtBackgroundBottom;					// UV of background bottom
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button

	// FIXME : 
	int						m_iNpcIndex;
	int						m_iCurQuestIndex;
	BOOL					m_bLockQuest;

	// Quest List
	std::vector<sQuestList>		m_vectorQuestList;

	// Event save data
	CTString				m_strTeamA, m_strTeamB;			// 골든 볼 이벤트 용
	int						m_ScoreTeamA, m_ScoreTeamB;		
	SYSTEMTIME				m_GoldenEndTime;
	BOOL					m_bGoldenBallEntry;				// 응모 가능
	BOOL					m_bGoldenBallRequital;			// 보상 가능
	BOOL					m_bStartGoldenBall;
	CTString				m_strCountry[WORLDCUP_MAX_COUNTRY];
	
protected:
	// Internal functions	
	void	RenderQuestBtns();
	void	GetQuestDesc( BOOL bFullDesc, int nQuestIndex, SBYTE sbFlag );
	void	AddQuestDescString( CTString &strDesc, const COLOR colDesc );
	void	PressOK( );

	void	AddQuestInfoString( CTString &strQuestInfo, COLOR colQuestInfo = 0xF2F2F2FF );
	void	GetQuestInfo( int nQuestIndex, SBYTE sbQuestFlag, int &nInfoWidth, int &nInfoHeight );
	void	ShowQuestInfo( BOOL bShowInfo, CUIButtonEx *pQuestBtn = NULL );

	// Network message functions ( send )
	void	SendQuest();
	
public:
	CUIQuest();
	~CUIQuest();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open remission learn
	ENGINE_API void	OpenQuest( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	ENGINE_API void	OpenQuest( int iMobIndex, FLOAT fX, FLOAT fZ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )	
	void	QuestError( UBYTE ubError );

	// Init & Close
	BOOL	InitQuest( );
	void	CloseQuest();

	// Add & Clear Quest List
	void	AddToQuestList(INDEX iQuestIndex, SBYTE sbQuestFlag);
	void	ClearQuestList();

	void	LockQuest(BOOL bLock);
	void	SetCurQuest(int iQuestIndex) { m_iCurQuestIndex = iQuestIndex; }
	
	// Get npc_pos 
	// 생성된 창을 지워주기 위해 UIManager에서 처리한다 
	int 	GetNpcPosX(){ return m_fNpcX;}
	int		GetNpcPosZ(){ return m_fNpcZ;}
	
	int		GetNpcIndex(){ return m_iNpcIndex; }
	
	// event data function
	//------------- 골든볼 이벤트 용-----------//
	void	SetStrTeamName( CTString strTeamA, CTString strTeamB, SYSTEMTIME GoldenEndTime, BOOL bFlag )
	{ 
		m_strTeamA = strTeamA; m_strTeamB = strTeamB;
		if( bFlag ) m_GoldenEndTime = GoldenEndTime;
	}

	void	SetTeamScore( int ScoreA, int ScoreB ) { m_ScoreTeamA = ScoreA; m_ScoreTeamB = ScoreB; }
	void	SetGoldenBallEntry( BOOL bType ) {
		m_bGoldenBallEntry = bType;
		if( bType == TRUE ) m_bStartGoldenBall = TRUE;
	}
	void	SetGoldenBallRequital( BOOL bType ) {
		m_bGoldenBallRequital = bType;
		if( bType == FALSE ) m_bStartGoldenBall = FALSE;
	}

	CTString	GetStrTeamA( void ) { return m_strTeamA; }
	CTString	GetStrTeamB( void ) { return m_strTeamB; }

	int			GetScoreTeamA( void ) { return m_ScoreTeamA; }
	int			GetScoreTeamB( void ) { return m_ScoreTeamB; }

	SYSTEMTIME	GetGoldenTime( void ) { return m_GoldenEndTime; }
	//-----------------------------------------//

};


#endif	// UIQUEST_H_
