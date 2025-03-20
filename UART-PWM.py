import serial
from time import sleep
import RPi.GPIO as GPIO

# Definir pines
IN1 = 17  
IN2 = 27  
ENA1 = 12  
ENA2 = 13  

# Configuración de GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(IN1, GPIO.OUT)
GPIO.setup(IN2, GPIO.OUT)
GPIO.setup(ENA1, GPIO.OUT)
GPIO.setup(ENA2, GPIO.OUT)  

# Configuración PWM
pwm1 = GPIO.PWM(ENA1, 1000)
pwm2 = GPIO.PWM(ENA2, 1000)
pwm1.start(0)  
pwm2.start(0)  

# UART
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)

ser.reset_input_buffer()  
sleep(2)  

# Función de lectura del duty cycle
def leer_duty_cycle():
    try:
        with open("dutycycle.txt", "r") as file:
            duty = int(file.read().strip())
            if 0 <= duty <= 100:
                return duty
            else:
                print("⚠ Valor fuera de rango (0-100), usando 50% por defecto.")
                return 50
    except Exception as e:
        print(f"⚠ Error leyendo el archivo: {e}, usando 50% por defecto.")
        return 50

while True:
    try:
        if ser.in_waiting > 0:  
            value = ser.readline().decode('utf-8').rstrip()  
            print(f"Datos recibidos: {value}")  

            if value == "MOTOR1":  
                GPIO.output(IN1, GPIO.HIGH)
                GPIO.output(IN2, GPIO.LOW)
                duty_cycle = leer_duty_cycle()
                pwm1.ChangeDutyCycle(duty_cycle)  
            elif value == "MOTOR2":  
                GPIO.output(IN1, GPIO.LOW)
                GPIO.output(IN2, GPIO.HIGH)
                duty_cycle = leer_duty_cycle()
                pwm2.ChangeDutyCycle(duty_cycle)  
    except Exception as e:
        print(f"Error: {e}")
