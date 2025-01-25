from flask import Flask, redirect, url_for
from flask_bcrypt import Bcrypt
from flask_login import LoginManager
from flask_sqlalchemy import SQLAlchemy

from sqlalchemy import MetaData
from sqlalchemy.orm import DeclarativeBase


metadata = MetaData(
    naming_convention={
        "ix": "ix_%(column_0_label)s",
        "uq": "uq_%(table_name)s_%(column_0_name)s",
        "ck": "ck_%(table_name)s_%(constraint_name)s",
        "fk": "fk_%(table_name)s_%(column_0_name)s_%(referred_table_name)s",
        "pk": "pk_%(table_name)s",
    }
)


class Base(DeclarativeBase):
    metadata = metadata


db = SQLAlchemy(model_class=Base)
login_manager = LoginManager()
bcrypt = Bcrypt()


def create_app():
    app = Flask(__name__)
    app.config.from_mapping(
        SECRET_KEY="dev",
        SQLALCHEMY_DATABASE_URI="sqlite:///db.sqlite",
    )

    db.init_app(app)
    with app.app_context():
        from . import view, cli, model

    @app.route('/')
    def start_redirect():
        return redirect(url_for('servers_view'))
    
    @login_manager.user_loader
    def load_user(user_id):
        return model.Admin.query.get(int(user_id))
    
    login_manager.init_app(app)
    
    return app