# -*- coding: utf-8-*-
import argparse

from flask_restful import Api
from svlog import FlaskWithLog

from example import Example
from examples import Examples
from predict import Predict
from train import Train
from variable import PORT

app = FlaskWithLog(__name__)
api = Api(app)
app.config['JSON_AS_ASCII'] = False

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=str, default=PORT)
    conf = parser.parse_args()
    PORT = conf.port

    conf = parser.parse_args()
    api.add_resource(Example, '/example')
    api.add_resource(Examples, '/examples')
    api.add_resource(Train, '/train')
    api.add_resource(Predict, '/predict')
    app.run(host='0.0.0.0', port=PORT, debug=False, threaded=False)
