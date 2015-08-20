/*
 * HID++ 2.0 library - headers file.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Based on the HID++ 2.0 documentation provided by Nestor Lopez Casado at:
 *   https://drive.google.com/folderview?id=0BxbRzx7vEV7eWmgwazJ3NUFfQ28&usp=sharing
 */

#ifndef HIDPP_20_H
#define HIDPP_20_H

#include "hidpp10.h"

extern const char *hidpp_errors[0xFF];

struct _hidpp20_message {
	uint8_t report_id;
	uint8_t device_idx;
	uint8_t sub_id;
	uint8_t address;
	uint8_t parameters[LONG_MESSAGE_LENGTH - 4U];
} __attribute__((packed));

union hidpp20_message {
	struct _hidpp20_message msg;
	uint8_t data[LONG_MESSAGE_LENGTH];
};

int hidpp20_request_command(struct ratbag_device *device, union hidpp20_message *msg);

/* -------------------------------------------------------------------------- */
/* 0x0000: Root                                                               */
/* -------------------------------------------------------------------------- */

#define HIDPP_PAGE_ROOT					0x0000

int hidpp_root_get_feature(struct ratbag_device *device,
			   uint16_t feature,
			   uint8_t *feature_index,
			   uint8_t *feature_type,
			   uint8_t *feature_version);
int hidpp20_root_get_protocol_version(struct ratbag_device *device,
				      unsigned *major,
				      unsigned *minor);
/* -------------------------------------------------------------------------- */
/* 0x0001: Feature Set                                                        */
/* -------------------------------------------------------------------------- */

#define HIDPP_PAGE_FEATURE_SET				0x0001

struct hidpp20_feature {
	uint16_t feature;
	uint8_t type;
};

/**
 * allocates a list of features that has to be freed by the caller.
 *
 * returns the elements in the list or a negative error
 */
int hidpp20_feature_set_get(struct ratbag_device *device,
			    struct hidpp20_feature **feature_list);

/* -------------------------------------------------------------------------- */
/* 0x2200: Mouse Pointer Basic Optical Sensors                                */
/* -------------------------------------------------------------------------- */

#define HIDPP_PAGE_MOUSE_POINTER_BASIC			0x2200

#define HIDDP20_MOUSE_POINTER_FLAGS_VERTICAL_TUNING	(1 << 4)
#define HIDDP20_MOUSE_POINTER_FLAGS_OS_BALLISTICS	(1 << 3)

#define HIDDP20_MOUSE_POINTER_FLAGS_ACCELERATION_MASK	0x03
#define HIDDP20_MOUSE_POINTER_ACCELERATION_NONE		0x00
#define HIDDP20_MOUSE_POINTER_ACCELERATION_LOW		0x01
#define HIDDP20_MOUSE_POINTER_ACCELERATION_MEDIUM	0x02
#define HIDDP20_MOUSE_POINTER_ACCELERATION_HIGH		0x03

int hidpp20_mousepointer_get_mousepointer_info(struct ratbag_device *device,
					       uint16_t *resolution,
					       uint8_t *flags);

/* -------------------------------------------------------------------------- */
/* 0x2201: Adjustable DPI                                                     */
/* -------------------------------------------------------------------------- */

#define HIDPP_PAGE_ADJUSTABLE_DPI			0x2201

/**
 * either dpi_steps is not null or the values are stored in the null terminated
 * array dpi_list.
 */
struct hidpp20_sensor {
	uint8_t index;
	uint16_t dpi;
	uint16_t dpi_min;
	uint16_t dpi_max;
	uint16_t dpi_steps;
	uint16_t default_dpi;
	uint16_t dpi_list[LONG_MESSAGE_LENGTH / 2 + 1];
};

/**
 * allocates a list of sensors that has to be freed by the caller.
 *
 * returns the elements in the list or a negative error
 */
int hidpp20_adjustable_dpi_get_sensors(struct ratbag_device *device,
				       struct hidpp20_sensor **sensors_list);

/* -------------------------------------------------------------------------- */
/* 0x1b04: Special keys and mouse buttons                                     */
/* -------------------------------------------------------------------------- */

#define HIDPP_PAGE_SPECIAL_KEYS_BUTTONS			0x1b04

struct hidpp20_control_id {
	uint8_t index;
	uint16_t control_id;
	uint16_t task_id;
	uint8_t flags;
	uint8_t position;
	uint8_t group;
	uint8_t group_mask;
	uint8_t raw_XY;
	struct {
		uint8_t raw_XY;
		uint8_t persist;
		uint8_t divert;
		uint16_t remapped;
	} reporting;
};

struct hidpp20_1b04_mapping {
	uint16_t value;
	const char *name;
};

/**
 * allocates a list of controls that has to be freed by the caller.
 *
 * returns the elements in the list or a negative error
 */
int hidpp20_special_key_mouse_get_controls(struct ratbag_device *device,
					   struct hidpp20_control_id **controls_list);

const char *hidpp20_1b04_get_logical_mapping(uint16_t value);
const char *hidpp20_1b04_get_physical_mapping(uint16_t value);

#endif /* HIDPP_20_H */
