#!/usr/bin/python3.5
import numpy as np
import cv2
import wiringpi2 as wpi
import time


face = cv2.CascadeClassifier('/home/odroid/Desktop/bullseye_Stage0-15.xml')
cap = cv2.VideoCapture(0)
cx = 0

wpi.wiringPiSetup()  #set up 
wpi.pinMode(0,1)
wpi.digitalWrite(0,0)

##cap.set(cv2.CAP_PROP_FRAME_WIDTH, 325)  #resolution changes 
##cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 325)

serial = wpi.serialOpen('/dev/ttySAC0', 9600)  #open serial port for UART 

def moveCenter(x):
    if x<250:
         print("Left")
         wpi.serialPuts(serial, "2")  #Sends TX commands
         time.sleep(0.1)
         return 0
    elif x>375:
         print("Right")
         wpi.serialPuts(serial, "1")
         time.sleep(0.1)
         return 0
         
    else:
        print("MID")
        cx=0
        wpi.serialPuts(serial, "D")
        time.sleep(0.1)
         
        return 1
    
def moveForward(P):
    if P<700:
        print("FORWARD")
        wpi.serialPuts(serial, "3")
        time.sleep(0.1)
        return 0
    else:
        wpi.serialPuts(serial, "D")
        time.sleep(0.1) 
        print("reached", P)
        return 1
        
def runImage():   #this function follows a user for a set amount of time
                  #This is ran with the heel command
    p_prev = 0
    z=0
    wpi.serialPuts(serial, "D")
    wpi.serialPuts(serial, "D")
    time.sleep(0.1)
    flag = 0
    while z != 2000:
        
          a = 0
          b=0
          
          z = z+1
          ret, img = cap.read()
          wpi.digitalWrite(0,0)
          gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
          faces = face.detectMultiScale(gray, 1.3, 1,minSize=(110,110))
          faces = face.detectMultiScale(gray, 1.1, 1)

          for (x,y,w,h) in faces:
              flag = 1
              p = x+y+w+h
              cx = x+w//2
              a = moveCenter(cx);
              b = moveForward((p+p_prev)//2);
              cy = y+h//2
              cv2.rectangle(img, (x,y), (x+w, y+h), (178,255,34),5) #draws rectange around image, for debugging purposes. Comment out during ops

              wpi.digitalWrite(0,1)	                    #turn on LED
              
              if a != 1:              #function sets a = 1 when robot is centered
                 a = moveCenter(cx);
              if a ==1:             #when a = 1 robot moves forward based on size of p
                                      
                  if b == 0:
                      b = moveForward((p+p_prev)//2);
              p_prev = p
          if flag == 0:
                  wpi.serialPuts(serial, "D")
                  time.sleep(0.1)
          else:
              flag = 0
              
              
          cv2.imshow('Cam',img)  #comment out during operation
          if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    wpi.serialPuts(serial, "D")   #send default value to UART
    time.sleep(0.1)                 
    wpi.digitalWrite(0,0)       #turn off LED
    cap.release()               #destroy windows
    cv2.destroyAllWindows()

def approach():   #like runImage funtion but it only approaches user. Might need to improve this part
    z=0
    b=0
    while z != 10:
          a = 0
          
          
       
          ret, img = cap.read()
          
          gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
          #faces = face.detectMultiScale(gray, 1.3, 1,minSize=(110,110))
          faces = face.detectMultiScale(gray, 1.1, 1)
          

          for (x,y,w,h) in faces:
              cx = x+w//2
    ##        cy = y+h//2
              cv2.rectangle(img, (x,y), (x+w, y+h), (178,255,34),5)

              p = x+y+w+h
              print(p)
              
              
              if a != 1:
                 a = moveCenter(cx);
              if a ==1:
                  #print(p)
                  if b >=0:
        
                      b = b+moveForward(p);
                      ##print(b)
                      if b == 3:
                          z = z+1
                          b = 0
              
               

                  
              #print("a:",a, " b:",b)    

          cv2.imshow('Cam',img)
          if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()
    

runImage()
#approach()
