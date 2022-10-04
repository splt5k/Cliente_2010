/*
 *  Serious Sam
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */

#include "StdH.h"
#include "MainWindow.h"
#include "resource.h"
#include "CmdLine.h"
#include "Engine/GameState.h"
#include <Engine/Interface/UIManager.h>

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Graphics/ViewPort.h>
#include <Engine/LocalDefine.h>

extern PIX _pixDesktopWidth;
extern PIX _pixDesktopHeight;


extern PIX _pixDesktopWidth;
extern PIX _pixDesktopHeight;

extern ENGINE_API BOOL	_bWindowChanging;    // ignores window messages while this is set	// yjpark
extern ENGINE_API HWND	_hwndMain;

static char achWindowTitle[256]; // current window title

static HBITMAP _hbmSplash = NULL;
static BITMAP  _bmSplash;

// for window reposition function
static PIX _pixLastSizeI, _pixLastSizeJ;

BOOL _bIMEProc = false; // 이기환 수정 11.12

//	±è¿µÈ¯:
bool	_bWinSize = false;

// window procedure active while window changes are occuring
long FAR PASCAL WindowProc_WindowChanging( HWND hWnd, UINT message, 
			    WPARAM wParam, LPARAM lParam )
{
	switch( message ) {
	case WM_POWERBROADCAST:
		{
//			delete _pTimer;
//			_pTimer = new CTimer;
			if ( wParam == PBT_APMQUERYSUSPEND )
				return BROADCAST_QUERY_DENY;
			else if ( wParam == PBT_APMBATTERYLOW )
			{
				MessageBox(NULL,"Battery power is low.","Warning!",MB_OK);
			}
			return true;
		} break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps); 
		EndPaint(hWnd, &ps); 

		return 0;
	} break;
	case WM_ERASEBKGND: {

		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps); 
		RECT rect;
		GetClientRect(hWnd, &rect); 
		FillRect(ps.hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-10)
		//HDC hdcMem = CreateCompatibleDC(ps.hdc); 
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-10)
		//SelectObject(hdcMem, _hbmSplash); 
		//BitBlt(ps.hdc, 0, 0, _bmSplash.bmWidth, _bmSplash.bmHeight, hdcMem, 0, 0, SRCCOPY); 
