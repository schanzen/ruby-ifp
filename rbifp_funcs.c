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

#include <rbifp_funcs.h>
#include <rbifp_helper_funcs.h>

//IFP module functions.

//Returns version array.
VALUE rb_ifp_version(VALUE self)
{
	
	VALUE ary = rb_ary_new();
	rb_ary_push(ary, RBIFP_MAJOR_VERSION);
	rb_ary_push(ary, RBIFP_MINOR_VERSION);
	rb_ary_push(ary, RBIFP_MICRO_VERSION);
	return ary;
	
}

//IFP::Player functions.

//Returns capacity.
VALUE rb_ifp_capacity(VALUE self)
{
	
	struct ifp_device *ifpdev;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	return rb_int_new(ifp_capacity(ifpdev));
	
}

//Returns free space.
VALUE rb_ifp_free_space(VALUE self)
{
	
	struct ifp_device *ifpdev;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	return rb_int_new(ifp_freespace(ifpdev));
	
}

//Returns the firmware version.
VALUE rb_ifp_firmware_version(VALUE self)
{
	
	struct ifp_device *ifpdev; 
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	return rb_int_new(ifp_firmware_version(ifpdev));
	
}

//Returns battery condition. This does really not work at all.
//Not just this binding. ifpline returns horrible results too.
VALUE rb_ifp_battery_state(VALUE self)
{
	
	struct ifp_device *ifpdev;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	return rb_int_new(ifp_battery(ifpdev));
	
}

//Returns the iFP model.
VALUE rb_ifp_player_model(VALUE self)
{
	
	struct ifp_device *ifpdev;
	
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	ifp_model(ifpdev, buf, IFP_MAXPATHLEN);
	
	return rb_str_new2(buf);
	
}

//Renames a file on the player.
VALUE rb_ifp_rename(self, old_path, new_path)
    VALUE self, old_path, new_path;
{
	
	struct ifp_device *ifpdev;
	char oldbuf[IFP_MAXPATHLEN];
	char newbuf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);    
	
	slash2backslash(oldbuf, StringValuePtr(old_path), IFP_MAXPATHLEN);
	normalize_path(oldbuf, IFP_MAXPATHLEN);
	
	slash2backslash(newbuf, StringValuePtr(new_path), IFP_MAXPATHLEN);
	normalize_path(newbuf, IFP_MAXPATHLEN);
	
	if (!ifp_rename(ifpdev, oldbuf, newbuf))
	{
		
		return Qtrue;
		
	} else {
		
		return Qfalse;
		
	}
	
}

//Removes file.
VALUE rb_ifp_remove(VALUE self, VALUE file)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(file), IFP_MAXPATHLEN);
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (!ifp_delete(ifpdev, buf)) {
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error removing file!");
		return Qfalse;
		
	}
	
}

//list_dirs callback function.
static int rb_ifp_list_cb(void * cont, int type,  const char * name, int size)
{
	
	if (rb_block_given_p())
 	{
		
		VALUE ary = rb_ary_new();
		rb_ary_push(ary, rb_str_new2(name));
		rb_ary_push(ary, rb_int_new(size));
		
		if (type == IFP_FILE)
		{
			
			rb_ary_push(ary, rb_str_new2("File"));
			
		} else {
			
			rb_ary_push(ary, rb_str_new2("Directory"));
			
		}
		
		rb_yield(ary);
		
	}
	
	return 0;
	
}
 
//Lists entries in dir.
VALUE rb_ifp_list_dirs(VALUE self, VALUE dir)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	void *context = NULL;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(dir), IFP_MAXPATHLEN);
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (!ifp_list_dirs(ifpdev, buf, rb_ifp_list_cb, context)) {
		
		return Qtrue;
		
	} else {
		
		return Qfalse;
		
	}
	
}

//Progress callback function.
static int update_progress(void * context, struct ifp_transfer_status * status)
{
	
	if (rb_block_given_p())
	{
		
		VALUE ary = rb_ary_new();
		rb_ary_push(ary, rb_str_new2(status->file_name));
		rb_ary_push(ary, rb_int_new(status->file_bytes));
		rb_ary_push(ary, rb_int_new(status->file_total));
		rb_ary_push(ary, rb_int_new(status->files_count));
		rb_ary_push(ary, rb_int_new(status->files_total));
		rb_yield(ary);
		
	}
	
	return 0;
	
}

