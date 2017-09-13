# intent

## Requirements
```commandline
conda env create -f environment.yml
```

## Engine

### [FasttextWrapper](FasttextWrapper.md)

## Definition
### App
* 하나의 bot을 의미 한다.
* 예 : Autron bot, Gangnam bot
* 하나의 App 에서 어떤 Module을 사용 할지 선택 할 수 있다.

### Module
* 의도 분류에 사용되는 엔진을 의미 한다.
* 예 : fasttext
* 학습에 사용된 파라미터 등에 따라 다양한 Resource 파일을 생성 할 수 있다.
* 실제 서비스에서는 선택한 Resource 파일의 결과를 출력한다.

### Resource
* 학습 시점, 사용된 파라미터 등에 따라 다른 Resource 파일을 생성한다.
* 리소스 버전 규칙은 Module-Major.Minor.Patch 로 한다.
* 예 : fasttext.1.0.0.bin

### Example
* 하나의 입력 문장과 정답을 가지고 있는 단위이다.
* 단, 정답은 없을 수 있다.

## Agent 내부 데이터
* APP_DATA : app 이름을 key로 app id를 value로 가진다.
* INDENT_DATA : app id을 key로 가지고 value에는 의도에 대한 정보를 dict 형태로 가지고 있다.
  * 의도에 대한 정보는 의도를 key로 의도에 대한 id를 value로 가진다.
* EXAMPLE_DATA : app id을 key로 가지고 value에는 의도에 따른 example 정보을 dict 형태로 가지고 있다.
  * 의도에 따른 example 정보는 의도에 대한 id를 key로 value에는 example에 대한 정보를 dict 형태로 가지고 있다.
    * example에 대한 정보는 입력 문장을 key로 입력 문장에 대한 id를 value로 가지고 있다.
* MODEL_DATA : app id를 key로 가지고 있고 module을 value로 가진다.    

## API
### [API](doc/API.md)

## Todo
### 2017-09-29
1. 학습시간이 오래 걸리므로 학습 프로세스는 별도의 프로세스로 동작하도록 수정한다.
2. 학습 전 데이터 전처리를 호출하도록 수정한다.
3. Linux에서 동작 확인한다.
4. Docker image 생성한다.

### 2017-10-31
1. __App__, __Module__, __Resource Version__ 버전을 관리 할 수 있는 구조로 재 설계하고 수정한다.
2. app, model, indent, example 정보를 db에 저장하고 읽는 구조로 수정한다.
3. 리소스 버저닝등 전반적인 구조에 대한 리뷰를 받는다.
4. 워드 임베딩 학습 및 테스트 관련 API를 생성한다.
5. 문자열 입력을 __std::string__ 에서 __std::istream__ 으로 변경한다.
6. fasttext에서 지원하는 __Analogies__, __NN__, __GetNgrams__, __GetSentenceVector__, __GetWordVectors__, __Test__, __Quantize__ 기능을 위한 __API__ 를 추가한다.


### 2017-11-30
1. 미 구현된 API 작성한다. (예: /api/v1.0/apps/<int:id> )
2. 기 학습된 리소스도 로딩 가능하도록 API를 생성한다.
3. 에러 메세지를 상세하게 작성한다.
4. Log를 상세하게 작성한다.
5. 용어 정의 및 문서를 통일 되게 정리한다.

### 미정
1. 기존에 작성된 API는 [LUIS APIs V1.0](https://dev.projectoxford.ai/docs/services/56d95961e597ed0f04b76e58/operations/56f8a55119845511c81de462)을 참고 하였습니다.
[LUIS APIs V2.0](https://westus.dev.cognitive.microsoft.com/docs/services/5890b47c39e2bb17b84a55ff/operations/5890b47c39e2bb052c5b9c45) 을 참고 하여 수정한다.
