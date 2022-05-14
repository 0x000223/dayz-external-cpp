#ifndef CONFIG_H
#define CONFIG_H

#include <chrono>

/*
 * Rendering Flags
 */
namespace config {
	extern bool show_menu;

	/* World Tables */
	extern bool table_entity_near;
	extern bool table_entity_far;
	extern bool table_entity_slow;
	extern bool table_item;
	extern bool table_bullet;

	extern bool entity_skeleton;
	extern bool entity_name;

	extern bool item_inventoryitem;
	extern bool item_weapons;
	extern bool item_clothing;
	extern bool item_magazines;
	extern bool item_optics;
}

#endif