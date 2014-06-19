from optparse import OptionParser

def client(options):
    #create an INET, STREAMing socket
    s = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)
    #now connect to the web server on port 80
    # - the normal http port
    s.connect((options.ip, int(options.port)))

def server(options):
    #create an INET, STREAMing socket
    serversocket = socket.socket(
        socket.AF_INET, socket.SOCK_STREAM)
    #bind the socket to a public host,
    # and a well-known port
    serversocket.bind((socket.gethostname(), options.port))
    #become a server socket
    serversocket.listen(1)

    #accept connections from outside
    (clientsocket, address) = serversocket.accept()


def main():
    parser = OptionParser()
    parser.add_option("-l", "--listen", dest="listen",
                     help="Be a server",action="store_true", default=False)

    parser.add_option("-p", "--port",
                      dest="port", default=7331,
                     help="The port")

    parser.add_option("-i", "--ip",
                     dest="ip",
                     help="The ip to connect to")

    (options, args) = parser.parse_args()

    if options.listen:
        server(options)
    elif options.ip is not None:
        client(options)
    else:
        raise Exception("You probably didn't spsify the ip")

    

if '__main__' == __name__:
    main()
