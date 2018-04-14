/*
 iolock.c - i/o locking
  
 Author:	Paul Barker
 Part of:	COS
 Created:	06/11/04
 Last Modified:	06/11/04

 Copyright (C) 2004 Paul Barker
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

                     (See file "Copying")
*/

#include <cosbase.h>

/* Lock an i/o object for reading */
void iolock_reader(iolock_t* i)
{
	cli();
	if (i->wlock)
		wait(i->rq);
	i->rlock++;
	sti();
}

void iounlock_reader(iolock_t* i)
{
	cli();
	if (!--(i->rlock))
		wake_one(i->wq);
	sti();
}

void iolock_writer(iolock_t* i)
{
	cli();
	if (i->rlock || i->wlock)
		wait(i->wq);
	i->wlock++;
	sti();
}

void iounlock_writer(iolock_t* i)
{
	cli();
	i->wlock--;
	if (!wake_all(i->rq))
		wake_one(i->wq);
	sti();
}
