﻿/****************************************************************************
    Qt-LCoV Library Editor for Qt-BSch3V
    Copyright (C) 2002-2005 H.Okada

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
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/


#ifndef _LCOLISTBOXITEM_H
#define _LCOLISTBOXITEM_H

//#include <q3listbox.h>
#include <QVariant>
#include <QListWidgetItem>

class SCompIndex;

class SLCoListBoxItem : public QListWidgetItem
{
public:
    SLCoListBoxItem(QListWidget *parent = 0):QListWidgetItem("test",parent,QListWidgetItem::UserType+1) {
        m_pCompIndex=NULL;
        m_bAlias = false;
    }
    virtual ~SLCoListBoxItem() {
    }

public:
    bool m_bAlias;
    SCompIndex* m_pCompIndex;
public:
    virtual QVariant data(int role);
    virtual QListWidgetItem * clone () const;
};

#endif
