package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

type ppiConfig struct {
	PiList map[string]ppiNode `json:"piList"`
}

type ppiNode struct {
	PiId      string `json:"piId"`
	ID        int    `json:"id"`
	IpAddress string `json:"ipAddress"`
}

func (n ppiNode) powerOn() error {
	client := &http.Client{
		Timeout: time.Second * 10,
	}

	resp, err := client.Get(fmt.Sprintf("http://%v/poweron?id=%v", n.IpAddress, n.ID))
	if err != nil {
		panic(err)
	}
	respBytes, _ := ioutil.ReadAll(resp.Body)
	if string(respBytes) != "ok" {
		return fmt.Errorf("response was not \"ok\"")
	}

	return nil
}

func (n ppiNode) powerOff() error {
	client := &http.Client{
		Timeout: time.Second * 10,
	}

	resp, err := client.Get(fmt.Sprintf("http://%v/poweroff?id=%v", n.IpAddress, n.ID))
	if err != nil {
		panic(err)
	}
	respBytes, _ := ioutil.ReadAll(resp.Body)
	if string(respBytes) != "ok" {
		return fmt.Errorf("response was not \"ok\"")
	}

	return nil
}
