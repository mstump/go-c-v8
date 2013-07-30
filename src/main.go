package main

import "gocv8"

func main() {
	eventJSON := `{"a_field":"TROLOLOL I'm an Event!!!"}`
	eventJS := `log(JSON.stringify(JSON.parse(event_data.data)))`

	badJS := `3\/1L H4X0R!!1!!`

    r := gocv8.NewContext()
	gocv8.ProcessEvent(r, eventJSON, eventJS)
	gocv8.ProcessEvent(r, eventJSON, badJS)
}