# Raspberry-Pi-Pico-function-generator

n bit function generator for the Raspberry Pi Pico

Work in progress...

## Schematic
<table>
  <tr>
    <td>
      <img src="https://raw.githubusercontent.com/MarkusVLA/Raspberry-Pi-Pico-function-generator/main/img/schematic.jpg" alt="Schematic">
    </td>
    <td>
      <img src="https://raw.githubusercontent.com/MarkusVLA/Raspberry-Pi-Pico-function-generator/main/img/pcb.jpg" alt="PCB">
    </td>
  </tr>
</table>


## Usage

Ensure you have the Raspberry Pi Pico SDK installed on your system. If you haven't installed it yet, you can clone the SDK repository by running the following commands:

```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

Edit the Makefile in the top directory of the project to include the path to the pico-sdk you installed.

```Makefile
all:
	cd build && export PICO_SDK_PATH=/path/to/pico-sdk
	cd build && cmake ../src 
	cd build && make
```

Now you can run the makefile to compile the program.

```bash
make
```
Connect the GPIO pins 0 to 7 to the ADC and upload the compiled firmware.

![alt text](https://raw.githubusercontent.com/MarkusVLA/Raspberry-Pi-Pico-function-generator/main/img/scope.jpg)
Vout in oscilloscope

