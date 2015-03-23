/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "field.h"

Field::Field()
{
    Init();
}

Field::~Field()
{
    CleanUp();
}

Field::Field(const Field &other)
{
    Init();
    if (other.raw)
        SetByteValue(other.value, other.length, other.type, other.length);
    else
        SetStructuredValue((char*)other.value, other.type);
}

Field &Field::operator = (const Field &other)
{
    if (this == &other)
        return *this; 

    Init();
    if (other.raw)
        SetByteValue(other.value, other.length, other.type, other.length);
    else
        SetStructuredValue((char*)other.value, other.type);

    return *this;

}


void Field::SetByteValue(const void* newValue, const size_t newSize, enum_field_types newType, uint32 length)
{
    if (value)
        CleanUp();

    // This value stores raw bytes that have to be explicitly cast later
    if (newValue)
    {
        value = new char[newSize];
        memcpy(value, newValue, newSize);
        length = length;
    }
    type = newType;
    raw = true;
}

void Field::SetStructuredValue(char* newValue, enum_field_types newType)
{
    if (value)
        CleanUp();

    // This value stores somewhat structured data that needs function style casting
    if (newValue)
    {
        size_t size = strlen(newValue);
        value = new char [size+1];
        strcpy((char*)value, newValue);
        length = size;
    }

    type = newType;
    raw = false;
}

