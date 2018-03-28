# -*- coding: utf-8-*-
import json

from flask import make_response, request
from flask_restful import Resource

import variable
from fasttext_default import make_parameter, export, INTENT_MODEL, create_model_version, print_parameter


class Train(Resource):
    def get(self):
        if variable.TRAIN_STATUS == 0:
            return make_response("Not yet trained.", 200)
        else:
            return make_response("Aleady trained.", 200)

    def post(self):
        request_data = request.get_json(force=True)

        variable.GLOBAL_LOCK.acquire()
        variable.TRAIN_STATUS = 0
        model_version = create_model_version()
        export(model_version)
        parameter = make_parameter(request_data, model_version)
        result = INTENT_MODEL.Train(json.dumps(parameter))
        if not result:
            return make_response("Train fail.", 400)
        print_parameter(parameter, model_version)
        variable.TRAIN_STATUS = 1
        variable.GLOBAL_LOCK.release()

        return make_response("Train complete.", 200)
