import click
from flask import current_app

from . import bcrypt, db
from .model import Admin, Server, ServerStatus
from .server_status import ServerConfig, ParseStatus


@current_app.cli.command("clear_info")
def force_info_clear():
    db.drop_all()
    db.create_all()


@current_app.cli.command("add_admin")
@click.argument("name")
@click.argument("pwd")
def add_admin(name, pwd):
    password_hash = bcrypt.generate_password_hash(pwd)
    new_user = Admin(login=name, password_hash=password_hash)
    db.session.add(new_user)
    db.session.commit()


@current_app.cli.command("add_server")
@click.argument("path")
def add_admin(path):
    server = ServerConfig(path)
    if server.status != ParseStatus.SUCCESS:
        return 
    
    new_server = Server(login=server.login, 
                        name=server.name, 
                        ip=server.ip, 
                        status=server.server_status.value,
                        current_map_uid=server.current_map.uid,
                        current_map_name=server.current_map.name,
                        current_players=server.current_players,
                        max_players=server.max_players,
                        gamemode=server.current_gamemode.value,
                        server_port=server.server_port,
                        p2p_port=server.p2p_port,
                        rpc_port=server.rpc_port)
    
    db.session.add(new_server)
    db.session.commit()