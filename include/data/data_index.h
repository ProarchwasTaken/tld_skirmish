// data/data_index.h
#pragma once
#include <string>


struct IndexData {
  std::string subject_name;
  std::string subject_class;
  std::string subject_status;
  std::string description;

  float desc_progress = 0;
};
