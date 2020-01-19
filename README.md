This is an example of a uart based project. Currently investigating why 256 bytes using uart hal is not read correctly.


```
# Load and run test on micro
make swd

# Turn on swo viewer
./swo_parser.py

# Send test data to device
./stm32_impulse_test.py /dev/tty.usbmodem14403
```

## For hal uart rx buffer size of 256-12

```
I: ************************* Rx *********************************
E: Rx Received (HAL_OK)
I: Got 244 valid bytes
I: Rx :
    0 |  01 02 03 04 05 06 07 08 09 01 02 03 04 05 06 07
   16 |  08 09 01 02 03 04 05 06 07 08 09 01 02 03 04 05
   32 |  06 07 08 09 01 02 03 04 05 06 07 08 09 01 02 03
   48 |  04 05 06 07 08 09 01 02 03 04 05 06 07 08 09 01
   64 |  02 03 04 05 06 07 08 09 01 02 03 04 05 06 07 08
   80 |  09 01 02 03 04 05 06 07 08 09 01 02 03 04 05 06
   96 |  07 08 09 01 02 03 04 05 06 07 08 09 01 02 03 04
  112 |  05 06 07 08 09 01 02 03 04 05 06 07 08 09 01 02
  128 |  03 04 05 06 07 08 09 01 02 03 04 05 06 07 08 09
  144 |  01 02 03 04 05 06 07 08 09 01 02 03 04 05 06 07
  160 |  08 09 01 02 03 04 05 06 07 08 09 01 02 03 04 05
  176 |  06 07 08 09 01 02 03 04 05 06 07 08 09 01 02 03
  192 |  04 05 06 07 08 09 01 02 03 04 05 06 07 08 09 01
  208 |  02 03 04 05 06 07 08 09 01 02 03 04 05 06 07 08
  224 |  09 01 02 03 04 05 06 07 08 09 01 02 03 04 05 06
  240 |  07 08 09 01
```

## For hal uart rx buffer size of 256+12

```
I: ************************* Rx *********************************
E: Rx Timeout (HAL_OK)
E: Got only 12 valid bytes out of 268
I: Rx :
    0 |  01 02 03 04 05 06 07 08 09 01 02 03 01 02 03 04
   16 |  05 06 07 08 09 01 02 03 00 00 00 00 00 00 00 00
   32 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
   48 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
   64 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
   80 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
   96 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  112 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  128 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  144 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  160 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  176 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  192 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  208 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  224 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  240 |  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  256 |  00 00 00 00 00 00 00 00 00 00 00 00
```

## For hal uart rx buffer size of 256

```
I: ************************* Rx *********************************
E: Rx Timeout (HAL_OK)
E: Got only 0 valid bytes out of 256
I: Rx :
 0 | All 00 (256 Bytes)
```

--------------

Forked from:

# stm32h7xx-cubemx-makefile-openocd-gdbgui-example

This is a makefile based workflow experiment on programming and automated CI build/test of this board.

To test this repositiory you need:
* NUCLEO-H743ZI2 dev board (stlinkV3 onboard)

You also need to have installed
* openocd
* arm-none-eabi-* build tools (etc... arm-none-eabi-gcc)
* gdbgui

It's important to have GDB gui, since what is nice about STM32IDE is the visual GUI. However I've not yet worked out how to get CMSIS perpherial register views to function correctly.

**This is a template, you can click on the green button on top in github to copy it**

## Status

* Sample program flashes led correctly in sequence on a NUCLEO-H743ZI2
* Confirmed that `make swd` works
* Confirmed that `make swd-gdbgui` works
* Added comments to gdbinit to take dual flash bank into account (Important for bootloader dev).

WIP:

* CI build using circleCI or bitbucket etc...
* Later on, use this as a basis for a bootloader and application
* Is there a way to not clash with cubemx makefile code generation, when manually modifying the makefile that it generated?

## What was added or changed?

This was initially from a basic cubemx generated project for a NUCLEO-H743ZI2, where the output is a makefile project.

From there, this is what was modified or added to this project:

* `Makefile`                  : Modified to add useful debugging commands and best practices
* `Makefile_ci_assist.make`   : Useful for CI builds (Work In Progress)
* `gdbinit_stm32h7xx`         : gdbinit script for stm32h7xx
* `gdbinit_stm32h7xx_attach`  : Similar to `gdbinit_stm32h7xx` but for actively running devices
* `./Drivers/CMSIS/Lib/`      : Removed as it had >10mb libary file that does not appear to be used and conflicts with Github's templating feature. You can run cubemx again if you need it.

### Makefile

This was added near the bottom of the makefile to provide

* `make swd`                : Load binary via swd programmer and launch gdb
* `make swd-attach`         : Attach to running device with same firmware
* `make swd-console`        : Access semihost console only (if avaliable)
* `make swd-gdbgui`         : Launch gdbgui and load program
* `make swd-gdbgui-attach`  : Launch gdbgui and attach to running device with same firmware

Also added `-std=c99` as we like modern C99 conveniences (e.g. variable init in for loops).
And added checks for all warnings and throw errors for all warnings.

(Note: For below snippet you need to convert the 2 space to tabs in makefile)

