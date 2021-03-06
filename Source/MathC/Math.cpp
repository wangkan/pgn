#define PGN_DLL_EXPORT
#include <PGN/Math/Math.h>
#undef PGN_DLL_EXPORT

#include <float.h>
#include <math.h>
#include <PGN/Math/Utilities.h>

void pgn::mul(Float4x4* _a, Float4x4* _b, Float4x4* _result)
{
	Float4x4& a = *_a;
	Float4x4& b = *_b;
	Float4x4& r = *_result;

	for(int row = 0; row < 4; row++)
		for(int col = 0; col < 4; col++)
			r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2] + a[3][row] * b[col][3];
}

void pgn::mul(Float4x4* _a, Float4x3* _b, Float4x3* _result)
{
	Float4x4& a = *_a;
	Float4x3& b = *_b;
	Float4x3& r = *_result;

	for (int row = 0; row < 4; row++)
	for (int col = 0; col < 3; col++)
		r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2] + a[3][row] * b[col][3];
}

void pgn::mul(Float4x3* _a, Float4x3* _b, Float4x3* _result)
{
	Float4x3& a = *_a;
	Float4x3& b = *_b;
	Float4x3& r = *_result;

	for(int row = 0; row < 3; row++)
		for(int col = 0; col < 3; col++)
			r[col][row] = a[0][row] * b[col][0] + a[1][row] * b[col][1] + a[2][row] * b[col][2];
}

void pgn::transformVertex(Float3* _v, Float4x3* _m, Float3* _result)
{
	Float3& v = *_v;
	Float4x3& m = *_m;
	Float3& r = *_result;

	r.x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
	r.y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
	r.z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];
}

void pgn::transformVector(Float3* _v, Float4x3* _m, Float3* _result)
{
	Float3& v = *_v;
	Float4x3& m = *_m;
	Float3& r = *_result;

	r.x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2];
	r.y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2];
	r.z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2];
}

