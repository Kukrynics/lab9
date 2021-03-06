#include <boost/program_options.hpp>

#include "ThreadPool.h"
#include "boost/program_options.hpp"
#include "htmlDownloader.hpp"
#include "htmlParser.hpp"
#include "iostream"
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  po::options_description desc("Must have options");
  desc.add_options()
      ("help", "produce help message")
      ("url", po::value<std::string>(), "HTML page address")
      ("network_threads", po::value<int>(), "number of threads to download pages")
      ("parser_threads", po::value<int>(),"number of threads to process pages")
      ("depth",po::value<int>(), "page search depth")
      ("output",po::value<std::string>(),"path to output file")
      ;
  po::variables_map vm; //в вм будет ключ аргумент
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
 //получаю значение аргументов по ключам словаря (--    это ключ)
  outPut output = outPut(vm["output"].as<std::string>()); //хранится путь к файлу
  htmlParser parser = htmlParser(output, vm["parser_threads"].as<int>());
  htmlDownloader downloader =
      htmlDownloader(parser, vm["network_threads"].as<int>());

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  url url = {vm["url"].as<std::string>(), "/"};

  downloader.startDownload({url}, vm["depth"].as<int>()); //запуск краулера
  int tmp; //чтобы программа не заканчивалась
  std::cin >> tmp;
}