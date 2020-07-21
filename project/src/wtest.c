
/***************************************************************************
 *                         _______ _______ _______                         *
 *                        |  ___  |____   |  ___  |                        *
 *                        | |   |_|    / /| |   |_|                        *
 *                        | |_____    / / | |_____                         *
 *                        |_____  |  / /  |_____  |                        *
 *                         _    | | / /    _    | |                        *
 *                        | |___| |/ /____| |___| |                        *
 *                        |_______|_______|_______|                        *
 *                                                                         *
 *                            Wiimms SZS Tools                             *
 *                          https://szs.wiimm.de/                          *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This file is part of the SZS project.                                 *
 *   Visit https://szs.wiimm.de/ for project details and sources.          *
 *                                                                         *
 *   Copyright (c) 2011-2020 by Dirk Clemens <wiimm@wiimm.de>              *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   See file gpl-2.0.txt or http://www.gnu.org/licenses/gpl-2.0.txt       *
 *                                                                         *
 ***************************************************************************/

#define _GNU_SOURCE 1

#include <math.h>
#include <dirent.h>
#include <stddef.h>
#include <time.h>
//#include <sys/stat.h>
//#include <fcntl.h>

#include "dclib-debug.h"
#include "version.h"
#include "lib-std.h"
#include "lib-image.h"
#include "lib-object.h"
#include "lib-kmp.h"
#include "lib-brres.h"
#include "lib-mdl.h"
#include "lib-bzip2.h"
#include "lib-mkw.h"
#include "lib-bmg.h"
#include "lib-lecode.h"
#include "lib-common.h"

#define CMD1_FW 13

#if HAVE_PCRE || 1
  #include "dclib-regex.h"
#endif

#ifdef HAVE_WIIMM_EXT
  #include "wtest+.c"
#endif

///////////////////////////////////////////////////////////////////////////////

#define NAME "wtest"
#undef TITLE
#define TITLE NAME " v" VERSION " r" REVISION " " SYSTEM " - " AUTHOR " - " DATE

//
///////////////////////////////////////////////////////////////////////////////
///////////////			helpers				///////////////
///////////////////////////////////////////////////////////////////////////////

static void print_title ( FILE * f )
{
    static bool done = false;
    if (!done)
    {
	done = true;
	if ( verbose >= 1 && f == stdout )
	    fprintf(f,"\n%s\n\n",TITLE);
	else
	    fprintf(f,"*****  %s  *****\n",TITLE);
    }
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_float()			///////////////
///////////////////////////////////////////////////////////////////////////////

static void test_float ( int argc, char ** argv )
{
    uint i;
    for ( i = 1; i < argc; i++ )
    {
	char buf[5];
	memset(buf,0,sizeof(buf));
	strncpy(buf,argv[i],4);
	float f = bef4(buf);
	printf(" |%s| %11.8f %11.8e %11.8a |%.4s|\n",buf,f,f,f,(ccp)&f);
    }
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			create_float_tab()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError create_float_tab()
{
    union { float f; u32 i; } u;
    uint i;
    for ( i = 1; i <= 100; i += 5 )
    {
	uint j;
	for ( j = i; j <i+5; j++ )
	{
	    u.f = j;
	    printf("0x%8x, ",u.i);
	}
	printf("  // %2u .. %2u\n",i,i+4);
    }

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_matrix()			///////////////
///////////////////////////////////////////////////////////////////////////////

static inline double GetRandomD ( double range )
{
    return ( MyRandom(200000001) * 1e-8 - 1.0 ) * range;
}

static double3 GetRandomD3 ( double range )
{
    double3 res;
    res.x = GetRandomD(range);
    res.y = GetRandomD(range);
    res.z = GetRandomD(range);
    return res;
}

static double3 GetRandomD3nn ( double range ) // 'nn' : not null
{
    double3 res;
    res.x = GetRandomD(range); if ( fabs(res.x) < 1e-2 ) res.x = 1.0;
    res.y = GetRandomD(range); if ( fabs(res.y) < 1e-2 ) res.y = 1.0;
    res.z = GetRandomD(range); if ( fabs(res.z) < 1e-2 ) res.z = 1.0;
    return res;
}

///////////////////////////////////////////////////////////////////////////////

static int test_matrix ( int argc, char ** argv )
{
    //--- scan param

    uint n_pass = 10;
    bool verbose = false;

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	if (!strcmp(argv[i],"-v"))
	    verbose = true;
	else if ( argv[i][0] != '-' )
	{
	    uint num = strtoul(argv[1],0,10);
	    if ( num > 0 )
		n_pass = num < 1000000000 ? num : 1000000000;
	}
    }


    //--- setup test vectors

    PATCH_FILE_MODE = 0; // disable logging

    double3 d = GetRandomD3(15000);
    double dtab[] = { -10000, -5000, -10000/3.0, 0, 10000.0/3.0,
			5000, 10000, d.x, d.y, d.z };
    #undef  N_NUM
    #define N_NUM (sizeof(dtab)/sizeof(*dtab))
    #define N_VEC (N_NUM*N_NUM*N_NUM)
    double3 vtab[N_VEC], *vptr;

    printf("n(numbers)=%zu, n(vectors)=%zu, n(pass)=%u\n",N_NUM,N_VEC,n_pass);

    uint x, y, z;
    vptr = vtab;
    for ( x = 0; x < N_NUM; x++ )
    {
	printf(" num[%u] = %11.3f\n",x,dtab[x]);
	for ( y = 0; y < N_NUM; y++ )
	{
	    for ( z = 0; z < N_NUM; z++ )
	    {
		DASSERT( vptr < vtab+N_VEC );
		vptr->x = dtab[x];
		vptr->y = dtab[y];
		vptr->z = dtab[z];
		vptr++;
	    }
	}
    }
    fflush(stdout);
    DASSERT( vptr == vtab+N_VEC );

    //---

    uint pass, err_count = 0, total_count = 0;
    for ( pass = 0; pass < n_pass; pass++ )
    {
	ResetTransformation();
	if (pass)
	{
	    // random setup
	    double3 val, origin;

	    val    = GetRandomD3nn(10.0);
	    origin = GetRandomD3(1e5);
	    SetScaleMatrixD(&opt_transform,&val,&origin);

	    val = GetRandomD3(1e5);
	    SetShiftMatrixD(&opt_transform,&val);

	    val = GetRandomD3(1e5);
	    SetTranslateMatrixD(&opt_transform,&val);

	    val = GetRandomD3(1000.0);
	    uint xyz;
	    for ( xyz = 0; xyz < 3; xyz++ )
	    {
		origin = GetRandomD3(1e4);
		SetRotateMatrixD(&opt_transform,xyz,val.v[xyz],0.0,&origin);
	    }
	}
	CloseTransformation();
	global_check_mode = opt_no_check
			? 0
			: GetCheckMode(false,brief_count>0,!verbose,long_count>0);

	if (verbose)
	{
	    char buf[60];
	    uint len = snprintf(buf,sizeof(buf),"#####  PASS %u/%u [err=%u]  #####",
			pass+1 ,n_pass, err_count );
	    printf("\n%.*s\n%s\n%.*s\n",len,Hash200,buf,len,Hash200);
	    PrintTestMatrix(true);
	}
	else if ( pass % 1000 == 999 )
	    printf("##### PASS %u/%u [err=%u] #####\n",pass+1,n_pass,err_count);
	fflush(stdout);

	int i, fail = -1;
	for ( i = 0; i < N_VEC; i++ )
	{
	    total_count++;
	    if (!TestMatrix(vtab+i,1,true))
	    {
		err_count++;
		fail = i;
	    }
	}

	if ( fail >= 0 )
	{
	    MatrixD_t *m = &opt_transform;
	    printf("\n\n"
		" --scale %5.3f,%5.3f,%5.3f@%5.3f,%5.3f,%5.3f"
		" --shift %5.3f,%5.3f,%5.3f"
		" --xrot %5.3f@%5.3f,%5.3f,%5.3f"
		" --yrot %5.3f@%5.3f,%5.3f,%5.3f"
		" --zrot %5.3f@%5.3f,%5.3f,%5.3f"
		" --translate %5.3f,%5.3f,%5.3f"
		" %5.3f,%5.3f,%5.3f"
		"\n\n",
		m->scale.x, m->scale.y, m->scale.z,
		m->scale_origin.x, m->scale_origin.y, m->scale_origin.z,
		m->shift.x, m->shift.y, m->shift.z,
		m->rotate_deg.v[0], m->rotate_origin[0].x,
			m->rotate_origin[0].y, m->rotate_origin[0].z,
		m->rotate_deg.v[1], m->rotate_origin[1].x,
			m->rotate_origin[1].y, m->rotate_origin[1].z,
		m->rotate_deg.v[2], m->rotate_origin[2].x,
			m->rotate_origin[2].y, m->rotate_origin[2].z,
		m->translate.x, m->translate.y, m->translate.z,
		vtab[fail].x, vtab[fail].y, vtab[fail].z );
	}
    }

    printf("\nerr=%u, total=%u\n",err_count,total_count);
    if ( err_count && max_error < ERR_DIFFER )
	max_error = ERR_DIFFER;
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_match_pattern()		///////////////
///////////////////////////////////////////////////////////////////////////////

static void test_match_pattern ( int argc, char ** argv )
{
    int i, j, i_end = argc, j_start = 1;
    for ( i = 1; i < argc; i++ )
	if (!strcmp(argv[i],"-"))
	{
	    i_end   = i;
	    j_start = i + 1;
	    break;
	}

    for ( i = 1; i < i_end; i++ )
	for ( j = j_start; j < argc; j++ )
	    printf("%u |%s|%s|\n",MatchPattern(argv[i],argv[j],'/'),argv[i],argv[j]);
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_median_cut()		///////////////
///////////////////////////////////////////////////////////////////////////////

static void call_mc ( const u32 * tab, uint ntab, uint max_pal )
{
    u32 pal[0x200];
    u16 *res = (u16*)iobuf;

    ASSERT( ntab <= sizeof(iobuf)/sizeof(*tab) );
    ASSERT( max_pal <= sizeof(pal)/sizeof(*pal) );

    u64 usec = GetTimerUSec();
    const uint npal = MedianCut(tab,res,ntab,0,1,pal,max_pal);
    usec = GetTimerUSec() - usec;

    printf("\n*** %u palettes calculated in %llu usec ***",npal,usec);

    uint i;
    for ( i = 0; i < npal; i++ )
	printf("%c%08x", i&7 ? ' ' : '\n', pal[i] );

    for ( i = 0; i < ntab; i++ )
	printf("%s%02x", !(i&15) ? "\n" : i&3 ? " " : "  ", res[i] );
    putchar('\n');
}

#define MC_CALL(t,n) call_mc(t,sizeof(t)/sizeof(u32),n);

///////////////////////////////////////////////////////////////////////////////

static u32 mctab_4[] = { 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff };

///////////////////////////////////////////////////////////////////////////////

static enumError test_median_cut ( int argc, char ** argv )
{
    uint done_count = 0, wrong_arg_count = 0;

    int i;
    for ( i = 1; i < argc; i++ )
    {
	char *end;
	uint num = strtoul(argv[i],&end,10);
	if ( *end || end == argv[i] )
	    num = 0;

	done_count++;
	switch(num)
	{
	    case  1: MC_CALL(mctab_4,256); break;
	    case  2: MC_CALL(mctab_4,2); break;
	    case  3: MC_CALL(mctab_4,3); break;

	    default:
		printf("Syntax error: %s\n",argv[i]);
		wrong_arg_count++;
		break;
	}
    }

    if ( !done_count || wrong_arg_count )
	fprintf(stderr,
		"\n"
		"usage: %s median-cut mode...\n"
		" 1: 4 -> 256\n"
		" 2: 4 -> 2\n"
		" 3: 4 -> 3\n"
		"\n",
		NAME );

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_cmpr()			///////////////
///////////////////////////////////////////////////////////////////////////////

static uint calc_distance ( const u8 * v1, const u8 * v2 )
{
    const int d0 = (int)*v1++ - (int)*v2++;
    const int d1 = (int)*v1++ - (int)*v2++;
    const int d2 = (int)*v1++ - (int)*v2++;
    return abs(d0) + abs(d1) + abs(d2);
}

//-----------------------------------------------------------------------------

static uint calc_distance_square ( const u8 * v1, const u8 * v2 )
{
    const int d0 = (int)*v1++ - (int)*v2++;
    const int d1 = (int)*v1++ - (int)*v2++;
    const int d2 = (int)*v1++ - (int)*v2++;
    return d0*d0 + d1*d1 + d2*d2;
}

///////////////////////////////////////////////////////////////////////////////

void CMPR_distance
(
    const u8		*data,		// source data
    cmpr_info_t		*info,		// info data structure
    bool		square_dist	// true: use square distance
)
{
    DASSERT(info);
    memset(info,0,sizeof(*info));

    uint (*dist_func) ( const u8 * v1, const u8 * v2 );
    if ( square_dist)
    {
	dist_func = calc_distance_square;
	info->name = "Dist^2";
    }
    else
    {
	dist_func = calc_distance;
	info->name = "Dist";
    }

    //--- analyze data (algorithm adapted from Chadderz CTools)

    uint opaque_count = 0;
    int max_dist = -1;

    const u8 *data_end = data + CMPR_DATA_SIZE;
    const u8 *pal0 = data; // palette entry #0
    const u8 *pal1 = data; // palette entry #1

    const u8 *d1;
    for ( d1 = data; d1 < data_end; d1 += 4 )
    {
	if ( d1[3] & 0x80 )
	{
	    if (!opaque_count++)
		pal0 = pal1 = data;
	    const u8 *d2;
	    for ( d2 = d1+4; d2 < data_end; d2 += 4 )
	    {
		if ( d2[3] & 0x80 )
		{
		    const int dist = dist_func(d1,d2);
		    if ( max_dist < dist )
		    {
			 max_dist = dist;
			 pal0 = d1;
			 pal1 = d2;
		    }
		}
	    }
	}
    }


    //--- calculate and assign values

    info->opaque_count = opaque_count;
    if (opaque_count)
    {
	memcpy(info->p[0],pal0,4);
	memcpy(info->p[1],pal1,4);
    }
}

///////////////////////////////////////////////////////////////////////////////

static ccp print_col ( const u8 * val )
{
    if ( val[3] < 0x80 )
	return "-transp-";

    static char buf[10];
    snprintf(buf,sizeof(buf),"%02x-%02x-%02x",val[0],val[1],val[2]);
    return buf;
}

///////////////////////////////////////////////////////////////////////////////

static enumError test_cmpr ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
	fprintf(stderr,
		"\n"
		"usage: %s cmpr [color]...\n"
		" color: 6 hexdigit color, each channel 00..ff\n"
		"        or the keyword 't' -> transparent pixel\n"
		"\n",
		NAME );
	return ERR_OK;
    }


    //--- scan params

    u8 vector[CMPR_MAX_COL][4];
    memset(vector,0,sizeof(vector));
    uint n_vector = 0;

    int i;
    for ( i = 1; i < argc && n_vector < CMPR_MAX_COL; i++ )
    {
	ccp arg = argv[i];
	if ( *arg == 't' || *arg == 'T' )
	    n_vector++;
	else
	{
	    uint num = strtoul(arg,0,16);
	    if ( num < 0x1000000 && n_vector < CMPR_MAX_COL )
	    {
		u8 *v = vector[n_vector++];
		*v++ = num >> 16;
		*v++ = num >>  8;
		*v++ = num;
		*v++ = 0xff;
	    }
	}
    }

    if ( n_vector > 0 )
	while ( n_vector < CMPR_MAX_COL )
	{
	    u8 *v = vector[n_vector];
	    memcpy(v,v-4,4);
	    n_vector++;
	}


    //---- collect data

    const uint n_info = 3;
    cmpr_info_t info_tab[n_info];

    const uint N_LOOP = 100000;
    for ( i = 0; i < n_info; i++ )
    {
	cmpr_info_t *info = info_tab + i;
	const u64 usec = GetTimerUSec();
	uint n = N_LOOP;
	while ( n-- > 0 )
	    switch (i)
	    {
		case  0: CMPR_distance(vector[0],info,false); break;
		case  1: CMPR_distance(vector[0],info,true); break;
		case  2: CMPR_wiimm(vector[0],info); break;
		default: ASSERT(0);
	    }
	info->usec = GetTimerUSec() - usec;
	CMPR_close_info(vector[0],info,info->cmpr,true);
    }


    //---- print out

    const uint seplen = 9 + 9 * n_info;
    printf("%.*s\n         ",seplen,Minus300);

    for ( i = 0; i < n_info; i++ )
	printf(" %8.8s",info_tab[i].name);
    printf("\n%.*s\n",seplen,Minus300);

    uint v;
    for ( v = 0; v < 2; v++ )
    {
	printf("result-%u:",v);
	for ( i = 0; i < n_info; i++ )
	{
	    u8 *c = info_tab[i].cmpr + 4*v;
	    printf(" %02x%02x%02x%02x",c[0],c[1],c[2],c[3]);
	}
	putchar('\n');
    }
    printf("%.*s\n",seplen,Minus300);

    for ( v = 0; v < 4; v++ )
    {
	printf("      P%u:",v);
	for ( i = 0; i < n_info; i++ )
	    printf(" %s",print_col(info_tab[i].p[v]));
	putchar('\n');
    }
    printf("%.*s\n",seplen,Minus300);

    for ( v = 0; v < n_vector; v++ )
    {
	printf("%s:",print_col(vector[v]));
	for ( i = 0; i < n_info; i++ )
	{
	    cmpr_info_t *info = info_tab + i;
	    printf("%6u,%u ",info->dist[v],info->index[v]);
	}
	putchar('\n');
    }

    printf("%.*s\nsum-dist:",seplen,Minus300);
    for ( i = 0; i < n_info; i++ )
	printf("%8u ",info_tab[i].total_dist);
    printf("\n nanosec:");
    for ( i = 0; i < n_info; i++ )
	printf("%8llu ",info_tab[i].usec * 1000 / N_LOOP );
    printf("\n%.*s\n",seplen,Minus300);

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			create_images()			///////////////
///////////////////////////////////////////////////////////////////////////////

#undef SETUP_PART
#define SETUP_PART(a,b) x1=(a)*wd+2; x2=x1+wd-4; y1=(b)*ht+2; y2=y1+ht-4;

static void CreateTestImage ( ccp dest_path )
{
    //--- setup

    const uint N  =   4;
    const uint wd = 100;
    const uint ht = 100;
    int x1, x2, y1, y2, i, j, x, y;

    Color_t white, black, red, blue, col1[5], col2[5];
    //SETCOLOR(trans,0xff,0xff,0xff,0x00);
    SETCOLOR(white,0xff,0xff,0xff,0xff);
    SETCOLOR(black,0x00,0x00,0x00,0xff);
    SETCOLOR(red,  0xff,0x00,0x00,0xff);
    SETCOLOR(blue, 0x00,0x00,0xff,0xff);
     SETCOLOR(col1[0],0x00,0x00,0x00,0xff);
     SETCOLOR(col1[1],0xff,0x00,0x00,0xff);
     SETCOLOR(col1[2],0x00,0xff,0x00,0xff);
     SETCOLOR(col1[3],0x00,0x00,0xff,0xff);
     SETCOLOR(col1[4],0xff,0xff,0xff,0xff);
    SETCOLOR(col2[0],0x00,0x00,0x00,0x00);
    SETCOLOR(col2[1],0xff,0x00,0x00,0x00);
    SETCOLOR(col2[2],0x00,0xff,0x00,0x00);
    SETCOLOR(col2[3],0x00,0x00,0xff,0x00);
    SETCOLOR(col2[4],0xff,0xff,0xff,0x00);

    Image_t img;
    InitializeIMG(&img);
    CreateIMG(&img,true,N*wd,N*ht,white);


    //--- stripes, wd=1

    SETUP_PART(0,0);
    x = x1+1;
    for ( ; x < x2; x += 3 )
	DrawVLineIMG(&img, x, y1, y2, black );

    SETUP_PART(1,0);
    y = y1+1;
    for ( ; y < y2; y += 3 )
	DrawHLineIMG(&img, x1, x2, y, black );

    SETUP_PART(2,0);
    x = x1+1; y = y1+1;
    for ( ; x < x2; x += 3, y += 3 )
    {
	DrawHLineIMG(&img, x1, x2, y, black );
	DrawVLineIMG(&img, x, y1, y2, black );
    }


    //--- stripes, wd=1..3

    SETUP_PART(0,1);
    x = x1+3;
    for ( i = 1; x+i < x2; i++ )
    {
	for ( j = 0; j < 4 && x+i < x2; j++ )
	{
	    FillRectIMG(&img,x,y1,x+i,y2,black,false);
	    x += 2*i;
	}
	x += 3;
    }

    SETUP_PART(1,1);
    y = y1+3;
    for ( i = 1; y+i < y2; i++ )
    {
	for ( j = 0; j < 4 && y+i < y2; j++ )
	{
	    FillRectIMG(&img,x1,y,x2,y+i,black,false);
	    y += 2*i;
	}
	y += 3;
    }

    SETUP_PART(2,1);
    x = x1+3; y = y1+3;
    for ( i = 1; x+i < x2 && y+i < y2; i++ )
    {
	for ( j = 0; j < 4 && x+i < x2 && y+i < y2; j++ )
	{
	    FillRectIMG(&img,x,y1,x+i,y2,black,false);
	    FillRectIMG(&img,x1,y,x2,y+i,black,false);
	    x += 2*i;
	    y += 2*i;
	}
	x += 3;
	y += 3;
    }


    //--- gradients

    SETUP_PART(0,2);
    x = x1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillVGradientIMG(&img,x,y1,x+wd/10,y2,col1[i],white);
	x += wd/5;
    }

    SETUP_PART(1,2);
    y = y1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillHGradientIMG(&img,x1,y,x2,y+wd/10,col1[i],white);
	y += wd/5;
    }

    SETUP_PART(2,2);
    x = x1 + wd/10;
    y = y1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillVGradientIMG(&img,x,y1,x+wd/10,y2,col1[i],white);
	FillHGradientIMG(&img,x1,y,x2,y+wd/10,col1[i],white);
	x += wd/5;
	y += wd/5;
    }


    //--- transparent gradients

    SETUP_PART(0,3);
    x = x1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillVGradientIMG(&img,x,y1,x+wd/10,y2,col1[i],col2[i]);
	x += wd/5;
    }

    SETUP_PART(1,3);
    y = y1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillHGradientIMG(&img,x1,y,x2,y+wd/10,col1[i],col2[i]);
	y += wd/5;
    }

    SETUP_PART(2,3);
    x = x1 + wd/10;
    y = y1 + wd/10;
    for ( i = 0; i < 4; i++ )
    {
	FillVGradientIMG(&img,x,y1,x+wd/10,y2,col1[i],col2[i]);
	FillHGradientIMG(&img,x1,y,x2,y+wd/10,col1[i],col2[i]);
	x += wd/5;
	y += wd/5;
    }


    //--- rhombus

    SETUP_PART(3,0);
    x = x1 + wd/2;
    for ( i = 0; i < wd/2-1; i++ )
    {
	DrawHLineIMG(&img, x-i, x+i+1, y1+i, black );
	DrawHLineIMG(&img, x-i, x+i+1, y2-i, black );
    }

    SETUP_PART(3,1);
    x = x1 + wd/2;
    for ( i = 0; i < wd/2-1; i++ )
    {
	DrawHLineIMG(&img, x-i/2, x+i/2+1, y1+i, blue );
	DrawHLineIMG(&img, x-i/2, x+i/2+1, y2-i, blue );
    }

    SETUP_PART(3,2);
    y = y1 + wd/2;
    for ( i = 0; i < wd/2-1; i++ )
    {
	DrawVLineIMG(&img, x1+i, y-i/2, y+i/2+1, red );
	DrawVLineIMG(&img, x2-i, y-i/2, y+i/2+1, red );
    }


    //--- circle

    Image_t circ;
    InitializeIMG(&circ);
    CreateIMG(&circ,true,1000,1000,white);
    x1 = circ.width/2;
    y1 = circ.height/2;

    int r = circ.width/2 - 50;
    x = r;
    y = 0;
    r *= r;
    int r2 = r;

    while ( x >= 0 )
    {
	DrawHLineIMG(&circ, x1-x, x1+x, y1-y, black );
	DrawHLineIMG(&circ, x1-x, x1+x, y1+y, black );
	if ( r > r2 )
	{
	    x--;
	    r -= 2*x + 1;
	}
	else
	{
	    y++;
	    r += 2*y + 1;
	}
    }

    SmartResizeIMG(&circ,false,0,wd,ht);
    SETUP_PART(3,3);
    CopyRectIMG(&img,x1,y1, &circ,0,0, wd,ht, PIM_COPY );


    //--- terminate

    SavePNG(&img,true,dest_path,"test-image.png",0,true,0);
    ResetIMG(&img);
}

