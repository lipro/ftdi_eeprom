/***************************************************************************
                             main.c  -  description
                           -------------------
    begin                : Mon Apr  7 12:05:22 CEST 2003
    copyright            : (C) 2003 by Intra2net AG
    email                : opensource@intra2net.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <confuse.h>
#include <ftdi.h>

int main(int argc, char *argv[]) {
    /*
	configuration options
    */
    cfg_opt_t opts[] = {
	CFG_INT("vendor_id", 0, 0),
	CFG_INT("product_id", 0, 0),
	CFG_BOOL("self_powered", cfg_true, 0),
	CFG_BOOL("remote_wakeup", cfg_true, 0),
	CFG_BOOL("BM_type_chip", cfg_true, 0),
	CFG_BOOL("in_is_isochronous", cfg_false, 0),
	CFG_BOOL("out_is_isochronous", cfg_false, 0),
	CFG_BOOL("suspend_pull_downs", cfg_false, 0),
	CFG_BOOL("use_serial", cfg_false, 0),
	CFG_BOOL("change_usb_version", cfg_false, 0),
	CFG_INT("usb_version", 0, 0),
	CFG_INT("max_power", 0, 0),
	CFG_STR("manufacturer", "Acme Inc.", 0),
	CFG_STR("product", "USB Serial Converter", 0),
	CFG_STR("serial", "08-15", 0),
	CFG_STR("filename", "", 0),
	CFG_END()
    };
    cfg_t *cfg;
    
    /*
	normal variables
    */ 
    int	_read = 0, _erase = 0, _flash = 0;
    unsigned char eeprom_buf[128];
    char *filename;
    int size_check;
    int i, argc_filename;
    FILE *fp;

    struct ftdi_context ftdi;
    struct ftdi_eeprom eeprom;

    printf("\nFTDI eeprom generator v%s\n", VERSION);
    printf ("(c) Intra2net AG <opensource@intra2net.com>\n");
    
    if (argc != 2 && argc != 3) {
	printf("Syntax: %s [commands] config-file\n", argv[0]);
	printf("Valid commands:\n");
	printf("--read-eeprom		Read eeprom and write to -filename- from config-file\n");
	printf("--erase-eeprom		Erase eeprom\n");
	printf("--flash-eeprom		Flash eeprom\n");
	exit (-1);
    }
    
    if (argc == 3) {
	if (strcmp(argv[1], "--read-eeprom") == 0)
	    _read = 1;
	if (strcmp(argv[1], "--erase-eeprom") == 0)
	    _erase = 1;
	if (strcmp(argv[1], "--flash-eeprom") == 0)
	    _flash = 1;
	
	argc_filename = 2;
    } else {
	argc_filename = 1;
    }

    if ((fp = fopen(argv[argc_filename], "r")) == NULL) {
	printf ("Can't open configuration file\n");
	exit (-1);
    }
    fclose (fp);

    cfg = cfg_init(opts, 0);
    cfg_parse(cfg, argv[argc_filename]);
    filename = cfg_getstr(cfg, "filename");

    if (cfg_getbool(cfg, "self_powered") && cfg_getint(cfg, "max_power") > 0)
	printf("Hint: Self powered devices should have a max_power setting of 0.\n");

    ftdi_eeprom_initdefaults (&eeprom);

    eeprom.vendor_id = cfg_getint(cfg, "vendor_id");
    eeprom.product_id = cfg_getint(cfg, "product_id");
    eeprom.BM_type_chip = cfg_getbool(cfg, "BM_type_chip");

    eeprom.self_powered = cfg_getbool(cfg, "self_powered");
    eeprom.remote_wakeup = cfg_getbool(cfg, "remote_wakeup");
    eeprom.max_power = cfg_getint(cfg, "max_power");

    eeprom.in_is_isochronous = cfg_getbool(cfg, "in_is_isochronous");
    eeprom.out_is_isochronous = cfg_getbool(cfg, "out_is_isochronous");
    eeprom.suspend_pull_downs = cfg_getbool(cfg, "suspend_pull_downs");

    eeprom.use_serial = cfg_getbool(cfg, "use_serial");
    eeprom.change_usb_version = cfg_getbool(cfg, "change_usb_version");
    eeprom.usb_version = cfg_getint(cfg, "usb_version");


    eeprom.manufacturer = cfg_getstr(cfg, "manufacturer");
    eeprom.product = cfg_getstr(cfg, "product");
    eeprom.serial = cfg_getstr(cfg, "serial");

    if (_read > 0 || _erase > 0 || _flash > 0) {
        printf("FTDI init: %d\n", ftdi_init(&ftdi));
	i = ftdi_usb_open(&ftdi, eeprom.vendor_id, eeprom.product_id);
	
	if (i != 0) {
	    printf("Unable to find FTDI devices under given vendor/product id: 0x%X/0x%X\n", eeprom.vendor_id, eeprom.product_id);
	    printf("Retrying with default FTDI id.\n");

    	    i = ftdi_usb_open(&ftdi, 0x0403, 0x6001);
	    if (i != 0) {
		printf("Error: %s\n", ftdi.error_str);
		exit (-1);
	    }
	}
    }

    if (_read > 0) {
        printf("FTDI read eeprom: %d\n", ftdi_read_eeprom(&ftdi, (char *)&eeprom_buf));
	if (filename != NULL && strlen(filename) > 0) {
	    FILE *fp = fopen (filename, "wb");
	    fwrite (&eeprom_buf, 1, 128, fp);
	    fclose (fp);
	} else {
	    printf("Warning: Not writing eeprom, you must supply a valid filename\n");
	}

	goto cleanup;
    }

    if (_erase > 0) {
	printf("FTDI erase eeprom: %d\n", ftdi_erase_eeprom(&ftdi));
    }    

    size_check = ftdi_eeprom_build(&eeprom, eeprom_buf);
    if (size_check == -1) {
	printf ("Sorry, the eeprom can only contain 128 bytes (100 bytes for your strings).\n");
	printf ("You need to short your string by: %d bytes\n", size_check);
	goto cleanup;
    } else {
	printf ("Used eeprom space: %d bytes\n", 128-size_check); 
    }

    if (_flash > 0) {
	printf ("FTDI write eeprom: %d\n", ftdi_write_eeprom(&ftdi, (char *)&eeprom_buf));
    }

    // Write to file?
    if (filename != NULL && strlen(filename) > 0) {
        fp = fopen(filename, "w");
	if (fp == NULL) {
	    printf ("Can't write eeprom file.\n");
	    exit (-1);
	} else 
	    printf ("Writing to file: %s\n", filename);
	
	fwrite(eeprom_buf, 128, 1, fp);
	fclose(fp);
    }

cleanup:
    if (_read > 0 || _erase > 0 || _flash > 0) {
        printf("FTDI close: %d\n", ftdi_usb_close(&ftdi));
    }

    ftdi_deinit (&ftdi);
    
    cfg_free(cfg);
    
    printf("\n");
    return 0;
}
