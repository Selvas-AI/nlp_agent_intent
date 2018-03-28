# -*- coding: utf-8-*-

from flask import request, make_response, jsonify
from flask_restful import Resource, reqparse
from jsonschema import validate

import variable
from variable import EXAMPLES_REPO, EXAMPLES_SCHEMA


class Examples(Resource):
    def __init__(self):
        self.req_parse = reqparse.RequestParser()
        self.req_parse.add_argument('count', type=int, default=999999999)
        super(Examples, self).__init__()

    def get(self):
        args = self.req_parse.parse_args(strict=True)
        output = []

        variable.GLOBAL_LOCK.acquire()
        for index, example in EXAMPLES_REPO.items():
            example["EXAMPLE_ID"] = index
            output.append(example)
            if len(output) > args['count']:
                break
        variable.GLOBAL_LOCK.release()

        return jsonify(output)

    def post(self):
        examples = request.get_json(force=True)
        try:
            validate(examples, EXAMPLES_SCHEMA)
        except Exception:
            return make_response("invalid examples format", 400)

        utterances = examples['utterances']

        variable.GLOBAL_LOCK.acquire()
        for example in utterances:
            EXAMPLES_REPO[variable.EXAMPLE_ID] = example
            variable.EXAMPLE_ID += 1
        variable.TRAIN_STATUS = 0
        variable.GLOBAL_LOCK.release()

        return make_response("added %d items" % len(utterances), 200)
