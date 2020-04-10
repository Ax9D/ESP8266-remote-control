
let nw = new Networker("192.168.2.7", 8080, "24y4novrOt5rr1x!NdWkume2L7PtmLoF");

var key_table_t = {
    'a': false,
    'd': false
};
var key_table_m = {
    'w': false,
    's': false
}

var beacon_timer;

function key_down(event) {
    if(!event.repeat)
    {
        if (event.key == 'a' || event.key == 'd')
            key_table_t[event.key] = true;
        else if (event.key == 'w' || event.key == 's')
            key_table_m[event.key] = true;

        send_state();
    }
}
function send_beacon()
{
    nw.send("BEACON",{text:"SAME"});
}
function send_state() {


    if(nw.authed)
    {
        clearInterval(beacon_timer);
        beacon_timer=setInterval(send_beacon,500);

        let mstate_=0;
        let tstate_=0;
        
        if(key_table_m.w)
            mstate_=1;
        else if(key_table_m.s)
            mstate_=2;
        
        if(key_table_t.a)
            tstate_=1;
        else if(key_table_t.d)
            tstate_=2;

        nw.send("STEER",{
            mstate:mstate_,
            tstate:tstate_
        });
    }
}
function key_up(event) {
    if (event.key == 'a' || event.key == 'd')
        key_table_t[event.key] = false;
    else if (event.key == 'w' || event.key == 's')
        key_table_m[event.key] = false;

    send_state();
}
window.addEventListener("keydown", key_down, event);
window.addEventListener("keyup", key_up, event);



nw.auth_success_handler = () => {

};
nw.auth_failed_handler = () => {

};
nw.disconnect_handler = () => {

};

nw.start_con();
