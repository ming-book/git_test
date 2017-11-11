#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "keymng_msg.h"
int mainxxx()
{
	int				ret = 0;
	MsgKey_Req		msgKeyReq;

	unsigned char	*outData = NULL;
	int				outLen = 0;
	MsgKey_Req		*pMsgKeyReq = NULL;
	int				iMsgKeyReqType = 0;

	memset(&msgKeyReq, 0, sizeof(MsgKey_Req));

	msgKeyReq.cmdType = KeyMng_NEWorUPDATE;
	strcpy(msgKeyReq.clientId, "1111");
	strcpy(msgKeyReq.AuthCode, "1111");
	strcpy(msgKeyReq.serverId, "0001");
	strcpy(msgKeyReq.r1, "1111");

	ret = MsgEncode(&msgKeyReq, ID_MsgKey_Req, &outData, &outLen);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}
	

	ret = MsgDecode(outData, outLen, (void **)&pMsgKeyReq, &iMsgKeyReqType);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}
		
	MsgMemFree((void **)&outData, 0); 
	MsgMemFree((void **)&pMsgKeyReq, iMsgKeyReqType);

	printf("hello...\n");
	system("pause");
	return 0;
}

int mywritefile3(unsigned char *buf, int len)
{
	FILE *fp = NULL;
	fp = fopen("c:/teacher.ber", "wb+");
	if (fp == NULL)
	{
		printf("fopen file error \n");
		return -1;
	}
	fwrite(buf, 1, len, fp);
	fclose(fp);
	return 0;
}



int main()
{
	Teacher			t1;
	int				ret = 0; //给变量赋值 意思是说: 给变量所代表的内存空间赋值

	unsigned char	*myout = NULL; //0xdd11
	int				outlen = 0;
	Teacher			*outStruct = NULL;
	int				iTeacherType = 0;

	//结构体赋值 
	memset(&t1, 0, sizeof(Teacher));
	t1.age = 32;
	strcpy(t1.name, "myname");
	//strcpy(t1.p, "mynameaaaff");
	t1.p = (char *)malloc(100);
	strncpy(t1.p, "dsafdsafdsafdsafdsa", 9);
	t1.plen = 9;
	t1.p[t1.plen] = '\0';

	ret = MsgEncode(&t1, ID_MsgKey_Teacher, &myout, &outlen);
	if (ret != 0)
	{
		printf("func MsgEncode() err:%d \n", ret);
		return ret;
	}

	//mywritefile3(myout, outlen);

	ret = MsgDecode(myout, outlen, (void **)&outStruct, &iTeacherType);
	if (ret != 0)
	{
		printf("func MsgDecode() err:%d \n", ret);
		return ret;
	}

	//校验
	if (strcmp(t1.p, outStruct->p) == 0 &&
		t1.age == outStruct->age)
	{
		printf("\n111111111 ok\n");
		printf("编码 和 解码 结果一致  ok\n");
		printf("\n 2222222 ok\n");
	}
	else
	{
		printf("编码 和 解码 结果不一致  err\n");
	}

	free(myout); //

	//MsgMemFree((void **)&myout, 0);
	MsgMemFree((void **)&outStruct, iTeacherType);

	//system("pause");
	return 0;
}