//Downloads file from player.
VALUE rb_ifp_download_file(VALUE self, VALUE remotefile, VALUE localfile)
{
	
	struct ifp_device *ifpdev;
	char remote_buf[IFP_MAXPATHLEN];
	char local_buf[IFP_MAXPATHLEN];
	ifp_progress progress = update_progress;
	void *p_context = NULL;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	strncpy(local_buf, StringValuePtr(localfile), IFP_MAXPATHLEN);
	
	slash2backslash(remote_buf, StringValuePtr(remotefile), IFP_MAXPATHLEN);
	
	normalize_path(remote_buf, IFP_MAXPATHLEN);
	
	if (!ifp_download_file(ifpdev, remote_buf, local_buf, progress, p_context))
	{
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error downloading file!");
		return Qfalse;
		
	}
	
}

//Uploads file to player.
VALUE rb_ifp_upload_file(VALUE self, VALUE localfile, VALUE remotefile)
{
	
	struct ifp_device *ifpdev;
	char remote_buf[IFP_MAXPATHLEN];
	char local_buf[IFP_MAXPATHLEN];
	ifp_progress progress = update_progress;
	void *p_context = NULL;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	strncpy(local_buf, StringValuePtr(localfile), IFP_MAXPATHLEN);
	
	slash2backslash(remote_buf, StringValuePtr(remotefile), IFP_MAXPATHLEN);
	
	normalize_path(remote_buf, IFP_MAXPATHLEN);
	
	if (!ifp_upload_file(ifpdev, local_buf, remote_buf, progress, p_context))
	{
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error uploading file!");
		return Qfalse;
		
	}
	
}

//Downloads dir from player.
VALUE rb_ifp_download_dir(VALUE self, VALUE remotedir, VALUE localdir)
{
	
	struct ifp_device *ifpdev;
	char remote_buf[IFP_MAXPATHLEN];
	char local_buf[IFP_MAXPATHLEN];
	ifp_progress progress = update_progress;
	void *p_context = NULL;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	strncpy(local_buf, StringValuePtr(localdir),  IFP_MAXPATHLEN);
	
	slash2backslash(remote_buf, StringValuePtr(remotedir), IFP_MAXPATHLEN);
	
	normalize_path(remote_buf, IFP_MAXPATHLEN);
	
	if (!ifp_download_dir(ifpdev, remote_buf, local_buf, progress, p_context))
	{
		
		 return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error downloading directory!");
		return Qfalse;
		
	}
	
}

//Uploads dir onto player
VALUE rb_ifp_upload_dir(VALUE self, VALUE remotedir, VALUE localdir)
{
	
	struct ifp_device *ifpdev;
	char remote_buf[IFP_MAXPATHLEN];
	char local_buf[IFP_MAXPATHLEN];
	ifp_progress progress = update_progress;
	void *p_context = NULL;
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	strncpy(local_buf, StringValuePtr(localdir), IFP_MAXPATHLEN);
	
	slash2backslash(remote_buf, StringValuePtr(remotedir), IFP_MAXPATHLEN);
	
	normalize_path(remote_buf, IFP_MAXPATHLEN);
	
	if (!ifp_upload_dir(ifpdev, remote_buf, local_buf, progress, p_context))
	{
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error uploading directory!");
		return Qfalse;
		
	}
	
}

//Creates a directory on the player.
VALUE rb_ifp_mkdir(VALUE self, VALUE dir)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(dir), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (!ifp_mkdir(ifpdev, buf))
	{
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error creating directory!");
		return Qfalse;
		
	}
	
}

//Removes a directory on the player.
VALUE rb_ifp_rmdir(VALUE self, VALUE dir)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(dir), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (!ifp_rmdir(ifpdev, buf))
	{
		
		return Qtrue;
		
	} else {
		
		rb_raise(rb_eIOError, "Error removing directory!");
		return Qfalse;
		
	}
	
}

