package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

type ppiConfig struct {
	PiList map[string]ppiNode `json:"piList"`
}

type ppiNode struct {
	PiId string `json:"piId"`
	ID   int    `json:"id"`
	URL  string `json:"url"`
}

func (n ppiNode) powerOn() error {
	client := &http.Client{
		Timeout: time.Second * 10,
	}

	resp, err := client.Get(fmt.Sprintf("%v/poweron/%v", n.URL, n.ID))
	if err != nil {
		log.Fatal(err)
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

	resp, err := client.Get(fmt.Sprintf("%v/poweroff/%v", n.URL, n.ID))
	if err != nil {
		log.Fatal(err)
	}

	respBytes, _ := ioutil.ReadAll(resp.Body)
	if string(respBytes) != "ok" {
		return fmt.Errorf("response was not \"ok\"")
	}

	return nil
}
