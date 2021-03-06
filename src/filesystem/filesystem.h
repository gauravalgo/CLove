/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

void filesystem_init(void);

int filesystem_read(char const* filename, char** output);
int filesystem_write(const char* name, const char* data);
int filesystem_append(const char* name, const char* data);
int filesystem_exists(const char* name);
int filesystem_remove(const char* name);
