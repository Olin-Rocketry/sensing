void fetchAccelerometerData() {

  // Fetches linear acceleration data (ignores gravity)
  imu::Vector<3> orientationData = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> accelerometerData = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  // Calculates the true acceleration in the upward direction


  //remap sensor ouput to correct cooridinate frame
  float x_acc=accelerometerData.z();
  float y_acc=accelerometerData.x() * -1;
  float z_acc=accelerometerData.y() * -1;

  //remap sensor ouput to correct cooridinate frame
  //z alwasy alights with north pole (you have to move the board when power up)
  //y alighns with grav
  //x based onright hand rule

  float x=orientationData.y() * -1; //alpha
  float y=orientationData.z() - 90 + 360; //beta
  float z=orientationData.x() * -1 + 360; //gamma

  

  
  float alpha=x*Pi/180 ;  //convert angle from deg to rad
  float beta=y*Pi/180;  //convert angle from deg to rad
  float gamma=z*Pi/180;



  float x_comp = (-1*sin(beta)) * x_acc;
  float y_comp = (cos(beta)*sin(alpha)) * y_acc;
  float z_comp = (cos(beta)*cos(alpha)) * z_acc;
  
  // Below equation comes from https://en.wikipedia.org/wiki/Rotation_matrix


  float z_global= (x_comp + y_comp + z_comp);
  


  
//  Serial.print("True Z Acceleration: ");
//  Serial.print(z_global);
//  Serial.println(" m/s/s");
  IMU[0]=x_acc;
  IMU[1]=y_acc;
  IMU[2]=z_acc;
  IMU[3]=x;
  IMU[4]=y;
  IMU[5]=z;
  IMU[6] = z_global;
  
}
