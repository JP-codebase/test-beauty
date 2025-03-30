#include "nexpar_functions.h"

// nexpar
// It returns true if the program reaches its end (`true` == exeuction
// finished`)

// Pointer arithmetic version
bool nexpar_ptr(unsigned int *partition, unsigned int partition_size) {

  // Find active position
  if (partition[0] == 1)
    return true;

  unsigned int active_position;
  unsigned int sum{0};

  for (active_position = 0; active_position < partition_size - 1;
       active_position++) {
    if ((partition[active_position + 1] == 1) ||
        (partition[active_position + 1] == 0))
      break;
    sum += partition[active_position];
  }

  partition[active_position]--;
  sum += partition[active_position];

  unsigned int quotient{(partition_size - sum) / partition[active_position]};
  unsigned int remainder{(partition_size - sum) -
                         quotient * partition[active_position]};

  for (unsigned int i = active_position + 1;
       i < (active_position + 1 + quotient); i++) {
    partition[i] = partition[active_position];
  }

  if (remainder != 0) {
    partition[active_position + 1 + quotient] = remainder;

    for (unsigned int i = active_position + 2 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  } else {
    for (unsigned int i = active_position + 1 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  }

  return false;
}

// With indexes
bool nexpar(unsigned int *partition, unsigned int partition_size) {

  // Find active position
  if (partition[0] == 1)
    return true;

  unsigned int active_position;
  unsigned int sum{0};

  for (active_position = 0; active_position < partition_size - 1;
       active_position++) {
    if ((partition[active_position + 1] == 1) ||
        (partition[active_position + 1] == 0))
      break;
    sum += partition[active_position];
  }

  partition[active_position]--;
  sum += partition[active_position];

  unsigned int quotient{(partition_size - sum) / partition[active_position]};
  unsigned int remainder{(partition_size - sum) -
                         quotient * partition[active_position]};

  for (unsigned int i = active_position + 1;
       i < (active_position + 1 + quotient); i++) {
    partition[i] = partition[active_position];
  }

  if (remainder != 0) {
    partition[active_position + 1 + quotient] = remainder;

    for (unsigned int i = active_position + 2 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  } else {
    for (unsigned int i = active_position + 1 + quotient; i < partition_size;
         i++) {
      partition[i] = 0;
    }
  }

  return false;
}
