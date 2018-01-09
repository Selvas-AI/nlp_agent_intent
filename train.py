# -*- coding: utf-8-*-
import json
import variable

from fasttext_default import make_parameter, export, INTENT_MODEL
from flask import make_response, request
from flask_restful import Resource


class Train(Resource):
    def get(self):
        if variable.TRAIN_STATUS == 0:
            return make_response("Not yet trained.", 200)
        else:
            return make_response("Aleady trained.", 200)

    def post(self):
        if variable.TRAIN_STATUS == 1:
            return make_response("Aleady trained.", 200)

        variable.TRAIN_STATUS = 0
        export()
        request_data = request.get_json(force=True)
        parameter = make_parameter(request_data)
        result = INTENT_MODEL.Train(json.dumps(parameter))
        if not result:
            return make_response("Train fail.", 400)
        variable.TRAIN_STATUS = 1
        return make_response("Train complete.", 200)
