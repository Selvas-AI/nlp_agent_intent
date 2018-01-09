/*!
 *      @date    2017/08/30
 *      @file    fasttext_wrapper.h
 *      @author  jupark
 *
 *      Copyright   2017 by SELVAS AI Co.,LTD. All rights reserved.
 */

#ifndef FASTTEXT_WRAPPER_H_
#define FASTTEXT_WRAPPER_H_ 1

#include <map>

#include "fasttext.h"
#include "json/json.h"


namespace selvy {

class FasttextWrapper {
public:
  typedef fasttext::FastText Model;
  typedef fasttext::Args Parameter;

  typedef std::pair<std::string, float> Result;
  typedef std::vector<Result> ResultList;

  typedef std::pair<bool, ResultList> ReturnType;

  typedef std::vector<float> RealVector;
  typedef std::pair<std::string, RealVector> RealVectorResult;
  typedef std::vector<RealVectorResult> VectorResultList;
  typedef std::pair<bool, VectorResultList> VectorListReturnType;

  typedef std::pair<bool, RealVector> VectorReturnType;
  typedef std::pair<bool, std::string> StringReturnType;

public:
  FasttextWrapper();
  ~FasttextWrapper();

public:
  bool LoadModel(char* path);
  bool Train(char* info);
  ReturnType Analogies(int k, char* A, char* B, char* C);
  ReturnType NN(int k, char* query);
  VectorListReturnType GetNgrams(char* word);
  VectorReturnType GetSentenceVector(char* query);
  VectorReturnType GetWordVectors(char* query);
  ReturnType Predict(int k, char* query);
  StringReturnType Test(int k, char* file_path);
  bool Quantize(char* info);

private:
  void Init();
  bool GetParameter(char* string, Parameter* parameter);

  bool SetInputPath(const Json::Value& info, Parameter* parameter);
  bool SetOutputPath(const Json::Value& info, Parameter* parameter);
  bool SetVerbose(const Json::Value& info, Parameter* parameter);
  bool SetDictionary(const Json::Value& info, Parameter* parameter);
  bool SetTraining(const Json::Value& info, Parameter* parameter);
  bool SetQuantization(const Json::Value& info, Parameter* parameter);

  // dictionary
  bool SetMinCount(const Json::Value& info, Parameter* parameter);
  bool SetMinCountLabel(const Json::Value& info, Parameter* parameter);
  bool SetWordNgrams(const Json::Value& info, Parameter* parameter);
  bool SetBucket(const Json::Value& info, Parameter* parameter);
  bool SetMinN(const Json::Value& info, Parameter* parameter);
  bool SetMaxN(const Json::Value& info, Parameter* parameter);
  bool SetT(const Json::Value& info, Parameter* parameter);
  bool SetLabel(const Json::Value& info, Parameter* parameter);

  // training
  bool SetModel(const Json::Value& info, Parameter* parameter);
  bool SetLr(const Json::Value& info, Parameter* parameter);
  bool SetLrUpdateRate(const Json::Value& info, Parameter* parameter);
  bool SetDim(const Json::Value& info, Parameter* parameter);
  bool SetWs(const Json::Value& info, Parameter* parameter);
  bool SetEpoch(const Json::Value& info, Parameter* parameter);
  bool SetNeg(const Json::Value& info, Parameter* parameter);
  bool SetLoss(const Json::Value& info, Parameter* parameter);
  bool SetThread(const Json::Value& info, Parameter* parameter);
  bool SetPretrainedVectors(const Json::Value& info, Parameter* parameter);
  bool SetSaveOutput(const Json::Value& info, Parameter* parameter);

  // quantization
  bool SetCutOff(const Json::Value& info, Parameter* parameter);
  bool SetReTrain(const Json::Value& info, Parameter* parameter);
  bool SetQNorm(const Json::Value& info, Parameter* parameter);
  bool SetQOut(const Json::Value& info, Parameter* parameter);
  bool SetDSub(const Json::Value& info, Parameter* parameter);

private:
  static const char kInputPath[];
  static const char kOutputPath[];
  static const char kVerbose[];
  static const char kDictionary[];
  static const char kMinCount[];
  static const char kMinCountLabel[];
  static const char kWordNGrams[];
  static const char kBucket[];
  static const char kMinN[];
  static const char kMaxN[];
  static const char kT[];
  static const char kLabel[];
  static const char kTraining[];
  static const char kModel[];
  static const char kLr[];
  static const char kLrUpdateRate[];
  static const char kDim[];
  static const char kWs[];
  static const char kEpoch[];
  static const char kNeg[];
  static const char kLoss[];
  static const char kThread[];
  static const char kPretrainedVectors[];
  static const char kSaveOutput[];
  static const char kQuantization[];
  static const char kCutOff[];
  static const char kReTrain[];
  static const char kQNorm[];
  static const char kQOut[];
  static const char kDSub[];

  static const char kSkipGram[];
  static const char kCbow[];
  static const char kSupervised[];

  static const char kHs[];
  static const char kNs[];
  static const char kSoftmax[];

  std::shared_ptr<Model> model_;
};

}  // namespace selvy

#endif // FASTTEXT_WRAPPER_H_
