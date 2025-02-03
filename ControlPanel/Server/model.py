import enum

from flask_login import UserMixin
from sqlalchemy import Date, ForeignKey, Integer, String
from sqlalchemy.orm import Mapped, mapped_column, relationship

from . import db


class Admin(db.Model, UserMixin):
    __tablename__ = "users"
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    login: Mapped[str] = mapped_column(String(255), unique=True)
    password_hash: Mapped[str] = mapped_column(String(255))


class ServerStatus(enum.Enum):
    STATUS_WORKING = 1
    STATUS_SLEEPING = 2
    STATUS_REBOOTING = 3


class GameMode(enum.Enum):
    MODE_TIMEATTACK = 1
    MODE_ROUNDS = 2
    MODE_OTHER = 1 << 31


class Server(db.Model):
    __tablename__ = "servers"
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    login: Mapped[str] = mapped_column(String(255), unique=True)
    name: Mapped[str] = mapped_column(String(255))
    status: Mapped[int] = mapped_column(Integer)
    current_map_uid: Mapped[str] = mapped_column(String(255))
    current_map_name: Mapped[str] = mapped_column(String(255))
    current_players: Mapped[int] = mapped_column(Integer)
    max_players: Mapped[int] = mapped_column(Integer)
    gamemode: Mapped[int] = mapped_column(Integer)
    ip: Mapped[str] = mapped_column(String(15))
    server_port: Mapped[int] = mapped_column(Integer)
    p2p_port: Mapped[int] = mapped_column(Integer)
    rpc_port: Mapped[int] = mapped_column(Integer)


class TrackmaniaMap:
    uid: str
    name: str