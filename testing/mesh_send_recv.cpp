/* Copyright (C) Marco Heisig, Dominik Ernst 2014 - GNU GPLv3 or later */
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IO
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "config.hpp"
#include <mpi.h>
#include "Mesh.hpp"


using namespace std;

char * argv[] = { (char*) "./mesh_send_recv", nullptr };
int argc = 1;

BOOST_AUTO_TEST_CASE( SEND_RECV )
{

	char** argv_dummy = argv;
	MPI_Init(&argc,  &argv_dummy );

	int rank;
	int size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	BOOST_CHECK( size>1 );

	cout << rank << "/" <<  size << "\n";

    Mesh mesh1;
	mesh1.readFileOBJ( triangle_obj );
	int size1 = 0;
	void *mem1 = NULL;
	mesh1.dump(&size1, &mem1);

	if(rank == 0) {
		mesh1.MPI_Send(1, 0);

	} else if (rank == 1) {
		Mesh mesh2;

		mesh2.MPI_Recv( 0, 0);

		int size2 = 0;
		void *mem2 = NULL;
		mesh2.dump(&size2, &mem2);
		BOOST_CHECK(size1 == size2);
		BOOST_CHECK(mesh1.VN() == mesh2.VN());
		BOOST_CHECK(mesh1.FN() == mesh2.FN());

		auto f_iter1 = mesh1.face.begin();
		auto f_iter2 = mesh2.face.begin();
		while( f_iter1 != mesh1.face.end() && f_iter2 != mesh2.face.end() ) {
			Face& f1 = *f_iter1;
			Face& f2 = *f_iter2;

			BOOST_CHECK(f1.VN() == f2.VN());
			if(f1.VN() == f2.VN()) {
				for(int i = 0; i < f1.VN(); i++) {
					int vertexId1 = vcg::tri::Index(mesh1, f1.V(i));
					int vertexId2 = vcg::tri::Index(mesh2, f2.V(i));
					BOOST_CHECK(vertexId1 == vertexId2);
				}
			}
			++f_iter1;
			++f_iter2;
		}
		BOOST_CHECK(f_iter1 == mesh1.face.end());
		BOOST_CHECK(f_iter2 == mesh2.face.end());
	}

	MPI_Finalize();
}
