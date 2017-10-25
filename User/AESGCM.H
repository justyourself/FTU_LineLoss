//
//int Decrypt_ByteData(unsigned char* pKey/*密钥*/,unsigned char* pIV /*初始化向量*/,unsigned char* pHDR/*头-附加数据*/,int nHdrLen,unsigned char* pCiphertext /*密文*/,int *nCtextLen,unsigned char* pTag /*认证识别码*/,unsigned char* pOutPlaintext/*明文*/);
//int Encrypt_ByteData(unsigned char* pKey/*密钥*/,int nKeyLen,unsigned char* pIV/*初始化向量*/,int nIVLen,unsigned char* pHDR,int nHdrLen,unsigned char* pPlaintext/*明文*/,int nPtextLen,unsigned char* pOutCiphertext/*密文*/,unsigned char* pOutTag/*认证识别码*/);

int Decrypt_OR_Encrypt(unsigned short COM_No,unsigned char* Ciphertext,unsigned short *nCtextLen,unsigned char use_crypt);   //判断解密类型，做类型标识输出加解密密文,use_crypt:0--输入（判控制字判断是否需要解密），1--加密输出..
int Decrypt_ByteData(unsigned char* key/*密钥*/,unsigned char* iv /*初始化向量*/,unsigned char* hdr/*头-附加数据*/,int hdr_len,unsigned char* ctx /*密文*/,int ctx_len,unsigned char* pTag /*认证识别码*/,unsigned char* pOutPlaintext/*明文*/);
int Encrypt_ByteData(unsigned char* key/*密钥*/,int key_len,unsigned char* iv/*初始化向量*/,int iv_len,unsigned char* hdr,int hdr_len,unsigned char* ptx/*明文*/,int ptx_len,unsigned char* pOutCiphertext/*密文*/,unsigned char* pOutTag/*认证识别码*/);
