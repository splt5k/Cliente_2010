#include "stdh.h"

#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Network/Compression.h>

#include <Engine/Math/Functions.h>
#include <Engine/Base/CRC.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/CTString.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ObjectRestore.h>

#include <Engine/Base/ListIterator.inl>
#include <Engine/Templates/ReusableContainer.cpp>
#include <Engine/Base/MemoryTracking.h>

// array of stream blocks to reduce allocation and dealocations
CReusableContainer<CNetworkStreamBlock> _rcStreamBlockContainer(4);

static struct ErrorCode ErrorCodes[] = {
// message types
//0522 kwon 삭제.
//ERRORCODE(MSG_REQ_ENUMSERVERS, "MSG_REQ_ENUMSERVERS"),
//ERRORCODE(MSG_SERVERINFO, "MSG_SERVERINFO"),
//ERRORCODE(MSG_INF_DISCONNECTED, "MSG_INF_DISCONNECTED"),
  ERRORCODE(MSG_REQ_CONNECTLOCALSESSIONSTATE, "MSG_REQ_CONNECTLOCALSESSIONSTATE"),
  ERRORCODE(MSG_REP_CONNECTLOCALSESSIONSTATE, "MSG_REP_CONNECTLOCALSESSIONSTATE"),
//ERRORCODE(MSG_REQ_CONNECTREMOTESESSIONSTATE, "MSG_REQ_CONNECTREMOTESESSIONSTATE"),
//ERRORCODE(MSG_REP_CONNECTREMOTESESSIONSTATE, "MSG_REP_CONNECTREMOTESESSIONSTATE"),
//ERRORCODE(MSG_REQ_GAMESTATE, "MSG_REQ_GAMESTATE"),
//ERRORCODE(MSG_REQ_GAMESTATE, "MSG_REP_GAMESTATE"),
  ERRORCODE(MSG_REQ_CONNECTPLAYER, "MSG_REQ_CONNECTPLAYER"),
  ERRORCODE(MSG_REP_CONNECTPLAYER, "MSG_REP_CONNECTPLAYER"),
  ERRORCODE(MSG_ACTIONS, "MSG_ACTIONS"),
//ERRORCODE(MSG_ACTIONPREDICT, "MSG_ACTIONPREDICT"),
  ERRORCODE(MSG_SEQ_ALLACTIONS, "MSG_SEQ_ALLACTIONS"),   
//ERRORCODE(MSG_SEQ_ADDPLAYER, "MSG_SEQ_ADDPLAYER"),    
//ERRORCODE(MSG_SEQ_REMPLAYER, "MSG_SEQ_REMPLAYER"),    
  ERRORCODE(MSG_GAMESTREAMBLOCKS, "MSG_GAMESTREAMBLOCKS"), 
  ERRORCODE(MSG_REQUESTGAMESTREAMRESEND, "MSG_REQUESTGAMESTREAMRESEND"),
};
extern struct ErrorTable MessageTypes = ERRORTABLE(ErrorCodes);

/////////////////////////////////////////////////////////////////////
// CNetworkMessage

/*
 * Constructor for empty message (for receiving).
 */
CNetworkMessage::CNetworkMessage(void)
{
  nm_slMaxSize = MAX_NETWORKMESSAGE_SIZE;
  // Allocate memory required by network message
  nm_pubMessage = (UBYTE*) AllocMemory(nm_slMaxSize);
  // Initialize network message
  Initialize();
}

/*
 * Constructor for initializing message that is to be sent.
 */
CNetworkMessage::CNetworkMessage(UBYTE mtType)
{
  nm_slMaxSize = MAX_NETWORKMESSAGE_SIZE;
  // Allocate memory required by network message
  nm_pubMessage = (UBYTE*) AllocMemory(nm_slMaxSize);
  Initialize(mtType);
}

/* Copying. */
CNetworkMessage::CNetworkMessage(const CNetworkMessage &nmOriginal)
{
  // allocate message buffer
  nm_slMaxSize = nmOriginal.nm_slMaxSize;
  nm_pubMessage = (UBYTE*) AllocMemory(nm_slMaxSize);
  Initialize(nmOriginal);
}

void CNetworkMessage::operator=(const CNetworkMessage &nmOriginal)
{
  if (nm_slMaxSize != nmOriginal.nm_slMaxSize) {
    ASSERTALWAYS("Must be same size");

    if (nm_pubMessage!=NULL) {
      FreeMemory(nm_pubMessage);
    }

    // allocate message buffer
    nm_slMaxSize = nmOriginal.nm_slMaxSize;
    nm_pubMessage = (UBYTE*) AllocMemory(nm_slMaxSize);
  }

  // init read/write pointer and size
  nm_pubPointer = nm_pubMessage+sizeof(UBYTE);// + (nmOriginal.nm_pubPointer-nmOriginal.nm_pubMessage);
  nm_iBit = 0;
  nm_slSize = nmOriginal.nm_slSize;

  // copy from original
  memcpy(nm_pubMessage, nmOriginal.nm_pubMessage, nm_slSize);

  // remember the type
  nm_mtType = nmOriginal.nm_mtType;
}

