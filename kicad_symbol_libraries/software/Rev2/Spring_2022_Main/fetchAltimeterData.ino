void fetchAltimeterData() {
  // Fetches altitude data
        //altitude = mpl.getAltitude(); // m
        altitude = sensor_read_data();
}
