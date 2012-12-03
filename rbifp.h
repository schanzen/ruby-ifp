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

#define RBIFP_MAJOR_VERSION rb_int_new(0)
#define RBIFP_MINOR_VERSION rb_int_new(1)
#define RBIFP_MICRO_VERSION rb_int_new(1)

VALUE rb_ifp_player_init(VALUE self);

static void rb_release_ifp(void *dh);

VALUE rb_ifp_player_new(VALUE klass);

extern void Init_rbifp();
