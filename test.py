import socket

HOST = 'localhost'
PORT = 5000

# connect
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostbyname(HOST), PORT))

# recieve and validate header the gbx header
## header length, 4 bytes int, big endian
data = s.recv(4)
headerLength = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)

## header data, bytes of length n=headerLength
data = s.recv(headerLength)
header = data.decode() # decode bytes to string

## the header should equal "GBXRemote 2"
if header != "GBXRemote 2":
    print('Invalid header.')
    exit(0)

print("Valid header!")

# we need to authenticate
print('Authenticating ...')
handler = 0x80000000 # handler starts at this value

## build the packet
### pack handler, 4 bytes int, big endian
handlerBytes = bytes([
    handler & 0xFF,
    (handler >> 8) & 0xFF,
    (handler >> 16) & 0xFF,
    (handler >> 24) & 0xFF])
### pack method call, xml structure
data = b'<?xml version="1.0"?>'
data += b'<methodCall>'
data += b'<methodName>Authenticate</methodName>' # call the Authenticate method
data += b'<params>'
data += b'<param><value>SuperAdmin</value></param>' # username: SuperAdmin
data += b'<param><value>yr4K5AxSGqkb6tEP928HRg</value></param>' # password: SuperAdmin
data += b'</params>'
data += b'</methodCall>'
### compile packet
packet = bytes()
#### packet length
packetLen = len(data)
packet += bytes([
    packetLen & 0xFF,
    (packetLen >> 8) & 0xFF,
    (packetLen >> 16) & 0xFF,
    (packetLen >> 24) & 0xFF
])
#### handler
packet += handlerBytes
#### data
packet += data

### Send the authentication call
print('sending packet: ' + str(packet))
s.send(packet)

## recieve authentication response
### recieve response header, 8 bytes
header = s.recv(8)
#### unpack response size, 4 bytes int, big endian
size = header[0] | (header[1] << 8) | (header[2] << 16) | (header[3] << 24)
#### unpack handler, 4 bytes int, big endian
responseHandler = header[4] | (header[5] << 8) | (header[6] << 16) | (header[7] << 24)
##### the response must have the same handler value
if responseHandler != handler:
    print('Response handler does not match!')
    exit(0)
#### recieve response data
response = s.recv(size)
print('got response: ' + str(response))