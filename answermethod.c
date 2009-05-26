#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

uint level = 21614,width=160,height=128
    ,imrate=25
    ,bitrate=200
    ,qscale=6,ok=0;
char* clip="127.0.0.1";




void start(DBusMessage* msg,DBusConnection* conn)
{
printf("video started \n");
   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &ok)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}



void image_size_set(DBusMessage* msg,DBusConnection* conn)
{  

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;
   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not integer!\n");

  printf("image size set\n");


   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &ok)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}


void image_rate_get(DBusMessage* msg,DBusConnection* conn)
{

   DBusMessage* reply;
   DBusMessageIter args;
   dbus_uint32_t serial = 0;
   char* param = "";

   // read the arguments
   if (!dbus_message_iter_init(msg, &args))
      fprintf(stderr, "Message has no arguments!\n");
   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
      fprintf(stderr, "Argument is not string!\n");
   else
      dbus_message_iter_get_basic(&args, &param);

   printf("image_rate_get called with %s\n", param);

   // create a reply from the message
   reply = dbus_message_new_method_return(msg);

   // add the arguments to the reply
   dbus_message_iter_init_append(reply, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &width)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &height)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }

   // send the reply && flush the connection
   if (!dbus_connection_send(conn, reply, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      exit(1);
   }
   dbus_connection_flush(conn);

   // free the reply
   dbus_message_unref(reply);
}

/**
* Server that exposes a method call and waits for it to be called
*/
void listen()
{
   DBusMessage* msg;
   DBusMessage* reply;
   DBusConnection* conn;
   DBusError err;
   int ret;
   dbus_uint32_t serial = 0;
   printf("Listening for method calls\n");

   // initialise the error
   dbus_error_init(&err);

   // connect to the bus and check for errors
   conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
   if (dbus_error_is_set(&err)) {
      fprintf(stderr, "Connection Error (%s)\n", err.message);
      dbus_error_free(&err);
   }
   if (NULL == conn) {
      fprintf(stderr, "Connection Null\n");
      exit(1);
   }

   // request our name on the bus and check for errors
   ret = dbus_bus_request_name(conn, "ch.cett.misse.ffmpeg",
DBUS_NAME_FLAG_REPLACE_EXISTING , &err);
   if (dbus_error_is_set(&err)) {
      fprintf(stderr, "Name Error (%s)\n", err.message);
      dbus_error_free(&err);
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
      fprintf(stderr, "Not Primary Owner (%d)\n", ret);
      exit(1);
   }

   // loop, testing for new messages
   while (true)
   {
      // non blocking read of the next available message
      dbus_connection_read_write(conn, 0);
      msg = dbus_connection_pop_message(conn);

      // loop again if we haven't got a message
      if (NULL == msg) {
         sleep(1);
         continue;
      }

      // check this is a method call for the right interface & method

      if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Introspectable", "Introspect")) 
	{
		printf("must do introspection \n");
	         static const char* introspect_xml =
	  	"<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
	 	 "                      \"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
		  "<node>\n"
		  "  <interface name=\"ch.cett.misse.ffmpeg\">\n"
	  	"    <method name=\"image_rate_get\">\n"
	  	"      <arg name=\"port\" direction=\"out\" type=\"uu\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"start\">\n"
	  //	"      <arg name=\"port\" direction=\"out\" type=\"uu\"/>\n"
	  	"    </method>\n"
	  	"    <method name=\"image_rate_set\">\n"
                "    <arg name=\"width\"  direction=\"in\" type=\"u\" /> \n"
                "    <arg name=\"height\"  direction=\"in\" type=\"u\" /> \n"
	  	"    <arg name=\"port\" direction=\"out\" type=\"u\"/>\n"
	  	"    </method>\n"

	 	 "  </interface>\n"
	 	 "</node>\n";
		reply = dbus_message_new_method_return(msg);
               if (!dbus_message_append_args (reply,
                                 DBUS_TYPE_STRING, &introspect_xml,
                                 DBUS_TYPE_INVALID))
		{
		      fprintf(stderr, "Out Of Memory!\n");
		      exit(1);
		}

		if (!dbus_connection_send(conn, reply, &serial)) 
		{
      		fprintf(stderr, "Out Of Memory!\n");
      		exit(1);
   		}
   		dbus_connection_flush(conn);

		dbus_message_unref(reply);
	}
      if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_rate_get")) 
	{
         image_rate_get(msg, conn);
	} 
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "start")) 
	{
	 start(msg,conn);
	}
     if (dbus_message_is_method_call(msg, "ch.cett.misse.ffmpeg", "image_size_set")) 
	{
	 image_size_set(msg,conn);
	}
/*
	else 
	 {
	      fprintf(stderr, "Unknown message: member=%s\n", dbus_message_get_member(msg));
         }
*/
      // free the message
      dbus_message_unref(msg);
   }

}




int main(int argc, char** argv)
{
listen();
return 0;
}

