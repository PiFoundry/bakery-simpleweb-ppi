package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	var err error

	var config ppiConfig
	var configPath string
	flag.StringVar(&configPath, "C", "ppiConfig.json", "Path to ppi config file in JSON format.")

	configBytes, err := ioutil.ReadFile(configPath)
	if err != nil {
		log.Fatal(err)
	}

	err = json.Unmarshal(configBytes, &config)
	if err != nil {
		log.Fatal(err)
	}

	reqByte, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}

	var params ppiParams
	err = json.Unmarshal(reqByte, &params)
	if err != nil {
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
		log.Fatal(err.Error())
	}

	fmt.Printf("ok")
}
