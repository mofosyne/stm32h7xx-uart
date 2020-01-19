#!/usr/bin/env python3

import sys
import serial
import struct
from time import sleep


# TOOLS
def sendhex(payload, DESCP=""):
  b = bytearray(payload)
  ser.write(b)
  #print("tx: "+str(payload)+" - "+str(b))
  print("tx: "+b.hex()+" ("+DESCP+")")

def readhex(count, DESCP=""):
  in_bin = ser.read(count)
  #print("rx: "+hex(int.from_bytes(in_bin,byteorder='big')))
  print("rx: "+in_bin.hex()+" ("+DESCP+")")


PORT=sys.argv[1]
print(f"PORT: {PORT}")

# SERIAL SETUP (ST use 8E1, but we are using 8N1 for convenience for now)
#ser = serial.Serial(port=PORT, baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=0.1)
ser = serial.Serial(port=PORT, baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=0.1)


print("-----------------------------------------------------------------------")
print("WRITE")
# STM32BL_CMD_WRITE
addr = 0x08020000;
nBytes = 256

#buff = [00, 0x00, 0x02, 0x20, 0x65, 0x55, 0x02, 0x08, 0x21, 0x0f, 0x02, 0x08, 0x25, 0x0f, 0x02, 0x08, 0x29, 0x0f, 0x02, 0x08, 0x2d, 0x0f, 0x02, 0x08, 0x31, 0x0f, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x0f, 0x02, 0x08, 0x39, 0x0f, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x0f, 0x02, 0x08, 0x41, 0x0f, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08, 0xbd, 0x55, 0x02, 0x08]
buff = []
for i  in range(0, nBytes):
  buff.append((i%10))

# WTF... we need to split this up to make this test work?
buff1 = []
for i  in range(0, 128+1):
  buff1.append((i%10))
buff2 = []
for i  in range(128+1, 256):
  buff2.append((i%10))
buff3 = []
for i  in range(256+1, 384):
  buff3.append((i%10))
buff4 = []
for i  in range(384+1, 512):
  buff4.append((i%10))

# Data Payload CRC calculation
crcc = len(buff)-1; # Checksum includes the nBytes byte
for i  in range(0, len(buff)):
  crcc = crcc ^ buff[i]

print("nBytes: " + str(len(buff)))
print("buff: "   + str(buff))
print("crcc: "   + str(crcc))

sleep(1)

while 1:
  sendhex(buff, "buff")
  #sendhex(buff1, "buff")
  #sendhex(buff2, "buff")
  #sendhex(buff3, "buff")
  #sendhex(buff4, "buff")
  sleep(3);


print(ser)