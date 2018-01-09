/*!
 *      @date    2017/08/30
 *      @file    fasttext_wrapper.cc
 *      @author  jupark
 *
 *      Copyright   2017 by SELVAS AI Co.,LTD. All rights reserved.
 */

#include <string>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <fstream>
#include <iconv.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#endif

#include <sstream>

#include "fasttext_wrapper.h"


#ifdef _WIN32
std::string UTF8ToANSI(char* src) {
  std::wstring utf16_str;
  int str_len = MultiByteToWideChar(CP_UTF8, 0, src, strlen(src), NULL, NULL);
  utf16_str.resize(str_len);
  MultiByteToWideChar(CP_UTF8, 0, src, strlen(src) + 1, &utf16_str[0], utf16_str.size());

  str_len = WideCharToMultiByte(CP_ACP, 0, &utf16_str[0], utf16_str.size(), NULL, 0, NULL, NULL);
  std::string ansi_str;
  ansi_str.resize(str_len);
  WideCharToMultiByte(CP_ACP, 0, &utf16_str[0], utf16_str.size(), &ansi_str[0], ansi_str.size(), NULL, NULL);
  return ansi_str;
}

std::string ANSIToUTF8(char* src) {
  std::wstring utf16_str;
  int str_len = MultiByteToWideChar(CP_ACP, 0, src, strlen(src), NULL, NULL);
  utf16_str.resize(str_len);
  MultiByteToWideChar(CP_ACP, 0, src, strlen(src) + 1, &utf16_str[0], utf16_str.size());

  str_len = WideCharToMultiByte(CP_UTF8, 0, &utf16_str[0], utf16_str.size(), NULL, 0, NULL, NULL);
  std::string utf8_str;
  utf8_str.resize(str_len);
  WideCharToMultiByte(CP_UTF8, 0, &utf16_str[0], utf16_str.size(), &utf8_str[0], utf8_str.size(), NULL, NULL);
  return utf8_str;
}
#else
std::string ANSIToUTF8(char* src) {
  char dst[1024];
  memset(dst, '\0', 1024);

  char* input_buf_ptr = src;
  char* output_buf_ptr = dst;

  size_t in_size = strlen(src);
  size_t  out_size = sizeof(dst);
  /************************************************************************/
  iconv_t it = iconv_open("UTF-8//TRANSLIT//IGNORE", "CP949//TRANSLIT//IGNORE"); // EUC-KR을 UTF-8로
  int ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);
  if (ret < 0) {
    char err_buf[1024];
    int err = errno;
    strerror_r(errno, err_buf, sizeof(err_buf));
    printf("ANSIToUTF8 ret : %d, errno : %d(%s)\n", ret, errno, err_buf);
    return "";
  }
  iconv_close(it);
  return dst;
}

std::string UTF8ToANSI(char* src) {
  char dst[1024];
  memset(dst, '\0', 1024);

  char* input_buf_ptr = src;
  char* output_buf_ptr = dst;

  size_t in_size = strlen(src);
  size_t  out_size = sizeof(dst);
  /************************************************************************/
  iconv_t it = iconv_open("CP949//TRANSLIT//IGNORE", "UTF-8//TRANSLIT//IGNORE"); // EUC-KR을 UTF-8로
  int ret = iconv(it, &input_buf_ptr, &in_size, &output_buf_ptr, &out_size);
  if (ret < 0) {
    char err_buf[1024];
    int err = errno;
    strerror_r(errno, err_buf, sizeof(err_buf));
    printf("UTF8ToANSI ret : %d, errno : %d(%s)\n", ret, errno, err_buf);
    return "";
  }
  iconv_close(it);
  return dst;
}
#endif

