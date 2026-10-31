import serial
from datetime import datetime

PORT = "/dev/ttyUSB0"   # Windows: "COM3"
BAUD = 9600
LOG_FILE = "serial_log.txt"
LOG_INTERVAL = 4  # seconds between logs

# Only log lines containing these keys
VALID_KEYS = ["Node_ID","Sensor_moisture","Pump","Valve_ID","Valve_switch_1","Valve_switch_2"] 

last_logged_time = datetime.min


def is_valid_data(line):
    return any(line.startswith(key) for key in VALID_KEYS)


ser = serial.Serial(PORT, BAUD, timeout=1)
ser.setDTR(False)
ser.setRTS(False)

print(f"Logging only valid sensor data to {LOG_FILE}")

try:
    with open(LOG_FILE, "w", buffering=1) as f:  # line-buffered
        while True:
            raw_line = ser.readline()

            try:
                line = raw_line.decode("utf-8", errors="ignore").strip()
            except Exception:
                continue

            now = datetime.now()

            if line and is_valid_data(line):
                timestamp = now.strftime("%Y-%m-%d %H:%M:%S")
                log_line = f"[{timestamp}] {line}\n"

                print(log_line, end="")
                f.write(log_line)

                last_logged_time = now

except KeyboardInterrupt:
    print("\nStopped logging")

finally:
    ser.close()

