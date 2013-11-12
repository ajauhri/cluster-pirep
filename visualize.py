#!/usr/bin/python

# for parsing csv file
import csv
from collections import defaultdict

# for plotting
import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

# Pre-defined values
CSV_FILE = 'data/aircraft_reports_AK_2013.csv' 
LAT = 'Latitude'
LONG = 'Longitude'
ALT = 'Altitude(Ft)'
DEBUG = 1

def strToInt(l):
	return [ float(x) for x in l ]

def visualize():
	columns = defaultdict(list)
	try:
		with open(CSV_FILE) as f:
			reader = csv.DictReader(f)
			for row in reader:
				for (k,v) in row.items():
					columns[k].append(v)
		if len(columns[LAT]) == 0:
			raise Exception("Latitude list is empty")
		if len(columns[LONG]) == 0:
			raise Exception("Longitude list is empty")
		if len(columns[ALT]) == 0:
			raise Exception("Altitude list is empty")
		print 'Done with parsing'
		columns[LAT] = strToInt(columns[LAT])
		columns[LONG] = strToInt(columns[LONG])
		columns[ALT] = strToInt(columns[ALT])
		X = np.asarray(columns[LAT])
		Y = np.asarray(columns[LONG])
		Z = np.asarray(columns[ALT])
		if DEBUG:
			print X
			print Y
			print Z
		fig = plt.figure()
		ax = fig.add_subplot(111, projection='3d')
		ax.scatter(X, Y, Z, c='b', marker='^')
		ax.set_xlabel(LAT)
		ax.set_ylabel(LONG)
		ax.set_zlabel(ALT)
		plt.show()


	except EnvironmentError, e: #parent of IOError, OSError 
		print e 

if __name__ == "__main__":
	visualize()

