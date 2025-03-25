package main

import (
	"fmt"
	"log"

	hdisc "github.com/KennyZ69/HdiscLib"
)

func main() {
	fmt.Printf("Hello world!\n")
	devs, err := hdisc.ARPScan()
	if err != nil {
		log.Fatalf("Error scanning for devices on net: %s\n", err)
	}

	for _, dev := range devs {
		fmt.Printf("%-15s %-17s %-30s %-10s\n", dev.IP.String(), dev.Mac.String(), "", dev.Manuf)
	}
}