namespace selvy {
const char FasttextWrapper::kInputPath[] = "input_path";
const char FasttextWrapper::kOutputPath[] = "output_path";
const char FasttextWrapper::kVerbose[] = "verbose";
const char FasttextWrapper::kDictionary[] = "dictionary";
const char FasttextWrapper::kMinCount[] = "minCount";
const char FasttextWrapper::kMinCountLabel[] = "minCountLabel";
const char FasttextWrapper::kWordNGrams[] = "wordNgrams";
const char FasttextWrapper::kBucket[] = "bucket";
const char FasttextWrapper::kMinN[] = "minn";
const char FasttextWrapper::kMaxN[] = "maxn";
const char FasttextWrapper::kT[] = "t";
const char FasttextWrapper::kLabel[] = "label";
const char FasttextWrapper::kTraining[] = "training";
const char FasttextWrapper::kModel[] = "model";
const char FasttextWrapper::kLr[] = "lr";
const char FasttextWrapper::kLrUpdateRate[] = "lrUpdateRate";
const char FasttextWrapper::kDim[] = "dim";
const char FasttextWrapper::kWs[] = "ws";
const char FasttextWrapper::kEpoch[] = "epoch";
const char FasttextWrapper::kNeg[] = "neg";
const char FasttextWrapper::kLoss[] = "loss";
const char FasttextWrapper::kThread[] = "thread";
const char FasttextWrapper::kPretrainedVectors[] = "pretrainedVectors";
const char FasttextWrapper::kSaveOutput[] = "saveOutput";
const char FasttextWrapper::kQuantization[] = "quantization";
const char FasttextWrapper::kCutOff[] = "cutoff";
const char FasttextWrapper::kReTrain[] = "retrain";
const char FasttextWrapper::kQNorm[] = "qnorm";
const char FasttextWrapper::kQOut[] = "qout";
const char FasttextWrapper::kDSub[] = "dsub";

const char FasttextWrapper::kSkipGram[] = "skipgram";
const char FasttextWrapper::kCbow[] = "cbow";
const char FasttextWrapper::kSupervised[] = "supervised";

const char FasttextWrapper::kHs[] = "hs";
const char FasttextWrapper::kNs[] = "ns";
const char FasttextWrapper::kSoftmax[] = "softmax";

FasttextWrapper::FasttextWrapper() {
}

FasttextWrapper::~FasttextWrapper() {
}

void FasttextWrapper::Init() {
}

bool FasttextWrapper::LoadModel(char* path) {
  if (path == NULL) {
    std::cerr << "Path is null!" << std::endl;
    return false;
  }

  std::string path_string = UTF8ToANSI(path);
  model_ = std::make_shared<Model>();
  if (model_->loadModel(path_string) == false) {
    std::cerr << "Model can't load!" << std::endl;
    return false;
  }
  return true;
}

bool FasttextWrapper::Train(char* info) {
  if (info == NULL) {
    std::cerr << "Parameter is null!" << std::endl;
    return false;
  }

  Parameter parameter;
  if (GetParameter(info, &parameter) == false) {
    std::cerr << "Parameter can't get!" << std::endl;
    return false;
  }
  std::shared_ptr<Parameter> shared_parameter = std::make_shared<Parameter>(parameter);

  model_ = std::make_shared<Model>();
  if (model_->train(shared_parameter) == false) {
    std::cerr << "Model can't train!" << std::endl;
    return false;
  }
  return true;
}

FasttextWrapper::ReturnType FasttextWrapper::Analogies(int k, char* A, char* B,
    char* C) {
  ResultList result;
  if ((model_ == NULL) || (A == NULL) || (B == NULL) || (C == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else if (A == NULL) {
      std::cerr << "A is null!" << std::endl;
    } else if (B == NULL) {
      std::cerr << "B is null!" << std::endl;
    } else {
      std::cerr << "C is null!" << std::endl;
    }

    return ReturnType(false, result);
  }

  if (model_->analogies(k, A, B, C, &result) == false) {
    std::cerr << "Model can\'t analogies!" << std::endl;
    return ReturnType(false, result);
  }
  return ReturnType(true, result);
}

FasttextWrapper::ReturnType FasttextWrapper::NN(int k, char* query) {
  ResultList result;
  if ((model_ == NULL) || (query == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else {
      std::cerr << "query is null!" << std::endl;
    }
    return ReturnType(false, result);
  }

  if (model_->nn(k, query, &result) == false) {
    std::cerr << "Model can\'t nn!" << std::endl;
    return ReturnType(false, result);
  }
  return ReturnType(true, result);
}

FasttextWrapper::VectorListReturnType FasttextWrapper::GetNgrams(char* word) {
  VectorResultList result;
  if ((model_ == NULL) || (word == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else {
      std::cerr << "word is null!" << std::endl;
    }
    return VectorListReturnType(false, result);
  }

  if (model_->ngramVectors(word, &result) == false) {
    std::cerr << "Model can\'t ngramVectors!" << std::endl;
    VectorListReturnType(false, result);
  }
  return VectorListReturnType(true, result);
}

FasttextWrapper::VectorReturnType FasttextWrapper::GetSentenceVector(char* query) {
  RealVector vector;
  vector.clear();
  if ((model_ == NULL) || (query == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else {
      std::cerr << "query is null!" << std::endl;
    }
    return VectorReturnType(false, vector);
  }

  std::string query_string = query;
  query_string += '\n';
  std::istringstream query_stream(query_string);

  if (model_->printSentenceVectors(query_stream, &vector) == false) {
    std::cerr << "Model can\'t printSentenceVectors!" << std::endl;
    return VectorReturnType(false, vector);
  }
  return VectorReturnType(true, vector);
}

FasttextWrapper::VectorReturnType FasttextWrapper::GetWordVectors(char* query) {
  RealVector vector;
  vector.clear();
  if ((model_ == NULL) || (query == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else {
      std::cerr << "query is null!" << std::endl;
    }
    return VectorReturnType(false, vector);
  }

  if (model_->printWordVectors(query, &vector) == false) {
    std::cerr << "Model can\'t printWordVectors!" << std::endl;
    return VectorReturnType(false, vector);
  }
  return VectorReturnType(true, vector);
}

FasttextWrapper::ReturnType FasttextWrapper::Predict(int k, char* query) {
  ResultList result;
  if ((model_ == NULL) || (query == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model is null!" << std::endl;
    } else {
      std::cerr << "query is null!" << std::endl;
    }
    return ReturnType(false, result);
  }

  std::string query_string = query;
  query_string += '\n';  
  std::istringstream query_stream(query_string);

  if (model_->predict(k, query_stream, &result) == false) {
    std::cerr << "Model can\'t predict!" << std::endl;
    return ReturnType(false, result);
  }
  return ReturnType(true, result);
}

FasttextWrapper::StringReturnType FasttextWrapper::Test(int k, char* file_path) {
  std::string result = "";
  if ((model_ == NULL) || (file_path == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model name is null!" << std::endl;
    } else {
      std::cerr << "file path is null!" << std::endl;
    }
    return StringReturnType(false, result);
  }

  std::string file_path_string = UTF8ToANSI(file_path);

  int n = 0;
  fasttext::real precision = 0.0f;
  fasttext::real recall = 0.0f;
  if (model_->test(file_path_string, k, &n, &precision, &recall) == false) {
    std::cerr << "Model can\'t test!" << std::endl;
    return StringReturnType(false, result);
  }

  result += "{";
  result += ("\"n\":" + std::to_string(n) + ",");
  result += ("\"precision\":" + std::to_string(precision) + ",");
  result += ("\"recall\":" + std::to_string(recall));
  result += "}";

  std::string result_string = ANSIToUTF8(const_cast<char*>(result.c_str()));
  return StringReturnType(true, result_string);
}

bool FasttextWrapper::Quantize(char* info) {
  if ((model_ == NULL) || (info == NULL)) {
    if (model_ == NULL) {
      std::cerr << "Model name is null!" << std::endl;
    } else {
      std::cerr << "Parameter is null!" << std::endl;
    }
    return false;
  }

  Parameter parameter;
  if (GetParameter(info, &parameter) == false) {
    std::cerr << "Parameter can't get!" << std::endl;
    return false;
  }
  std::shared_ptr<Parameter> shared_parameter = std::make_shared<Parameter>(parameter);

  model_ = std::make_shared<Model>();
  if (model_->quantize(shared_parameter) == false) {
    std::cerr << "Model can\'t quantize!" << std::endl;
    return false;
  }

  return true;
}

bool FasttextWrapper::GetParameter(char* string, Parameter* parameter) {
  if ((string == NULL) || (parameter == NULL)) {
    if (string == NULL) {
      std::cerr << "string is null!" << std::endl;
    } else {
      std::cerr << "Parameter is null!" << std::endl;
    }
    return false;
  }

  std::string parameter_string = UTF8ToANSI(string);

  Json::Value parameter_info;
  Json::Reader reader;
  bool result = reader.parse(parameter_string, parameter_info);
  if ((result == false) || (parameter_info.empty())) {
    std::cerr << "json can\'t parse!" << std::endl;
    return false;
  }

  if (SetInputPath(parameter_info, parameter) == false) {
    std::cerr << "input path can\'t set!" << std::endl;
    return false;
  }

  if (SetOutputPath(parameter_info, parameter) == false) {
    std::cerr << "output path can\'t set!" << std::endl;
    return false;
  }

  if (SetVerbose(parameter_info, parameter) == false) {
    std::cerr << "verbose can\'t set!" << std::endl;
    return false;
  }

  if (SetDictionary(parameter_info, parameter) == false) {
    std::cerr << "dictionary can\'t set!" << std::endl;
    return false;
  }

  if (SetTraining(parameter_info, parameter) == false) {
    std::cerr << "training can\'t set!" << std::endl;
    return false;
  }

  if (SetQuantization(parameter_info, parameter) == false) {
    std::cerr << "quantization can\'t set!" << std::endl;
    return false;
  }
  return true;
}

bool FasttextWrapper::SetInputPath(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kInputPath) == false) {
    std::cerr << "input path isn\'t member!" << std::endl;
    return true;
  }

  Json::Value input_path = info[kInputPath];
  if (input_path.empty() == true) {
    std::cerr << "input path is empty!" << std::endl;
    return false;
  }
  parameter->input = input_path.asString();
  return true;
}

bool FasttextWrapper::SetOutputPath(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kOutputPath) == false) {
    std::cerr << "output path isn\'t member!" << std::endl;
    return true;
  }

  Json::Value output_path = info[kOutputPath];
  if (output_path.empty() == true) {
    std::cerr << "output path is empty!" << std::endl;
    return false;
  }
  parameter->output = output_path.asString();
  return true;
}

bool FasttextWrapper::SetVerbose(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kVerbose) == false) {
    std::cerr << "verbose isn\'t member!" << std::endl;
    return true;
  }

