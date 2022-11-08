
package require autoionize
autoionize -psf ./ligandOnly.psf -pdb ./ligandOnly.pdb -neutralize -cation SOD -anion CLA -seg IO2 -o ./ligandOnly
mol new ./ligandOnly.psf
mol addfile ./ligandOnly.pdb
set aa [atomselect top all]
$aa set beta 0
set solute [atomselect top "not water and not ion"]
$solute set beta 1
$aa writexyz ./ligandOnly.xyz

exit
