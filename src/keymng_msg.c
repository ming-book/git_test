
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "keymng_msg.h"
#include "itcast_asn1_der.h"
#include "itcastderlog.h"



//
/*
typedef struct _Teacher
{
	char name[64];
	int age;
	char *p;
	int plen;
}Teacher;

typedef struct _AdvTeacher
{
	int tag;
	Teacher *p;
}AdvTeacher;
*/

//
int TeacherEncode_(Teacher *pStruct /*in*/, ITCAST_ANYBUF **pOutNode)
{
	int			ret = 0;
	ITCAST_ANYBUF		*pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF		*pTmpBuf = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;
	unsigned char		*tmpout = NULL;
	int					tmpoutlen = 0;

	//把C字符串转化成ITASN1_PRINTABLESTRING
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pStruct->name, strlen(pStruct->name));
	if (ret != 0)
	{
		printf("func DER_ITCAST_String_To_AnyBuf() err:%d \n", ret);
		return ret;
	}

	//编码name  //name TLV
	ret = DER_ItAsn1_WritePrintableString(pTmp, &pHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		printf("func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	//DER_ITCAST_FreeQueue(pTmp);
	pTmp = pHeadBuf;
	/*
	ret = DER_ItAsn1_WriteInteger(pStruct->age, &pOutData);
	if (ret != 0)
	{
	printf("func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
	return ret;
	}
	pHeadBuf->next = pOutData;
	*/

	//编码 age
	ret = DER_ItAsn1_WriteInteger(pStruct->age, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//编码  p 
	ret = EncodeChar(pStruct->p, pStruct->plen, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func EncodeChar() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//编码plen
	ret = DER_ItAsn1_WriteInteger(pStruct->plen, &(pTmp->next));
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteInteger() err:%d \n", ret);
		return ret;
	}

	//编码大结构体
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadBuf);

	//间接赋值
	*pOutNode = pOutData;
	/*

	//间接赋值
	*out = (unsigned char *)malloc(pOutData->dataLen);
	memcpy(*out, pOutData->pData, pOutData->dataLen);
	*outlen = pOutData->dataLen;
	DER_ITCAST_FreeQueue(pOutData);
	*/
	return ret;
}


//老师解码 api
int TeacherDecode_(unsigned char *out, int outlen, Teacher  **pOutStruct)
{
	int					ret = 0;
	ITCAST_ANYBUF		*pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF		*pTmpDABuf = NULL;
	ITCAST_ANYBUF		*pOutData = NULL;
	ITCAST_ANYBUF		*inAnyBuf = NULL;
	int					tmpNum = 0;
	Teacher				*pTmpStru = NULL;

	//把C的内存块 转成 ITCAST_ANYBUF
	inAnyBuf = (ITCAST_ANYBUF *)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL)
	{
		ret = 101;
		printf("func malloc() err:%d \n ", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF));
	inAnyBuf->next = NULL;  //这个是废话.... 
	inAnyBuf->prev = NULL;

	inAnyBuf->pData = (unsigned char *)malloc(outlen);
	memcpy(inAnyBuf->pData, out, outlen);
	inAnyBuf->dataLen = outlen;

	//解码 解大结构
	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(inAnyBuf);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);

	pTmp = pHead; //两个指针指向链表的头部

	//分配结构体内存
	pTmpStru = (Teacher *)malloc(sizeof(Teacher));
	if (pTmpStru == NULL)
	{
		ret = 102;
		DER_ITCAST_FreeQueue(pHead);
		printf("func malloc() err:%d \n ", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(Teacher));


	//解析name
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}
	memcpy(pTmpStru->name, pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf);

	//解析 age
	ret = DER_ItAsn1_ReadInteger(pTmp, &pTmpStru->age);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//解码 p
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		printf("func DER_ItAsn1_ReadPrintableString() err:%d \n", ret);
		return ret;
	}

	pTmpStru->p = (unsigned char *)malloc(pTmpDABuf->dataLen + 1);
	if (pTmpStru->p == NULL)
	{
		ret = 104;
		DER_ITCAST_FreeQueue(pHead);
		printf("func malloc() err :%d \n", ret);
		return ret;
	}
	memcpy(pTmpStru->p, pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmpStru->p[pTmpDABuf->dataLen] = '\0';
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf);

	//解码plen
	ret = DER_ItAsn1_ReadInteger(pTmp, &pTmpStru->plen);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHead);
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}

	DER_ITCAST_FreeQueue(pHead);

	*pOutStruct = pTmpStru;

	return ret;
}

void TeacherFree2(Teacher **pStruct)
{
	Teacher  *tmpStr = NULL;
	if (pStruct == NULL)
	{
		return;
	}
	tmpStr = *pStruct;
	if (tmpStr->p != NULL)
	{
		free(tmpStr->p);
	}
	free(tmpStr);
	*pStruct = NULL; //
	return;
}




//////////////////////////////////////////////////////////////////////////