  Json::Value verbose = info[kVerbose];
  if (verbose.empty() == true) {
    std::cerr << "verbose is empty!" << std::endl;
    return false;
  }
  parameter->verbose = verbose.asInt();
  return true;
}

bool FasttextWrapper::SetDictionary(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kDictionary) == false) {
    std::cerr << "dictionary isn\'t member!" << std::endl;
    return true;
  }

  Json::Value dictionary = info[kDictionary];
  if (dictionary.empty() == true) {
    std::cerr << "dictionary is empty!" << std::endl;
    return false;
  }

  if (SetMinCount(dictionary, parameter) == false) {
    std::cerr << "min count can\'t set!" << std::endl;
    return false;
  }

  if (SetMinCountLabel(dictionary, parameter) == false) {
    std::cerr << "min count label can\'t set!" << std::endl;
    return false;
  }

  if (SetWordNgrams(dictionary, parameter) == false) {
    std::cerr << "word ngrams can\'t set!" << std::endl;
    return false;
  }

  if (SetBucket(dictionary, parameter) == false) {
    std::cerr << "bucket can\'t set!" << std::endl;
    return false;
  }

  if (SetMinN(dictionary, parameter) == false) {
    std::cerr << "min n can\'t set!" << std::endl;
    return false;
  }

  if (SetMaxN(dictionary, parameter) == false) {
    std::cerr << "max n can\'t set!" << std::endl;
    return false;
  }

  if (SetT(dictionary, parameter) == false) {
    std::cerr << "T can\'t set!" << std::endl;
    return false;
  }

  if (SetLabel(dictionary, parameter) == false) {
    std::cerr << "label can\'t set!" << std::endl;
    return false;
  }
  return true;
}

