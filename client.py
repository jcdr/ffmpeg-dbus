#!/usr/bin/env python
import dbus

service = "ch.cett.misse.ffmpeg"
bus = dbus.SessionBus()
object = bus.get_object(service,"/")
ffmpeg = dbus.Interface(object, dbus_interface=service)

print ffmpeg.image_rate_get()
