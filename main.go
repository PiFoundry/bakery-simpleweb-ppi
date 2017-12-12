package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	logger := log.New(os.Stderr, "", 0)

	var config ppiConfig
	configBytes, err := ioutil.ReadFile("ppiConfig.json")
	if err != nil {
		logger.Println("Unable to load config.json")
		panic(err)
	}

	err = json.Unmarshal(configBytes, &config)
	if err != nil {
		logger.Println("Unable to parse config.json")
		panic(err)
	}

	reqByte, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		logger.Println("action json could not be read")
		panic(err)
	}

	var params ppiParams
	err = json.Unmarshal(reqByte, &params)
	if err != nil {
		logger.Println("action json could not be parsed")
		panic(err)
	}

	switch action := params.Action; action {
	case "poweron":
		config.PiList[params.PiId].powerOn()

	case "poweroff":
		config.PiList[params.PiId].powerOff()
	}

	fmt.Printf("ok")
}
