#ifndef CLASSIFIER_SUBGRADIENT_HINGE_H_
#define CLASSIFIER_SUBGRADIENT_HINGE_H_

#include <iostream>
#include <vector>

#include "../../utility/calc.h"

namespace classifier {
namespace subgradient {
class SubgradientHinge {
 public:
  explicit SubgradientHinge(double eta = 1.0);
  ~SubgradientHinge() {};

  void Train(const datum& datum);
  void Train(const std::vector<datum>& data,
             const size_t iteration = 1);
  void Test(const feature_vector& fv, std::string* predict) const;
  void GetFeatureWeight(size_t feature_id,
                        std::vector<std::pair<std::string, double> >* results) const;

 private:
  void CalcScores(const feature_vector& fv,
                  score2class* scores) const;

  void Update(const datum& datum,
              const score2class& scores);

  weight_matrix weight_;
  size_t dataN_;
  double eta_;
};

} //namespace
} //namespace

#endif //CLASSIFIER_SUBGRADIENT_HINGE_H_
