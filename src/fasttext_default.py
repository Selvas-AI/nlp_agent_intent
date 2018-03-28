# -*- coding: utf-8-*-

import copy
import datetime
import json
import os

import variable
from fasttext import FasttextWrapper
from variable import INTENT_ROOT_PATH, EXAMPLES_REPO, preprocessing


FASTTEXT_ROOT_PATH = os.path.join(INTENT_ROOT_PATH, 'fasttext')
WORD_EMBEDDING_ROOT_PATH = os.path.join(FASTTEXT_ROOT_PATH, 'word_vectors')
if not os.path.exists(FASTTEXT_ROOT_PATH):
    os.makedirs(FASTTEXT_ROOT_PATH)

MODEL_VERSION = '180110_0'

FASTTEXT_MODEL_PATH = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + MODEL_VERSION)
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


def export(model_version):
    train_data_path = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + model_version + '.train')
    with open(train_data_path, 'w', encoding='utf8') as file:
        for example in EXAMPLES_REPO.values():
            intent_name = example['IntentName']
            example_text = example['ExampleText']
            example = preprocessing(example_text)
            line = "__label__" + intent_name + " , " + example + "\n"
            file.write(line)


def make_parameter(request_data, model_version):
    parameter = copy.deepcopy(request_data)
    train_data_path = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + model_version + '.train')
    parameter['input_path'] = train_data_path
    parameter['output_path'] = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + model_version)

    if 'dictionary' not in parameter.keys():
        parameter['dictionary'] = dict()
        parameter['dictionary']['minCount'] = 1
        parameter['dictionary']['minn'] = 0
        parameter['dictionary']['maxn'] = 0

    if 'training' not in parameter.keys():
        parameter['training'] = dict()
        parameter['training']['model'] = 'supervised'
        parameter['training']['lr'] = 0.1
        parameter['training']['loss'] = 'softmax'

    if 'pretrainedVectors' in parameter['training'].keys():
        parameter['training']['pretrainedVectors'] = os.path.join(WORD_EMBEDDING_ROOT_PATH,
                                                                  parameter['training']['pretrainedVectors'])
    return parameter


def print_parameter(parameter, model_version):
    file_path = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + model_version + '.yml')
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write('maker: intent agent\n')
        file.write('code:\n')
        file.write('  - origin: \'http://gitlab.selvasai.com/nlp/agent/intent.git\'\n')
        file.write('  - revision: \'None\'\n')
        file.write('corpus:\n')
        file.write('  - data: ' + parameter['input_path'] + '\n')
        if ('training' in parameter) and ('pretrainedVectors' in parameter['training']):
            word_embedding = os.path.split(parameter['training']['pretrainedVectors'])[1]
            file_name = os.path.splitext(word_embedding)[0]
            infos = file_name.split('_')
            file.write('word_embedding:\n')
            file.write('- model: ' + infos[0] + '\n')
            file.write('- version: ' + infos[1] + '\n')
        file.write('parameter: ' + json.dumps(parameter) + '\n')
        file.write('version: ' + model_version + '\n')


def create_model_version():
    today = datetime.date.today().strftime('%y%m%d')
    for index in range(100):
        model_version = today + '_' + str(index)
        model_path = os.path.join(FASTTEXT_ROOT_PATH, 'intent' + '_' + model_version)
        if not os.path.exists(model_path + '.bin'):
            return model_version
    return ''
