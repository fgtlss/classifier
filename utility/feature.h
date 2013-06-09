#ifndef CLASSIFIER_TOOL_FEATURE_H_
#define CLASSIFIER_TOOL_FEATURE_H_

#include <limits>
#include <vector>
#include <unordered_map>

namespace classifier {
typedef std::vector<std::pair<size_t, double> > feature_vector;
typedef std::unordered_map<std::string, size_t> feature2id;
struct datum {
  std::string category;
  feature_vector fv;
};

const std::string non_class = "None";
const double non_class_score = std::numeric_limits<double>::min();
} //namespace

#endif //CLASSIFIER_TOOL_FEATURE_H_

