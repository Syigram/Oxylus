/**
 * Copyright (C) 2018
 * SIPLab Tecnológico de Costa Rica
 *
 * Project: Oxylus Framework
 *
 * Check the LICENCE.txt file for further information
 *
 * \author: Pablo Alvarado
 * \date:   2018-08-31
 */

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <exception>
#include <cstdlib>
#include <complex>
#include <sstream>

/**
 * Unit tests for the matrix class
 */

#include <oxylus/training/matrix.h>

#include <oxylus/configuration/Serialization.h>
#include <boost/serialization/complex.hpp>

// Explicit instantiation of all methods of Matrix
typedef std::complex<double> dcomplex;


template class rdf::Matrix<dcomplex>;
template class rdf::Matrix<double  >;
template class rdf::Matrix<float   >;
template class rdf::Matrix<int     >;

typedef rdf::Matrix<dcomplex> cmatrix;
typedef rdf::Matrix<double  > dmatrix;
typedef rdf::Matrix<float   > fmatrix;
typedef rdf::Matrix<int     > imatrix;

#if 1
# define dispatchTest(func) \
  func<cmatrix>();          \
  func<dmatrix>();          \
  func<fmatrix>();          \
  func<imatrix>();
#else
# define dispatchTest(func) func<dmatrix>();
#endif


BOOST_AUTO_TEST_SUITE( Matrix )

template<class M>
void testConstructors() {
  // Constructors
  { // default
    M a;
    BOOST_CHECK( a.rows() == 0);
    BOOST_CHECK( a.cols() == 0);
  }
  { // unitilialized
    M a(2,3,rdf::DoNotInitialize);
    BOOST_CHECK( a.rows() == 2);
    BOOST_CHECK( a.cols() == 3);
  }
  { // default initialized
    M a(3,2);
    BOOST_CHECK( a.rows() == 3);
    BOOST_CHECK( a.cols() == 2);
    BOOST_CHECK( a(0,0) == typename M::value_type(0));
  }
  { // default initialized
    M a(3,2,typename M::value_type(4));
    BOOST_CHECK( a.rows() == 3);
    BOOST_CHECK( a.cols() == 2);
    BOOST_CHECK( a(0,0) == typename M::value_type(4));
  }
  { // initializer_list
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    BOOST_CHECK( a.rows() == 3);
    BOOST_CHECK( a.cols() == 5);

    BOOST_CHECK( a(0,0) == typename M::value_type(1));
    BOOST_CHECK( a(1,2) == typename M::value_type(8));
    BOOST_CHECK( a(2,3) == typename M::value_type(14));
  }
  { // Copy constructor
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    M b(a);

    BOOST_CHECK( a==b );
    BOOST_CHECK( b.rows() == 3 );
    BOOST_CHECK( b.cols() == 5 );
    BOOST_CHECK( b.data() != a.data());
  }

  { // Move constructor
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    M b(std::move(a));

    BOOST_CHECK( b.rows() == 3 );
    BOOST_CHECK( b.cols() == 5 );

    BOOST_CHECK( a.empty() );
  }
  { // Mem constructor
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    M b(a.rows(),a.cols(),a.data());

    BOOST_CHECK( a==b );
    BOOST_CHECK( b.rows() == 3 );
    BOOST_CHECK( b.cols() == 5 );
    BOOST_CHECK( b.data() != a.data() );
  }
}


/**
 * Instantiate and test the methods of the Matrix class
 */
BOOST_AUTO_TEST_CASE( Constructors ) {
  dispatchTest(testConstructors);
}

template<class M>
void testComparison() {
  // == and !=
  M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
  M b = { {1,2,3,4,5},{6,7,9,9,10},{11,12,13,14,15} };

  BOOST_CHECK( (a!=b) );

  b(1,2)=typename M::value_type(8);

  BOOST_CHECK( (a==b) );
}

BOOST_AUTO_TEST_CASE(Comparison)
{
  dispatchTest(testComparison);
}

template<class M>
void testAssignment() {
  { // Move assignment
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    M c(a);
    M b;
    b=std::move(a);
    BOOST_CHECK(a.empty() );
    BOOST_CHECK(!b.empty() );
    BOOST_CHECK(b.rows()==3 );
    BOOST_CHECK(b.cols()==5 );
    BOOST_CHECK(b==c );
  }
  { // assignment
    M a = { {1,2,3,4,5},{5,6,7,8,9},{9,10,11,12,13} };
    M b;
    b=a;
    BOOST_CHECK( a==b );
  }
  { // assignment with another allocator type
    typedef typename M::value_type type;
    rdf::Matrix<type> a = { {1, 2, 3, 4, 5},
                             {5, 6, 7, 8, 9},
                             {9,10,11,12,13} };
    M b;
    b=a;

    BOOST_CHECK(a.rows()==b.rows() );
    BOOST_CHECK(a.cols()==b.cols() );
    const std::size_t bytes = a.cols()*sizeof(type);
    for (size_t i=0;i<a.rows();++i) {
      BOOST_CHECK(memcmp(a[i],b[i],bytes)==0);
    }
  }
  { // swap
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    M b = { {13,14},{15,16} };

    M c(a);
    M d(b);

    BOOST_CHECK( a==c );
    BOOST_CHECK( d==b );

    c.swap(d);
    BOOST_CHECK( a==d );
    BOOST_CHECK( b==c );
  }
  { // column
    M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
    std::vector<typename M::value_type> col = a.column(1);
    std::vector<typename M::value_type> ref = {2,7,12};
    BOOST_CHECK( col == ref );
  }
}

BOOST_AUTO_TEST_CASE(Assignment)
{
  dispatchTest(testAssignment);
}


template<class M>
void testSerialization()
{
  std::stringstream ss;
  boost::archive::xml_oarchive xoarc(ss);

  M a = { {1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15} };
  xoarc << boost::serialization::make_nvp("matrix",a);

  boost::archive::xml_iarchive xiarc(ss);
  M b;
  xiarc >> boost::serialization::make_nvp("matrix",b);

  BOOST_CHECK( a == b );
}

BOOST_AUTO_TEST_CASE(Serialization)
{
  dispatchTest(testSerialization);
  //testSerialization<dmatrix>();
}



BOOST_AUTO_TEST_SUITE_END()