//		StretchBlt(ps.hdc, 0, 0, rect.right, rect.bottom,
//		hdcMem, 0,0, _bmSplash.bmWidth, _bmSplash.bmHeight, SRCCOPY); 

		EndPaint(hWnd, &ps); 

		return 1; 
	} break;
	case WM_CLOSE:
		return 0;
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// window procedure active normally
long FAR PASCAL WindowProc_Normal( HWND hWnd, UINT message, 
			    WPARAM wParam, LPARAM lParam )
{
	switch (message) {

	case WM_POWERBROADCAST:
	{
		//			delete _pTimer;
		//			_pTimer = new CTimer;
		if (wParam == PBT_APMQUERYSUSPEND)
			return BROADCAST_QUERY_DENY;
		else if (wParam == PBT_APMBATTERYLOW)
		{
			MessageBox(NULL, "Battery power is low", "Warning!", MB_OK);
		}
		return true;
	} break;
	// system commands
	case WM_SYSCOMMAND: {
		switch (wParam & ~0x0F) {
			// window resizing messages
		case SC_MINIMIZE:
		case SC_RESTORE:
		case SC_MAXIMIZE:
			// relay to application
			PostMessage(NULL, message, wParam & ~0x0F, lParam);
			// do not allow automatic resizing
			return 0;
			break;
			// prevent screen saver and monitor power down
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
	} break;
		// when close box is clicked
	case WM_CLOSE:
		// relay to application
		PostMessage(NULL, message, wParam, lParam);
		// do not pass to default wndproc
		return 0;

		// some standard focus loose/gain messages
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_CANCELMODE:
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	case WM_ACTIVATEAPP:
	case WM_SETCURSOR:
	case WM_MOUSEMOVE:
	case WM_INPUTLANGCHANGE:
	case WM_INPUTLANGCHANGEREQUEST:
		// relay to application
		PostMessage(NULL, message, wParam, lParam);
		// pass to default wndproc
		break;
	case WM_EXITSIZEMOVE:
	{
		if ((sam_bFullScreenActive != 1) && (_bWinSize))
		{
			//	À©µµ¿ì ¸ðµå¿¡¼­ Å©±â º¯°æÀÌ ¹ß»ýÇÑnks °æ¿ì¿¡¸¸,
			_bWinSize = false;

			RECT	t_Rect;
			GetClientRect(_hwndMain, &t_Rect);
			long t_Width = t_Rect.right - t_Rect.left;
			long t_Height = t_Rect.bottom - t_Rect.top;

			sam_iScreenSizeI = t_Width;
			sam_iScreenSizeJ = t_Height;

			// mark to start ignoring window size/position messages until settled down
			_bWindowChanging = TRUE;

			// destroy canvas if existing
			_pUIMgr->GetGame()->DisableLoadingHook();
			if (_pvpViewPortMain != NULL)
			{
				_pGfx->DestroyWindowCanvas(_pvpViewPortMain);
				_pvpViewPortMain = NULL;
				_pdpNormalMain = NULL;
			}

			// create canvas
			ASSERT(_pvpViewPortMain == NULL);
			ASSERT(_pdpNormalMain == NULL);
			_pGfx->CreateWindowCanvas(_hwndMain, &_pvpViewPortMain, &_pdpNormalMain);

			// erase context of both buffers (for the sake of wide-screen)
			_pdpMain = _pdpNormalMain;
			if (_pdpMain != NULL && _pdpMain->Lock()) {
				_pdpMain->Fill(C_BLACK | CT_OPAQUE);
				_pdpMain->Unlock();
				_pvpViewPortMain->SwapBuffers();
				_pdpMain->Lock();
				_pdpMain->Fill(C_BLACK | CT_OPAQUE);
				_pdpMain->Unlock();
				_pvpViewPortMain->SwapBuffers();
			}

			// lets try some wide screen screaming :)
			const PIX pixYBegAdj = _pdpMain->GetHeight() * 21 / 24;
			const PIX pixYEndAdj = _pdpMain->GetHeight() * 3 / 24;
			const PIX pixXEnd = _pdpMain->GetWidth();

			// initial screen fill and swap, just to get context running
			BOOL bSuccess = FALSE;
			if (_pdpMain != NULL && _pdpMain->Lock())
			{
				//pdp->Fill( LCDGetColor( C_dGREEN|CT_OPAQUE, "bcg fill"));
				_pdpMain->Fill(C_BLACK | CT_OPAQUE);
				_pdpMain->Unlock();
				_pvpViewPortMain->SwapBuffers();
				bSuccess = TRUE;
			}

			_pUIMgr->GetGame()->EnableLoadingHook(_pdpMain);

			// if the mode is not working, or is not accelerated
			if (!bSuccess || !_pGfx->IsCurrentModeAccelerated())
			{ // report error
#ifdef _DEBUG
				CPrintF(TRANS("This mode does not support hardware acceleration.\n"));
#endif
				// destroy canvas if existing
				if (_pvpViewPortMain != NULL) {
					_pUIMgr->GetGame()->DisableLoadingHook();
					_pGfx->DestroyWindowCanvas(_pvpViewPortMain);
					_pvpViewPortMain = NULL;
					_pdpNormalMain = NULL;
				}
				// close the application window
				CloseMainWindow();
				// report failure
				return FALSE;
			}

	
			// Adjust position of UIs
			_pUIMgr->AdjustUIPos(_pdpMain);

			// Date : 2005-09-22(오후 8:26:30), By Lee Ki-hwan
			// 타이틀 바 재 설졍 
			_pUIMgr->SetTitleName(sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ);
		}
		break;
	}
	case WM_SIZE:
		{
			//	±è¿µÈ¯ : À©µµ¿ì Ã¢ Å©±â º¯°æÀÌ ¹ß»ýÇÑ °æ¿ì º¯¼ö ¼³Á¤.
			_bWinSize = true;
			break;
		}
	case WM_GETMINMAXINFO:
	{
		(reinterpret_cast<MINMAXINFO*>(lParam))->ptMaxTrackSize.x = 2200;
		(reinterpret_cast<MINMAXINFO*>(lParam))->ptMaxTrackSize.y = 1500;
	}
	break;
	}

	// if we get to here, we pass the message to default procedure
	return _bIMEProc?0:DefWindowProc(hWnd, message, wParam, lParam);  // 이기환 수정 11.12
}

// main window procedure
long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
			    WPARAM wParam, LPARAM lParam )
{
	/*if (message == WM_SETCURSOR) {
		message = message; 
		return 1;
	}*/

	// dispatch to proper window procedure
	if(_bWindowChanging) {
		return WindowProc_WindowChanging(hWnd, message, wParam, lParam);
	} else {
		return WindowProc_Normal(hWnd, message, wParam, lParam);
	}
}


// init/end main window management
void MainWindow_Init(void)
{
	// register the window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstanceMain;
	wc.hIcon = LoadIcon( _hInstanceMain, (LPCTSTR)IDR_MAINFRAME );
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = APPLICATION_NAME;
	wc.lpszClassName = APPLICATION_NAME;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	// load bitmaps
	_hbmSplash = LoadBitmap(_hInstanceMain, (char*)IDB_SPLASH);
	ASSERT(_hbmSplash!=NULL);
	GetObject(_hbmSplash, sizeof(BITMAP), (LPSTR) &_bmSplash); 
	// here was loading and setting of no-windows-mouse-cursor
}


