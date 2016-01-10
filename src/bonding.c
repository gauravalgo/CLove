/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "bonding.h"

bonding_Version const * bonding_getVersion(void) {
  static const bonding_Version version = {
    .major = 0,
    .minor = 2,
    .revision = 0,
    .codename = "HappyGiraffe"
  };

  return &version;
}
