all:
	cd build && export PICO_SDK_PATH=/Path/to/pico-sdk
	cd build && cmake ../src 
	cd build && make
	
	