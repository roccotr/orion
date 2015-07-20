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

#include "CoreEndn.hpp"

typedef unsigned char byte;

bool BigEndianSystem;

short (*BigShort) ( short s );
short (*LittleShort) ( short s );
int (*BigLong) ( int i );
int (*LittleLong) ( int i );
float (*BigFloat) ( float f );
float (*LittleFloat) ( float f );
double (*BigDouble) ( double d );
double (*LittleDouble) ( double d );
int64vcl (*BigInt64) ( int64vcl i );
int64vcl (*LittleInt64) ( int64vcl i );


//adapted from Quake 2 source

short ShortSwap( short s ) {
	byte b1, b2;
	
	b1 = s & 255;
	b2 = (s >> 8) & 255;

	return (b1 << 8) + b2;
}

short ShortNoSwap( short s ) {
	return s;
}

int LongSwap (int i) {
	byte b1, b2, b3, b4;

	b1 = i & 255;
	b2 = ( i >> 8 ) & 255;
	b3 = ( i>>16 ) & 255;
	b4 = ( i>>24 ) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

int LongNoSwap( int i ) {
	return i;
}

float FloatSwap( float f ) {
	union
	{
		float f;
		byte b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

float FloatNoSwap( float f ) {
	return f;
}

double DoubleSwap( double d ) {
	union
	{
		double d;
		byte b[8];
	} dat1, dat2;

	dat1.d = d;
	dat2.b[0] = dat1.b[7];
	dat2.b[1] = dat1.b[6];
	dat2.b[2] = dat1.b[5];
	dat2.b[3] = dat1.b[4];
	dat2.b[4] = dat1.b[3];
	dat2.b[5] = dat1.b[2];
	dat2.b[6] = dat1.b[1];
	dat2.b[7] = dat1.b[0];
	return dat2.d;
}

double DoubleNoSwap( double d ) {
	return d;
}

int64vcl Int64Swap( int64vcl i ) {
	int64vcl iVreturn;

	UTswap_int64(iVreturn, i);
	return iVreturn;
}

int64vcl Int64NoSwap( int64vcl i ) {
	return i;
}

void InitEndian( void ) {
	//clever little trick from Quake 2 to determine the endian
	//of the current system without depending on a preprocessor define

	byte SwapTest[2] = { 1, 0 };
	
	if( *(short *) SwapTest == 1 )
	{
		//little endian
		BigEndianSystem = false;

		//set func pointers to correct funcs
		BigShort = ShortSwap;
		LittleShort = ShortNoSwap;
		BigLong = LongSwap;
		LittleLong = LongNoSwap;
		BigFloat = FloatSwap;
		LittleFloat = FloatNoSwap;
		BigDouble = DoubleSwap;
		LittleDouble = DoubleNoSwap;
		BigInt64 = Int64Swap;
		LittleInt64 = Int64NoSwap;
	}
	else
	{
		//big endian
		BigEndianSystem = true;

		BigShort = ShortNoSwap;
		LittleShort = ShortSwap;
		BigLong = LongNoSwap;
		LittleLong = LongSwap;
		BigFloat = FloatNoSwap;
		LittleFloat = FloatSwap;
		BigDouble = DoubleNoSwap;
		LittleDouble = DoubleSwap;
		BigInt64 = Int64NoSwap;
		LittleInt64 = Int64Swap;

	}
}