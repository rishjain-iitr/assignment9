#include<iostream>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<memory>
#include"page_replacement_algos.h"

using namespace std;

// Length of the page read sequences.
const int kSequenceLength = 50;
// The number of sequnces that will be used for determining the average page
// fault rate for a (num_pages, num_frames) combination.
const int kNumSequencesPerStep = 100;
// The different values of num_frames  parameter that will be used.
const int kValuesNumFrames[] = {1, 2, 3, 4, 5, 6, 10, 20};
// The different values of num_pages parameter that will be used.
const int kValuesNumPages[] = {6, 10, 15, 30};


void run_one_step(const int num_pages, const int num_frames,
    float fault_count[], float run_time_ns[]) {
  // Initialize page fault counts.
  int total_faults_counter = 0, total_faults_stack = 0, total_faults_aging= 0, 
      total_faults_clock = 0;
  float run_time_counter = 0, run_time_stack = 0, run_time_aging= 0, 
      run_time_clock = 0;

  for (int seq_no = 1; seq_no <= kNumSequencesPerStep; ++seq_no) {
    // Generate sequence.
    vector<int> sequence(kSequenceLength);
    for (int i = 0; i < kSequenceLength; ++i) {
      sequence[i] = rand()%num_pages + 1;
    }
    int fault_count, avg_run_time;
    ReadSequenceLRUCounter(sequence, num_frames, fault_count, avg_run_time);
    total_faults_counter += fault_count;
    run_time_counter += avg_run_time;
    ReadSequenceLRUStack(sequence, num_frames, fault_count, avg_run_time);
    total_faults_stack += fault_count;
    run_time_stack += avg_run_time;
    ReadSequenceLRUAging(sequence, num_frames, fault_count, avg_run_time);
    total_faults_aging += fault_count;
    run_time_aging += avg_run_time;
    ReadSequenceLRUClock(sequence, num_frames, fault_count, avg_run_time);
    total_faults_clock += fault_count;
    run_time_clock += avg_run_time;
  }

  fault_count[0] = total_faults_counter / float(kNumSequencesPerStep);
  fault_count[1] = total_faults_stack / float(kNumSequencesPerStep);
  fault_count[2] = total_faults_aging / float(kNumSequencesPerStep);
  fault_count[3] = total_faults_clock / float(kNumSequencesPerStep);

  run_time_ns[0] = run_time_counter / float(kNumSequencesPerStep);
  run_time_ns[1] = run_time_stack / float(kNumSequencesPerStep);
  run_time_ns[2] = run_time_aging / float(kNumSequencesPerStep);
  run_time_ns[3] = run_time_clock / float(kNumSequencesPerStep);
}


void run_tester() {
  ofstream fault_count_file("result_fault_count.csv");
  ofstream run_time_file("result_avg_run_time.csv");
  // Save the header.
  fault_count_file<<"num_pages,num_frames,lru_counter,lru_stack,"
                         <<"lru_aging,lru_clock\n";
  run_time_file<<"num_pages,num_frames,lru_counter,lru_stack,"
                         <<"lru_aging,lru_clock\n";

  // Get the number of page number values and frame number values for which
  // the test is to be done.
  const int num_page_values = 
      sizeof(kValuesNumPages)/sizeof(kValuesNumPages[0]);
  const int num_frame_values = 
      sizeof(kValuesNumFrames)/sizeof(kValuesNumFrames[0]);

  for (int i = 0; i < num_page_values; ++i ) {
    for (int j = 0; j < num_frame_values; ++j) {
      int num_pages = kValuesNumPages[i];
      int num_frames = kValuesNumFrames[j];

      float fault_count[4];
      float run_time_ns[4];

      run_one_step(num_pages, num_frames, fault_count, run_time_ns);

      // Save results.
      fault_count_file<<num_pages<<','<<num_frames<<','<<fault_count[0]<<','
                      <<fault_count[1]<<','<<fault_count[2]<<','
                      <<fault_count[3]<<endl;
      run_time_file<<num_pages<<','<<num_frames<<','<<run_time_ns[0]<<','
                    <<run_time_ns[1]<<','<<run_time_ns[2]<<','
                    <<run_time_ns[3]<<endl;
    }
  }
}

void run_test_corner_case() {
  int num_pages = 6, num_frames = 4;
  // Test for direct sequence.
  cout<<"\nCorner case: Direct sequence\n";
  cout<<"num_pages = "<<num_pages<<" num_frames = "<<num_frames<<endl;
  // Generate the sequence.
  vector<int> sequence;
  for (int i = 0; i<=kSequenceLength ; ++i) {
    sequence.push_back(i%num_pages + 1);
  }
  int fault_count, avg_run_time_ns;
  ReadSequenceLRUCounter(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Counter: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUStack(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Stack: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUAging(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Aging: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUClock(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Clock: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;

  // Test for direct inverted sequence.
  cout<<"\nCorner case: Direct inverted sequence\n";
  cout<<"num_pages = "<<num_pages<<" num_frames = "<<num_frames<<endl;
  // Generate the sequence.
  sequence.clear();
  int page_num = 1;
  bool increment_flag = true;
  for (int i = 0; i<=kSequenceLength ; ++i) {
    sequence.push_back(page_num);
    if (page_num == num_pages) {
      increment_flag = false;
    } 
    if (page_num == 1) {
      increment_flag = true;
    }
    page_num += increment_flag ? 1  : -1;
  }
  ReadSequenceLRUCounter(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Counter: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUStack(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Stack: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUAging(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Aging: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
  ReadSequenceLRUClock(sequence, num_frames, fault_count, avg_run_time_ns); 
  cout<<"LRU Clock: fault_count = "<<fault_count<<", avg_run_time_ns = "
      << avg_run_time_ns<<endl;
}


int main() {
  srand(time(0));

  // run_tester();
  
  run_test_corner_case();
  return 0;
}
