package main

type ppiConfig struct {
	PiList map[string]ppiNode `json:"piList"`
}

type ppiNode struct {
	PiId      string `json:"piId"`
	ID        int    `json:"id"`
	IpAddress string `json:"ipAddress"`
}

func (n ppiNode) powerOn() {
	//fmt.Printf("powerOn %v\n", n.IpAddress)
}

func (n ppiNode) powerOff() {
	//fmt.Printf("powerOff %v\n", n.IpAddress)
}
