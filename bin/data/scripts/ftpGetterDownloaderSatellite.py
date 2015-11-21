#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ftplib
import sys
import os
import time

verbose = True

#create capture folder based on time and date
folder = 'data/satellite/' + time.strftime("%b %d %Y %H:%M:%S")
os.mkdir(folder)

#current capture
currentcapture = 0
currentsubfolder = 0

if verbose:
	ftp = ftplib.FTP('ftp2.bom.gov.au')
	ftp.login()
	print ftp.getwelcome()
	ftp.close()

try:
	now = time.strftime("%b %d %Y %H:%M:%S")

	print 'attampting to connect @ ', now

	ftp = ftplib.FTP('ftp2.bom.gov.au')
	ftp.login()
	ftp.cwd('anon/gen/difacs/')

	files = []
	files = ftp.nlst()
	files.sort()


	for f in files:
		if 'IDX1' in f and '.gif' in f:
			subfolder = folder + '/' + str(currentsubfolder).zfill(2)
			try:
				os.mkdir(subfolder)
			except:
				pass

			print 'downloading ', f, ' at ', now 

			localf = open(subfolder + '/' + f, 'wb');
			ftp.retrbinary('RETR ' + f, localf.write, 8*1024)
			localf.close

			#todo
			#if faildeddownloads =< 3
				#try again

			currentcapture += 1
			currentsubfolder = currentcapture/24

	if verbose:
		print 'nCaptures: ', currentcapture
		print 'nFolders ', currentsubfolder

	ftp.quit()

except ftplib.all_errors as e:
	print 'ftp connection failed. ', e
	# probably a good idea to print out error message here