void pgn::inverse(Float4x3* _m, Float4x3* result)
{
	Float4x3& m = *_m;
	Float4x3& r = *result;

	float invDet = 1.0f / dot(cross(m[0].float3, m[1].float3), m[2].float3);

	r[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
	r[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
	r[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
	r[0][3] = 0;

	r[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
	r[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
	r[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;
	r[1][3] = 0;

	r[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
	r[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
	r[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;
	r[2][3] = 0;
}

void pgn::lerp(Float3* _a, Float3* _b, float t, Float3* _result)
{
	Float3& a = *_a;
	Float3& b = *_b;
	Float3& r = *_result;

	float k0 = 1.0f - t;
	float k1 = t;
	r.x = a.x * k0 + b.x * k1;
	r.y = a.y * k0 + b.y * k1;
	r.z = a.z * k0 + b.z * k1;
}

void pgn::slerp(Float4* _a, Float4* _b, float t, Float4* _result)
{
	Float4& a = *_a;
	Float4 b = *_b;
	Float4& r = *_result;

	float cosOmega = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];

	if (cosOmega < 0.0f)
	{
		b[0] = -b[0];
		b[1] = -b[1];
		b[2] = -b[2];
		b[3] = -b[3];
		cosOmega = -cosOmega;
	}

	float k0, k1;

	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
		float omega = atan2(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	r[0] = a[0] * k0 + b[0] * k1;
	r[1] = a[1] * k0 + b[1] * k1;
	r[2] = a[2] * k0 + b[2] * k1;
	r[3] = a[3] * k0 + b[3] * k1;
}

void pgn::quatToMat(Float4* _q, Float4x3* _m)
{
	Float4& q = *_q;
	Float4x3& m = *_m;

	float _2xx = 2.0f * q[0] * q[0];
	float _2xy = 2.0f * q[0] * q[1];
	float _2xz = 2.0f * q[0] * q[2];
	float _2xw = 2.0f * q[0] * q[3];
	float _2yy = 2.0f * q[1] * q[1];
	float _2yz = 2.0f * q[1] * q[2];
	float _2yw = 2.0f * q[1] * q[3];
	float _2zz = 2.0f * q[2] * q[2];
	float _2zw = 2.0f * q[2] * q[3];

	m[0][0] = 1.0f - _2yy - _2zz;	m[1][0] = _2xy + _2zw;			m[2][0] = _2xz - _2yw;
	m[0][1] = _2xy - _2zw;			m[1][1] = 1.0f - _2xx - _2zz;	m[2][1] = _2yz + _2xw;
	m[0][2] = _2xz + _2yw;			m[1][2] = _2yz - _2xw;			m[2][2] = 1.0f - _2xx - _2yy;
}

void pgn::add(Float4* a, Float4* b, Float4* results, int count)
{
	for (int i = 0; i < count; i++)
	{
		results[i][0] = a[i][0] + b[i][0];
		results[i][1] = a[i][1] + b[i][1];
		results[i][2] = a[i][2] + b[i][2];
		results[i][3] = a[i][3] + b[i][3];
	}
}

void pgn::sub(Float4* a, Float4* b, Float4* results, int count)
{
	for (int i = 0; i < count; i++)
	{
		results[i][0] = a[i][0] - b[i][0];
		results[i][1] = a[i][1] - b[i][1];
		results[i][2] = a[i][2] - b[i][2];
		results[i][3] = a[i][3] - b[i][3];
	}
}

bool pgn::pointInCCWTriangle(Float2* _p, Float2* _a, Float2* _b, Float2* _c)
{
	Float2& p = *_p;
	Float2& a = *_a;
	Float2& b = *_b;
	Float2& c = *_c;

	return cross(p - a, b - a) >= 0.0f
		&& cross(p - b, c - b) >= 0.0f
		&& cross(p - c, a - c) >= 0.0f;
}

void pgn::closestPtSegmentSegment(Float2* _p1, Float2* _q1, Float2* _p2, Float2* _q2, Float2* _c1, Float2* _c2)
{
	Float2& p1 = *_p1;
	Float2& q1 = *_q1;
	Float2& p2 = *_p2;
	Float2& q2 = *_q2;
	Float2& c1 = *_c1;
	Float2& c2 = *_c2;

	Float2 d1 = q1 - p1;
	Float2 d2 = q2 - p2;
	Float2 r = p1 - p2;
	float a = dot(d1, d1);
	float e = dot(d2, d2);
	float f = dot(d2, r);
	float s, t;
	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		c1 = p1;
		c2 = p2;
	}
	if (a <= FLT_EPSILON) {
		s = 0.0f;
		t = f / e;
		t = clamp(t, 0.0f, 1.0f);
	}
	else {
		float c = dot(d1, r);
		if (e <= FLT_EPSILON) {
			t = 0.0f;
			s = clamp(-c / a, 0.0f, 1.0f);
		}
		else {
			float b = dot(d1, d2);
			float denom = a*e - b*b;
			if (denom != 0.0f)
				s = clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			else
				s = 0.0f;
			t = (b*s + f) / e;
			if (t < 0.0f) {
				t = 0.0f;
				s = clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
}

void pgn::precomputeYTriangle(Float2* _Axz, Float2* _Bxz, Float2* _Cxz, float Ay, float By, float Cy, Float4* intermediateVars1, Float2* intermediateVars2)
{
	Float2& Axz = *_Axz;
	Float2& Bxz = *_Bxz;
	Float2& Cxz = *_Cxz;

	float Bx_Ax = Bxz.x - Axz.x;
	float Bz_Az = Bxz.y - Axz.y;
	float Cx_Ax = Cxz.x - Axz.x;
	float Cz_Az = Cxz.y - Axz.y;

	float denom1 = Bx_Ax * Cz_Az - Bz_Az * Cx_Ax;
	float denom2 = Cx_Ax * Bz_Az - Cz_Az * Bx_Ax;

	Float4 vars1;
	vars1[0] = Cz_Az / denom1;
	vars1[1] = -Cx_Ax / denom1;
	vars1[2] = Bz_Az / denom2;
	vars1[3] = -Bx_Ax / denom2;

	*intermediateVars1 = vars1;

	intermediateVars2->x = By - Ay;
	intermediateVars2->y = Cy - Ay;
}

float pgn::computeYTriangle(Float4* _intermediateVars1, Float2* _intermediateVars2, Float2* _xz)
{
	Float4& vars1 = *_intermediateVars1;
	Float2& vars2 = *_intermediateVars2;
	Float2& xz = *_xz;

	float s = vars1[0] * xz.x + vars1[1] * xz.y;
	float t = vars1[2] * xz.x + vars1[3] * xz.y;

	return s * vars2.x + t * vars2.y;
}

bool pgn::pointInTriangle(Float3& p, Float3& a, Float3& b, Float3& c)
{
	pgn::Float3 v0 = { c.x - a.x, c.y - a.y, c.z - a.z };
	pgn::Float3 v1 = { b.x - a.x, b.y - a.y, b.z - a.z };
	pgn::Float3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z };

	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	float inverDeno = 1.0f / (dot00 * dot11 - dot01 * dot01);

	float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
	if (u < 0.0f || u > 1.0f)
		return false;

	float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
	if (v < 0.0f || v > 1.0f)
		return false;

	return u + v <= 1;
}

void pgn::sincos(float x, float* s, float* c)
{
	*s = sinf(x);
	*c = cosf(x);
}
