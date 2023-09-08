#pragma once
/*********************************************************************
*
* Device     : RX
*
* File Name  : typedefine.h
*
* Abstract   : Aliases of Integer Type.
*
* History    : 1.00  (2009-08-07)
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2009 Renesas Electronics Corporation.
* and Renesas Solutions Corporation. All rights reserved.
*
*********************************************************************/

// ���d�R���p�C���}�~
#ifndef _TYPEDEF_DEFINE_H
#define _TYPEDEF_DEFINE_H

/* -------------- */
/*  �ȉ��Ǝ���`  */
/* -------------- */
/* ON/OFF */
#define ON					(1)
#define OFF					(0)

/* Hi/Lo */
#define HI					(1)
#define LO					(0)

/* �X�R�[�v�ݒ� */
#define PUBLIC
#define PROTECTED
#define	PRIVATE				static

/* BOOL�^ */
#define	TRUE			(1)
#define	FALSE			(0)
//typedef	unsigned char	BOOL;
//typedef	BOOL* PBOOL;

/* const�Astatic�Avolatile */
#define CONST				const
#define STATIC				static
#define VOLATILE			volatile

/* �^��` */
typedef unsigned char		UCHAR;
typedef char				CHAR;
typedef unsigned short		USHORT;
typedef short				SHORT;
typedef unsigned int        UINT;
typedef int                 INT;
//typedef unsigned long		ULONG;
//typedef long				LONG;
typedef float				FLOAT;
typedef double				DOUBLE;
typedef UCHAR* PUCHAR;
typedef USHORT* PUSHORT;
typedef ULONG* PULONG;
typedef CHAR* PCHAR;
typedef SHORT* PSHORT;
typedef LONG* PLONG;
typedef FLOAT* PFLOAT;
typedef DOUBLE* PDOUBLE;
typedef CHAR				STR;			// ������͂��̒�`���g�p
typedef CONST CHAR			CSTR;			// ������͂��̒�`���g�p
typedef CHAR* PSTR;			// ������͂��̒�`���g�p
typedef CONST CHAR* PCSTR;			// PSTR��ROM�Ɋi�[����ꍇ�͂�������g�p
typedef volatile CHAR		VCHAR;
typedef volatile UCHAR		VUCHAR;
typedef volatile SHORT		VSHORT;
typedef volatile USHORT		VUSHORT;
typedef volatile LONG		VLONG;
typedef volatile ULONG		VULONG;
typedef volatile FLOAT		VFLOAT;
typedef void* PVOID;

typedef unsigned char   uint8_t;
typedef char            int8_t;
typedef unsigned short  uint16_t;
typedef short           int16_t;
typedef unsigned int    uint16_t;
typedef int             int16_t;
typedef unsigned long   uint32_t;
typedef long            int32_t;


/* BOOL�^ */
#define	TRUE			(1)
#define	FALSE			(0)

/* �}�N�� */
#define FABS(x)						( (x)>=0 ? (x) : -(x) )


/* ------------- */
/*  Renesas��`  */
/* ------------- */
typedef signed char _SBYTE;
typedef unsigned char _UBYTE;
typedef signed short _SWORD;
typedef unsigned short _UWORD;
typedef signed int _SINT;
typedef unsigned int _UINT;
typedef signed long _SDWORD;
typedef unsigned long _UDWORD;
typedef signed long long _SQWORD;
typedef unsigned long long _UQWORD;


#endif	// _TYPEDEF_DEFINE_H