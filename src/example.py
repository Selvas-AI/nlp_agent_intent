# -*- coding: utf-8-*-

from flask import request, make_response, jsonify
from flask_restful import Resource, reqparse
from jsonschema import validate

import variable
from variable import EXAMPLES_REPO, EXAMPLE_SCHEMA, EXAMPLE_ID_STRING


class Example(Resource):
    def __init__(self):
        self.req_parse = reqparse.RequestParser()
        self.req_parse.add_argument(EXAMPLE_ID_STRING, type=int, required=True)
        super(Example, self).__init__()

    def delete(self):
        args = self.req_parse.parse_args(strict=True)

        variable.GLOBAL_LOCK.acquire()
        try:
            del EXAMPLES_REPO[args[EXAMPLE_ID_STRING]]
        except Exception:
            return make_response("invalid example id", 400)
        variable.GLOBAL_LOCK.release()

        return make_response("[%d] example was deleted" % args[EXAMPLE_ID_STRING], 200)

    def get(self):
        args = self.req_parse.parse_args(strict=True)
        try:
            return jsonify(EXAMPLES_REPO[args[EXAMPLE_ID_STRING]])
        except Exception:
            return make_response("invalid example id", 400)

    def post(self):
        example = request.get_json(force=True)
        try:
            validate(example, EXAMPLE_SCHEMA)
        except Exception:
            return make_response("invalid example format", 400)

        variable.GLOBAL_LOCK.acquire()
        EXAMPLES_REPO[variable.EXAMPLE_ID] = example
        variable.EXAMPLE_ID += 1
        variable.TRAIN_STATUS = 0
        variable.GLOBAL_LOCK.release()

        return make_response("added item", 200)