bool FasttextWrapper::SetTraining(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kTraining) == false) {
    std::cerr << "training isn\'t member!" << std::endl;
    return true;
  }

  Json::Value training = info[kTraining];
  if (training.empty() == true) {
    std::cerr << "training is empty!" << std::endl;
    return false;
  }

  if (SetModel(training, parameter) == false) {
    std::cerr << "model can\'t set!" << std::endl;
    return false;
  }

  if (SetLr(training, parameter) == false) {
    std::cerr << "Lr can\'t set!" << std::endl;
    return false;
  }

  if (SetLrUpdateRate(training, parameter) == false) {
    std::cerr << "LrUpdateRate can\'t set!" << std::endl;
    return false;
  }

  if (SetDim(training, parameter) == false) {
    std::cerr << "Dim can\'t set!" << std::endl;
    return false;
  }

  if (SetWs(training, parameter) == false) {
    std::cerr << "Ws can\'t set!" << std::endl;
    return false;
  }

  if (SetEpoch(training, parameter) == false) {
    std::cerr << "Epoch can\'t set!" << std::endl;
    return false;
  }

  if (SetNeg(training, parameter) == false) {
    std::cerr << "Neg can\'t set!" << std::endl;
    return false;
  }

  if (SetLoss(training, parameter) == false) {
    std::cerr << "Loss can\'t set!" << std::endl;
    return false;
  }

  if (SetThread(training, parameter) == false) {
    std::cerr << "Thread can\'t set!" << std::endl;
    return false;
  }

  if (SetPretrainedVectors(training, parameter) == false) {
    std::cerr << "PretrainedVectors can\'t set!" << std::endl;
    return false;
  }

  if (SetSaveOutput(training, parameter) == false) {
    std::cerr << "SaveOutput can\'t set!" << std::endl;
    return false;
  }
  return true;
}