//Tests if f is a file and exists.
VALUE rb_ifp_is_file(VALUE self, VALUE f)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(f), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (ifp_is_file(ifpdev, buf))
	{
		
		return Qtrue;
		
	} else {
		
		return Qfalse;
		
	}
	
}

//Tests if d is a directory and exists.
VALUE rb_ifp_is_dir(VALUE self, VALUE d)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(d), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (ifp_is_dir(ifpdev, buf))
	{
		
		return Qtrue;
		
	} else {
		
		return Qfalse;
		
	}
	
}

//Tests if f exists.
VALUE rb_ifp_exists(VALUE self, VALUE f)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(f), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	if (!ifp_exists(ifpdev, buf))
	{
		
		return Qfalse;
		
	} else  {
		
		return Qtrue;
		
	}
	
}

//Returns array list of files in directory.
VALUE rb_ifp_treewalk_dirs(VALUE self, VALUE dir)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_MAXPATHLEN];
	struct ifp_treewalk_entry * f = NULL;
	void * d = NULL;
	VALUE ary = rb_ary_new();
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	slash2backslash(buf, StringValuePtr(dir), IFP_MAXPATHLEN);
	
	normalize_path(buf, IFP_MAXPATHLEN);
	
	ifp_treewalk_open(ifpdev, buf, &d);
	
	while ((f = ifp_treewalk_next(d)) != NULL)
	{
		
		rb_ary_push(ary, rb_str_new2(f->path));
		
		if (rb_block_given_p())
		{
			
			VALUE meta_ary = rb_ary_new();
			rb_ary_push(meta_ary, rb_str_new2(f->path));
			rb_ary_push(meta_ary, rb_int_new(f->filesize));
			
			switch (f->type)
			{
				
				case IFP_WALK_FILE:
					rb_ary_push(meta_ary, rb_str_new2("File"));
					rb_yield(meta_ary);
					break;
					
				case IFP_WALK_DIR_PRE:
					rb_ary_push(meta_ary, rb_str_new2("Directory"));
					rb_yield(meta_ary);
					break;
					
				//We don't need the dir twice.
				case IFP_WALK_DIR_POST:
					break;
					
				default:
					rb_ary_push(meta_ary, Qnil);
					
			}
			
		}
		
	}
	
	ifp_treewalk_close(d);

	d = NULL;
	
	return ary;
	
}

//FM tuner methods.

//Get radio station #n.
VALUE rb_ifp_get_station(VALUE self, VALUE n)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_TUNER_PRESET_DATA];
	char callsign[IFP_TUNER_LABEL + 1];
	long station_num = rb_num2int(n);
	int freq;
	
	if (station_num < 0 || station_num > IFP_PRESET_TOTAL)
	{
		
		rb_raise(rb_eIOError, "Stationnumber out of range!");
		
	}
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	ifp_get_tuner_presets(ifpdev, buf, sizeof(buf));
		
	ifp_get_station(station_num, buf, callsign, &freq);
	
	VALUE ary = rb_ary_new();
	
	rb_ary_push(ary, rb_str_new2(callsign));
	rb_ary_push(ary, rb_int_new(freq));

	return ary;

}

//Set radio station #n with label 'callsign' and frequency 'freq_val'.
VALUE rb_ifp_set_station(VALUE self, VALUE n, VALUE callsign, VALUE freq_val)
{
	
	struct ifp_device *ifpdev;
	char buf[IFP_TUNER_PRESET_DATA];
	long station_num = rb_num2int(n);
	long freq = rb_num2int(freq_val);
	
	if (station_num < 0 || station_num >= IFP_PRESET_TOTAL)
	{
		
		rb_raise(rb_eIOError, "Stationnumber out of range!");
		
	}
	
	if (freq == 0 || freq < IFP_FREQ_MIN || freq > IFP_FREQ_MAX)
	{
		
		rb_raise(rb_eIOError, "Frequency out of range!");
		
	}
	
	Data_Get_Struct(self, struct ifp_device, ifpdev);
	
	ifp_get_tuner_presets(ifpdev, buf, sizeof(buf));
	
	ifp_set_station(station_num, buf, StringValuePtr(callsign), freq);
	
	ifp_set_tuner_presets(ifpdev, buf, sizeof(buf));
	
	return Qtrue;
	
}
