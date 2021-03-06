# Makefile for ZYNQ_driver
# This driver is to be used with a higher level program
# which sends the bits to be set to the ZYNQ_driver
# History:
# 	MEP 6/3/14, initial version

.PHONY: clean

CC                 = arm-xilinx-linux-gnueabi-gcc
LD		   = arm-xilinx-linux-gnueabi-gcc

CFLAGS	= -c -Wall

C_EXT = c
OBJ_EXT = o
EXE_EXT = exe

EXE = gpio_test_1.$(EXE_EXT) gpio_test_2.$(EXE_EXT) gpio_test_3.$(EXE_EXT) gpio_test_4.$(EXE_EXT)
DRIVER = ZYNQ_driver.$(OBJ_EXT) 
OBJECTS = gpio_test_1.$(OBJ_EXT) gpio_test_2.$(OBJ_EXT) gpio_test_3.$(OBJ_EXT) gpio_test_4.$(OBJ_EXT) \
	  $(DRIVER)

%.o : %.c
	$(CC) $(CFLAGS) $*.$(C_EXT) -o $*.$(OBJ_EXT)

gpio_test_1.$(EXE_EXT): gpio_test_1.$(OBJ_EXT) $(DRIVER)
	$(LD) -o gpio_test_1.$(EXE_EXT) $^

gpio_test_2.$(EXE_EXT): gpio_test_2.$(OBJ_EXT) $(DRIVER)
	$(LD) -o gpio_test_2.$(EXE_EXT) $^

gpio_test_3.$(EXE_EXT): gpio_test_3.$(OBJ_EXT) $(DRIVER)
	$(LD) -o gpio_test_3.$(EXE_EXT) $^

gpio_test_4.$(EXE_EXT): gpio_test_4.$(OBJ_EXT) $(DRIVER)
	$(LD) -o gpio_test_4.$(EXE_EXT) $^

all: $(EXE)
	

clean:
	rm -f $(OBJECTS) $(EXE)