/*
 * Destructor.
 */
CNetworkMessage::~CNetworkMessage(void)
{
  ASSERT(nm_pubMessage!=NULL);
  if (nm_pubMessage!=NULL) {
    FreeMemory(nm_pubMessage);
    nm_pubMessage = NULL;
  }
}

// Network message initialization
void CNetworkMessage::Initialize(void)
{
  ASSERT(nm_pubMessage!=NULL); // must be valid

  // mangle pointer and size so that it could not be accidentally read/written
  nm_slMaxSize = MAX_NETWORKMESSAGE_SIZE;
  nm_pubPointer = NULL;
  nm_iBit = -1;
  nm_slSize = -1;
}

void CNetworkMessage::Initialize(const CNetworkMessage &nmOriginal)
{
  ASSERT(nm_pubMessage!=NULL); // must be valid

  nm_slMaxSize = nmOriginal.nm_slMaxSize;
  // init read/write pointer and size
  nm_pubPointer = nm_pubMessage + (nmOriginal.nm_pubPointer-nmOriginal.nm_pubMessage);
  nm_iBit = nmOriginal.nm_iBit;
  nm_slSize = nmOriginal.nm_slSize;

  // copy from original
  memcpy(nm_pubMessage, nmOriginal.nm_pubMessage, nm_slSize);

  // remember the type
  nm_mtType = nmOriginal.nm_mtType;
}

void CNetworkMessage::Initialize(UBYTE mtType)
{
  ASSERT(nm_pubMessage!=NULL); // must be valid

  // init read/write pointer and size
  nm_slMaxSize = MAX_NETWORKMESSAGE_SIZE;
  nm_pubPointer = nm_pubMessage;
  nm_iBit = 0;
  nm_slSize = 0;

  // remember the type
  nm_mtType = mtType;
  // write the type
  UBYTE ubType = nm_mtType;
  Write(&ubType, sizeof(ubType));
}
//0524 kwon 추가.
void CNetworkMessage::SetLoginType( UBYTE LoginType)
{
	UBYTE ubType = nm_mtType;
	Write(&ubType, sizeof(ubType));
}

// reinit a message that is to be sent (to write different contents)
void CNetworkMessage::Reinit(void)
{
  // init read/write pointer and size
  nm_slMaxSize = MAX_NETWORKMESSAGE_SIZE;
  nm_pubPointer = nm_pubMessage;
  nm_iBit = 0;
  nm_slSize = 0;
  // write the type
  UBYTE ubType = nm_mtType;
  Write(&ubType, sizeof(ubType));
}

/*
 * Ignore the contents of this message.
 */
void CNetworkMessage::IgnoreContents(void)
{
}


/* Check if end of message. */
BOOL CNetworkMessage::EndOfMessage(void)
{
  ASSERTMSG((nm_pubPointer-nm_pubMessage) <= nm_slSize, "Message over-reading!");
  return (nm_pubPointer-nm_pubMessage) >= nm_slSize;
}

CNetworkMessage &CNetworkMessage::operator>>(CTString &str)
{
  // start reading string from message
  str = "";
  nm_iBit = 0;
  // repeat
  for(;;) {
    // if reached end of message (this happens when we read string-only messages)
    if (nm_pubPointer-nm_pubMessage>=nm_slSize) {
      // stop
      return *this;
    }
    // get next char
    char strChar[2];
    strChar[0] = *nm_pubPointer++;
    strChar[1] = 0;
    // if end of string
    if (strChar[0]==0) {
      // stop
      return *this;
    // if normal char
    } else {
      // append to the string
      str+=strChar;
    }
  }
}
CNetworkMessage &CNetworkMessage::operator<<(const CTString &str)
{
  // start writing string to message
  nm_iBit = 0;
  const char *pstr = (const char *)str;
  // repeat
  for(;;) {
    // if reached one byte before end of message
    if (nm_pubPointer-nm_pubMessage>=nm_slMaxSize-1) {
      // put the end marker
      *nm_pubPointer++ = 0;
      nm_slSize++;
      // report error and stop
      CPrintF(TRANS("Warning: Message over-writing!\n"));
      ASSERT(FALSE);
      return *this;
    }
    // get next char
    const char chr = *pstr++;
    // write it to message
    *nm_pubPointer++ = chr;
    nm_slSize++;
    // if end
    if (chr==0) {
      // stop
      return *this;
    }
  }
}

