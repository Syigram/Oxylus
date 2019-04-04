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

#include <oxylus/configuration/config.h>
#include <oxylus/memory/mem_info.h>
#include <oxylus/memory/file_reader.h>
#include <oxylus/mpi_message/image_allocation_message.h>
#include <oxylus/mpi_message/memory_message.h>
#include <oxylus/memory/helper.h>
#define MPI_MASTER 0
namespace mpi = boost::mpi;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

typedef std::vector<ImageAllocationMessage> ImageAllocVect;
typedef std::vector<MemoryMessage> MemAllocVect;



ImageAllocVect ExtractMemoryMessageInfo(MemAllocVect messages){
  ImageAllocVect imageAllocVector;
  MemAllocVect::iterator it;
  int counter = 0;
  int i = 0;
  for(it = messages.begin(); it != messages.end(); it++, i++){
    ImageAllocationMessage imageAllocMsg;
    int batchSize = it->GetMemAvailable() / IMAGE_AVG_SIZE;
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


  FileReader reader;
  reader.ReadImages(1, 1001);

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
