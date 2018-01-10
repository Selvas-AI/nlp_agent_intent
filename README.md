# intent

## Requirements
```commandline
conda env create -f environment.yml
```

## Engine

### [FasttextWrapper](FasttextWrapper.md)

## Definition
### Example
* 하나의 입력 문장과 정답을 가지고 있는 단위이다.
* 단, 정답은 없을 수 있다.

## API
### [API](doc/API.md)

## Todo
* 학습시간이 오래 걸리므로 학습 프로세스는 별도의 프로세스로 동작하도록 수정한다.
* oktpy 안의 트위터 형태소 분석기를 사용하여 아래 리소스를 생성한다.
  * 워드 임베딩
  * 의도 분류 모델 
* Log를 상세하게 작성한다.
* 학습 파라미터를 확인하고 용량이 큰 이유를 확인한다.