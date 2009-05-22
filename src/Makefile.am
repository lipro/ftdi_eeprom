bin_PROGRAMS = ftdi_eeprom

# set the include path found by configure
INCLUDES = @LIBFTDI_CFLAGS@

# the library search path.
ftdi_eeprom_LDFLAGS = 
ftdi_eeprom_SOURCES = main.c

noinst_DATA =  example.conf
ftdi_eeprom_LDADD = -lconfuse @LIBFTDI_LIBS@

EXTRA_DIST = example.conf
