import pathlib
import enum
import xml.etree.ElementTree as ET


class XmlListConfig(list):
    def __init__(self, aList):
        for element in aList:
            if element:
                # treat like dict
                if len(element) == 1 or element[0].tag != element[1].tag:
                    self.append(XmlDictConfig(element))
                # treat like list
                elif element[0].tag == element[1].tag:
                    self.append(XmlListConfig(element))
            elif element.text:
                text = element.text.strip()
                if text:
                    self.append(text)


class XmlDictConfig(dict):
    '''
    Example usage:

    >>> tree = ElementTree.parse('your_file.xml')
    >>> root = tree.getroot()
    >>> xmldict = XmlDictConfig(root)

    Or, if you want to use an XML string:

    >>> root = ElementTree.XML(xml_string)
    >>> xmldict = XmlDictConfig(root)

    And then use xmldict for what it is... a dict.
    '''
    def __init__(self, parent_element):
        if parent_element.items():
            self.update(dict(parent_element.items()))
        for element in parent_element:
            if element:
                # treat like dict - we assume that if the first two tags
                # in a series are different, then they are all different.
                if len(element) == 1 or element[0].tag != element[1].tag:
                    aDict = XmlDictConfig(element)
                # treat like list - we assume that if the first two tags
                # in a series are the same, then the rest are the same.
                else:
                    # here, we put the list in dictionary; the key is the
                    # tag name the list elements all share in common, and
                    # the value is the list itself 
                    aDict = {element[0].tag: XmlListConfig(element)}
                # if the tag has attributes, add those to the dict
                if element.items():
                    aDict.update(dict(element.items()))
                self.update({element.tag: aDict})
            # this assumes that if you've got an attribute in a tag,
            # you won't be having any text. This may or may not be a 
            # good idea -- time will tell. It works for the way we are
            # currently doing XML configuration files...
            elif element.items():
                self.update({element.tag: dict(element.items())})
            # finally, if there are no child tags and no attributes, extract
            # the text
            else:
                self.update({element.tag: element.text})


class ParseStatus(enum.Enum):
    SUCCESS = 1
    FAILTURE = 2
    OTHER = 1 << 31

class ServerConfig:
    ip = "0.0.0.0"
    port = 2000
    login = ""
    name = "No name"
    max_players = 0
    status = ParseStatus.OTHER

    def __init__(self, path: str):
        name = "dedicated_cfg.txt"
        result = sorted(pathlib.Path('.').glob(f'{path}/**/{name}'))
        if len(result) == 0:
            self.status = ParseStatus.FAILTURE
            return
        
        cfg_file = result[0]
        tree = ET.parse(cfg_file)
        root = tree.getroot()
        xmldict = XmlDictConfig(root)

        self.login = xmldict['masterserver_account']['login']
        self.name = self.name if xmldict['server_options']['name'] == None else xmldict['server_options']['name']

        self.ip = self.ip if xmldict['system_config']['force_ip_address'] == None else xmldict['system_config']['force_ip_address']
        self.port = int(xmldict['system_config']['server_port'])

        self.max_players = int(xmldict['server_options']['max_players'])

        if(self.login != None and self.port != None and self.max_players != None):
            self.status = ParseStatus.SUCCESS
            return
        
        self.status = ParseStatus.FAILTURE