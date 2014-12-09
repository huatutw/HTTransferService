#pragma once
#ifndef DCFILE_RC4
#define DCFILE_RC4
#define swap_byte(x,y) t = *(x); *(x) = *(y); *(y) = t
typedef struct rc4_key
{      
	unsigned char state[256];       
	unsigned char x;        
	unsigned char y;
} rc4_key;
#endif


class DCCrypt
{
public:
	DCCrypt(void);
	~DCCrypt(void);
	BOOL Encrypt_RC4(LPBYTE lpszBuffer,DWORD dwSize,char* lpszPassword);
	void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
	void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key);
};

//--------------”√∑®:-----------------------------------------------------------------------------
//LPBYTE bt = (LPBYTE)calloc(sizeof(struct),sizeof(BYTE));	
//fread(bt,sizeof(struct),1,fp);
//DGCommonFunc::Encrypt_RC4(bt,sizeof(struct),"√‹‘ø");
//memcpy(pInfo,bt,sizeof(struct));
//fclose(fp);
//free(bt);//