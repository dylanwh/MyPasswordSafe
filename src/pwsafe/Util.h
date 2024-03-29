// Util.h
//-----------------------------------------------------------------------------
#ifndef Util_h
#define Util_h

//#include "PasswordSafe.h"

//#include "MyString.h"
#include "securedstring.hpp"
#include <assert.h>

#include <string>
using std::string;
#include "PW_sha1.h"
#include "PwsPlatform.h"
#include "myutil.hpp"

// For now
#define LPTSTR wchar_t *
#define LPCTSTR const wchar_t *
#define LPSTR char *
#define LPCSTR const char *

#define SaltLength 20
#define SaltSize 20
#define StuffSize 10

// this is for the undocumented 'command line file encryption'
#define CIPHERTEXT_SUFFIX ".PSF"

//Use non-standard dash (ANSI decimal 173) for separation
// FIXME (nolan): define _T
#define _T

#define SPLTCHR _T('\xAD')
#define SPLTSTR _T("  \xAD  ")
#define DEFUSERCHR _T('\xA0')

//Version defines
#define V10 0
#define V15 1

//Some extra typedefs -- I'm addicted to typedefs
typedef char    int8;
typedef short   int16;
typedef int     int32;
//typedef __int64 int64;

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
//typedef unsigned __int64 uint64;

void xormem(unsigned char* mem1, unsigned char* mem2, int length);

extern void trashMemory(SHA1_CTX& context);
extern void trashMemory(unsigned char* buffer,
                        long length,
                        int numiter = 30);
extern void trashMemory(char *buffer, long length, int numiter = 30 );
extern void GenRandhash(const SecuredString &passkey,
                        const unsigned char* m_randstuff,
                        unsigned char* m_randhash);
extern unsigned char newrand();

extern unsigned int  RangeRand(size_t len);
extern char GetRandAlphaNumChar(void);
extern char GetRandAlphaNumSymbolChar(void);
string   GetAlphaNumPassword(unsigned int pwlen);

class BlowFish;
extern BlowFish *MakeBlowFish(const unsigned char *pass, int passlen,
			      const unsigned char *salt, int saltlen);

// buffer is allocated by _readcbc, *** delete[] is responsibility of caller ***
extern int _readcbc(FILE *fp, unsigned char* &buffer, unsigned int &buffer_len,
		    const unsigned char *pass, int passlen,
		    const unsigned char* salt, int saltlen,
		    unsigned char* cbcbuffer);
extern int _writecbc(FILE *fp, const unsigned char* buffer, int length,
		     const unsigned char *pass, int passlen,
		     const unsigned char* salt, int saltlen,
		     unsigned char* cbcbuffer);

/*
 * Get an integer that is stored in little-endian format
 */
inline int getInt32( const unsigned char *buf )
{
	assert(sizeof(int) == 4);
	//#if defined(LITTLE_ENDIAN)
#if(BYTE_ORDER == LITTLE_ENDIAN)
#if defined(_DEBUG)
	if ( *(int*) buf != (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24)) )
	{
		DBGOUT( "Warning: LITTLE_ENDIAN defined but architecture is big endian\n" );
	}
#endif
	return *(int *) buf;
	//#elif defined(BIG_ENDIAN)
#elif(BYTE_ORDER == BIG_ENDIAN)
#if defined(_DEBUG)
	// Folowing code works for big or little endian architectures but we'll warn anyway
	// if CPU is really little endian
	if ( *(int*) buf == (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24)) )
	{
		DBGOUT( "Warning: BIG_ENDIAN defined but architecture is little endian\n" );
	}
#endif
	return (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24) );
#else
#error Is the target CPU big or little endian?
#endif
}

/*
 * Store an integer that is stored in little-endian format
 */
inline void putInt32( unsigned char *buf, const int val )
{
	assert(sizeof(int) == 4);
	//#if defined(LITTLE_ENDIAN)
#if(BYTE_ORDER == LITTLE_ENDIAN)
	*(int32 *) buf = val;
#if defined(DEBUG)
	if ( *(int*) buf != (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24)) )
	{
		DBGOUT( "Warning: LITTLE_ENDIAN defined but architecture is big endian\n" );
	}
#endif
	//#elif defined(BIG_ENDIAN)
#elif(BYTE_ORDER == BIG_ENDIAN)
	buf[0] = val & 0xFF;
	buf[1] = (val >> 8) & 0xFF;
	buf[2] = (val >> 16) & 0xFF;
	buf[3] = (val >> 24) & 0xFF;
#if defined(DEBUG)
	// Above code works for big or little endian architectures but we'll warn anyway
	// if CPU is really little endian
	if ( *(int*) buf == (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24)) )
	{
		DBGOUT( "Warning: BIG_ENDIAN defined but architecture is little endian\n" );
	}
#endif
#else
#error Is the target CPU big or little endian?
#endif
}

inline char * strCopy( char *target, const char *source )
{
	return strcpy( target, source );
}

#if defined(UNICODE)
inline LPTSTR strCopy( LPTSTR target, LPCTSTR source )
{
	return wcscpy( target, source );
}
#endif

inline int strLength( const char *str )
{
	return strlen( str );
}

#if defined(UNICODE)
inline int strLength( const wchar_t *str )
{
	return wcslen( str );
}
#endif

inline char * strFind( const char *str, const char *fstr )
{
	return (char *)strstr( str, (char *)fstr );
}

#if defined(UNICODE)
inline wchar_t * strFind( const wchar_t *str, const wchar_t *fstr )
{
	return wcsstr( str, fstr );
}
#endif

#if defined(WITH_LEGACY_CMDLINE)
//void _encryptFile(CString filepath);
//void _decryptFile(CString filepath);
//void convertToLongFilePath(CString& filepath);
void manageCmdLine(CString m_lpCmdLine);
#endif

extern long		fileLength( FILE *fp );

#endif // Util_h
//-----------------------------------------------------------------------------
// Local variables:
// mode: c++
// End:
