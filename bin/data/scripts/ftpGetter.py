	#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ftplib

ftp = ftplib.FTP('ftp2.bom.gov.au')
ftp.login()

files = []
files = ftp.nlst('anon/gen/radar/')
# ftp.cwd('anon/gen/radar/')
# files = ftp.nlst()
for f in files:
	# print 'size=',ftp.size(f)
	print f

#todo - filter out files of size < x ?