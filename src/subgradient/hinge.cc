#include <subgradient/hinge.h>

#include <cmath>
#include <algorithm>

namespace classifier {
namespace subgradient {
SubgradientHinge::SubgradientHinge(double eta) : dataN_(0), eta_(eta) {
  weight_matrix().swap(weight_);
}

void SubgradientHinge::Train(const datum& datum) {
  ++dataN_;
  std::vector<std::pair<double, std::string> > score2class(0);
  CalcScores(datum.fv, &score2class);

  Update(datum.category, score2class, datum.fv);
}

void SubgradientHinge::Train(const std::vector<datum>& data,
                             const size_t iteration) {
  for (size_t iter = 0; iter < iteration; ++iter) {
    for (size_t i = 0; i < data.size(); ++i) {
      Train(data[i]);
    }
  }
}

void SubgradientHinge::Test(const feature_vector& fv,
                            std::string* predict) const {
  std::vector<std::pair<double, std::string> > score2class(0);
  CalcScores(fv, &score2class);
  *predict = score2class[0].second;
}

void SubgradientHinge::CalcScores(const feature_vector& fv,
                                  std::vector<std::pair<double, std::string> >* score2class) const {
  score2class->push_back(make_pair(non_class_score, non_class));

  for (weight_matrix::const_iterator it = weight_.begin();
       it != weight_.end();
       ++it) {
    weight_vector wv = it->second;
    double score = InnerProduct(fv, &wv);
    score2class->push_back(make_pair(score, it->first));
  }

  sort(score2class->begin(), score2class->end(),
       std::greater<std::pair<double, std::string> >());
}

void SubgradientHinge::Update(const std::string& correct,
                              const std::vector<std::pair<double, std::string> >& score2class,
                              const feature_vector& fv) {
  std::string non_correct_predict;
  double hinge_loss = CalcHingeLoss(score2class, correct, &non_correct_predict);

  if (hinge_loss > 0.0) {
    double step_distance = eta_ / (std::sqrt(dataN_) * 2.0);

    for (feature_vector::const_iterator it = fv.begin();
         it != fv.end();
         ++it) {
      weight_[correct][it->first] += step_distance * it->second;
      if (non_correct_predict != non_class)
        weight_[non_correct_predict][it->first] -= step_distance * it->second;
    }
  }
}

double SubgradientHinge::CalcHingeLoss(const std::vector<std::pair<double, std::string> >& score2class,
                                       const std::string& correct,
                                       std::string* non_correct_predict) const {
  bool correct_done = false;
  bool predict_done = false;
  double score = 1.0;
  for (std::vector<std::pair<double, std::string> >::const_iterator
           it = score2class.begin();
       it != score2class.end();
       ++it) {
    if (it->second == correct) {
      score -= it->first;
      correct_done = true;
    } else if (!predict_done) {
      *non_correct_predict = it->second;
      if (*non_correct_predict != non_class)
        score += it->first;
      predict_done = true;
    }

    if (correct_done && predict_done)
      break;
  }

  return score;
}

void SubgradientHinge::GetFeatureWeight(const std::string& feature,
                                        std::vector<std::pair<std::string, double> >* results) const {
  ReturnFeatureWeight(feature, weight_, results);
}

} //namespace
} //namespace
