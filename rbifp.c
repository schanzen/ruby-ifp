/* Ruby-iFP. Ruby bindings for the libifp library.
 * Copyright (C) 2005 Martin Schanzenbach
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <rbifp.h>
#include <rbifp_funcs.h>

static VALUE iFP, pClass;

//init function
VALUE rb_ifp_player_init(VALUE self)
{
	
	return self;
	
}

//Cleaning up.
static void rb_release_ifp(void *ifpdev)
{
	
	struct ifp_device *ddev;
	ddev = (struct ifp_device*) ifpdev;
	ifp_finalize(ddev);
	
}

//Initialize IFP::Player class.
VALUE rb_ifp_player_new(VALUE klass)
{
	
	VALUE self;
	
	//Most of this is just copied over from libifp example code.
	struct usb_device *dev = NULL;
	usb_dev_handle *dh;
	struct ifp_device *ifpdev;
	int i=0;
	
	ifpdev = ALLOC(struct ifp_device);
	
	usb_init();
	
	dh = ifp_find_device();
	if (dh == NULL) {
		
		rb_raise(rb_eInterrupt, "iRiver iFP device not found.");
		
	}
	
	dev = usb_device(dh);
	
	/* "must be called" written in the libusb documentation */
	if (usb_claim_interface(dh, dev->config->interface->altsetting->bInterfaceNumber)) {
		
		rb_raise(rb_eInterrupt, "Device is busy.");
		usb_close(dh);
		
	}
	
	i = ifp_init(ifpdev, dh);
	if (i) {
		
		rb_raise(rb_eInterrupt, "Device not responding.");
		
	}
	
	//This is binding code.
	self = Data_Wrap_Struct(klass, 0, rb_release_ifp, ifpdev);
	rb_obj_call_init(self, 0, NULL);
	
	return self;
	
}

//Building iFP module and Player class.
extern void Init_ifp()
{
	
	iFP = rb_define_module("IFP");
	
	pClass = rb_define_class_under(iFP, "Player", rb_cObject);
	
	//IFP module functions.
	rb_define_module_function(iFP, "version", rb_ifp_version, 0);
	
	//Singleton methods.
	rb_define_singleton_method(pClass, "new", rb_ifp_player_new, 0);
	rb_define_singleton_method(pClass, "initialize", rb_ifp_player_init, 0);
	
	//Module constants.
	rb_define_const(iFP, "MAX_PATHLEN", rb_int_new(IFP_MAXPATHLEN));
	rb_define_const(iFP, "MAX_NAMELEN", rb_int_new(IFP_MAX_FILENAME));
	rb_define_const(iFP, "FM_STATIONS", rb_int_new(IFP_PRESET_TOTAL));
	rb_define_const(iFP, "MAX_FREQ", rb_int_new(IFP_FREQ_MAX));
	rb_define_const(iFP, "MIN_FREQ", rb_int_new(IFP_FREQ_MIN));
	
	//IFP::Player methods.
	rb_define_method(pClass, "capacity", rb_ifp_capacity, 0);
	rb_define_method(pClass, "free_space", rb_ifp_free_space, 0);
	rb_define_method(pClass, "firmver", rb_ifp_firmware_version, 0);
	rb_define_method(pClass, "battery_state", rb_ifp_battery_state, 0);
	rb_define_method(pClass, "model", rb_ifp_player_model, 0);
	rb_define_method(pClass, "rename", rb_ifp_rename, 2);
	rb_define_method(pClass, "remove_file", rb_ifp_remove, 1);
	rb_define_method(pClass, "download_file", rb_ifp_download_file, 2);
	rb_define_method(pClass, "upload_file", rb_ifp_upload_file, 2);
	rb_define_method(pClass, "download_dir", rb_ifp_download_dir, 2);
	rb_define_method(pClass, "upload_dir", rb_ifp_upload_dir, 2);
	rb_define_method(pClass, "mkdir", rb_ifp_mkdir, 1);
	rb_define_method(pClass, "rmdir", rb_ifp_rmdir, 1);
	rb_define_method(pClass, "file?", rb_ifp_is_file, 1);
	rb_define_method(pClass, "directory?", rb_ifp_is_dir, 1);
	rb_define_method(pClass, "exists?", rb_ifp_exists, 1);
	rb_define_method(pClass, "entries", rb_ifp_list_dirs, 1);
	rb_define_method(pClass, "treewalk_dirs", rb_ifp_treewalk_dirs, 1);
	
	//FM methods
	rb_define_method(pClass, "get_station", rb_ifp_get_station, 1);
	rb_define_method(pClass, "set_station", rb_ifp_set_station, 3);
	
	//Aliases.
	rb_define_alias(pClass, "get_capacity", "capacity");
	rb_define_alias(pClass, "get_firmver", "firmver");
	rb_define_alias(pClass, "get_model", "model");
	
}
