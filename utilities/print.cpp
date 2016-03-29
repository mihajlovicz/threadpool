#include "print.h"

/*
  Initialize static member std::mutex Print::m to avoid linker error LNK2005
*/

std::mutex Print::m;