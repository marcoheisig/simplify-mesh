#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IO
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "config.hpp"
#include "Mesh.hpp"

BOOST_AUTO_TEST_CASE(OBJ)
{
    Mesh mesh;
    mesh.readFileOBJ( triangle_obj );
    BOOST_CHECK(mesh.VN() == 6);
    BOOST_CHECK(mesh.FN() == 4);
}

BOOST_AUTO_TEST_CASE(DUMP)
{
    Mesh mesh;

    mesh.readFileOBJ( triangle_obj );
    int dump_size = 0;
    void *mem = NULL;
    mesh.dump(&dump_size, &mem);
    BOOST_CHECK(dump_size >= 0);
    BOOST_CHECK(mem != NULL);
}

BOOST_AUTO_TEST_CASE(READ)
{
    Mesh mesh1;
    Mesh mesh2;

    mesh1.readFileOBJ( triangle_obj );
    int size1 = 0;
    void *mem1 = NULL;
    mesh1.dump(&size1, &mem1);
    mesh2.read(mem1);
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
