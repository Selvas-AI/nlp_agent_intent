# -*- coding: utf-8-*-
import argparse
import sys

from flask import Flask
from flask_restful import Api

sys.path.append("./fasttext")

app = Flask(__name__)
api = Api(app)
app.config['JSON_AS_ASCII'] = False


def add_module(file, class_name, method_name):
    try:
        api.add_resource(getattr(__import__(file), class_name), method_name)
    except Exception as e:
        print(str(e))
        pass


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int, default=9000)
    conf = parser.parse_args()
    add_module('example', 'Example', '/intent/example')
    add_module('examples', 'Examples', '/intent/examples')
    add_module('train', 'Train', '/intent/train')
    add_module('predict', 'Predict', '/intent/predict')
    app.run(host='0.0.0.0', port=conf.port, debug=False, threaded=False)