void MainWindow_End(void)
{
	DeleteObject(_hbmSplash);
}


// close the main application window
void CloseMainWindow(void)
{
	// if window exists
	if( _hwndMain!=NULL) {
		// destroy it
		DestroyWindow(_hwndMain);
		_hwndMain = NULL;
	}
}


void ResetMainWindowNormal(void)
{
	ShowWindow( _hwndMain, SW_HIDE);
	// add edges and title bar to window size so client area would have size that we requested
	RECT rWindow, rClient;
	GetClientRect( _hwndMain, &rClient);
	GetWindowRect( _hwndMain, &rWindow);
	const PIX pixWidth  = _pixLastSizeI + (rWindow.right-rWindow.left) - (rClient.right-rClient.left);
	const PIX pixHeight = _pixLastSizeJ + (rWindow.bottom-rWindow.top) - (rClient.bottom-rClient.top);
	int pixPosX,pixPosY;
	if (cmd_iWindowLeft > 0) {    
		pixPosX = cmd_iWindowLeft>(::GetSystemMetrics(SM_CXSCREEN))?(::GetSystemMetrics(SM_CXSCREEN)):cmd_iWindowLeft;
		cmd_iWindowLeft = -1;
	} else {
		pixPosX   = (::GetSystemMetrics(SM_CXSCREEN) - pixWidth ) /2;
	}
	if (cmd_iWindowTop > 0) {
		pixPosY = cmd_iWindowTop>(::GetSystemMetrics(SM_CYSCREEN))?(::GetSystemMetrics(SM_CYSCREEN)):cmd_iWindowTop;
		cmd_iWindowTop = -1;
	} else {
		pixPosY   = (::GetSystemMetrics(SM_CYSCREEN) - pixHeight ) /2;
	}
	// set new window size and show it
	SetWindowPos( _hwndMain, NULL, pixPosX,pixPosY, pixWidth,pixHeight, SWP_NOZORDER);
	ShowWindow(   _hwndMain, SW_SHOW);
}


// open the main application window for windowed mode
void OpenMainWindowNormal( PIX pixSizeI, PIX pixSizeJ)
{
	ASSERT(_hwndMain==NULL);

	// create a window, invisible initially
	_hwndMain = CreateWindowEx(
	  WS_EX_APPWINDOW,
	  APPLICATION_NAME,
	  "",   // title
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_SIZEBOX,
	  10,10,
	  100,100,  // window size
	  NULL,
	  NULL,
	  _hInstanceMain,
	  NULL);
	// didn't make it?
	if( _hwndMain==NULL) FatalError(TRANS("Cannot open main window!"));
	SE_UpdateWindowHandle( _hwndMain);

	sprintf( achWindowTitle, TRANS("Loading...") );
	
	SetWindowText( _hwndMain, achWindowTitle);
	_pixLastSizeI = pixSizeI;
	_pixLastSizeJ = pixSizeJ;
	ResetMainWindowNormal();
}


// open the main application window for fullscreen mode
void OpenMainWindowFullScreen( PIX pixSizeI, PIX pixSizeJ)
{
	ASSERT( _hwndMain==NULL);
	// create a window, invisible initially
	_hwndMain = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_APPWINDOW,
		APPLICATION_NAME,
		"",   // title
		WS_POPUP,
		0,0,
		pixSizeI, pixSizeJ,  // window size
		NULL,
		NULL,
		_hInstanceMain,
		NULL);
	// didn't make it?
	if( _hwndMain==NULL) FatalError(TRANS("Cannot open main window!"));
	SE_UpdateWindowHandle( _hwndMain);

	sprintf( achWindowTitle, TRANS("Loading...") );

	SetWindowText( _hwndMain, achWindowTitle);
	ShowWindow(    _hwndMain, SW_SHOWNORMAL);
}


// open the main application window invisible
void OpenMainWindowInvisible(void)
{
	ASSERT(_hwndMain==NULL);
	// create a window, invisible initially
	_hwndMain = CreateWindowEx(
	  WS_EX_APPWINDOW,
	  APPLICATION_NAME,
	  "",   // title
		WS_POPUP,
	  0,0,
	  10, 10,  // window size
	  NULL,
	  NULL,
	  _hInstanceMain,
	  NULL);
	// didn't make it?
	if( _hwndMain==NULL) FatalError(TRANS("Cannot open main window!"));
	SE_UpdateWindowHandle( _hwndMain);

	sprintf( achWindowTitle, TRANS("Loading...") );

	SetWindowText( _hwndMain, achWindowTitle);
}