/*
 * Insert a sub-message into this message.
 */
void CNetworkMessage::InsertSubMessage(const CNetworkMessage &nmSubMessage)
{
  // write sub-message size
  operator<<(nmSubMessage.nm_slSize);
  // write the contents of the sub-message
  Write(nmSubMessage.nm_pubMessage, nmSubMessage.nm_slSize);
}

/*
 * Extract a sub-message from this message.
 */
void CNetworkMessage::ExtractSubMessage(CNetworkMessage &nmSubMessage)
{
  // read sub-message size
  operator>>(nmSubMessage.nm_slSize);
  // read the contents of the sub-message
  Read(nmSubMessage.nm_pubMessage, nmSubMessage.nm_slSize);
  // init the submessage read/write pointer
  nmSubMessage.nm_pubPointer = nmSubMessage.nm_pubMessage;
  nmSubMessage.nm_iBit = 0;
  // get the submessage type
  UBYTE ubType = 0;
  nmSubMessage>>ubType;
  nmSubMessage.nm_mtType = (UBYTE)ubType;
}

// rewind message to start, so that written message can be read again
void CNetworkMessage::Rewind(void)
{
  nm_pubPointer = nm_pubMessage+sizeof(UBYTE);
  nm_iBit = 0;
}

/*
 * Pack a message to another message (message type is left untouched).
 */
void CNetworkMessage::Pack(CNetworkMessage &nmPacked, CCompressor &comp)
{
  // get size and pointers for packing, leave the message type alone
  SLONG slUnpackedSize = nm_slSize-sizeof(UBYTE);
  void *pvUnpacked     = nm_pubMessage+sizeof(UBYTE);

  SLONG slPackedSize    = nmPacked.nm_slMaxSize-sizeof(UBYTE);
  void *pvPacked        = nmPacked.nm_pubMessage+sizeof(UBYTE);

  // pack it there
  ASSERT(comp.NeededDestinationSize(slUnpackedSize)<=slPackedSize);
  BOOL bSucceded = comp.Pack(pvUnpacked, slUnpackedSize, pvPacked, slPackedSize);
  ASSERT(bSucceded);

  // set up the destination message size
  nmPacked.nm_slSize = slPackedSize+sizeof(UBYTE);
}
/*
 * Unpack a message to another message (message type is left untouched).
 */
void CNetworkMessage::Unpack(CNetworkMessage &nmUnpacked, CCompressor &comp)
{
  // get size and pointers for unpacking, leave the message type alone
  SLONG slPackedSize = nm_slSize-sizeof(UBYTE);
  void *pvPacked     = nm_pubMessage+sizeof(UBYTE);

  SLONG slUnpackedSize = nmUnpacked.nm_slMaxSize-sizeof(UBYTE);
  void *pvUnpacked     = nmUnpacked.nm_pubMessage+sizeof(UBYTE);

  // unpack it there
  BOOL bSucceeded = comp.Unpack(pvPacked, slPackedSize, pvUnpacked, slUnpackedSize);
  ASSERT(bSucceeded);

  // set up the destination message size
  nmUnpacked.nm_slSize = slUnpackedSize+sizeof(UBYTE);
}


// NOTE:
// compression type bits in the messages are different than compression type cvar values
// this is to keep backward compatibility with old demos saved with full compression
void CNetworkMessage::PackDefault(CNetworkMessage &nmPacked)
{
  extern INDEX net_iCompression;
//0707

  int comp_type = 0;
#ifdef COMPRESSION_MESSAGE
	comp_type = 2;
#else
	comp_type = 0;
#endif
  

  if (comp_type==2) {
    // pack with zlib only
    CzlibCompressor compzlib;
    Pack(nmPacked, compzlib);
    (int&)nmPacked.nm_mtType|=0<<6;
  } else if (comp_type==1) {
    // pack with LZ only
    CLZCompressor compLZ;
    Pack(nmPacked, compLZ);
    (int&)nmPacked.nm_mtType|=1<<6;
  } else {
    // no packing
    SLONG slUnpackedSize = nm_slSize-sizeof(UBYTE);
    void *pvUnpacked     = nm_pubMessage+sizeof(UBYTE);
    void *pvPacked       = nmPacked.nm_pubMessage+sizeof(UBYTE);
    nmPacked.nm_slSize   = slUnpackedSize+sizeof(UBYTE);
    memcpy(pvPacked, pvUnpacked, slUnpackedSize);
    (int&)nmPacked.nm_mtType|=2<<6;
  }
  nmPacked.nm_pubMessage[0] = (UBYTE)nmPacked.nm_mtType;

  /*  
  // pack with RLE and LZ
  CNetworkMessage nmPackedRLE(GetType());
  CRLEBBCompressor compRLE;
  Pack(nmPackedRLE ,compRLE);
  CLZCompressor compLZ;
  nmPackedRLE.Pack(nmPacked, compLZ);
  //*/
}
void CNetworkMessage::UnpackDefault(CNetworkMessage &nmUnpacked)
{
  switch (nm_mtType>>6) {
  case 0: {
    // unpack with zlib only
    CzlibCompressor compzlib;
    Unpack(nmUnpacked,compzlib);
          } break;
  case 1: {
    // unpack with LZ only
    CLZCompressor compLZ;
    Unpack(nmUnpacked,compLZ);
          } break;
  default:
  case 2: {
    // no unpacking
    SLONG slPackedSize = nm_slSize-sizeof(UBYTE);
    void *pvPacked     = nm_pubMessage+sizeof(UBYTE);
    void *pvUnpacked   = nmUnpacked.nm_pubMessage+sizeof(UBYTE);
    nmUnpacked.nm_slSize = slPackedSize+sizeof(UBYTE);
    memcpy(pvUnpacked, pvPacked, slPackedSize);
          } break;
  }

  /*
  // unpack with LZ and RLE
  CNetworkMessage nmUnpackedLZ(GetType());
  CLZCompressor compLZ;
  Unpack(nmUnpackedLZ,compLZ);
  CRLEBBCompressor compRLE;
  nmUnpackedLZ.Unpack(nmUnpacked, compRLE);
  //*/
}

