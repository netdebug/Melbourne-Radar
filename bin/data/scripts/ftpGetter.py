#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ftplib

ftp = ftplib.FTP('ftp2.bom.gov.au')
ftp.login()

files = []

files = ftp.nlst('anon/gen/radar/')
for f in files:
    print f
