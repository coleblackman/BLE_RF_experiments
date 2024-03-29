/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)
{
	char src_addr[18];

	// Convert address to typical MAC address format.
	bt_addr_le_to_str(addr, src_addr, 18);	

	if (rssi < 70) {
		if (strcmp("F4:EA:75:EF:C4:62", src_addr) == 0) {
				printk("[BLE ADV] src: %s (rssi: %i)\n", src_addr, rssi);
			}
	}

	// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/scan.html

}

int main(void)
{
	struct bt_le_scan_param scan_param = {
		.type       = BT_HCI_LE_SCAN_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = 0x0010,
		.window     = 0x0010,
	};
	int err;

	printf("Starting Scanner\n");

	// Initialize the Bluetooth Subsystem
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return -1;
	}

	printk("Bluetooth initialized\n");

	// Cole
	static int mfg_data[] = { 0xff, 0xff, 0x00 };
	static const struct bt_data ad[] = {BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3)};
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Cole's Module Failed");
	}

	err = bt_le_scan_start(&scan_param, scan_cb);
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return -1;
	}
}