// dump message to console
void CNetworkMessage::Dump(void)
{
  CPrintF("Message size: %d\n", nm_slSize);
  CPrintF("Message contents:");
  for(INDEX iByte=0; iByte<nm_slSize; iByte++) {
    if (iByte%16==0) {
      CPrintF("\n");
    }
    CPrintF("%02x", nm_pubMessage[iByte]);
  }
  CPrintF("\n--\n");
}

//! 메시지 버퍼를 꼭 맞게 줄인다.
// shrink message buffer to exactly fit contents
void CNetworkMessage::Shrink(void)
{
  // return; // shrinkg has been disabled cos of CReusableContainer

  ASSERT(nm_slSize>0);
  // remember original pointer offset
  SLONG slOffset = nm_pubPointer-nm_pubMessage;
  // allocate message buffer
  // ShrinkMemory((void**)&nm_pubMessage, nm_slSize);
  nm_slMaxSize = nm_slSize;
  // set new pointer
  nm_pubPointer = nm_pubMessage + slOffset;
}

// copy one bit from a byte to another byte
static inline void CopyBit(const UBYTE &ubSrc, INDEX iSrc, UBYTE &ubDst, INDEX iDst)
{
  if (ubSrc&(1<<iSrc)) {
    ubDst |= (1<<iDst);
  } else {
    ubDst &= ~(1<<iDst);
  }
}

void CNetworkMessage::ReadBits(void *pvBuffer, INDEX ctBits)
{
  UBYTE *pubDstByte = (UBYTE *)pvBuffer;
  // for each bit
  INDEX iDstBit = 0;
  for (INDEX iBit=0; iBit<ctBits; iBit++) {
    // get next bit and byte in message
    UBYTE *pubSrcByte = nm_pubPointer;
    INDEX iSrcBit = nm_iBit;
    // if bit is 0 - start of byte
    if (nm_iBit==0) {
      // we will start reading from this byte, advance message to next byte
      nm_pubPointer++;
    // if bit is not 0 - inside byte
    } else {
      // we will read from previous byte, keep message as-is
      pubSrcByte--;
    }

    // copy the bit
    CopyBit(*pubSrcByte, iSrcBit, *pubDstByte, iDstBit);

    // go to next bit
    nm_iBit++;
    if (nm_iBit>=8) {
      nm_iBit = 0;
    }
    iDstBit++;
    if (iDstBit>=8) {
      iDstBit = 0;
      pubDstByte++;
    }
  }
}

void CNetworkMessage::WriteBits(const void *pvBuffer, INDEX ctBits)
{
  const UBYTE *pubSrcByte = (const UBYTE *)pvBuffer;
  // for each bit
  INDEX iSrcBit = 0;
  for (INDEX iBit=0; iBit<ctBits; iBit++) {
    // get next bit and byte in message
    UBYTE *pubDstByte = nm_pubPointer;
    INDEX iDstBit = nm_iBit;
    // if bit is 0 - start of byte
    if (nm_iBit==0) {
      // we will start writing to this byte, advance message to next byte
      nm_pubPointer++;
      nm_slSize++;
    // if bit is not 0 - inside byte
    } else {
      // we will write to previous byte, keep message as-is
      pubDstByte--;
    }

    // copy the bit
    CopyBit(*pubSrcByte, iSrcBit, *pubDstByte, iDstBit);

    // go to next bit
    nm_iBit++;
    if (nm_iBit>=8) {
      nm_iBit = 0;
    }
    iSrcBit++;
    if (iSrcBit>=8) {
      iSrcBit = 0;
      pubSrcByte++;
    }
  }
}


