// you need to modify this file

#include "scheduler.h"
#include "common.h"
#include <algorithm>
#include <iostream>
#include <list>


using namespace std;





void print_process(vector<Process> & p) {
    cout << "\n PRINTING PROCESS OF SIZE: " << sizeof(p) << endl;
    for (auto i : p) {
        cout << "Process ID: " << i.id << endl;
        cout << "Arrival T: " << i.arrival_time << endl;
        cout << "Burst: " << i.burst << endl;
    }
}

// this is the function you should edit
//
// runs Round-Robin scheduling simulator
// input:
//   quantum = time slice
//   max_seq_len = maximum length of the reported executing sequence
//   processes[] = list of process with populated IDs, arrival_times, and bursts
// output:
//   seq[] - will contain the execution sequence but trimmed to max_seq_len size
//         - idle CPU will be denoted by -1
//         - other entries will be from processes[].id
//         - sequence will be compressed, i.e. no repeated consecutive numbers
//   processes[]
//         - adjust finish_time and start_time for each process
//         - do not adjust other fields
//
void simulate_rr(
    int64_t quantum, 
    int64_t max_seq_len,
    std::vector<Process> & processes,
    std::vector<int> & seq
) {
    // print_process(processes);
    // Copy of processes to track remaining burst
    vector<Process> processes_copy = processes;

    // print_process(processes_copy);

    seq.clear();
    vector<pair<int64_t, int>> arrvial_times;

    // for (int i = 0; i < sizeof(processes_copy); i++) {
    //     // arrvial_times[i].first = -processes_copy[i].arrival_time;
    //     // arrvial_times[i].second = i;
    //     arrvial_times.emplace_back(-processes_copy[i].arrival_time, i);
    // }
    
    int arrival_count = 0;
    for (auto i : processes_copy) {
        arrvial_times.emplace_back(i.arrival_time, arrival_count);
        arrival_count++;
    }

    sort(arrvial_times.begin(), arrvial_times.end());
    reverse(arrvial_times.begin(), arrvial_times.end());

    cout << "\nArray Times vector of size " << arrvial_times.size() << endl;
    for (auto i : arrvial_times) {
        // cout << "Index: " << i.second << endl;
        // cout << "ATime: " << i.first << endl;

        printf("[ %ld / %d ]\n", i.first, i.second);
    }



    int64_t current_time = 0; // Current cpu time
    int64_t quantum_used = 0;
    int jobs_remaining = processes.size(); // Number of processes left


    // Ready queue, the first process from processes will be put in ready queue
    vector<int> ready_queue;

    // Current state of cpu
    // -1 means IDLE
    // in_cpu >= 0 means process ID on the cpu
    int in_cpu = -1;
    int64_t next_arrival_time = -1;
    while(true) {
        // cout << "\nWE HERE AT THE START" << endl;
        // cout << "Nxt arrival time: " << next_arrival_time << endl;
        // // cout << "CPU: " << in_cpu << endl;
        // // cout << "CTime: " << current_time << endl;
        // cout << "Q_Used: " << quantum_used << endl;
        // // cout << "RQ Size: " << ready_queue.size() << endl;
        // cout << "Jobs Rem: " << jobs_remaining << endl;

        // printf("Ready Queue [%ld]: [ ", ready_queue.size());
        // for (auto i : ready_queue) {
        //     printf("%d, ", i);
        // }
        // printf(" ]\n");

        if (in_cpu >= 0) printf("\nT%ld CPU[%d/%ld]\n", current_time, processes_copy[in_cpu].id, processes_copy[in_cpu].burst);
        else printf("\nT%ld CPU[%d/-1]\n", current_time, in_cpu);

        // Check for loop end condition
        if (jobs_remaining == 0) break;

        // Check to see when the next job will arrive
        if (!arrvial_times.empty()) {
            next_arrival_time = arrvial_times.back().first;
        }

        
        // If the cpu is not idle
        // Check if the current job is finished
        // If so, update finish type
        // If RQ has jobs, grab the front job and erase from RQ
        // Otherwise set cpu to idle
        // Reset quantum count incase job finishes before time slice ends
        // Decrement jobs left and go to next loop iteration
        if (in_cpu > -1) {
            // cout << "\nWE HERE" << endl;
            if (processes_copy[in_cpu].burst == 0) {
                // TODO: Mark task done
                processes[in_cpu].finish_time = current_time;
                // Remove task from ready queue
                // for (int i = 0; i < ready_queue.size(); i++) {
                //     if (ready_queue[i] == in_cpu) {
                //         cout << "WTF WHY DOES THIS WORK?" << endl;
                //         ready_queue.erase(ready_queue.begin() + i);
                //         break;
                //     }
                // }
                // cout << "Job Done ID: " << processes[in_cpu].id << endl;
                // cout << "Job Done: " << in_cpu << endl; 
                
                if (!ready_queue.empty()) {
                    in_cpu = ready_queue.front();
                    ready_queue.erase(ready_queue.begin());
                } else in_cpu = -1;
                
                // printf("Ready Queue [%ld]: [ ", ready_queue.size());
                // for (auto i : ready_queue) {
                //     printf("%d, ", i);
                // }
                // printf(" ]\n");

                
                quantum_used = 0;
                jobs_remaining--;
                continue;
            }

            if (quantum_used == quantum) {
                if (processes_copy[in_cpu].burst > 0) ready_queue.emplace_back(in_cpu);
                in_cpu = -1;
                quantum_used = 0;
            }
        } 
        



        // If cpu idle and there are jobs in rdy queue, move P from RQ to CPU
        if (in_cpu == -1 && !ready_queue.empty()) {
            in_cpu = ready_queue.front();
            ready_queue.erase(ready_queue.begin());
            // cout << "Cont after RQ -> CPU when cpu idle" << endl;
            continue;
        }

        // cout << "WE HERE 1" << endl;

        // Process Arrivals
        // for (int i = 0; i < sizeof(processes_copy); i ++) {
        //     // If current time is when a process is supposed to arrive
        //     // Add it to ready queue
        //     if (current_time == processes_copy[i].arrival_time && in_ready_queue[i] == false) {
        //         ready_queue.push_back(processes_copy[i]);
        //         in_ready_queue[i] = true;
        //     }
        // }
        // if (current_time == next_arrival_time) {
        //     ready_queue.emplace_back(processes_copy[arrvial_times.back().second].id, processes_copy[arrvial_times.back().second].burst);
        //     arrvial_times.pop_back();
        // }
        
        // Process Arrivals
        // Check if current time is same as next arrival time
        // If so, put it on ready queue
        // Remove the arrival time from arrival_times vector
        if (current_time == next_arrival_time && !arrvial_times.empty()) {
            ready_queue.emplace_back(processes_copy[arrvial_times.back().second].id);
            // ready_queue.emplace_back(arrvial_times.back().second);
            arrvial_times.pop_back();
            cout << "PROCESSING ARRIVAL: " << endl;
            printf("Ready Queue [%ld]: [ ", ready_queue.size());
            for (auto i : ready_queue) {
                printf("%d, ", i);
            }
            printf(" ]\n");

            // cout << "Cont after arrival" << endl;

            continue;
        }



        
        if (seq.size() < (unsigned)max_seq_len) {
            if (seq.empty()) {
                seq.push_back(in_cpu);
            } else if (seq.back() != in_cpu) seq.push_back(in_cpu);
        }
        
        // TIMESKIP CODE for processes with VERY LARGE arrival times:
        /*
        First scenario: 
            RQ is empty
            There are jobs left
            cpu is idle

            meaning nothing has arrived so we time skip to 1 before the arrival time

        Second scenario:
            RQ is empty
            There are jobs left
            cpu is NOT idle

            meaning something is currently executing on CPU with a possible long burst time
                - Scenario 2.1: This is the last task. nothing else will arrive in which case do the whole burst in a time skip
                - Scenario 2.2: This is not the last task. Something will arrive but it will arrive after the burst for the current task finishes
                                Therefore, do the whole burst in a time skip
                - Scenario 2.3: This is not the last task. Something will arrive and it will arrive in between the burst for the current task
                                Therefore, cannot do the whole burst in one time skip
                                TODO:
        
        Third scenario: TODO:
            RQ is not empty
            There are jobs left
            CPU is not idle

            There are tasks being executed with possibly very large CPU bursts so incrementing time by 1 is not optimal
                - Scenario 3.1: All remaining tasks are on the ready queue. Nothing new will arrive. In which case
                                3.1.1:  Increment time by quantum IF it does not exceed the tasks burst. If it does increment by (quantum - burst - 1)
                - Scenario 3.2: All remaining tasks are not on the ready queue. There will be arrivals.
                                Will be same as 3.1.1 but must consider not going over the arrival window.
        */
        if (ready_queue.size() == 0 && jobs_remaining > 0) {
            if (in_cpu == -1 && (next_arrival_time - current_time) > 1) { // First Scenario
                current_time = next_arrival_time - 1;
                continue;
            }

            if (in_cpu > -1 && arrvial_times.empty()) { // Scenario 2.1
                cout << "LAST TASK: " << in_cpu << " " << processes_copy[in_cpu].burst << endl;
                current_time += processes_copy[in_cpu].burst;
                processes_copy[in_cpu].burst = 0;
                continue;
            }


            if (in_cpu > -1 && (next_arrival_time - current_time) > processes_copy[in_cpu].burst) { // Scenario 2.2
                current_time += processes_copy[in_cpu].burst;
                processes_copy[in_cpu].burst = 0;
                continue;
            }

            if (in_cpu > -1 && ((current_time + processes_copy[in_cpu].burst) > next_arrival_time)) { // Scenario 2.3
                int64_t quantum_multiple = (next_arrival_time - current_time)/quantum;
                if (quantum_multiple > 0) {
                    current_time += quantum_multiple * quantum;
                    processes_copy[in_cpu].burst -= (quantum_multiple * quantum);
                    quantum_used = quantum;
                    continue;
                }

            }
            
        }

        // if (ready_queue.size() > 0 && in_cpu > -1) {
        if (in_cpu > -1) {
            // Check if process has a start time, if not set it to current time
            if (processes[in_cpu].start_time == -1) processes[in_cpu].start_time = current_time;

            // If remaning burst is less than quantum
            if (processes_copy[in_cpu].burst < quantum) {
                current_time += processes_copy[in_cpu].burst;
                processes_copy[in_cpu].burst = 0;
                continue;
            }

            // If the next task arrives after current time + quantum 
            if ((current_time + quantum) < next_arrival_time) {
                current_time += quantum;
                quantum_used = quantum;
                processes_copy[in_cpu].burst -= quantum;
                continue;
            }

            // if ((current_time + quantum) >= next_arrival_time) {
            //     current_time += (next_arrival_time - current_time);
            //     quantum_used = (next_arrival_time - current_time);
            //     processes_copy[in_cpu].burst -=(next_arrival_time - current_time);
            //     continue;
            // }

            
        }

        if (in_cpu > -1) {
            if (processes[in_cpu].start_time == -1) processes[in_cpu].start_time = current_time;
            processes_copy[in_cpu].burst--;
            quantum_used++;
        }







        current_time++;
        // cout << "WE HERE AT THE END" << endl;
    }
}