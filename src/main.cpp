#define BOOST_LOG_DYN_LINK 1

#include <boost/mpi.hpp>
#include <boost/mpi/datatype.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

#include <oxylus/configuration/configuration_constants.h>
#include <oxylus/image/image_structure.h>
#include <oxylus/memory/mem_info.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/memory/helper.h>
#include <oxylus/configuration/configuration_object.h>

#define MPI_MASTER 0

namespace mpi = boost::mpi;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

typedef std::vector<ImageAllocationMessage> ImageAllocVect;
typedef std::vector<MemoryMessage> MemAllocVect;

using namespace std;


ImageAllocVect ExtractMemoryMessageInfo(MemAllocVect messages){
  ImageAllocVect imageAllocVector;
  MemAllocVect::iterator it;
  int counter = 0;
  int i = 0;
  for(it = messages.begin(); it != messages.end(); it++, i++){
    ImageAllocationMessage imageAllocMsg;
    int batchSize = it->GetMemAvailable() / rdf::bpc::constants::IMAGE_AVG_SIZE;
    int start = counter;
    int end = counter + batchSize;
    imageAllocMsg.SetBatchSize(batchSize);
    imageAllocMsg.SetIndexStart(start);
    imageAllocMsg.SetIndexEnd(end);
    imageAllocMsg.SetIdProcess(i);
    counter = end + 1;
    imageAllocMsg.Print();
    imageAllocVector.push_back(imageAllocMsg);
  }
  return imageAllocVector;
}

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
  ImageAllocationMessage imageAllocMsg(1, 20, 40, 20);

  int indexStart = imageAllocMsg.GetIndexStart();
  int indexEnd = imageAllocMsg.GetIndexEnd();

  rdf::ConfigurationObject config;

  cout << "rows: " << config.GetImagesRows() << endl;
  cout << "cols: " << config.GetImagesCols() << endl;
  cout << "images: " << config.GetImagesNumber() << endl;

  MemInfo info;


  rdf::bpc::FileReader::ReadImageTest("testl.png");

  rdf::bpc::FileReader::ReadImageTest("testd.png");

  rdf::bpc::ImageStructure first_struct(&config);

  /* int get_size_of_first = first_struct.GetSizeOf(); */


  /* cout << "(GetSizeOf) ImageStructure: " << get_size_of_first << endl; */
  /* cout << "Total space: " << mem << endl; */
  /* int mem = info.GetMemAvailable(); */
  /* int total =  mem / get_size_of_first; */

  /* cout << "This means I have space for: " << total << " images" << endl; */
  rdf::bpc::FileReader reader(&config);
  reader.ReadImages(indexStart, indexEnd);

  /* ImageOperations::RandomPointsSelection(200,200,1000); */



  /* std::string base = "Data_"; */
  /* Helper help; */
  /* help.ImageFileCounter(base, 1,5000); */


  // std::cout << "END: " <<  rank << '\n';
  // ImageAllocator alloc;
  // MemInfo
  // int total = alloc.GetMaxBatchSize();

  // std::string val = memInfo.GetMemoryValue(TOTAL);
  // std::string val = exec("cat /proc/meminfo | grep MemAvailable");
  // std::cout << "values: " << val  << '\n';
  return 0;
}


void first_gather_and_scatter_working(){

  mpi::environment env;
  mpi::communicator world;

  int rank = world.rank();
  int worldSize = world.size();

  MemoryMessage memMessage(rank);
  memMessage.Print();
  std::vector<MemoryMessage> memoryMessageVector;
  ImageAllocationMessage imageAllocScatter;

  mpi::gather<MemoryMessage>(world, memMessage, memoryMessageVector, 0);
  ImageAllocVect imageAllocVector = ExtractMemoryMessageInfo(memoryMessageVector);
  mpi::scatter(world, imageAllocVector, imageAllocScatter, 0);
  std::cout << "---------------- Scatter begin ----------------" << '\n';
  imageAllocScatter.Print();
}
