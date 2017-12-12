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
	var err error

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

	pi, exists := config.PiList[params.PiId]
	if !exists {
		fmt.Printf("Pi with ID %v not in ppi ppiConfiguration.json", params.PiId)
		return
	}

	switch action := params.Action; action {
	case "poweron":
		err = pi.powerOn()

	case "poweroff":
		err = pi.powerOff()
	}

	if err != nil {
		logger.Fatal(err.Error())
	}

	fmt.Printf("ok")
}
