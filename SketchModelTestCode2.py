#UDP Socket Test 2.0

import os
import SocketServer
import serial
import math


#FILENAME = '~/Desktop/out.txt'

PORTNO = 10552

Dest = [48.85837,2.294481]
ServoPos=45
ser = serial.Serial('/dev/tty.usbmodem1411', 9600, timeout=1)

[TimeStamp, MagHead, TrueHead, HeadAcc, Lat, Long, LocAcc] = [0,0,0,0,0,0,0]


   

class handler(SocketServer.DatagramRequestHandler):
    def handle(self):
        newmsg = self.rfile.readline().rstrip()
        newmsgARR = [float(s) for s in newmsg.split(',')]
        
##        with open(FILENAME, "a") as out:
##            m  = "Client %s said ``%s''" % (self.client_address[0], newmsg)
##            out.write(m + '\n')
##        [TimeStamp, MagHead, TrueHead, HeadAcc, Lat, Long, LocAcc] = newmsg[0]
##        [TimeStamp, MagHead, TrueHead, HeadAcc, Lat, Long, LocAcc]2 = newmsg
        print newmsgARR
##        tc1=math.atan2(math.sin(Dest[1]-Long)*math.cos(lat2), \
##                      math.cos(Lat)*math.sin(Dest[0])-math.sin(Lat)*math.cos(Dest[0])*math.cos(Dest[1]-Long))
        Lat = float(newmsgARR[4])
        Long = float(newmsgARR[5])
        tc1=math.atan2(math.sin(Dest[1]-Long)*math.cos(Dest[0]),\
                       math.cos(Lat)*math.sin(Dest[0])-math.sin(Lat)*math.cos(Dest[0])*math.cos(Dest[1]-Long))
        
        tc1 = (int((tc1 % 2*math.pi)*360/math.pi))%360
        print tc1
        ServoPos = int(360- (newmsgARR[1] - tc1))
        print 'POS:' + str(ServoPos)
        ser.write('a'+str(ServoPos)+'z')
        self.wfile.write(self.server.oldmsg)
        self.server.oldmsg = newmsg
        
        
##os.remove(FILENAME)
s = SocketServer.UDPServer(('',PORTNO), handler)
print "Awaiting UDP messages on port %d" % PORTNO
s.oldmsg = "This is the starting message."
s.serve_forever()
