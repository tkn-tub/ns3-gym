/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 *  Copyright (C) 2001  Pierre L'Ecuyer (lecuyer@iro.umontreal.ca)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 *  02110-1301 USA
 */

#include "rng-mrg32k3a.h"
#include "seed-generator.h"
#include <iostream>

#ifndef MAXINT
#define	MAXINT	2147483647	// XX [for now]
#endif


using namespace std; 
namespace 
{ 
	const double m1 = 4294967087.0; 
	const double m2 = 4294944443.0; 
	const double norm = 1.0 / (m1 + 1.0); 
	const double a12 = 1403580.0; 
	const double a13n = 810728.0; 
	const double a21 = 527612.0; 
	const double a23n = 1370589.0; 
	const double two17 = 131072.0; 
	const double two53 = 9007199254740992.0; 
	const double fact = 5.9604644775390625e-8; /* 1 / 2^24 */ 

	// The following are the transition matrices of the two MRG 
	// components (in matrix form), raised to the powers -1, 1, 
	// 2^76, and 2^127, resp. 

	const double InvA1[3][3] = { // Inverse of A1p0 
		{ 184888585.0, 0.0, 1945170933.0 }, 
		{ 1.0, 0.0, 0.0 }, 
		{ 0.0, 1.0, 0.0 } 
	}; 

	const double InvA2[3][3] = { // Inverse of A2p0 
		{ 0.0, 360363334.0, 4225571728.0 }, 
		{ 1.0, 0.0, 0.0 }, 
		{ 0.0, 1.0, 0.0 } 
	}; 

	const double A1p0[3][3] = { 
		{ 0.0, 1.0, 0.0 }, 
		{ 0.0, 0.0, 1.0 }, 
		{ -810728.0, 1403580.0, 0.0 } 
	}; 

	const double A2p0[3][3] = { 
		{ 0.0, 1.0, 0.0 }, 
		{ 0.0, 0.0, 1.0 }, 
		{ -1370589.0, 0.0, 527612.0 } 
	}; 

	const double A1p76[3][3] = { 
		{ 82758667.0, 1871391091.0, 4127413238.0 }, 
		{ 3672831523.0, 69195019.0, 1871391091.0 }, 
		{ 3672091415.0, 3528743235.0, 69195019.0 } 
	}; 

	const double A2p76[3][3] = { 
		{ 1511326704.0, 3759209742.0, 1610795712.0 }, 
		{ 4292754251.0, 1511326704.0, 3889917532.0 }, 
		{ 3859662829.0, 4292754251.0, 3708466080.0 } 
	}; 

	const double A1p127[3][3] = { 
		{ 2427906178.0, 3580155704.0, 949770784.0 }, 
		{ 226153695.0, 1230515664.0, 3580155704.0 }, 
		{ 1988835001.0, 986791581.0, 1230515664.0 } 
	}; 

	const double A2p127[3][3] = { 
		{ 1464411153.0, 277697599.0, 1610723613.0 }, 
		{ 32183930.0, 1464411153.0, 1022607788.0 }, 
		{ 2824425944.0, 32183930.0, 2093834863.0 } 
	}; 

} // end of anonymous namespace 

//------------------------------------------------------------------- 
// Return (a*s + c) MOD m; a, s, c and m must be < 2^35 
// 

double 
RngMrg32k3a::MultModM (double a, double s, double c, double m) 
{ 
	double v; 
	long a1; 
	v=a*s+c; 
	
	if (v >= two53 || v <= -two53) { 
		a1 = static_cast<long> (a / two17); a -= a1 * two17; 
		v =a1*s; 
		a1 = static_cast<long> (v / m); v -= a1 * m; 
		v = v * two17 + a * s + c; 
	} 
	a1 = static_cast<long> (v / m); 
	/* in case v < 0)*/ 
	if ((v -= a1 * m) < 0.0) return v += m; else return v; 
} 

//------------------------------------------------------------------- 
// Compute the vector v = A*s MOD m. Assume that -m < s[i] < m. 
// Works also when v = s. 
// 
void
RngMrg32k3a::MatVecModM (const double A[3][3], const double s[3], double v[3], 
			 double m) 
{ 
	int i; 
	double x[3]; // Necessary if v = s 
	for (i = 0; i < 3; ++i) { 
		x[i] = MultModM (A[i][0], s[0], 0.0, m); 
		x[i] = MultModM (A[i][1], s[1], x[i], m); 
		x[i] = MultModM (A[i][2], s[2], x[i], m); 
	} 
	for (i = 0; i < 3; ++i) 
		v[i] = x[i]; 
} 

//------------------------------------------------------------------- 
// Compute the matrix C = A*B MOD m. Assume that -m < s[i] < m. 
// Note: works also if A = C or B = C or A = B = C. 
// 
void 
RngMrg32k3a::MatMatModM (const double A[3][3], const double B[3][3], 
			 double C[3][3], double m) 
{ 
	int i, j; 
	double V[3], W[3][3]; 
	for (i = 0; i < 3; ++i) { 
		for (j = 0; j < 3; ++j) 
			V[j] = B[j][i]; 
		MatVecModM (A, V, V, m); 
		for (j = 0; j < 3; ++j) 
			
			W[j][i] = V[j]; 
		} 
	for (i = 0; i < 3; ++i) 
		for (j = 0; j < 3; ++j) 
			C[i][j] = W[i][j]; 
} 

