# -*- coding: utf-8-*-
import argparse

import codecs
import copy
from flask import Flask, Response, abort, request
import json
from model.fasttext import FasttextWrapper
import os
import threading

app = Flask(__name__)


# global variables
global_lock = threading.Lock()

APP_DATA = dict() # key : app name, value : app index
INDENT_DATA = dict() # key : app index, value : dict for key : indent, value : indent index
EXAMPLE_DATA = dict() # key : app index, value : dict for key : indent index, value : dict for key : example, value : example index
MODEL_DATA = dict()

APP_NAME = "Name"
EXAMPLE_TEXT = "ExampleText"
INTENT_NAME = "IntentName"
UTTERANCES = "utterances"
SKIP = "skip"
COUNT = "count"
FASTTEXT_MODEL = "fasttext"
MODEL = "model"
EXAMPLE = "example"

INPUT_PATH = "input_path"
OUTPUT_PATH = "output_path"
TRAINING = "training"
PRETRAINED_VECTORS = "pretrainedVectors"
SKIPGRAM = "skipgram"
CBOW = "cbow"
SUPERVISED = "supervised"


LAST_APP_ID = 0
LAST_INDENT_ID = 0
LAST_EXAMPLE_ID = 0
LAST_MODEL_ID = 0


def get_app_name(id):
    global_lock.acquire()
    ids = APP_DATA.values()
    index = 0
    for value in ids:
        if value == id:
            break
        index += 1

    if index == len(APP_DATA):
        return False, ""

    key = list(APP_DATA.keys())[index]
    global_lock.release()
    return True, key


def reset_file(folder, file):
    folder_path = "./" + folder + "/"
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)

    file_path = folder_path + file + ".txt"
    if os.path.exists(file_path):
        os.remove(file_path)
    return file_path


# Version 1.0 API
@app.route("/api/v1.0/apps", methods=["POST"])
def create_app():
    global LAST_APP_ID
    global LAST_MODEL_ID

    global_lock.acquire()
    try:
        json_data = request.get_json(force=True)

        if not APP_NAME in json_data:
            abort(400)

        name = json_data[APP_NAME]
        if name in APP_DATA.keys():
            abort(400)

        APP_DATA[name] = LAST_APP_ID
        INDENT_DATA[LAST_APP_ID] = dict()
        EXAMPLE_DATA[LAST_APP_ID] = dict()
        EXAMPLE_DATA[LAST_APP_ID][-1] = dict()
        MODEL_DATA[LAST_APP_ID] = dict()
        MODEL_DATA[LAST_APP_ID][FASTTEXT_MODEL] = (LAST_MODEL_ID, FasttextWrapper())
        LAST_MODEL_ID += 1
        LAST_APP_ID += 1

        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response(str(LAST_APP_ID - 1))


@app.route("/api/v1.0/apps/<int:id>", methods=["DELETE"])
def delete_app(id):
    try:
        result, key = get_app_name(id)
        if not result:
            abort(400)

        global_lock.acquire()
        del EXAMPLE_DATA[id]
        del MODEL_DATA[id]
        del INDENT_DATA[id]
        del APP_DATA[key]
        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>", methods=["GET"])
def get_app(id):
    # @todo(andrew) 앱 정보 리턴하는 함수 구현
    return Response("Success")


@app.route("/api/v1.0/apps", methods=["GET"])
def get_app_list():
    global_lock.acquire()
    try:
        response = json.dumps(APP_DATA)
        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response(response)


@app.route("/api/v1.0/apps/<int:id>/export", methods=["GET"])
def export(id):
    # @todo(andrew) 앱 데이터 리턴하는 함수 구현
    return Response("success")


@app.route("/api/v1.0/apps/import", methods=["POST"])
def import_app():
    name = request.args.get(APP_NAME, None)
    json_data = request.get_json(force=True)
    # @todo(andrew) 앱 데이터를 임포트하는 함수 구현
    # app id 리턴
    return Response("success")


@app.route("/api/v1.0/apps/<int:id>/example", methods=["POST"])
def add_example(id):
    global LAST_INDENT_ID
    global LAST_EXAMPLE_ID

    global_lock.acquire()
    try:
        json_data = request.get_json(force=True)

        if not EXAMPLE_TEXT in json_data:
            abort(400)

        if not id in INDENT_DATA.keys():
            abort(400)

        example = json_data[EXAMPLE_TEXT]

        intent_index = -1
        if INTENT_NAME in json_data:
            intent_name = json_data[INTENT_NAME]

            if not intent_name in INDENT_DATA[id].keys():
                INDENT_DATA[id][intent_name] = LAST_INDENT_ID
                EXAMPLE_DATA[id][LAST_INDENT_ID] = dict()
                LAST_INDENT_ID += 1

            intent_index = INDENT_DATA[id][intent_name]

        EXAMPLE_DATA[id][intent_index][example] = LAST_EXAMPLE_ID
        LAST_EXAMPLE_ID += 1

        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response(str(LAST_EXAMPLE_ID - 1))


