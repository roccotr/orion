/*
Copyright (C) 2003  Promit Roy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef COREENDN
#define COREENDN
#include "CoreVcl_.hpp"


#define UTswap_int16(x,z)  { x = (((z & 0xff) << 8) | ((z >> 8) & 0xff)); }
#define UTswap_int32(x,z)  { x = ((z & 0xff) << 24) | ((z & 0xff00) << 8) \
                              | ((z >> 8) & 0xff00) | ((z >> 24) & 0xff); }
#define UTswap_int64(x,z)  { x = (((((z      ) & 0xff) << 56) | \
                                (((z >>  8) & 0xff) << 48) | \
                                (((z >> 16) & 0xff) << 40) | \
                                (((z >> 24) & 0xff) << 32) | \
                                (((z >> 32) & 0xff) << 24) | \
                                (((z >> 40) & 0xff) << 16) | \
                                (((z >> 48) & 0xff) <<  8) | \
                                (((z >> 56) & 0xff)      ))); }
#define UTswap_int16_inplace(z) UTswap_int16(z,z)
#define UTswap_int32_inplace(z) UTswap_int32(z,z)
#define UTswap_int64_inplace(z) UTswap_int64(z,z)


//this file contains definitions for endian functionality
//NOTE: This only fixes endians, and is not complete for all
//architectures. It'll work where short is 2 bytes and int is 4 bytes
//If that's not the case, it'll probably explode.

//a BIG thanks to the Quake 2 source code here

extern bool BigEndianSystem;

void InitEndian( void );		//makes use of a clever trick in Quake 2

short ShortSwap( short s );
short ShortNoSwap( short s );

int LongSwap( int i );
int LongNoSwap( int i );

float FloatSwap( float f );
float FloatNoSwap( float f );

double DoubleSwap( double d );
double DoubleNoSwap( double d );

int64vcl Int64Swap( int64vcl i );
int64vcl Int64NoSwap( int64vcl i );

//Use these functions
extern short (*BigShort) ( short s );
extern short (*LittleShort) ( short s );
extern int (*BigLong) ( int i );
extern int (*LittleLong) ( int i );
extern float (*BigFloat) ( float f );
extern float (*LittleFloat) ( float f );
extern double (*BigDouble) ( double d );
extern double (*LittleDouble) ( double d );
extern int64vcl (*BigInt64) ( int64vcl i );
extern int64vcl (*LittleInt64) ( int64vcl i );



#endif