import enum

from parser.parser import get_status
from .model import ServerStatus, GameMode, TrackmaniaMap


class ParseStatus(enum.Enum):
    SUCCESS = 1
    FAILTURE = 2
    OTHER = 1 << 31


class ServerConfig:
    ip = '127.0.0.1'
    server_port = 2000
    p2p_port = 3000
    rpc_port = 5000
    server_status = ServerStatus.STATUS_SLEEPING
    login = ''
    name = 'No name'
    current_players = 0
    max_players = 0
    current_map = TrackmaniaMap()
    current_gamemode = GameMode.MODE_TIMEATTACK

    status = ParseStatus.OTHER

    def __init__(self, path: str):
        name = "server_status.json"
        config = get_status(f'{path}/{name}')
        if config == None: 
            return

        self.login = config.get('login', self.login)
        self.name = config.get('name', self.name)
        self.ip = config.get('ip', self.ip)

        self.server_port = config.get('server_port', self.server_port)
        self.p2p_port = config.get('p2p_port', self.p2p_port)
        self.rpc_port = config.get('rpc_port', self.rpc_port)

        serv_status = config.get('servet_status', 'sleeping')
        if serv_status == 'sleeping':
            self.server_status = ServerStatus.STATUS_SLEEPING
        elif serv_status == 'rebooting':
            self.server_status = ServerStatus.STATUS_REBOOTING
        else:
            self.server_status = ServerStatus.STATUS_WORKING

        self.current_players = config.get('current_players', self.current_players)
        self.max_players = config.get('max_players', self.max_players)

        cur_gm = config.get('current_gamemode', 'other')
        if cur_gm == 'TimeAttack':
            self.current_gamemode = GameMode.MODE_TIMEATTACK
        elif cur_gm == 'Rounds':
            self.current_gamemode = GameMode.MODE_ROUNDS
        else:
            self.current_gamemode = GameMode.MODE_OTHER

        cur_map = config.get('current_map', None)
        if cur_map != None:
            self.current_map.uid = cur_map['uid']
            self.current_map.name = cur_map['name']
            self.status = ParseStatus.SUCCESS
            return
        
        self.status = ParseStatus.FAILTURE