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