//------------------------------------------------------------------- 
// Compute the matrix B = (A^(2^e) Mod m); works also if A = B. 
// 
void 
RngMrg32k3a::MatTwoPowModM (const double A[3][3], double B[3][3], double m, 
			    long e) 
{ 
	int i, j; 
	/* initialize: B = A */ 
	if (A != B) { 
		for (i = 0; i < 3; ++i) 
			for (j = 0; j < 3; ++j) 
				B[i][j] = A[i][j]; 
	} 
	/* Compute B = A^(2^e) mod m */ 
	for (i = 0; i < e; i++) 
		MatMatModM (B, B, B, m); 
} 

//------------------------------------------------------------------- 
// Compute the matrix B = (A^n Mod m); works even if A = B. 
// 
void 
RngMrg32k3a::MatPowModM (const double A[3][3], double B[3][3], double m, 
			 long n) 
{ 
	int i, j; 
	double W[3][3]; 
	/* initialize: W = A; B = I */ 
	for (i = 0; i < 3; ++i) 
		for (j = 0; j < 3; ++j) { 
			W[i][j] = A[i][j]; 
			B[i][j] = 0.0; 
		} 
	for (j = 0; j < 3; ++j) 
		B[j][j] = 1.0; 
	/* Compute B = A^n mod m using the binary decomposition of n */
	while (n > 0) { 
		if (n % 2) MatMatModM (W, B, B, m); 
		MatMatModM (W, W, W, m); 
		
		n/=2; 
	} 
} 

//-------------------------------------------------------------------- 
// Check that the seeds are legitimate values. Returns 0 if legal 
// seeds, -1 otherwise. 
// 
int 
RngMrg32k3a::CheckSeed (const unsigned long seed[6]) 
{ 
	int i; 
	for (i = 0; i < 3; ++i) { 
		if (seed[i] >= m1) { 
			std::cerr << "****************************************" << std::endl
				  << "ERROR: Seed["<<i<<"] >= 4294967087, Seed is not set." << std::endl
				  << "****************************************" << std::endl;
			return (-1); 
		} 
	} 
	for (i = 3; i < 6; ++i) { 
		if (seed[i] >= m2) { 
			std::cerr << "****************************************" << std::endl
				  << "ERROR: Seed["<<i<<"] >= 429444443, Seed is not set."<<std::endl
				  << "****************************************" <<std::endl;
			return (-1); 
		} 
	} 
	if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) { 
		std::cerr<< "****************************************" << std::endl
			 << "ERROR: First 3 seeds = 0." << std::endl 
			 << "****************************************" << std::endl;
		return (-1); 
	} 
	if (seed[3] == 0 && seed[4] == 0 && seed[5] == 0) { 
		std::cerr << "****************************************" <<std::endl
			  << "ERROR: Last 3 seeds = 0." << std::endl
			  << "****************************************" << std::endl;
		return (-1); 
	} 
	return 0; 
} 


//------------------------------------------------------------------------- 
// Generate the next random number. 
// 
double RngMrg32k3a::U01 () 
{ 
	long k; 
	double p1, p2, u; 
	/* Component 1 */ 
	p1 = a12 * Cg_[1] - a13n * Cg_[0]; 
	k = static_cast<long> (p1 / m1); 
	p1 -= k * m1; 
	if (p1 < 0.0) p1 += m1; 
	Cg_[0] = Cg_[1]; Cg_[1] = Cg_[2]; Cg_[2] = p1; 
	/* Component 2 */ 
	p2 = a21 * Cg_[5] - a23n * Cg_[3]; 
	k = static_cast<long> (p2 / m2); 
	p2 -= k * m2; 
	if (p2 < 0.0) p2 += m2; 
	Cg_[3] = Cg_[4]; Cg_[4] = Cg_[5]; Cg_[5] = p2; 
	/* Combination */ 
	u = ((p1 > p2) ? (p1 - p2) * norm : (p1 - p2 + m1) * norm); 
	return u;
} 

//------------------------------------------------------------------------- 
// Generate the next random number with extended (53 bits) precision. 
// 
double RngMrg32k3a::U01d () 
{ 
	double u; 
	u = U01(); 
	u += (U01() - 1.0) * fact; 
	return (u < 0.0) ? u + 1.0 : u; 
} 

//************************************************************************* 
// Public members of the class start here 
//------------------------------------------------------------------------- 

RngMrg32k3a::RngMrg32k3a ()
{}

void RngMrg32k3a::reset (long seed) 
{
	for (int i = 0; i < 6; ++i) { 
		Bg_[i] = Cg_[i] = Ig_[i] = seed; 
	} 
}


//------------------------------------------------------------------------- 
// Generate the next random number. 
// 

uint32_t 
RngMrg32k3a::get_max (void) const
{
	return MAXINT;
}
uint32_t 
RngMrg32k3a::get_min (void) const
{
	return 0;
}

uint32_t 
RngMrg32k3a::get_uint (void)
{  
	return get_uint (0, MAXINT);
}

uint32_t
RngMrg32k3a::get_uint (uint32_t n) 
{
	return get_uint (0, n);
}

uint32_t
RngMrg32k3a::get_uint (uint32_t low, uint32_t high) 
{ 
	return ((uint32_t) (low + (uint32_t) (((uint32_t) 
					       (high-low+1)) * U01())));
}

double 
RngMrg32k3a::get_double (void)
{
	
	return U01d ();
}
