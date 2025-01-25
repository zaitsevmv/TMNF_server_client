from flask import Response, abort, current_app, redirect, render_template, request, url_for
from flask_login import current_user, login_required, login_user, logout_user

from . import db, login_manager
from .model import Admin, Server

@current_app.route("/servers")
def servers_view():
    servers: Server = None
    if not current_user.is_authenticated:
        servers = Server.query.with_entities(Server.name, Server.login, Server.max_players, Server.status).all()
    else:
        servers = Server.query.all()
    return render_template("servers_view.html", servers=servers, authorized=current_user.is_authenticated)


@current_app.route("/sign-in")
def sign_in():
    return render_template("sign_in.html")


