void fetchRadio(char* buffer2){
  //sprintf(buffer, "Time: %lu\nPhase: %d\nLat: %f\nLong: %f\n",lastCallTime, phase, GPSArray[0],GPSArray[1]);
  // Send a message to rf95_server
  rf95.send((uint8_t *)buffer2, 70);
  delay(10);
  rf95.waitPacketSent();
}
