GPIO_PIN="24"
echo $GPIO_PIN > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio${GPIO_PIN}/direction
echo 1 > /sys/class/gpio/gpio${GPIO_PIN}/value
