
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

#include <rbifp_helper_funcs.h>

/* I copied those helper functions over from one of the examples
 * in the libifp package (ifpline) */

void slash2backslash(char *dest, const char *src, size_t count)
{
	
	for (; *src != '\0' && count > 0; count--)
	{
		
		if (*src == '/')
		{
			
			*dest++ = '\\'; src++;
			
		} else {
			
			*dest++ = *src++;
			
		}
		
	}
	
	*dest = '\0';

}

void remove_double_slash(char *path, size_t size)
{
	
	int nocopy = 0;
	int i;
	char *pos;
	
	for (i = 0, pos = path; i < (int)(size-1) && *path != '\0'; i++)
	{
		
		if (*path == '/')
		{
			
			if (nocopy == 0)
			{
				
				*pos++ = *path++;
				nocopy = 1;
				
			} else {
				
				path++;
				
			}
			
		} else {
			
			*pos++ = *path++;
			nocopy = 0;
			
		}
		
	}
	
	*pos = '\0';
	
}

void normalize_path(char *path, size_t size)
{
	
	char *pos;
	
	/* /path/to//file/// -> /path/to/file/ */
	remove_double_slash(path, size);
	
	/* /path/to/file/ -> /path/to/file */
	for (pos = path; *pos != '\0'; pos++);
		
		if (*(pos-1) == '/' && pos - 1 != path)
		{
			
			*(pos-1) = '\0';
			
		}
		
}
