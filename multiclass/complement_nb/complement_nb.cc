#include "complement_nb.h"

#include <cmath>
namespace classifier {
namespace naivebayes {
double ComplementNaiveBayes::CalculateProbability(const feature_vector& fv,
                                                  const std::string& category) const {
  double probability = 0.0;
  double smoothing_parameter = 0.0;
  if (smoothing_)
    smoothing_parameter = alpha_ - 1.0;

  // Class Probability
  probability -= log(
      ((document_sum_ - document_count_.at(category)) + smoothing_parameter) /
      ((double)document_sum_ + document_count_.size() * smoothing_parameter) );

  // Calculate Word Sum Except One Category
  double word_sum_except_a_category = 0.0;
  for (word_sum_vector::const_iterator it = word_sum_in_each_category_.begin();
       it != word_sum_in_each_category_.end();
       ++it) {
    if (it->first == category) continue;
    word_sum_except_a_category += it->second;
  }

  // Calculate Word Count Except One Category
  for (feature_vector::const_iterator fv_it = fv.begin();
       fv_it != fv.end();
       ++fv_it) {
    size_t word_id = fv_it->first;
    double word_count_except_a_category = 0.0;
    for (word_matrix::const_iterator cate_it = word_count_in_each_category_.begin();
         cate_it != word_count_in_each_category_.end();
         ++cate_it) {
      if (cate_it->first == category) continue;

      const word_vector &word_count_in_a_category
          = word_count_in_each_category_.at(cate_it->first);
      if (word_id < word_count_in_a_category.size())
        word_count_except_a_category += word_count_in_a_category.at(word_id);
      else
        continue;
    }

    // Word Probability
    if (word_count_except_a_category != 0) {
      probability -= fv_it->second * log(
          (word_count_except_a_category + smoothing_parameter)
          / ((double)word_sum_except_a_category
             + (fv.size() * smoothing_parameter)) );
    } else {
      if (!smoothing_) {
        probability = - non_class_score;
        break;
      }

      // Approximate the number of word summation
      probability -= fv_it->second * log(
          smoothing_parameter /
          ((double)word_sum_except_a_category
           + (fv.size() * smoothing_parameter)) );
    }
  }

  //std::cout << category << " : " << probability << std::endl;
  return probability;
}

} //namespace
} //namespace
