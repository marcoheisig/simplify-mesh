#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include "Mesh.hpp"

BOOST_AUTO_TEST_CASE(addElements)
{
    Mesh mesh;

    Mesh::VertexIterator vi = vcg::tri::Allocator<Mesh>::AddVertices(mesh, 3);
    Mesh::FaceIterator   fi = vcg::tri::Allocator<Mesh>::AddFaces(mesh, 1);
    Mesh::VertexPointer ivp[4];
    ivp[0]=&*vi; vi->P()=Mesh::CoordType(0.0, 0.0, 0.0); ++vi;
    ivp[1]=&*vi; vi->P()=Mesh::CoordType(1.0, 0.0, 0.0); ++vi;
    ivp[2]=&*vi; vi->P()=Mesh::CoordType(0.0, 1.0, 0.0); ++vi;
    fi->V(0)=ivp[0];
    fi->V(1)=ivp[1];
    fi->V(2)=ivp[2];

    BOOST_CHECK(mesh.VN() == 3);
    BOOST_CHECK(mesh.FN() == 1);
}
