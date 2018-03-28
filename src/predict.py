# -*- coding: utf-8-*-
import json

from flask import make_response
from flask_restful import Resource, reqparse

import variable
from fasttext_default import INTENT_MODEL
from variable import CANDIDATE_NUMBER, preprocessing


class Predict(Resource):
    def __init__(self):
        self.req_parse = reqparse.RequestParser()
        self.req_parse.add_argument('example', type=str, required=True)
        super(Predict, self).__init__()

    def get(self):
        if variable.TRAIN_STATUS == 0:
            return make_response("Not yet trained.", 200)

        args = self.req_parse.parse_args(strict=True)
        example_text = args['example']
        example = preprocessing(example_text)

        variable.GLOBAL_LOCK.acquire()
        result = INTENT_MODEL.Predict(CANDIDATE_NUMBER, example)
        variable.GLOBAL_LOCK.release()

        if not result.first:
            return make_response("Predict fail.", 400)

        outputs = result.second
        results = dict()
        for output in outputs:
            label = output[0]
            results[label[9:]] = output[1]

        return make_response(json.dumps(results, ensure_ascii=False), 200)
