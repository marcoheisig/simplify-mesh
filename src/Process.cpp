/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#include "Process.hpp"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/clean.h>
#include <mpi.h>
#include "StaticScheduler.hpp"
#include "Mesh.hpp"
#include "Version.hpp"

using namespace std;

Process::Process(int *argc, char ***argv) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    namespace po = boost::program_options;
    po::options_description desc("options");
    desc.add_options()
        ("help", "produce help message")
        ("version", "display version information")
        ("verbose,v", "emit detailed log messages")
        ("size,s",
         po::value<int>()->default_value(5000),
         "set target number of faces")
        ("merge-tolerance,m",
         po::value<float>()->default_value(1.0e-7f),
         "tolerance for merging mesh boundaries")
        ("input,i",
         po::value<std::vector<std::string> >(),
         "input files")
        ("output,o",
         po::value<std::string>(),
         "output file");

    po::positional_options_description p;
    p.add("input", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(*argc, *argv).
              options(desc).positional(p).run(), vm);
    po::notify(vm);


    if (vm.count("help")) {
        if(rank == 0) {
            std::cout << "simplify-mesh " << version << std::endl;
            std::cout << desc << std::endl;
        }
        throw StopProgram();
    }

    if (vm.count("version")) {
        if(rank == 0) {
            std::cout << "simplify-mesh " << version << std::endl;
        }

        throw StopProgram();
    }

    if (vm.count("verbose")) {
        logging = true;
    } else {
        logging = false;
    }

    if(vm.count("output")) {
        outfile = vm["output"].as<std::string>();
    } else {
        throw std::runtime_error("no output file specified");
    }

    if(vm.count("input")) {
        infiles = vm["input" ].as<std::vector<std::string> >();
    } else {
        throw std::runtime_error("no input files specified");
    }

    if (infiles.size() == 1) {
        if(rank==0) std::cout << "converting from file " << infiles[0]
                              << " to "                  << outfile
                              << "." << std::endl;
    } else {
        if(rank==0) std::cout << "Merging " << infiles.size() << " files into "
                              << outfile << "." << std::endl;
    }

    if (vm.count("size")) {
        if(rank==0) std::cout << "target mesh size was set to "
                              << vm["size"].as<int>() << " faces.\n";
        this->target_faces = vm["size"].as<int>();
    } else {
        throw std::runtime_error("no target number of faces specified");
    };

    if (vm.count("merge-tolerance")) {
        this->merge_tolerance = vm["merge-tolerance"].as<float>();
    } else {
        throw std::runtime_error("no target number of faces specified");
    };
}

void Process::run() {
    Scheduler* scheduler = new StaticScheduler(infiles, num_procs);
    Mesh mesh;
    bool alive = true;
    int iteration = 0;
    while(alive) {
        Task task = scheduler->getTask(rank, iteration++,  mesh);
        switch( task.type ) {
        case TASK_RECEIVE:
            {
                Mesh recvMesh;
                recvMesh.recv( task.receive.mpi_rank, task.receive.mpi_tag );
                mesh.merge(recvMesh, merge_tolerance);
                vcg::tri::UpdateBounding<Mesh>::Box(mesh);
                vcg::tri::UpdateTopology<Mesh>::FaceFace(mesh);
                vcg::tri::UpdateTopology<Mesh>::VertexFace(mesh);
                vcg::tri::UpdateFlags<Mesh>::FaceBorderFromFF(mesh);
                vcg::tri::UpdateFlags<Mesh>::FaceBorderFromVF(mesh);
                vcg::tri::UpdateFlags<Mesh>::VertexBorderFromFace(mesh);
                mesh.simplify( this->target_faces, true);
                if(logging) printf("%2d: received %d faces from rank %d\n",
                                   rank, recvMesh.FN(), task.receive.mpi_rank);
            }
            break;
        case TASK_SEND:
            {
                vcg::tri::Allocator<Mesh>::CompactFaceVector(mesh);
                vcg::tri::Allocator<Mesh>::CompactVertexVector(mesh);
                mesh.send( task.send.mpi_rank, task.send.mpi_tag );
                if(logging) printf("%2d: sent %d faces to rank %d\n",
                                   rank, mesh.FN(), task.send.mpi_rank);
            }
            break;
        case TASK_READ:
            {
                for( const auto& filename : task.read.filenames) {
                    Mesh new_mesh;
                    new_mesh.readFileOBJ( filename.c_str() );
                    mesh.merge(new_mesh, merge_tolerance);
                    vcg::tri::UpdateBounding<Mesh>::Box(mesh);
                    vcg::tri::UpdateTopology<Mesh>::FaceFace(mesh);
                    vcg::tri::UpdateTopology<Mesh>::VertexFace(mesh);
                    vcg::tri::UpdateFlags<Mesh>::FaceBorderFromFF(mesh);
                    vcg::tri::UpdateFlags<Mesh>::FaceBorderFromVF(mesh);
                    vcg::tri::UpdateFlags<Mesh>::VertexBorderFromFace(mesh);
                    vcg::tri::Clean<Mesh>::RemoveNonManifoldVertex(mesh);
                    vcg::tri::Clean<Mesh>::RemoveNonManifoldFace(mesh);
                    mesh.simplify( this->target_faces, true);
                    if(logging) printf("%2d: read %d faces from %s\n",
                                       rank, new_mesh.FN(), filename.c_str());
                }
            }
            break;
        case TASK_DIE:
            {
                alive = false;
                if(logging) printf("%2d: finished\n", rank);
            }
            break;
        case TASK_IDLE:
            {
                if(logging) printf("%2d: idle\n", rank);
                boost::this_thread::sleep(boost::posix_time::milliseconds(20));
                break;
            }
        case TASK_WRITE:
            mesh.simplify( this->target_faces, false);
            mesh.writeFileOBJ( outfile.c_str() );

            if(logging) printf("%2d: wrote %d faces to %s\n",
                               rank, mesh.FN(), outfile.c_str());

            alive = false;
            if(logging) printf("%2d: finished\n", rank);
            break;
        default:
            throw std::runtime_error("invalid task type");
            break;
        }
    }
}

