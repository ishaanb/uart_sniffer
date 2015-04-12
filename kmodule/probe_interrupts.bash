
#!/bin/bash
echo "" >  interrupts.log
while (true)
do
	cat /proc/interrupts | grep "Timer Tick" >> interrupts.log
	sleep 0.0001
done	
