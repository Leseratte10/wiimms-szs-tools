
//
///////////////////////////////////////////////////////////////////////////////
//////   This file is created by a script. Modifications will be lost!   //////
///////////////////////////////////////////////////////////////////////////////


#include "db-dol.h"
#include "lib-staticr.h"

//
///////////////////////////////////////////////////////////////////////////////
/////////////////////////   struct DolSectionMap_t   //////////////////////////
///////////////////////////////////////////////////////////////////////////////

const DolSectionMap_t DolSectionMap[N_DOL_SECTION_MAP] =
{
  // 0
	{ STR_M_JAP, 0, "D3", 0x20,
	  "Section D3 of original MKWii/JAP.",
		{ 0x40,0x65,0xb7,0x71,0x65,0x6e,0xbc,0x7f,0x04,0x1c,
		  0xe3,0x44,0xb5,0x32,0x42,0x0d,0x43,0x05,0x76,0xa8 }},
  // 1
	{ STR_M_KOR, 0, "D3", 0x20,
	  "Section D3 of original MKWii/KOR.",
		{ 0xcd,0x27,0xcf,0xe9,0xce,0x3f,0x28,0xbc,0x64,0xb3,
		  0xb2,0x05,0x70,0x10,0xaf,0x9a,0x5c,0xe6,0x04,0xf7 }},
  // 2
	{ STR_M_USA, 0, "D3", 0x20,
	  "Section D3 of original MKWii/USA.",
		{ 0xe1,0x05,0x64,0x2d,0x86,0x59,0xc9,0xac,0x76,0xde,
		  0x8c,0x84,0xd0,0x67,0xc6,0x9a,0x37,0x2f,0xd5,0xab }},
  // 3
	{ STR_M_PAL, 0, "D3", 0x20,
	  "Section D3 of original MKWii/PAL.",
		{ 0xeb,0x28,0xbc,0x20,0x23,0x0a,0xce,0xa4,0xc9,0xe0,
		  0x6d,0x67,0xee,0x25,0x20,0x1c,0xa1,0x55,0x39,0x7d }},
  // 4
	{ STR_M_USA, 0, "D2", 0xc0,
	  "Section D2 of original MKWii/USA.",
		{ 0x4a,0x87,0x49,0xbd,0xa1,0x78,0x9f,0x8c,0x9c,0x9e,
		  0x25,0x6a,0x6b,0xda,0x19,0x7f,0x7f,0xc7,0xf5,0x7f }},
  // 5
	{ STR_M_KOR, 0, "D2", 0xc0,
	  "Section D2 of original MKWii/KOR.",
		{ 0x7f,0xcd,0xfb,0xdd,0x3e,0x2b,0xf4,0xf2,0x35,0xd0,
		  0x4a,0x57,0xcb,0x9a,0xb0,0xa2,0x2a,0x86,0xf0,0xa5 }},
  // 6
	{ STR_M_JAP, 0, "D2", 0xc0,
	  "Section D2 of original MKWii/JAP.",
		{ 0xb4,0x63,0xae,0x32,0x03,0x5d,0xcc,0xa7,0xb0,0x64,
		  0xee,0x4a,0x1c,0x24,0xd3,0x10,0x84,0xd9,0xf7,0xe1 }},
  // 7
	{ STR_M_PAL, 0, "D2", 0xc0,
	  "Section D2 of original MKWii/PAL.",
		{ 0xf6,0x7f,0x2b,0x81,0x56,0x65,0x9d,0xac,0xd2,0x56,
		  0xb0,0xcd,0x6c,0xca,0xfd,0x72,0x39,0xe5,0x3d,0x2e }},
  // 8
	{ STR_M_PAL, 0, "D8", 0x130,
	  "Section D8 of Revolution.alpha/PAL.",
		{ 0x3c,0x9c,0xba,0x13,0x93,0x33,0x3d,0xee,0x66,0x36,
		  0xed,0x7e,0xb1,0xdf,0xeb,0xf9,0x96,0xf4,0x99,0xba }},
  // 9
	{ STR_M_USA, 0, "D8", 0x130,
	  "Section D8 of Revolution.alpha/USA.",
		{ 0x40,0x37,0x79,0x85,0xab,0xc0,0x6d,0xc2,0xe8,0x6a,
		  0xc6,0xbb,0xbd,0xbf,0x9e,0xb1,0x0e,0xda,0x4b,0x1a }},
  // 10
	{ STR_M_PAL, 0, "D8", 0x130,
	  "Section D8 of CT-CODE 2015-10/PAL.",
		{ 0x79,0x0b,0xe8,0xe8,0x09,0x4e,0x3f,0xf0,0xe3,0xd6,
		  0xd2,0x5c,0x15,0x6e,0x06,0x8c,0x24,0x3c,0x36,0xf5 }},
  // 11
	{ STR_M_PAL, 0, "D8", 0x130,
	  "Section D8 of Revolution.Tock64/PAL.",
		{ 0x9a,0x7e,0x74,0x67,0x74,0xc2,0x9a,0x2b,0x1f,0xda,
		  0x34,0xd3,0x11,0x63,0xe8,0x36,0x12,0x1b,0x98,0xf4 }},
  // 12
	{ STR_M_JAP, 0, "D8", 0x130,
	  "Section D8 of Revolution.alpha/JAP.",
		{ 0x9f,0xa3,0xe3,0xb1,0xa3,0xfc,0xaf,0x8d,0x68,0x5a,
		  0x0b,0x1b,0xa4,0xb2,0x26,0x6b,0x42,0x00,0xeb,0xb2 }},
  // 13
	{ STR_M_JAP, 0, "D8", 0x130,
	  "Section D8 of Revolution.Tock64/JAP.",
		{ 0xa7,0x2a,0x72,0xe6,0x05,0xd8,0x86,0xaf,0xac,0x17,
		  0x84,0x71,0xcf,0x10,0xca,0x27,0x86,0xe5,0x7c,0x63 }},
  // 14
	{ STR_M_JAP, 0, "D8", 0x130,
	  "Section D8 of CT-CODE 2015-10/JAP.",
		{ 0xc8,0xc4,0x66,0x2f,0xd4,0x91,0x30,0xd5,0xbf,0x44,
		  0xf5,0x91,0xf5,0x1d,0x5d,0x18,0xbf,0x0c,0xfb,0xbc }},
  // 15
	{ STR_M_USA, 0, "D8", 0x130,
	  "Section D8 of Revolution.Tock64/USA.",
		{ 0xce,0x00,0xe1,0xca,0xef,0x38,0x2c,0x65,0xc3,0x1d,
		  0x30,0xbc,0x3c,0xf1,0x89,0x0c,0xdd,0xe0,0x1f,0x5a }},
  // 16
	{ STR_M_USA, 0, "D8", 0x130,
	  "Section D8 of CT-CODE 2015-10/USA.",
		{ 0xdf,0xcb,0xe7,0x9d,0xf5,0x04,0x3b,0xac,0xf5,0xc7,
		  0xa8,0xd7,0x3c,0xaa,0xaa,0xfa,0x5f,0x6e,0xe0,0xd2 }},
  // 17
	{ STR_M_PAL, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/PAL (D8 moved).",
		{ 0x38,0xde,0x8a,0x97,0xe3,0x2b,0x73,0x3d,0xb5,0xc9,
		  0xb5,0x6f,0xa4,0x50,0xbe,0xf2,0xd2,0x95,0x97,0x62 }},
  // 18
	{ STR_M_PAL, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/PAL.",
		{ 0x5a,0x45,0x51,0xe2,0x39,0x61,0x27,0x05,0xe4,0xb0,
		  0x1d,0xe9,0x88,0x0f,0x04,0xed,0x75,0xb0,0xfa,0xe4 }},
  // 19
	{ STR_M_JAP, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/JAP (D8 moved).",
		{ 0x5c,0x6d,0x69,0x3a,0x11,0xa0,0xa5,0x27,0x6b,0x1a,
		  0xb1,0xa2,0xce,0xa9,0x32,0xf1,0x3a,0x05,0x20,0x59 }},
  // 20
	{ STR_M_USA, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/USA (D8 moved).",
		{ 0x8f,0xb0,0xb9,0x74,0x8f,0xd6,0xd1,0x06,0x84,0x4e,
		  0xd2,0x50,0xfc,0x9b,0xea,0x7d,0x86,0xb9,0x5b,0x9e }},
  // 21
	{ STR_M_JAP, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/JAP.",
		{ 0x92,0x4a,0x14,0x8b,0x00,0x16,0xd0,0xa0,0x71,0x8b,
		  0x81,0x73,0x98,0x56,0x81,0x38,0x5b,0x40,0x10,0x95 }},
  // 22
	{ STR_M_USA, 0, "T2", 0x2ac,
	  "Section T2 of CT-CODE 2015-10/USA.",
		{ 0xc5,0x0d,0x38,0xd7,0xba,0x4f,0x35,0x9b,0x4f,0x2b,
		  0x22,0xf9,0x34,0x92,0x65,0xd6,0x11,0x5b,0x00,0x47 }},
  // 23
	{ STR_M_USA, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/USA (D8 moved).",
		{ 0x17,0x87,0x71,0x5e,0x56,0x06,0x7c,0xda,0xc2,0x41,
		  0xfa,0xc7,0xdd,0x7d,0xc0,0xef,0x2e,0xed,0x43,0x53 }},
  // 24
	{ STR_M_USA, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/USA.",
		{ 0x2a,0x7b,0xe1,0x25,0xc0,0x7c,0x47,0x33,0x96,0x95,
		  0x23,0x49,0x46,0x4f,0xa0,0xc2,0x33,0x72,0x9b,0x99 }},
  // 25
	{ STR_M_PAL, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/PAL.",
		{ 0x81,0x5d,0x23,0xd7,0xd9,0xbe,0x26,0xf6,0xa3,0x7d,
		  0xd5,0x8b,0x85,0x08,0xeb,0x77,0x4b,0x70,0x77,0xa7 }},
  // 26
	{ STR_M_PAL, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/PAL (D8 moved).",
		{ 0x8e,0xb3,0x50,0x53,0xc4,0xf8,0x28,0x0e,0xe4,0x81,
		  0xae,0xfd,0xed,0x97,0xfd,0x72,0xd8,0xc9,0xbe,0x3c }},
  // 27
	{ STR_M_JAP, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/JAP.",
		{ 0x99,0x71,0xef,0x50,0x08,0x73,0xbf,0xa3,0xc7,0xcb,
		  0x98,0x56,0x59,0x88,0xae,0xbf,0x52,0x52,0x0f,0x1f }},
  // 28
	{ STR_M_JAP, 0, "T2", 0x3a0,
	  "Section T2 of Revolution.alpha/JAP (D8 moved).",
		{ 0xbf,0xb6,0x75,0x1e,0x2a,0x99,0x4d,0x50,0x69,0xa5,
		  0x87,0xef,0x01,0x39,0xa1,0x6b,0x15,0x99,0xd4,0xb7 }},
  // 29
	{ STR_M_KOR, 0, "D0", 0x5c0,
	  "Section D0 of original MKWii/KOR.",
		{ 0x2d,0x62,0xc1,0xdb,0x2a,0x82,0x64,0x0a,0xa7,0x61,
		  0xdd,0x6d,0xfc,0x4a,0x4d,0x59,0xb4,0x51,0x6c,0x27 }},
  // 30
	{ STR_M_USA, 0, "D0", 0x5c0,
	  "Section D0 of original MKWii/USA.",
		{ 0x47,0x6d,0x32,0x6c,0x21,0xaa,0x65,0xf2,0x9e,0x69,
		  0xf9,0xde,0xd2,0xaa,0xb6,0xc2,0x76,0x94,0x19,0x09 }},
  // 31
	{ STR_M_JAP, 0, "D0", 0x5c0,
	  "Section D0 of original MKWii/JAP.",
		{ 0x7f,0xcd,0xe8,0xc8,0x06,0xfb,0x10,0xbc,0x81,0x9c,
		  0xb8,0xbb,0xa0,0x72,0x6a,0xd8,0x75,0xb2,0xb7,0x6a }},
  // 32
	{ STR_M_PAL, 0, "D0", 0x5c0,
	  "Section D0 of original MKWii/PAL.",
		{ 0xc8,0xbf,0xf1,0x5d,0x1d,0x5a,0x05,0x77,0xbc,0xec,
		  0x42,0x05,0xe7,0x11,0x76,0x64,0xa1,0x87,0xfd,0x3d }},
  // 33
	{ STR_M_JAP, 0, "D1", 0x8a0,
	  "Section D1 of original MKWii/JAP.",
		{ 0x5a,0x2c,0x70,0x39,0x11,0x58,0xa0,0x71,0xc0,0xb9,
		  0x09,0xac,0x3e,0xde,0x55,0x64,0xb0,0x70,0xb7,0x52 }},
  // 34
	{ STR_M_PAL, 0, "D1", 0x8a0,
	  "Section D1 of original MKWii/PAL.",
		{ 0x6c,0xe1,0x67,0x91,0x92,0x92,0x44,0x05,0x12,0x12,
		  0x64,0xff,0x57,0x7f,0xae,0x81,0xe5,0xa8,0xf8,0x7a }},
  // 35
	{ STR_M_USA, 0, "D1", 0x8a0,
	  "Section D1 of original MKWii/USA.",
		{ 0x73,0x5b,0x90,0x85,0x58,0x13,0x3f,0x1e,0x56,0x67,
		  0xeb,0x61,0x3a,0xa9,0xe9,0x13,0xc4,0xd3,0xd5,0x6f }},
  // 36
	{ STR_M_KOR, 0, "D1", 0x8a0,
	  "Section D1 of original MKWii/KOR.",
		{ 0x8b,0x99,0x7f,0x81,0xdf,0x8b,0x0e,0x1d,0x96,0x3e,
		  0xbf,0xe7,0xa3,0xb2,0x0e,0x92,0x7d,0xcc,0x5f,0x5b }},
  // 37
	{ STR_M_USA, 0, "D6", 0x13c0,
	  "Section D6 of original MKWii/USA.",
		{ 0x98,0x55,0xd3,0xa2,0x15,0x10,0xe6,0xd4,0x78,0xa8,
		  0x40,0x51,0xe5,0x51,0x77,0x04,0x10,0x18,0x29,0x63 }},
  // 38
	{ STR_M_PAL, 0, "D6", 0x13c0,
	  "Section D6 of original MKWii/PAL.",
		{ 0xaa,0x9a,0x25,0x46,0xb8,0xd9,0x57,0x5e,0x22,0x6d,
		  0x72,0x2f,0x71,0xff,0x86,0x49,0xc6,0x12,0x2d,0xcf }},
  // 39
	{ STR_M_JAP, 0, "D6", 0x13c0,
	  "Section D6 of original MKWii/JAP.",
		{ 0xb7,0x6b,0x13,0x92,0x1c,0xad,0x77,0x78,0x92,0xe8,
		  0xff,0xe4,0x5d,0x45,0xe6,0x1c,0x06,0xb8,0x4d,0xdb }},
  // 40
	{ STR_M_KOR, 0, "D6", 0x13e0,
	  "Section D6 of original MKWii/KOR.",
		{ 0xad,0x8f,0x5f,0x3d,0xf0,0x9d,0xa5,0x26,0x9f,0x71,
		  0xfb,0xeb,0x7b,0x68,0x99,0x72,0x71,0xdc,0x65,0xcc }},
  // 41
	{ STR_M_PAL, 0, "D7", 0x21a0,
	  "Section D7 of original MKWii/PAL.",
		{ 0x43,0x0d,0x6f,0xd3,0xe0,0x12,0x72,0x2f,0x44,0x5b,
		  0x9e,0xdc,0x92,0x1b,0xe0,0x62,0x8e,0x72,0xe8,0x07 }},
  // 42
	{ STR_M_JAP, 0, "D7", 0x21a0,
	  "Section D7 of original MKWii/JAP.",
		{ 0x4d,0xc4,0xf5,0x81,0x3b,0x8b,0x9e,0x5d,0xc3,0x36,
		  0xfb,0x91,0xa9,0xc0,0x5b,0xd7,0x59,0x11,0xee,0x25 }},
  // 43
	{ STR_M_USA, 0, "D7", 0x21a0,
	  "Section D7 of original MKWii/USA.",
		{ 0x9b,0x2c,0x27,0x2b,0x6b,0x43,0xff,0xce,0x11,0x05,
		  0x92,0x29,0x42,0x26,0x6f,0x9c,0x59,0xd6,0x55,0xe8 }},
  // 44
	{ STR_M_KOR, 0, "D7", 0x21a0,
	  "Section D7 of original MKWii/KOR.",
		{ 0xc3,0x04,0xfc,0x7f,0xf8,0x6f,0xf0,0x39,0xa9,0xe7,
		  0xda,0x78,0xf3,0x1b,0xfa,0x41,0x91,0x7d,0xf1,0x3c }},
  // 45
	{ STR_M_USA, 0, "T0", 0x2460,
	  "Section T0 of original MKWii/USA.",
		{ 0x5e,0x16,0x94,0x9f,0x50,0x5f,0xc6,0xc3,0x6e,0xca,
		  0x76,0x5b,0x8a,0x07,0x6e,0x23,0x68,0x77,0x84,0x67 }},
  // 46
	{ STR_M_PAL, 0, "T0", 0x2460,
	  "Section T0 of original MKWii/PAL.",
		{ 0x67,0xa0,0xfd,0x48,0x8f,0x9b,0xe2,0xb8,0xa1,0x35,
		  0xed,0x61,0x48,0xe7,0x41,0x0b,0xb6,0x5b,0xdf,0x2f }},
  // 47
	{ STR_M_KOR, 0, "T0", 0x2460,
	  "Section T0 of original MKWii/KOR.",
		{ 0x8e,0x71,0x21,0xdf,0x02,0x9f,0xff,0x15,0x45,0x5b,
		  0xc9,0x30,0xdd,0xda,0x1a,0x84,0x35,0x41,0xe8,0xc3 }},
  // 48
	{ STR_M_JAP, 0, "T0", 0x2460,
	  "Section T0 of original MKWii/JAP.",
		{ 0xa6,0x86,0x2e,0xbc,0x13,0xdd,0x9e,0x7e,0xfa,0xa9,
		  0x0d,0x49,0x19,0x5f,0x9b,0xe6,0x30,0x19,0x01,0x4e }},
  // 49
	{ STR_M_USA, 0, "T0", 0x2460,
	  "Section T0 of Revolution.alpha/USA.",
		{ 0xce,0x3f,0x05,0xb9,0x3d,0x39,0xa0,0x44,0xea,0x87,
		  0x3c,0xe9,0x1d,0xff,0x8e,0xa8,0x93,0x98,0x05,0xe4 }},
  // 50
	{ STR_M_KOR, 0, "D4", 0x13100,
	  "Section D4 of original MKWii/KOR.",
		{ 0x23,0x8c,0xc9,0x84,0xd1,0x07,0x8a,0x1b,0xc4,0x56,
		  0x1b,0x56,0x30,0xe0,0x1a,0x4d,0xbc,0xa1,0x48,0x0c }},
  // 51
	{ STR_M_JAP, 0, "D4", 0x13120,
	  "Section D4 of original MKWii/JAP.",
		{ 0xba,0x43,0xcc,0x17,0x8b,0x01,0x44,0xc0,0x5b,0x38,
		  0x59,0x9d,0xfc,0x2c,0x14,0xcd,0xa6,0x9b,0xc1,0x8a }},
  // 52
	{ STR_M_USA, 0, "D4", 0x13420,
	  "Section D4 of original MKWii/USA.",
		{ 0xbd,0x62,0x9a,0x5e,0x1b,0xda,0xf5,0x46,0xe8,0x04,
		  0xcb,0xa1,0xfa,0xca,0xac,0x38,0xac,0x7a,0xe6,0xaf }},
  // 53
	{ STR_M_PAL, 0, "D4", 0x136c0,
	  "Section D4 of original MKWii/PAL.",
		{ 0x4f,0xd6,0xcf,0x0f,0x65,0x76,0xc1,0x23,0x96,0x86,
		  0xc8,0x37,0x1b,0x95,0x93,0x9f,0xba,0x72,0xf6,0xc5 }},
  // 54
	{ STR_M_KOR, 0, "D5", 0x39d00,
	  "Section D5 of original MKWii/KOR.",
		{ 0x4a,0x10,0xfb,0x98,0x57,0xb0,0xa5,0x96,0xb7,0xf6,
		  0x92,0x06,0xed,0xf2,0xdf,0x04,0x5e,0x6d,0x63,0x55 }},
  // 55
	{ STR_M_USA, 0, "D5", 0x47aa0,
	  "Section D5 of original MKWii/USA.",
		{ 0x9b,0x08,0x11,0xf2,0xb5,0xed,0x17,0x69,0x7e,0x4b,
		  0xb3,0xef,0xb4,0x65,0x23,0x78,0x21,0x24,0x27,0xc1 }},
  // 56
	{ STR_M_JAP, 0, "D5", 0x4bac0,
	  "Section D5 of original MKWii/JAP.",
		{ 0x16,0x24,0x5d,0xad,0xb9,0x7f,0xe9,0x5f,0x00,0x7f,
		  0xef,0x6f,0xec,0xf6,0x24,0xe5,0x35,0xc1,0x7c,0x38 }},
  // 57
	{ STR_M_PAL, 0, "D5", 0x4bac0,
	  "Section D5 of original MKWii/PAL.",
		{ 0x4a,0x9a,0x76,0x06,0x69,0x35,0x0b,0xd2,0x54,0xd2,
		  0xbf,0x22,0xac,0xd9,0xf9,0xc8,0x13,0xcf,0x95,0xd8 }},
  // 58
	{ STR_M_JAP, 0, "T1", 0x23da40,
	  "Section T1 of original MKWii/JAP.",
		{ 0x93,0xb4,0x6b,0xbe,0xab,0xf5,0x7d,0x98,0x93,0xf2,
		  0x16,0x07,0x4f,0xc0,0x44,0x82,0x01,0xd1,0x42,0x40 }},
  // 59
	{ STR_M_USA, 0, "T1", 0x23da80,
	  "Section T1 of original MKWii/USA.",
		{ 0x3a,0x6d,0x28,0xc3,0xdd,0xc8,0x10,0xcc,0xf0,0xb2,
		  0x7f,0xe7,0xd8,0x7d,0x12,0x8f,0x62,0x72,0xd0,0xed }},
  // 60
	{ STR_M_PAL, 0, "T1", 0x23db20,
	  "Section T1 of original MKWii/PAL.",
		{ 0x1c,0xbe,0x24,0xaf,0x98,0x6a,0x6e,0x3e,0xbe,0xa1,
		  0x5c,0xc9,0x66,0x49,0xce,0xc7,0x7d,0x73,0x75,0x7b }},
  // 61
	{ STR_M_KOR, 0, "T1", 0x23dea0,
	  "Section T1 of original MKWii/KOR.",
		{ 0xfc,0xf6,0x28,0x91,0x85,0x47,0x11,0xc3,0x4e,0xac,
		  0x2e,0x61,0x47,0x53,0x5e,0xe1,0x7c,0x3c,0xb8,0xe6 }},
  // 62
	{ STR_M_KOR, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI, "", 0x2916c0,
	  "Original KOR file with Wiimmfi support.",
		{ 0xd3,0x9e,0x00,0xa6,0xcb,0x15,0x5d,0xbd,0x86,0x60,
		  0xc2,0xd2,0x1e,0x91,0x83,0x29,0x75,0x06,0x42,0x42 }},
  // 63
	{ STR_M_KOR, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_ORIG, "", 0x2916c0,
	  "Original KOR file.",
		{ 0xd8,0x86,0x46,0x75,0x0f,0x67,0x63,0x5e,0x17,0x91,
		  0x1c,0xf9,0x69,0x20,0xfb,0x28,0xc9,0x8e,0x6d,0xfe }},
  // 64
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_ORIG, "", 0x29f340,
	  "Original USA file.",
		{ 0x1b,0x36,0x72,0x09,0x91,0x9f,0xcc,0x22,0x5c,0x74,
		  0xa1,0x55,0xdc,0x46,0xf2,0x9c,0x5d,0x64,0xb9,0xa2 }},
  // 65
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI, "", 0x29f340,
	  "Original USA file with Wiimmfi support.",
		{ 0xb7,0x69,0x39,0x0c,0x2a,0x37,0x20,0x6e,0xad,0x2c,
		  0xcd,0xd5,0x56,0xc8,0x81,0x4c,0x05,0xf2,0x9e,0x36 }},
  // 66
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x29f71c,
	  "Original USA file with Wiimmfi support and CT-CODE (2015-10).",
		{ 0x10,0xa8,0x82,0x21,0xc8,0xbd,0xb2,0x80,0xa5,0x9a,
		  0xa5,0xa1,0x91,0xcc,0x17,0x34,0x08,0xdb,0x32,0x5f }},
  // 67
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x29f71c,
	  "Original USA file with CT-CODE (2015-10, D8 moved).",
		{ 0x7d,0x24,0x63,0xf1,0x27,0x1f,0xee,0x81,0xe1,0xbc,
		  0x01,0xdd,0xf7,0xc7,0x85,0x95,0x88,0x74,0x4c,0x79 }},
  // 68
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x29f71c,
	  "Original USA file with CT-CODE (2015-10).",
		{ 0x81,0x45,0xcb,0xa4,0x67,0x29,0xe2,0x8d,0x59,0xcc,
		  0x45,0xb7,0x68,0x86,0x43,0xb1,0x65,0x54,0xd1,0xe6 }},
  // 69
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x29f71c,
	  "Original USA file with Wiimmfi support and CT-CODE (2015-10, D8 moved).",
		{ 0x8b,0x2f,0xb1,0x69,0xe5,0x3f,0x1e,0xe5,0x4b,0xec,
		  0x16,0x1a,0x31,0xf4,0x19,0x5b,0xfe,0xbd,0xe4,0xe0 }},
  // 70
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x29f810,
	  "Original USA file with Wiimmfi support and CT-CODE (2013-10).",
		{ 0x32,0x29,0xbf,0xf9,0x82,0x8c,0xb5,0x2d,0xbb,0x95,
		  0x6a,0x23,0x3c,0x13,0xa1,0x90,0xae,0xb7,0xc9,0x29 }},
  // 71
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x29f810,
	  "CTGPR USA file with CT-CODE (2013-10).",
		{ 0x3a,0x70,0x4f,0xa6,0xff,0xaa,0x78,0x1d,0xcc,0x17,
		  0x31,0xa4,0xee,0x69,0x4e,0x33,0x39,0x93,0xc1,0x0d }},
  // 72
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x29f810,
	  "Original USA file with Wiimmfi support and CT-CODE (2013-10, D8 moved).",
		{ 0x48,0xde,0x3d,0xc8,0xab,0x44,0xb6,0x51,0x8a,0x20,
		  0xc9,0xd4,0x11,0x90,0x68,0x4c,0xc9,0x90,0xf4,0x3c }},
  // 73
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x29f810,
	  "Original USA file with CT-CODE (2013-10).",
		{ 0x9b,0x05,0x02,0xcd,0x74,0x9b,0xa4,0x2b,0x8d,0x72,
		  0xd2,0xe1,0x05,0x54,0xc7,0xa1,0x88,0xe8,0x65,0x2f }},
  // 74
	{ STR_M_USA, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x29f810,
	  "Original USA file with CT-CODE (2013-10, D8 moved).",
		{ 0xd4,0xcd,0x5f,0x41,0x71,0xb7,0x39,0xb1,0x68,0x48,
		  0xff,0xae,0xe9,0x5a,0x16,0x4c,0xa9,0x56,0x30,0x05 }},
  // 75
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_ORIG, "", 0x2a3020,
	  "Original JAP file.",
		{ 0x3b,0xf9,0xbe,0x2e,0x65,0x0e,0xae,0xe2,0xda,0xb3,
		  0xb3,0xde,0xbc,0x2d,0xc6,0xdf,0x7f,0x84,0x0c,0x47 }},
  // 76
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI, "", 0x2a3020,
	  "Original JAP file with Wiimmfi support.",
		{ 0x98,0x64,0xc3,0xb5,0x5f,0xeb,0x5a,0x47,0xb4,0xdf,
		  0xba,0x8d,0xfc,0x61,0x72,0x4b,0xe0,0x0c,0xd4,0x9f }},
  // 77
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a33fc,
	  "Original JAP file with CT-CODE (2015-10).",
		{ 0x0f,0x35,0x0f,0x5d,0xd1,0x86,0x97,0xb6,0xff,0xc5,
		  0x26,0xee,0x08,0x57,0x88,0xf8,0x6d,0x1a,0x11,0x0e }},
  // 78
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a33fc,
	  "Original JAP file with Wiimmfi support and CT-CODE (2015-10, D8 moved).",
		{ 0x87,0x91,0x52,0xcf,0x0b,0xb8,0x72,0x0b,0xaf,0x37,
		  0xf3,0xe0,0xa8,0x9d,0x50,0x26,0x06,0x25,0x54,0x94 }},
  // 79
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a33fc,
	  "Original JAP file with CT-CODE (2015-10, D8 moved).",
		{ 0xe4,0xbd,0x64,0x34,0xf8,0x4c,0x2f,0x6a,0x74,0xa7,
		  0xab,0x06,0x46,0x07,0x49,0xea,0x91,0x52,0x13,0x42 }},
  // 80
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a33fc,
	  "Original JAP file with Wiimmfi support and CT-CODE (2015-10).",
		{ 0xec,0x12,0xe9,0x5e,0x0a,0x23,0x3d,0x8a,0xeb,0x8a,
		  0xb4,0x2e,0xc6,0x5f,0x8c,0x4a,0x56,0x1d,0x05,0x4f }},
  // 81
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a34f0,
	  "Original JAP file with CT-CODE (2013-10).",
		{ 0x29,0x3c,0x59,0xbf,0x2c,0xd2,0x2f,0xcc,0x3f,0x2c,
		  0x0b,0x8f,0xc7,0xc2,0xcd,0x85,0xb3,0xfc,0x5e,0x88 }},
  // 82
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a34f0,
	  "Original JAP file with CT-CODE (2013-10, D8 moved).",
		{ 0xa3,0xfb,0xc3,0x4a,0xf7,0x6e,0x4c,0x65,0x7d,0xa7,
		  0x31,0x5e,0xb3,0xc4,0x42,0x86,0xef,0x0b,0x62,0x3a }},
  // 83
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a34f0,
	  "Original JAP file with Wiimmfi support and CT-CODE (2013-10).",
		{ 0xaa,0x63,0xac,0x04,0x39,0xaf,0xe5,0xa6,0xaf,0xf1,
		  0x36,0xb8,0x93,0x00,0x30,0x25,0x37,0x8f,0x8e,0x8b }},
  // 84
	{ STR_M_JAP, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a34f0,
	  "Original JAP file with Wiimmfi support and CT-CODE (2013-10, D8 moved).",
		{ 0xbe,0x31,0xc1,0x67,0x29,0xdc,0x47,0xdc,0x9f,0x66,
		  0x1f,0xf7,0x56,0x11,0x33,0x32,0x00,0xc2,0x44,0x14 }},
  // 85
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI, "", 0x2a36a0,
	  "Original PAL file with Wiimmfi support.",
		{ 0xa5,0x5b,0x4e,0xfa,0x05,0x86,0xfa,0x9d,0xbd,0xe3,
		  0xc9,0xc4,0x86,0x54,0xef,0x7a,0xfd,0xde,0xf1,0x95 }},
  // 86
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_ORIG, "", 0x2a36a0,
	  "Original PAL file.",
		{ 0xac,0x7d,0x72,0x44,0x86,0x30,0xad,0xe7,0x65,0x5f,
		  0xc8,0xbc,0x5f,0xd7,0xa6,0x54,0x3c,0xb5,0x3a,0x49 }},
  // 87
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a3a7c,
	  "Original PAL file with Wiimmfi support and CT-CODE (2015-10, D8 moved).",
		{ 0x94,0x66,0xc7,0x26,0xa4,0x91,0x58,0xe1,0xc0,0xd5,
		  0x20,0x6f,0xf4,0xde,0x17,0x7c,0xc6,0xe0,0x3f,0x13 }},
  // 88
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a3a7c,
	  "Original PAL file with Wiimmfi support and CT-CODE (2015-10).",
		{ 0xad,0x41,0x54,0x79,0xc5,0xda,0x9c,0x0d,0x2f,0x45,
		  0xed,0xf4,0xfd,0x7b,0xe3,0x22,0xd8,0xad,0x6b,0xed }},
  // 89
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a3a7c,
	  "Original PAL file with CT-CODE (2015-10, D8 moved).",
		{ 0xd4,0x55,0x35,0x2e,0x5f,0x8a,0xf0,0xfc,0xb4,0x51,
		  0x98,0x77,0xe6,0x74,0x29,0xbb,0xd5,0x75,0xcf,0x11 }},
  // 90
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a3a7c,
	  "Original PAL file with CT-CODE (2015-10).",
		{ 0xec,0xc2,0x1a,0xd8,0x82,0x4f,0x56,0x16,0x5f,0xa4,
		  0x20,0xc1,0x52,0xc5,0x3e,0xaa,0x98,0x85,0x8b,0x19 }},
  // 91
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a3b70,
	  "Original PAL file with Wiimmfi support and CT-CODE (2013-10).",
		{ 0x2d,0x07,0x07,0xdc,0x84,0x1a,0x33,0x72,0x93,0xe0,
		  0x33,0x4f,0x5f,0x9a,0xb3,0x44,0x07,0xd7,0xd5,0x62 }},
  // 92
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_WIIMMFI|DOL_S_CTCODE, "", 0x2a3b70,
	  "Original PAL file with Wiimmfi support and CT-CODE (2013-10, D8 moved).",
		{ 0x2d,0x6c,0x00,0x55,0x70,0x43,0x45,0x5c,0xc9,0x8d,
		  0x1b,0x4d,0x2b,0x13,0x48,0x43,0x30,0x7d,0x93,0xb9 }},
  // 93
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a3b70,
	  "Original PAL file with CT-CODE (2013-10).",
		{ 0x32,0xf8,0xf1,0xa1,0xa3,0x45,0xf9,0x5d,0x4f,0x0c,
		  0x33,0x6b,0x7c,0x4f,0x6d,0xfd,0xda,0x48,0x5c,0x4a }},
  // 94
	{ STR_M_PAL, DOL_S_ANALYZED|DOL_S_KNOWN|DOL_S_CTCODE, "", 0x2a3b70,
	  "Original PAL file with CT-CODE (2013-10, D8 moved).",
		{ 0xfa,0x52,0x60,0x34,0x4a,0x82,0x6b,0x30,0x56,0x36,
		  0x29,0xda,0x5f,0x67,0x9b,0xe5,0xa2,0xbe,0x87,0xdd }},
};

