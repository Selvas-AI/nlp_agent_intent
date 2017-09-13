# intent

## Requirements
```commandline
conda env create -f environment.yml
```

## Engine

### [FasttextWrapper](FasttextWrapper.md)

## API

## Todo
1. 문자열 입력을 __std::string__ 에서 __std::istream__ 으로 변경한다.
2. fasttext에서 지원하는 __Analogies__, __NN__, __GetNgrams__, __GetSentenceVector__, __GetWordVectors__, __Test__, __Quantize__ 기능을 위한 __API__ 를 추가한다.
3. __App__, __Module__, __Resource Version__ 버전을 관리 할 수 있는 구조로 재 설계하고 수정한다.
4. app, model, indent, example 정보를 db에 저장하고 읽는 구조로 수정한다.
5. 기존에 작성된 API는 [LUIS APIs V1.0](https://dev.projectoxford.ai/docs/services/56d95961e597ed0f04b76e58/operations/56f8a55119845511c81de462)을 참고 하였습니다.
[LUIS APIs V2.0](https://westus.dev.cognitive.microsoft.com/docs/services/5890b47c39e2bb17b84a55ff/operations/5890b47c39e2bb052c5b9c45) 을 참고 하여 수정한다.
6. 미 구현된 API 작성한다. (예: /api/v1.0/apps/<int:id> )
7. 학습시간이 오래 걸리므로 학습 프로세스는 별도의 프로세스로 동작하도록 수정한다.
8. 에러 메세지를 상세하게 작성한다.
9. Log를 상세하게 작성한다.
10. Linux에서 동작 확인한다.
11. Docker image 생성한다.
12. 기 학습된 리소스도 로딩 가능하도록 API를 생성한다.