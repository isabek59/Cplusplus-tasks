#pragma once

int* LowerBound(int* first, int* last, int value) {
  if (*first >= value) {
    return first;
  }
  while (first < last) {
    int* mid = first + (last - first) / 2;
    if (*mid < value) {
      first = mid + 1;
    } else {
      last = mid;
    }
  }
  return last;
}