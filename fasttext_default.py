# -*- coding: utf-8-*-
import copy
import os
import variable

from fasttext import FasttextWrapper

from variable import INTENT_ROOT_PATH, INTENT_MODEL, EXAMPLES_REPO

FASTTEXT_ROOT_PATH = os.path.join(INTENT_ROOT_PATH, 'fasttext')
if not os.path.exists(FASTTEXT_ROOT_PATH):
    os.makedirs(FASTTEXT_ROOT_PATH)

FASTTEXT_MODEL_PATH = os.path.join(FASTTEXT_ROOT_PATH, 'intent')
FASTTEXT_TRAIN_DATA_PATH = os.path.join(FASTTEXT_ROOT_PATH, 'intent.train')

INTENT_MODEL = FasttextWrapper()
if os.path.exists(FASTTEXT_MODEL_PATH + '.bin'):
    INTENT_MODEL.LoadModel(FASTTEXT_MODEL_PATH + '.bin')
    variable.TRAIN_STATUS = 1

DICTIONARY_SCHEMA = {
    "minCount": {"type": "integer"},
    "minCountLabel": {"type": "integer"},
    "wordNgrams": {"type": "integer"},
    "bucket": {"type": "integer"},
    "minn": {"type": "integer"},
    "maxn": {"type": "integer"},
    "t": {"type": "number"},
    "label": {"type": "string"}
}

TRAINING_SCHEMA = {
    "model": {"type": "string"},
    "lr": {"type": "number"},
    "lrUpdateRate": {"type": "integer"},
    "dim": {"type": "integer"},
    "ws": {"type": "integer"},
    "epoch": {"type": "integer"},
    "neg": {"type": "integer"},
    "loss": {"type": "string"},
    "thread": {"type": "integer"},
    "pretrainedVectors": {"type": "string"},
    "saveOutput": {"type": "integer"},
}

QUANTIZATION_SCHEMA = {
    "cutoff": {"type": "integer"},
    "retrain": {"type": "boolean"},
    "qnorm": {"type": "boolean"},
    "qout": {"type": "boolean"},
    "dsub": {"type": "integer"},
}

FASTTEXT_PARAMETER_SCHEMA = {
    "input_path": {"type": "string"},
    "output_path": {"type": "string"},
    "verbose": {"type": "integer"},
    "dictionary": DICTIONARY_SCHEMA,
    "training": TRAINING_SCHEMA,
    "quantization": QUANTIZATION_SCHEMA
}

# 환경 변수
INPUT_PATH = "input_path"
OUTPUT_PATH = "output_path"
DICTIONARY = "dictionary"
MIN_COUNT = "minCount"
MIN_N = "minn"
MAX_N = "maxn"
TRAINING = "training"
LR = "lr"
LOSS = "loss"
SOFTMAX = "softmax"
MODEL = "model"
SUPERVISED = "supervised"


def export():
    with open(FASTTEXT_TRAIN_DATA_PATH, 'w', encoding='utf8') as file:
        for example in EXAMPLES_REPO.values():
            intent_name = example['IntentName']
            example_text = example['ExampleText']
            line = "__label__" + intent_name + " , " + example_text + "\n"
            file.write(line)


def make_parameter(request_data):
    parameter = copy.deepcopy(request_data)
    parameter[INPUT_PATH] = FASTTEXT_TRAIN_DATA_PATH
    parameter[OUTPUT_PATH] = FASTTEXT_MODEL_PATH

    if DICTIONARY not in parameter.keys():
        parameter[DICTIONARY] = dict()
        parameter[DICTIONARY][MIN_COUNT] = 1
        parameter[DICTIONARY][MIN_N] = 0
        parameter[DICTIONARY][MAX_N] = 0

    if TRAINING not in parameter.keys():
        parameter[TRAINING] = dict()
        parameter[TRAINING][MODEL] = SUPERVISED
        parameter[TRAINING][LR] = 0.1
        parameter[TRAINING][LOSS] = SOFTMAX
    return parameter
