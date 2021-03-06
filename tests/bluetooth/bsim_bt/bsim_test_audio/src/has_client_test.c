/*
 * Copyright (c) 2022 Codecoup
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_BT_HAS_CLIENT
#include "bluetooth/audio/has.h"

#include "common.h"

extern enum bst_result_t bst_result;

CREATE_FLAG(g_is_connected);
CREATE_FLAG(g_service_discovered);
CREATE_FLAG(g_preset_switched);

static struct bt_conn *g_conn;
static struct bt_has *g_has;

static void discover_cb(struct bt_conn *conn, int err, struct bt_has *has,
			enum bt_has_hearing_aid_type type, enum bt_has_capabilities caps)
{
	if (err) {
		FAIL("Failed to discover HAS (err %d)\n", err);
		return;
	}

	printk("HAS discovered type %d caps %d\n", type, caps);

	g_has = has;
	SET_FLAG(g_service_discovered);
}

static void preset_switch_cb(struct bt_has *has, uint8_t index)
{
	printk("Active preset index %d\n", index);

	SET_FLAG(g_preset_switched);
}

static const struct bt_has_client_cb has_cb = {
	.discover = discover_cb,
	.preset_switch = preset_switch_cb,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err > 0) {
		char addr[BT_ADDR_LE_STR_LEN];

		bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

		FAIL("Failed to connect to %s (err %u)\n", addr, err);
		return;
	}

	g_conn = conn;
	SET_FLAG(g_is_connected);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

static void test_main(void)
{
	int err;

	err = bt_enable(NULL);
	if (err < 0) {
		FAIL("Bluetooth discover failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = bt_has_client_cb_register(&has_cb);
	if (err < 0) {
		FAIL("Failed to register callbacks (err %d)\n", err);
		return;
	}

	err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, device_found);
	if (err < 0) {
		FAIL("Scanning failed to start (err %d)\n", err);
		return;
	}

	printk("Scanning successfully started\n");

	WAIT_FOR_COND(g_is_connected);

	err = bt_has_client_discover(g_conn);
	if (err < 0) {
		FAIL("Failed to discover HAS (err %d)\n", err);
		return;
	}

	WAIT_FOR_COND(g_service_discovered);
	WAIT_FOR_COND(g_preset_switched);

	PASS("HAS main PASS\n");
}

static const struct bst_test_instance test_has[] = {
	{
		.test_id = "has_client",
		.test_post_init_f = test_init,
		.test_tick_f = test_tick,
		.test_main_f = test_main,
	},
	BSTEST_END_MARKER
};

struct bst_test_list *test_has_client_install(struct bst_test_list *tests)
{
	return bst_add_tests(tests, test_has);
}
#else
struct bst_test_list *test_has_client_install(struct bst_test_list *tests)
{
	return tests;
}

#endif /* CONFIG_BT_HAS_CLIENT */
