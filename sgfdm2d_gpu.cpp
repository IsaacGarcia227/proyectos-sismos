/*
 * sgfdm2d.cpp
 * 2D elastic wave propagation exercise
 * J.D. De Basabe (jonas@cicese.mx)
 */
#include "macros.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
// Macros to compute the x and z coordinates of grid nodes


#define XX(i) (XMIN + i*hx)
#define ZZ(i) (ZMIN + i*hz)
#define TT(i) (i*ht)

//#define ZTOP(x) (0.5*(ZMIN+ZMAX))
#define ZTOP(x) ZMAX

//#define ZTOP(x) (ZMIN)

// P-wave velocity

#pragma omp declare target

double alpha(double x, double z)
{
	    if( z < ZMAX )
        return 2000.0;
    return 2000.0;
}

// S-wave velocity
double beta(double x, double z)
{
	if(z < ZMAX)
		return 0.0;
	return 1000.0;
}

// Density
double rho(double x, double z)
{
	if( z < ZMAX )
		return 1000.0;
	return 2000.0;
}

// Ricker wavelet
double Ricker(double t)
{
	double t0 = 0.78/PKFREQ;
	return -2.0*sqr(M_PI*PKFREQ)*(1.0-2.0*sqr(M_PI*PKFREQ*(t-t0)))
		*exp( -sqr(M_PI*PKFREQ*(t-t0)) );
}

// Gaussian distribution
double Gauss(double t, double mn, double sd)
{
	return exp( -sqr( (t-mn)/sd ) )/(sd*sqrt(2.0*M_PI));
}

// Source
double Source(double x, double z, double t)
{
	return Gauss(x,XSRC,XSW)*Gauss(z,ZSRC,ZSW)*Ricker(t);
}
#pragma omp end declare target

// Wave propagation function
void sgfd2d(int nx, int nz, int nt)
{
	double hx=0.0, hz, ht, px, pz, l2m, l, m, b;
	int ix, iz, it;
	char fnsnapshot[256];
	if( nx<=1 || nz<=1 )
	{ // check for errors in input parameters
		printf("\nERROR in input parameters, nx=%i, nz=%i\n",nx,nz);
		return; // Exit the function if there is an error in nx or nz
	}
	// Declare matrices for the pressure field.
	int n = nx*nz;
	double *vx  = new double[n]();
	double *vz  = new double[n]();
	double *sxx = new double[n]();
	double *szz = new double[n]();
	double *sxz = new double[n]();

	printf("\n\tStaggered-Grid Finite-Diference 2D Elastic Wave Propagation\n");
	// Compute increments
	hx = (double) (XMAX - XMIN)/(nx-1.0);
	hz = (double) (ZMAX - ZMIN)/(nz-1.0);
	printf("Finite Difference Mesh = %i x %i\n",nx,nz);
	printf("Increments: hx = %g, hz = %g\n", hx, hz);
	if( nt<=0 )
	{// compute nt and ht if not provided
		ht= (hx<hz) ? 0.7*hx/VMAX : 0.7*hz/VMAX;  // get the smallest increments
		/*
        if( hx<hz )
            ht= 0.7*hx/VMAX;
        else
            ht= 0.7*hz/VMAX;
         */
		nt= TMAX/ht;
		if( nt*ht<TMAX )// make sure the time domain is fully covered
			++nt;
	}
	else// compute ht
		ht= (double) TMAX/nt;
	px= ht/hx;
	pz= ht/hz;
	printf("Time increment = %g, NT = %i\n", ht, nt);
	printf("Beginning time-stepping loop...\n");
	#pragma omp target data map(tofrom: vx[0:n], vz[0:n], sxx[0:n], szz[0:n], sxz[0:n])
{
	
	// Time-stepping loop - Not parallel
	for( it=1; it<=nt; ++it )
	{
		// Update velocity
#pragma omp target teams distribute parallel for private(ix, iz, b) collapse(2)
		for( ix=1; ix<(nx-1); ++ix )
			for( iz=1; iz<(nz-1); ++iz )
			{
				b  = 1.0/rho(XX(ix),ZZ(iz));

	vx[ix*nz+iz]+= b*(px*(sxx[(ix+1)*nz+iz] - sxx[ix*nz+iz])
                + pz*(sxz[ix*nz+iz] - sxz[ix*nz+(iz-1)]) )
                + ht*NSX*Source(XX(ix),ZZ(iz),TT(it));
        vz[ix*nz+iz]+= b*(pz*(szz[ix*nz+(iz+1)] - szz[ix*nz+iz])
                + px*(sxz[ix*nz+iz] - sxz[(ix-1)*nz+iz]) )
                + ht*NSZ*Source(XX(ix),ZZ(iz),TT(it));
			}

		// Update stress at t= (l+1/2)ht
#pragma omp target teams distribute parallel for private(ix, iz, b, m, l2m, l) collapse(2)	
	for( ix=1; ix<(nx-1); ++ix )
			for( iz=1; iz<(nz-1); ++iz )
			{
b  = 1.0/rho(XX(ix),ZZ(iz)); 		// bouyancy
				m  = sqr(beta(XX(ix),ZZ(iz)))/b; 	// mu
				l2m= sqr(alpha(XX(ix),ZZ(iz)))/b; 	// lambda + 2 mu
				l  = l2m - 2.0*m;					// lambda
		sxx[ix*nz+iz]+= l2m*px*(vx[ix*nz+iz] - vx[(ix-1)*nz+iz])
   			 + l*pz*(vz[ix*nz+iz] - vz[ix*nz+(iz-1)]);
		szz[ix*nz+iz]+= l*px*(vx[ix*nz+iz] - vx[(ix-1)*nz+iz])
    			+ l2m*pz*(vz[ix*nz+iz] - vz[ix*nz+(iz-1)]);	
		sxz[ix*nz+iz]+= m*( px*(vz[(ix+1)*nz+iz] - vz[ix*nz+iz])
    			+ pz*(vx[ix*nz+(iz+1)] - vx[ix*nz+iz]) ); 
			}

		// Save snapshots
		if( it%SAVEINT == 0 || it==nt)
		{
		   #pragma omp target update from(vx[0:n], vz[0:n])
   			 sprintf(fnsnapshot,"./OUTPUT/ux%.8i.bin",it);
   			 FILE *fp = fopen(fnsnapshot, "wb");
		         fwrite(vx, sizeof(double), n, fp);
			 fclose(fp);
			 printf("t = %8.4g, BIN file: %s\n", it*ht, fnsnapshot);
    			 sprintf(fnsnapshot,"./OUTPUT/uz%.8i.bin",it);
                         FILE *fp2 = fopen(fnsnapshot, "wb");
                         fwrite(vz, sizeof(double), n, fp2);
                         fclose(fp2);
                         printf("t = %8.4g, BIN file: %s\n", it*ht, fnsnapshot);
		}
	}} // end target data
	
	printf("End of time-stepping loop.\n");

delete[] vx;
delete[] vz;
delete[] sxx;
delete[] szz;
delete[] sxz;
}

