import serial

PORT = 'COM5'
BAUD_RATE = 115200
FILENAME = "dataHuman.csv"


ser = serial.Serial('COM5', 115200, timeout=1)

EXPECTED_COLUMNS = 7
try:
    with open(FILENAME, "w") as file:
        file.write("ballX,ballY,vy,vx,paddleY,deltaY,action\n")
      
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip() #utf-8 format

            if not line:  #no log empty line  
                continue
            
            parts = line.split(',')
            if len(parts) == EXPECTED_COLUMNS:
                try: 
                    [float(p) for p in parts] #ep kieu sang float
                    print(line) #print line for debugging
                    file.write(line + "\n")
                    file.flush() #force write from buffer
                except ValueError:
                    continue
            else:
                continue

except KeyboardInterrupt:
    print("Stop logging")

except Exception as error: 
    print(error)
finally:
    ser.dtr = False # Giai phong reset
    ser.rts = False
    ser.close()