//
///////////////////////////////////////////////////////////////////////////////
/////////////////////////   struct DolAddressMap_t   //////////////////////////
///////////////////////////////////////////////////////////////////////////////


const DolAddressMap_t DolAddressMapPAL =
{
	0x800060a4, // entry_point
	0x802a4080, // bss_addr
	   0xe50fc, // bss_size
	0x802a4080, // shared_bss_addr
	   0xceb80, // shared_bss_size
	0x80006210, // f_setup_register
};

const DolAddressMap_t DolAddressMapUSA =
{
	0x800060a4, // entry_point
	0x8029fd00, // bss_addr
	   0xe50fc, // bss_size
	0x802a4080, // shared_bss_addr
	   0xceb80, // shared_bss_size
	0x80006210, // f_setup_register
};

const DolAddressMap_t DolAddressMapJAP =
{
	0x800060a4, // entry_point
	0x802a3a00, // bss_addr
	   0xe50fc, // bss_size
	0x802a4080, // shared_bss_addr
	   0xceb80, // shared_bss_size
	0x80006210, // f_setup_register
};

const DolAddressMap_t DolAddressMapKOR =
{
	0x800060a4, // entry_point
	0x80292080, // bss_addr
	   0xe511c, // bss_size
	0x802a4080, // shared_bss_addr
	   0xceb80, // shared_bss_size
	0x80006210, // f_setup_register
};

//
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////   E N D   //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

