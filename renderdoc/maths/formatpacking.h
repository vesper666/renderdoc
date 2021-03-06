/******************************************************************************
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Crytek
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/


#pragma once

#include <stdint.h>

#include "vec.h"

inline Vec4f ConvertFromR10G10B10A2(uint32_t data)
{
	return Vec4f( float((data>> 0) & 0x3ff) / 1023.0f,
				  float((data>>10) & 0x3ff) / 1023.0f,
				  float((data>>20) & 0x3ff) / 1023.0f,
				  float((data>>30) & 0x003) / 3.0f
				);
}

inline uint32_t ConvertToR10G10B10A2(Vec4f data)
{
	float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
	float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;
	float z = data.z < 1.0f ? (data.z > 0.0f ? data.z : 0.0f) : 1.0f;
	float w = data.w < 1.0f ? (data.w > 0.0f ? data.w : 0.0f) : 1.0f;

	return (uint32_t(x*1023)<< 0) |
		   (uint32_t(y*1023)<<10) |
		   (uint32_t(z*1023)<<20) |
		   (uint32_t(w*3)   <<30) ;
}

inline Vec3f ConvertFromR11G11B10(uint32_t data)
{
	uint32_t xMantissa = ((data>> 0) & 0x3f);
	uint32_t xExponent = ((data>> 6) & 0x1f);
	uint32_t yMantissa = ((data>>11) & 0x3f);
	uint32_t yExponent = ((data>>17) & 0x1f);
	uint32_t zMantissa = ((data>>22) & 0x1f);
	uint32_t zExponent = ((data>>27) & 0x1f);

	return Vec3f((float(xMantissa)/64.0f)*powf(2.0f, (float)xExponent - 15.0f),
				 (float(yMantissa)/32.0f)*powf(2.0f, (float)yExponent - 15.0f),
				 (float(zMantissa)/32.0f)*powf(2.0f, (float)zExponent - 15.0f));
}

/*
inline uint32_t ConvertToR11G11B10(Vec3f data)
{
	return 0;
}
*/

inline Vec4f ConvertFromB5G5R5A1(uint16_t data)
{
	return Vec4f(  (float)((data >> 0) & 0x1f) / 31.0f,
				   (float)((data >> 5) & 0x1f) / 31.0f,
				   (float)((data >> 10) & 0x1f) / 31.0f,
				   ((data & 0x8000) > 0) ? 1.0f : 0.0f );
}

inline Vec3f ConvertFromB5G6R5(uint16_t data)
{
	return Vec3f( (float)((data >> 0) & 0x1f) / 31.0f,
				  (float)((data >> 5) & 0x3f) / 63.0f,
				  (float)((data >> 11) & 0x1f) / 31.0f );
}

inline Vec4f ConvertFromB4G4R4A4(uint16_t data)
{
	return Vec4f( (float)((data >> 0) & 0xf) / 15.0f,
				  (float)((data >> 4) & 0xf) / 15.0f,
				  (float)((data >> 8) & 0xf) / 15.0f,
				  (float)((data >> 12) & 0xf) / 15.0f );
}

#include "half_convert.h"

