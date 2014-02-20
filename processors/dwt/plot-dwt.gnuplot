set term postscript eps color enhanced
set output "dwt-map.eps"

set pm3d map
splot "wpsd.txt" matrix
