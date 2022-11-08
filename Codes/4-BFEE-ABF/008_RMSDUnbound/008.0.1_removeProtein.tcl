
mol new ../complex.psf
mol addfile ../complex.pdb
set aa [atomselect top "not segname PROB"]
$aa writepsf ./ligandOnly.psf
$aa writepdb ./ligandOnly.pdb
exit
