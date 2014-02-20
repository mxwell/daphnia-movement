#! /usr/bin/env python

# arguments:
# - trace file

import os, re, sys
import numpy as np
from matplotlib import pyplot as plt

from astroML.fourier import\
    FT_continuous, IFT_continuous, sinegauss, sinegauss_FT, wavelet_PSD

if (len(sys.argv) < 2):
	print "Error: no trace file"
	quit()

who_am_i = sys.argv[0]
cur_path = who_am_i[:who_am_i.rfind('/') + 1]
trace_name = sys.argv[1]

trace = open(trace_name, "r")

t = []
speed = []

for line in trace:
	ar = line.split()[0:2]
	t.append(float(ar[0]))
	speed.append(float(ar[1]))

trace.close()

if (len(t) % 2 != 0):
	t = t[1:]
	speed = speed[1:]

print ("Starting with %lf, %lf" % (t[0], speed[0]))
print (" ..and going on for %d records" % len(t))

f0 = np.linspace(0.1, 1.0, 100)
wpsd = wavelet_PSD(t, speed, f0, Q=1.0)

out = open("wpsd.txt", "w")
for row in wpsd:
	for val in row:
		out.write("%lf " % val)
	out.write("\n")
out.close()

next_cmd = "gnuplot " + cur_path + "plot-dwt.gnuplot"
print("plotting with command: " + next_cmd)
if (os.system(next_cmd) != 0):
	print("Error: can't run command in shell: " + next_cmd)
	quit()

p = re.compile(".*\.log$")
if (p.match(trace_name)):
	trace_name = trace_name[:-4]

dwt_name = trace_name + "-dwt.eps"
print("renaming DWT plot to " + dwt_name)
os.rename("dwt-map.eps", dwt_name)
print("Done.")
