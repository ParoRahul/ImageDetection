#include "DBModule/include/DBModule.hpp"
#include <boost/timer/timer.hpp>

int main(int argc, char** argv) {
	if (argc != 2)
	{
        	LOGEROR("INVALID INPUT ARGUMENT KINDLY ENTRE LOAD OR SAVE AS ARGUMENT");
        	return EXIT_FAILURE;
	}
	//boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
	int64 t0 = cv::getTickCount();
	std::cout <<argv[0] << ": GOING TO " << argv[1]<< " DB OBJECTS \n";
	if (std::string(argv[1]) == "SAVE" ){
	   DBModule Db("SAVE");
	   //Db.createDBojects();
	   std::cout << "DB OBJECTS CREATION DONE\n";
	}
	else if (std::string(argv[1]) == "LOAD") {
		DBModule Db("LOAD");
	}
	else {
		std::cout << "INVALID OPTION \n";
	}
	//boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time();
	//boost::posix_time::time_duration diff = t2 - t1;
	//std::cout << "Total time Elapsed  " << diff.total_milliseconds() << std::endl;
	std::cout << "Total Computation Time ..." << ((cv::getTickCount() - t0) / cv::getTickFrequency()) << " sec" << std::endl;
	return 0;
}
