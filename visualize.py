#!/usr/bin/python
import csv
from collections import defaultdict
CSV_FILE = 'data/aircraft_reports_AK_2013.csv' 
DEBUG = 0

def visualize():
	columns = defaultdict(list)
	try:
		with open(CSV_FILE) as f:
			reader = csv.DictReader(f)
			for row in reader:
				for (k,v) in row.items():
					columns[k].append(v)
		if len(columns['Latitude']) == 0:
			raise Exception("Latitude list is empty")
		if len(columns['Longitude']) == 0:
			raise Exception("Longitude list is empty")
		if len(columns['Altitude(Ft)']) == 0:
			raise Exception("Altitude list is empty")
		if DEBUG:
			print columns['Latitude']
			print columns['Longitude']
			print columns['Altitude(Ft)']
		print 'Done with parsing'

	except EnvironmentError, e: #parent of IOError, OSError 
		print e 

if __name__ == "__main__":
	visualize()

