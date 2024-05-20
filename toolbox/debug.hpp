#pragma once

#if defined(TOOLBOX_TEST) || defined(TOOLBOX_DEVELOPMENT)

#include <fstream>
#include <filesystem>

using namespace std::filesystem;

struct debug {
 private:
  inline static path file_path = std::string(__FILE__);
  inline static path project_dir = file_path.parent_path().parent_path();
  inline static std::string output_path =
      project_dir.append("debug.info").string();

 public:
  inline static std::ofstream out = std::ofstream(output_path);
};

#endif