/////////////////////////////////////////////////////////////////////
// CNetworkStreamBlock

/*
 * Constructor for receiving -- uninitialized block.
 */
CNetworkStreamBlock::CNetworkStreamBlock(void)
  : CNetworkMessage()
  , nsb_iSequenceNumber(-1)
{
}

/*
 * Constructor for sending -- empty packet with given type and sequence.
 */
CNetworkStreamBlock::CNetworkStreamBlock(UBYTE mtType, INDEX iSequenceNumber)
  : CNetworkMessage(mtType)
  , nsb_iSequenceNumber(iSequenceNumber)
{
}

//! 받은 메시지로부터 블록을 읽어들인다.
/*
 * Read a block from a received message.
 */
void CNetworkStreamBlock::ReadFromMessage(CNetworkMessage &nmToRead)
{
  // read sequence number from message
  nmToRead>>nsb_iSequenceNumber;
  ASSERT(nsb_iSequenceNumber>=0);
  // read the block as a submessage
  nmToRead.ExtractSubMessage(*this);
}

/*
 * Add a block to a message to send.
 */
void CNetworkStreamBlock::WriteToMessage(CNetworkMessage &nmToWrite)
{
  // write sequence number to message
  ASSERT(nsb_iSequenceNumber>=0);
  nmToWrite<<nsb_iSequenceNumber;
  // write the block as a submessage
  nmToWrite.InsertSubMessage(*this);
}

/*
 * Remove the block from stream. */
void CNetworkStreamBlock::RemoveFromStream(void)
{
  nsb_lnInStream.Remove();
}

// Network stream block clearing (same block may be used again)
void CNetworkStreamBlock::Clear(void)
{
  // Fake list node destructor
  if(nsb_lnInStream.IsLinked()) {
    // remove it from list
    nsb_lnInStream.Remove();
  }
  // must be unlinked by now
  ASSERT(!nsb_lnInStream.IsLinked());
  // Reset sequence number
  nsb_iSequenceNumber = -1;
  // Fake network message initialization
  Initialize();
  // 
}

/* Read/write the block from file stream. */
void CNetworkStreamBlock::Write_t(CTStream &strm) // throw char *
{
  // write sequence number
  strm<<nsb_iSequenceNumber;
  // write block size
  strm<<nm_slSize;
  // write block contents
  strm.Write_t(nm_pubMessage, nm_slSize);
}
void CNetworkStreamBlock::Read_t(CTStream &strm) // throw char *
{
  // read sequence number
  strm>>nsb_iSequenceNumber;
  // read block size
  strm>>nm_slSize;
  // read block contents
  strm.Read_t(nm_pubMessage, nm_slSize);
  // init the message read/write pointer
  nm_pubPointer = nm_pubMessage;
  nm_iBit = 0;
  // get the message type
  UBYTE ubType = 0;
  (*this)>>ubType;
  nm_mtType = (UBYTE)ubType;
}

void CNetworkStreamBlock::operator delete(void *p)
{
  // delete must be called from DeleteNetworkStreamBlock function
  if(!_rcStreamBlockContainer.rc_bNowDeleting) {
    ASSERTALWAYS("Block must be deleted with DeleteNetworkStreamBlock function");
    // Halt release version if not this isn't final version
    extern INDEX dbg_bAtHome;
    if(dbg_bAtHome) {
      _asm int 3;
    }
  }
  FreeMemory(p);
}

/////////////////////////////////////////////////////////////////////
// CNetworkStream
/*
 * Constructor.
 */
CNetworkStream::CNetworkStream(void)
{
}

/*
 * Destructor.
 */
CNetworkStream::~CNetworkStream(void)
{
  // report number of blocks left in stream
  //_RPT1(_CRT_WARN, "Destructing stream, %d blocks contained.\n", ns_lhBlocks.Count());
  // remove all blocks
  Clear();
}

/*
 * Clear the object (remove all blocks).
 */
void CNetworkStream::Clear(void)
{
  // for each block in list
  FORDELETELIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsbInList) {
    // remove it from list
    itnsbInList->nsb_lnInStream.Remove();
    // delete it
    DeleteNetworkStreamBlock(&*itnsbInList);
  }
}
/* Copy from another network stream. */
void CNetworkStream::Copy(CNetworkStream &nsOther)
{
  // for each block in list
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, nsOther.ns_lhBlocks, itnsb) {
    // add it here
    AddBlock(*itnsb);
  }
}

// get number of blocks used by this object
INDEX CNetworkStream::GetUsedBlocks(void)
{
  return ns_lhBlocks.Count();
}