bool FasttextWrapper::SetQuantization(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kQuantization) == false) {
    std::cerr << "quantization isn\'t member!" << std::endl;
    return true;
  }

  Json::Value quantization = info[kQuantization];
  if (quantization.empty() == true) {
    std::cerr << "quantization is empty!" << std::endl;
    return false;
  }

  if (SetCutOff(quantization, parameter) == false) {
    std::cerr << "cut off can\'t set!" << std::endl;
    return false;
  }

  if (SetReTrain(quantization, parameter) == false) {
    std::cerr << "retrain can\'t set!" << std::endl;
    return false;
  }

  if (SetQNorm(quantization, parameter) == false) {
    std::cerr << "qnorm can\'t set!" << std::endl;
    return false;
  }

  if (SetQOut(quantization, parameter) == false) {
    std::cerr << "qout can\'t set!" << std::endl;
    return false;
  }

  if (SetDSub(quantization, parameter) == false) {
    std::cerr << "dsub can\'t set!" << std::endl;
    return false;
  }
  return true;
}

bool FasttextWrapper::SetMinCount(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kMinCount) == false) {
    std::cerr << "min count isn\'t member!" << std::endl;
    return true;
  }

  Json::Value min_count = info[kMinCount];
  if (min_count.empty() == true) {
    std::cerr << "min count is empty!" << std::endl;
    return false;
  }
  parameter->minCount = min_count.asInt();
  return true;
}

bool FasttextWrapper::SetMinCountLabel(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kMinCountLabel) == false) {
    std::cerr << "min count label isn\'t member!" << std::endl;
    return true;
  }

  Json::Value min_count_label = info[kMinCountLabel];
  if (min_count_label.empty() == true) {
    std::cerr << "min count label is empty!" << std::endl;
    return false;
  }
  parameter->minCountLabel = min_count_label.asInt();
  return true;
}

bool FasttextWrapper::SetWordNgrams(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kWordNGrams) == false) {
    std::cerr << "word ngrams isn\'t member!" << std::endl;
    return true;
  }

  Json::Value word_ngrams = info[kWordNGrams];
  if (word_ngrams.empty() == true) {
    std::cerr << "word ngrams is empty!" << std::endl;
    return false;
  }
  parameter->wordNgrams = word_ngrams.asInt();
  return true;
}

bool FasttextWrapper::SetBucket(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kBucket) == false) {
    std::cerr << "bucket isn\'t member!" << std::endl;
    return true;
  }

  Json::Value bucket = info[kBucket];
  if (bucket.empty() == true) {
    std::cerr << "bucket is empty!" << std::endl;
    return false;
  }
  parameter->bucket = bucket.asInt();
  return true;
}

bool FasttextWrapper::SetMinN(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kMinN) == false) {
    std::cerr << "min n isn\'t member!" << std::endl;
    return true;
  }

  Json::Value min_n = info[kMinN];
  if (min_n.empty() == true) {
    std::cerr << "min n is empty!" << std::endl;
    return false;
  }
  parameter->minn = min_n.asInt();
  return true;
}

