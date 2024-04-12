#!/bin/python3

import time 
from time import sleep
import os
import serial 
import threading 
f = serial.Serial('/dev/ttyACM0', timeout=4)

os.environ['TZ'] = 'CST6CDT'

time.tzset()

stat = ''

now = ''
eclipse = False 
last = '12:17:30'

c1 =  '12:17:51'

c2rotate  = '13:35:55'
c2 =  '13:36:06'
maxTotality = '13:37:35'
c3 =  '13:39:04' 
c3rotate = '13:39:10'

def reader():
	print("Opened serial")
	while True:
		sleep(1)
		stat = f.readline().decode('ascii').strip()
		print("  " + now + " " +  stat, end ="\r")


readerThread = threading.Thread(target=reader)
readerThread.start()
while True:
	try:
		sleep(0.3)
		now = time.strftime('%H:%M:%S')
		if now != last:
			if last == '12:17:30':
				print ('Tracking')
			last = now
			if now == c1:
				eclipse=True
				#Make sure fliter is rotated in
				f.write(b'U')
				print(now + ": Eclipse started")
			if now == c2rotate:
				f.write(b'C')
				print(now + ": rotating filter out for C2")
				#rotate fliter out
			if now == maxTotality:
				pass
			if now == c3rotate:
				f.write(b'U')
				#rotate fliter in
				print(now + ": rotating filter in for C3")

	except Exception as e:
		print (e)
		pass