// get amount of memory used by this object
SLONG CNetworkStream::GetUsedMemory(void)
{
  SLONG slMem = 0;
  // for each block in list
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsb) {
    // add its usage
    slMem+=sizeof(CNetworkStreamBlock)+itnsb->nm_slMaxSize;
  }
  return slMem;
}

// get index of newest sequence stored
INDEX CNetworkStream::GetNewestSequence(void)
{
  // if the stream is empty
  if (ns_lhBlocks.IsEmpty()) {
    // return dummy
    return -1;
  }
  // get head of list
  CNetworkStreamBlock *pnsb = LIST_HEAD(ns_lhBlocks, CNetworkStreamBlock, nsb_lnInStream);
  // return its index
  return pnsb->nsb_iSequenceNumber;
}

//! 블록을 이미 할당된 스트림에 더한다.
/*
 * Add a block that is already allocated to the stream.
 */
void CNetworkStream::AddAllocatedBlock(CNetworkStreamBlock *pnsbBlock)
{
  // search all blocks already in list
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsbInList) {
    // if the block in list has same sequence as the one to add
    if (itnsbInList->nsb_iSequenceNumber == pnsbBlock->nsb_iSequenceNumber) {
      // just discard the new block
      DeleteNetworkStreamBlock(pnsbBlock);
      return;
    }
    // if the block in list has lower sequence than the one to add
    if (itnsbInList->nsb_iSequenceNumber < pnsbBlock->nsb_iSequenceNumber) {
      // stop searching
      break;
    }
  }
  // add the new block before current one
  itnsbInList.InsertBeforeCurrent(pnsbBlock->nsb_lnInStream);
}

/*
 * Add a block to the stream.
 */
void CNetworkStream::AddBlock(CNetworkStreamBlock &nsbBlock)
{
  // CNetworkStreamBlock *pnsbCopy = new CNetworkStreamBlock(nsbBlock);

  // create a copy of the block
  CNetworkStreamBlock *pnsbCopy = CreateNetworkStreamBlock();
  // Fake constructor for CNetworkMessage
  pnsbCopy->Initialize(nsbBlock);
  // Rest of copy copy constructor for CNetworkStreamBlock
  pnsbCopy->nsb_iSequenceNumber = nsbBlock.nsb_iSequenceNumber;

  // shrink it
  pnsbCopy->Shrink();
  // add it to the list
  AddAllocatedBlock(pnsbCopy);
}

//! 서브메시지로서 블록을 메시지로부터 읽는다. 그리고 그것을 스트림에 추가한다.
/*
 * Read a block as a submessage from a message and add it to the stream.
 */
void CNetworkStream::ReadBlock(CNetworkMessage &nmMessage)
{
  // create an empty block
  CNetworkStreamBlock *pnsbRead = CreateNetworkStreamBlock();
  // CNetworkStreamBlock *pnsbRead = new CNetworkStreamBlock();

  // read it from message
  pnsbRead->ReadFromMessage(nmMessage);
  // shrink it
  pnsbRead->Shrink();
  // add it to the list
  AddAllocatedBlock(pnsbRead);
}

/*
 * Get a block from stream by its sequence number.
 */
CNetworkStream::Result CNetworkStream::GetBlockBySequence(
  INDEX iSequenceNumber, CNetworkStreamBlock *&pnsbBlock)
{
  BOOL bNewerFound = FALSE;
  // search all blocks in list
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsbInList) {
    CNetworkStreamBlock &nsbInList = *itnsbInList;
    // if the block in list has newer sequence
    if (nsbInList.nsb_iSequenceNumber >= iSequenceNumber) {
      // remember that at least one newer block was found
      bNewerFound = TRUE;
    }

    // if the block in list has wanted sequence
    if (nsbInList.nsb_iSequenceNumber == iSequenceNumber) {
      // return it
      pnsbBlock = itnsbInList;
      return R_OK;
    }
    // if the block in list has older sequence
    if (nsbInList.nsb_iSequenceNumber < iSequenceNumber) {
      // stop searching
      break;
    }
  }

  // ...if none found

  // if some block of newer sequence number was found
  if (bNewerFound) {
    // return that the block is missing (probably should be resent)
    pnsbBlock = NULL;
    return R_BLOCKMISSING;
  // if no newer blocks were found
  } else {
    // we assume that the wanted block is not yet received
    pnsbBlock = NULL;
    return R_BLOCKNOTRECEIVEDYET;
  }
}

