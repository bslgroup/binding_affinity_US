Configuration files and scripts used for setup/analysis of Bias Exchange Umbrella Sampling (BEUS) Simulations discussed 
in the Manuscript: Binding Affinity Estimation From Restrained Umbrella Sampling Simulations. 

Authors:  Vivek Govind Kumar, Shilpi Agrawal, Thallapuranam Krishnaswamy Suresh Kumar, and Mahmoud Moradi


#CODES

#1-SMD Simulations

Sample configuration files are provided for the SMD simulation setup discussed in the manuscript.

namd.conf - NAMD configuration file used for the SMD simulations

colvarconfig_"filename".conf - defines the collective variables/reaction coordinates and restraints used for the SMD simulations; provided for each SMD simulation setup discussed in the manuscript.

step3_pbcsetup.pdb, step3_pbcsetup.xplor.ext.psf - PDB and PSF files for the hFGF1-heparin system generated using CHARMM-GUI

wt.pdb, wt.psf - PDB and PSF files for the unbound hFGF1 system generated using CHARMM-GUI

freehep.pdb, freehep.psf - PDB and PSF files for the free ligand generated using CHARMM-GUI


#2-BEUS Simulations

Sample configuration files are provided for the BEUS simulation setup discussed in the manuscript.

beus.conf - primary BEUS configuration file

beus.namd - BEUS code

namd.conf - NAMD configuration file used for the BEUS simulations

colvar_"filename".conf - defines the collective variables/reaction coordinates and restraints used for the BEUS simulations; provided for each BEUS simulation setup discussed in the manuscript.

centers.tcl - defines specific BEUS images/windows by assigning forceConstant and centers to override information in the colvars configuration file.; a sample file from the distance-based BEUS simulation with orientation restraints is provided here.

exchange.sh - calculates the exchange rate between neighboring image pairs based on accepted exchanges reported in the main NAMD log file.

step3_pbcsetup.pdb, step3_pbcsetup.xplor.ext.psf - PDB and PSF files for the hFGF1-heparin system generated using CHARMM-GUI

wt.pdb, wt.psf - PDB and PSF files for the unbound hFGF1 system generated using CHARMM-GUI

freehep.pdb, freehep.psf - PDB and PSF files for the free ligand generated using CHARMM-GUI


#3-Free Energy Calculations

Scripts/codes used to calculate PMF/free energy are provided.

do.sh - combines the colvar trajectory files with BEUS history files and assigns the actual sampled collective variable values associated with each sample to its appropriate image id.

pot.sh - calculates biasing potentials based on the image centers and actual sampled collective variables.

npwham.cp - code for the Non-Parametric Weighted Histogram Analysis (npwham) Method

fe.sh - estimates free energies from the biasing potentials using npwham. Generates text files containing the converged free energy estimates and the probability of each sampled conformation.

err.sh - uses npwham to generate error estimates via bootstrapping

pmf.sh - builds the 1D-PMF in terms of the ligand-protein distance

distvector.sh - builds the 3D-PMF in terms of the distance vector


#NPWHAM EXAMPLE

Sample input and output files (for the Distance-based BEUS simulation with no restraints) are provided for scripts that use the npwham code. 

npwham.cp - code for the Non-Parametric Weighted Histogram Analysis (npwham) Method

#1-Free energy 

fe.sh - estimates free energies from the biasing potentials using npwham. Generates text files containing the converged free energy estimates and the probability of each sampled conformation.

potall.txt - input file containing biasing potentials based on the image centers and actual sampled collective variables.

densityall.txt - output file for fe.sh; contains probability of each sampled conformation

densityall.err - output file for fe.sh; contains converged free energy estimates

#2-Bootstrapping

err.sh - uses npwham to generate error estimates in addition to the free energies via bootstrapping

potall.txt - input file containing biasing potentials based on the image centers and actual sampled collective variables.

bootstrapall.txt - output file for err.sh; contains probability of each sampled conformation

bootstrapall.err - output file for err.sh; contains average free energy estimates with standard deviations.