#undef SETUP_PART

///////////////////////////////////////////////////////////////////////////////

static enumError save_image
(
    Image_t		* img,		// pointer to valid img
    ccp			path1,		// NULL or part #1 of path
    ccp			path2,		// NULL or part #2 of path
    Color_t		line_col	// color of lines
)
{
    DASSERT(img);

    DrawFrameIMG(img,0,0,img->width-1,img->height-1,line_col);
    DrawHLineIMG(img,0,img->width,img->height/2,line_col);
    DrawVLineIMG(img,img->width/2,0,img->height,line_col);

    return SavePNG(img,true,path1,path2,0,true,0);
}

///////////////////////////////////////////////////////////////////////////////

static int create_images ( int argc, char ** argv )
{
    if ( argc != 2 )
    {
	fprintf(stderr,"Usage: %s CREATE-IMAGES destdir\n",NAME);
	return ERR_SYNTAX;
    }

    Color_t base, col1, col2;
    SETCOLOR(base,0xff,0xff,0xff,0x00);

    const uint len1  = 201;
    const uint size1 =  20;
    const uint next1 =  30;
    const uint beg1  =  ( len1 - 5 * next1 - size1 ) / 2;

    const uint len2 = 391;
    const uint beg2 = 5;
    const uint end2 = len2 - beg2;

    uint pos;


    //--- bg-temp.png

    Image_t bg_img;
    SETCOLOR(col1,0xff,0xff,0xff,0xff);
    CreateIMG(&bg_img,true,len1+len2,len1+len2,col1);

    const uint bg_step = 16;
    SETCOLOR(col1,0xc0,0xc0,0xc0,0xff);
    int y, xstart = 0;
    for ( y = -4; y < (int)bg_img.height; y += bg_step )
    {
	xstart = bg_step - xstart;
	int x;
	for ( x = xstart-4; x < (int)bg_img.width; x += 2*bg_step )
	    FillRectIMG(&bg_img,x,y,x+bg_step,y+bg_step,col1,true);
    }
    SavePNG(&bg_img,true,argv[1],"bg-temp.png",0,true,0);


    //--- blue-temp.png

    Image_t blue_img;
    CreateIMG(&blue_img,true,len1,len2,base);

    pos = beg1;
    SETCOLOR(col1,0x00,0x00,0xff,0xff);
    FillRectIMG(&blue_img,pos,beg2,pos+size1,end2,col1,true);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0xff,0xff);
    SETCOLOR(col2,0x00,0x00,0x00,0xff);
    FillVGradientIMG(&blue_img,pos,beg2,pos+size1,end2,col1,col2);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0xff,0xff);
    SETCOLOR(col2,0x00,0x00,0xff,0x00);
    FillVGradientIMG(&blue_img,pos,beg2,pos+size1,end2,col1,col2);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0x00,0xff);
    SETCOLOR(col2,0xff,0xff,0xff,0xff);
    FillVGradientIMG(&blue_img,pos,beg2,pos+size1,end2,col1,col2);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0x00,0xff);
    SETCOLOR(col2,0x00,0x00,0x00,0x00);
    FillVGradientIMG(&blue_img,pos,beg2,pos+size1,end2,col1,col2);

    SETCOLOR(col1,0x00,0x00,0x80,0xff);
    save_image(&blue_img,argv[1],"blue-temp.png",col1);


   //--- red-temp.png

    Image_t red_img;
    CreateIMG(&red_img,true,len2,len1,base);

    pos = beg1;
    SETCOLOR(col1,0xff,0x00,0x00,0xff);
    FillRectIMG(&red_img,beg2,pos,end2,pos+size1,col1,true);

    pos += next1;
    SETCOLOR(col1,0xff,0x00,0x00,0xff);
    SETCOLOR(col2,0x00,0x00,0x00,0xff);
    FillHGradientIMG(&red_img,beg2,pos,end2,pos+size1,col1,col2);

    pos += next1;
    SETCOLOR(col1,0xff,0x00,0x00,0xff);
    SETCOLOR(col2,0xff,0x00,0x00,0x00);
    FillHGradientIMG(&red_img,beg2,pos,end2,pos+size1,col1,col2);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0x00,0xff);
    SETCOLOR(col2,0xff,0xff,0xff,0xff);
    FillHGradientIMG(&red_img,beg2,pos,end2,pos+size1,col1,col2);

    pos += next1;
    SETCOLOR(col1,0x00,0x00,0x00,0xff);
    SETCOLOR(col2,0x00,0x00,0x00,0x00);
    FillHGradientIMG(&red_img,beg2,pos,end2,pos+size1,col1,col2);

    SETCOLOR(col1,0x80,0x00,0x00,0xff);
    save_image(&red_img,argv[1],"red-temp.png",col1);


    //--- mix image

    Image_t mix_img;
    InitializeIMG(&mix_img);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"mode-copy-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_FOREGROUND);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"mode-foreground-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_BACKGROUND);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"mode-background-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_MIX);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"mode-mix-temp.png",0,true,0);


    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_LEFT|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-left-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_RIGHT|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-right-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_TOP|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-top-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_BOTTOM|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-bottom-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_TOP|PIM_INS_RIGHT|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-top_right-temp.png",0,true,0);

    PatchIMG(&mix_img,&blue_img,&red_img,PIM_COPY|PIM_INS_BOTTOM|PIM_INS_LEFT|PIM_GROW);
    PatchIMG(&mix_img,&mix_img,&bg_img,PIM_BACKGROUND);
    SavePNG(&mix_img,true,argv[1],"insert-bottom_left-temp.png",0,true,0);


    //--- test image

    CreateTestImage(argv[1]);


    //--- term

    ResetIMG(&bg_img);
    ResetIMG(&blue_img);
    ResetIMG(&red_img);
    ResetIMG(&mix_img);
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_resize()			///////////////
///////////////////////////////////////////////////////////////////////////////

