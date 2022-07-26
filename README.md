Configuration files and scripts used for setup/analysis of Bias Exchange Umbrella Sampling (BEUS) Simulations discussed 
in the Manuscript: Binding Affinity Estimation From Restrained Umbrella Sampling Simulations. 

Authors:  Vivek Govind Kumar, Shilpi Agrawal, Thallapuranam Krishnaswamy Suresh Kumar, and Mahmoud Moradi

#Simulation Setup

Sample configuration files are provided for the BEUS simulation setup discussed in the manuscript.

beus.conf - primary BEUS configuration file

beus.namd - BEUS code

namd.conf - NAMD configuration file used for the BEUS simulations

colvar_"filename".conf - defines the collective variables/reaction coordinates and restraints used for the BEUS simulations; provided for each BEUS simulation setup discussed in the manuscript.

centers.tcl - defines specific BEUS images/windows by assigning forceConstant and centers to override information in the colvars configuration file.; a sample file from the distance-based BEUS simulation with orientation restraints is provided here.

exchange.sh - calculates the exchange rate between neighboring image pairs based on accepted exchanges reported in the main NAMD log file.

step3_pbcsetup.pdb, step3_pbcsetup.xplor.ext.psf - PDB and PSF files for the hFGF1-heparin system generated using CHARMM-GUI


#Free Energy Calculations

Scripts/codes used to calculate PMF/free energy are provided.

do.sh - combines the colvar trajectory files with BEUS history files and assigns the actual sampled collective variable values associated with each sample to its appropriate image id.

pot.sh - calculates biasing potentials based on the image centers and actual sampled collective variables.

npwham.cp - code for the Non-Parametric Weighted Histogram Analysis (npwham) Method

fe.sh - estimates free energies from the biasing potentials using npwham. Generates text files containing the converged free energy estimates and the probability of each sampled conformation.

err.sh - uses npwham to generate error estimates via bootstrapping

pmf.sh - builds the 1D-PMF in terms of the ligand-protein distance

distvector.sh - builds the 3D-PMF in terms of the distance vector


