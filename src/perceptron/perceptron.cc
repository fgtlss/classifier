#include <perceptron/perceptron.h>

#include <algorithm>

namespace classifier {
namespace perceptron {
Perceptron::Perceptron() {
  weight_matrix().swap(weight_);
}

void Perceptron::Train(const datum& datum) {
  std::string predict;
  Test(datum.fv, &predict);
  Update(datum, predict);
}

void Perceptron::Train(const std::vector<datum>& data,
                       const size_t iteration) {
  for (size_t iter = 0; iter < iteration; ++iter) {
    for (size_t i = 0; i < data.size(); ++i) {
      Train(data[i]);
    }
  }
}

void Perceptron::Test(const feature_vector& fv,
                      std::string* predict) const {
  score2class scores(0);
  CalcScores(fv, &scores);
  *predict = scores[0].second;
}

void Perceptron::CalcScores(const feature_vector& fv,
                            score2class* scores) const {
  scores->push_back(make_pair(non_class_score, non_class));

  for (weight_matrix::const_iterator it = weight_.begin();
       it != weight_.end();
       ++it) {
    weight_vector wv = it->second;
    double score = InnerProduct(fv, &wv);
    scores->push_back(make_pair(score, it->first));
  }

  sort(scores->begin(), scores->end(),
       std::greater<std::pair<double, std::string> >());
}

void Perceptron::Update(const datum& datum,
                        const std::string& predict) {
  if (datum.category == predict)
    return;

  std::vector<double> &correct_weight = weight_[datum.category];
  for (feature_vector::const_iterator it = datum.fv.begin();
       it != datum.fv.end();
       ++it) {
    if (correct_weight.size() <= it->first)
      correct_weight.resize(it->first + 1, 0.0);
    correct_weight[it->first] += it->second / 2.0;
  }

  if (predict == non_class)
    return;

  std::vector<double> &wrong_weight = weight_[predict];
  for (feature_vector::const_iterator it = datum.fv.begin();
       it != datum.fv.end();
       ++it) {
    if (wrong_weight.size() <= it->first)
      wrong_weight.resize(it->first + 1, 0.0);
    wrong_weight[it->first] -= it->second / 2.0;
  }
}

void Perceptron::GetFeatureWeight(size_t feature_id,
                                  std::vector<std::pair<std::string, double> >* results) const {
  ReturnFeatureWeight(feature_id, weight_, results);
}

} //namespace
} //namespace
