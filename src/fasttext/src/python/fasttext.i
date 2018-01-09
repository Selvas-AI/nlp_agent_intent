/* fasttext.i */
%module fasttext

%{
#include "fasttext_wrapper.h"
%} 

// Parse the original header file
%include "fasttext_wrapper.h"

%include "stl.i"
%template(result) std::pair<std::string, float >;
%template(realVectorResult) std::pair<std::string, std::vector<float> >;

%template(returnType) std::pair<bool, std::vector<std::pair<std::string, float > > >;
%template(vectorListReturnType) std::pair<bool, std::vector<std::pair<std::string, std::vector<float> > > >;
%template(vectorReturnType) std::pair<bool, std::vector<float > >;
%template(stringReturnType) std::pair<bool, std::string >;

%template(resultList) std::vector<std::pair<std::string, float > >;
%template(realVector) std::vector<float>;
%template(vectorResultList) std::vector<std::pair<std::string, std::vector<float> > >;