static int test_resize ( int argc, char ** argv )
{
    if ( argc != 3 )
    {
	fprintf(stderr,"\n"
		"Usage: %s RESIZE old-size new-size\n\n"
		,NAME);
	return ERR_SYNTAX;
    }

    uint src_size = strtoul(argv[1],0,10);
    uint dest_size = strtoul(argv[2],0,10);

    ResizeHelper_t r;
    InitializeResize(&r,src_size,dest_size);
    printf("\nRESIZE: %u -> %u, step: %u + %u/%u, sum_factor: %u\n\n",
		r.src_size, r.dest_size,
		r.idx_inc, r.frac_inc, r.frac_next,
		r.sum_factor );

    bool stat;
    for ( stat = FirstResize(&r); stat; stat = NextResize(&r) )
    {
	printf(" %5u -> %5u", r.src_idx, r.dest_idx );
	ResizeElement_t *re;
	for ( re = r.elem; re->n_elem; re++ )
	    printf(" | %4u*%4u", re->n_elem, re->factor );
	putchar('\n');
    }
    putchar('\n');
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			resize_image()			///////////////
///////////////////////////////////////////////////////////////////////////////

static int resize_image ( int argc, char ** argv )
{
    if ( argc < 4 )
    {
	fprintf(stderr,"\n"
		"Usage: %s RESIZE-IMAGE new_width['%%']"
			" ('w'|'a'|new_height['%%']) img-file...\n\n"
		,NAME);
	return ERR_SYNTAX;
    }

    u32 wd_param = 0, ht_param = 0;
    char *end = ScanNumU32(argv[1],0,&wd_param,0,1000);
    const bool wd_percent = end && *end == '%';

    const bool aspect = argv[2][0] == 'a' || argv[2][0] == 'A';
    end = ScanNumU32(argv[2],0,&ht_param,0,1000);
    bool ht_percent = end && *end == '%';
    if ( argv[2][0] == 'w' || argv[2][0] == 'W' )
    {
	ht_param = wd_param;
	ht_percent = wd_percent;
    }

    uint i;
    for ( i = 3; i < argc; i++ )
    {
	Image_t img1;
	enumError err = LoadIMG(&img1,true,argv[i],0,false,true,false);
	if (err)
	    return ERR_WARNING;

	const uint wd = wd_percent ? ( img1.width * wd_param + 50 ) / 100 : wd_param;
	const uint ht = aspect
			    ? img1.height * wd / img1.width
			    : ht_percent
				? ( img1.height * ht_param + 50 ) / 100
				: ht_param;

	printf("NEW SIZE: %u*%u -> %u*%u: %s\n",
		img1.width, img1.height, wd, ht, argv[i] );

	Image_t img2;
	u64 time = GetTimeUSec(false);
	err = SmartResizeIMG(&img2,true,&img1,wd,ht);
	time = GetTimeUSec(false) - time;
	printf("S2: %5llu usec [smart resize, new image]\n",time);
	if (err)
	    return ERR_WARNING;

	Image_t img3;
	time = GetTimeUSec(false);
	err = FastResizeIMG(&img3,true,&img1,wd,ht);
	time = GetTimeUSec(false) - time;
	printf("F3: %5llu usec [fast resize, new image]\n",time);
	if (err)
	    return ERR_WARNING;

	time = GetTimeUSec(false);
	err = SmartResizeIMG(&img1,false,0,wd,ht);
	time = GetTimeUSec(false) - time;
	printf("S1: %5llu usec [smart resize, inplace image]\n",time);
	if (err)
	    return ERR_WARNING;

	char path[PATH_MAX];
	NewFileExtS(path,sizeof(path),argv[i],"1smart.png");
	err = SavePNG(&img1,false,path,0,true,true,0);
	if (err)
	    return ERR_WARNING;

	NewFileExtS(path,sizeof(path),argv[i],"2smart.png");
	err = SavePNG(&img2,false,path,0,true,true,0);
	if (err)
	    return ERR_WARNING;

	NewFileExtS(path,sizeof(path),argv[i],"3fast.png");
	err = SavePNG(&img3,false,path,0,true,true,0);
	if (err)
	    return ERR_WARNING;

	ResetIMG(&img1);
	ResetIMG(&img2);
	ResetIMG(&img3);
    }
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_scan_text()		///////////////
///////////////////////////////////////////////////////////////////////////////

static int test_scan_text ( int argc, char ** argv )
{
    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ccp src = argv[i];
	char *dest = iobuf;
	char *end = iobuf + sizeof(iobuf) - 1;
	while ( *src && dest < end )
	{
	    const char ch = *src++;
	    *dest++ = ch == '|' ? '\n' : ch;
	}
	*dest = 0;

	printf("* %s\n",argv[i]);
	ScanInfo_t si;
	InitializeSI(&si,iobuf,dest-iobuf,argv[i],0);
     #if HAVE_PRINT
	si.debug = 1;
     #endif
	for(;;)
	{
	    NextCharSI(&si,true);
	    ScanFile_t *sf = si.cur_file;
	    DASSERT(sf);
	    //PRINT("      >%s\n",sf->ptr);

	    ccp beg = sf->ptr;
	    while (NextCharSI(&si,false))
		sf->ptr++;
	    if ( beg == sf->ptr )
		break;
	    printf("   #%u \"%.*s\"\n",
		sf->line, (int)( sf->ptr - beg ), argv[i] + ( beg - sf->data ) );
	}
	ResetSI(&si);
    }
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_scan_string()		///////////////
///////////////////////////////////////////////////////////////////////////////

static int test_scan_string ( int argc, char ** argv )
{
    uint i;
    for ( i = 1; i < argc; i++ )
    {
	uint len1;
	char buf[1000];
	ScanEscapedString(buf,sizeof(buf),argv[i],-1,true,0,&len1);

	uint len2;
	char out[1000];
	PrintEscapedString(out,sizeof(out),buf,-1,CHMD__ALL,0,&len2);

	printf("\nsrc:  %3u |%s|\ndest: %3u |%s|\n",len1,argv[i],len2,out);
    }
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_scan_file()		///////////////
///////////////////////////////////////////////////////////////////////////////

static int test_scan_file ( int argc, char ** argv )
{
    static VarMap_t vm = {0};
    if (!vm.used)
	DefineParserVars(&vm);

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ccp fname = argv[i];
	File_t F;
	enumError err = OpenFILE(&F,true,fname,false,false);
	if (err)
	    return err;

	uint data_size = F.st.st_size;
	PRINT("LOAD FILE: size = %#x = %u\n",data_size,data_size);
	char * data = MALLOC(data_size+1);
	data[data_size] = 0; // termination for text files

	if ( fread(data,1,data_size,F.f) != data_size )
	{
	    err = FILEERROR1(&F,ERR_READ_FAILED,"Read failed: %s\n",fname);
	    ResetFile(&F,false);
	    return err;
	}
	ResetFile(&F,false);

	printf("* %s\n",argv[i]);

	ScanInfo_t si;
	InitializeSI(&si,data,data_size,fname,0);
	si.predef = &vm;
     #if HAVE_PRINT
	si.debug = 1;
     #endif

 #if 1
	while (NextLineSI(&si,false,true))
	    if (NextCharSI(&si,false))
	    {
		ccp ptr = si.cur_file->ptr;
		ccp eol = FindNextLineFeedSI(&si,true);
		int len = eol - ptr;
		ccp post = "";
		if ( len > 50 )
		{
		    len = 50;
		    post = "...";
		}
		ERROR0(ERR_WARNING,"Line ignored: %.*s%s\n",len,ptr,post);
		GotoEolSI(&si);
	    }
 #else
	uint n;
	for ( n = 0; n < 3; n++ )
	{
	    NextCharSI(&si,true);
	    u32 num;
	    enumError err = ScanU32SI(&si,&num,1,0);
	    printf("\t\t%2u: err=%3u, line=%3u, num=%8u = %6x\n",
			n, err, si.cur_file->line, num, num );
	}
 #endif
	ResetSI(&si);
    }
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_filename()			///////////////
///////////////////////////////////////////////////////////////////////////////

static int test_filename ( int argc, char ** argv )
{
    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ParamList_t param;
	memset(&param,0,sizeof(param));
	param.arg = STRDUP(argv[i]);
	printf("\nsrc:  %s\n",param.arg);

	char dest[PATH_MAX];
	uint stat = NumberedFilename(dest,sizeof(dest),param.arg,0,0,true);
	printf("num:  %s [%u]\n",dest,stat);

	SubstDest( dest, sizeof(dest), param.arg,
		opt_dest && *opt_dest ? opt_dest : "\1P/\1N\1?T", "\1N\1?T",
		GetExtFF(FF_BZ,FF_WU8), false );
	printf("dest: %s\n",dest);

	NORMALIZE_FILENAME_PARAM(&param);
	printf("norm: %s\n",param.arg);
	SubstDest( dest, sizeof(dest), param.arg,
		opt_dest && *opt_dest ? opt_dest : "\1P/\1N\1?T", "\1N\1?T",
		GetExtFF(FF_BZ,FF_WU8), false );
	printf("dest: %s\n",dest);

	stat = NumberedFilename(dest,sizeof(dest),dest,0,0,true);
	printf("num:  %s [%u]\n",dest,stat);

	FREE(param.arg);
    }
    putchar('\n');
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			draw_startpos()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError save_img ( Image_t *img, char type, uint n )
{
    DASSERT(img);

    char fname[100];
    snprintf(fname,sizeof(fname),"./_start-pos.tmp/start-pos-%c-%02u.png",type,n);
    enumError err = SavePNG(img,false,fname,0,0,true,0);
    if (!err)
	printf("Image %s created.\n",fname);
    return err;
}

///////////////////////////////////////////////////////////////////////////////

static enumError draw_startpos_helper ( uint n )
{
    double3 tab_w[12], tab_n[12];

    GetStartPosKMP(tab_w,n,false,false,0,0);
    GetStartPosKMP(tab_n,n,false,true,0,0);

    int i, z1;
    const int xmid	= 1200;
    const int zmid	=  300;
    const int zmax	= 5600;
    const double scale	= 1/20.0;

    Image_t imgw, imgn, imgx;
    Color_t col1, col2;
    col1.val = 0xff600000;

    enumError err1 = CreateIMG(&imgw,true,2*xmid*scale,(zmid+zmax)*scale,col1);
    enumError err2 = CreateIMG(&imgn,true,2*xmid*scale,(zmid+zmax)*scale,col1);
    enumError err3 = CreateIMG(&imgx,true,2*xmid*scale,(zmid+zmax)*scale,col1);
    if (err1||err2||err3)
	goto abort;

    //--- draw vertical `500´ lines

    col1.val = 0xff804000;
    z1 = zmid * scale;

    for ( i = -500; i <= 500; i += 1000 )
    {
	uint x = ( xmid + i ) * scale;
	FillRectIMG( &imgw, x-1, z1, x, imgw.height, col1, true );
    }


    //--- draw horizontal `500´ lines

    for ( i = zmid+500; i < zmid+zmax; i += 1000 )
    {
	uint z = i * scale;
	FillRectIMG( &imgw, 0, z-1, imgw.width, z, col1, true );
    }


    //--- draw vertical `1000´ lines

    col1.val = 0xff808000;
    z1 = zmid * scale;

    for ( i = -1000; i <= 1000; i += 1000 )
    {
	uint x = ( xmid + i ) * scale;
	FillRectIMG( &imgw, x-1, z1, x+1, imgw.height, col1, true );
    }


    //--- draw horizontal `1000´ lines

    for ( i =  zmid; i < zmid+zmax; i += 1000 )
    {
	uint z = i * scale;
	FillRectIMG( &imgw, 0, z-1, imgw.width, z+1, col1, true );
    }


    //--- draw finisch line

    col1.val = 0xff0000ff;
    FillRectIMG( &imgw,	0,
			(zmid-25)*scale,
			imgw.width,
			(zmid+25)*scale,
			col1, true );


    //--- copy the images

    CopyIMG(&imgn,false,&imgw,false);
    CopyIMG(&imgx,false,&imgw,false);


    //--- draw start positions

    col1.val = 0xff00ffff;
    col2.val = 0xff0080ff;

    for ( i = 0; i < n; i++ )
    {

	//--- draw narrow

	const double3 *d = tab_n + i;
	FillRectIMG( &imgn,
			(xmid+d->x-100)*scale,
			(zmid+d->z-180)*scale,
			(xmid+d->x+100)*scale,
			(zmid+d->z+180)*scale,
			col1, true );

	FillRectIMG( &imgx,
			(xmid+d->x-100)*scale,
			(zmid+d->z-180)*scale,
			(xmid+d->x+100)*scale,
			(zmid+d->z+180)*scale,
			col2, true );

	//--- draw wide

	d = tab_w + i;
	FillRectIMG( &imgw,
			(xmid+d->x-100)*scale,
			(zmid+d->z-180)*scale,
			(xmid+d->x+100)*scale,
			(zmid+d->z+180)*scale,
			col1, true );

	FillRectIMG( &imgx,
			(xmid+d->x-100)*scale,
			(zmid+d->z-180)*scale,
			(xmid+d->x+100)*scale,
			(zmid+d->z+180)*scale,
			col1, true );
    }


    //--- draw defined start position

    col1.val = 0xff0000ff;

    FillRectIMG( &imgw,	(xmid-80)*scale,
			(zmid-80)*scale,
			(xmid+80)*scale,
			(zmid+80)*scale,
			col1, true );

    FillRectIMG( &imgn,	(xmid-80)*scale,
			(zmid-80)*scale,
			(xmid+80)*scale,
			(zmid+80)*scale,
			col1, true );

    FillRectIMG( &imgx,	(xmid-80)*scale,
			(zmid-80)*scale,
			(xmid+80)*scale,
			(zmid+80)*scale,
			col1, true );

    //--- save the images

    err1 = save_img(&imgw,'w',n);
    err2 = save_img(&imgn,'n',n);
    err3 = save_img(&imgx,'x',n);

    MARK_USED(col2);

 abort:
    ResetIMG(&imgw);
    ResetIMG(&imgn);
    ResetIMG(&imgx);
    return err1 || err2 || err3 ? ERR_ERROR : ERR_OK;
}

///////////////////////////////////////////////////////////////////////////////

static int draw_startpos ( int argc, char ** argv )
{
    opt_mkdir = true;

    uint n;
    for ( n = 1; n<= 12; n++ )
	draw_startpos_helper(n);
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			draw_startpos()			///////////////
///////////////////////////////////////////////////////////////////////////////

static uint sort_mode = 0;

//-----------------------------------------------------------------------------

static int sort_path_cmp ( ccp *p1, ccp *p2 )
{
    return PathCmp(*p1,*p2,sort_mode);
}

//-----------------------------------------------------------------------------

static int test_path_cmp ( int argc, char ** argv )
{
    if ( argc < 3 )
    {
	fprintf(stderr,"Usage: PATH-CMP mode string1 string2 [stringx]...\n");
	return ERR_SYNTAX;
    }

    sort_mode = strtoul(argv[1],0,0) & 7;
    argc -= 2;
    argv += 2;

    printf("* Sort %u strings with mode %u\n",argc,sort_mode);
    qsort( argv, argc, sizeof(*argv), (qsort_func)sort_path_cmp );

    uint i;
    for ( i = 0; i < argc; i++ )
	printf("%5u. %s\n",i,argv[i]);
    return 0;
}
//
///////////////////////////////////////////////////////////////////////////////
///////////////			ana_file_objflow()		///////////////
///////////////////////////////////////////////////////////////////////////////

static ccp print16 ( u16 val )
{
    if (!val)
	return "-";

    static char buf[10];
    snprintf(buf,sizeof(buf),"%x",val);
    return buf;
}

//-----------------------------------------------------------------------------

static int ana_file_objflow ( int argc, char ** argv )
{
    char buf[20];

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	u8 *raw_data;
	uint data_size;
	enumError err = OpenReadFILE(argv[i],0,false,&raw_data,&data_size,0,0);
	if (err)
	    return err;
	printf("* %s\n",argv[i]);
	objflow_bin_t *odata = (objflow_bin_t*)(raw_data+2);

	#undef PLEN
	#define PLEN ( sizeof(odata->param) / sizeof(*odata->param) )
	uint param_fw[PLEN];
	memset(param_fw,0,sizeof(param_fw));

	uint max_name_size = 4, max_resources_size = 4;
	const uint N = be16(raw_data);
	objflow_bin_t *ptr, *end = odata + N;
	for ( ptr = odata; ptr < end && ptr->id != 0xffff; ptr++ )
	{
	    uint len = strlen(ptr->name);
	    if ( max_name_size < len )
		 max_name_size = len;
	    len = strlen(ptr->resources);
	    if ( max_resources_size < len )
		 max_resources_size = len;

	    ptr->id = be16(&ptr->id);
	    uint p;
	    for ( p = 0; p < PLEN; p++ )
	    {
		u32 val = be16(ptr->param+p);
		ptr->param[p] = val;
		if ( param_fw[p] < val )
		     param_fw[p] = val; // store max value first
	    }
	}
	if ( max_name_size > sizeof(odata->name) - 1 )
	     max_name_size = sizeof(odata->name) - 1;
	if ( max_resources_size > sizeof(odata->resources) - 1 )
	     max_resources_size = sizeof(odata->resources) - 1;

	uint linelen = printf(" Pos   ID %-*s", max_name_size, "Name" )
		     + max_resources_size + 3;
	uint p;
	for ( p = 0; p < PLEN; p++ )
	    if (param_fw[p])
	    {
		uint fw = snprintf(buf,sizeof(buf),"%3x",param_fw[p]);
		param_fw[p] = fw > 3 ? fw : 3;
		linelen += printf("%*s%u",param_fw[p],"P",p);
	    }
	printf("  Resources\n%.*s\n", linelen, Minus300);

	for ( ptr = odata; ptr < end && ptr->id != 0xffff; ptr++ )
	{
	    printf(" %3zu  %03x %-*s", ptr-odata, ptr->id, max_name_size, ptr->name );
	    for ( p = 0; p < PLEN; p++ )
		if (param_fw[p])
		    printf(" %*s",param_fw[p],print16(ptr->param[p]));
	    printf("  %s\n",ptr->resources);
	}

	#undef PLEN

	FREE(raw_data);
    }
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_line()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError scan_double
	( double * pt, int n_dim, ScanInfo_t *si, double def_x, double def_y )
{
    DASSERT(pt);
    DASSERT(si);
    DASSERT( n_dim == 2 || n_dim == 3 );

    // set defaults
    pt[0] = def_x;
    pt[1] = def_y;
    if ( n_dim == 3 )
	pt[2] = 0.0;

    char ch = NextCharSI(si,false);
    if ( ch == ',' )
    {
	si->cur_file->ptr++;
	ch = NextCharSI(si,false);
    }

    if ( ch == '.' )
    {
	si->cur_file->ptr++;
	return ERR_OK;
    }

    uint dim;
    for ( dim=0; dim < n_dim; dim++ )
    {
	double d;
	enumError err = ScanDValueSI(si,&d);
	if (err)
	    return err;

	pt[dim] = d;
	char ch = NextCharSI(si,false);
	if ( !ch || ch == ',' )
	    break;
    }

    return ERR_OK;
}

//-----------------------------------------------------------------------------

static enumError scan_float
	( float * pt, int n_dim, ScanInfo_t *si, double def_x, double def_y )
{
    DASSERT(pt);
    DASSERT(si);

    DASSERT( n_dim <= 10 );
    double temp[10];
    enumError err = scan_double(temp,n_dim,si,def_x,def_y);
    uint i;
    for ( i = 0; i < n_dim; i++ )
	pt[i] = temp[i];

    return err;
}

//-----------------------------------------------------------------------------

static void print_double_vec ( double *pt, int n_dim, ccp title )
{
    if ( n_dim == 3 )
	printf("%s%9.3f %9.3f %9.3f\n",title,pt[0],pt[1],pt[2]);
    else
	printf("%s%9.3f %9.3f\n",title,pt[0],pt[1]);
}

//-----------------------------------------------------------------------------

static void print_float_vec ( float *pt, int n_dim, ccp title )
{
    if ( n_dim == 3 )
	printf("%s%9.3f %9.3f %9.3f\n",title,pt[0],pt[1],pt[2]);
    else
	printf("%s%9.3f %9.3f\n",title,pt[0],pt[1]);
}

//-----------------------------------------------------------------------------

static int test_line ( int n_dim, int argc, char ** argv )
{
    n_dim = n_dim == 3 ? 3 : 2;
    if ( argc < 2 )
    {
	fprintf(stderr,
		"\n"
		"Usage: %s 'LINE2' N [,P1] [,P2] [,Pa] [,Pb]\n"
		"\n"
		"   'Px' is a point with %u dimensions, missing dimensions are filled with 0.\n"
		"   A point means: default value (P1=0, P2=1000, P3=P4=500 500)\n"
		"\n"
		"   => Create a line with N points from P1..P2.\n"
		"      Pa and Pb are helpers for Bézier curves.\n"
		"\n"
		"      Linear:    B(t) = (1-t)*P1 + t*P2\n"
		"      Quadratic: B(t) = (1-t)^2*P1 + 2*(1-t)*t*Pa + t^2*P2\n"
		"      Cubic:     B(t) = (1-t)^3*P1 + 3*(1-t)^2*t*Pa\n"
		"                      + 3*(1-t)*t^2*Pb + t^3*P2\n"
		"      where t=0.0 .. 1.0\n"
		"\n"
		,NAME, n_dim );
	return ERR_SYNTAX;
    }

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ScanInfo_t si;
	InitializeSI(&si,argv[i],strlen(argv[i]),"arg",0);

	Line_t lin;
	InitializeLine(&lin);

	long n;
	enumError err = ScanUValueSI(&si,&n,false);
	if (err)
	    return err;
	lin.n = n;

	float p1[3] = { 0.0, 0.0, 0.0 };
	if (NextCharSI(&si,false))
	{
	    err = scan_float(p1,n_dim,&si,0.0,0.0);
	    if (err)
		return err;
	}

	float p2[3] = { 1000.0, 0.0, 0.0 };
	if (NextCharSI(&si,false))
	{
	    err = scan_float(p2,n_dim,&si,1000.0,0.0);
	    if (err)
		return err;
	}

	while ( lin.n_pt < 2 && NextCharSI(&si,false) )
	{
	    err = scan_double(lin.pt[lin.n_pt],n_dim,&si,500.0,500.0);
	    if (err)
		return err;
	    lin.n_pt++;
	}

	printf("\n-----------------------------------------------\n");
	print_float_vec(p1,n_dim,"  P1: ");
	if ( lin.n_pt >= 1 )
	    print_double_vec(lin.pt[0],n_dim,"  Pa: ");
	if ( lin.n_pt >= 2 )
	    print_double_vec(lin.pt[1],n_dim,"  Pb: ");
	print_float_vec(p2,n_dim,"  P2: ");
	printf("-----------------------------------------------\n");
	uint p;
	for ( p = 0; p <= lin.n; p++ )
	{
	    float pt[3];
	    CalcLine(&lin,pt,p1,p2,n_dim,p);
	    printf("  -> %2u: ",p);
	    print_float_vec(pt,n_dim,"");
	}
	printf("-----------------------------------------------\n");
    }
    return ERR_OK;
}

#if 0
	enumError err = n_dim == 3
			? ScanLine3(&lin,&si,true)
			: ScanLine2(&lin,&si,true);
	if (err)
	    continue;
#endif

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test()				///////////////
///////////////////////////////////////////////////////////////////////////////

int test ( int argc, char ** argv )
{
    printf("sizeof: Color_t=%zu\n",sizeof(Color_t));
    Color_t c;
    c.r = 0x01;
    c.g = 0x02;
    c.b = 0x03;
    c.a = 0x04;
    printf("col=%x\n",c.val);

    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_memfile()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_memfile ( int argc, char ** argv )
{
    MemFile_t mf;
    InitializeMemFile(&mf,1000,0x100000);
    mf.err_name = "TEST";
    DumpMemFile(stdout,2,&mf);

    WriteMemFile(&mf,"Hallo Dirk\n",11);
    WriteMemFile(&mf,"Hallo Dirk\n",11);
    WriteMemFileAt(&mf,4,"xxx",2);
    WriteMemFile(&mf,"yyy",2);
    DumpMemFile(stdout,2,&mf);
    SaveMemFile(&mf,0,"memfile1.tmp",FM_OVERWRITE,false);

    WriteMemFileAt(&mf,20,"|Hallo Dirk\n",12);
    DumpMemFile(stdout,2,&mf);
    SaveMemFile(&mf,0,"memfile2.tmp",FM_OVERWRITE,false);
    ResetMemFile(&mf);

    LoadMemFile(&mf,false,0,"memfile1.tmp",1,0,false);
    SaveMemFile(&mf,0,"memfile3.tmp",FM_OVERWRITE,false);

    WriteMemFileAt(&mf,0x10080,"a",1);
    WriteMemFileAt(&mf,0x20080,"",0);
    SaveMemFile(&mf,0,"sparse.tmp",FM_OVERWRITE,true);

    for(;;)
    {
	char buf[5];
	uint size = ReadMemFile(&mf,buf,sizeof(buf));
	if (!size)
	    break;
	fwrite(buf,1,size,stdout);
    }
    ResetMemFile(&mf);

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_find_dol()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_find_dol ( int argc, char ** argv )
{
    if ( argc < 3 )
    {
	fprintf(stderr,"params: main.dol size...\n");
	return ERR_SYNTAX;
    }

    dol_header_t dh;
    enumError err = LoadFILE(argv[1],0,0,&dh,sizeof(dh),1,false,false);
    if (err)
	return err;

    dh.bss_size = htonl(0x2000000);
    DumpDolHeader(stdout,3,&dh,0,2);

    uint i;
    for ( i = 2; i < argc; i++ )
    {
	const uint size = str2ul(argv[i],0,10);
	u32 space, addr = FindFreeBssSpaceDOL(&dh,size,32,&space);
	printf("%s %8x => %8x + %8x\n",argv[1],size,addr,space);
    }

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_subfile()			///////////////
///////////////////////////////////////////////////////////////////////////////

static int SubFileIt
(
    SubFileIterator_t	*sfi,		// iterator data
    uint		mode,		// 0:file, 1:open dir, 2:close dir
    SubDir_t		*dir,		// current directory
    SubFile_t		*file		// NULL or current file
)
{
    printf("%*s%u %s\n",sfi->depth,"",mode,sfi->path);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static enumError test_subfile ( int argc, char ** argv )
{
    SubDir_t dir;
    InitializeSubDir(&dir);

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ccp arg = argv[i];
	bool new;
	SubFile_t *sf = InsertSubFile(&dir,MemByString(arg),&new);
	printf("%6s: %s\n",!sf?"FAIL":new?"INSERT":"FOUND",arg);
    }

    putchar('\n');
    DumpSubDir(stdout,2,&dir);

    SubFileIterator_t it;
    memset(&it,0,sizeof(it));
    it.func = SubFileIt;
    IterateSubDir(&dir,&it);

    SaveSubFiles(&dir,"_subfile.tmp");

    for ( i = 1; i < argc; i++ )
    {
	ccp arg = argv[i];
	SubFile_t *sf = FindSubFile(&dir,MemByString(arg));
	if (sf)
	    printf("FOUND: %s -> %s\n",arg,sf->fname);
	else
	    printf("NOT FOUND: %s\n",arg);
    }

    ResetSubDir(&dir);
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_str_list()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_str_list ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
	printf(	"STR-LIST | STR [option|string]...\n"
		"  -a : sort by ascii -> strcmp() [DEFAULT]\n"
		"  -i : sort 'ignore case'-> strcasecmp()\n"
		"  -n : sort numeric -> StrNumCmp()\n"
		);
	return ERR_OK;
    }

    StringField_t sf;
    InitializeStringField(&sf);

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ccp arg = argv[i];
	if (!strcmp(arg,"-a"))
	{
	    WriteStringField(stdout,"stdout",&sf,"\t",0);
	    fputs("-----\n",stdout);
	    sf.func_cmp = strcmp;
	    SortStringField(&sf);
	}
	else if (!strcmp(arg,"-i"))
	{
	    WriteStringField(stdout,"stdout",&sf,"\t",0);
	    fputs("-----\n",stdout);
	    sf.func_cmp = strcasecmp;
	    SortStringField(&sf);
	}
	else if (!strcmp(arg,"-n"))
	{
	    WriteStringField(stdout,"stdout",&sf,"\t",0);
	    fputs("-----\n",stdout);
	    sf.func_cmp = StrNumCmp;
	    SortStringField(&sf);
	}
	else
	    InsertStringField(&sf,arg,false);
    }

    WriteStringField(stdout,"stdout",&sf,"\t",0);
    ResetStringField(&sf);

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_container()		///////////////
///////////////////////////////////////////////////////////////////////////////

static void test_container_data ( int mode, ContainerData_t *cdata, uint modify )
{
    Container_t c;
    CatchContainerData(&c,true,cdata);
    if ( mode >= 0 )
	DumpInfoContainer(stdout,colout,2,"FUNC: ",&c, mode > 0 ? 32 : 0 );

    if ( modify == 1 )
    {
	ModifyAllContainer(&c);
	if ( c.cdata && c.cdata->data )
	    StringCopyS((char*)c.cdata->data,c.cdata->size,"MODIFIED!");
	DumpInfoContainer(stdout,colout,2,"-MOD: ",&c, mode > 0 ? 32 : 0 );
    }
    else if ( modify == 2 && c.cdata && c.cdata->size > 20 )
    {
	char *data = (char*)c.cdata->data + 2;
	if (ModifyContainer(&c,data,12,CPM_COPY))
	    data = (char*)c.cdata->data;
	StringCopyE(data,(ccp)c.cdata->data+c.cdata->size,"EXTRACT!");
	DumpInfoContainer(stdout,colout,2,"-EXT: ",&c, mode > 0 ? 32 : 0 );
    }
    ResetContainer(&c);
}

///////////////////////////////////////////////////////////////////////////////

static enumError test_container ( int argc, char ** argv )
{
    //-----------------------------------------------------------------------------
    //				to test
    //-----------------------------------------------------------------------------
    // + Container_t * CreateContainer ( Container_t, protect, data,size, CopyMode_t )
    // - Container_t * InheritContainer ( Container_t cur, protect, Container_t parent, data,size )
    // + bool AssignContainer ( Container_t, protect, data,size, CopyMode_t )
    // + void ResetContainer ( Container_t )
    // / void DeleteContainer ( Container_t )
    // + void UnlinkContainerData ( Container_t )
    // + ContainerData_t * LinkContainerData ( Container_t )
    // + ContainerData_t * MoveContainerData ( Container_t )
    // + Container_t * CatchContainerData ( Container_t, int protect, ContainerData_t )
    //------------
    // - ContainerData_t DisjoinContainerData ( Container_t )
    // - void JoinContainerData ( Container_t, ContainerData_t )
    // + void FreeContainerData ( ContainerData_t )
    //------------
    // + static inline bool ModificationAllowed ( const Container_t )
    // + void ModifyAllContainer ( Container_t )
    // + void ModifyContainer ( Container_t, data, size, CopyMode_t )
    // + int SetProtectContainer ( Container_t, int new_protect )
    // + static inline int AddProtectContainer ( Container_t, int add_protect )
    // - static inline bool InContainerP ( const Container_t, cvp ptr )
    // + static inline bool InContainerS ( const Container_t, cvp ptr, uint size )
    // - static inline bool InContainerE ( const Container_t, cvp ptr, cvp end )
    // + uint DumpInfoContainer ( f,col,indent,prefix, Container_t, hexdump_len )
    // + uint DumpInfoContainerData ( f,col,indent,prefix, CData_t, hex_len, hex_indent )
    //-----------------------------------------------------------------------------

    if ( argc < 2 )
    {
	printf(	"Syntax: CONTAINTER [['-'|'+']command]...\n"
		"\n"
		"Commands:\n"
		"\n"
		"  dump    : do nothing, dump only.\n"
		"  dump2   : do nothing, dump only second container.\n"
		"  unlink2 : unlink second container.\n"
		"\n"
		"  inherit : inherit base data.\n"
		"  assign  : re-assign base data.\n"
		"  free    : free linked data.\n"
		"  catch   : catch by second container.\n"
		"  reset   : reset second container.\n"
		"\n"
		"  p0..p2  : set protection level\n"
		"  p- p+   : decrement or increment protection level\n"
		"\n"
		"  null    : call function & use NULL\n"
		"  link    : call function & use LinkContainerData()\n"
		"  link+   : call function & use LinkContainerData() & modify\n"
		"  link++  : call function & use LinkContainerData() & extract\n"
		"  move    : call function & use MoveContainerData()\n"
		"  move+   : call function & use MoveContainerData() & modify\n"
		"  move++  : call function & use MoveContainerData() & extract\n"
		"\n"
		"The base container is automatically recreated if empty.\n"
		"\n"
		"Command prefix:\n"
		"  - : silent = Don't dump.\n"
		"  + : verbose = include hex dump.\n"
		"\n"
		);
	return 0;
    }

    char buf[1000];
    memset(buf,0,sizeof(buf));
    strcpy(buf,"0123456789 init");
    Container_t c, cc;
    CreateContainer(&c,true,buf,sizeof(buf),CPM_LINK);
    CreateContainer(&cc,true,0,0,0);
    DumpInfoContainer(stdout,colout,2,"BASE: ",&c,32);

    uint i;
    for ( i = 1; i < argc; i++ )
    {
	ccp arg = argv[i];
	if ( !arg || !*arg )
	    continue;
	int mode = 0;
	if ( *arg == '-' )
	    mode--, arg++;
	else if ( *arg == '+' )
	    mode++, arg++;

	if ( mode >= 0 )
	    printf("-------------------- %d. |%s| --------------------\n",i,arg);

	if (!strcasecmp(arg,"p0"))
	    SetProtectContainer(&c,0);
	else if (!strcasecmp(arg,"p1"))
	    SetProtectContainer(&c,1);
	else if (!strcasecmp(arg,"p2"))
	    SetProtectContainer(&c,2);
	else if (!strcasecmp(arg,"p-"))
	    AddProtectContainer(&c,-1);
	else if (!strcasecmp(arg,"p+"))
	    AddProtectContainer(&c,1);

	else if (!strcasecmp(arg,"null"))
	    test_container_data(mode,0,0);

	else if (!strcasecmp(arg,"link"))
	    test_container_data(mode,LinkContainerData(&c),0);
	else if (!strcasecmp(arg,"link+"))
	    test_container_data(mode,LinkContainerData(&c),1);
	else if (!strcasecmp(arg,"link++"))
	    test_container_data(mode,LinkContainerData(&c),2);

	else if (!strcasecmp(arg,"move"))
	    test_container_data(mode,MoveContainerData(&c),0);
	else if (!strcasecmp(arg,"move+"))
	    test_container_data(mode,MoveContainerData(&c),1);
	else if (!strcasecmp(arg,"move++"))
	    test_container_data(mode,MoveContainerData(&c),2);

	else if (!strcasecmp(arg,"unlink2"))
	{
	    UnlinkContainerData(&cc);
	}
	else if (!strcasecmp(arg,"inherit"))
	{
	    InheritContainer(&cc,2,&c,buf+5,3);
	    if ( mode >= 0 )
		DumpInfoContainer(stdout,colout,2,"C2:   ",&cc, mode > 0 ? 32 : 0 );
	}
	else if (!strcasecmp(arg,"assign"))
	{
	    memset(buf,0,sizeof(buf));
	    strcpy(buf,"0123456789 init");
	    AssignContainer(&c,true,buf,sizeof(buf),CPM_LINK);
	}
	else if (!strcasecmp(arg,"free"))
	{
	    FreeContainerData(LinkContainerData(&c));
	}
	else if (!strcasecmp(arg,"catch"))
	{
	    ResetContainer(&cc);
	    CatchContainerData(&cc,true,LinkContainerData(&c));
	    if ( mode >= 0 )
		DumpInfoContainer(stdout,colout,2,"C2:   ",&cc, mode > 0 ? 32 : 0 );
	}
	else if (!strcasecmp(arg,"reset"))
	{
	    ResetContainer(&cc);
	    if ( mode >= 0 )
		DumpInfoContainer(stdout,colout,2,"C2:   ",&cc, mode > 0 ? 32 : 0 );
	}
	else if (strcasecmp(arg,"dump"))
	{
	    printf("  >>> IGNORED!\n");
	    continue;
	}
	else if (strcasecmp(arg,"dump2"))
	{
	    DumpInfoContainer(stdout,colout,2,"C2:   ",&cc, mode > 0 ? 32 : 0 );
	    continue;
	}

	if ( mode >= 0 )
	    DumpInfoContainer(stdout,colout,2,"BACK: ",&c, mode > 0 ? 32 : 0 );

	if (!c.cdata)
	{
	    memset(buf,0,sizeof(buf));
	    strcpy(buf,"0123456789 init");
	    AssignContainer(&c,true,buf,sizeof(buf),CPM_LINK);
	}
    }

    ResetContainer(&c);
    ResetContainer(&cc);
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_uuid()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_uuid ( int argc, char ** argv )
{
    uint i;
    char buf[100];

    printf("\n--- test ReadFromUrandom()\n");
    for ( i = 0; i < 5; i++ )
    {
	uint stat = ReadFromUrandom(buf,15);
	HexDump16(stdout,4,0,buf,stat);
    }

    printf("\n--- test CreateUUID() & MyRandom(1000000)\n");
    for ( i = 0; i < 5; i++ )
    {
	uint stat = CreateTextUUID(buf,sizeof(buf));
	printf(" %u: %s  %6u [avail=%d,use=%d]\n",
		stat, buf, MyRandom(1000000),
		urandom_available, use_urandom_for_myrandom );
    }

    printf("\n--- test ScanUUID()\n");
    for ( i = 1; i < argc; i++ )
    {
	uuid_buf_t uuid;
	ccp stat = ScanUUID(uuid,argv[i]);
	PrintUUID(buf,sizeof(buf),uuid);
	printf("%s %s<= %s\n", buf, stat==argv[i] ? "FAILED! " : "", argv[i] );
    }

    putchar('\n');
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_analyse_magic()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_analyse_magic ( int argc, char ** argv )
{
    uint i;
    u8 magic[8];

    printf("\n--- test AnalyseMagic() + RepairMagic()\n");
    for ( i = 1; i < argc; i++ )
    {
	ccp fname = argv[i];
	printf("\n%s\n", fname );

	u8 *data;
	uint data_size;
	enumError err = OpenReadFILE(fname,0,false,&data,&data_size,0,0);
	if ( err || data_size < sizeof(magic) )
	    return err;

	memcpy(magic,data,sizeof(magic));

	repair_ff_t rep;
	AnalyseMagic(&rep,data,data_size,data_size,FF_UNKNOWN,fname,false);
	printf("\t\e[35;1m=> Ana0: %s\e[0m\n",GetRepairMagicInfo(&rep));
	RepairMagic(&rep,data,data_size,data_size,FF_UNKNOWN,fname,false);
	printf("\t\e[35;1m=> Rep0: %s\e[0m\n",GetRepairMagicInfo(&rep));

	memcpy(data,magic,sizeof(magic));
	AnalyseMagic(&rep,data,data_size,data_size,FF_UNKNOWN,fname,true);
	printf("\t\e[35;1m=> Ana1: %s\e[0m\n",GetRepairMagicInfo(&rep));
	RepairMagic(&rep,data,data_size,data_size,FF_UNKNOWN,fname,true);
	printf("\t\e[35;1m=> Rep1: %s\e[0m\n",GetRepairMagicInfo(&rep));

	if (rep.magic_len)
	{
	    fputs("\t\e[35;1m=> magic0:",stdout);
	    uint i;
	    for ( i = 0; i < rep.magic_len; i++ )
		printf(" %02x",magic[i]);
	    fputs("\e[0m\n\t\e[35;1m=> magic1:",stdout);
	    for ( i = 0; i < rep.magic_len; i++ )
		printf(" %02x",data[i]);
	    fputs("\e[0m\n",stdout);
	}
	FREE(data);
    }

    putchar('\n');
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_vr()			///////////////
///////////////////////////////////////////////////////////////////////////////

// get VR diff by b-spline curve (by chadders, outdated (not exact))
// https://www.desmos.com/calculator/gr17y7d944

static double GetVrDiffBSpline ( double d )
{
    //-------------------------------------------
    //	x = loser - winner
    //
    //	f(x):
    //	  (0..1( : 1/6 * (3x^3 - 6x^2 + 4 )
    //	  (1..2( : 1/6 * (2-x)^3
    //	  else   : 0
    //
    //-------------------------------------------

    if ( d >= 0.0 && d < 1.0 )
	return ( 3*d*d*d - 6*d*d + 4 )/6.0;

    if ( d >= 1.0 && d < 2.0 )
    {
	d = 2 - d;
	return d*d*d/6.0;
    }

    return 0;
}


double GetVrDiffD ( int diff )
{
    // diff := Pwinner - Plooser


    //-- cut differences

    if ( diff > 9998 )
	diff = 9998;
    else if ( diff < -9998 )
	diff = -9998;


    //-- calculate rating as sum of 10 knots

    double sum = 0;
    int j;
    for ( j = 0; j < 10; j++ )
    {
	static int knot[] = {0,0,0,1,8,50,125,125,125,125};
	const double temp = fabs( (2-diff)/5000.0 - ( j - 4 ) );
	sum += GetVrDiffBSpline(temp) * knot[j];
    }

    return sum;
}

static inline int GetVrDiffI ( int diff )
	{ return floor(GetVrDiffD(diff)); }

///////////////////////////////////////////////////////////////////////////////

static enumError test_vr ( int argc, char ** argv )
{
    const struct pair { int delta; int vr_add; } known[] =
    {
	{-20000, 112 },
	{ -9998, 112 },
	{ -9980, 112 },
	{ -9890, 111 },
	{ -9190, 105 },
	{ -8000,  92 },
	{ -6000,  67 },
	{ -4000,  44 },
	{ -2000,  26 },
	{ -1000,  19 },
	{     0,  13 },
	{  2000,   6 },
	{  4000,   2 },
	{  6000,   1 },
	{  8000,   0 },
	{  9980,   0 },
	{  9998,   0 },
	{ 20000,   0 },
	{-1,-1}
    };

    int i;
    for ( i = 1; i < argc; i++ )
    {
	bool verbose = !strcmp(argv[i],"test+");
	if ( verbose || !strcmp(argv[i],"test") )
	{
	    fputs("\n"
		"   delta :  result  :  tab : good : status\n"
		"-------------------------------------------\n",stdout);
	    int count = 0, total = 0;
	    const struct pair *ptr;
	    for ( ptr = known; ptr->vr_add != -1; ptr++ )
	    {
		total++;
		double r = GetVrDiffD(ptr->delta);
		int i = floor(r);
		int j = GetVrDiffByTab(ptr->delta);
		bool differ = i != ptr->vr_add || i != j;
		count += differ;
		if ( differ || verbose )
		    printf("%8d : %8.4f : %4d : %4d %s\n",
			ptr->delta, r, j, ptr->vr_add,
			differ ? " : DIFFER" : "" );
	    }
	    printf("-------------------------------------------\n"
			"    >>> %u of %u differ,%s\n",
			count, total,
			verbose ? "" : " [use test+ to show all]" );
	    continue;
	}

	if (!strcmp(argv[i],"table3"))
	{
	    fputs("\n"
		"\t//  winner-loser add\n"
		"\t//  from,    to,  vr    //    N\n"
		"\t//------------------------------\n",stdout);

	    int diff = -10000, max_diff = 10000, n_record = 0;

	    for ( ; diff < max_diff; diff++ )
	    {
		int start = diff, val = GetVrDiffI(diff);
		while ( diff < max_diff && GetVrDiffI(diff) == val )
		    diff++;
		printf("\t{ %6d, %5d, %3d }, // %4u\n",
				start, diff, val, diff-start+1 );
		n_record++;
	    }
	    printf("\t// %u records total\n",n_record);
	    continue;
	}

	if ( !strcmp(argv[i],"table2") || !strcmp(argv[i],"table") )
	{
	    fputs("\n"
		"\t// delta, +vr    //    N\n"
		"\t//------------------------------\n",stdout);

	    int diff = -10000, max_diff = 10000, n_record = 0;

	    for ( ; diff < max_diff; diff++ )
	    {
		int start = diff, val = GetVrDiffI(diff);
		while ( diff < max_diff && GetVrDiffI(diff) == val )
		    diff++;
		printf("\t{ %6d, %3d }, // %4u\n",
				start, val, diff-start+1 );
		n_record++;
	    }
	    fputs("\t{  10000,   0 }  // upper border\n",stdout);
	    printf("\t// %u+1 records total\n",n_record);
	    continue;
	}

	int d = str2l(argv[i],0,10);
	double r = GetVrDiffD(d);
	printf("%8d -> %10.4f\n",d,r);
    }
    putchar('\n');
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_vr2()			///////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct vr1_diff_t
{
    s16 diff;
    s16 add_vr;
}
vr1_diff_t;

//-----------------------------------------------------------------------------

static const vr1_diff_t vr1_diff_tab[] =
{
	// delta, +vr    //    N
	//------------------------------
	{ -10000, 112 }, //   68
	{  -9932, 111 }, //  127
	{  -9805, 110 }, //  122
	{  -9683, 109 }, //  116
	{  -9567, 108 }, //  113
	{  -9454, 107 }, //  110
	{  -9344, 106 }, //  106
	{  -9238, 105 }, //  104
	{  -9134, 104 }, //  101
	{  -9033, 103 }, //   99
	{  -8934, 102 }, //   97
	{  -8837, 101 }, //   95
	{  -8742, 100 }, //   94
	{  -8648,  99 }, //   92
	{  -8556,  98 }, //   91
	{  -8465,  97 }, //   89
	{  -8376,  96 }, //   89
	{  -8287,  95 }, //   87
	{  -8200,  94 }, //   87
	{  -8113,  93 }, //   85
	{  -8028,  92 }, //   85
	{  -7943,  91 }, //   84
	{  -7859,  90 }, //   84
	{  -7775,  89 }, //   83
	{  -7692,  88 }, //   82
	{  -7610,  87 }, //   81
	{  -7529,  86 }, //   82
	{  -7447,  85 }, //   81
	{  -7366,  84 }, //   80
	{  -7286,  83 }, //   80
	{  -7206,  82 }, //   80
	{  -7126,  81 }, //   79
	{  -7047,  80 }, //   80
	{  -6967,  79 }, //   79
	{  -6888,  78 }, //   79
	{  -6809,  77 }, //   79
	{  -6730,  76 }, //   78
	{  -6652,  75 }, //   79
	{  -6573,  74 }, //   79
	{  -6494,  73 }, //   78
	{  -6416,  72 }, //   79
	{  -6337,  71 }, //   79
	{  -6258,  70 }, //   79
	{  -6179,  69 }, //   79
	{  -6100,  68 }, //   79
	{  -6021,  67 }, //   80
	{  -5941,  66 }, //   79
	{  -5862,  65 }, //   80
	{  -5782,  64 }, //   81
	{  -5701,  63 }, //   80
	{  -5621,  62 }, //   82
	{  -5539,  61 }, //   81
	{  -5458,  60 }, //   82
	{  -5376,  59 }, //   83
	{  -5293,  58 }, //   83
	{  -5210,  57 }, //   84
	{  -5126,  56 }, //   85
	{  -5041,  55 }, //   85
	{  -4956,  54 }, //   87
	{  -4869,  53 }, //   87
	{  -4782,  52 }, //   88
	{  -4694,  51 }, //   89
	{  -4605,  50 }, //   90
	{  -4515,  49 }, //   91
	{  -4424,  48 }, //   91
	{  -4333,  47 }, //   93
	{  -4240,  46 }, //   94
	{  -4146,  45 }, //   96
	{  -4050,  44 }, //   96
	{  -3954,  43 }, //   98
	{  -3856,  42 }, //   98
	{  -3758,  41 }, //  101
	{  -3657,  40 }, //  101
	{  -3556,  39 }, //  103
	{  -3453,  38 }, //  105
	{  -3348,  37 }, //  106
	{  -3242,  36 }, //  107
	{  -3135,  35 }, //  110
	{  -3025,  34 }, //  111
	{  -2914,  33 }, //  113
	{  -2801,  32 }, //  115
	{  -2686,  31 }, //  118
	{  -2568,  30 }, //  119
	{  -2449,  29 }, //  122
	{  -2327,  28 }, //  125
	{  -2202,  27 }, //  127
	{  -2075,  26 }, //  130
	{  -1945,  25 }, //  133
	{  -1812,  24 }, //  137
	{  -1675,  23 }, //  140
	{  -1535,  22 }, //  144
	{  -1391,  21 }, //  149
	{  -1242,  20 }, //  153
	{  -1089,  19 }, //  159
	{   -930,  18 }, //  164
	{   -766,  17 }, //  171
	{   -595,  16 }, //  178
	{   -417,  15 }, //  187
	{   -230,  14 }, //  197
	{    -33,  13 }, //  208
	{    175,  12 }, //  221
	{    396,  11 }, //  238
	{    634,  10 }, //  256
	{    890,   9 }, //  279
	{   1169,   8 }, //  308
	{   1477,   7 }, //  343
	{   1820,   6 }, //  393
	{   2213,   5 }, //  459
	{   2672,   4 }, //  562
	{   3234,   3 }, //  730
	{   3964,   2 }, // 1036
	{   5000,   1 }, // 1599
	{   6599,   0 }, // 3401
	{  10000,   0 }  // upper border
	// 113+1 records total
};

//-----------------------------------------------------------------------------

int GetVr1DiffByTab ( int diff ) // diff = VR(winner) - VR(loser)
{
    int i = 0;
    int j = sizeof(vr1_diff_tab) / sizeof(*vr1_diff_tab) - 2;
    DASSERT( vr1_diff_tab[i].diff   == -10000 );
    DASSERT( vr1_diff_tab[j+1].diff ==  10000 );

 #if HAVE_PRINT
    {
	static int done = 0;
	PRINT_IF(!done++,
		"vr1_diff_tab: %d,%d => %d,%d / %d,%d\n",
		vr1_diff_tab[i].diff, vr1_diff_tab[i].add_vr,
		vr1_diff_tab[j].diff, vr1_diff_tab[j].add_vr,
		vr1_diff_tab[j+1].diff, vr1_diff_tab[j+1].add_vr );
    }
 #endif

    if ( diff <= -10000 )
	return vr1_diff_tab[0].add_vr;
    if ( diff >= 10000 )
	return vr1_diff_tab[j].add_vr;

    while ( i < j )
    {
	const int k = (i+j)/2;
	if ( diff < vr1_diff_tab[k].diff )
	    j = k-1;
	else if ( diff >= vr1_diff_tab[k+1].diff )
	    i = k+1;
	else
	    return vr1_diff_tab[k].add_vr;
    }

    return vr1_diff_tab[i].add_vr;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static const s16 vr2_diff_tab[112] =
{
          6598,  4999,  3963,  3233,  2671,  2212,  1819,  1476,  //   0 ..   7
          1168,   889,   633,   395,   174,   -34,  -231,  -418,  //   8 ..  15
          -596,  -767,  -931, -1090, -1243, -1392, -1536, -1676,  //  16 ..  23
         -1813, -1946, -2076, -2203, -2328, -2450, -2569, -2687,  //  24 ..  31
         -2802, -2915, -3026, -3136, -3243, -3349, -3454, -3557,  //  32 ..  39
         -3658, -3759, -3857, -3955, -4051, -4147, -4241, -4334,  //  40 ..  47
         -4425, -4516, -4606, -4695, -4783, -4870, -4957, -5042,  //  48 ..  55
         -5127, -5211, -5294, -5377, -5459, -5540, -5622, -5702,  //  56 ..  63
         -5783, -5863, -5942, -6022, -6101, -6180, -6259, -6338,  //  64 ..  71
         -6417, -6495, -6574, -6653, -6731, -6810, -6889, -6968,  //  72 ..  79
         -7048, -7127, -7207, -7287, -7367, -7448, -7530, -7611,  //  80 ..  87
         -7693, -7776, -7860, -7944, -8029, -8114, -8201, -8288,  //  88 ..  95
         -8377, -8466, -8557, -8649, -8743, -8838, -8935, -9034,  //  96 .. 103
         -9135, -9239, -9345, -9455, -9568, -9684, -9806, -9933,  // 104 .. 111
};

///////////////////////////////////////////////////////////////////////////////

int GetVr2DiffByTab ( int diff ) // diff = VR(winner) - VR(loser)
{
    if ( diff > 6598 )
	return 0;
    if ( diff < -9933 )
	return 112;

    const s16 search = diff;
    int i = 0, j = sizeof(vr2_diff_tab) / sizeof(*vr2_diff_tab) - 1;
    
    while ( i < j )
    {
	const int k = (i+j)/2;
	if ( search > vr2_diff_tab[k] )
	    j = k-1;
	else if ( search <= vr2_diff_tab[k+1] )
	    i = k+1;
	else
	    return k+1;
    }

    return i+1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_vr2 ( int argc, char ** argv )
{
    if ( argc > 1 && !strcmp(argv[1],"-c") )
    {
	int i, j, vr = 0, n = sizeof(vr1_diff_tab) / sizeof(*vr1_diff_tab) - 1;
	printf("\nstatic const int16_t vr2_diff_tab[%u] =\n{\n",n-1);

	for ( i = n-1; i > 0; i -= 8, vr += 8 )
	{
	    putchar('\t');
	    for ( j = 0; j < 8; j++ )
	    {
		int idx = i - j;
		if ( idx > 0 )
		    printf("%6d,", vr1_diff_tab[idx].diff - 1 );
	    }
	    printf("  // %3u .. %3u\n",vr,vr+7);
	}
	printf("};\n\n");
	return ERR_OK;
    }

    const int from = -10000;
    const int to   =  10000;
    printf(">>> Compare 3x VR from %d to %d <<<\n",from,to);

    int diff, err_count = 0;
    for ( diff = from; diff <= to; diff++ )
    {
	const int vr0 = GetVrDiffByTab(diff);
	const int vr1 = GetVr1DiffByTab(diff);
	const int vr2 = GetVr2DiffByTab(diff);

	if ( vr0 != vr1 || vr1 != vr2 )
	{
	    if ( ++err_count < 20 )
		printf("ERR: diff %6d : vr012 %3d %3d %3d\n",diff,vr0,vr1,vr2);
	}
    }

    if (err_count)
	printf("!!! %u error%s total!\n",err_count,err_count==1?"":"s");
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_regex()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_regex ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
	fprintf(stderr,"\nUsage: wtest RE|REGEXP reg_expr string...\n\n");
	return ERR_SYNTAX;
    }

    Regex_t re;
    enumError err = ScanRegex(&re,true,argv[1]);
    if ( err || !re.valid )
	return ERROR0(err,"Invalid regex: %s\n",argv[1]);

    struct { FastBuf_t b; char space[500]; } res;
    InitializeFastBuf(&res,sizeof(res));

    int i;
    for ( i = 2; i < argc; i++ )
    {
	ccp arg = argv[i];
	int stat = ReplaceRegex(&re,&res.b,arg,-1);
	printf("\n%s\n -> [%d] %s\n",arg,stat,GetFastBufString(&res.b));
    }

    putchar('\n');
    ResetFastBuf(&res.b);
    ResetRegex(&re);

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_ct_mode()			///////////////
///////////////////////////////////////////////////////////////////////////////

static void test_ct_mode_helper ( ct_mode_t mode, bool show_id )
{
    ct_bmg_t ctb;
    SetupCtBMG(&ctb,mode,mode);
    putchar('\n');
    if (show_id)
	printf("%s\n",GetCtBMGIdentification(&ctb,true));
    DumpCtBMG(stdout,0,&ctb);
}

static enumError test_ct_mode ( int argc, char ** argv )
{
    bool show_id = false;
    int i;
    for ( i = 1; i < argc; i++ )
    {
	ccp arg = argv[i];
	if (!strcmp(arg,"-l"))
	    show_id = true;
    }

    test_ct_mode_helper(CTM_NINTENDO,show_id);
    test_ct_mode_helper(CTM_CTCODE,show_id);
    test_ct_mode_helper(CTM_LECODE1,show_id);
    test_ct_mode_helper(CTM_LECODE2,show_id);
    putchar('\n');
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_scan_hex()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_scan_hex ( int argc, char ** argv )
{
    PRINT_SIZEOF1(VarMode_t);
    PRINT_SIZEOF1(IntMode_t);
    PRINT_SIZEOF1(Var_t);
    PRINT_SIZEOF1(FastBuf_t);

    PRINT1("offsetof(Var_t,name)     = %zu\n",offsetof(Var_t,name));
    PRINT1("offsetof(Var_t,mode)     = %zu\n",offsetof(Var_t,mode));
    PRINT1("offsetof(Var_t,int_mode) = %zu\n",offsetof(Var_t,int_mode));
    PRINT1("offsetof(Var_t,i)        = %zu\n",offsetof(Var_t,i));

    FastBuf_t fb;
    InitializeFastBuf(&fb,1000);

    int i;
    for ( i = 1; i < argc; i++ )
    {
	char name[20];
	snprintf(name,sizeof(name),"Param #%u",i);

	ScanInfo_t si;
	InitializeSI(&si,argv[i],strlen(argv[i]),name,0);

	ClearFastBuf(&fb);
	ScanHexlineSI(&si,&fb,true);
	HexDump16(stdout,0,0,fb.buf,fb.ptr-fb.buf);
	putchar('\n');
	CheckEolSI(&si);
	ResetSI(&si);
    }

    ResetFastBuf(&fb);
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_shift_jis()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_shift_jis ( int argc, char ** argv )
{
    printf("\nShift-JIS: %s\n\n",GetShiftJISStatistics());

    {
	static char info[] = "|\x82\x4a|\x82\x50|";
	HexDump16(stdout,0,0,info,sizeof(info));

	cucp src0 = (cucp)info;
	cucp src1 = (cucp)info;
	cucp end1 = src1 + strlen(info);
	while ( *src0 || *src1 )
	{
	    u16 code0 = ScanShiftJISChar(&src0);
	    u16 code1 = ScanShiftJISCharE(&src1,end1);
	    u16 ch0   = GetShiftJISChar(code0);
	    u16 ch1	  = GetShiftJISChar(code1);

	    printf("%4x %4x : %3zd %3zd : %4x %4x\n",
		    code0, code1, src0-(cucp)info, src1-(cucp)info, ch0, ch1 );
	}
    }

    printf("\n------------------------------------------\n");

    {
	static u16 info2[] = { 0x7199, 0xff10, 0xff80, 0xff9f, 0xffff, 0 };
	char buf[100], *dest = buf;

	const u16 *p;
	for ( p = info2; *p; p++ )
	{
	    u16 code = GetShiftJISChar(*p);
	    printf("%4x -> %4x\n",*p,code);

	    if ( code >= 0x100 )
	    {
		write_be16(dest,code);
		dest += 2;
	    }
	    else if ( code > 0 )
		*dest++ = code;
	}
	*dest++= 0;
	HexDump16(stdout,0,0,buf,dest-buf);

	cucp ptr = (cucp)buf;
	while (*ptr)
	{
	    u16 code = ScanShiftJISChar(&ptr);
	    u16 ch   = GetShiftJISChar(code);

	    printf("%4x : %3zd : %4x\n", code, ptr-(cucp)buf, ch );
	}
    }

    printf("\n------------------------------------------\n");

    {
	static char info[] = "|\xff\x80|";
	HexDump16(stdout,0,0,info,sizeof(info));

	cucp src0 = (cucp)info;
	cucp src1 = (cucp)info;
	cucp end1 = src1 + strlen(info);
	while ( *src0 || *src1 )
	{
	    u16 beg0  = be16(src0);
	    u16 beg1  = be16(src1);
	    u16 code0 = ScanShiftJISChar(&src0);
	    u16 code1 = ScanShiftJISCharE(&src1,end1);
	    u16 ch0   = GetShiftJISChar(code0);
	    u16 ch1	  = GetShiftJISChar(code1);

	    printf(" %4x %4x : %4x %4x : %3zd %3zd : %4x %4x\n",
		    beg0, beg1, code0, code1,
		    src0-(cucp)info, src1-(cucp)info, ch0, ch1 );
	}
    }

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_item_slot()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_item_slot ( int argc, char ** argv )
{
    int i;
    for ( i = 1; i < argc; i++ )
    {
	u8 *data;
	uint size;
	ccp fname;
	enumError err = OpenReadFILE(argv[i],0,false,&data,&size,&fname,0);
	if (err)
	    continue;

	printf("\n\e[36;1m----- %s [%u] -----\e[0m\n",fname,size);
	u8 *ptr = data;
	u8 *end = data+size;
	uint n_tab = *ptr++;
	printf("%u tables\n",n_tab);
	uint t;
	for ( t = 0; t < n_tab; t++ )
	{
	    uint tabsize = ptr[0] * ptr[1];
	    u8 *nexttab = ptr + tabsize + 2;
	    if ( nexttab > end )
		break;

	    printf("%2u/%u: %2u*%-2u %03zx..%03zx/%x\n",
		t, n_tab, ptr[0], ptr[1], ptr - data, nexttab - data, size );
	    ptr = nexttab;
	}
	FREE(data);
    }

    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			Since 2001			///////////////
///////////////////////////////////////////////////////////////////////////////
// hour support

#if 0 // [[?]] ???
int hour2elapsed ( $hour = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time();
    $ref = self::time2hour($ref_time);
    if ( !isset($hour) || $hour == $ref )
	return ( $ref_time - self::hour2time($ref) ) / self::DUR_HOUR;

    return $hour < $ref ? 1.0 : 0.0;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// day support

#if 0 // [[?]] ???
int day2elapsed ( $day = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time();
    $ref = self::time2day($ref_time);
    if ( !isset($day) || $day == $ref )
	return ( $ref_time - self::day2time($ref) ) / self::DUR_DAY;

    return $day < $ref ? 1.0 : 0.0;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// week support

#if 0 // [[?]] ???
int week2elapsed ( $week = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time();
    $ref = self::time2week($ref_time);
    if ( !isset($week) || $week == $ref )
	return ( $ref_time - self::week2time($ref) ) / self::DUR_WEEK;

    return $week < $ref ? 1.0 : 0.0;
}
#endif

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#if 0 // [[?]] ???
int GetWeekInfoByYear ( $year )
{
    $r = new \stdClass;
    $r->year = intval($year);

    $tim = gmmktime( 12,0,0, 1,4,$r->year );
    $r->first_week = self::time2week($tim);

    $tim = gmmktime( 12,0,0, 12,28,$r->year );
    $r->last_week = self::time2week($tim);

    return $r;
}
#endif

///////////////////////////////////////////////////////////////////////////

#if 0 // [[?]] ???
int GetWeekYearByWeek ( $week )
{
    $l = explode(',',gmstrftime('%G,%V',
			    self::week2time($week) + self::DUR_DAY*3 ));

    $r = new \stdClass;
    $r->year = intval($l[0]);	// real year number
    $r->week = intval($l[1]);	// 1..12
    return $r;
}
#endif

///////////////////////////////////////////////////////////////////////////

#if 0 // [[?]] ???
int GetWeekYearByTime ( $time )
{
    $l = explode(',',gmstrftime('%G,%V,%u',$time));

    $r = new \stdClass;
    $r->year    = intval($l[0]);	// real year number
    $r->week    = intval($l[1]);	// 1..12
    $r->weekday = intval($l[2]);	// 1=monday .. 7=sunday
    return $r;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// month support

#if 0 // [[?]] ???
int month2elapsed ( $month = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time($ref_time);
    $ref = self::time2month($ref_time);
    if ( !isset($month) || $month == $ref )
    {
	$beg = self::month2time($ref);
	return ( $ref_time - $beg ) / ( self::month2time($ref+1) - $beg );
    }

    return $month < $ref ? 1.0 : 0.0;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// quarter support

#if 0 // [[?]] ???
int quarter2elapsed ( $quarter = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time();
    $ref = self::time2quarter($ref_time);
    if ( !isset($quarter) || $quarter == $ref )
    {
	$beg = self::quarter2time($ref);
	return ( $ref_time - $beg ) / ( self::quarter2time($ref+1) - $beg );
    }

    return $quarter < $ref ? 1.0 : 0.0;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// year support

#if 0 // [[?]] ???
int year2elapsed ( $year = NULL, $ref_time = NULL )
{
    if (!$ref_time)
	$ref_time = time($ref_time);
    $ref = self::time2year($ref_time);
    if ( !isset($year) || $year == $ref )
    {
	$beg = self::year2time($ref);
	return ( $ref_time - $beg ) / ( self::year2time($ref+1) - $beg );
    }

    return $year < $ref ? 1.0 : 0.0;
}
#endif

//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// misc

#if 0 // [[?]] ???
int GetAverageDuration ( $period )
{
    // Values for day and week are exact.
    // Values for month, quarter and year are based on 365.2425 days/year.

    switch ($period)
    {
	case 'hour':	return self::DUR_HOUR; break;
	case 'day':	return self::DUR_DAY; break;
	case 'week':	return self::DUR_WEEK; break;
	case 'month':	return self::DUR_MONTH; break;
	case 'quarter':	return self::DUR_QUARTER; break;
	case 'year':	return self::DUR_YEAR; break;
    }
    return false;
}
#endif

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_since2001()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_since2001 ( int argc, char ** argv )
{
    uint i;
    for ( i = 1; i < argc; i++ )
    {
	s64 time0 = argv[i][0] == 'n' ? GetTimeSec(false)
		  : argv[i][0] == '+' ? 978307200
		  : str2l(argv[i],0,10);
	printf("\n\e[36m %s -> %lld -> %s \e[0m\n",
		argv[i], time0, PrintTimeByFormat("%F %T",time0) );

	int num;
	s64 tim;
	ccp t1, t2, format = "%c:%7d  %-10s  %-13s  %-19s  %9u\n";

	//--- hour

	num = time2hour(time0);
	tim = hour2time(num);
	t1  = hour2text(num,0,0,0);
	t2  = hour2text(num,99,0,0);
	printf(format,'H',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		hour2duration(num));

	t1  = hour2text(num,10,0,0);
	t2  = hour2text(num,11,0,0);
	printf(format,'H',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		hour2duration(num));

	//--- day

	num = time2day(time0);
	tim = day2time(num);
	t1  = day2text(num,0,0,0);
	t2  = day2text(num,99,0,0);
	printf(format,'D',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		day2duration(num));

	//--- week

	num = time2week(time0);
	tim = week2time(num);
	t1  = week2text(num,0,0,0);
	t2  = week2text(num,99,0,0);
	printf(format,'W',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		week2duration(num));

	//--- month

	num = time2month(time0);
	tim = month2time(num);
	t1  = month2text(num,0,0,0);
	t2  = month2text(num,99,0,0);
	printf(format,'M',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		month2duration(num));

	//--- quarter

	num = time2quarter(time0);
	tim = quarter2time(num);
	t1  = quarter2text(num,0,0,0);
	t2  = quarter2text(num,99,0,0);
	printf(format,'Q',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		quarter2duration(num));

	//--- year

	num = time2year(time0);
	tim = year2time(num);
	t1  = year2text(num,0,0,0);
	t2  = year2text(num,99,0,0);
	printf(format,'Y',num,t1,t2,
		PrintTimeByFormat("%F %T",tim),
		year2duration(num));
    }
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_timezone()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_timezone ( int argc, char ** argv )
{
    printf("\nEndless timezone test with interval of 30 minutes started (pid=%u)\n",
		getpid() );
    printf("Typical usage: nohup ./%s TIMEZONE >>timezone-test.tmp 2>&1 &\n\n",progname);

    for(;;)
    {
	u_usec_t start = GetTimerUSec();
	ccp t1 = PrintTimevalByFormatUTC("%F %T.@@@@@@ %z",0);
	ccp t2 = PrintTimevalByFormat("%F %T.@@@@@@ %z",0);
	u_usec_t dur = GetTimerUSec() - start;
	printf("%s : %s : in %llu usec\n",t1,t2,dur);
	fflush(stdout);
	sleep(1800);
    }
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_condition()		///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_condition ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
	fprintf(stderr,
		"\n"
		"Usage: wtest 'CO' arg...\n"
		"   or: wtest 'CONDITION' arg...\n"
		"ARG is one of: hex | 'r' | 's<NUM>' | 'max' \n"
		"\n"
		);
	return 0;
    }

    int setting = -1;
    uint argi;
    for ( argi = 1; argi < argc; argi++ )
    {
	ccp arg = argv[argi];
	if ( *arg == 's' )
	{
	    setting = strtol(arg+1,0,10);
	    continue;
	}

	if (!strcmp(arg,"max"))
	{
	    uint max = 0, mask;
	    for ( mask = 0; mask <= 0xffff; mask++ )
	    {
		gobj_condition_set_t gcs;
		const uint n = GetGobjConditions(&gcs,setting,mask);
		if ( max < n )
		    max = n;
	    }
	    printf("Max conditions for setting %2d: %u/%u\n",
			setting, max, GOBJ_COND_SET_MAX );
	    continue;
	}

	const u16 mask = strcmp(arg,"r") ? str2ul(arg,0,16) : MyRandom(0x10000);
	gobj_condition_set_t gcs;
	const uint n = GetGobjConditions(&gcs,setting,mask);
	printf("\n0x%04x = %u [setting %d -> %02x]\n",
		mask, mask, setting, gcs.setting_mask );

	uint i;
	for ( i = 0; i < n; i++ )
	{
	    const gobj_condition_t *c = gcs.cond+i;
	    printf("  %04x.%02x %2d : %2d..%-2d %2d..%-2d : %s\n",
		c->mask,
		c->setting,
		c->bitnum,
		c->min_offline,
		c->max_offline,
		c->min_online,
		c->max_online,
		c->info );
	}
    }
    putchar('\n');
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_xbit()			///////////////
///////////////////////////////////////////////////////////////////////////////

int GetXPFBitNum ( int is_online, int n_offline, int n_online )
{
    if ( is_online > 0 )
    {
	int base = n_online <  7 ?  4
		 : n_online < 10 ?  6
		 : n_online < 13 ?  8
		 : n_online < 19 ? 10
		 :		   12;
	return base + ( n_offline > 1 );
    }
    else
    {
	return n_offline < 2 ? 0
	     : n_offline > 4 ? 3
	     : n_offline - 1;
    }
}

//-----------------------------------------------------------------------------

static enumError test_xbit ( int argc, char ** argv )
{
    if ( argc < 2 )
    {
	fprintf(stderr,
		"\n"
		"Usage: wtest 'XBIT' tupel..\n"
		"TUPEL is: id_online,n_offline,n_online\n"
		"Default is 0 for each param.\n"
		"\n"
		);
	return 0;
    }

    uint argi;
    for ( argi = 1; argi < argc; argi++ )
    {
	int is_online = 0, n_offline = 0, n_online = 0;

	ccp arg = argv[argi];
	is_online = str2l(arg,0,10);
	arg = strchr(arg,',');
	if (arg)
	{
	    n_offline = str2l(arg+1,0,10);
	    arg = strchr(arg+1,',');
	    if (arg)
		n_online = str2l(arg+1,0,10);
	}

	int bitnum = GetXPFBitNum(is_online,n_offline,n_online);
	printf("%3d %3d %3d => %2d\n",is_online,n_offline,n_online,bitnum);
    }
    putchar('\n');
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			test_in_arch()			///////////////
///////////////////////////////////////////////////////////////////////////////

static enumError test_in_arch ( int argc, char ** argv )
{
    int total = 0, differ = 0;
    const DbFileFILE_t *ptr;
    for ( ptr = DbFileFILE; ptr->file; ptr++ )
    {
	total++;
	bool old = OLD_DBF_IN_ARCHIVE(ptr->flags);
	bool new = DBF_ARCH_REQUIRED(ptr->flags);
	if ( old != new )
	{
	    differ++;
	    printf("old=%d, new=%d : %05x : %s\n",
			old, new, ptr->flags, ptr->file );
	}
    }
    printf(" %u/%u differ\n",differ,total);
    return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			develop()			///////////////
///////////////////////////////////////////////////////////////////////////////

static void test_dropfastbuf ( FastBuf_t *fb, int index, int size )
{
    AssignFastBuf(fb,"0123456789",-1);
    DropFastBuf(fb,index,size);
    printf("DROP(,%d,%d): %s: %s\n",
	index, size, GetFastBufStatus(fb), GetFastBufString(fb) );
}

//-----------------------------------------------------------------------------

const gobj_cond_ref_t * FindConditionRef2 ( u16 ref_id )
{
    const gobj_cond_ref_t *rc = cond_ref_tab;
    for ( ; rc->ref_id; rc++ )
	if ( rc->ref_id == ref_id )
	    return rc;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

static enumError develop ( int argc, char ** argv )
{
 #if 1
    static const u16 tab[] = { 0x1000,0x103f, 0x1900,0x193f, 0 };
    const u16 *ptr;

   #if 1
    fputs("\nCompare results of FindConditionRef() and FindConditionRef2()\n",stdout);
    for ( ptr = tab; *ptr; ptr += 2 )
    {
	u16 ref_id;
	for ( ref_id = ptr[0]; ref_id <= ptr[1]; ref_id++ )
	{
	    const gobj_cond_ref_t *r1 = FindConditionRef(ref_id);
	    const gobj_cond_ref_t *r2 = FindConditionRef2(ref_id);
	    if ( r1 != r2 )
		printf("DIFFER: %#06x : %#6x %#6x\n",
			ref_id, r1 ? r1->ref_id : 0, r2 ? r2->ref_id : 0 );
	}
    }
   #endif
    putchar('\n');

    const int N1000 = 100;
    for ( ptr = tab; *ptr; ptr += 2 )
    {
	uint i;
	u64 duration;

	duration = -GetTimerUSec();
	for ( i = 0; i < 1000*N1000; i++ )
	{
	    u16 ref_id;
	    for ( ref_id = ptr[0]; ref_id <= ptr[1]; ref_id++ )
		FindConditionRef(ref_id);
	}
	duration += GetTimerUSec();
	printf( " %s total, %s/1000, %u*1000 * FindConditionRef (%#x..%#x)\n",
		PrintTimerUSec6(0,0,duration,true),
		PrintTimerUSec6(0,0,duration/N1000/(ptr[1]-ptr[0]+1),true),
		N1000, ptr[0], ptr[1] );

	duration = -GetTimerUSec();
	for ( i = 0; i < 1000*N1000; i++ )
	{
	    u16 ref_id;
	    for ( ref_id = ptr[0]; ref_id <= ptr[1]; ref_id++ )
		FindConditionRef2(ref_id);
	}
	duration += GetTimerUSec();
	printf( " %s total, %s/1000, %u*1000 * FindConditionRef2(%#x..%#x)\n",
		PrintTimerUSec6(0,0,duration,true),
		PrintTimerUSec6(0,0,duration/N1000/(ptr[1]-ptr[0]+1),true),
		N1000, ptr[0], ptr[1] );
    }
    putchar('\n');

 #elif 1
    static int ff_list[] = { FF_CHR, FF_CLR, FF_MDL, FF_PAT, FF_SCN, FF_SHP,
				FF_SRT, FF_TEX, FF_TEX_CT, 0 };
    int *ff;
    for ( ff = ff_list; *ff; ff++ )
    {
	printf("--- %s ---\n",GetNameFF(0,*ff));
	int v;
	for ( v = 0; v < 20; v++ )
	{
	    const brsub_info_t *bi = GetInfoBRSUB(*ff,v);
	    int ns = GetKnownSectionNumBRSUB(*ff,v);
	    if ( ( bi ? bi->n_sect : -1 ) != ns )
		printf("FAIL: %s v%u: %d != %d\n",
			GetNameFF(0,*ff), v,
			bi ? bi->n_sect : -1, ns );
	}
    }

 #elif 1

    kmp_special_t special;
    memset(special,0,sizeof(special));
    u8 *ptr = (u8*)special;
    ptr[0] = 1;
    ptr[1] = 2;
    printf("size=%zd : %zd : %d,%d: %d,%d\n",
	sizeof(special), (u8*)(special+1) - (u8*)(special),
	special[0], special[1], ptr[0], ptr[1] );

 #elif 1

    FastBuf_t fb1;
    InitializeFastBuf(&fb1,sizeof(fb1));
    printf("fb1: %s\n",GetFastBufStatus(&fb1));

    struct { FastBuf_t b; char space[2000]; } fb2;
    InitializeFastBuf(&fb2,sizeof(fb2));
    printf("fb2: %s\n",GetFastBufStatus(&fb2.b));

    char fb3x[2000];
    FastBuf_t *fb3 = InitializeFastBuf(&fb3x,sizeof(fb3x));
    printf("fb3: %s\n",GetFastBufStatus(fb3));

    AppendFastBuf(&fb1,"aaa",-1); printf("fb1: %s: %s\n",GetFastBufStatus(&fb1),fb1.buf);
    AppendFastBuf(&fb1,"BB",-1); printf("fb1: %s: %s\n",GetFastBufStatus(&fb1),fb1.buf);
    InsertFastBuf(&fb1,1,"111",-1); printf("fb1: %s: %s\n",GetFastBufStatus(&fb1),fb1.buf);
    InsertFastBuf(&fb1,-1,"222",-1); printf("fb1: %s: %s\n",GetFastBufStatus(&fb1),fb1.buf);

    test_dropfastbuf(&fb1,12,4);
    test_dropfastbuf(&fb1, 8,4);
    test_dropfastbuf(&fb1, 3,4);
    test_dropfastbuf(&fb1,-9,4);
    test_dropfastbuf(&fb1,-15,4);
    test_dropfastbuf(&fb1,5,-3);
    test_dropfastbuf(&fb1,5,-8);

 #elif 1
    if ( argc > 2 )
    {
	double d1 = strtod(argv[1],0);
	double d2 = strtod(argv[2],0);

	int i;
	for ( i = -5; i< 60; i += 5 )
	{
	    bool sf = IsSameF(d1,d2,i);
	    bool sd = IsSameD(d1,d2,i);
	    printf(" %3i: %d %d %12.10f %12.10f\n",
			i, sf, sd, d1, d2 );
	}
    }

 #else
    int i;
    for ( i = 1; i < argc; i++ )
    {
	char buf[1000];
	uint len = EncodeBase64(buf,sizeof(buf),argv[i],strlen(argv[i]),
			TableEncode64x, true, "\n\t", 10 );
	printf("-----\n%s [%zu->%u]\n\t%s\n",argv[i],strlen(argv[i]),len,buf);
    }
 #endif
    return ERR_OK;
}

//
///////////////////////////////////////////////////////////////////////////////
///////////////			command definitions		///////////////
///////////////////////////////////////////////////////////////////////////////

enum
{
    CMD_HELP,			// help_exit()
    CMD_TEST,			// test()
    CMD_DEVELOP,		// develop(argc,argv)
    CMD_WIIMM,			// test_wiimm(argc,argv)

    CMD_FLOAT,			// test_float(argc,argv)
    CMD_FLOAT_TAB,		// create_float_tab()
    CMD_MATRIX,			// test_matrix(argc,argv)
    CMD_MATCH_PATTERN,		// test_match_pattern(argc,argv)
    CMD_MEDIAN_CUT,		// test_median_cut(argc,argv)
    CMD_CMPR,			// test_cmpr(argc,argv)
    CMD_CREATE_IMAGES,		// create_images(argc,argv)
    CMD_RESIZE,			// test_resize(argc,argv)
    CMD_RESIZE_IMAGE,		// resize_image(argc,argv)
    CMD_SCAN_TEXT,		// test_scan_text(argc,argv)
    CMD_SCAN_STRING,		// test_scan_string(argc,argv)
    CMD_SCAN_FILE,		// test_scan_file(argc,argv)
    CMD_FILENAME,		// test_filename(argc,argv)
    CMD_DRAW_STARTPOS,		// draw_startpos(argc,argv)
    CMD_PATH_CMP,		// test_path_cmp(argc,argv)
    CMD_LINE2,			// test_line(2,argc,argv)
    CMD_LINE3,			// test_line(3,argc,argv)
    CMD_OBJFLOW,		// ana_file_objflow(argc,argv)
    CMD_MEMFILE,		// test_memfile(argc,argv)
    CMD_FIND_DOL,		// test_find_dol(argc,argv)
    CMD_SUBFILE,		// test_subfile(argc,argv)
    CMD_STR_LIST,		// test_str_list(argc,argv)
    CMD_CONTAINER,		// test_container(argc,argv)
    CMD_UUID,			// test_uuid(argc,argv)
    CMD_ANA_MAGIC,		// test_analyse_magic(argc,argv)
    CMD_VR,			// test_vr(argc,argv)
    CMD_VR2,			// test_vr2(argc,argv)
    CMD_REGEX,			// test_regex(argc,argv)
    CMD_CT_MODE,		// test_ct_mode(argc,argv)
    CMD_SCAN_HEX,		// test_scan_hex(argc,argv)
    CMD_SHIFT_JIS,		// test_shift_jis(argc,argv)
    CMD_ITEMSLOT,		// test_item_slot(argc,argv)
    CMD_SINCE2001,		// test_since2001(argc,argv)
    CMD_TIMEZONE,		// test_timezone(argc,argv)
    CMD_CONDITION,		// test_condition(argc,argv)
    CMD_XBIT,			// test_xbit(argc,argv)
    CMD_IN_ARCH,		// test_in_arch(argc,argv)

    CMD__N
};

///////////////////////////////////////////////////////////////////////////////

static const KeywordTab_t CommandTab[] =
{
	{ CMD_HELP,		"HELP",		"?",		0 },
	{ CMD_TEST,		"TEST",		"T",		0 },
	{ CMD_DEVELOP,		"DEVELOP",	"D",		0 },

 #ifdef HAVE_WIIMM_EXT
	{ CMD_WIIMM,		"WIIMM",	"W",		0 },
 #endif

	{ CMD_FLOAT,		"FLOAT",	"F",		0 },
	{ CMD_FLOAT_TAB,	"FLOAT-TAB",	"FT",		0 },
	{ CMD_MATRIX,		"MATRIX",	0,		0 },
	{ CMD_MATCH_PATTERN,	"MATCH",	0,		0 },
	{ CMD_MEDIAN_CUT,	"MEDIAN-CUT",	"MC",		0 },
	{ CMD_CMPR,		"CMPR",		0,		0 },
	{ CMD_CREATE_IMAGES,	"CREATE-IMAGES","CI",		0 },
	{ CMD_RESIZE,		"RESIZE",	"RES",		0 },
	{ CMD_RESIZE_IMAGE,	"RESIZE-IMAGE",	"RI",		0 },
	{ CMD_SCAN_TEXT,	"SCAN-TEXT",	"ST",		0 },
	{ CMD_SCAN_STRING,	"SCAN-STRING",	"SS",		0 },
	{ CMD_SCAN_FILE,	"SCAN-FILE",	"SF",		0 },
	{ CMD_FILENAME,		"FILENAME",	"FNAME",	0 },
	{ CMD_DRAW_STARTPOS,	"DRAW-STARTPOS","DSP",		0 },
	{ CMD_PATH_CMP,		"PATH-CMP",	"PC",		0 },
	{ CMD_LINE2,		"LINE2",	"L2",		0 },
	{ CMD_LINE3,		"LINE3",	"L3",		0 },
	{ CMD_OBJFLOW,		"ANA-OBJFLOW",	"AOF",		0 },
	{ CMD_MEMFILE,		"MEMFILE",	"MF",		0 },
	{ CMD_FIND_DOL,		"FIND-DOL",	"FD",		0 },
	{ CMD_SUBFILE,		"SUBFILE",	0,		0 },
	{ CMD_STR_LIST,		"STR-LIST",	"SL",		0 },
	{ CMD_CONTAINER,	"CONTAINER",	0,		0 },
	{ CMD_UUID,		"UUID",		0,		0 },
	{ CMD_ANA_MAGIC,	"ANA-MAGIC",	"ANAMAGIC",	0 },
	{ CMD_VR,		"VR",		0,		0 },
	{ CMD_VR2,		"VR2",		0,		0 },
	{ CMD_REGEX,		"REGEXP",	"RE",		0 },
	{ CMD_CT_MODE,		"CT-MODE",	"CTM",		0 },
	{ CMD_SCAN_HEX,		"SCAN-HEX",	"SH",		0 },
	{ CMD_SHIFT_JIS,	"SHIFT-JIS",	"SJIS",		0 },
	{ CMD_ITEMSLOT,		"ITEM-SLOT",	"IS",		0 },
	{ CMD_SINCE2001,	"SINCE2001",	"S2",		0 },
	{ CMD_TIMEZONE,		"TIMEZONE",	"TZ",		0 },
	{ CMD_CONDITION,	"CONDITION",	"CO",		0 },
	{ CMD_XBIT,		"XBIT",		0,		0 },
	{ CMD_IN_ARCH,		"IN-ARCH",	"INARCH",	0 },

	{ CMD__N,0,0,0 }
};

//
///////////////////////////////////////////////////////////////////////////////
///////////////			help_exit()			///////////////
///////////////////////////////////////////////////////////////////////////////

void help_exit()
{
    printf("\nCommands:\n\n");
    const KeywordTab_t * cmd;
    for ( cmd = CommandTab; cmd->name1; cmd++ )
	if (cmd->name2)
	    printf("  %-*s | %s\n",CMD1_FW,cmd->name1,cmd->name2);
	else
	    printf("  %s\n",cmd->name1);
    putchar('\n');
 #ifdef HAVE_WIIMM_EXT
    wiimm_help_exit(false);
 #endif
    exit(ERR_SYNTAX);
};

//
///////////////////////////////////////////////////////////////////////////////
///////////////			main()				///////////////
///////////////////////////////////////////////////////////////////////////////

//#define XXX 0x123
//#define MM1(a) #a
//#define MM2(a) MM1(a)

void AddOne ( double m[3][4] )
{
    m[0][0] += 1.0;
    m[1][1] += 1.0;
    m[2][2] += 1.0;
}

int main ( int argc, char ** argv )
{
    printf("*****  %s  *****\n",TITLE);
    print_title_func = print_title;
    SetupLib(argc,argv,NAME);
    fflush(stdout);

    //printf(" |" MM1(XXX) "|" MM2(XXX) "\n");

    if (0)
    {
	double34 x;
	x.m[0][0] = 10.0;
	x.m[1][1] = 20.0;
	x.m[2][2] = 30.0;
	printf(" %6.2f %6.2f %6.2f\n",x.m[0][0],x.m[1][1],x.m[2][2]);
	AddOne(x.m);
	printf(" %6.2f %6.2f %6.2f\n",x.m[0][0],x.m[1][1],x.m[2][2]);
    }

    printf("term width = %d\n",GetTermWidth(80,0));

    if ( argc < 2 )
	help_exit();

    int cmd_stat;
    const KeywordTab_t * cmd_ct = ScanKeyword(&cmd_stat,argv[1],CommandTab);
    if (!cmd_ct)
    {
	PrintKeywordError(CommandTab,argv[1],cmd_stat,0,0);
	help_exit();
    }

    argv[1] = argv[0];
    argv++;
    argc--;

    switch(cmd_ct->id)
    {
	case CMD_TEST:			test(argc,argv); break;
	case CMD_DEVELOP:		develop(argc,argv); break;

 #ifdef HAVE_WIIMM_EXT
	case CMD_WIIMM:			test_wiimm(argc,argv); break;
 #endif

	case CMD_FLOAT:			test_float(argc,argv); break;
	case CMD_FLOAT_TAB:		create_float_tab(); break;
	case CMD_MATRIX:		test_matrix(argc,argv); break;
	case CMD_MATCH_PATTERN:		test_match_pattern(argc,argv); break;
	case CMD_MEDIAN_CUT:		test_median_cut(argc,argv); break;
	case CMD_CMPR:			test_cmpr(argc,argv); break;
	case CMD_CREATE_IMAGES:		create_images(argc,argv); break;
	case CMD_RESIZE:		test_resize(argc,argv); break;
	case CMD_RESIZE_IMAGE:		resize_image(argc,argv); break;
	case CMD_SCAN_TEXT:		test_scan_text(argc,argv); break;
	case CMD_SCAN_STRING:		test_scan_string(argc,argv); break;
	case CMD_SCAN_FILE:		test_scan_file(argc,argv); break;
	case CMD_FILENAME:		test_filename(argc,argv); break;
	case CMD_DRAW_STARTPOS:		draw_startpos(argc,argv); break;
	case CMD_PATH_CMP:		test_path_cmp(argc,argv); break;
	case CMD_LINE2:			test_line(2,argc,argv); break;
	case CMD_LINE3:			test_line(3,argc,argv); break;
	case CMD_OBJFLOW:		ana_file_objflow(argc,argv); break;
	case CMD_MEMFILE:		test_memfile(argc,argv); break;
	case CMD_FIND_DOL:		test_find_dol(argc,argv); break;
	case CMD_SUBFILE:		test_subfile(argc,argv); break;
	case CMD_STR_LIST:		test_str_list(argc,argv); break;
	case CMD_CONTAINER:		test_container(argc,argv); break;
	case CMD_UUID:			test_uuid(argc,argv); break;
	case CMD_ANA_MAGIC:		test_analyse_magic(argc,argv); break;
	case CMD_VR:			test_vr(argc,argv); break;
	case CMD_VR2:			test_vr2(argc,argv); break;
	case CMD_REGEX:			test_regex(argc,argv); break;
	case CMD_CT_MODE:		test_ct_mode(argc,argv); break;
	case CMD_SCAN_HEX:		test_scan_hex(argc,argv); break;
	case CMD_SHIFT_JIS:		test_shift_jis(argc,argv); break;
	case CMD_ITEMSLOT:		test_item_slot(argc,argv); break;
	case CMD_SINCE2001:		test_since2001(argc,argv); break;
	case CMD_TIMEZONE:		test_timezone(argc,argv); break;
	case CMD_CONDITION:		test_condition(argc,argv); break;
	case CMD_XBIT:			test_xbit(argc,argv); break;
	case CMD_IN_ARCH:		test_in_arch(argc,argv); break;
	//case CMD_HELP:
	default:
	    help_exit();
    }

    if (SIGINT_level)
	ERROR0(ERR_INTERRUPT,"Program interrupted by user.");

    return max_error;
}

///////////////////////////////////////////////////////////////////////////////