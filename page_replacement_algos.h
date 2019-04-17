#ifndef __PAGE_REPLACEMENT_ALGOS__
#define __PAGE_REPLACEMENT_ALGOS__

// A bare minimum implementation of common page replacement algorithms just to test their page fault rate.

#include<vector>
// Implements LRU counter implementation.
void ReadSequenceLRUCounter(const std::vector<int>& sequence,
    const int num_frames, int& fault_count, int& avg_run_time_ns);

// Implements LRU stack implementation.
void ReadSequenceLRUStack(const std::vector<int>& sequence,
    const int num_frames, int& fault_count, int& avg_run_time_ns);

// Implements LRU Aging register implementation.
void ReadSequenceLRUAging(const std::vector<int>& sequence,
    const int num_frames, int& fault_count, int& avg_run_time_ns);

// Implements approximate LRU clock (second chance) algorithm.
void ReadSequenceLRUClock(const std::vector<int>& sequence,
    const int num_frames, int& fault_count, int& avg_run_time_ns);


#endif  // __PAGE_REPLACEMENT_ALGOS__
