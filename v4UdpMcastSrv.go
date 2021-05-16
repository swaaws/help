package main

import (
	"log"
	"net"
	"reflect"
	"unsafe"
)

const (
	srvAddr         = "224.0.0.1:9999"
	maxDatagramSize = 8192
)

func main() {
	serveMulticastUDP(srvAddr, msgHandler)
}

func BytesToString(b []byte) string {
    bh := (*reflect.SliceHeader)(unsafe.Pointer(&b))
    sh := reflect.StringHeader{bh.Data, bh.Len}
    return *(*string)(unsafe.Pointer(&sh))
}

func msgHandler(src *net.UDPAddr, n int, b []byte) {
	str1 := BytesToString(b[:n])
	log.Println(str1, src)
}

func serveMulticastUDP(a string, h func(*net.UDPAddr, int, []byte)) {
	addr, err := net.ResolveUDPAddr("udp", a)
	if err != nil {
		log.Fatal(err)
	}
	l, err := net.ListenMulticastUDP("udp", nil, addr)
	l.SetReadBuffer(maxDatagramSize)
	for {
		b := make([]byte, maxDatagramSize)
		n, src, err := l.ReadFromUDP(b)
		if err != nil {
			log.Fatal("ReadFromUDP failed:", err)
		}
		h(src, n, b)
	}
}
