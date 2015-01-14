/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IO
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "config.hpp"
#include "StaticScheduler.hpp"

using namespace std;

char* argv[] = {(char*)"./scheduler", nullptr};
int argc = 1;

BOOST_AUTO_TEST_CASE(SCHEDULER) {
  for (int proc_count = 2; proc_count < 1024; proc_count += 2) {
    for (int file_count : { proc_count/2, proc_count, (int)(proc_count * 1.137),
                (int)(proc_count * 1.737), (int)(proc_count * 2.0),
                (int)(proc_count * 11.2)}) {
      vector<string> files(file_count);
      int n = 0;
      generate_n(begin(files), file_count, [&n]() { return to_string(n++); });
      vector<vector<string>> proc_files(proc_count);

      unique_ptr<Scheduler> s(new StaticScheduler(files, proc_count));

      Mesh dummy_mesh;

      int iteration = 0;

      bool alive = true;
      while (alive) {
        alive = false;
        for (int i = 0; i < (int)proc_files.size(); i++) {
          Task task = s->getTask(i, iteration, dummy_mesh);
          switch (task.type) {
            case TASK_READ:
              proc_files[i].insert(begin(proc_files[i]),
                                   begin(task.read.filenames),
                                   end(task.read.filenames));
              //                cout << "read: "  << i << " "
              //     << task.read.filenames.size() << "\n";
              alive = true;
              break;
            case TASK_RECEIVE: {
              Task other_task =
                  s->getTask(task.receive.mpi_rank, iteration, dummy_mesh);
              //  cout << "receive: " << i << "<-"
              //    << task.receive.mpi_rank
              //     << " (" << task.receive.mpi_tag << ")\n";

              // cout << "send: " << task.receive.mpi_rank << "->"
              //     << other_task.send.mpi_rank
              //     << " (" << other_task.send.mpi_tag << ")\n";

              if (other_task.type == TASK_SEND &&
                  other_task.send.mpi_rank == i &&
                  other_task.send.mpi_tag == task.receive.mpi_tag) {
                //                cout << "Match\n";
                proc_files[i].insert(begin(proc_files[i]),
                                     begin(proc_files[task.receive.mpi_rank]),
                                     end(proc_files[task.receive.mpi_rank]));
              }
            }
              alive = true;
              break;
            case TASK_SEND:
              alive = true;
              break;
            case TASK_IDLE:
              alive = true;
              break;
            case TASK_DIE:
              break;
            case TASK_WRITE:
              BOOST_CHECK(i == 0);
              std::sort(begin(files), end(files));
              std::sort(begin(proc_files[i]), end(proc_files[i]));
              BOOST_CHECK(std::equal(begin(proc_files[i]), end(proc_files[i]),
                                     begin(files)));
              break;
          }
        }
        iteration++;
      }
    }
  }
}
