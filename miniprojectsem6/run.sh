if [-f serial_log.txt];then
	rm serial_log.txt
fi
if [-f output.json];then
	rm output.json
fi
python3 loggeresp32sensor.py &
sleep 1000s
python3 firebasesendesp32sensor.py &
