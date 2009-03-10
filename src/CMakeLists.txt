ADD_EXECUTABLE(ftdi_eeprom main.c)
TARGET_LINK_LIBRARIES(ftdi_eeprom ${libs})

install (TARGETS ftdi_eeprom DESTINATION bin)