@app.route("/api/v1.0/apps/<int:id>/examples", methods=["POST"])
def add_examples(id):
    global LAST_INDENT_ID
    global LAST_EXAMPLE_ID

    global_lock.acquire()
    try:
        json_data = request.get_json(force=True)
        if not UTTERANCES in json_data:
            abort(400)

        utterances = json_data[UTTERANCES]
        for utterance in utterances:
            if not EXAMPLE_TEXT in utterance:
                abort(400)

            if not id in INDENT_DATA.keys():
                abort(400)

            example = utterance[EXAMPLE_TEXT]

            intent_index = -1
            if INTENT_NAME in utterance:
                intent_name = utterance[INTENT_NAME]

                if not intent_name in INDENT_DATA[id].keys():
                    INDENT_DATA[id][intent_name] = LAST_INDENT_ID
                    EXAMPLE_DATA[id][LAST_INDENT_ID] = dict()
                    LAST_INDENT_ID += 1

                intent_index = INDENT_DATA[id][intent_name]

            EXAMPLE_DATA[id][intent_index][example] = LAST_EXAMPLE_ID
            LAST_EXAMPLE_ID += 1

        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/examples/<int:example>", methods=["DELETE"])
def delete_examples(id, example):
    global_lock.acquire()
    try:
        if not id in INDENT_DATA.keys():
            abort(400)

        for intent in EXAMPLE_DATA[id].keys():
            examples = EXAMPLE_DATA[id][intent].values()
            index = 0
            for value in examples:
                if value == example:
                    break
                index += 1

            if index < len(EXAMPLE_DATA[id][intent]):
                key = list(EXAMPLE_DATA[id][intent].keys())[index]
                del EXAMPLE_DATA[id][intent][key]
                break

        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/examples", methods=["GET"])
def get_examples(id):
    skip = request.args.get(SKIP, None)
    count = request.args.get(COUNT, None)
    # @todo(andrew) examples를 반환 하는 함수 구현
    return Response("success")


@app.route("/api/v1.0/apps/<int:id>/intents", methods=["POST"])
def create_intents(id):
    global LAST_MODEL_ID

    global_lock.acquire()
    try:
        json_data = request.get_json(force=True)

        if not MODEL in json_data:
            abort(400)

        model = json_data[MODEL]

        # @todo(andrew) 모델에 대한 추가 파라미터 받기

        if model in MODEL_DATA[id].keys():
            abort(400)

        # @todo(andrew) 지원 모델에 대한 예외 처리로 수정
        if model != FASTTEXT_MODEL:
            abort(400)

        MODEL_DATA[id][model] = (LAST_MODEL_ID, FasttextWrapper())
        LAST_MODEL_ID += 1
        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response(str(LAST_MODEL_ID - 1))


@app.route("/api/v1.0/apps/<int:id>/intents/<int:intent_id>", methods=["DELETE"])
def delete_intents(id, intent_id):
    global_lock.acquire()
    try:
        if not id in MODEL_DATA.keys():
            abort(400)

        ids = MODEL_DATA[id].values()
        index = 0
        for value in ids:
            if value[0] == intent_id:
                break
            index += 1

        if index == len(MODEL_DATA[id]):
            abort(400)

        key = list(MODEL_DATA[id].keys())[index]
        del MODEL_DATA[id][key]

        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)

    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/intents/<int:intent_id>", methods=["GET"])
def get_intents(id, intent_id):
    # @todo(andrew) intent 모델 정보 리턴하는 함수 구현
    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/intents", methods=["GET"])
def get_intents_list(id):
    # @todo(andrew) intent 모델들 정보 리턴하는 함수 구현
    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/predict", methods=["GET"])
def prediction(id):
    example = request.args.get(EXAMPLE, None)
    # @todo(andrew) fasttext 이외의 모델에서도 동작하도록 수정
    if not id in MODEL_DATA:
        abort(400)

    success, app_name = get_app_name(id)
    if not success:
        abort(400)

    model = MODEL_DATA[id][FASTTEXT_MODEL][1]
    result = model.Predict(app_name, 1, example)
    if not result.first:
        abort(400)

    # @todo(andrew) 결과 확인

    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/train", methods=["POST"])
def train(id):
    # @todo(andrew) fasttext 이외의 모델에서도 동작하도록 수정
    if not id in MODEL_DATA:
        abort(400)

    success, app_name = get_app_name(id)
    if not success:
        abort(400)

    model = MODEL_DATA[id][FASTTEXT_MODEL][1]

    global_lock.acquire()
    try:
        intents = INDENT_DATA[id]

        input_file_path = reset_file("temp", app_name)

        with codecs.open(input_file_path, 'w', encoding='utf8') as f:
            for key, value in intents.items():
                label = "__label__" + str(value)
                examples = EXAMPLE_DATA[id][value]
                for example in examples.keys():
                    line = label + " , " + example + "\n"
                    f.write(line)

        output_file_path = reset_file("resource", app_name)

        json_data = request.get_json(force=True)
        parameter = copy.deepcopy(json_data)
        parameter[INPUT_PATH] = input_file_path
        parameter[OUTPUT_PATH] = output_file_path
        result = model.Train(app_name, json.dumps(parameter))
        global_lock.release()
    except Exception as e:
        global_lock.release()
        abort(400)


    return Response("Success")


@app.route("/api/v1.0/apps/<int:id>/train", methods=["GET"])
def get_train_state(id):
    # @todo(andrew) 학습 상태 확인
    return Response("Success")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=str, default='8000')
    conf = parser.parse_args()

    app.run(host='0.0.0.0', port=conf.port, debug=False, threaded=True)