int MsgEncode(
	void			*pStruct, /*in*/
	int				type,
	unsigned char	**outData, /*out*/
	int				*outLen)
{
	ITCAST_ANYBUF			*pHeadbuf = NULL, *pTemp = NULL;
	ITCAST_ANYBUF			*pOutData = NULL;
	int				ret = 0;

	if (pStruct == NULL || type < 0 || outData == NULL || outLen == NULL)
	{
		ret = KeyMng_ParamErr;
		printf("func MsgEncode() err:%d check  (pStruct == NULL || type < 0 || outData == NULL || outLen == NULL)\n", ret);
		return ret;
	}

	//编码type
	ret = DER_ItAsn1_WriteInteger(type, &pHeadbuf);
	if (ret != 0)
	{
		printf("func DER_ItAsn1_WriteInteger() err:%d  \n", ret);
		return ret;
	}

	switch (type)
	{
		//编码老师结构体
	case ID_MsgKey_Teacher:
		ret = TeacherEncode_((Teacher *)pStruct /*in*/, &pTemp);
		break;
	case ID_MsgKey_Req:
		//
		break;
	case ID_MsgKey_Res:
		//
		break;
	default:
		ret = KeyMng_TypeErr;
		printf("输入结构体的标签 不支持:itype%d \n", type);
	}

	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("编码失败\n");
		return ret;
	}
	else
	{
		pHeadbuf->next = pTemp;
	}

	//编码大结构体
	ret = DER_ItAsn1_WriteSequence(pHeadbuf, &pOutData);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadbuf);
		printf("func DER_ItAsn1_WriteSequence() err:%d  \n", ret);
		return ret;
	}

	//分配内存 间接赋值
	*outData = (unsigned char *)malloc(pOutData->dataLen);
	if (*outData == NULL)
	{
		ret = KeyMng_MallocErr;
		printf("func malloc() err:%d  \n", ret);
		return ret;
	}
	memcpy(*outData, pOutData->pData, pOutData->dataLen);
	*outLen = pOutData->dataLen;

	DER_ITCAST_FreeQueue(pHeadbuf);
	DER_ITCAST_FreeQueue(pOutData);
	return ret;
}


int MsgDecode(
	unsigned char *inData,/*in*/
	int           inLen,
	void          **pStruct /*out*/,
	int           *type /*out*/)
{
	ITCAST_ANYBUF		*pHeadBuf = NULL, *pTmp = NULL;
	int					ret = 0;
	unsigned long		itype = 0;

	if (inData == NULL || inLen < 0 || pStruct == NULL || type == NULL)
	{
		ret = KeyMng_ParamErr;
		printf("func MsgDecode() err:%d  (inData == NULL || inLen < 0 || pStruct == NULL || type == NULL) \n", ret);
		return ret;
	}

	//把内存数据 转成 AnyBuf
	pTmp = (ITCAST_ANYBUF *)malloc(sizeof(ITCAST_ANYBUF));
	if (pTmp == NULL)
	{
		ret = KeyMng_MallocErr;
		printf("func malloc() err:%d \n", ret);
		return ret;
	}
	memset(pTmp, 0, sizeof(ITCAST_ANYBUF));
	pTmp->pData = (unsigned char *)malloc(inLen);
	if (pTmp->pData == NULL)
	{
		DER_ITCAST_FreeQueue(pTmp);
		ret = KeyMng_MallocErr;
		printf("func malloc() err:%d \n", ret);
		return ret;
	}
	memcpy(pTmp->pData, inData, inLen);
	pTmp->dataLen = inLen;
	
	ret = DER_ItAsn1_ReadSequence(pTmp, &pHeadBuf);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pTmp);
		printf("func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);

	//解码itype
	ret = DER_ItAsn1_ReadInteger(pHeadBuf, &itype);
	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}

	switch (itype)
	{
	case ID_MsgKey_Teacher:
		//老师结构解码
		ret =  TeacherDecode_(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (Teacher  **)pStruct);
		break;
	case ID_MsgKey_Req:
		//请求报文结构解码
		break;
	case ID_MsgKey_Res:
		//应答报文结构解码
		break;
	default:
		ret = KeyMng_TypeErr;
		printf("输入结构体的标签 不支持:itype%d \n", itype);
	}

	if (ret != 0)
	{
		DER_ITCAST_FreeQueue(pHeadBuf);
		printf("解码失败\n");
		return ret;
	}

	
	//给结构体标签 间接赋值
	*type = itype;
	DER_ITCAST_FreeQueue(pHeadBuf);


	return ret;
}



int MsgMemFree(void **point, int type)
{
	int			ret = 0;
	if (point == NULL)
	{
		return 0;
	}

	if (type == 0)
	{
		if (*point != NULL)
		{
			free(*point);
		}
		*point = NULL;
		return 0;
	}


	switch (type)
	{
	case ID_MsgKey_Teacher:
		 TeacherFree2((Teacher **)point);
		break;
	case ID_MsgKey_Req:
		break;
	case ID_MsgKey_Res:
		break;
	default:
		ret = KeyMng_TypeErr;
	}

	return 0;
}