// find oldest block after given one (for batching missing sequences)
INDEX CNetworkStream::GetOldestSequenceAfter(INDEX iSequenceNumber)
{
  // block are sorted newer first, so we just remember the last block found
  // until we find the given one
  INDEX iOldest = iSequenceNumber;
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsb) {
    CNetworkStreamBlock &nsb = *itnsb;
    if (nsb.nsb_iSequenceNumber<iSequenceNumber) {
      break;
    } else {
      iOldest = nsb.nsb_iSequenceNumber;
    }
  }
  return iOldest;
}

/*
 * Write given number of newest blocks to a message.
 */
INDEX CNetworkStream::WriteBlocksToMessage(CNetworkMessage &nmMessage, INDEX ctBlocks)
{
  // for given number of newest blocks in list
  INDEX iBlock=0;
  FOREACHINLIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsbInList) {
    // write the block to message
    itnsbInList->WriteToMessage(nmMessage);
    iBlock++;
    if (iBlock>=ctBlocks) {
      return iBlock;
    }
  }
  return iBlock;
}

/*
 * Remove all blocks but the given number of newest ones.
 */
void CNetworkStream::RemoveOlderBlocks(INDEX ctBlocksToKeep)
{
  // for each block in list
  INDEX iBlock = 0;
  FORDELETELIST(CNetworkStreamBlock, nsb_lnInStream, ns_lhBlocks, itnsbInList) {
    iBlock++;
    // if it is older that given count
    if (iBlock>ctBlocksToKeep) {
      // remove it from list
      itnsbInList->nsb_lnInStream.Remove();
      // delete it
      DeleteNetworkStreamBlock(&*itnsbInList);
    }
  }
}

/* Remove all blocks with sequence older than given. */
void CNetworkStream::RemoveOlderBlocksBySequence(INDEX iLastSequenceToKeep)
{
  // while there are any blocks in the list
  while(!ns_lhBlocks.IsEmpty()) {
    // get the tail of the list
    CNetworkStreamBlock *pnsb = LIST_TAIL(ns_lhBlocks, CNetworkStreamBlock, nsb_lnInStream);
    // if it is not too old
    if (pnsb->nsb_iSequenceNumber >= iLastSequenceToKeep) {
      // stop
      break;
    }
    // remove the tail
    DeleteNetworkStreamBlock(pnsb);
  };
}

/////////////////////////////////////////////////////////////////////
// CPlayerAction

CPlayerAction::CPlayerAction(void)
{
  Clear();
}

/*
 * Clear the object (this sets up no actions).
 */
void CPlayerAction::Clear(void)
{
  pa_vTranslation = FLOAT3D(0.0f,0.0f,0.0f);
  pa_aRotation = ANGLE3D(0,0,0);
  pa_aViewRotation = ANGLE3D(0,0,0);
  pa_ulButtons = 0;
  pa_ulCreated = 0;
}
// normalize action (remove invalid floats like -0)
void CPlayerAction::Normalize(void)
{
  volatile FLOAT *pf = (FLOAT*)&pa_vTranslation;
  for (INDEX i=0; i<9; i++) {
    if (*pf==0) {
      *pf=0;
    }
    pf++;
  }
}

void CPlayerAction::Lerp(const CPlayerAction &pa0, const CPlayerAction &pa1, FLOAT fFactor)
{
  pa_vTranslation  = ::Lerp(pa0.pa_vTranslation , pa1.pa_vTranslation , fFactor);
  pa_aRotation     = ::Lerp(pa0.pa_aRotation    , pa1.pa_aRotation    , fFactor);
  pa_aViewRotation = ::Lerp(pa0.pa_aViewRotation, pa1.pa_aViewRotation, fFactor);
  pa_ulButtons = pa1.pa_ulButtons;
}

// player action compression algorithm:
// - all axes (9 of them) are compressed as one bit telling whether the axis value is used
// if that bit is ==0, then the axis value is 0.0, if it is 1, the value follows in next 32 
// bits
// - the flags are compressed by preceding them with a bit sequence telling how many bits
// are saved after that:
//   (0)          1       = no bits follow, value is 0
//   (1)          01      = no bits follow, value is 1
//   (2-3)        001     = 1 bit follows, value is 1x where x is the given bit
//   (4-15)       0001    = 4 bit value follows
//   (16-255)     00001   = 8 bit value follows
//   (256-65535)  000001  = 16 bit value follows
//   (65536-)     000000  = 32 bit value follows
// note: above bits are ordered in reverse as they come when scanning bit by bit

