#!/usr/bin/env python
import dbus

service = "ch.cett.misse.ffmpeg"
bus = dbus.SessionBus()
eth0 = bus.get_object(service,"/")
ffmpeg = dbus.Interface(eth0,dbus_interface="ch.cett.misse.Type")

#ffmpeg = dbus.Interface(dbus.SessionBus().get_object(service,"/"), service)

print ffmpeg.image_rate_get()
 
