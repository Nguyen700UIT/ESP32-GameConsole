import serial

ser = serial.Serial('COM5', 115200)

with open("data.csv", "w") as file:
    file.write("ballY,vy,vx,paddleY,action\n")

    while True:
        try:
            line = ser.readline().decode('utf-8').strip() #utf-8 format

            if not line:  #no log empty line  
                continue

            print(line) #print line for debugging

            file.write(line + "\n")
            file.flush() #force write from buffer
        
        except KeyboardInterrupt:
            print("Stop logging")
            break

