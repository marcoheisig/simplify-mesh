#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Simplify
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "config.hpp"
#include "Mesh.hpp"

BOOST_AUTO_TEST_CASE(Simplify)
{
    Mesh mesh;
    mesh.readFileOBJ( triangle_obj );

    mesh.simplify(3);
    std::cout << "VN: " << mesh.VN() << "FN: " << mesh.FN() << std::endl;
    BOOST_CHECK(mesh.VN() == 6);
    BOOST_CHECK(mesh.FN() == 4);

}
