/*  Non-Parametric Weighted Histogram Analysis Method (npwham)
    by Mahmoud Moradi, University of Arkansas (Email: moradi@uark.edu)
    1/28/2013-9/18/2017
    The implementation was first introduced in:
    Moradi and Tajkhorshid, "Mechanistic picture for conformational transition of a membrane transporter at atomic resolution", PNAS 110:18916 (2013)
    The algorithms are based on C. Bartels, "Analyzing Biased Monte Carlo and Molecular Dynamics Simulations", Chem. Phys. Letters 331:446 (2000)
    and J. S. Hub, B. L. de Groot, and D. van der Spoel, "g_wham—A Free Weighted Histogram Analysis Implementation Including Robust Error and Autocorrelation Estimates", J. Chem. Theory Comput. 6:3713–3720 (2010)
*/

#include <stdio.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <limits.h>


using namespace std;

#define boltzman 0.001987191683

int main(int argc,char *argv[]) {

    // general parameters    
    double temperature = 300;
    int L = 1; // number of replicas
    int N = 1; // number of input data
    int M = 1; // number of blocks (per replica) for error estimate
    int it = 1000; // self-consistency maximum number of iterations
    int IT = 1; // bootstraping number of iterations
    double df = 0.1; // self-consistency targeted accuracy
    char const *restart = "";
    char const *start = "";
    int argi = 0;
    while (++argi < argc) {
	std::string argstr(argv[argi]);
	if ( argstr == "--windows" || argstr == "-w" )
	    L = atoi(argv[++argi]);
	else if  ( argstr == "--lines" || argstr == "-l" )
	    N = atoi(argv[++argi]);
	else if  ( argstr == "--blocks" || argstr == "-b" )
	    M = atoi(argv[++argi]);
	else if  ( argstr == "--bootstrap" || argstr == "-B" )
	    IT = atoi(argv[++argi]);
	else if  ( argstr == "--iterations" || argstr == "-i" )
	    it = atoi(argv[++argi]);
	else if  ( argstr == "--accuracy" || argstr == "-a" )
	    df = atof(argv[++argi]);
	else if  ( argstr == "--temperature" || argstr == "-t" )
	    temperature = atof(argv[++argi]);
	else if  ( argstr == "--restart" || argstr == "-r" )
	    restart = argv[++argi];
	else if  ( argstr == "--start" || argstr == "-s" )
	    start = argv[++argi];
	else if  ( argstr == "--help" || argstr == "-h" ) {
	    std::cerr
		<< "Non-Parametric Weighted Histogram Analysis Method (npwham)\n"
		<< "   by Mahmoud Moradi, University of Arkansas (Email: moradi@uark.edu)\n"
		<< "   1/28/2013-9/18/2017\n"
		<< "The implementation was first introduced in:\n Moradi and Tajkhorshid,\n"
		<< "   \"Mechanistic picture for conformational transition of a membrane transporter\n"
		<< "   at atomic resolution\", PNAS 110:18916 (2013)\n"
		<< "The algorithms are based on C. Bartels, \"Analyzing Biased Monte Carlo and\n"
		<< "   Molecular Dynamics Simulations\", Chem. Phys. Letters 331:446 (2000)\n"
		<< "   and J. S. Hub, B. L. de Groot, and D. van der Spoel, \"g_wham—A Free Weighted\n"
		<< "   Histogram Analysis Implementation Including Robust Error and\n Autocorrelation\n"
		<< "   Estimates\", J. Chem. Theory Comput. 6:3713–3720 (2010)\n\n\n"
		<< "Usage:  stdin: data\n"
		<< "        stdout: optimized probabilities (only for the last iteration)\n"
		<< "        stderr: log file including the free energies at the end of each iteration\n"
		<< " -- Options:\n"
		<< "	    --help (-h)\n"
		<< "	    --windows (-w) number of windows (default: 1)\n"
        	<< "	    --lines (-l) number of data points (lines) read from the stdandard input  (default: 1)\n"
                << "		    Each line (in stdin) includes \"i [b] t u_0 u_1 ... \"\n"
                << "        	    t and i are the time and window index (both integers)\n"
                << "        	    u_i is the biasing potential associated with the window i\n"
                << "        	    b is the block index; must be included only when the number of blocks is greater than 1\n"
        	<< "	    --iterations (-i) maximum number of iterations for self-consistent solution (default: 1000)\n"
        	<< "	    --accuracy (-a) accuracy in kcal/mol for self-consistent solution (default: 0.1)\n"
        	<< "	    --blocks (-b) number of blocks for bootstrap algorithm (default: 1)\n"
        	<< "	`   --bootstrap (-B) number of bootstrap iterations (default: 1; i.e., no bootstrapping)\n"
        	<< "	    --temperature (-t) simulation temperature in Kelvin (default: 300)\n"
		<< "	    --restart (-r) a filename to store resulted p's and f's to continue iterations (default: "")\n"
		<< "            --start (s) an existing restart file to continue iterations (default: "")\n"
		<< "Program generates the {p(i,t)} probability for each data point \"[I] i [b] t p\" (in stdout)\n"
		<< "  I is the bootstrap iteration (reported only if bootstrapping is performed)\n"
		<< "  b is the block index (reported only if  bootstrapping is performed and the number of blocks is greater than 1)\n"
		<< "Program also reports the estimated free energies of windows (at the end of each iteration) in stderr\n";
	    exit(0);
	}
    }

    
    std::cerr << "# Done reading parameters:\n";
    std::cerr << "# Temperature: "<< temperature <<"\n";
    std::cerr << "# Number of windows: "<< L <<"\n";
    std::cerr << "# Number of data points: "<< N <<"\n";
    std::cerr << "# Maximum number of iterations for self-consistent algorithm: "<< it <<"\n";
    std::cerr << "# Accuracy (in kcal/mol) to be targeted for self-consistent algorithm: "<< df <<"\n";
    std::cerr << "# Number of blocks per window: "<< M <<"\n";
    std::cerr << "# Number of iterations for bootstraping: "<< IT << "\n";

    double beta = 1/(boltzman*temperature);
    int LM=L*M;
    df *= boltzman;

    double* p = new double[N]; // weights (at iteration k)
    double* p_ = new double[N]; // weights (at iteration k-1)
    double* c = new double[N*L]; // biases
    double* f = new double[L]; // window proportionality factors
    double* Sumf = new double[L]; // sum of all free energy bootstraps
    double* Sumf2 = new double[L]; // sum of all free energy bootstrap squareds
    double* n = new double[L]; // number of points for each window
    double* bb = new double[LM]; // bayesian bootstraping weights
    int* m = new int[LM]; // number of points for each window/block
    int* T = new int[N]; // actual time for each data point
    int* W = new int[N]; // window of each data point
    int* B = new int[N]; // block of each data point

    for (int i=0; i<L; i++) {
	Sumf[i] = 0;
	Sumf2[i] = 0;
	f[i] = 1.0;
	for (int j=0; j<M; j++)
	    m[M*i+j] = 0;
    }

    // reading the data
    double U;
    std::cerr << "# Reading data points starts...\n";
    for (int t=0; t<N; t++) {
	std::cin >> W[t];
	if (M>1)
	    std::cin >> B[t];
	else
	    B[t]=0;
	std::cin >> T[t];
	m[M*W[t]+B[t]]++;
	for (int i=0; i<L; i++) {
	    std::cin >> U;
	    c[t*L+i]=exp(-beta*U);
	}
	std::cin.ignore(INT_MAX,'\n');
    }
    std::cerr << "# Done reading " << N << " data points.\n";
    for (int i = 0; i < L; i++) {
	n[i]=0;
	std::cerr << "# replica " << i << " : ";
	for (int j = 0; j < M; j++) {
	    n[i]+=m[M*i+j];
	    std::cerr << m[M*i+j] << " ";
	}
	std::cerr << "(" << n[i] << ")\n";
    }

    // initializing the p's and f's from start file
    if (start!="") {
	ifstream init (start);
	if (init) {
	    for (int i=0; i<L; i++) init >> f[i];
	    for (int t=0; t<N; t++) init >> p[t];
	    init.close();
	    std::cerr << "Done reading the \"" << start << "\" file!\n";
	} else {
	    std::cerr << "Error: Could not open \"" << start << "\" file!\n";
	    exit(EXIT_FAILURE);
	}
    }

    // starting the calculations
    srand ( time(NULL) ); // for bayesian bootstraping

    // bayesian bootstraping
    int bootstrap=0;
    while ( bootstrap++ < IT ) {
	if (IT>1) {
	    bb[0]=0.0;
	    for (int ij = 1; ij < LM; ij++) {
		double varij=double(rand())/double(RAND_MAX);
		int ij_=1;
		while (ij_ < ij && varij > bb[ij_]) ij_++;
		for (int ij__=ij; ij__ > ij_; ij__--) bb[ij__]=bb[ij__-1];
		bb[ij_]=varij;
	    }
	    for (int ij = 0; ij < LM-1; ij++) bb[ij]=(bb[ij+1]-bb[ij])*N/double(m[ij]);
	    bb[LM-1]=(1.0-bb[LM-1])*N/double(m[LM-1]);
	} else
	    for (int ij = 0; ij < LM; ij++)
		bb[ij]=1.0;
	std::cerr << "# bootstrap " << bootstrap << " populations multiplied by:\n" ;
	for (int i = 0; i < L; i++) {
	    n[i]=0;
	    std::cerr << "# replica " << i << " :";
	    for (int j = 0; j < M; j++) {
		n[i]+=m[M*i+j]*bb[M*i+j];
		std::cerr << " ( "<< m[M*i+j] <<" x ) " << bb[M*i+j];
	    }
	    std::cerr << "\n";
	}
	// self-consistent algorithm
	if (it>0) {
	    double dfe=df;
	    double minf;
	    int k = 0;
	    int tt = 0;
	    while(k<it && tt<N) {
		std::cerr << "#";
		if (IT>1)
		    std::cerr << " bootstrap " << bootstrap;
		std::cerr << " iteration " << k++ << "\n";
		for (int t=0; t<N; t++) {
		    double P = 0.0;
		    for (int i=0; i<L; i++) 
			    P+=n[i]*f[i]*c[t*L+i];
		    p[t]=1.0/P;
		}
	        for (int i=0; i<L; i++) {
		    double F = 0.0;
		    for (int t=0; t<N; t++)
		        F+=bb[M*W[t]+B[t]]*p[t]*c[t*L+i];
		    f[i]=1.0/F;
		    if (i==0) minf=f[i]; else if (minf>f[i]) minf=f[i];
		}
		for (int i=0; i<L; i++)
		    std::cerr << i << " " << log(f[i]/minf)/beta << "\n";
		if (k>0) {
		    int t = 0;
		    while( abs(log(p[t]/p_[t]))<df && t<N ) t++;
		    tt = t;
		}
		for (int t=0; t<N; t++) p_[t]=p[t];
	    }

	    for (int i=0; i<L; i++) {
		Sumf[i]+=log(f[i]/minf)/beta;
		Sumf2[i]+=log(f[i]/minf)*log(f[i]/minf)/(beta*beta);
	    }

	    double Pt=0;
    	    for (int t=0; t<N; t++)
    		Pt+=bb[M*W[t]+B[t]]*p[t];
	    for (int t=0; t<N; t++) {
    		if (IT>1) std::cout << bootstrap << " ";
		std::cout << W[t];
		if (IT>1&&M>1) std::cout << " " << B[t];
		std::cout << " "  << T[t] << " " << bb[M*W[t]+B[t]]*p[t]/Pt << "\n";
	    }
	}
    }
    if (IT == 1) {
	for (int i=0; i<L; i++)
	    std::cerr << "# Final: " << i << " " << Sumf[i] << "\n";
    } else {
	for (int i=0; i<L; i++)
	    std::cerr << "# Average/SD: " << i << " " << Sumf[i]/IT << " " << ((IT*Sumf2[i]>Sumf[i]*Sumf[i])?(sqrt(IT*Sumf2[i]-Sumf[i]*Sumf[i])/IT):0) << "\n";
    }
    if ( restart!="" && IT == 1 ) {
	ofstream rest (restart);
	for (int i=0; i<L; i++) rest << " " << f[i];
	rest << "\n";
	for (int t=0; t<N; t++) rest << p[t] << "\n";
	rest.close();
    }

    delete[] c;
    delete[] p;
    delete[] p_;
    delete[] n;
    delete[] m;
    delete[] f;
    delete[] Sumf;
    delete[] Sumf2;
    delete[] bb;
    delete[] W;
    delete[] B;
    delete[] T;
}
