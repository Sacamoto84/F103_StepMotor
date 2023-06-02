package main

import (
	_ "bufio"
	"fmt"
	"net"
	"os"
	"strings"
	"time"
)

func main() {
	// Получаем текущую дату
	now := time.Now()

	// Формируем имя файла в формате "2006-01-02.txt", где 2006 - это год, 01 - это месяц, 02 - это день
	filename := now.Format("2006-01-02") + ".txt"

	// Создаем новый UDP адрес для приема broadcast пакетов на порту 1234
	addr, err := net.ResolveUDPAddr("udp", "224.0.0.1:8888")
	if err != nil {
		fmt.Println("Error: ", err)
		return
	}

	// Создаем новый соединение UDP
	conn, err := net.ListenMulticastUDP("udp", nil, addr)
	if err != nil {
		fmt.Println("Error: ", err)
		return
	}

	// Создаем файл для записи данных с именем, содержащим дату
	file, err := os.Create(filename)
	if err != nil {
		fmt.Println("Error: ", err)
		return
	}
	defer file.Close()

	// Бесконечный цикл для приема и записи данных
	for {
		// Создаем буфер для получения данных
		buf := make([]byte, 1024)

		// Читаем данные из соединения
		_, remoteAddr, err := conn.ReadFromUDP(buf)
		if err != nil {
			fmt.Println("Error: ", err)
			continue
		}

		currentTime := time.Now().Format("15:04:05.000")

		newStr := strings.Replace(string(buf), "\x00", "", -1)

		newStr1 := fmt.Sprintf("%v Received packet from %v: %v", currentTime, remoteAddr, newStr)

		// Записываем данные в файл
		_, err = file.Write([]byte(newStr1))
		if err != nil {
			fmt.Println("Error: ", err)
			continue
		}

		fmt.Printf(newStr1)

	}
}
