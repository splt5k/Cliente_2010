#include "stdh.h"
#include <Engine/Network/ChatMsgBuffer.h>
#include <Engine/Interface/UITextureManager.h>

// ----------------------------------------------------------------------------
// Name : ChatMsgBuffer()
// Desc : Constructor
// ----------------------------------------------------------------------------
ChatMsgBuffer::ChatMsgBuffer()
{
	m_ubMsgCount = 0;
}

// ----------------------------------------------------------------------------
// Name : ChatMsgBuffer()
// Desc : Destructor
// ----------------------------------------------------------------------------
ChatMsgBuffer::~ChatMsgBuffer()
{
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void ChatMsgBuffer::AddString( CTString &strMessage )
{
	if( m_ubMsgCount >= CHATMSG_LINE_MAX )
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = CHATMSG_CHAR_MAX - 4;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( m_strMsg[CHATMSG_LINE_MAX - 1][iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Trim right
		m_strMsg[CHATMSG_LINE_MAX - 1].TrimRight( nSplitPos );
		m_strMsg[CHATMSG_LINE_MAX - 1] += CTString( "..." );

		return;
	}

	// Get length of string
	INDEX	nLength = strMessage.Length();
	if( nLength == 0 )
		return;

	// If length of string is less than max char
	if( nLength <= CHATMSG_CHAR_MAX )
	{
		m_strMsg[m_ubMsgCount++] = strMessage;

		if( m_nWidth == 0 )
			m_nWidth = nLength;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = CHATMSG_CHAR_MAX;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strMessage[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Split string
		CTString	strTemp;
		strMessage.Split( nSplitPos, m_strMsg[m_ubMsgCount++], strTemp );

		m_nWidth = CHATMSG_CHAR_MAX;
		AddString( strTemp );
	}
}

// ----------------------------------------------------------------------------
// Name : SetChatMsg()
// Desc :
// ----------------------------------------------------------------------------
void ChatMsgBuffer::SetChatMsg( CTString &strMessage, COLOR colMessage )
{
	m_ubMsgCount = 0;
	m_colMsg = colMessage;
	m_llMsgTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	m_nWidth = 0;
	AddString( strMessage );

	int	nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	m_nWidth = m_nWidth * nFontWidth + 1;
}