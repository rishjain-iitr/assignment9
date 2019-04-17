#include"page_replacement_algos.h"

#include<iostream>
#include<list>
#include<ctime>
#include<chrono>

using namespace std;
using namespace chrono;

//--------------------------- LRU Counter -------------------------------------
void ReadSequenceLRUCounter(const vector<int>& sequence, const int num_frames,
    int& fault_count, int& avg_run_time_ns) {
  // List of pages in memory.
  int* page_list = new int[num_frames];
  // Reference count of corresponding page in page_list.
  int* counter = new int[num_frames];
  // Current size of page_list.
  int cur_list_size = 0;
  // Variable representing the clock, incremented at every page access.
  int clock_time = 0;

  fault_count = 0;
  avg_run_time_ns = 0;
  auto start_time = chrono::high_resolution_clock::now();
  for (int page_num : sequence) {
    // Check if page is already in page_list.
    bool found = false;
    for (int i = 0; i<cur_list_size; ++i) {
      if (page_list[i] == page_num) {
        // Store current time in counter.
        counter[i] = clock_time++;
        found = true;
        break;
      }
    }

    if (found)
      continue;
    // Page fault.
    fault_count++;

    // If free frames are available, put the page there.
    if (cur_list_size < num_frames) {
      page_list[cur_list_size] = page_num;
      counter[cur_list_size] = clock_time++;

      cur_list_size++;
      continue;
    }

    // Replace a page.
    // Find the page with least counter value.
    int min_idx = 0;
    for (int i = 1; i< cur_list_size; ++i) {
      if (counter[i] < counter[min_idx]) {
        min_idx = i;
      }
    }
    page_list[min_idx] = page_num;
    counter[min_idx] = clock_time++;
  }
  auto end_time = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end_time - start_time);
  avg_run_time_ns = duration.count() / float(sequence.size());

  delete[] page_list;
  delete[] counter;
}
//-----------------------------------------------------------------------------

//---------------------------- LRU Stack --------------------------------------
void ReadSequenceLRUStack(const vector<int>& sequence, const int num_frames,
    int& fault_count, int& avg_run_time_ns) {
  // List of pages in memory.
  list<int> page_list;

  fault_count = 0;
  avg_run_time_ns = 0;
  auto start_time = chrono::high_resolution_clock::now();
  for (int page_num : sequence) {
    // Check if page is already in page_list.
    bool found = false;
    for (auto it = page_list.begin(); it != page_list.end(); ++it) {
      if (*it == page_num) {
        // Remove page from the position.
        page_list.erase(it);
        // Add it to the end.
        page_list.push_back(page_num);

        found = true;
        break;
      }
    }
    if (found)
      continue;

    // Page fault.
    fault_count++;

    // If free frames are available, put the page there.
    if (page_list.size() < num_frames) {
      page_list.push_back(page_num);
      continue;
    }

    // Replace the page at the front.
    page_list.pop_front();
    page_list.push_back(page_num);
  }
  auto end_time = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end_time - start_time);
  avg_run_time_ns = duration.count() / float(sequence.size());
}
//-----------------------------------------------------------------------------

//---------------------------- LRU Aging --------------------------------------
void ReadSequenceLRUAging(const vector<int>& sequence, const int num_frames,
    int& fault_count, int& avg_run_time_ns) {
  // List of pages in already in memory.
  int* page_list = new int[num_frames];
  // Array representing aging register of corresponding page in page_list.
  uint* aging_reg = new  uint [num_frames];
  // Current size of page_list.
  int cur_list_size = 0;
  // The update interval of the aging register.
  const int tick_interval = 5;

  fault_count = 0;
  avg_run_time_ns = 0;
  auto start_time = chrono::high_resolution_clock::now();
  for (int page_num : sequence ) {
    // Simulate the delay between page accesses.
    int delay = rand() % 11;
    int tick_count = delay/tick_interval;
    // Update the aging registers.
    for (int i=0; i<cur_list_size; ++i) {
      aging_reg[i]>>=tick_count;
    }

    // Check if page is already in page_list.
    bool found = false;
    for (int i = 0; i<cur_list_size; ++i) {
      if (page_list[i] == page_num) {
        // Set MSB of aging register.
        aging_reg[i] |= 1<<(sizeof(uint)*8 - 1);
        found = true;
        break;
      }
    }
    if (found)
      continue;
    // Page fault.
    fault_count++;

    if (cur_list_size < num_frames) {
      page_list[cur_list_size] = page_num;
      aging_reg[cur_list_size] = 0 | 1<<(sizeof(uint) * 8 - 1);

      cur_list_size++;
      continue;
    }
    // Find the page with minimum aging register value.
    int min_idx = 0;
    for (int i = 1; i<cur_list_size; ++i) {
      if (aging_reg[i] < aging_reg[min_idx]) 
        min_idx = i;
    }
    page_list[min_idx] = page_num;
    aging_reg[min_idx] = 0 | 1<<(sizeof(uint) * 8 - 1);
  }

  auto end_time = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end_time - start_time);
  avg_run_time_ns = duration.count() / float(sequence.size());

  delete[] page_list;
  delete[] aging_reg;
}
//-----------------------------------------------------------------------------

//---------------------------- LRU Clock --------------------------------------
void ReadSequenceLRUClock(const vector<int>& sequence, const int num_frames,
    int& fault_count, int& avg_run_time_ns) {
  // List of pages in memory.
  int* page_list = new int[num_frames];
  // Bool array that stores the second chance status of the corresponding page
  // in page_list.
  bool* has_second_chance = new bool[num_frames];
  // Current size of page_list.
  int cur_list_size = 0;
  // The index in page_list at which the clock_pointer is pointing.
  int clock_pointer = 0;

  fault_count = 0;
  avg_run_time_ns = 0;
  auto start_time = high_resolution_clock::now();
  for (int page_num : sequence) {
    // Search for the page in page_list.
    bool found = false;
    for (int i = 0; i < cur_list_size; ++i) {
      if (page_list[i] == page_num) {
        // Update has_second_chance status.
        has_second_chance[i] = true;
        found = true;
        break;
      }
    }

    if (found)
      continue;
    // Page fault.
    fault_count++;

    // If free frames are available, put the page there.
    if (cur_list_size < num_frames) {
      page_list[cur_list_size] = page_num;
      has_second_chance[cur_list_size] = false;

      cur_list_size++;
      continue;
    }

    // Replace a page.
    while(true) {
      if (!has_second_chance[clock_pointer]) {
        // Replace this page. has_second_chance remains false.
        page_list[clock_pointer] = page_num;

        clock_pointer = (clock_pointer + 1) % num_frames;
        break;
      }

      has_second_chance[clock_pointer] = false;
      clock_pointer = (clock_pointer + 1) % num_frames;
    }
  }
  auto end_time = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end_time - start_time);
  avg_run_time_ns = duration.count() / float(sequence.size());

  delete[] page_list;
  delete[] has_second_chance;
}
//----------------------------------------------------------------------------
