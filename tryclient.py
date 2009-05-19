#!/usr/bin/env python
import dbus

service = "ch.cett.misse.ffmpeg"
ffmpeg = dbus.Interface(dbus.SessionBus().get_object(service,"/"), service)
print 'Available commands are: \n'
print '1- start \n'
print '2- stop \n'
print '3- image_size_set \n'
print '4- image_size_get \n'
print '5- image_rate_set \n'
print '6- image_rate_get \n'
print '7- bit_rate_set \n'
print '8- bit_rate_get \n'
print '9- qscale_set \n'
print '10- qscale_get \n'
print '11- client_ip_set \n'
print '12- client_ip_get \n'

while True:
 x = int(raw_input("enter the number of the command: "))
 if x < 1:
      exit()
 elif x == 1:
      print ffmpeg.start()
 elif x == 2:
      ffmpeg.stop()
 elif x == 3:
      w = int(raw_input("the width of the frame is: "))
      h = int(raw_input("the height of the frame is: "))
      print ffmpeg.image_size_set(w, h)
 elif x == 4:
      print ffmpeg.image_size_get()
 elif x == 5:
      r= int(raw_input("the image rate is: "))
      print ffmpeg.image_rate_set(r)
 elif x == 6:
      print ffmpeg.image_rate_get()
 elif x == 7:
      r = int(raw_input("the bit rate is: "))
      print ffmpeg.bit_rate_set (r)
 elif x == 8:
      print ffmpeg.bit_rate_get()
 elif x == 9:
      r = int(raw_input("the qscale is: "))
      print ffmpeg.qscale_set (r)
 elif x == 10:
      print ffmpeg.qscale_get()
 elif x == 11:
      s=raw_input("the ip is: ")
      print ffmpeg.client_ip_set(s)
 elif x == 12:
      print ffmpeg.client_ip_get()

 else:
      print 'invalid'


