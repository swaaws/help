//Faster solution ->
//while true; do     sleep 60;     echo \`ip addr\` > /dev/udp/224.0.0.1/9999  ; done
package main

import (
	"log"
	"net"
	"time"
	"strings"
)

const (
	srvAddr         = "224.0.0.1:9999"
	maxDatagramSize = 8192
)

func main() {




	go ping(srvAddr)
	serveMulticastUDP(srvAddr, msgHandler)
}

func ping(a string) {
	addr, err := net.ResolveUDPAddr("udp", a)
	if err != nil {
		log.Fatal(err)
	}
	c, err := net.DialUDP("udp", nil, addr)
	for {
		as, err := getMacAddr()
		if err != nil {
			log.Fatal(err)
		}

		justString := strings.Join(as," ")

		c.Write([]byte(justString))
		time.Sleep(30 * time.Second)
	}
}

func msgHandler(src *net.UDPAddr, n int, b []byte) {
}

func serveMulticastUDP(a string, h func(*net.UDPAddr, int, []byte)) {
	for {
	}
}

func getMacAddr() ([]string, error) {
    ifas, err := net.Interfaces()
    if err != nil {
        return nil, err
    }
    var as []string
    for _, ifa := range ifas {
        a := ifa.HardwareAddr.String()
        if a != "" {
            as = append(as, a)
        }
    }
    return as, nil
}