bool FasttextWrapper::SetMaxN(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kMaxN) == false) {
    std::cerr << "max n isn\'t member!" << std::endl;
    return true;
  }

  Json::Value max_n = info[kMaxN];
  if (max_n.empty() == true) {
    std::cerr << "max n is empty!" << std::endl;
    return false;
  }
  parameter->maxn = max_n.asInt();
  return true;
}

bool FasttextWrapper::SetT(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kT) == false) {
    std::cerr << "T isn\'t member!" << std::endl;
    return true;
  }

  Json::Value t = info[kT];
  if (t.empty() == true) {
    std::cerr << "T is empty!" << std::endl;
    return false;
  }
  parameter->t = t.asDouble();
  return true;
}

bool FasttextWrapper::SetLabel(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kLabel) == false) {
    std::cerr << "label isn\'t member!" << std::endl;
    return true;
  }

  Json::Value label = info[kLabel];
  if (label.empty() == true) {
    std::cerr << "label is empty!" << std::endl;
    return false;
  }
  parameter->label = label.asString();
  return true;
}

bool FasttextWrapper::SetModel(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kModel) == false) {
    std::cerr << "model isn\'t member!" << std::endl;
    return true;
  }

  Json::Value model = info[kModel];
  if (model.empty() == true) {
    std::cerr << "model is empty!" << std::endl;
    return false;
  }
  std::string model_name = model.asString();
  if (model_name.compare(kSkipGram) == 0) {
    parameter->model = fasttext::model_name::sg;
  } else if (model_name.compare(kCbow) == 0) {
    parameter->model = fasttext::model_name::cbow;
  } else if (model_name.compare(kSupervised) == 0) {
    parameter->model = fasttext::model_name::sup;
  } else {
    std::cerr << "model don\'t support name!" << std::endl;
    return false;
  }

  return true;
}

bool FasttextWrapper::SetLr(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kLr) == false) {
    std::cerr << "lr isn\'t member!" << std::endl;
    return true;
  }

  Json::Value lr = info[kLr];
  if (lr.empty() == true) {
    std::cerr << "lr is empty!" << std::endl;
    return false;
  }
  parameter->lr = lr.asDouble();
  return true;
}

bool FasttextWrapper::SetLrUpdateRate(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kLrUpdateRate) == false) {
    std::cerr << "lr update rate isn\'t member!" << std::endl;
    return true;
  }

  Json::Value lr_update_rate = info[kLrUpdateRate];
  if (lr_update_rate.empty() == true) {
    std::cerr << "lr update rate is empty!" << std::endl;
    return false;
  }
  parameter->lrUpdateRate = lr_update_rate.asInt();
  return true;
}

bool FasttextWrapper::SetDim(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kDim) == false) {
    std::cerr << "dim isn\'t member!" << std::endl;
    return true;
  }

  Json::Value dim = info[kDim];
  if (dim.empty() == true) {
    std::cerr << "dim is empty!" << std::endl;
    return false;
  }
  parameter->dim = dim.asInt();
  return true;
}

bool FasttextWrapper::SetWs(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kWs) == false) {
    std::cerr << "ws isn\'t member!" << std::endl;
    return true;
  }

  Json::Value ws = info[kWs];
  if (ws.empty() == true) {
    std::cerr << "ws is empty!" << std::endl;
    return false;
  }
  parameter->ws = ws.asInt();
  return true;
}

bool FasttextWrapper::SetEpoch(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kEpoch) == false) {
    std::cerr << "epoch isn\'t member!" << std::endl;
    return true;
  }

  Json::Value epoch = info[kEpoch];
  if (epoch.empty() == true) {
    std::cerr << "epoch is empty!" << std::endl;
    return false;
  }
  parameter->epoch = epoch.asInt();
  return true;
}

bool FasttextWrapper::SetNeg(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kNeg) == false) {
    std::cerr << "neg isn\'t member!" << std::endl;
    return true;
  }

  Json::Value neg = info[kNeg];
  if (neg.empty() == true) {
    std::cerr << "neg is empty!" << std::endl;
    return false;
  }
  parameter->neg = neg.asInt();
  return true;
}

