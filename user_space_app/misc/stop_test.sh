GPIO_PIN="24"
echo 0 > /sys/class/gpio/gpio${GPIO_PIN}/value
echo $GPIO_PIN > /sys/class/gpio/unexport
