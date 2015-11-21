#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ftplib
import sys
import os
import time

verbose = True

if verbose:
	ftp = ftplib.FTP('ftp2.bom.gov.au')
	ftp.login()
	print ftp.getwelcome()
	ftp.close()

folder = 'data/' + time.strftime("%b %d %Y %H:%M:%S")
os.mkdir(folder)


#loop forever
while True :

	try:
		now = time.strftime("%b %d %Y %H:%M:%S")

		print 'attampting to connect @ ', now
		
		ftp = ftplib.FTP('ftp2.bom.gov.au')
		ftp.login()
		ftp.cwd('anon/gen/radar/')

		files = []
		files = ftp.nlst()

		nfound = 0
		ndownloaded = 0
		nskipped = 0

		for f in files:
			if 'IDR024' in f and '.png' in f:
				nfound += 1

				if f not in os.listdir(folder):
					print 'downloading ', f, ' at ', now 
					ndownloaded += 1

					localf = open(folder + '/' + f, 'wb');
					ftp.retrbinary('RETR ' + f, localf.write, 8*1024)
					localf.close

					#todo
					#if faildeddownloads =< 3
						#try again

				else:
					nskipped += 1

		if verbose:
			print 'found: ', nfound
			print 'skipped: ', nskipped
			print 'downloaded: ', ndownloaded

		ftp.quit()

	except ftplib.all_errors as e:
		print 'ftp connection failed. ', e
		# probably a good idea to print out error message here


	#interval between checking for new images
	time.sleep(600)

