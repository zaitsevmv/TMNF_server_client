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
    map: Mapped[str] = mapped_column(String(255), nullable=True)
    max_players: Mapped[int] = mapped_column(Integer)
    gamemode: Mapped[int] = mapped_column(Integer, nullable=True)
    ip: Mapped[str] = mapped_column(String(15))
    port: Mapped[int] = mapped_column(Integer)