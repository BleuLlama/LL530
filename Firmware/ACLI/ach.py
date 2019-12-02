
import pprint
import json
import subprocess
import sys

def usage():
	print "Usage: ach <boardtype> <get>"
	print "Where:"
	print "		boardtype is Leonardo, or Uno, etc"
	print "		get is 'fqvn' or 'port'"
	sys.exit( -1 )

if len( sys.argv ) != 3:
	usage()

if sys.argv[2] != "fqvn" and sys.argv[2] != "port":
	usage()

lookFor = sys.argv[1]
request = sys.argv[2]


out = subprocess.Popen(['arduino-cli', 'board', 'list', '--format', 'json' ], 
           stdout=subprocess.PIPE, 
           stderr=subprocess.STDOUT)

stdout,stderr = out.communicate()

def ascii_encode_dict(data):
    ascii_encode = lambda x: x.encode('ascii') if isinstance(x, unicode) else x 
    return dict(map(ascii_encode, pair) for pair in data.items())


data = json.loads( stdout, object_hook=ascii_encode_dict )

pp = pprint.PrettyPrinter(indent=4)

#print "---------------------------------"
for i in data:
	if 'boards' in i:
		#/pp.pprint( i['boards'][0]['name'] )
		if lookFor in i['boards'][0]['name']:
			if( request == 'fqvn' ):
				print i['boards'][0]['FQBN']
			if( request == 'port' ):
				print i['address']
