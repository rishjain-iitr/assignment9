# Page-replacement-Assignment

Different implementations of the Least Recently Used (LRU) page replacement algorithm. The implementations of interest for this assignmenst are:
Counter based, Stack based, Ageing register base and also the Approximate version of LRU: Second chance algorithm.

### Implementation details
Counter based : Instead of storing the clock at the time of accessing, a global counter is kept, and at each page access, this counter is updated and its value is stored with the page that is being accessed.

Stack based: Since addition of elements had to be done from one end and deletion from the other end, the STL list was used for this implementation.

Aging register method: An uint variable was associated with each page to represent a 32-bit aging register. Instead of updating the registers periodically (asynchronously), at the time of each page access, some amount of time (randomly generated) is assumed to have passed since the previous access. From this generated delay time we can calculate how many clock ticks would have happened within this period, and this is used to update the ageing registers before the page access algorithm begins.

Second chance: A normal array is used to store the pages with an associated boolean array to represent its second chance status.

Testing parameters:
- For each run, the number of pages and the number of frames were selected from the set of values {6, 10, 15, 30} and {1, 2, 3, 4, 5, 6, 10, 20} respectively.
- For each combination of (num_pages, num_frames), the average page fault count and the algorithm run time were calculated. A randomly generated sequence of 50 pages were used for one run, and 100 such runs were used to  obtain the average values.

### Results
Run the test_algos.cpp file to generate the results: result_fault_count.csv and result_avg_run_time.csv. To plot the results, run plot_results.py. A sample graph is shown below

![alt-text](/graphs/fault_count_npages_30.png)

### Discussion
Page fault count:
- The page fault rate of Counter and stack implementations were identical, as expected. 
- The aging register results were slightly different because multiple pages could be read within the same tick and this could lead to multiple pages havin the same register value.
- Second chance algorithm also performed nearly as good as the exact implementations.

Algorithm run time:
- Exact implementations require search of the entire page list before replacing one, hence it was expected of them to take more run time. All the exact implementations have atleast a time complexity of O(num_frames) in case of replacements. The second chance algorithm does better because the entire table need no be searched.
- In terms of run time, the second chance algorithms appears to scale better with number of frames.
..
