#define BOOST_LOG_DYN_LINK 1

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

#include <boost/mpi.hpp>
#include <boost/mpi/datatype.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/serialization/export.hpp>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/image/image_structure.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/memory/helper.h>
#include <oxylus/training/cell.h>
#include <oxylus/communication/communicator.h>
#include <oxylus/training/features.h>
#include <oxylus/training/matrix.h>
#include <oxylus/training/weak_learner_node.h>
#include <oxylus/training/leaf_node.h>
#include <oxylus/training/tree.h>


#define MPI_MASTER 0

namespace mpi = boost::mpi;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

using namespace std;
using namespace rdf::bpc;

/* BOOST_CLASS_EXPORT(WeakLearnerNode) */
/* BOOST_CLASS_EXPORT(LeafNode) */


void init()
{
    logging::add_file_log(
       keywords::file_name = "sample_%N.log",
       keywords::rotation_size = 10 * 1024 * 1024,
       keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
       /* keywords::format = "[%TimeStamp%]:[%Severity%] %Message%" */
       keywords::format = (
         expr::stream
          << "["
          << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
          << "] | <"
          << logging::trivial::severity
          << "> - "
          << expr::smessage
      )
    );

    /* logging::add_common_attributes(); */
    /* logging::core::get()->set_filter */
    /* ( */
    /*     logging::trivial::severity >= logging::trivial::info */
    /* ); */
}


int main(int argc, char const *argv[]) {


  /* init(); */
  /* logging::add_common_attributes(); */
  /* BOOST_LOG_SEV(info) << "Info test"; */
  /* BOOST_LOG_TRIVIAL(trace) << "Finished run"; */
  /* BOOST_LOG_TRIVIAL(error) << "ERRor test"; */

  rdf::ConfigurationObject* config = new rdf::ConfigurationObject();
  /* FileReader fileReader(config); */
  /* fileReader.DisplayImages(55,1000); */
  rdf::bpc::Communicator comm(config);
  comm.AssignImagesToEachNode();
  comm.LoadImagesToStructures();
  auto start = chrono::high_resolution_clock::now();
  comm.BeginTraining();
  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Rank: " << comm.GetRank() << " -- Elapsed time: " << elapsed.count() << "s" << std::endl;
  cout << "rank: " << comm.GetRank() << "... ended\n";
  if (!comm.GetRank()) {
    std::cout << "No. Processes: " << comm.GetSize() << std::endl;
    config->Print();
  }
  delete config;

  /* WeakLearnerNode* node1 = new WeakLearnerNode(1); */
  /* WeakLearnerNode* node2 = new WeakLearnerNode(2); */
  /* WeakLearnerNode* node3 = new WeakLearnerNode(3); */
  /* WeakLearnerNode* node4 = new WeakLearnerNode(4); */
  /* WeakLearnerNode* node5 = new WeakLearnerNode(5); */
  /* WeakLearnerNode* node6 = new WeakLearnerNode(6); */
  /* WeakLearnerNode* node7 = new WeakLearnerNode(7); */
  /* WeakLearnerNode* node8 = new WeakLearnerNode(8); */
  /* WeakLearnerNode* node9 = new WeakLearnerNode(9); */
  /* WeakLearnerNode* node10 = new WeakLearnerNode(10); */
  /* WeakLearnerNode* node11 = new WeakLearnerNode(11); */
  /* WeakLearnerNode* node12 = new WeakLearnerNode(12); */
  /* WeakLearnerNode* node13 = new WeakLearnerNode(13); */
  /* WeakLearnerNode* node14 = new WeakLearnerNode(14); */

  /* LeafNode* node15 = new LeafNode(15); */
  /* LeafNode* node16 = new LeafNode(16); */
  /* LeafNode* node17 = new LeafNode(17); */
  /* LeafNode* node18 = new LeafNode(18); */
  /* LeafNode* node19 = new LeafNode(19); */
  /* LeafNode* node20 = new LeafNode(20); */
  /* LeafNode* node21 = new LeafNode(21); */
  /* LeafNode* node22 = new LeafNode(22); */
  /* LeafNode* node23 = new LeafNode(23); */
  /* LeafNode* node24 = new LeafNode(24); */
  /* LeafNode* node25 = new LeafNode(25); */

  /* Tree tree; */
  /* tree.Insert(node1); */
  /* tree.Insert(node2); */
  /* tree.Insert(node3); */
  /* tree.Insert(node4); */
  /* tree.Insert(node5); */
  /* tree.Insert(node6); */
  /* tree.Insert(node7); */
  /* tree.Insert(node8); */
  /* tree.Insert(node9); */
  /* tree.Insert(node10); */
  /* tree.Insert(node11); */
  /* tree.Insert(node12); */
  /* tree.Insert(node13); */
  /* tree.Insert(node14); */
  /* tree.Insert(node15); */
  /* tree.Insert(node16); */
  /* tree.Insert(node17); */
  /* tree.Insert(node18); */
  /* tree.Insert(node19); */
  /* tree.Insert(node20); */
  /* tree.Insert(node21); */
  /* tree.Insert(node22); */
  /* tree.Insert(node23); */
  /* tree.Insert(node24); */
  /* tree.Insert(node25); */
  /* tree.PrintTree(tree.GetRoot()); */

    /* std::ofstream ofs("treefile"); */

    /* // create class instance */

    /* // save data to archive */
    /* { */
    /*     boost::archive::text_oarchive oa(ofs); */
    /*     // write class instance to archive */
    /*     /1* oa.register_type<WeakLearnerNode>(); *1/ */
    /*     oa << tree; */
    /* 	// archive and stream closed when destructors are called */
    /* } */

    /* // ... some time later restore the class instance to its orginal state */


    /* Tree tree0; */
    /* Tree tree1; */
    /* Tree tree2; */

    /* tree0.Deserialize("tree_0.dat"); */
    /* tree1.Deserialize("tree_1.dat"); */
    /* tree2.Deserialize("tree_2.dat"); */

    /* std::cout << "Printing tree 0" << std::endl; */
    /* tree0.PrintTree(tree0.GetRoot()); */
    /* std::cout << "Printing tree 1" << std::endl; */
    /* tree1.PrintTree(tree1.GetRoot()); */
    /* std::cout << "Printing tree 2" << std::endl; */
    /* tree2.PrintTree(tree2.GetRoot()); */




    /* { */
    /*     // create and open an archive for input */
    /*     std::ifstream ifs("treefile"); */
    /*     boost::archive::text_iarchive ia(ifs); */
    /*    /1* ia.register_type<WeakLearnerNode>(); *1/ */
    /*     // read class state from archive */
    /*     ia >> newTree; */
    /*     // archive and stream closed when destructors are called */
    /* } */

    /* std::cout << "Printing new tree" << std::endl; */
    /* newTree.PrintTree(newTree.GetRoot()); */


  return 0;
}
