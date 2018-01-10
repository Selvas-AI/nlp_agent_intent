# -*- coding: utf-8-*-
import collections
import os
import threading

EXAMPLE_SCHEMA = {
    "ExampleText": {"type": "string"},
    "IntentName": {"type": "string"},
    "required": [
        "ExampleText", "IntentName"
    ]
}

EXAMPLES_SCHEMA = {
    "utterances": {
        "type": "array",
        "items": EXAMPLE_SCHEMA
    }
}

EXAMPLE_ID_STRING = 'exampleid'

GLOBAL_LOCK = threading.Lock()

EXAMPLES_REPO = collections.OrderedDict()
EXAMPLE_ID = 0
TRAIN_STATUS = 0
MODEL_ROOT = os.getenv('MODEL_ROOT', '')
INTENT_ROOT_PATH = os.path.join(MODEL_ROOT, 'intent')

INTENT_MODEL = None

CANDIDATE_NUMBER = 5
