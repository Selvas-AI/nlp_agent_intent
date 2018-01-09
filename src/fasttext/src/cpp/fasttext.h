/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FASTTEXT_FASTTEXT_H
#define FASTTEXT_FASTTEXT_H

#define FASTTEXT_VERSION 11 /* Version 1a */
#define FASTTEXT_FILEFORMAT_MAGIC_INT32 793712314

#include <time.h>

#include <atomic>
#include <memory>
#include <set>
#include <queue>

#include "args.h"
#include "dictionary.h"
#include "matrix.h"
#include "qmatrix.h"
#include "model.h"
#include "real.h"
#include "utils.h"
#include "vector.h"

namespace fasttext {

class FastText {
public:
  typedef std::pair<real, std::string> Candidate;
  typedef std::priority_queue<Candidate> Heap;
  typedef std::pair<std::string, real> Result;
  typedef std::vector<Result> ResultList;
  typedef std::vector<real> RealVector;
  typedef std::pair<std::string, RealVector> RealVectorResult;
  typedef std::vector<RealVectorResult> RealVectorResultList;

  private:
    std::shared_ptr<Args> args_;
    std::shared_ptr<Dictionary> dict_;

    std::shared_ptr<Matrix> input_;
    std::shared_ptr<Matrix> output_;

    std::shared_ptr<QMatrix> qinput_;
    std::shared_ptr<QMatrix> qoutput_;

    std::shared_ptr<Model> model_;

    std::atomic<int64_t> tokenCount;
    clock_t start;
    void signModel(std::ostream&);
    bool checkModel(std::istream&);

    bool quant_;

  public:
    FastText();

    void getVector(Vector&, const std::string&) const;
    bool saveVectors();
    bool saveOutput();
    bool saveModel();
    void loadModel(std::istream&);
    bool loadModel(const std::string&);
    void printInfo(real, real);

    void supervised(Model&, real, const std::vector<int32_t>&,
                    const std::vector<int32_t>&);
    void cbow(Model&, real, const std::vector<int32_t>&);
    void skipgram(Model&, real, const std::vector<int32_t>&);
    std::vector<int32_t> selectEmbeddings(int32_t) const;
    bool quantize(std::shared_ptr<Args>);
    void test(std::istream&, int32_t);
    bool test(const std::string& path, const int k, int* n, real* precision, 
              real* recall);
    void predict(std::istream&, int32_t, bool);
    void predict(
        std::istream&,
        int32_t,
        std::vector<std::pair<real, std::string>>&) const;
    bool predict(const int32_t k, std::istream& query,
                 ResultList* predictions) const;
    void wordVectors();
    bool wordVectors(const std::string& query, RealVector* result);
    void sentenceVectors();
    bool sentenceVectors(std::istream& query, RealVector* result);
    void ngramVectors(std::string);
    bool ngramVectors(const std::string& query, RealVectorResultList* results);
    void textVectors();
    bool textVectors(std::istream& query, RealVector* result);
    void printWordVectors();
    bool printWordVectors(const std::string& query, RealVector* result);
    void printSentenceVectors();
    bool printSentenceVectors(std::istream& query, RealVector* result);
    void precomputeWordVectors(Matrix&);
    void findNN(const Matrix&, const Vector&, int32_t,
                const std::set<std::string>&);

    bool findNN(const Matrix& wordVectors, const Vector& queryVec, 
                const int32_t k, const std::set<std::string>& banSet, 
                ResultList* D);

    void nn(int32_t);
    bool nn(const int32_t k, const std::string& queryWord, ResultList* D);
    void analogies(int32_t);
    bool analogies(const int32_t k, const std::string& A, const std::string& B, 
                   const std::string& C, ResultList* D);
    void trainThread(int32_t);
    bool train(std::shared_ptr<Args>);

    bool loadVectors(std::string);
    int getDimension() const;
};

}
#endif