/* Write an object into message. */
CNetworkMessage &operator<<(CNetworkMessage &nm, const CPlayerAction &pa)
{
  nm.Write(&pa.pa_ulCreated, sizeof(pa.pa_ulCreated));

  const ULONG *pul = (const ULONG*)&pa.pa_vTranslation;
  for (INDEX i=0; i<9; i++) {
    if (*pul==0) {
      UBYTE ub=0;
      nm.WriteBits(&ub, 1);
    } else {
      UBYTE ub=1;
      nm.WriteBits(&ub, 1);
      nm.WriteBits(pul, 32);
    }
    pul++;
  }
  ULONG ulFlags = pa.pa_ulButtons;

  // (0)          1       = no bits follow, value is 0
  if (ulFlags==0) {
    UBYTE ub=1;
    nm.WriteBits(&ub, 1);
  // (1)          01      = no bits follow, value is 1
  } else if (ulFlags==1) {
    UBYTE ub=2;
    nm.WriteBits(&ub, 2);
  // (2-3)        001     = 1 bit follows, value is 1x where x is the given bit
  } else if (ulFlags <= 3) {
    UBYTE ub=4;
    nm.WriteBits(&ub, 3);
    nm.WriteBits(&ulFlags, 1);
  // (4-15)       0001    = 4 bit value follows
  } else if (ulFlags <= 15) {
    UBYTE ub=8;
    nm.WriteBits(&ub, 4);
    nm.WriteBits(&ulFlags, 4);
  // (16-255)     00001   = 8 bit value follows
  } else if (ulFlags <= 255) {
    UBYTE ub=16;
    nm.WriteBits(&ub, 5);
    nm.WriteBits(&ulFlags, 8);
  // (256-65535)  000001  = 16 bit value follows
  } else if (ulFlags <= 65535) {
    UBYTE ub=32;
    nm.WriteBits(&ub, 6);
    nm.WriteBits(&ulFlags, 16);
  // (65536-)     000000  = 32 bit value follows
  } else {
    UBYTE ub=0;
    nm.WriteBits(&ub, 6);
    nm.WriteBits(&ulFlags, 32);
  }
  return nm;
}
/* Read an object from message. */
CNetworkMessage &operator>>(CNetworkMessage &nm, CPlayerAction &pa)
{
  nm.Read(&pa.pa_ulCreated, sizeof(pa.pa_ulCreated));

  ULONG *pul = (ULONG*)&pa.pa_vTranslation;
  for (INDEX i=0; i<9; i++) {
    UBYTE ub = 0;
    nm.ReadBits(&ub, 1);
    if (ub==0) {
      *pul = 0;
    } else {
      nm.ReadBits(pul, 32);
    }
    pul++;
  }

  // find number of zero bits for flags
  INDEX iZeros;
  for( iZeros=0; iZeros<6; iZeros++) {
    UBYTE ub=0;
    nm.ReadBits(&ub, 1);
    if (ub!=0) {
      break;
    }
  }
  ULONG ulFlags = 0;
  // now read flags according to the number of bits
  // (0)          1       = no bits follow, value is 0
  if (iZeros==0) {
    ulFlags = 0;
  // (1)          01      = no bits follow, value is 1
  } else if (iZeros==1) {
    ulFlags = 1;
  // (2-3)        001     = 1 bit follows, value is 1x where x is the given bit
  } else if (iZeros==2) {
    ulFlags = 0;
    nm.ReadBits(&ulFlags, 1);
    ulFlags |= 2;
  // (4-15)       0001    = 4 bit value follows
  } else if (iZeros==3) {
    ulFlags = 0;
    nm.ReadBits(&ulFlags, 4);
  // (16-255)     00001   = 8 bit value follows
  } else if (iZeros==4) {
    ulFlags = 0;
    nm.ReadBits(&ulFlags, 8);
  // (256-65535)  000001  = 16 bit value follows
  } else if (iZeros==5) {
    ulFlags = 0;
    nm.ReadBits(&ulFlags, 16);
  // (65536-)     000000  = 32 bit value follows
  } else {
    ulFlags = 0;
    nm.ReadBits(&ulFlags, 32);
  }
  pa.pa_ulButtons = ulFlags;
  return nm;
}
/* Write an object into stream. */
CTStream &operator<<(CTStream &strm, const CPlayerAction &pa)
{
  strm.Write_t(&pa,sizeof(pa));
  return strm;
}
/* Read an object from stream. */
CTStream &operator>>(CTStream &strm, CPlayerAction &pa)
{
  strm.Read_t(&pa,sizeof(pa));
  return strm;
}


// Create new network stream block (use instead of new CNetworkStreamBlock)
extern CNetworkStreamBlock *CreateNetworkStreamBlock(void)
{
  TRACKMEM(Mem,"Network");
  return _rcStreamBlockContainer.CreateObject();
}

// Delete network stream block (use instead of delete CNetworkStreamBlock)
extern void DeleteNetworkStreamBlock(CNetworkStreamBlock *pnsbBlock)
{
  TRACKMEM(Mem,"Network");
  CObjectRestore<BOOL> or(_rcStreamBlockContainer.rc_bNowDeleting,TRUE);
  _rcStreamBlockContainer.DeleteObject(pnsbBlock);
}
