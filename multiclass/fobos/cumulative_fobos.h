#ifndef CLASSIFIER_FOBOS_CUMULATIVE_FOBOS_H_
#define CLASSIFIER_FOBOS_CUMULATIVE_FOBOS_H_

#include <iostream>
#include <vector>

#include "../../utility/calc.h"

namespace classifier {
namespace fobos {
class CumulativeFOBOS {
 public:
  CumulativeFOBOS(double eta = 1.0,
                  double lambda = 1.0);
  ~CumulativeFOBOS() {};

  void Train(const datum& datum, bool truncate = true);
  void Train(const std::vector<datum>& data,
             const size_t iteration = 1);
  void Test(const feature_vector& fv, std::string* predict) const;
  void GetFeatureWeight(size_t feature_id,
                        std::vector<std::pair<std::string, double> >* results) const;

 private:
  void Truncate(const feature_vector& fv);

  void TruncateAll();

  void Update(const datum& datum,
              const score2class& scores);

  void CalcScores(const feature_vector& fv,
                  score2class* scores) const;

  weight_matrix weight_;
  weight_matrix prev_truncate_;

  size_t dataN_;
  double eta_;
  double lambda_;
  double truncate_sum_;
};

} //namespace
} //namespace

#endif //CLASSIFIER_FOBOS_CUMULATIVE_FOBOS_H_
