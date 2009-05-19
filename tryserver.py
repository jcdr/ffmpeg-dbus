#!/usr/bin/env python
import subprocess
import signal
import gobject
import dbus
import dbus.service
import dbus.mainloop.glib
import os

service = "ch.cett.misse.ffmpeg"

class FFMpeg(dbus.service.Object):
    width=160
    height=128
    imrate=25
    bitrate=200
    clip='10.0.33.1'
    qscale=6
    @dbus.service.method(service, in_signature='', out_signature='i')
    def start(self):
        print "Should start ffmpeg"


	s='ffmpeg -f video4linux2 -s '+ str(self.width)+'x'+str(self.height)+' -r '+ str(self.imrate) +' -b '+str(self.bitrate) +' -i  /dev/video0 -qscale '+str(self.qscale)+' -f mjpeg  udp:'+self.clip+':1234'
        self.p =subprocess.Popen(s, shell=True,stdout=subprocess.PIPE)
        return 0

    @dbus.service.method(service, in_signature='', out_signature='i')
    def stop(self):
        print "Should stop ffmpeg"
        os.kill(self.p.pid, signal.SIGTERM)
        while self.p.poll() == 0:
            pass
        return 0

    @dbus.service.method(service, in_signature='ii', out_signature='i')
    def image_size_set(self, width, height):
        print "image_size_set: width=%d height=%d"%(width, height)
	self.width = width
	self.height = height
        return 0

    @dbus.service.method(service, in_signature='', out_signature='ii')
    def image_size_get(self):
        print "image_size_get:"
	return (self.width, self.height)


    @dbus.service.method(service, in_signature='i', out_signature='i')
    def image_rate_set(self,imrate):
        print "image_rate_set: %d"%imrate
	self.imrate=imrate
	return 0

    @dbus.service.method(service, in_signature='', out_signature='i')
    def image_rate_get(self):
        print "image_rate_get:"
	return (self.imrate)

    @dbus.service.method(service, in_signature='i', out_signature='i')
    def bit_rate_set(self,bitrate):
        print "bit_rate_set: %d"%bitrate
	self.bitrate=bitrate
	return 0



    @dbus.service.method(service, in_signature='', out_signature='i')
    def bit_rate_get(self):
        print "Bit_rate_get:"
	return (self.bitrate)




    @dbus.service.method(service, in_signature='i', out_signature='i')
    def qscale_set(self,qscale):
        print "qscale_set: %d"%qscale
	self.qscale=qscale
	return 0



    @dbus.service.method(service, in_signature='', out_signature='i')
    def qscale_get(self):
        print "qscale_get:"
	return (self.qscale)


  

    @dbus.service.method(service, in_signature='s', out_signature='i')
    def client_ip_set(self,clip):
        print "client_ip_set:"
        self.clip=clip
	return 0


    @dbus.service.method(service, in_signature='', out_signature='s')
    def client_ip_get(self):
        print "client_ip_get:"
	return (self.clip)




if __name__ == '__main__':
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus = dbus.SessionBus()
    name = dbus.service.BusName(service, bus)
    object = FFMpeg(bus, '/')
    mainloop = gobject.MainLoop()
    mainloop.run()

