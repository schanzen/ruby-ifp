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

#include <ruby.h>
#include <usb.h>
#include <ifp.h>
#include <rbifp.h>

VALUE rb_ifp_version(VALUE self);

VALUE rb_ifp_capacity(VALUE self);

VALUE rb_ifp_free_space(VALUE self);

VALUE rb_ifp_firmware_version(VALUE self);

VALUE rb_ifp_battery_state(VALUE self);

VALUE rb_ifp_player_model(VALUE self);

VALUE rb_ifp_rename(VALUE self, VALUE old_path, VALUE new_path);

VALUE rb_ifp_remove(VALUE self, VALUE file);

static int rb_ifp_list_cb(void * cont, int type,  const char * name, int size);

VALUE rb_ifp_list_dirs(VALUE self, VALUE dir);

VALUE rb_ifp_download_file(VALUE self, VALUE remotefile, VALUE localfile);

VALUE rb_ifp_upload_file(VALUE self, VALUE remotefile, VALUE localfile);

VALUE rb_ifp_download_dir(VALUE self, VALUE remotedir, VALUE localdir);

VALUE rb_ifp_upload_dir(VALUE self, VALUE remotedir, VALUE localdir);

VALUE rb_ifp_mkdir(VALUE self, VALUE dir);

VALUE rb_ifp_rmdir(VALUE self, VALUE dir);

VALUE rb_ifp_is_file(VALUE self, VALUE f);

VALUE rb_ifp_is_dir(VALUE self, VALUE d);

VALUE rb_ifp_exists(VALUE self, VALUE f);

VALUE rb_ifp_treewalk_dirs(VALUE self, VALUE dir);

VALUE rb_ifp_get_station(VALUE self, VALUE n);

VALUE rb_ifp_set_station(VALUE self, VALUE n, VALUE callsign, VALUE freq_val);