```.c
#######################################
# Custom Flags (Not Autogenerated)
#######################################

# Enable C99 (gcc C std default is C90)
CFLAGS += -std=c99

# Good Practices
# (Check all warnings `-Wall` and make all warnings and throw error `-Werror` )
CFLAGS += -Wall -Wpointer-arith -Werror


#######################################
# SWD
#######################################
BIN_IMAGE = $(BUILD_DIR)/$(TARGET).elf

# make swd (OpenOCD CLI Telnet server on TCP port 4444)
swd: all
  arm-none-eabi-gdb -x gdbinit_stm32h7xx $(BIN_IMAGE)
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log

# make swd-attach
swd-attach: all
  arm-none-eabi-gdb -x boards/gdbinit_stm32h7xx_attach $(BIN_IMAGE)
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log

# make swd-console
swd-console: all
  @echo "--------------- WARN: Must have semihost enabled FW ------------------"
  /usr/local/bin/openocd                                                       \
    -s /usr/local/share/openocd/scripts                                        \
    -f /usr/local/share/openocd/scripts/interface/stlink.cfg                   \
    -f /usr/local/share/openocd/scripts/target/stm32h7x_dual_bank.cfg          \
    -c "init; arm semihosting enable; log_output /tmp/openocd.log; reset run;"
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log

# make swd-load
swd-load: all
  /usr/local/bin/openocd                                                       \
    -f /usr/local/share/openocd/scripts/interface/stlink.cfg                   \
    -f /usr/local/share/openocd/scripts/target/stm32h7x_dual_bank.cfg          \
    -c "log_output /tmp/openocd.log"                                           \
    -c init                                                                    \
    -c "reset halt"                                                            \
    -c "flash write_image erase ${BIN_IMAGE}"                                  \
    -c "verify_image ${BIN_IMAGE}"                                             \
    -c reset                                                                   \
    -c shutdown
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log

# make swd-gdbgui
swd-gdbgui: all
  gdbgui --gdb-args="-x gdbinit_stm32h7xx" $(BIN_IMAGE)
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log

# make swd-gdbgui-attached
swd-gdbgui-attached: all
  gdbgui --gdb-args="-x gdbinit_stm32h7xx_attach" $(BIN_IMAGE)
  @echo "--------------- openocd.log (First 20 lines) ---------------------"
  @head -20 /tmp/openocd.log


##################################################
# CI Auto Package (Not Typically Used During Dev)
##################################################
ifdef GCC_PATH
STRIP = $(GCC_PATH)/$(PREFIX)strip
STRINGS = $(GCC_PATH)/$(PREFIX)strings
else
STRIP = $(PREFIX)strip
STRINGS = $(PREFIX)strings
endif

$(BUILD_DIR)/$(TARGET).pub.elf: $(BUILD_DIR)/$(TARGET).elf
  $(Q)$(STRIP) $< -o $@
  $(Q)$(SZ) $@

$(BUILD_DIR)/$(TARGET).pub.bin: $(BUILD_DIR)/$(TARGET).pub.elf
  $(Q)$(CP) -O binary $< $@

$(BUILD_DIR)/$(TARGET).pub.hex: $(BUILD_DIR)/$(TARGET).pub.elf
  $(Q)$(CP) -O ihex $< $@

zipped_public_release: $(BUILD_DIR)/$(TARGET).pub.elf $(BUILD_DIR)/$(TARGET).pub.hex $(BUILD_DIR)/$(TARGET).pub.bin
  @echo "zipping up binary releases (symbol table stripped)";
  mkdir -p $(BUILD_DIR)/pub_release/
  cp $(BUILD_DIR)/$(TARGET).pub.elf                $(BUILD_DIR)/pub_release/
  cp $(BUILD_DIR)/$(TARGET).pub.bin                $(BUILD_DIR)/pub_release/
  cp $(BUILD_DIR)/$(TARGET).pub.hex                $(BUILD_DIR)/pub_release/
  cp gdbinit_stm32h7xx                             $(BUILD_DIR)/pub_release/gdbinit_stm32h7xx
  cp gdbinit_stm32h7xx_attach                      $(BUILD_DIR)/pub_release/gdbinit_stm32h7xx_attach
  cp Makefile_ci_assist.make                       $(BUILD_DIR)/pub_release/Makefile

zipped_internal_release:
  @echo "zipping up binary releases (has symbol table)";
  mkdir -p $(BUILD_DIR)/prv_release/
  cp $(BUILD_DIR)/$(TARGET).elf                    $(BUILD_DIR)/prv_release/
  cp $(BUILD_DIR)/$(TARGET).bin                    $(BUILD_DIR)/prv_release/
  cp $(BUILD_DIR)/$(TARGET).hex                    $(BUILD_DIR)/prv_release/
  cp $(BUILD_DIR)/$(TARGET).map                    $(BUILD_DIR)/prv_release/
  cp STM32H743XIHx_FLASH.ld                        $(BUILD_DIR)/prv_release/
  cp gdbinit_stm32h7xx                             $(BUILD_DIR)/prv_release/gdbinit_stm32h7xx
  cp gdbinit_stm32h7xx_attach                      $(BUILD_DIR)/prv_release/gdbinit_stm32h7xx_attach
  cp Makefile_ci_assist.make                       $(BUILD_DIR)/prv_release/Makefile
```