const net = require('net');
const Struct = require('struct');

class PacketInfo {
    constructor(code, struct) {
        this.code = code;
        this.struct = struct;
        this.struct.allocate();
    }
}

var packets = [];

function add_packet(name, pcode, pstruct) {
    packets[name] = new PacketInfo(pcode, pstruct);
}


add_packet("AUTH", 0, Struct().chars("key", 32)
);

add_packet("STEER", 1, Struct().word32Ule("mstate")
    .word32Ule("tstate")
);
add_packet("BEACON", 2, Struct().chars("text",4)

);
add_packet("ADJUST", 3, Struct().word8("lmotor")
    .word8("lmotor")
    .floatle("lpercent")
    .word16Ule("lmax")
    .word8("rmotor")
    .floatle("rpercent")
    .word16Ule("rmax")
);

//add more packets like this

class Networker {

    constructor(ip, port, auth_key) {
        this.ip = ip;
        this.port = port;
        this.auth_key = auth_key;
        this.client = net.Socket();

        this.connected = false;
        this.authed = false;

        this.response_string = "";
        //this.client.connect(port,ip,authenticate);
        this.wait_response = false;

        this.auth_success_handler=null;
        this.auth_failed_handler = null;
        this.disconnect_handler = null;

        this.client.on('connect', () => {
            this.connected = true;
            this.authenticate();
        });

        this.client.on('data', (data) => {
            if (this.wait_response) {

                console.log("DATA: "+data);

                let recv_str = data.toString();
                let newline_ix = recv_str.indexOf("\n");
                if (newline_ix != -1) {
                    recv_str = recv_str.substring(0, newline_ix);
                    this.wait_response = false;
                    //Do stuff with response
                    if (this.response_string + recv_str == "Auth Failed") {
                        console.log("Authentication Failure");
                        this.auth_failed_handler();
                    }
                    else {
                        console.log("Authenticated");

                        this.auth_success_handler();

                        this.authed = true;
                    }
                    this.response_string="";
                }
                else
                    this.response_string += recv_str;
            }
        });
        this.client.on('close', () => {

            this.connected = false;
            if (this.authed) {
                this.disconnect_handler();
                this.try_reconnect();
            }
            this.authed = false;
        });
    }
    try_reconnect() {
        if (!this.connected) {
            console.log("Trying to reconnect");
            this.start_con();
            setTimeout(()=>{this.try_reconnect()}, 1000);
        }
    }
    start_con() {
        this.client.connect(this.port, this.ip);
    }
    send(name, jb_rep) {
        let cur_packet = packets[name];
        let header = "TYPE" + cur_packet.code;

        for (let key of Object.keys(jb_rep))
            cur_packet.struct.set(key, jb_rep[key]);

        let buf = cur_packet.struct.buffer();

        this.client.write(header);
        this.client.write(buf);


    }
    authenticate() {
        this.send("AUTH", { key: this.auth_key });
        this.wait_response = true;
    }
}