bool FasttextWrapper::SetLoss(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kLoss) == false) {
    std::cerr << "loss isn\'t member!" << std::endl;
    return true;
  }

  Json::Value loss = info[kLoss];
  if (loss.empty() == true) {
    std::cerr << "loss is empty!" << std::endl;
    return false;
  }
  std::string loss_name = loss.asString();
  if (loss_name.compare(kHs) == 0) {
    parameter->loss = fasttext::loss_name::hs;
  } else if (loss_name.compare(kNs) == 0) {
    parameter->loss = fasttext::loss_name::ns;
  } else if (loss_name.compare(kSoftmax) == 0) {
    parameter->loss = fasttext::loss_name::softmax;
  } else {
    std::cerr << "loss don\'t support name!" << std::endl;
    return false;
  }

  return true;
}

bool FasttextWrapper::SetThread(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kThread) == false) {
    std::cerr << "thread isn\'t member!" << std::endl;
    return true;
  }

  Json::Value thread = info[kThread];
  if (thread.empty() == true) {
    std::cerr << "thread is empty!" << std::endl;
    return false;
  }
  parameter->thread = thread.asInt();
  return true;
}

bool FasttextWrapper::SetPretrainedVectors(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kPretrainedVectors) == false) {
    std::cerr << "pretrained vectors isn\'t member!" << std::endl;
    return true;
  }

  Json::Value pretrained_vectors = info[kPretrainedVectors];
  if (pretrained_vectors.empty() == true) {
    std::cerr << "pretrained vectors is empty!" << std::endl;
    return false;
  }
  parameter->pretrainedVectors = pretrained_vectors.asString();
  return true;
}

bool FasttextWrapper::SetSaveOutput(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kSaveOutput) == false) {
    std::cerr << "save output isn\'t member!" << std::endl;
    return true;
  }

  Json::Value save_output = info[kSaveOutput];
  if (save_output.empty() == true) {
    std::cerr << "save output is empty!" << std::endl;
    return false;
  }
  parameter->saveOutput = save_output.asInt();
  return true;
}

bool FasttextWrapper::SetCutOff(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kCutOff) == false) {
    std::cerr << "cutoff isn\'t member!" << std::endl;
    return true;
  }

  Json::Value cutoff = info[kCutOff];
  if (cutoff.empty() == true) {
    std::cerr << "cutoff is empty!" << std::endl;
    return false;
  }
  parameter->cutoff = cutoff.asUInt();
  return true;
}

bool FasttextWrapper::SetReTrain(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kReTrain) == false) {
    std::cerr << "retrain isn\'t member!" << std::endl;
    return true;
  }

  Json::Value retrain = info[kReTrain];
  if (retrain.empty() == true) {
    std::cerr << "retrain is empty!" << std::endl;
    return false;
  }
  parameter->retrain = retrain.asBool();
  return true;
}

bool FasttextWrapper::SetQNorm(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kQNorm) == false) {
    std::cerr << "qnorm isn\'t member!" << std::endl;
    return true;
  }

  Json::Value qnorm = info[kQNorm];
  if (qnorm.empty() == true) {
    std::cerr << "qnorm is empty!" << std::endl;
    return false;
  }
  parameter->qnorm = qnorm.asBool();
  return true;
}

bool FasttextWrapper::SetQOut(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kQOut) == false) {
    std::cerr << "qout isn\'t member!" << std::endl;
    return true;
  }

  Json::Value qout = info[kQOut];
  if (qout.empty() == true) {
    std::cerr << "qout is empty!" << std::endl;
    return false;
  }
  parameter->qout = qout.asBool();
  return true;
}

bool FasttextWrapper::SetDSub(const Json::Value& info, Parameter* parameter) {
  if (parameter == NULL) {
    std::cerr << "parameter is null!" << std::endl;
    return false;
  }

  if (info.isMember(kDSub) == false) {
    std::cerr << "dsub isn\'t member!" << std::endl;
    return true;
  }

  Json::Value dsub = info[kDSub];
  if (dsub.empty() == true) {
    std::cerr << "dsub is empty!" << std::endl;
    return false;
  }
  parameter->dsub = dsub.asUInt();
  return true;
}

} // namespace selvy
