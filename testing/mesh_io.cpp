#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IO
#include <boost/test/unit_test.hpp>
#include <unistd.h>
#include "config.hpp"
#include "Mesh.hpp"

BOOST_AUTO_TEST_CASE(OBJ)
{
    chdir(CMAKE_SOURCE_DIR);
    Mesh mesh;
    mesh.readFileOBJ("testing/triangle.obj");
    BOOST_CHECK(mesh.VN() == 6);
    BOOST_CHECK(mesh.FN() == 4);
}
