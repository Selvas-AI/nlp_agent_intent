# FasttextWrapper

## Requirements

**jsoncpp** 1.8.3 버전 사용

## Engine

### Input Parameter format
#### Train, Quantize 함수에서 사용한다.
#### 파라미터 값은 [fasttext README 파일](src/fasttext/README.md)을 참조한다.
#### [fasttext README 파일](src/fasttext/README.md) 에서 input은 아래 json 에서 input_path
#### [fasttext README 파일](src/fasttext/README.md) 에서 output은 아래 json 에서 output_path
#### 아래 json에서 model은 `skipgram`, `cbow`, `supervised`만 가능하다.
``` json
{
  "input_path" : string,
  "output_path" : string,
  "verbose" : int,
  "dictionary" : {
    "minCount" : int,
    "minCountLabel" : int,
    "wordNgrams" : int,
    "bucket" : int,
    "minn" : int,
    "maxn" : int,
    "t" : double,
    "label" : string
  },
  "training" : {
    "model" : string,
    "lr" : double,
    "lrUpdateRate" : int,
    "dim" : int,
    "ws" : int,
    "epoch" : int,
    "neg" : int,
    "loss" : string,
    "thread" : int,
    "pretrainedVectors" : string,
    "saveOutput" : int
  },
  "quantization" : {
    "cutoff" : unsigned int,
    "retrain" : bool,
    "qnorm" : bool,
    "qout" : bool,
    "dsub" : unsigned int
  }
}
```

### Return Type
#### ReturnType
##### 후보와 점수에 대한 쌍의 리스트를 표현하기 위함
``` cpp
std::pair<bool, std::vector<std::pair<std::string, float> > >
```

#### VectorListReturnType
##### 후보와 벡터에 대한 쌍의 리스트를 표현하기 위함
``` cpp
std::pair<bool, std::vector<std::pair<std::string, std::vector<float> > > >
```

#### VectorReturnType
##### 후보와 벡터에 대한 쌍을 표현하기 위함
``` cpp
std::pair<bool, std::vector<float> >
```

#### StringReturnType
##### 후보를 표현하기 위함
``` cpp
std::pair<bool, std::string>
```

### FasttextWrapper API
#### LoadModel
##### 리소스를 메모리에 로딩한다.
``` cpp
bool LoadModel(char* resource path)
```

#### Train
##### fasttext 라이브러리를 이용하여 워드 임베딩 또는 텍스트 분류를 학습한다.
``` cpp
bool Train(char* parameter)
```

#### Analogies
##### A - B + C 의 결과에 대한 k개의 후보 단어를 반환한다.
``` cpp
ReturnType Analogies(ink k, char* A, char* B, char* C)
```

#### NN
##### query 에 가까운 k개의 후보 단어를 반환한다.
``` cpp
ReturnType NN(ink k, char* query)
```

#### GetNgrams
``` cpp
VectorListReturnType GetNgrams(char* word)
```

#### GetSentenceVector
##### query 에 대한 sentence vector를 반환한다.
``` cpp
VectorReturnType GetSentenceVector(char* query)
```

#### GetWordVectors
##### query 에 대한 word vector를 반환한다.
``` cpp
VectorReturnType GetWordVectors(char* query)
```

#### Predict
##### query 에 대한 텍스트 분류 결과를 반환한다.
``` cpp
ReturnType Predict(int k, char* query)
```

#### Test
##### file path 에 대한 텍스트 분류 결과를 반환한다.
``` cpp
StringReturnType Test(int k, char* file path)
```

#### Quantize
``` cpp
bool Quantize(char* parameter)
```
