// HSMD5Encrypt.h: interface for the CHSMD5Encrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSMD5ENCRYPT_H__97D80BA5_BBE6_4308_A060_6F652EFD5540__INCLUDED_)
#define AFX_HSMD5ENCRYPT_H__97D80BA5_BBE6_4308_A060_6F652EFD5540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef	DWORD	UINT4;

class CHSMD5Encrypt : public CObject  
{
public:
	CHSMD5Encrypt();
	virtual ~CHSMD5Encrypt();
public:
	typedef struct _MD5_CTX
	{
		UINT4				i[2]; /* number of _bits_ handled mod 2^64 */
		UINT4				buf[4]; /* scratch buffer */
		unsigned char		in[64]; /* input buffer */
		unsigned char		digest[32]; /* actual digest after MD5Final call*/
	} MD5_CTX;

// Operations
public:
	void	digest_md5( unsigned char* buffer, int buffer_len, unsigned char* digest );

protected:
	void	MD5Init( MD5_CTX *mdContext );
	void	MD5Update( MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen );
	void	MD5Final( MD5_CTX *mdContext );
	void	Transform( UINT4 *buf, UINT4 *in );

};

#endif // !defined(AFX_HSMD5ENCRYPT_H__97D80BA5_BBE6_4308_A060_6F652EFD5540__INCLUDED_)
