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

def readStrToInt(count):
  try:
    stringVal = ser.read(count).decode("utf-8")
    print("rx: "+str(stringVal))
    return int(stringVal);
  except ValueError:
    return 0

def genTestByteBuffer(nBytes):
  buff = []
  for i  in range(0, nBytes):
    buff.append((i%9)+1)
  return buff

# Args
PORT=sys.argv[1]

################################################################################
ser = serial.Serial(port=PORT, baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=5)
print("-----------------------------------------------------------------------")
print(ser)
print("-----------------------------------------------------------------------")
print(f"PORT: {PORT}")
print("-----------------------------------------------------------------------")

while 1:
  print("-----------------------------------------------------------------------")
  nByte = readStrToInt(3);
  if nByte > 0:
    byteBuff = genTestByteBuffer(nByte)
    print("Tx buff (" + str(nByte) + ") :" + str(byteBuff))
    sendhex(byteBuff, "buff")
  else:
    print("Timeout")
  sleep(0.1);


