from flask import Flask

from .views import blueprint


def create_app():
    """Factory for creating app instances."""

    app = Flask(__name__)
    app.config.from_pyfile('config.py')
    app.register_blueprint(blueprint)

    return app
